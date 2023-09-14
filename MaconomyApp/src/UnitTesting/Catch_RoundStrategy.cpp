
#ifdef UNIT_TEST

#include "../../include/Remote/catch.hpp"
#include "../../include/UnitTesting/Catch_UnitTesting.h"

#include "../../include/Importer/RoundStrategy.h"
#include "../../include/Misc/Constants.h"

#include <iostream>
#include <initializer_list>
#include <sstream>
#include <utility>


using namespace Maconomy;
using namespace UnitTest;


namespace {

	// Create a matrix.
	Matrix2D matrix(std::initializer_list<std::vector<double>> vecs) {
		Matrix2D res{};
		for (auto&& vec : vecs) res.push_back(vec);
		return res;
	}


	// Output matrix.
	std::string outputMatrix(const Matrix2D& matrix) {
		std::ostringstream ss{};
		for (const auto& row : matrix) {
			for (const auto& col : row) {
				ss << col << " ";
			}
			ss << '\n';
		}
		return ss.str();
	}


	// Compare a matrix against a reference matrix.
	void compareMatrices(const Matrix2D& ref,
						 const Matrix2D& matrix) {
		const size_t refRowSize = ref.size();
		const size_t matRowSize = matrix.size();

		// Require equal number of rows.
		REQUIRE(refRowSize == matRowSize);
		if (refRowSize == 0) return;

		// Require equally sized rows.
		const size_t refColSize = ref[0].size();
		for (const auto& vec : ref) REQUIRE(vec.size() == refColSize);

		const size_t matColSize = matrix[0].size();
		for (const auto& vec : matrix) REQUIRE(vec.size() == matColSize);

		// Require equal number of columns.
		REQUIRE(refColSize == matColSize);
		if (refColSize == 0) return;

		// Require same values in matrix and reference.
		std::ostringstream ss{};
		ss << "Matrix failed, <row, col>: ";
		for (size_t row{}; row < refRowSize; ++row) {
			for (size_t col{}; col < refColSize; ++col) {
				std::ostringstream ss2{};
				ss2 << "<" << std::to_string(row) << ", ";
				ss2 << std::to_string(col) << ">\n";

				INFO(ss.str() + ss2.str() << outputMatrix(matrix));
				REQUIRE(std::abs(ref[row][col] - matrix[row][col]) < 0.001);
			}
		}
	}


	// Apply strategy to matrix and test against reference matrix.
	void testMatrix(roundFn strategy,
					const Matrix2D& ref,
					const Matrix2D& matrix) {
		REQUIRE(strategy != nullptr);
		compareMatrices(ref, strategy(matrix));
	}

}


