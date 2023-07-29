#include "../../include/Importer/TextImporter.h"
#include "../../include/Config/Config.h"
#include "../../include/Importer/Entry.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>


using namespace Maconomy;


// Convert a time string (hhmm) to hours.
double toDoubleHours(const std::string& time) {
    const double hours = std::stod(time.substr(0, 2));
    const double minutes = std::stod(time.substr(2));

    return hours + minutes / 60.0;
}


// Is valid time interval?
bool isTimeInterval(const std::string& arg) {
    int count{};
    for (char c : arg) if (isdigit(c)) ++count;
    return (count == 8);
}


// Calculate a time interval.
double calculateTimeInterval(const std::string& arg) {
    std::string tmp;
    for (char c : arg) if (isdigit(c)) tmp += c;

    // Sanitiy check.
    if (tmp.size() != 8) return -1;

    const double a = toDoubleHours(tmp.substr(0, 4));
    const double b = toDoubleHours(tmp.substr(4));

    return b - a;
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


// Time line key.
std::string TextImporter::TimeLine::key() const {
    return processed ? std::to_string(row) : entry.key();
}


// Import time log.
void TextImporter::import() {
    std::ifstream file;
    file.open(_config->timeLogPath);
    if (!file) {
        std::cerr << "Unable to open time log file: " << _config->timeLogPath << std::endl;
        return;
    }

    _timeLines.clear();

    int weekday{ -1 };
    std::string line;
    for (int row{}; std::getline(file, line); ++row) {
        struct TimeLine timeLine;

        if (line.empty()) {
            timeLine.row = row;
            timeLine.processed = true;
        }

        std::istringstream ss(line);
        std::string arg;
        for (int column{}; std::getline(ss, arg, '\t'); ++column) {
            timeLine.raw.push_back(arg);

            const int day = dayToNumber(arg);
            if (day != -1) {
                weekday = day;
                timeLine.processed = true;
                break;
            }

            switch (column) {
                case 0: { // Description.
                    timeLine.entry.description = arg;
                    continue;
                }
                case 1: { // Jobs.
                    std::istringstream tmpss(arg);
                    std::string tmp;
                    while (std::getline(tmpss, tmp, ' ')) {
                        timeLine.entry.jobNumber.push_back(tmp);
                    }
                    continue;
                }
                case 2: { // Task name.
                    timeLine.entry.taskName = arg;
                    continue;
                }
                default: { break; /* continue evalutation */ }
            }

            if (isTimeInterval(arg) && weekday != -1) {
                timeLine.entry.times[weekday] += calculateTimeInterval(arg);
            } else if (isSpec3(arg)) {
                timeLine.entry.spec3 = arg;
            } else if (isProcessed(arg)) {
                timeLine.processed = true;
            } else {
                timeLine.entry.valid = false;
            }
        }

        // Set default Spec3 if none was set.
        if (timeLine.entry.spec3.empty()) {
            timeLine.entry.spec3 = _config->spec3();
        }

        insertTimeLine(_timeLines, timeLine);
    }

    file.close();

    splitEntries();
    setJobAndTask();
}


// Get Maconomy entries.
std::vector<Entry*> TextImporter::getEntries() {
    std::vector<Entry*> entries;

    for (auto it = _timeLines.begin(); it != _timeLines.end(); ++it) {
        struct TimeLine& current = it->second;
        if (!current.processed && current.entry.valid) {
            entries.push_back(&current.entry);
        }
    }

    return entries;
}


// Write updated data to time log.
void TextImporter::writeToTimeLog() const {
    writeToFile(_config->timeLogPath, true);
}


// Write to log file.
void TextImporter::writeToLog() const {
    writeToFile(_config->logPath, false);
}


// Insert time line into map.
void TextImporter::insertTimeLine(std::unordered_map<std::string, struct TimeLine>& map,
                                  struct TimeLine timeLine) {
    auto it = map.find(timeLine.key());
    if (it == map.end()) {
        map[timeLine.key()] = timeLine;
        return;
    }
    it->second.entry.merge(timeLine.entry);
}


// Split the entries.
void TextImporter::splitEntries() {
    std::vector<std::string> splitKeys{};
    for (auto it = _timeLines.begin(); it != _timeLines.end(); ++it) {
        struct TimeLine& current = it->second;
        if (!current.processed && current.entry.canSplit()) {
            splitKeys.push_back(it->first);
        }
    }

    for (const auto& key : splitKeys) {
        auto it = _timeLines.find(key);
        if (it == _timeLines.end()) {
            std::cerr << "splitEntries() - Key cannot be found, key: " << key << std::endl;
            continue;
        }

        struct TimeLine current = it->second;
        struct TimeLine newLine;
        newLine.entry = current.entry.split();
        newLine.processed = current.processed;

        // Remove current from map since the key has changed after split.
        _timeLines.erase(key);

        insertTimeLine(_timeLines, current);
        insertTimeLine(_timeLines, newLine);
    }
}


// Set Maconomy job number and task name.
void TextImporter::setJobAndTask() {
    std::vector<std::string> keys{};
    for (auto it = _timeLines.begin(); it != _timeLines.end(); ++it) {
        keys.push_back(it->first);
    }

    for (const auto& key : keys) {
        auto it = _timeLines.find(key);
        if (it == _timeLines.end()) {
            std::cerr << "setJobAndTask() - Key cannot be found, key: " << key << std::endl;
            continue;
        }
        struct TimeLine current = it->second;
        if (current.processed) continue;

        const std::string jobTaskKey{
            current.entry.jobNumber.back() + ";" + current.entry.taskName };


        bool changed{ false };
        for (auto& job : current.entry.jobNumber) {
            const std::string jobNumber = _config->getJob(jobTaskKey);
            if (jobNumber.empty()) {
                current.entry.valid = false;
            } else {
                job = jobNumber;
                changed = true;
            }
        }

        const std::string taskName = _config->getTask(jobTaskKey);
        if (taskName.empty()) {
            current.entry.valid = false;
        } else {
            current.entry.taskName = taskName;
            changed = true;
        }

        if (changed) {
            _timeLines.erase(key);
            insertTimeLine(_timeLines, current);
        }
    }
}


// Sort time lines and return as vector.
std::vector<const struct TextImporter::TimeLine*> TextImporter::sortedTimeLines() const {
    typedef const struct TimeLine* TimeLinePtr;

    std::vector<TimeLinePtr> res{};
    for (auto it = _timeLines.cbegin(); it != _timeLines.cend(); ++it) {
        res.push_back(&(it->second));
    }

    std::sort(res.begin(),
              res.end(),
              [](TimeLinePtr lhs, TimeLinePtr rhs) {
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
    
    for (const struct TimeLine* timeLine : sortedTimeLines()) {
        if (!toTimeLog && timeLine->raw.empty()) continue;
        if (!toTimeLog && (timeLine->processed || timeLine->entry.valid)) continue;

        const size_t count = timeLine->raw.size();
        for (size_t i{}; i < count; ++i) {
            out << timeLine->raw[i];
            if (i != (count - 1)) out << '\t';
            else if (toTimeLog &&
                     !timeLine->processed &&
                     timeLine->entry.valid) out << '\t' << 'x';
        }
        out << '\n';
    }

    out.close();
}