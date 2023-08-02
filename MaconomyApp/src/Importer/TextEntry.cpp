#include "../../include/Importer/TextEntry.h"

#include <numeric>
#include <random>


using namespace Maconomy;


// Is the sum of the values an integer?
bool isInteger(const std::vector<double>& values) {
	double fraction;
	const double sum = std::accumulate(values.cbegin(), values.cend(), 0.0);
	return (std::modf(sum, &fraction) == 0.0);
}


// Key.
std::string TextEntry::key() const {
	return processed ? std::to_string(row) : Entry::key();
}


// Can this entry be splitted?
bool TextEntry::canSplit() const {
	return !processed && Entry::canSplit();
}


// Split this entry.
Entry::ptr TextEntry::split() {
	std::unique_ptr<TextEntry> other = std::make_unique<TextEntry>();
	other->description = description;
	other->taskName = taskName;
	other->spec3 = spec3;
	other->jobNumber.push_back(jobNumber.back());
	jobNumber.pop_back();
	other->processed = processed;

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
		other->times[i] = splitTimes[static_cast<int>(1 - assign)];
		if (perfectSplit) assign = 1 - assign;
	}

	return other;
}


// Is this a valid entry for transfer?
bool TextEntry::isValid() const {
	return !processed && valid;
}