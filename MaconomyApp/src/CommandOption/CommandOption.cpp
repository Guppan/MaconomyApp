#include "../../include/CommandOption/CommandOption.h"
#include "../../include/CommandOption/ConfigOption.h"
#include "../../include/CommandOption/ImporterOption.h"
#include "../../include/CommandOption/LoginOption.h"
#include "../../include/CommandOption/LogPathOption.h"
#include "../../include/CommandOption/TimePathOption.h"
#include "../../include/CommandOption/TransferOption.h"
#include "../../include/CommandOption/WriteTimeLogOption.h"
#include "../../include/CommandOption/WriteLogOption.h"
#include "../../include/CommandOption/OptionConstants.h"


using namespace Maconomy;


// Command option factory.
std::unique_ptr<CommandOption> Maconomy::optionFactory(OptionParam& param) {
	typedef std::vector<std::string> V_;

	std::unique_ptr<CommandOption> option = nullptr;
	const std::string key = param.key;

	if (key == LOGIN_KEY) {
		option = std::make_unique<LoginOption>(
			LOGIN_KEY,
			1000,
			V_{ CONFIG_KEY });

	} else if (key == CONFIG_KEY) {
		option = std::make_unique<ConfigOption>(
			CONFIG_KEY,
			3000,
			V_{ LOGIN_KEY });

	} else if (key == TRANSFER_KEY) {
		option = std::make_unique<TransferOption>(
			TRANSFER_KEY,
			500,
			V_{ LOGIN_KEY, CONFIG_KEY, IMPORTER_KEY });

	} else if (key == IMPORTER_KEY && param.internal) {
		option = std::make_unique<ImporterOption>(
			IMPORTER_KEY,
			1500,
			V_{ LOGIN_KEY, CONFIG_KEY });

	} else if (key == TIMEPATH_KEY) {
		option = std::make_unique<TimePathOption>(
			TIMEPATH_KEY,
			2000,
			V_{ CONFIG_KEY });

	} else if (key == LOGPATH_KEY) {
		option = std::make_unique<LogPathOption>(
			LOGPATH_KEY,
			2000,
			V_{ CONFIG_KEY });

	} else if (key == WRITETIMELOG_KEY && param.internal) {
		option = std::make_unique<WriteTimeLogOption>(
			WRITETIMELOG_KEY,
			200,
			V_{ IMPORTER_KEY });

	} else if (key == WRITELOG_KEY && param.internal) {
		option = std::make_unique<WriteLogOption>(
			WRITELOG_KEY,
			200,
			V_{ IMPORTER_KEY });

	}

	if (option) option->set(std::move(param.args));
	return option;
}


// Constructor.
CommandOption::CommandOption(const std::string& id,
							 const int priority,
							 std::vector<std::string>&& required) :
	_id{ id },
	_priority{ priority },
	_required{ std::move(required) } { }


// Option id.
const std::string& CommandOption::id() const {
	return _id;
}


// Execution priority.
int CommandOption::priority() const {
	return _priority;
}


// Required options for this option.
const std::vector<std::string>& CommandOption::required() const {
	return _required;
}


// Set arguments.
void CommandOption::set(std::vector<std::string>&& args) {
	_args = args;
}


// Can this option be appended to the list of options?
bool CommandOption::canAppend(const CommandOption::vector& options) const {
	for (const auto& option : options) if (option->id() == id()) return false;
	return true;
}


// This option's usage text (displayed in terminal).
void CommandOption::usage(std::ostringstream& ss) const {
	const auto it = USAGE_MAP.find(id());
	if (it != USAGE_MAP.cend()) ss << it->second;
}