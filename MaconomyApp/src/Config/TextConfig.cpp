#include "../../include/Config/TextConfig.h"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>


using namespace Maconomy;

// =========== Definitions ======================================= //

// Defines start of different definitions.
enum Definition {
	FILEPATHS = 0, // Start of file path definitions.
	DEFAULTS, // Start of default definitions.
	DEFINITIONS, // Start of job/task definitions.
	ERROR // Representing an error.
};


// String to enum map between the definitions.
const std::map<std::string, Definition> _definitionMap = {
	{"FILEPATHS", Definition::FILEPATHS},
	{"DEFAULTS", Definition::DEFAULTS},
	{"DEFINITIONS", Definition::DEFINITIONS}
};


// Get a definition enum. Returns ERROR if not found.
Definition definition(const std::string& key) {
	const auto it = _definitionMap.find(key);
	if (it != _definitionMap.cend()) return it->second;
	return Definition::ERROR;
}

// ============================================================== //


// Import config.
bool TextConfig::import(const std::string& path) {
	std::ifstream file;
	file.open(path);
	if (!file) {
		std::cerr << "Couldn't open text config file: " << path << std::endl;
		return false;
	}

	std::string line;
	std::string localJob;
	std::string maconomyJob;
	int pathRow{};

	bool valid{ true };
	Definition current = Definition::ERROR;
	for (int row{}; std::getline(file, line); ++row) {
		// Ignore empty lines and comments.
		if (line.empty()) continue;
		if (line.front() == '#') continue;

		const Definition def = definition(line);
		if (def != Definition::ERROR) {
			current = def;
			pathRow = 0;
			continue;
		}

		switch (current) {
			case Definition::FILEPATHS: {
				valid = importFilePaths(line, row, pathRow);
				break;
			}
			case Definition::DEFAULTS: {
				valid = importDefaults(line, row, pathRow);
				break;
			}
			case Definition::DEFINITIONS: {
				valid = importDefinitions(line, row, localJob, maconomyJob);
				break;
			}
			case Definition::ERROR: {
				std::cerr << "No definition set, row: " << row << std::endl;
				valid = false;
				break;
			}
			default: {
				std::cerr << "Invalid. Row: " << row << ", current: " << current << std::endl;
				valid = false;
				break;
			}
		}

		if (!valid) break;
	}

	file.close();
	return valid;
}


// Import file paths.
bool TextConfig::importFilePaths(const std::string& line,
								 const int row,
								 int& pathRow) {
	switch (pathRow++) {
		case 0: { timeLogPath = line; break; }
		case 1: { logPath = line; break; }
		default: {
			std::cerr << "Unknown file path on row: " << row << std::endl;
			return false;
			break;
		}
	}
	return true;
}


// Import defaults.
bool TextConfig::importDefaults(const std::string& line,
								const int row,
								int& pathRow) {
	switch (pathRow++) {
		case 0: { _spec3 = line; break; }
		default: {
			std::cerr << "Unknown default on row: " << row << std::endl;
			return false;
			break;
		}
	}
	return true;
}


// Import definitions.
bool TextConfig::importDefinitions(const std::string& line,
								   const int row,
								   std::string& localJob,
								   std::string& maconomyJob) {
	std::istringstream ss(line);
	std::string arg;

	std::string localTask;
	std::string maconomyTask;
	for (int column{}; std::getline(ss, arg, '\t'); ++column) {
		if (arg.empty()) continue;

		switch (column) {
			case 0: { localJob = arg; break; }
			case 1: { localTask = arg; break; }
			case 2: { maconomyJob = arg; break; }
			case 3: { maconomyTask = arg; break; }
			default: {
				std::cerr << "Too many arguments at row: " << row << std::endl;
				return false;
				break;
			}
		}
	}

	if ((localJob.empty() && maconomyJob.empty()) ||
		(localTask.empty() && maconomyTask.empty())) {
		std::cerr << "Invalid definition at row: " << row << std::endl;
		return false;
	}

	const std::string key = localJob + ";" + localTask;
	_jobNumberMap[key] = maconomyJob;
	_taskMap[key] = maconomyTask;
	
	return true;
}