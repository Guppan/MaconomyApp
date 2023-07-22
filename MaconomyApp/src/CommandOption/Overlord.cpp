#include "../../include/CommandOption/Overlord.h"
#include "../../include/CommandOption/CommandOption.h"
#include "../../include/CommandOption/OptionConstants.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>


using namespace Maconomy;


// Run the program.
int Overlord::run(int argc, char** argv) {
	std::ostringstream ss;
	std::vector<OptionParam> params = parse(argc, argv);
	if (argc == 1) {
		for (auto it = USAGE_MAP.cbegin(); it != USAGE_MAP.cend(); ++it) {
			ss << it->second;
		}
		std::cout << ss.str() << std::endl;
		return -1;
	}

	for (auto& param : params) {
		if (!append(param)) {
			std::cerr << "Invalid option:\n";
			std::cerr << "  Key: " << param.key << std::endl;
			return -1;
		}
	}
	spawn();
	sort();

	if (!validate(ss)) {
		std::cout << ss.str() << std::endl;
		return -1;
	}

	return execute() ? 0 : -1;
}


// Parse the command line.
std::vector<OptionParam> Overlord::parse(int argc, char** argv) {
	std::vector<std::string> args;
	if (argc > 1) args.assign(argv + 1, argv + argc);

	std::vector<OptionParam> params;

	OptionParam current;
	current.internal = false;
	for (unsigned i{}; i < args.size(); ++i) {
		const std::string arg = args[i];
		auto it = std::find(OPTION_KEYS.cbegin(),
							OPTION_KEYS.cend(),
							arg);

		if (it != OPTION_KEYS.cend()) {
			if (i != 0) {
				params.push_back(current);
				current.key.clear();
				current.args.clear();
			}

			current.key = arg;
			continue;
		}

		current.args.push_back(arg);
	}
	if (!current.key.empty()) params.push_back(current);

	return params;
}


// Append an option.
bool Overlord::append(OptionParam& param) {
	CommandOption::ptr option = optionFactory(param);
	if (!option || !option->canAppend(_options)) return false;

	_options.push_back(std::move(option));
	return true;
}


// Spawn dependent options.
void Overlord::spawn() {
	for (unsigned i{}; i < _options.size(); ++i) {
		CommandOption* option = _options[i].get();
		for (auto& spawned : option->optionSpawner()) {
			if (spawned && spawned->canAppend(_options)) {
				_options.push_back(std::move(spawned));
			}
		}
	}
}


// Sort the option based on their priority.
void Overlord::sort() {
	std::sort(_options.begin(),
			  _options.end(),
			  [](const CommandOption::ptr& lhs, const CommandOption::ptr& rhs) {
				  return lhs->priority() > rhs->priority();
			  });
}


// Validate the required options.
bool Overlord::validate(std::ostringstream& ss) const {
	std::unordered_set<std::string> set;

	// Populate the map. Ensure unique options.
	for (const auto& option : _options) {
		const std::string key = option->id();
		set.emplace(key);
	}

	// Ensure valid option and option dependency.
	for (const auto& option : _options) {
		if (!option->validate()) {
			option->usage(ss);
			return false;
		}

		for (const auto& key : option->required()) {
			const auto it = set.find(key);
			if (it == set.cend()) {
				const auto it2 = USAGE_MAP.find(key);
				ss << "Missing required option: " << key << '\n';
				if (it2 != USAGE_MAP.cend()) ss << it2->second;
				
				return false;
			}
		}
	}
	return true;
}


// Execute the options in the order of priority.
bool Overlord::execute() {
	bool success{ true };
	for (auto& option : _options) {
		if (!option->execute(_state)) {
			success = false;
			break;
		}
	}
	return success;
}