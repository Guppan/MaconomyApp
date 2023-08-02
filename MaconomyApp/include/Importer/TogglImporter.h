#pragma once

#include "Importer.h"
#include "TogglEntry.h"

#include <unordered_map>


namespace Maconomy {

	class TogglImporter : public Importer
	{
	public:
		// Use the base class constructor.
		using Importer::Importer;


		// Import the time log.
		void import() override;
	};

}