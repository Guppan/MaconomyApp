#include "../../include/Importer/TogglImporter.h"
#include "../../include/Config/Config.h"
#include "../../include/Misc/Constants.h"
#include "../../include/Remote/json.hpp"

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>


using namespace Maconomy;
using json = nlohmann::json;


namespace {
	
	// Compiler dependent implementation of localtime.
	std::tm localtime_safe(std::time_t time) {
		std::tm tm{};

		#ifdef _MSC_VER
		localtime_s(&tm, &time);
		#else
		tm = *std::localtime(&time);
		#endif // _MSC_VER

		return tm;
	}

	// Get the weekday number from a date.
	int weekday(const std::string& date) {
		std::tm tm{};
		std::istringstream ss(date);
		std::string tmp;
		for (int i{}; std::getline(ss, tmp, '-'); ++i) {
			switch (i) {
				case 0: { tm.tm_year = std::stoi(tmp) - 1900; break; }
				case 1: { tm.tm_mon = std::stoi(tmp) - 1; break; }
				case 2: { tm.tm_mday = std::stoi(tmp); break; }
				default: {
					std::cerr << "weekday(..) - Ill formed date string" << std::endl;
					return -1;
				}
			}
		}

		std::time_t time = std::mktime(&tm);
		std::tm res = localtime_safe(time);
 
		// Monday is index 0 in Maconomy but index 1 from std::tm.
		const int day{ (res.tm_wday - 1) % 7 };
		return day < 0 ? day + 7 : day;
	}

}


// Import the time log.
void TogglImporter::import() {
	std::ifstream file;
	file.open(_config->timeLogPath);
	if (!file) {
		std::cerr << "Unable to open time log file: " << _config->timeLogPath << std::endl;
		return;
	}

	_entries.clear();

	json parsed = json::parse(file);
	for (auto& elem : parsed[TOGGL_JSON_ARRAY]) {
		Entry::ptr entry = std::make_unique<Entry>();

		entry->description = elem[TOGGL_JSON_DESCRIPTION];
		entry->taskName = elem[TOGGL_JSON_CLIENT];

		std::istringstream ss;
		ss.str(elem[TOGGL_JSON_PROJECT]);
		std::string job;
		while (std::getline(ss, job, TOGGL_JOB_DELIMITER)) {
			entry->jobNumber.push_back(job);
		}

		for (auto& tag : elem[TOGGL_JSON_TAG]) {
			entry->spec3 = tag;
			break;
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
}