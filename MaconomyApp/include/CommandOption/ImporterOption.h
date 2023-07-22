#pragma once

#include "CommandOption.h"


namespace Maconomy {

	// Forward declarations.
	class Config;
	class Importer;


    class ImporterOption : public CommandOption
    {
	public:
		// Number of mandatory arguments.
		const static int mandatory = 0;

		// Number of optional arguments.
		const static int optional = 1;

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
		// Get the current mode.
		const std::string& mode() const;

		// Get the importer.
		std::unique_ptr<Importer> importer(Config* config) const;
    };

}