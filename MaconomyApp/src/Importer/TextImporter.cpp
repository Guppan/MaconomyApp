#include "../../include/Importer/TextImporter.h"
#include "../../include/Config/Config.h"
#include "../../include/Importer/TextEntry.h"

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


    // Represent a processed line?
    bool isProcessed(const std::string& arg) {
        return (arg == "x" || arg == "X");
    }


    // Is a Spec3 argument?
    bool isSpec3(const std::string& arg) {
        if (arg.empty() || isProcessed(arg)) return false;
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
        TextEntry::ptr entry = std::make_unique<TextEntry>();

        if (line.empty()) {
            entry->row = row;
            entry->processed = true;
        }

        std::istringstream ss(line);
        std::string arg;
        for (int column{}; std::getline(ss, arg, '\t'); ++column) {
            entry->raw.push_back(arg);

            const int day = dayToNumber(arg);
            if (day != -1) {
                weekday = day;
                entry->processed = true;
                break;
            }

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
            } else if (isProcessed(arg)) {
                entry->processed = true;
            } else {
                entry->valid = false;
            }
        }

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


// Write updated data to time log.
void TextImporter::writeToTimeLog() const {
    writeToFile(_config->timeLogPath, true);
}


// Write to log file.
void TextImporter::writeToLog() const {
    writeToFile(_config->logPath, false);
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


// Sort time lines and return as vector.
std::vector<const TextEntry*> TextImporter::sortedTimeLines() const {
    std::vector<const TextEntry*> res{};
    for (auto it = _entries.cbegin(); it != _entries.cend(); ++it) {
        res.push_back(dynamic_cast<TextEntry*>(it->second.get()));
    }

    std::sort(res.begin(),
              res.end(),
              [](const TextEntry* lhs, const TextEntry* rhs) {
                  return lhs->row > rhs->row;
              });

    return res;
}


// Write to file.
void TextImporter::writeToFile(const std::string& path,
                               bool toTimeLog) const {
    std::ofstream out;
    out.open(path, std::ios::trunc);
    if (!out) {
        std::cerr << "Unable to open file with path: " << path << std::endl;
        return;
    }
    
    for (const TextEntry* entry : sortedTimeLines()) {
        if (!toTimeLog && entry->raw.empty()) continue;
        if (!toTimeLog && (entry->processed || entry->valid)) continue;

        const size_t count = entry->raw.size();
        for (size_t i{}; i < count; ++i) {
            out << entry->raw[i];
            if (i != (count - 1)) out << '\t';
            else if (toTimeLog &&
                     !entry->processed &&
                     entry->valid) out << '\t' << 'x';
        }
        out << '\n';
    }

    out.close();
}