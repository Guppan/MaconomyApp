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
		Importer(Config* config);
		virtual ~Importer() = default;


		// Run the importer.
		void run();

		// Get Maconomy entries.
		std::vector<Entry*> getEntries();

		// Write to log file.
		void writeToLog() const;

	protected:
		// Import time log.
		virtual void import() = 0;

		// Convert a time string to hours.
		virtual double toHours(const std::string& time) const;

		// Insert an entry into the entries map.
		void insertEntry(Entry::ptr entry);

	private:
		// Execute rounding strategy.
		void executeRoundingStrategy();

		// Split entries.
		void splitEntries();

		// Set Maconomy job and task.
		void setJobAndTask();
	};

	// Importer factory.
	Importer::ptr importerFactory(Config* config);

}