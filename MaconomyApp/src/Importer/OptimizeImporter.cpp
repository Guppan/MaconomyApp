#include "../../include/Importer/OptimizeImporter.h"
#include "../../include/Config/Config.h"
#include "../../include/Misc/Constants.h"

#include <fstream>
#include <iostream>
#include <sstream>


using namespace Maconomy;


namespace {

	// Extract substring given a start and end position.
	std::string substring(const std::string & line,
						  const std::string & from,
						  const std::string & to) {
		std::size_t start = line.find(from);
		std::size_t end = line.find(to);
		if (start == std::string::npos || end == std::string::npos) return std::string{};

		start += from.size();
		return line.substr(start, (end - start));
	}


	// Is the given line a total line?
	bool isTotalLine(const std::string& line) {
		const std::string data = substring(line,
										   OPTIMIZE_TASK_START,
										   OPTIMIZE_TASK_END);
		return data == OPTIMIZE_TOTAL;
	}


	// Is the given line a task line?
	bool isTaskLine(const std::string& line) {
		const std::string data = substring(line,
										   OPTIMIZE_TASK_START,
										   OPTIMIZE_TASK_END);
		return !data.empty() && data != OPTIMIZE_TOTAL;
	}


	// Is the given string considered empty?
	bool isEmptyString(const std::string& str) {
		for (const char c : str) if (!isspace(c)) return false;
		return true;
	}
}


// Import the time log.
void OptimizeImporter::import() {
	std::ifstream file;
	file.open(_config->timeLogPath);
	if (!file) {
		std::cerr << "Unable to open time log file: " << _config->timeLogPath << std::endl;
		return;
	}

	_entries.clear();

	// Goto start of time entries.
	file.seekg(std::ios::beg);
	for (int i{}; i < OPTIMIZE_START - 1; ++i) {
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	Entry::ptr entry;
	std::string line;
	while (std::getline(file, line)) {
		if (isEmptyString(line)) continue;
		if (isTotalLine(line)) break;

		if (isTaskLine(line)) {
			entry = std::make_unique<Entry>();
			
			if (!setData(line, entry.get())) { entry->valid = false; continue; }
			if (!setTimes(file, entry.get())) { entry->valid = false; continue; }
			entry->spec3 = _config->spec3();

			insertEntry(std::move(entry));
		}
	}

	file.close();
}


// Convert a time string (hh:mm) to hours.
double OptimizeImporter::toHours(const std::string& time) const {
	if (time == OPTIMIZE_TIME_EMPTY) return 0;
	else if (time.size() < 4 || time.size() > 5) return -1;

	return Importer::toHours(time);
}


// Set data (job, task, description) on the entry. Returns true on success.
bool OptimizeImporter::setData(const std::string& line,
							   Entry* entry) const {
	const std::string data = substring(line,
									   OPTIMIZE_TASK_START,
									   OPTIMIZE_TASK_END);
	if (data.empty()) return false;

	std::istringstream ss(data);
	std::vector<std::string> args{};
	std::string cur;
	while (std::getline(ss, cur, OPTIMIZE_TASK_DESC_DELIMITER)) {
		args.push_back(cur);
	}

	if (args.size() != 2) {
		std::cerr << "setData() - ill formated input: " << data << std::endl;
		return false;
	}

	entry->description = args.front();

	ss.clear();
	ss.str(args.back());
	for (int i{}; ss >> cur;) {
		switch (i++) {
			case 0: {
				std::istringstream ssJob(cur);
				std::string job;
				while (std::getline(ssJob, job, OPTIMIZE_JOB_DELIMITER)) {
					entry->jobNumber.push_back(job);
				}
				break;
			}
			case 1: { entry->taskName = cur; break; }
			case 2: { entry->spec3 = cur; break; }
			default: {
				std::cerr << "setData() - unknown argument: " << cur << std::endl;
				break;
			}
		}
	}

	return validateEntry(entry);
}


// Set times on the entry. Returns true on success.
bool OptimizeImporter::setTimes(std::ifstream& file,
								Entry* entry) const {
	std::string line;
	for (int i{}; i < 5; ++i) {
		if (!std::getline(file, line)) return false;
		
		const std::string data = substring(line,
										   OPTIMIZE_TIME_START,
										   OPTIMIZE_TIME_END);
		
		if (data.empty()) return false;
		entry->times[i] += toHours(data);
	}

	return true;
}


// Validate an entry.
bool OptimizeImporter::validateEntry(const Entry* entry) const {
	if (!entry) return false;

	if (entry->description.empty()) return false;
	if (entry->jobNumber.empty()) return false;
	if (entry->taskName.empty()) return false;
	return true;
}