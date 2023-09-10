
#ifdef UNIT_TEST

#include "../../include/Remote/catch.hpp"
#include "../../include/UnitTesting/Catch_UnitTesting.h"

#include "../../include/Config/JsonConfig.h"
#include "../../include/Importer/TextImporter.h"

#include <iostream>
#include <unordered_set>


using namespace Maconomy;
using namespace UnitTest;


TEST_CASE("Testing import of time log from text file.", "[TextImport]") {
	
	const std::string path{ "src/UnitTesting/Catch_TextImporter_" };

	const std::string configPath{ path + "config.json" };
	Config::ptr config = std::make_unique<JsonConfig>();
	
	REQUIRE(config->import(configPath));

	Importer::ptr importer = std::make_unique<TextImporter>(config.get());
	refSet set{};
	
	SECTION("Importing empty time log") {
		config->timeLogPath = path + "timeLog1.txt";
		std::vector<Entry*> res = importGetEntries(importer.get());

		testEntries(res, set, 0);
	}
	
	SECTION("Importing single valid entry time log") {
		config->timeLogPath = path + "timeLog2.txt";
		std::vector<Entry*> res = importGetEntries(importer.get());

		set.emplace(json("JOB-0001", "J1", "T1", { 1.0, 0, 0, 0, 0 }));

		testEntries(res, set, 1);
	}
	
	SECTION("Importing multiple same job entries") {
		config->timeLogPath = path + "timeLog3.txt";
		std::vector<Entry*> res = importGetEntries(importer.get());

		set.emplace(json("JOB-0001", "J1", "T1", { 1.0, 1.0, 1.0, 1.0, 1.0 }));

		testEntries(res, set, 1);
	}
	
	SECTION("Importing two jobs simple") {
		config->timeLogPath = path + "timeLog4.txt";
		std::vector<Entry*> res = importGetEntries(importer.get());

		set.emplace(json("JOB-0001", "J1", "T1", { 1.0, 0, 0, 0, 0 }));
		set.emplace(json("JOB-0002", "J1", "T1", { 1.0, 0, 0, 0, 0 }));

		testEntries(res, set, 2);
	}
	
	SECTION("Importing two jobs simple week") {
		config->timeLogPath = path + "timeLog5.txt";
		std::vector<Entry*> res = importGetEntries(importer.get());

		set.emplace(json("JOB-0001", "J1", "T1", { 1.0, 0, 1.0, 0, 1.0 }));
		set.emplace(json("JOB-0002", "J1", "T1", { 1.0, 1.0, 0, 2.0, 0 }));

		testEntries(res, set, 2);
	}
	
	SECTION("Importing split job simple") {
		config->timeLogPath = path + "timeLog6.txt";
		std::vector<Entry*> res = importGetEntries(importer.get());

		set.emplace(json("JOB-0001", "J1", "T1", { 0.5, 0, 0, 0, 0 }));
		set.emplace(json("JOB-0001", "J2", "T1", { 0.5, 0, 0, 0, 0 }));

		testEntries(res, set, 2);
	}
	
	SECTION("Importing split job complex") {
		config->timeLogPath = path + "timeLog7.txt";
		std::vector<Entry*> res = importGetEntries(importer.get());

		set.emplace(json("JOB-0001", "J1", "T1", { 0.5, 1.0, 1.0, 0, 0 }));
		set.emplace(json("JOB-0001", "J2", "T1", { 0.5, 1.0, 0, 0, 0 }));
		set.emplace(json("JOB-0002", "J1", "T1", { 1.0, 0, 0.5, 0, 0 }));
		set.emplace(json("JOB-0002", "J2", "T1", { 0.5, 0, 1.0, 0, 0 }));

		testEntries(res, set, 4);
	}
	
	SECTION("Importing full time log complex") {
		config->timeLogPath = path + "timeLog8.txt";
		std::vector<Entry*> res = importGetEntries(importer.get());
		
		set.emplace(json("JOB-0001", "J1", "T2", { 2.0, 0.5, 1.0, 0, 0 }));
		set.emplace(json("JOB-0001", "J2", "T2", { 3.5, 1.5, 0, 0, 0 }));
		set.emplace(json("JOB-0001", "J3", "T2", { 1.0, 0, 0, 1.0, 2.0 }));
		set.emplace(json("JOB-0001", "J4", "T2", { 0.5, 0.5, 1.0, 0, 0 }));

		set.emplace(json("JOB-0002", "J1", "T1", { 0, 2.5, 0, 0, 0 }));
		set.emplace(json("JOB-0002", "J1", "T2", { 0, 0, 0, 3.0, 0 }));
		set.emplace(json("JOB-0002", "J2", "T2", { 0, 0, 0, 1.0, 0 }));
		set.emplace(json("JOB-0002", "J2", "T3", { 0, 1.5, 1.5, 0, 0 }));

		set.emplace(json("JOB-0003", "J4", "T1", { 0, 0.5, 0, 0, 0 }));

		set.emplace(json("JOB-0004", "J3", "T1", { 0, 0, 0, 3.0, 3.0 }));
		set.emplace(json("JOB-0004", "J4", "T1", { 0, 0, 0, 0, 3.0 }));

		testEntries(res, set, 11);
	}
}

#endif // UNIT_TEST