#pragma once

#include "Importer.h"

#include <string>
#include <unordered_map>
#include <vector>


namespace Maconomy {

	class TextImporter : public Importer
	{
	public:
		// Use base class constructor.
		using Importer::Importer;

	protected:
		// Import time log.
		void import() override;

		// Convert a time string to hours.
		double toHours(const std::string& time) const override;
	};

}
