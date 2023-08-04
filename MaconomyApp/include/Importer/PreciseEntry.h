#pragma once

#include "Entry.h"


namespace Maconomy {

	class PreciseEntry : public Entry {
	public:
		// Typedefs.
		typedef std::unique_ptr<PreciseEntry> ptr;


		// Use base class constructor.
		using Entry::Entry;


		// Split this entry.
		Entry::ptr split() override;
	};
}