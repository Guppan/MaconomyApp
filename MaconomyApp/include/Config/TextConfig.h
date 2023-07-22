#pragma once

#include "Config.h"

namespace Maconomy {

	class TextConfig : public Maconomy::Config
	{
	public:
		// Import config.
		bool import(const std::string& path) override;

	private:
		// Import file paths.
		bool importFilePaths(const std::string& line,
							 const int row,
							 int& pathRow);

		// Import defaults.
		bool importDefaults(const std::string& line,
							const int row,
							int& pathRow);

		// Import definitions.
		bool importDefinitions(const std::string& line,
							   const int row,
							   std::string& localJob,
							   std::string& maconomyJob);
	};

}