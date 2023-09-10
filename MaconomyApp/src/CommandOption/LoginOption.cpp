#include "../../include/CommandOption/LoginOption.h"
#include "../../include/Caller/Caller.h"
#include "../../include/CommandOption/Overlord.h"
#include "../../include/Misc/Constants.h"


using namespace Maconomy;


// Validate option arguments.
bool LoginOption::validate() const {
	return (_args.size() >= mandatory &&
			_args.size() <= mandatory + optional);
}


// Execute this option and returns true if the execution can continue.
bool LoginOption::execute(State& state) {
	// Create and set the caller.
	state.caller = std::make_unique<Caller>();
	if (!state.caller) return false;

	return state.caller->login(username(), password());
}


// Get username.
const std::string& LoginOption::username() const {
	return _args.front();
}


// Get password.
const std::string& LoginOption::password() const {
	return _args.back();
}