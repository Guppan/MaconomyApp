#include "../../include/CommandOption/WriteLogOption.h"
#include "../../include/CommandOption/Overlord.h"


using namespace Maconomy;


// Validate option arguments.
bool WriteLogOption::validate() const {
	return (_args.size() >= mandatory &&
			_args.size() <= mandatory + optional);
}


// Execute this option and returns true if the execution can continue.
bool WriteLogOption::execute(State& state) {
	if (!state.importer) return false;

	state.importer->writeToLog();
	return true;
}