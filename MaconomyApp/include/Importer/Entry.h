#pragma once

#include <string>
#include <vector>


namespace Maconomy {

	// Represent an entry in the Maconomy time sheet.
	struct Entry {
		// Daily description.
		std::string description;

		// Job number(s).
		std::vector<std::string> jobNumber;

		// Task name.
		std::string taskName;

		// Spec 3.
		std::string spec3;

		// Logged time.
		std::vector<double> times;

		// Is this entry valid?
		bool valid;


		// Constructor/Destructor.
		Entry();
		~Entry() = default;


		// Key.
		std::string key() const;

		// Merge another entry with this.
		void merge(Entry& other);

		// Can this entry be splitted?
		bool canSplit() const;

		// Split this entry.
		Entry split();

		// Convert to Json.
		std::string toJson() const;
	};

}


