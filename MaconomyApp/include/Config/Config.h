#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace Maconomy {

	class Config
	{
    public:
        // Typedefs.
        typedef std::unique_ptr<Config> ptr;

    protected:
        // Job name + task name -> job number map.
        std::unordered_map<std::string, std::string> _jobNumberMap;

        // Job name + task name -> Maconomy task name map.
        std::unordered_map<std::string, std::string> _taskMap;

        // Default Spec3.
        std::string _spec3;

    public:
        // Time log file path.
        std::string timeLogPath;

        // Log file path.
        std::string logPath;

    public:
        // Constructor/Destructor.
        Config() = default;
        virtual ~Config() = default;

        // Import config.
        virtual bool import(const std::string& path) = 0;

        // Get a job number.
        const std::string getJob(const std::string& key) const;

        // Get a task name.
        const std::string getTask(const std::string& key) const;

        // Get spec3.
        const std::string& spec3() const;
	};

}

