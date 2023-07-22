#pragma once

#include "../Caller/Caller.h"
#include "../Config/Config.h"
#include "CommandOption.h"
#include "../Importer/Importer.h"

#include <memory>
#include <sstream>
#include <string>
#include <vector>


namespace Maconomy {

	// Represent the state of the program.
	struct State {
		// Config.
		Config::ptr config;

		// Importer.
		Importer::ptr importer;

		// Caller.
		Caller::ptr caller;
	};


	// Responsible for executing the program.
	class Overlord
	{
	private:
		// State.
		State _state;

		// Options in this state.
		CommandOption::vector _options;

	public:
		//Constructor/Destructor.
		Overlord() = default;
		virtual ~Overlord() = default;


		// Run the program.
		int run(int argc, char** argv);
		
	private:
		// Parse the command line.
		std::vector<OptionParam> parse(int argc, char** argv);

		// Append an option.
		bool append(OptionParam& param);

		// Spawn dependent options.
		void spawn();

		// Sort the option based on their priority.
		void sort();

		// Validate the required options.
		bool validate(std::ostringstream& ss) const;

		// Execute the options in the order of priority.
		bool execute();
	};

}