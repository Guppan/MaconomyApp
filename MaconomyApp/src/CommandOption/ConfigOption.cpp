#include "../../include/CommandOption/ConfigOption.h"
#include "../../include/CommandOption/Overlord.h"
#include "../../include/CommandOption/OptionConstants.h"
#include "../../include/Config/TextConfig.h"
#include "../../include/Importer/TextImporter.h"

#include <algorithm>
#include <fstream>


using namespace Maconomy;


// Validate option arguments.
bool ConfigOption::validate() const {
	if (_args.size() < mandatory ||
		_args.size() > mandatory + optional) return false;

	// Check if config file exists.
	std::ifstream file(_args.front());
	if (!file.good()) return false;

	// Check if mode is okay.
	if (_args.size() > mandatory) {
		const std::string& mode = _args.back();
		
		auto it = std::find(MODES.cbegin(), MODES.cend(), mode);
		if (it == MODES.cend()) return false;
	}
	return true;
}


// Spawn other options this option might depend on.
CommandOption::vector ConfigOption::optionSpawner() const {
	CommandOption::vector vec;

	// Spawn importer.
	OptionParam param{ IMPORTER_KEY, { mode() }, true };
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


// Get the current mode.
const std::string& ConfigOption::mode() const {
	if (_args.size() > mandatory) return _args.back();
	return TEXT_MODE;
}


// Get the config to use.
Config::ptr ConfigOption::config() const {
	// Currently all modes uses TextConfig.
	return std::make_unique<TextConfig>();
}