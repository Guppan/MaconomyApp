#include "../../include/CommandOption/ConfigOption.h"
#include "../../include/CommandOption/Overlord.h"
#include "../../include/Misc/Constants.h"
#include "../../include/Importer/TextImporter.h"
#include "../../include/Config/JsonConfig.h"

#include <algorithm>
#include <fstream>


using namespace Maconomy;


// Validate option arguments.
bool ConfigOption::validate() const {
	if (_args.size() < mandatory ||
		_args.size() > mandatory + optional) return false;

	// Check if config file exists.
	std::ifstream file(_args.front());
	return file.good();
}


// Spawn other options this option might depend on.
CommandOption::vector ConfigOption::optionSpawner() const {
	CommandOption::vector vec;

	// Spawn importer.
	OptionParam param{ IMPORTER_KEY, { }, true };
	vec.emplace_back(optionFactory(param));

	return vec;
}


// Execute this option and returns true if the execution can continue.
bool ConfigOption::execute(State& state) {
	// Create and import the config.
	Config::ptr ptr = config();
	if (!ptr) return false;

	state.config = std::move(ptr);
	return state.config->import(filePath());
}


// Get the config file path.
const std::string& ConfigOption::filePath() const {
	return _args.front();
}


// Get the config to use.
Config::ptr ConfigOption::config() const {
	// Currently all modes uses JsonConfig.
	return std::make_unique<JsonConfig>();
}