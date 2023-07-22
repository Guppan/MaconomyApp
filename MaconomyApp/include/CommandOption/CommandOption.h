#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <vector>


namespace Maconomy {

	// Forward declarations.
	struct State;


	// Parameters for an option.
	struct OptionParam {
		// Key of the option.
		std::string key{};

		// Arguments of the option.
		std::vector<std::string> args{};

		// Internal construction (not available from the terminal)?
		bool internal = false;
	};


	class CommandOption
	{
	public:
		// Typedefs.
		typedef std::unique_ptr<CommandOption> ptr;
		typedef std::vector<ptr> vector;

	protected:
		// Arguments of this option.
		std::vector<std::string> _args;

		// Option id.
		std::string _id;

		// Execution priority.
		int _priority;

		// Required options for this options.
		std::vector<std::string> _required;

	public:
		// Constructor/Destructor.
		CommandOption(const std::string& id,
					  const int priority,
					  std::vector<std::string>&& required);
		virtual ~CommandOption() = default;


		// Option id.
		const std::string& id() const;

		// Execute priority for this option. A higher value equals higher priority.
		int priority() const;

		// Required options for this option.
		const std::vector<std::string>& required() const;

		// Set arguments.
		void set(std::vector<std::string>&& args);

		// Can this option be appended to the list of options?
		virtual bool canAppend(const CommandOption::vector& options) const;

		// Validate option arguments.
		virtual bool validate() const = 0;

		// Spawn other options this option might depend on.
		virtual CommandOption::vector optionSpawner() const { return {}; }

		// Execute this option and returns true if the execution can continue.
		virtual bool execute(State& state) = 0;

		// This option's usage text (displayed in terminal).
		void usage(std::ostringstream& ss) const;
	};

	// Command option factory.
	CommandOption::ptr optionFactory(OptionParam& param);
}