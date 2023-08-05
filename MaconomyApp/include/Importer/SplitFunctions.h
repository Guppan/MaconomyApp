#include "Entry.h"


namespace Maconomy {

	// Simple split function.
	// Divides times in half.
	Entry::ptr simpleSplitFn(Entry* entry);

	// Discrete split function.
	// Assumes all times are in half-hour increments.
	Entry::ptr discreteSplitFn(Entry* entry);

	// Precise split function.
	// Works on all times.
	Entry::ptr preciseSplitFn(Entry* entry);
}