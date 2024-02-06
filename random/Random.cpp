//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: Random.cpp
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#include "Random.h"

/// @brief Constructor to set the global seed. 
/// @param p_global_seed global seed
/// @param p_parameters shared pointer to the parameter object 
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

/// @brief Return a random integer drawn from uniform distribution
/// in the clossed interval [a,b]
/// @param a lower bound of the interval
/// @param b higher bound of the interval
/// @return random integer 
int Random::random_integer(int a, int b) {
	std::uniform_int_distribution<> distrib(a, b);
	return distrib(rng);
}


/// @brief Return a random float drawn from uniform distribution
/// in the clossed interval [a,b]
/// @param a lower bound of the interval
/// @param b higher bound of the interval
/// @return random integer number
float Random::random_float(float a, float b) {
	std::uniform_real_distribution<> distrib(a, b);
	return distrib(rng);
}

/// @brief Return a random Boolean value drawn from uniform real distribution
/// @param a probability used to draw the Boolean value by chance
/// @return randon Boolean value
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
