#pragma once

#include "CommandOption.h"
#include "../Config/Config.h"


namespace Maconomy {

	class ConfigOption : public CommandOption
	{
	public:
		// Number of mandatory arguments.
		const static int mandatory = 1;

		// Number of optional arguments.
		const static int optional = 0;

	public:
		// Use base class constructor.
		using CommandOption::CommandOption;


		// Validate option arguments.
		bool validate() const override;

		// Spawn other options this option might depend on.
		CommandOption::vector optionSpawner() const override;

		// Execute this option and returns true if the execution can continue.
		bool execute(State& state) override;

	private:
		// Get the config file path.
		const std::string& filePath() const;

		// Get the config to use.
		Config::ptr config() const;
	};

}