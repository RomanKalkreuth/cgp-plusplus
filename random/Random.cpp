// 					CGP++: Modern C++ Implementation of CGP
// ===============================================================================
//	File
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//  Author(s): Anonymous
//
//	License:
// -===============================================================================

#include "Random.h"

Random::Random(long long p_global_seed, std::shared_ptr<Parameters> p_parameters) {

	if (p_global_seed <= 0) {
		throw std::invalid_argument("Seed must be positive and greater zero!");
	}

	if (p_parameters != nullptr) {
		this->parameters = p_parameters;
	} else {
		throw std::invalid_argument("Nullpointer exception in fitnessclass!");
	}

	this->set_seed(p_global_seed);
}

Random::Random(std::shared_ptr<Parameters> p_parameters) {

	if (p_parameters != nullptr) {
		this->parameters = p_parameters;
	} else {
		throw std::invalid_argument("Nullpointer exception in fitnessclass!");
	}

	this->set_random_seed();
}

int Random::random_integer(int a, int b) {
	std::uniform_int_distribution<> distrib(a, b);
	return distrib(rng);
}

float Random::random_float(float a, float b) {
	std::uniform_real_distribution<> distrib(a, b);
	return distrib(rng);
}

bool Random::random_bool(float p) {
	std::uniform_real_distribution<> distrib(0.0, 1.0);
	return (distrib(rng) < p);
}

void Random::set_seed(long long p_global_seed) {
	this->global_seed = p_global_seed;
	rng.seed(this->global_seed);
}

void Random::set_random_seed() {
	this->global_seed =
			chrono::high_resolution_clock::now().time_since_epoch().count();
	rng.seed(this->global_seed);

	this->parameters->set_global_seed(this->global_seed);
}

long long Random::get_global_seed() const {
	return this->global_seed;
}
