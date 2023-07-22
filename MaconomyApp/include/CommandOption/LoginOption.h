#pragma once

#include "CommandOption.h"


namespace Maconomy {

	// Forward declarations.
	class Caller;


	class LoginOption : public CommandOption
	{
	public:
		// Number of mandatory arguments.
		const static int mandatory = 2;

		// Number of optional arguments.
		const static int optional = 0;

	public:
		// Use base class constructor.
		using CommandOption::CommandOption;


		// Validate option arguments.
		bool validate() const override;

		// Execute this option and returns true if the execution can continue.
		bool execute(State& state) override;

	private:
		// Get username.
		const std::string& username() const;

		// Get password.
		const std::string& password() const;
	};

}