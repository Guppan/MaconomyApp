#include "../../include/CommandOption/ImporterOption.h"
#include "../../include/Config/Config.h"
#include "../../include/Importer/TextImporter.h"
#include "../../include/Importer/TogglImporter.h"
#include "../../include/Importer/OptimizeImporter.h"
#include "../../include/CommandOption/Overlord.h"
#include "../../include/Misc/Constants.h"


using namespace Maconomy;


// Validate option arguments.
bool ImporterOption::validate() const {
	return (_args.size() >= mandatory &&
			_args.size() <= mandatory + optional);
}


// Spawn other options this option might depend on.
CommandOption::vector ImporterOption::optionSpawner() const {
	CommandOption::vector vec;

	// Spawn log writer option.
	OptionParam param2{ WRITELOG_KEY, { }, true };
	vec.emplace_back(optionFactory(param2));

	return vec;
}


// Execute this option and returns true if the execution can continue.
bool ImporterOption::execute(State& state) {
	// Create the importer and set it on the state.
	Importer::ptr ptr = importerFactory(state.config.get());
	if (!ptr) return false;

	state.importer = std::move(ptr);
	state.importer->run();

	return true;
}