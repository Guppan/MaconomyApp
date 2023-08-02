#include "../../include/Importer/Importer.h"
#include "../../include/Importer/Entry.h"
#include "../../include/Config/Config.h"

#include <iostream>


using namespace Maconomy;


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


// Split entries.
void Importer::splitEntries() {
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
		Entry::ptr newEntry = current->split();

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