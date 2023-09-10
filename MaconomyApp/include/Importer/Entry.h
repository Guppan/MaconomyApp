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

	public:
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

		// Raw input.
		std::vector<std::string> raw;

	public:
		Entry();
		virtual ~Entry() = default;


		// Key.
		std::string key() const;

		// Merge another entry with this.
		void merge(Entry* other);

		// Can this entry be splitted?
		bool canSplit() const;

		// Copy data in preparation for split.
		Entry::ptr splitCopy();

		// Split this entry.
		Entry::ptr split(Entry::ptr(*splitFn)(Entry*));

		// Is this a valid entry for transfer?
		bool isValid() const;

		// Convert to Json.
		std::string toJson() const;
	};

}