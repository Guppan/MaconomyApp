#pragma once

#include "CommandOption.h"

namespace Maconomy {

    class LogPathOption : public CommandOption
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

		// Execute this option and returns true if the execution can continue.
		bool execute(State& state) override;
    };

}