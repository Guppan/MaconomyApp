#include "../../include/Config/JsonConfig.h"
#include "../../include/Misc/Constants.h"
#include "../../include/Misc/Functionality.h"

#include <fstream>
#include <initializer_list>
#include <iostream>


using namespace Maconomy;
using json = nlohmann::json;


namespace {

	// Set the parsed strategy on the field 'field'.
	void setStrategy(nlohmann::json& parsed,
					 const std::string& key,
					 const std::vector<std::string>& domain,
					 std::string& field) {
		if (!parsed.contains(key)) return;

		std::string strategy;
		parsed[key].get_to(strategy);

		if (!inDomain(strategy, domain)) {
			std::cerr << "Invalid strategy: " << strategy << std::endl;
			return;
		}

		field = strategy;
	}

}


// Import config.
bool JsonConfig::import(const std::string& path) {
	std::ifstream file;
	file.open(path);
	if (!file) {
		std::cerr << "Couldn't open json config file: " << path << std::endl;
		return false;
	}

	json parsed = json::parse(file);

	importFilePaths(parsed);
	importImporterMode(parsed);
	importStrategies(parsed);
	importDefaults(parsed);
	importDefinitions(parsed);

	return true;
}


// Import file paths.
void JsonConfig::importFilePaths(nlohmann::json& parsed) {
	json* res{ nullptr };
	JsonAccess access(&parsed);
	
	res = access[JC_FILEPATHS][JC_FP_TIME_LOG].get();
	if (res) res->get_to(timeLogPath);

	res = access[JC_FILEPATHS][JC_FP_LOG_FILE].get();
	if (res) res->get_to(logPath);
}


// Import importer mode.
void JsonConfig::importImporterMode(nlohmann::json& parsed) {
	json* res{ nullptr };
	JsonAccess access(&parsed);

	res = access[JC_IMPORTER_MODE][JC_IM_MODE].get();
	if (res) res->get_to(_importerMode);
}


// Import strategies.
void JsonConfig::importStrategies(nlohmann::json& parsed) {
	if (!parsed.contains(JC_STRATEGIES)) return;
	json& strategies = parsed[JC_STRATEGIES];

	// Set round strategy.
	setStrategy(strategies,
				JC_ST_ROUND,
				ROUND_STRATEGY_KEYS,
				_roundStrategy);

	// Set split strategy.
	setStrategy(strategies,
				JC_ST_SPLIT,
				SPLIT_STRATEGY_KEYS,
				_splitStrategy);
}


// Import defaults.
void JsonConfig::importDefaults(nlohmann::json& parsed) {
	json* res{ nullptr };
	JsonAccess access(&parsed);

	res = access[JC_DEFAULTS][JC_DEFAULTS_SPEC3].get();
	if (res) res->get_to(_spec3);
}


// Import definitions.
void JsonConfig::importDefinitions(nlohmann::json& parsed) {
	if (!parsed.contains(JC_DEFINITIONS)) return;

	for (auto& def : parsed[JC_DEFINITIONS]) {
		if (!contains(def, { JC_DEF_LOCAL_JOB, JC_DEF_REMOTE_JOB })) {
			std::cerr << "Invalid json definition: job" << std::endl;
			continue;
		}
		if (!def.contains(JC_DEF_TASKS)) {
			std::cerr << "Invalid json definition: task" << std::endl;
			continue;
		}

		std::string localJob, remoteJob;
		def[JC_DEF_LOCAL_JOB].get_to(localJob);
		def[JC_DEF_REMOTE_JOB].get_to(remoteJob);

		for (auto& task : def[JC_DEF_TASKS]) {
			if (!contains(task, { JC_DEF_LOCAL_TASK, JC_DEF_REMOTE_TASK })) {
				std::cerr << "Invalid json definition: local/remote task" << std::endl;
				continue;
			}

			std::string localTask, remoteTask;
			task[JC_DEF_LOCAL_TASK].get_to(localTask);
			task[JC_DEF_REMOTE_TASK].get_to(remoteTask);

			const std::string key = localJob + ";" + localTask;
			_jobNumberMap[key] = remoteJob;
			_taskMap[key] = remoteTask;
		}
	}
}