TEST_CASE("Testing rounding strategies.", "[RoundStrategy]") {
	
	roundFn strategy = nullptr;

	// ===== Simple strategy ==========================
	SECTION("Simple rounding - single row matrix, half-integer") {
		strategy = roundStrategyFn(ROUND_SIMPLE_STRATEGY);

		Matrix2D m = matrix({ {0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0} });

		testMatrix(strategy, m, m);
	}

	SECTION("Simple rounding - multiple row matrix, half integer") {
		strategy = roundStrategyFn(ROUND_SIMPLE_STRATEGY);

		Matrix2D m = matrix({
			{0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0},
			{3.0, 2.5, 2.0, 1.5, 1.0, 0.5, 0.0},
			{0.0, 3.0, 0.5, 2.5, 1.0, 2.0, 1.5},
			{1.5, 0.5, 2.5, 1.0, 0.0, 2.0, 3.0} });

		testMatrix(strategy, m, m);
	}

	SECTION("Simple rounding - single row matrix, real values") {
		strategy = roundStrategyFn(ROUND_SIMPLE_STRATEGY);

		Matrix2D ref = matrix({ {0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0} });
		Matrix2D m = matrix({ {0.24, 0.25, 0.75, 1.74, 1.92, 2.30, 2.88} });

		testMatrix(strategy, ref, m);
	}
	
	SECTION("Simple rounding - multiple row matrix, real values") {
		strategy = roundStrategyFn(ROUND_SIMPLE_STRATEGY);

		Matrix2D ref = matrix({
			{0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0},
			{3.0, 2.5, 2.0, 1.5, 1.0, 0.5, 0.0},
			{0.0, 3.0, 0.5, 2.5, 1.0, 2.0, 1.5},
			{1.5, 0.5, 2.5, 1.0, 0.0, 2.0, 3.0} });

		Matrix2D m = matrix({
			{0.24, 0.25, 0.75, 1.74, 1.92, 2.30, 2.88},
			{3.12, 2.52, 2.23, 1.64, 1.19, 0.55, 0.01},
			{0.09, 3.24, 0.48, 2.65, 0.99, 1.77, 1.51},
			{1.58, 0.59, 2.53, 1.07, 0.01, 2.08, 3.05} });

		testMatrix(strategy, ref, m);
	}

	// ===== Adjust up strategy =======================
	SECTION("Adjust up rounding - single row matrix, half-integer") {
		strategy = roundStrategyFn(ROUND_ADJUST_UP_STRATEGY);

		Matrix2D m = matrix({ {0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0} });

		testMatrix(strategy, m, m);
	}

	SECTION("Adjust up rounding - multiple row matrix, half-integer") {
		strategy = roundStrategyFn(ROUND_ADJUST_UP_STRATEGY);

		Matrix2D m = matrix({
			{0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0},
			{3.0, 2.5, 2.0, 1.5, 1.0, 0.5, 0.0},
			{0.0, 3.0, 0.5, 2.5, 1.0, 2.0, 1.5},
			{1.5, 0.5, 2.5, 1.0, 0.0, 2.0, 3.0} });

		testMatrix(strategy, m, m);
	}

	SECTION("Adjust up rounding - single row matrix, real values - 1") {
		strategy = roundStrategyFn(ROUND_ADJUST_UP_STRATEGY);

		Matrix2D ref = matrix({ {0.5, 0.5, 0.0, 0.0, 0.0} });
		Matrix2D m = matrix({ {0.24, 0.24, 0.24, 0.24, 0.24} });
		
		testMatrix(strategy, ref, m);
	}
	
	SECTION("Adjust up rounding - single row matrix, real values - 2") {
		strategy = roundStrategyFn(ROUND_ADJUST_UP_STRATEGY);

		Matrix2D ref = matrix({ {0.5, 0.0, 0.0, 0.0, 0.0} });
		Matrix2D m = matrix({ {0.05, 0.05, 0.05, 0.05, 0.05} });

		testMatrix(strategy, ref, m);
	}

	SECTION("Adjust up rounding - single row matrix, real values - 3") {
		strategy = roundStrategyFn(ROUND_ADJUST_UP_STRATEGY);

		Matrix2D ref = matrix({ {0.0, 0.0, 0.0, 0.0, 0.0} });
		Matrix2D m = matrix({ {0.05, 0.05, 0.05, 0.05, 0.04} });

		testMatrix(strategy, ref, m);
	}

	SECTION("Adjust up rounding - double row matrix, real values - 1") {
		strategy = roundStrategyFn(ROUND_ADJUST_UP_STRATEGY);

		Matrix2D ref = matrix({
			{0.5, 0.5, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.5, 0.5, 0.0} });

		Matrix2D m = matrix({
			{0.24, 0.24, 0.24, 0.24, 0.24},
			{0.24, 0.24, 0.24, 0.24, 0.24} });

		testMatrix(strategy, ref, m);
	}
	
	SECTION("Adjust up rounding - double row matrix, real values - 2") {
		strategy = roundStrategyFn(ROUND_ADJUST_UP_STRATEGY);

		Matrix2D ref = matrix({
			{0.5, 0.0, 0.0, 0.0, 0.0},
			{0.0, 0.5, 0.0, 0.0, 0.0} });

		Matrix2D m = matrix({
			{0.05, 0.05, 0.05, 0.05, 0.05},
			{0.05, 0.05, 0.05, 0.05, 0.05} });

		testMatrix(strategy, ref, m);
	}

	SECTION("Adjust up rounding - double row matrix, real values - 3") {
		strategy = roundStrategyFn(ROUND_ADJUST_UP_STRATEGY);

		Matrix2D ref = matrix({
			{0.0, 0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0, 0.0} });

		Matrix2D m = matrix({
			{0.05, 0.05, 0.05, 0.05, 0.04},
			{0.05, 0.05, 0.05, 0.05, 0.04} });

		testMatrix(strategy, ref, m);
	}

	SECTION("Adjust up rounding - wrapping row matrix, real values - 1") {
		strategy = roundStrategyFn(ROUND_ADJUST_UP_STRATEGY);

		Matrix2D ref = matrix({
			{0.5, 0.5, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.5, 0.5, 0.0},
			{0.5, 0.0, 0.0, 0.0, 0.5},
			{0.0, 0.5, 0.5, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.5, 0.5},
			{0.5, 0.5, 0.0, 0.0, 0.0} });

		Matrix2D m = matrix({
			{0.24, 0.24, 0.24, 0.24, 0.24},
			{0.24, 0.24, 0.24, 0.24, 0.24},
			{0.24, 0.24, 0.24, 0.24, 0.24},
			{0.24, 0.24, 0.24, 0.24, 0.24},
			{0.24, 0.24, 0.24, 0.24, 0.24},
			{0.24, 0.24, 0.24, 0.24, 0.24} });

		testMatrix(strategy, ref, m);
	}
	
	SECTION("Adjust up rounding - wrapping row matrix, real values - 2") {
		strategy = roundStrategyFn(ROUND_ADJUST_UP_STRATEGY);

		Matrix2D ref = matrix({
			{0.5, 0.0, 0.0, 0.0, 0.0},
			{0.0, 0.5, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.5, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.5, 0.0},
			{0.0, 0.0, 0.0, 0.0, 0.5},
			{0.5, 0.0, 0.0, 0.0, 0.0} });

		Matrix2D m = matrix({
			{0.05, 0.05, 0.05, 0.05, 0.05},
			{0.05, 0.05, 0.05, 0.05, 0.05},
			{0.05, 0.05, 0.05, 0.05, 0.05},
			{0.05, 0.05, 0.05, 0.05, 0.05},
			{0.05, 0.05, 0.05, 0.05, 0.05},
			{0.05, 0.05, 0.05, 0.05, 0.05} });

		testMatrix(strategy, ref, m);
	}

	SECTION("Adjust up rounding - wrapping row matrix, real values - 3") {
		strategy = roundStrategyFn(ROUND_ADJUST_UP_STRATEGY);

		Matrix2D ref = matrix({
			{0.0, 0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0, 0.0},
			{0.0, 0.0, 0.0, 0.0, 0.0} });

		Matrix2D m = matrix({
			{0.05, 0.05, 0.05, 0.05, 0.04},
			{0.05, 0.05, 0.05, 0.05, 0.04},
			{0.05, 0.05, 0.05, 0.05, 0.04},
			{0.05, 0.05, 0.05, 0.05, 0.04},
			{0.05, 0.05, 0.05, 0.05, 0.04},
			{0.05, 0.05, 0.05, 0.05, 0.04} });

		testMatrix(strategy, ref, m);
	}
	
	SECTION("Adjust up rounding - multiple row matrix, real values - 1") {
		strategy = roundStrategyFn(ROUND_ADJUST_UP_STRATEGY);

		Matrix2D ref = matrix({
			{0.0, 1.5, 1.0, 4.0, 8.5},
			{5.5, 3.5, 2.0, 9.5, 4.0},
			{3.5, 3.5, 5.5, 1.0, 5.0} });

		Matrix2D m = matrix({
			{0.24, 0.94, 1.20, 4.23, 8.37},
			{5.24, 3.01, 2.18, 9.74, 4.20},
			{3.05, 3.14, 5.70, 1.20, 5.22} });

		testMatrix(strategy, ref, m);
	}
	
	
	// ===== Adjust down strategy =====================
	SECTION("Adjust down rounding - single row matrix, half-integer") {
		strategy = roundStrategyFn(ROUND_ADJUST_DOWN_STRATEGY);

		Matrix2D m = matrix({ {0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0} });

		testMatrix(strategy, m, m);
	}
	
	SECTION("Adjust down rounding - multiple row matrix, half-integer") {
		strategy = roundStrategyFn(ROUND_ADJUST_DOWN_STRATEGY);

		Matrix2D m = matrix({
			{0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0},
			{3.0, 2.5, 2.0, 1.5, 1.0, 0.5, 0.0},
			{0.0, 3.0, 0.5, 2.5, 1.0, 2.0, 1.5},
			{1.5, 0.5, 2.5, 1.0, 0.0, 2.0, 3.0} });

		testMatrix(strategy, m, m);
	}
	
	SECTION("Adjust down rounding - single row matrix, real values - 1") {
		strategy = roundStrategyFn(ROUND_ADJUST_DOWN_STRATEGY);

		Matrix2D ref = matrix({ {0.5, 0.5, 0.0, 0.0, 0.0} });
		Matrix2D m = matrix({ {0.25, 0.25, 0.25, 0.25, 0.25} }); // 1.5

		testMatrix(strategy, ref, m);
	}
	
	SECTION("Adjust down rounding - single row matrix, real values - 2") {
		strategy = roundStrategyFn(ROUND_ADJUST_DOWN_STRATEGY);

		Matrix2D ref = matrix({ {1.0, 1.0, 0.5, 0.5, 0.5} });
		Matrix2D m = matrix({ {0.75, 0.75, 0.75, 0.75, 0.75} });

		testMatrix(strategy, ref, m);
	}
	
	SECTION("Adjust down rounding - double row matrix, real values - 1") {
		strategy = roundStrategyFn(ROUND_ADJUST_DOWN_STRATEGY);
		Matrix2D ref = matrix({
			{0.5, 0.5, 0.0, 0.0, 0.0},
			{0.5, 0.5, 1.0, 1.0, 0.5} });

		Matrix2D m = matrix({
			{0.25, 0.25, 0.25, 0.25, 0.25},
			{0.75, 0.75, 0.75, 0.75, 0.75} });

		testMatrix(strategy, ref, m);
	}
}

#endif // UNIT_TEST