
#ifdef UNIT_TEST
#define CATCH_CONFIG_MAIN
#include "include/Remote/catch.hpp"

#else
#include "include/CommandOption/Overlord.h"

// Run the Maconomy application.
int main(int argc, char** argv) {
	Maconomy::Overlord overlord{};
	return overlord.run(argc, argv);
}

#endif // UNIT_TEST