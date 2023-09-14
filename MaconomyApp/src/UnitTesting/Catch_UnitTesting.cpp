#ifdef UNIT_TEST

#include "../../include/UnitTesting/Catch_UnitTesting.h"
#include "../../include/Remote/catch.hpp"


using namespace Maconomy;


std::vector<Entry*> UnitTest::importGetEntries(Importer* importer) {
	if (!importer) return {};

	importer->run();
	return importer->getEntries();
}


void UnitTest::testEntries(const std::vector<Entry*>& entries,
						   refSet& ref,
						   const int count) {

	REQUIRE(entries.size() == count);

	for (auto& entry : entries) {
		const std::string json{ entry->toJson() };
		
		auto it = ref.find(json);
		INFO("Json not in reference set: " << json);
		REQUIRE(it != ref.end());
		
		ref.erase(it);
	}

	REQUIRE(ref.size() == 0);
}


std::string UnitTest::json(const std::string& desc,
						   const std::string& job,
						   const std::string task,
						   const std::vector<double> hours,
						   const std::string spec3) {
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


#endif // UNIT_TEST