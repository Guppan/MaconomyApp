#include "../../include/Importer/Importer.h"
#include "../../include/Importer/TextImporter.h"
#include "../../include/Importer/TogglImporter.h"
#include "../../include/Importer/OptimizeImporter.h"
#include "../../include/Importer/Entry.h"
#include "../../include/Importer/RoundStrategy.h"
#include "../../include/Importer/SplitFunctions.h"
#include "../../include/Config/Config.h"
#include "../../include/Misc/Constants.h"


#include <fstream>
#include <iostream>
#include <sstream>


using namespace Maconomy;


// Importer factory.
Importer::ptr Maconomy::importerFactory(Config* config) {
	Importer::ptr ptr = nullptr;

	if (config) {
		const std::string& mode = config->importerMode();

		if (mode == IMPORTER_MODE_TEXT) {
			ptr = std::make_unique<TextImporter>(config);
		} else if (mode == IMPORTER_MODE_TOGGL) {
			ptr = std::make_unique<TogglImporter>(config);
		} else if (mode == IMPORTER_MODE_OPTIMIZE) {
			ptr = std::make_unique<OptimizeImporter>(config);
		}
	}

	return std::move(ptr);
}


// Constructor.
Importer::Importer(Config* config)
	: _config{ config } {}


// Run the importer.
void Importer::run() {
	import();
	executeRoundingStrategy();
	splitEntries();
	setJobAndTask();
}


// Get Maconomy entries.
std::vector<Entry*> Importer::getEntries() {
	std::vector<Entry*> res{};

	for (auto it = _entries.begin(); it != _entries.end(); ++it) {
		if (it->second->isValid()) {
			res.push_back(it->second.get());
		}
	}

	return res;
}


// Write to log file.
void Importer::writeToLog() const {
	std::ofstream out;
	out.open(_config->logPath, std::ios::trunc);
	if (!out) {
		std::cerr << "writeToLog() - Unable to open log file with path: " << _config->logPath << std::endl;
		return;
	}

	// Get invalid entries.
	std::vector<const Entry*> invalids{};
	for (auto it = _entries.cbegin(); it != _entries.cend(); ++it) {
		if (!it->second->isValid()) {
			invalids.push_back(it->second.get());
		}
	}

	// Output invalids to file.
	out << "----- Failed to upload following entries. Please add them manually to Maconomy -----\n\n";
	for (const Entry* entry : invalids) {
		out << "Description: " << entry->description << '\n';
		out << "Job name(s): ";
		for (const auto& job : entry->jobNumber) out << job << ' ';
		out << "\nTask name:   " << entry->taskName << '\n\n';
	}

	out.close();
}


// Convert a time string (hh:mm:ss) to hours.
double Importer::toHours(const std::string& time) const {
	static const std::vector<double> divs{ 1.0, 60.0, 3600.0 };

	double res{};
	const std::size_t size = time.size();
	std::string tmp{};

	for (std::size_t i{}, divsIdx{}; (i < size && divsIdx < divs.size()); ++i) {
		const bool end{ i == size - 1 };
		const char c = time[i];

		if (!isdigit(c) || end) {
			if (isdigit(c) && end) tmp.push_back(c);
			res += std::stod(tmp) / divs[divsIdx++];
			tmp.clear();
			continue;
		}
		tmp.push_back(c);
	}

	return res;
}


// Insert an entry into the entries.
void Importer::insertEntry(Entry::ptr entry) {
	if (!entry) return;

	const std::string key = entry->key();
	auto it = _entries.find(key);
	if (it == _entries.end()) {
		_entries[key] = std::move(entry);
		return;
	}
	it->second->merge(entry.get());
}


// Execute rounding strategy.
void Importer::executeRoundingStrategy() {
	// Get the rounding function. 
	roundFn strategy = roundStrategyFn(_config->roundStrategy());
	if (!strategy) return;

	// Create a time entry matrix and index mapping.
	Matrix2D matrix{};
	std::unordered_map<std::string, size_t> indexMap{};

	size_t index{};
	for (auto it = _entries.cbegin(); it != _entries.cend(); ++it) {
		if (!it->second->isValid()) continue;

		matrix.push_back(it->second->times);
		indexMap[it->first] = index++;
	}

	// Replace entry times with the rounded times.
	matrix = strategy(matrix);
	for (auto it = indexMap.cbegin(); it != indexMap.cend(); ++it) {
		auto it2 = _entries.find(it->first);
		if (it2 == _entries.end()) continue;

		size_t index = it->second;
		if (index < 0 || index >= matrix.size()) continue;

		Entry* entry = it2->second.get();
		entry->times = matrix[index];
	}
}


// Split entries.
void Importer::splitEntries() {
	// Get the split function.
	splitFn strategy = splitStrategyFn(_config->splitStrategy());
	if (!strategy) return;

	std::vector<std::string> keys{};
	for (auto it = _entries.cbegin(); it != _entries.cend(); ++it) {
		if (it->second->canSplit()) keys.push_back(it->first);
	}

	for (const auto& key : keys) {
		auto it = _entries.find(key);
		if (it == _entries.end()) {
			std::cerr << "splitEntries() - Key cannot be found, key: " << key << std::endl;
			continue;
		}

		Entry::ptr current = std::move(it->second);
		Entry::ptr newEntry = current->split(strategy);

		// Remove current from entries since the key has changed after split.
		_entries.erase(key);

		insertEntry(std::move(current));
		insertEntry(std::move(newEntry));
	}
}


// Set Maconomy job and task.
void Importer::setJobAndTask() {
	std::vector<std::string> keys{};
	for (auto it = _entries.cbegin(); it != _entries.cend(); ++it) {
		keys.push_back(it->first);
	}

	for (const auto& key : keys) {
		auto it = _entries.find(key);
		if (it == _entries.end()) {
			std::cerr << "setJobAndTask() - Key cannot be found, key: " << key << std::endl;
			continue;
		}

		Entry* current = it->second.get();
		if (!current->isValid()) continue;

		const std::string jobTaskKey{
			current->jobNumber.back() + ";" + current->taskName };

		bool changed{ true };
		for (auto& job : current->jobNumber) {
			const std::string jobNumber = _config->getJob(jobTaskKey);
			if (jobNumber.empty()) {
				current->valid = false;
				changed = false;
				break;
			}
			job = jobNumber;
		}

		const std::string taskName = _config->getTask(jobTaskKey);
		if (taskName.empty()) {
			current->valid = false;
			continue;
		}
		current->taskName = taskName;

		if (changed) {
			Entry::ptr entry = std::move(it->second);

			_entries.erase(key);
			insertEntry(std::move(entry));
		}
	}
}
