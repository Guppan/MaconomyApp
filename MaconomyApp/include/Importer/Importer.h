#pragma once

#include <memory>
#include <vector>


namespace Maconomy {

	// Forward declarations.
	class Config;
	struct Entry;

	// Repsonsible for importing and parsing the time log.
	class Importer
	{
	public:
		// Typedefs.
		typedef std::unique_ptr<Importer> ptr;

	protected:
		// Import data.
		Config* _config;

	public:
		// Constructor/Destructor.
		Importer(Config* config)
			: _config{ config } {};
		virtual ~Importer() = default;

		// Import time log.
		virtual void import() { }

		// Get Maconomy entries.
		virtual std::vector<Entry*> getEntries() = 0;

		// Write to time log.
		virtual void writeToTimeLog() const { }

		// Write to log file.
		virtual void writeToLog() const { }
	};

}