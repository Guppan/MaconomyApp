#include "../../include/Importer/TogglImporter.h"
#include "../../include/Config/Config.h"
#include "../../include/Importer/ImporterConstants.h"
#include "../../include/Remote/json.hpp"

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>


using namespace Maconomy;
using json = nlohmann::json;


// Get the weekday number from a date.
int weekday(const std::string& date) {
	std::tm tm{};

	std::istringstream ss(date);
	std::string tmp;
	for (int i{}; std::getline(ss, tmp, '-'); ++i) {
		switch (i) {
			case 0: { tm.tm_year = std::stoi(tmp) - 1900; break; }
			case 1: { tm.tm_mday = std::stoi(tmp) - 1; break; }
			case 2: { tm.tm_mday = std::stoi(tmp); break; }
			default: {
				std::cerr << "weekday(..) - Ill formed date string" << std::endl;
				return -1;
			}
		}
	}

	return tm.tm_wday;
}


// Converts a time (hh:mm:ss) to hours.
double toHours(const std::string& time) {
	const double hours = std::stod(time.substr(0, 2));
	const double minutes = std::stod(time.substr(3, 2));
	const double seconds = std::stod(time.substr(4));

	return hours + minutes / 60.0 + seconds / 3600.0;
}


// Import the time log.
void TogglImporter::import() {
	std::ifstream file;
	file.open(_config->timeLogPath);
	if (!file) {
		std::cerr << "Unable to open time log file: " << _config->timeLogPath << std::endl;
		return;
	}

	json parsed = json::parse(file);
	for (auto& elem : parsed[TOGGL_JSON_ARRAY]) {
		TogglEntry::ptr entry = std::make_unique<TogglEntry>();
		
		entry->description = elem[TOGGL_JSON_DESCRIPTION];
		entry->jobNumber.push_back(elem[TOGGL_JSON_PROJECT]);
		entry->taskName = elem[TOGGL_JSON_CLIENT];
		
		for (auto& tag : elem[TOGGL_JSON_TAG]) {
			entry->jobNumber.push_back(tag);
		}

		const int day = weekday(elem[TOGGL_JSON_DATE]);
		const double time = toHours(elem[TOGGL_JSON_DURATION]);
		entry->times[day] = time;

		// Set default Spec3 if none was set.
		if (entry->spec3.empty()) {
			entry->spec3 = _config->spec3();
		}

		insertEntry(std::move(entry));
	}

	file.close();

	splitEntries();
	setJobAndTask();
}