#include "../../include/CommandOption/LogPathOption.h"
#include "../../include/Config/Config.h"
#include "../../include/CommandOption/Overlord.h"
#include "../../include/CommandOption/OptionConstants.h"

#include <fstream>


using namespace Maconomy;


// Validate option arguments.
bool LogPathOption::validate() const {
	if (_args.size() < mandatory ||
		_args.size() > mandatory + optional) return false;

	std::ifstream file(_args.back());
	return file.good();
}


// Execute this option and returns true if the execution can continue.
bool LogPathOption::execute(State& state) {
	if (!state.config) return false;

	state.config->timeLogPath = _args.back();
	return true;
}