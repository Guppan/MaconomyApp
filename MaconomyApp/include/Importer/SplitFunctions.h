#include "Entry.h"


namespace Maconomy {

	// Typedefs.
	typedef Entry::ptr(*splitFn)(Entry*);

	// Get a split function given a key.
	splitFn splitStrategyFn(const std::string& key);
}