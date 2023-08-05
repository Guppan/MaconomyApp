#pragma once

#include "Importer.h"


namespace Maconomy {

	class OptimizeImporter : public Importer
	{
	public:
		// Use the base class constructor.
		using Importer::Importer;


		// Import the time log.
		void import() override;

	protected:
		// Split function for this importer.
		Entry::SplitFn splitFunction() const;

		// Convert a time string to hours.
		double toHours(const std::string& time) const override;

	private:
		// Set data (job, task, description) on the entry. Returns true on success.
		bool setData(const std::string& line,
					 Entry* entry) const;

		// Set times on the entry. Returns true on success.
		bool setTimes(std::ifstream& file,
					  Entry* entry) const;

		// Validate an entry.
		bool validateEntry(const Entry* entry) const;
	};

}