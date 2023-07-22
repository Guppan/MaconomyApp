#pragma once

#include "../Importer/Importer.h"
#include "Entry.h"

#include <string>
#include <unordered_map>
#include <vector>


namespace Maconomy {

	class TextImporter : public Importer
	{
	private:
		// Represent a line in the time log.
		struct TimeLine {
			// Maconomy entry.
			Entry entry;

			// Has this time line already been processed?
			bool processed{ false };

			// Row number.
			unsigned row{};

			// Raw input.
			std::vector<std::string> raw;


			// Key of this time line.
			std::string key() const;
		};

		// Time lines from the time log.
		std::unordered_map<std::string, struct TimeLine> _timeLines;

	public:
		// Use base class constructor.
		using Importer::Importer;


		// Import time log.
		void import() override;

		// Get Maconomy entries.
		std::vector<Entry*> getEntries() override;

		// Write updated data to time log.
		void writeToTimeLog() const override;

		// Write to log file.
		void writeToLog() const override;

	private:
		// Insert time line into map.
		void insertTimeLine(std::unordered_map<std::string, struct TimeLine>& map,
							struct TimeLine timeLine);

		// Split the entries.
		void splitEntries();

		// Set Maconomy job number and task name.
		void setJobAndTask();

		// Sort time lines and return as vector.
		std::vector<const struct TimeLine*> sortedTimeLines() const;

		// Write to file.
		void writeToFile(const std::string& path,
						 bool toTimeLog) const;
	};

}
