/*
 * Random.h
 *
 *  Created on: 13.04.2020
 *      Author: roman
 */

#ifndef RANDOM_RANDOM_H_
#define RANDOM_RANDOM_H_

#include <random>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <memory>

#include "../parameters/Parameters.h"

using namespace std;

class Random {
private:
	long long global_seed;
	std::random_device rd;
	std::mt19937 rng;

	std::shared_ptr<Parameters> parameters;

public:
	Random(long long p_global_seed, std::shared_ptr<Parameters> p_parameters);
	Random(std::shared_ptr<Parameters> p_parameters);
	virtual ~Random() = default;

	int random_integer(int a, int b);
	float random_float(float a, float b);
	bool random_bool(float p = 0.5);

	void set_seed(long long p_global_seed);
	void set_random_seed();
	long long get_global_seed() const;
};



#endif /* RANDOM_RANDOM_H_ */

