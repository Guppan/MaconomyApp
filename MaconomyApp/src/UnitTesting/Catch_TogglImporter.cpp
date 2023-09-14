
#ifdef UNIT_TEST

#include "../../include/Remote/catch.hpp"
#include "../../include/UnitTesting/Catch_UnitTesting.h"

#include "../../include/Config/JsonConfig.h"
#include "../../include/Importer/TogglImporter.h"

#include <iostream>
#include <unordered_set>


using namespace Maconomy;
using namespace UnitTest;


TEST_CASE("Testing import of time log from json file.", "[TogglImport]") {
	const std::string path{ "src/UnitTesting/Catch_TogglImporter_" };

	const std::string configPath{ "src/UnitTesting/Catch_TextImporter_config.json" };
	Config::ptr config = std::make_unique<JsonConfig>();

	REQUIRE(config->import(configPath));

	Importer::ptr importer = std::make_unique<TogglImporter>(config.get());
	refSet set{};

	SECTION("Importing single valid entry time log") {
		config->timeLogPath = path + "timeLog1.json";
		std::vector<Entry*> res = importGetEntries(importer.get());

		set.emplace(json("JOB-0001", "J1", "T1", { 1.0, 0, 0, 0, 0 }));

		testEntries(res, set, 1);
	}

	SECTION("Importing full time log complex") {
		config->timeLogPath = path + "timeLog2.json";
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