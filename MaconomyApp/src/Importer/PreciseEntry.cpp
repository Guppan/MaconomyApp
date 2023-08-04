#include "../../include/Importer/PreciseEntry.h"


using namespace Maconomy;


// Split this entry.
Entry::ptr PreciseEntry::split() {
	std::unique_ptr<PreciseEntry> other = std::make_unique<PreciseEntry>();
	other->description = description;
	other->taskName = taskName;
	other->spec3 = spec3;
	other->jobNumber.push_back(jobNumber.back());
	jobNumber.pop_back();

	for (unsigned i{}; i < times.size(); ++i) {
		double& time = times[i];
		if (time < 0.001) continue;

		time /= 2.0;
		other->times[i] = time;
	}

	return other;
}
