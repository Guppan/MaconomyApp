#include "../../include/Config/Config.h"


using namespace Maconomy;


// Get value associated with a key from the given map.
const std::string getValue(const std::unordered_map<std::string, std::string>& map,
						   const std::string& key) {
	const auto it = map.find(key);
	if (it != map.cend()) return it->second;
	return std::string{};
}


// Get a job number.
const std::string Config::getJob(const std::string& key) const {
	return getValue(_jobNumberMap, key);
}


// Get a task name.
const std::string Config::getTask(const std::string& key) const {
	return getValue(_taskMap, key);
}


// Get spec3.
const std::string& Config::spec3() const {
	return _spec3;
}