#include "../../include/Importer/RoundStrategy.h"
#include "../../include/Misc/Constants.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <set>
#include <utility>

using namespace Maconomy;


namespace {

	// Typedefs.
	typedef std::vector<double> Vector2D;
	typedef std::pair<size_t, size_t> MatrixSize;

	typedef std::pair<double, size_t> ValueIndex;
	typedef std::function<bool(ValueIndex, ValueIndex)> VIComp;
	typedef std::set<ValueIndex, VIComp> VISet;


	// Calculate the total value of a row in a matrix.
	double rowTotal(const Matrix2D& matrix,
					const size_t row) {
		if (row < 0 || row >= matrix.size()) return -1.0;

		double res{};
		for (const auto val : matrix[row]) res += val;
		return res;
	}


	// Calculate the total value of a column in a matrix.
	double columnTotal(const Matrix2D& matrix,
					   const size_t column) {
		if (column < 0) return -1.0;

		double res{};
		for (const auto& row : matrix) {
			if (column >= row.size()) return -1.0;
			res += row[column];
		}
		return res;
	}


	// Get the size of an mXn matrix.
	MatrixSize matrixSize(const Matrix2D& matrix) {
		MatrixSize res{ 0, 0 };

		res.first = matrix.size();
		if (res.first > 0) res.second = matrix[0].size();
		return res;
	}


	// Round a value to the nearest half-integer.
	double roundValue(const double value) {
		const double sign = std::copysign(1.0, value);

		double res;
		double fraction = std::modf(value, &res);

		if (std::abs(fraction) >= 0.75) res += sign * 1.0;
		else if (std::abs(fraction) >= 0.25) res += sign * 0.5;

		return res;
	}


	// Create a value-index set sorted in ascending order.
	VISet createValueIndexSet(const Matrix2D& matrix) {
		const MatrixSize size = matrixSize(matrix);
		VISet res{ std::less<>{} };
		if (size.second == 0) return res;

		for (size_t col{}; col < size.second; ++col) {
			res.insert({ columnTotal(matrix, col), col });
		}

		return res;
	}


	// Pop the index corresponding to the minimal value in
	// the given value-index set.
	size_t popMinVIIndex(VISet& set) {
		auto it = set.begin();
		const ValueIndex vi = *it;
		set.erase(it);
		return vi.second;
	}


	// Pop the index corresponding to the maximal value in
	// the given value-index set.
	size_t popMaxVIIndex(VISet& set) {
		auto it = std::prev(set.end());
		const ValueIndex vi = *it;
		set.erase(it);
		return vi.second;
	}


	// Adjust matrix up by the given value 'value'.
	void adjustMatrixUp(Matrix2D& matrix,
						VISet& set,
						const double value,
						const size_t row) {
		double adjustVal = roundValue(value);

		while (adjustVal > 0.001) {
			const size_t index = popMinVIIndex(set);

			matrix[row][index] += 0.5;
			set.insert({ columnTotal(matrix, index), index });

			adjustVal -= 0.5;
		}
	}


	// Adjust matrix down by the given value 'value'.
	void adjustMatrixDown(Matrix2D& matrix,
						  VISet& set,
						  const double value,
						  const size_t row) {
		double adjustVal = roundValue(value);

		while (adjustVal < -0.001) {
			const size_t index = popMaxVIIndex(set);
			double& current = matrix[row][index];
			if (current < 0.001) break;

			current -= 0.5;
			set.insert({ columnTotal(matrix, index), index });

			adjustVal += 0.5;
		}
	}


	// Simple rounding strategy.
	// Create a 2D matrix with values rounded to the nereast half-integer.
	Matrix2D simpleRoundStrategy(const Matrix2D& original) {
		const MatrixSize size = matrixSize(original);
		const size_t rowSize = size.first;
		const size_t colSize = size.second;

		Matrix2D res(rowSize, Vector2D(colSize));

		for (size_t row{}; row < rowSize; ++row) {
			for (size_t col{}; col < colSize; ++col) {
				res[row][col] = roundValue(original[row][col]);
			}
		}

		return res;
	}
	

	// Adjust up strategy.
	// Compares the total value of rows between the original and
	// rounded matrix and if necessary, adjusts the value upwards.
	Matrix2D adjustUpStrategy(const Matrix2D& original) {
		const MatrixSize size = matrixSize(original);
		Matrix2D rounded = simpleRoundStrategy(original);
		VISet set = createValueIndexSet(rounded);

		// Iterate each row (task).
		for (size_t row{}; row < size.first; ++row) {
			const double difference = rowTotal(original, row) - rowTotal(rounded, row);
			if (difference < 0.001) continue;

			adjustMatrixUp(rounded, set, difference, row);
		}

		return rounded;
	}


	// Adjust down strategy.
	// Compares the total value of rows between the original and
	// rounded matrix and if necessary, adjust the value downwards.
	Matrix2D adjustDownStrategy(const Matrix2D& original) {
		const MatrixSize size = matrixSize(original);
		Matrix2D rounded = simpleRoundStrategy(original);
		VISet set = createValueIndexSet(rounded);

		// Iterate each row (task).
		for (size_t row{}; row < size.first; ++row) {
			const double difference = rowTotal(original, row) - rowTotal(rounded, row);
			if (difference > -0.001) continue;

			adjustMatrixDown(rounded, set, difference, row);
		}

		return rounded;
	}


	// Adjust strategy.
	// Compares the total value of rows between the original and
	// rounded matrix and if necessary, adjust the value upwards or downwards.
	Matrix2D adjustStrategy(const Matrix2D& original) {
		const MatrixSize size = matrixSize(original);
		Matrix2D rounded = simpleRoundStrategy(original);
		VISet set = createValueIndexSet(rounded);

		// Iterate each row (task).
		for (size_t row{}; row < size.first; ++row) {
			const double difference = rowTotal(original, row) - rowTotal(rounded, row);
			if (difference == 0.0) continue;

			const bool adjustUp = (difference > 0.001);
			if (adjustUp) adjustMatrixUp(rounded, set, difference, row);
			else adjustMatrixDown(rounded, set, difference, row);
		}

		return rounded;
	}

}


// Get a rounding strategy function given a key.
roundFn Maconomy::roundStrategyFn(const std::string& key) {
	//if (key == ROUND_SIMPLE_STRATEGY) return &simpleRoundStrategy;
	//if (key == ROUND_ADJUST_UP_STRATEGY) return &adjustUpStrategy;
	//if (key == ROUND_ADJUST_DOWN_STRATEGY) return &adjustDownStrategy;
	//if (key == ROUND_ADJUST_STRATEGY) return &adjustStrategy;
	return nullptr;
}