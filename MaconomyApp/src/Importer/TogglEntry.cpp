#include "../../include/Importer/TogglEntry.h"


using namespace Maconomy;


// Split this entry.
Entry::ptr TogglEntry::split() {
	std::unique_ptr<TogglEntry> other = std::make_unique<TogglEntry>();
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
