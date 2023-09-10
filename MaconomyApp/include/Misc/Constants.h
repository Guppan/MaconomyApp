#pragma once

#include <string>
#include <unordered_map>
#include <vector>


namespace Maconomy {

	// ----- General Functions ------------------

	// Is the given key 'key' in the domain 'domain'?
	bool inDomain(const std::string& key,
				  const std::vector<std::string>& domain);


	// ===== Maconomy ============================
	// ----- Maconomy Paths ----------------------

	const std::string MACONOMY_SERVER = "https://me96763-iaccess.deltekfirst.com";
	const std::string MACONOMY_AUTH = "maconomy-api/auth/me96763";
	const std::string MACONOMY_INSTANCES = "/maconomy-api/containers/me96763/timeregistration/instances";

	// ----- Maconomy functions ------------------

	std::string maconomyAuthorization();
	std::string maconomyInstances();
	std::string maconomyInstanceId(const std::string& id);

	// ----- Maconomy error texts ----------------

	const std::string MACONOMY_LOGIN_ERROR = "Login to Maconomy failed";
	const std::string MACONOMY_INSTANCE_ERROR = "Instance retrieval failed";
	const std::string MACONOMY_EMPLOYEE_NR_ERROR = "Employee number retrieval failed";
	const std::string MACONOMY_ROW_COUNT_ERROR = "Row count retrieval failed";


	// ===== Config ==============================
	// ----- Json config keys --------------------

	const std::string JC_FILEPATHS = "filepaths";
	const std::string JC_FP_TIME_LOG = "time-log";
	const std::string JC_FP_LOG_FILE = "log-file";

	const std::string JC_IMPORTER_MODE = "importer-mode";
	const std::string JC_IM_MODE = "mode";

	const std::string JC_STRATEGIES = "strategies";
	const std::string JC_ST_ROUND = "round-strategy";
	const std::string JC_ST_SPLIT = "split-strategy";

	const std::string JC_DEFAULTS = "defaults";
	const std::string JC_DEFAULTS_SPEC3 = "spec3";

	const std::string JC_DEFINITIONS = "definitions";
	const std::string JC_DEF_TASKS = "tasks";
	const std::string JC_DEF_LOCAL_JOB = "local-job";
	const std::string JC_DEF_REMOTE_JOB = "remote-job";
	const std::string JC_DEF_LOCAL_TASK = "local-task";
	const std::string JC_DEF_REMOTE_TASK = "remote-task";

	// ----- Importer modes -----------------------

	const std::string IMPORTER_MODE_TEXT = "text";
	const std::string IMPORTER_MODE_TOGGL = "toggl";
	const std::string IMPORTER_MODE_OPTIMIZE = "optimize";

	const std::vector<std::string> IMPORTER_MODE_KEYS = {
		IMPORTER_MODE_TEXT,
		IMPORTER_MODE_TOGGL,
		IMPORTER_MODE_OPTIMIZE
	};

	// ----- Rounding strategies ------------------

	const std::string ROUND_SIMPLE_STRATEGY = "simple";
	const std::string ROUND_ADJUST_UP_STRATEGY = "adjust_up";
	const std::string ROUND_ADJUST_DOWN_STRATEGY = "adjust_down";
	const std::string ROUND_ADJUST_STRATEGY = "adjust";

	const std::vector<std::string> ROUND_STRATEGY_KEYS = {
		ROUND_SIMPLE_STRATEGY,
		ROUND_ADJUST_UP_STRATEGY
	};

	// ----- Splitting strategies -----------------

	const std::string SPLIT_SIMPLE_STRATEGY = "simple";
	const std::string SPLIT_DISCRETE_STRATEGY = "discrete";

	const std::vector<std::string> SPLIT_STRATEGY_KEYS = {
		SPLIT_SIMPLE_STRATEGY,
		SPLIT_DISCRETE_STRATEGY
	};


	// ===== Importer =============================
	// ----- Toggl Json tags ----------------------

	// Time entries array key.
	const std::string TOGGL_JSON_ARRAY = "TimeEntries";

	// Start date (used for determining weekday).
	const std::string TOGGL_JSON_DATE = "StartDate";

	// Daily description.
	const std::string TOGGL_JSON_DESCRIPTION = "Description";

	// Task duration.
	const std::string TOGGL_JSON_DURATION = "Duration";

	// Name of the project.
	const std::string TOGGL_JSON_PROJECT = "ProjectName";

	// Name of the client.
	const std::string TOGGL_JSON_CLIENT = "ClientName";

	// Tag key.
	const std::string TOGGL_JSON_TAG = "TagNames";

	// Job delimiter.
	const char TOGGL_JOB_DELIMITER = ';';

	// ----- Optimize html constants ----------------

	// Start row for time log entries.
	const int OPTIMIZE_START = 35;

	// Task start and end tags.
	const std::string OPTIMIZE_TASK_START = "<B>";
	const std::string OPTIMIZE_TASK_END = "</B>";

	// Job delimiter (used for splitting).
	const char OPTIMIZE_JOB_DELIMITER = ',';

	// Task desctiption delimiter.
	const char OPTIMIZE_TASK_DESC_DELIMITER = ';';

	// Time entry start and end tags.
	const std::string OPTIMIZE_TIME_START = "<CENTER>";
	const std::string OPTIMIZE_TIME_END = "</CENTER>";

	// Empty time log entry value.
	const std::string OPTIMIZE_TIME_EMPTY = "-";

	// End row tag.
	const std::string OPTIMIZE_TOTAL = "Total:";


	// ===== Command options ======================
	// ----- Option keys --------------------------

	const std::string CONFIG_KEY = "-c";
	const std::string IMPORTER_KEY = "-i";
	const std::string LOGIN_KEY = "-u";
	const std::string TRANSFER_KEY = "-s";
	const std::string WRITELOG_KEY = "-wt";

	const std::vector<std::string> OPTION_KEYS = {
		CONFIG_KEY,
		IMPORTER_KEY,
		LOGIN_KEY,
		TRANSFER_KEY,
		WRITELOG_KEY
	};

	// ----- Usage texts -------------------------

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

	const std::string TRANSFER_USAGE =
		"Transfer (-s <date>)\n\
\tPerforms the transfer of the time log data to Maconomy.\n\
\tExample usage: -s 2023-06-30\n\n";


	// Mapping option key to usage text.
	const std::unordered_map<std::string, std::string> USAGE_MAP = {
		{CONFIG_KEY, CONFIG_USAGE},
		{LOGIN_KEY, LOGIN_USAGE},
		{TRANSFER_KEY, TRANSFER_USAGE}
	};
}