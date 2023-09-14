#pragma once

#include "../Importer/Importer.h"
#include "../Importer/Entry.h"

#include <string>
#include <unordered_set>
#include <vector>


namespace Maconomy {
	namespace UnitTest {

		// Typedefs.
		typedef std::unordered_set<std::string> refSet;

		// Test import.
		std::vector<Entry*> importGetEntries(Importer* importer);

		// Test the entries.
		void testEntries(const std::vector<Entry*>& entries,
						 refSet& ref,
						 const int count);

		// Express as Json.
		std::string json(const std::string& desc,
						 const std::string& job,
						 const std::string task,
						 const std::vector<double> hours,
						 const std::string spec3 = "Developer");
	}
}