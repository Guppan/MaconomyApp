#pragma once

#include "Entry.h"


namespace Maconomy {

	class TextEntry : public Entry {
	public:
		// Typedefs.
		typedef std::unique_ptr<TextEntry> ptr;


		// Has this entry been processed?
		bool processed{ false };

		// Row number.
		unsigned row{};

		// Raw input.
		std::vector<std::string> raw;


		// Use base class constructor.
		using Entry::Entry;


		// Key.
		std::string key() const override;

		// Can this entry be splitted?
		bool canSplit() const override;

		// Split this entry.
		Entry::ptr split() override;

		// Is this a valid entry for transfer?
		bool isValid() const override;
	};
}