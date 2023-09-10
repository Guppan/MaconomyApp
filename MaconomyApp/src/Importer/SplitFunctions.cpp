#include "../../include/Importer/SplitFunctions.h"
#include "../../include/Misc/Constants.h"

#include <numeric>
#include <random>


using namespace Maconomy;


namespace {

	// Is the sum of the values an integer?
	bool isInteger(const std::vector<double>& values) {
		double fraction;
		const double sum = std::accumulate(values.cbegin(), values.cend(), 0.0);
		return std::modf(sum, &fraction) < 0.001;
	}


	// Simple split function.
	// Divides the times in two.
	Entry::ptr simpleSplitFn(Entry* entry) {
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
	// Works when the times are in half-hour increments.
	Entry::ptr discreteSplitFn(Entry* entry) {
		Entry::ptr other = entry->splitCopy();

		// Can times be splitted perfectly (both jobs get the same amount of time)?
		const bool perfectSplit = isInteger(entry->times);

		size_t assign{};
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
			other->times[i] = splitTimes[1 - assign];
			if (perfectSplit) assign = 1 - assign;
		}

		return other;
	}

}


// Get a split function given a key.
splitFn Maconomy::splitStrategyFn(const std::string& key) {
	if (key == SPLIT_SIMPLE_STRATEGY) return &simpleSplitFn;
	if (key == SPLIT_DISCRETE_STRATEGY) return &discreteSplitFn;
	return nullptr;
}