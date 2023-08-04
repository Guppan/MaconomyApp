#pragma once

#include "Entry.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>


namespace Maconomy {

	// Forward declarations.
	class Config;

	// Repsonsible for importing and parsing the time log.
	class Importer
	{
	public:
		// Typedefs.
		typedef std::unique_ptr<Importer> ptr;

	protected:
		// Import data.
		Config* _config;

		// Time log entries.
		std::unordered_map<std::string, Entry::ptr> _entries;

	public:
		// Constructor/Destructor.
		Importer(Config* config)
			: _config{ config } {};
		virtual ~Importer() = default;

		// Import time log.
		virtual void import() { }

		// Get Maconomy entries.
		std::vector<Entry*> getEntries();

		// Write to time log.
		virtual void writeToTimeLog() const { }

		// Write to log file.
		virtual void writeToLog() const { }

	protected:
		// Insert an entry into the entries map.
		void insertEntry(Entry::ptr entry);

		// Split entries.
		void splitEntries();

		// Set Maconomy job and task.
		void setJobAndTask();

		// Convert a time string to hours.
		virtual double toHours(const std::string& time) const;
	};

}