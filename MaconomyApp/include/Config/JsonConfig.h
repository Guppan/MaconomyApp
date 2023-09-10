#pragma once

#include "Config.h"
#include "../Remote/json.hpp"


namespace Maconomy {

	class JsonConfig : public Maconomy::Config
	{
	public:
		// Import config.
		bool import(const std::string& path) override;

	private:
		// Import file paths.
		void importFilePaths(nlohmann::json& parsed);

		// Import importer mode.
		void importImporterMode(nlohmann::json& parsed);

		// Import strategies.
		void importStrategies(nlohmann::json& parsed);

		// Import defaults.
		void importDefaults(nlohmann::json& parsed);

		// Import definitions.
		void importDefinitions(nlohmann::json& parsed);
	};

}