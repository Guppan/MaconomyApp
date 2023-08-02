#pragma once

#include "Entry.h"


namespace Maconomy {

	class TogglEntry : public Entry {
	public:
		// Typedefs.
		typedef std::unique_ptr<TogglEntry> ptr;


		// Use base class constructor.
		using Entry::Entry;


		// Split this entry.
		Entry::ptr split() override;
	};
}