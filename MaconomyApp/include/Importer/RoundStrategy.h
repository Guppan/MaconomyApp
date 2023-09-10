#pragma once

#include <string>
#include <vector>


namespace Maconomy {

	// Typedefs.
	typedef std::vector<std::vector<double>> Matrix2D;
	typedef Matrix2D(*roundFn)(const Matrix2D&);


	// Get a rounding strategy function given a key.
	// The rounding strategy function is used to perform
	// a rounding of the entries times.
	roundFn roundStrategyFn(const std::string& key);
}