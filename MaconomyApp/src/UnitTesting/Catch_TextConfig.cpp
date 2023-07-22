
#ifdef UNIT_TEST

#include "../../include/Remote/catch.hpp"

#include "../../include/Config/TextConfig.h"

#include <vector>
#include <utility>


using namespace Maconomy;


//Typedefs.
typedef std::vector<struct KeyVal> keyValVec;


struct KeyVal {
	std::string key;
	std::string job;
	std::string task;
};


void testFilePaths(Config* config,
				   const std::string& file,
				   const std::string& timeLogPath,
				   const std::string& logPath) {
	REQUIRE(config->import(file));
	REQUIRE(config->timeLogPath == timeLogPath);
	REQUIRE(config->logPath == logPath);
}


void testDefaults(Config* config,
				  const std::string& file,
				  const std::string& def) {
	REQUIRE(config->import(file));
	REQUIRE(config->spec3() == def);
}


void testDefinitions(Config* config,
					 const std::string& file,
					 const keyValVec& vec) {
	REQUIRE(config->import(file));

	for (const auto& keyVal : vec) {
		const std::string job = config->getJob(keyVal.key);
		const std::string task = config->getTask(keyVal.key);

		REQUIRE(job == keyVal.job);
		REQUIRE(task == keyVal.task);
	}
}

#include <iostream>
TEST_CASE("Testing text config.", "[TextImport]") {
	const std::string path{ "src/UnitTesting/Catch_TextConfig_" };

	Config::ptr config = std::make_unique<TextConfig>();
	
	SECTION("Importing empty config") {
		testDefinitions(config.get(), path + "config1.txt", {});
	}

	SECTION("Importing file paths") {
		testFilePaths(config.get(),
					  path + "config2.txt",
					  "C:/Users/test/timeLogFile.txt",
					  "C:/Users/test/logFile.txt");
	}

	SECTION("Importing defaults") {
		testDefaults(config.get(),
					 path + "config3.txt",
					 "Developer");
	}

	SECTION("Importing single definition config") {
		keyValVec vec{ {"SE;Task1", "J1", "T1"} };
		testDefinitions(config.get(), path + "config4.txt", vec);
	}

	SECTION("Importing multiple definition config") {
		keyValVec vec{
			{"SE;Task1", "J1", "T1"},
			{"SE;Task2", "J1", "T2"},
			{"NO;Task1", "J2", "T1"},
			{"NO;Task2", "J2", "T2"},
			{"DK;Task1", "J3", "T1"},
			{"DK;Task2", "J3", "T2"},
			{"DK;Task3", "J3", "T3"},
			{"FN;Task1", "J4", "T1"},
			{"FN;Task2", "J4", "T2"},
			{"FN;Task3", "J4", "T3"}
		};
		testDefinitions(config.get(), path + "config5.txt", vec);
	}

	SECTION("Importing fully defined config") {
		testFilePaths(config.get(),
					  path + "config2.txt",
					  "C:/Users/test/timeLogFile.txt",
					  "C:/Users/test/logFile.txt");

		testDefaults(config.get(),
					 path + "config3.txt",
					 "Developer");

		keyValVec vec{
			{"SE;Task1", "J1", "T1"},
			{"SE;Task2", "J1", "T2"},
			{"NO;Task1", "J2", "T1"},
			{"NO;Task2", "J2", "T2"},
			{"DK;Task1", "J3", "T1"},
			{"DK;Task2", "J3", "T2"},
			{"DK;Task3", "J3", "T3"},
			{"FN;Task1", "J4", "T1"},
			{"FN;Task2", "J4", "T2"},
			{"FN;Task3", "J4", "T3"}
		};
		testDefinitions(config.get(), path + "config5.txt", vec);
	}
}

#endif // CATCH_CONFIG_MAIN