#pragma once

#include <memory>
#include <string>
#include <vector>


namespace Maconomy {

	// Represent an entry in the Maconomy time sheet.
	class Entry {
	public:
		// Typedefs.
		typedef std::unique_ptr<Entry> ptr;


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
		virtual ~Entry() = default;


		// Key.
		virtual std::string key() const;

		// Merge another entry with this.
		void merge(Entry* other);

		// Can this entry be splitted?
		virtual bool canSplit() const;

		// Split this entry.
		virtual Entry::ptr split() = 0;

		// Is this a valid entry for transfer?
		virtual bool isValid() const;

		// Convert to Json.
		std::string toJson() const;
	};

}


