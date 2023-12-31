#pragma once

#include "Importer.h"


namespace Maconomy {

	class TogglImporter : public Importer
	{
	public:
		// Use the base class constructor.
		using Importer::Importer;

	protected:
		// Import the time log.
		void import() override;
	};

}