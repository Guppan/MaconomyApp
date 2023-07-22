#include "../../include/CommandOption/TransferOption.h"
#include "../../include/Caller/Caller.h"
#include "../../include/Importer/Importer.h"
#include "../../include/CommandOption/Overlord.h"
#include "../../include/CommandOption/OptionConstants.h"

#include <sstream>
#include <iostream>

using namespace Maconomy;


// Does the string represent a valid number?
bool isAllDigits(const std::string& arg) {
	for (const char c : arg) if (!isdigit(c)) return false;
	return true;
}


// Validate option arguments.
bool TransferOption::validate() const {
	if (_args.size() < mandatory ||
		_args.size() > mandatory + optional) return false;

	// Validate the date (on form yyyy-mm-dd).
	if (date().size() != 10) return false;

	std::istringstream ss(date());
	std::string tmp;
	for (int i{}; std::getline(ss, tmp, '-'); ++i) {
		switch (i) {
			case 0: {
				if (tmp.size() != 4 || !isAllDigits(tmp)) {
					return false;
				}
				break;
			}
			case 1:
			case 2: {
				if (tmp.size() != 2 || !isAllDigits(tmp)) {
					return false;
				}
				break;
			}
			default: {
				return false;
				break;
			}
		}
	}
	return true;
}


// Execute this option and returns true if the execution can continue.
bool TransferOption::execute(State& state) {
	if (!state.caller) return false;

	bool valid{ true };
	valid = state.caller->updateTokens(date());

	if (valid) {
		std::vector<Entry*> entries = state.importer->getEntries();
		state.caller->addRows(entries);
	}
	
	state.caller->logout();
	return valid;
}


// Get the date.
const std::string& TransferOption::date() const {
	return _args.front();
}