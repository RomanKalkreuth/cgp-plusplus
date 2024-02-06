//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: ObjectiveFunctions.h 
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef BENCHMARK_SYMBOLIC_REGRESSION_OBJECTIVEFUNCTIONS_H_
#define BENCHMARK_SYMBOLIC_REGRESSION_OBJECTIVEFUNCTIONS_H_

#include <cmath>

/// @brief Subset of objective functions proposed by McDermott et al. (2012) 
/// @see https://doi.org/10.1145/2330163.2330273
class ObjectiveFunctions {
public:
	ObjectiveFunctions() = delete;

	static float polynomial(int degree, float x) {
		float res = 0.0;

		for (int i = 0; i <= degree; i++) {
			res += pow(x, i);
		}

		return res;
	}

	static float poly2(float* xs) {
		float x = xs[0];
		return x * x + x;
	}

	static float poly3(float* xs) {
		float x = xs[0];
		return x * x * x + x * x + x;
	}

	static float koza1(float* xs) {
		float x = xs[0];
		return x * x * x * x + x * x * x + x * x + x;
	}

	static float koza3(float* xs) {
		float x = xs[0];
		return x * x * x * x * x * x - 2 * x * x * x * x + x * x;
	}

	static float koza2(float* xs) {
		float x = xs[0];
		return x * x * x * x * x - 2 * x * x * x + x;
	}

	static float nguyen3(float* xs) {
		float x = xs[0];
		return x * x * x * x * x + x * x * x * x + x * x * x + x * x + x;
	}

	static float nguyen4(float* xs) {
		float x = xs[0];
		return x * x * x * x * x * x + x * x * x * x * x + x * x * x * x + x * x * x + x * x + x;
	}

	static float nguyen5(float* xs) {
		float x = xs[0];
		return sin(x * x) * cos(x) - 1;
	}

	static float nguyen6(float* xs) {
		float x = xs[0];
		return sin(x) * sin(x + x * x);
	}

	static float nguyen7(float* xs) {
		float x = xs[0];
		return log(x + 1) + log(x * x + 1);
	}

	static float nguyen8(float* xs) {
		float x = xs[0];
		return sqrt(x);
	}

	static float nguyen9(float* xs) {
		float x = xs[0];
		float y = xs[1];
		return sin(x) + sin(y * y);
	}

	static float nguyen10(float* xs) {
		float x = xs[0];
		float y = xs[1];
		return 2.0 * sin(x) * cos(y);
	}

	static float keijzer4(float* xs) {
		float x = xs[0];
		return x * x * x * exp(-1.0 * x) * cos(x) * sin(x) * (sin(x) * sin(x) * cos(x) - 1);
	}

	static float keijzer6(float* xs) {
		float x = xs[0];
		float sum = 0;
		float fx = floor(x);
		int i;
		for (i = 1; i < fx + 1; i++)  // up to and including floor(x)
				{
			sum += (1.0 / i);
		}
		return sum;
	}

	static float korns12(float* xs) {
		float x = xs[0];
		float w = xs[3];
		return 2.0 - (2.1 * (cos(9.8 * x) * sin(1.3 * w)));
	}

	static float vladisleva4(float* xs) {
		float sum = 0;
		int i;
		for (i = 0; i < 5; i++) {
			sum += (xs[i] - 3) * (xs[i] - 3);
		}

		return 10.0 / (5.0 + sum);
	}

	static float vladislavleva6(float* xs) {
		float x = xs[0];
		float y = xs[1];
		return 6.0 * sin(x) * cos(y);
	}

	static float pagie1(float* xs) {
		float x = xs[0];
		float y = xs[1];
		float res = (1.0 / (1.0 + pow(x, -4.0))) + (1.0 / (1.0 + pow(y, -4.0)));
		return res;
	}

	static int get_num_erc(int problem) {
		if (problem == 11 || problem == 12) {
			return 1;
		} else {
			return 2;
		}
	}

};

#endif /* BENCHMARK_SYMBOLIC_REGRESSION_OBJECTIVEFUNCTIONS_H_ */
