#include "../../include/Importer/Entry.h"

#include <sstream>


using namespace Maconomy;


//Constructor.
Entry::Entry() {
	times = std::vector<double>(5);
	valid = true;
}


// Key.
std::string Entry::key() const {
	std::ostringstream ss{};

	ss << description;
	for (const auto& job : jobNumber) ss << ";" << job;
	ss << ";" << taskName;

	return ss.str();
}


// Merge another entry with this.
void Entry::merge(Entry* other) {
	for (unsigned i{}; i < other->times.size(); ++i) {
		const double time = other->times[i];
		if (time < 0.001) continue;

		times[i] += time;
	}
}


// Can this entry be splitted?
bool Entry::canSplit() const {
	return jobNumber.size() > 1;
}


// Is this a valid entry for transfer?
bool Entry::isValid() const {
	return valid;
}


// Convert to Json.
std::string Entry::toJson() const {
	std::string res;

	res += "{\"data\":{";
	res += "\"jobnumber\":\"" + jobNumber.back() + "\"";
	res += ",\"taskname\":\"" + taskName + "\"";
	res += ",\"specification3name\":\"" + spec3 + "\"";

	for (unsigned i{}; i < times.size(); ++i) {
		if (times[i] < 0.001) continue;

		const std::string day = std::to_string(i + 1);

		res += ",\"numberday" + day + "\":" + std::to_string(times[i]);
		res += ",\"descriptionday" + day + "\":\"" + description + "\"";
	}

	res += "}}";

	return res;
}