#pragma once

#include <string>
#include <vector>


namespace Maconomy {

	// ----- Option keys ---------------------------------------

	const std::string CONFIG_KEY = "-c";
	const std::string IMPORTER_KEY = "-i";
	const std::string LOGIN_KEY = "-u";
	const std::string TRANSFER_KEY = "-s";
	const std::string TIMEPATH_KEY = "-t";
	const std::string LOGPATH_KEY = "-l";
	const std::string WRITETIMELOG_KEY = "-wtl";
	const std::string WRITELOG_KEY = "-wt";

	const std::vector<std::string> OPTION_KEYS = {
		CONFIG_KEY,
		IMPORTER_KEY,
		LOGIN_KEY,
		TRANSFER_KEY,
		TIMEPATH_KEY,
		LOGPATH_KEY,
		WRITETIMELOG_KEY,
		WRITELOG_KEY
	};

	// ----- Modes --------------------------------------------

	const std::string TEXT_MODE = "txt";
	const std::string TOGGL_MODE = "toggl";

	const std::vector<std::string> MODES = {
		TEXT_MODE,
		TOGGL_MODE
	};
	
	// ----- Usage texts --------------------------------------

	const std::string CONFIG_USAGE =
		"Configuration (-c <path> [<mode>])\n\
\tImports the Maconomy configuration from the given path.\n\
\tAn additional mode can be passed to select which\n\
\tconfig to use.\n\
\tAvaliable modes: [ txt ]\n\
\tExample usage: -c c:/folder/config.txt txt\n\n";

	const std::string LOGIN_USAGE =
		"Maconomy login (-u <username> <password>)\n\
\tPerforms a login into Maconomy with the given credentials.\n\
\tExample usage: -u user 123456\n\n";

	const std::string LOGPATH_USAGE =
		"Log path (-l <path>)\n\
\tPath to an error log.\n\
\tExample usage: -l c:/folder/log.txt\n\n";

	const std::string TIMEPATH_USAGE =
		"Time log path (-t <path>)\n\
\tPath to a time log'.\n\
\tExample usage: -t c:/folder/timeLog.txt\n\n";

	const std::string TRANSFER_USAGE =
		"Transfer (-s <date>)\n\
\tPerforms the transfer of the time log data to Maconomy.\n\
\tExample usage: -s 2023-06-30\n\n";


	// Mapping option key to usage text.
	const std::unordered_map<std::string, std::string> USAGE_MAP{
		{CONFIG_KEY, CONFIG_USAGE},
		{LOGIN_KEY, LOGIN_USAGE},
		{LOGPATH_KEY, LOGPATH_USAGE},
		{TIMEPATH_KEY, TIMEPATH_USAGE},
		{TRANSFER_KEY, TRANSFER_USAGE}
	};
}