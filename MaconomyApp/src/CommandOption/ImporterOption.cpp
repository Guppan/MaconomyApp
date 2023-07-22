#include "../../include/CommandOption/ImporterOption.h"
#include "../../include/Config/Config.h"
#include "../../include/Importer/TextImporter.h"
#include "../../include/CommandOption/Overlord.h"
#include "../../include/CommandOption/OptionConstants.h"


using namespace Maconomy;


// Validate option arguments.
bool ImporterOption::validate() const {
	return (_args.size() >= mandatory &&
			_args.size() <= mandatory + optional);
}


// Spawn other options this option might depend on.
CommandOption::vector ImporterOption::optionSpawner() const {
	CommandOption::vector vec;

	// Spawn time log writer option.
	OptionParam param1{ WRITETIMELOG_KEY, { }, true };
	vec.emplace_back(optionFactory(param1));

	// Spawn log writer option.
	OptionParam param2{ WRITELOG_KEY, { }, true };
	vec.emplace_back(optionFactory(param2));

	return vec;
}


// Execute this option and returns true if the execution can continue.
bool ImporterOption::execute(State& state) {
	// Create the importer and set it on the state.
	Importer::ptr ptr = importer(state.config.get());
	if (!ptr) return false;

	state.importer = std::move(ptr);
	state.importer->import();

	return true;
}


// Get the current mode.
const std::string& ImporterOption::mode() const {
	if (_args.size() > mandatory) return _args.back();
	return TEXT_MODE;
}


// Get the importer.
Importer::ptr ImporterOption::importer(Config* config) const {
	Importer::ptr ptr = nullptr;
	if (config) {
		if (mode() == TEXT_MODE) {
			ptr = std::make_unique<TextImporter>(config);
		}
	}
	return ptr;
}