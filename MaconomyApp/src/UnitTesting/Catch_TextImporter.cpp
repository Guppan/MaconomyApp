
#ifdef UNIT_TEST

#include "../../include/Remote/catch.hpp"

#include "../../include/Config/TextConfig.h"
#include "../../include/Importer/TextImporter.h"

#include <iostream>
#include <unordered_set>


using namespace Maconomy;


// Typedefs.
typedef std::unordered_set<std::string> refSet;


std::vector<Entry*> importGetEntries(Importer* importer) {
	if (!importer) return {};

	importer->import();
	return importer->getEntries();
}


void testEntries(const std::vector<Entry*>& entries,
				 refSet& ref,
				 const int count) {

	REQUIRE(entries.size() == count);

	for (auto& entry : entries) {
		const std::string json{ entry->toJson() };

		auto it = ref.find(json);
		REQUIRE(it != ref.end());

		ref.erase(it);
	}

	REQUIRE(ref.size() == 0);
}


std::string json(const std::string& desc,
				 const std::string& job,
				 const std::string task,
				 const std::vector<double> hours,
				 const std::string spec3 = "Developer") {
	std::string res;

	res += "{\"data\":{";
	res += "\"jobnumber\":\"" + job + "\"";
	res += ",\"taskname\":\"" + task + "\"";
	res += ",\"specification3name\":\"" + spec3 + "\"";

	for (unsigned i{}; i < hours.size(); ++i) {
		if (hours[i] < 0.001) continue;

		const std::string day = std::to_string(i + 1);

		res += ",\"numberday" + day + "\":" + std::to_string(hours[i]);
		res += ",\"descriptionday" + day + "\":\"" + desc + "\"";
	}

	res += "}}";
	return res;
}


TEST_CASE("Testing import of time log from text file.", "[TextImport]") {
	const std::string path{ "src/UnitTesting/Catch_TextImporter_" };

	const std::string configPath{ path + "config.txt" };
	Config::ptr config = std::make_unique<TextConfig>();

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

	SECTION("Importing multiple same job entries with processed") {
		config->timeLogPath = path + "timeLog4.txt";
		std::vector<Entry*> res = importGetEntries(importer.get());

		set.emplace(json("JOB-0001", "J1", "T1", { 1.0, 1.0, 1.0, 1.0, 1.0 }));

		testEntries(res, set, 1);
	}

	SECTION("Importing two jobs simple") {
		config->timeLogPath = path + "timeLog5.txt";
		std::vector<Entry*> res = importGetEntries(importer.get());

		set.emplace(json("JOB-0001", "J1", "T1", { 1.0, 0, 0, 0, 0 }));
		set.emplace(json("JOB-0002", "J1", "T1", { 1.0, 0, 0, 0, 0 }));

		testEntries(res, set, 2);
	}

	SECTION("Importing two jobs simple week") {
		config->timeLogPath = path + "timeLog6.txt";
		std::vector<Entry*> res = importGetEntries(importer.get());

		set.emplace(json("JOB-0001", "J1", "T1", { 1.0, 0, 1.0, 0, 1.0 }));
		set.emplace(json("JOB-0002", "J1", "T1", { 1.0, 1.0, 0, 2.0, 0 }));

		testEntries(res, set, 2);
	}

	SECTION("Importing split job simple") {
		config->timeLogPath = path + "timeLog7.txt";
		std::vector<Entry*> res = importGetEntries(importer.get());

		set.emplace(json("JOB-0001", "J1", "T1", { 0.5, 0, 0, 0, 0 }));
		set.emplace(json("JOB-0001", "J2", "T1", { 0.5, 0, 0, 0, 0 }));

		testEntries(res, set, 2);
	}

	SECTION("Importing split job complex") {
		config->timeLogPath = path + "timeLog8.txt";
		std::vector<Entry*> res = importGetEntries(importer.get());

		set.emplace(json("JOB-0001", "J1", "T1", { 0.5, 1.0, 1.0, 0, 0 }));
		set.emplace(json("JOB-0001", "J2", "T1", { 0.5, 1.0, 0, 0, 0 }));
		set.emplace(json("JOB-0002", "J1", "T1", { 1.0, 0, 0.5, 0, 0 }));
		set.emplace(json("JOB-0002", "J2", "T1", { 0.5, 0, 1.0, 0, 0 }));

		testEntries(res, set, 4);
	}

	SECTION("Importing full time log complex") {
		config->timeLogPath = path + "timeLog9.txt";
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

#endif // CATCH_CONFIG_MAIN