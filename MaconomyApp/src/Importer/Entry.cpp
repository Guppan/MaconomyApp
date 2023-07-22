#include "../../include/Importer/Entry.h"

#include <numeric>
#include <random>
#include <sstream>


using namespace Maconomy;


// Is the sum of the values an integer?
bool isInteger(const std::vector<double>& values) {
	double fraction;
	const double sum = std::accumulate(values.cbegin(), values.cend(), 0.0);
	return (std::modf(sum, &fraction) == 0.0);
}

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
void Entry::merge(Entry& other) {
	for (unsigned i{}; i < other.times.size(); ++i) {
		const double time = other.times[i];
		if (time < 0.001) continue;

		times[i] += time;
	}
}


// Can this entry be splitted?
bool Entry::canSplit() const {
	return jobNumber.size() > 1;
}


// Split this entry. Only supporting 2-split.
Entry Entry::split() {
	Entry other{};
	other.description = description;
	other.taskName = taskName;
	other.spec3 = spec3;
	other.jobNumber.push_back(jobNumber.back());
	jobNumber.pop_back();

	// Can times be split perfectly (both jobs get the same amount of time)?
	const bool perfectSplit = isInteger(times);

	int assign{};
	std::vector<double> tmpTimes{ times };
	for (unsigned i{}; i < tmpTimes.size(); ++i) {
		const double time{ tmpTimes[i] };
		if (time < 0.001) continue;

		double integer;
		const double fraction = std::modf(time, &integer);

		const std::vector<double> splitTimes{
			integer / 2.0 + fraction,
			integer / 2.0
		};

		if (!perfectSplit) {
			// Add some randomness to distribute time between jobs better.
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dist(0, 1);
			assign = dist(gen);
		}

		times[i] = splitTimes[assign];
		other.times[i] = splitTimes[static_cast<int>(1 - assign)];
		if (perfectSplit) assign = 1 - assign;
	}

	return other;
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