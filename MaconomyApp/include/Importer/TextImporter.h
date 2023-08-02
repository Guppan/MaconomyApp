#pragma once

#include "Importer.h"
#include "TextEntry.h"

#include <string>
#include <unordered_map>
#include <vector>


namespace Maconomy {

	class TextImporter : public Importer
	{
	public:
		// Use base class constructor.
		using Importer::Importer;


		// Import time log.
		void import() override;

		// Write updated data to time log.
		void writeToTimeLog() const override;

		// Write to log file.
		void writeToLog() const override;

	private:
		// Sort time lines and return as vector.
		std::vector<const TextEntry*> sortedTimeLines() const;

		// Write to file.
		void writeToFile(const std::string& path,
						 bool toTimeLog) const;
	};

}
