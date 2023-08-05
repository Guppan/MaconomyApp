#include "../../include/Importer/SplitFunctions.h"

#include <numeric>
#include <random>


using namespace Maconomy;


namespace {

	// Is the sum of the values an integer?
	bool isInteger(const std::vector<double>& values) {
		double fraction;
		const double sum = std::accumulate(values.cbegin(), values.cend(), 0.0);
		return (std::modf(sum, &fraction) == 0.0);
	}

}


// Simple split function.
Entry::ptr Maconomy::simpleSplitFn(Entry* entry) {
	std::unique_ptr<Entry> other = entry->splitCopy();

	for (unsigned i{}; i < entry->times.size(); ++i) {
		double& time = entry->times[i];
		if (time < 0.001) continue;

		time /= 2.0;
		other->times[i] = time;
	}

	return other;
}


// Discrete split function.
Entry::ptr Maconomy::discreteSplitFn(Entry* entry) {
	Entry::ptr other = entry->splitCopy();

	// Can times be split perfectly (both jobs get the same amount of time)?
	const bool perfectSplit = isInteger(entry->times);

	int assign{};
	std::vector<double> tmpTimes{ entry->times };
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

		entry->times[i] = splitTimes[assign];
		other->times[i] = splitTimes[static_cast<int>(1 - assign)];
		if (perfectSplit) assign = 1 - assign;
	}

	return other;
}


// Precise split function.
Entry::ptr Maconomy::preciseSplitFn(Entry* entry) {
	// TODO implement this!
	return nullptr;
}