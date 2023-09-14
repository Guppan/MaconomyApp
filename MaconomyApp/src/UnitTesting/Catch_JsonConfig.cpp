
#ifdef UNIT_TEST

#include "../../include/Remote/catch.hpp"

#include "../../include/Config/JsonConfig.h"
#include "../../include/Misc/Constants.h"

#include <vector>
#include <utility>


using namespace Maconomy;


namespace {

	//Typedefs.
	typedef std::vector<struct KeyVal> keyValVec;

	struct KeyVal {
		std::string key;
		std::string job;
		std::string task;
	};


	// Test definitions.
	void testDefinitions(Config* config,
						 const keyValVec& vec) {
		for (const auto& keyVal : vec) {
			const std::string job = config->getJob(keyVal.key);
			const std::string task = config->getTask(keyVal.key);

			REQUIRE(job == keyVal.job);
			REQUIRE(task == keyVal.task);
		}
	}

}


TEST_CASE("Testing json config.", "[JsonConfig]") {
	const std::string path{ "src/UnitTesting/Catch_JsonConfig_" };

	Config::ptr config = std::make_unique<JsonConfig>();

	SECTION("Importing fully defined config") {
		REQUIRE(config->import(path + "config1.json"));

		// Testing file paths.
		REQUIRE(config->timeLogPath == "C:/Users/test/timeLogFile.txt");
		REQUIRE(config->logPath == "C:/Users/test/logFile.txt");

		// Testing importer mode.
		REQUIRE(config->importerMode() == IMPORTER_MODE_TEXT);

		// Testing strategies.
		REQUIRE(config->roundStrategy() == ROUND_SIMPLE_STRATEGY);
		REQUIRE(config->splitStrategy() == SPLIT_DISCRETE_STRATEGY);

		// Testing defaults.
		REQUIRE(config->spec3() == "Developer");

		// Testing definitions.
		const keyValVec vec{
			{"SE;Task1", "J1", "T1"},
			{"SE;Task2", "J1", "T2"},
			{"SE;Task3", "J1", "T3"},
			{"NO;Task1", "J2", "T1"},
			{"NO;Task2", "J2", "T2"},
			{"NO;Task3", "J2", "T3"},
			{"DK;Task1", "J3", "T1"},
			{"DK;Task2", "J3", "T2"},
			{"FN;Task1", "J4", "T1"},
			{"FN;Task2", "J4", "T2"}
		};
		testDefinitions(config.get(), vec);
	}
}

#endif // CATCH_CONFIG_MAIN