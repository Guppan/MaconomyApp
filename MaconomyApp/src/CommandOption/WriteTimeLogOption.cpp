#include "../../include/CommandOption/WriteTimeLogOption.h"
#include "../../include/CommandOption/Overlord.h"


using namespace Maconomy;


// Validate option arguments.
bool WriteTimeLogOption::validate() const {
	return (_args.size() >= mandatory &&
			_args.size() <= mandatory + optional);
}


// Execute this option and returns true if the execution can continue.
bool WriteTimeLogOption::execute(State& state) {
	if (!state.importer) return false;

	state.importer->writeToTimeLog();
	return true;
}