#include "../../include/Importer/TextImporter.h"
#include "../../include/Config/Config.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>


using namespace Maconomy;


namespace {

    // Is valid time interval?
    bool isTimeInterval(const std::string& arg) {
        int count{};
        for (char c : arg) if (isdigit(c)) ++count;
        return (count == 8);
    }


    // Convert day to day number.
    int dayToNumber(const std::string& day) {
        static const std::string weekdays[] = {
            "mandag", "tisdag", "onsdag", "torsdag", "fredag"
        };

        std::string lowerDay;
        for (const char c : day) lowerDay += std::tolower(c);

        for (int i{}; i < 5; ++i) {
            if (weekdays[i] == lowerDay) return i;
        }
        return -1;
    }


    // Is a Spec3 argument?
    bool isSpec3(const std::string& arg) {
        if (arg.empty()) return false;
        for (const char c : arg) {
            if (!isalpha(c)) return false;
        }
        return true;
    }

}


// Import time log.
void TextImporter::import() {
    std::ifstream file;
    file.open(_config->timeLogPath);
    if (!file) {
        std::cerr << "Unable to open time log file: " << _config->timeLogPath << std::endl;
        return;
    }

    _entries.clear();

    int weekday{ -1 };
    std::string line;
    for (int row{}; std::getline(file, line); ++row) {
        if (line.empty()) continue;

        Entry::ptr entry;

        std::istringstream ss(line);
        std::string arg;
        for (int column{}; std::getline(ss, arg, '\t'); ++column) {

            const int day = dayToNumber(arg);
            if (day != -1) {
                weekday = day;
                break;
            }

            if (!entry) entry = std::make_unique<Entry>();
            entry->raw.push_back(arg);

            switch (column) {
                case 0: { // Description.
                    entry->description = arg;
                    continue;
                }
                case 1: { // Jobs.
                    std::istringstream tmpss(arg);
                    std::string tmp;
                    while (std::getline(tmpss, tmp, ' ')) {
                        entry->jobNumber.push_back(tmp);
                    }
                    continue;
                }
                case 2: { // Task name.
                    entry->taskName = arg;
                    continue;
                }
                default: { break; /* continue evalutation */ }
            }

            if (isTimeInterval(arg) && weekday != -1) {
                entry->times[weekday] += toHours(arg);
            } else if (isSpec3(arg)) {
                entry->spec3 = arg;
            } else {
                entry->valid = false;
            }
        }

        if (!entry) continue;

        // Set default Spec3 if none was set.
        if (entry->spec3.empty()) {
            entry->spec3 = _config->spec3();
        }

        insertEntry(std::move(entry));
    }

    file.close();
}


// Convert a time string (hh:mm - hh:mm) to hours.
double TextImporter::toHours(const std::string& time) const {
    std::string tmp;
    for (char c : time) if (isdigit(c) || c == ':') tmp += c;

    // Sanitiy check.
    if (tmp.size() != 10) return -1;

    const double a = Importer::toHours(tmp.substr(0, 5));
    const double b = Importer::toHours(tmp.substr(5));

    return b - a;
}