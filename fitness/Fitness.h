//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: Fitness.h 
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================
#ifndef FITNESS_FITNESS_H_
#define FITNESS_FITNESS_H_

#include <type_traits>
#include <stdexcept>
#include <memory>
#include <limits>

#include "../parameters/Parameters.h"

/// @brief Class representing the handling of the fitness. 
/// @details Provides functions to check for better and ideal fitness. 
/// @tparam F Fitness type 
template<class F>
class Fitness {
private:
	bool minimize = true;
	bool strict = true;

	F ideal_fitness;

public:
	Fitness(std::shared_ptr<Parameters> p_parameters, F p_ideal_fitness);
	virtual ~Fitness() = default;

	F worst_value();

	bool is_better(F f1, F f2);
	bool is_ideal(F f);

	bool is_minimize() const;
	void set_minimize(bool p_minimize);

	bool is_strict() const;
	void set_strict(bool strict);

	F get_ideal_fitness() const;
	void set_ideal_fitness(F p_ideal_fitness);
};

template<class F>
Fitness<F>::Fitness(std::shared_ptr<Parameters> p_parameters, F p_ideal_fitness) {

	if (p_parameters != nullptr) {
		this->minimize = p_parameters->is_minimizing_fitness();
		this->ideal_fitness = p_ideal_fitness;
	} else {
		throw std::invalid_argument(
				"Nullpointer exception in fitnessclass!");
	}

	if (std::is_arithmetic<F>::value == false) {
		throw std::invalid_argument(
				"This class supports only arithmetic types!");
	}
}


/// @brief Returns the worst fitness value that is possible in the respective domain
/// @return worst possible fitness value
template<class F>
F Fitness<F>::worst_value() {
	if (this->minimize) {
		return std::numeric_limits<F>::max();
	}
	return std::numeric_limits<F>::min();
}


/// @brief Checks if the first fitness value is better than the second fitness value.
/// @details Distinguishes between strict and non-strict selection. Strict selection 
/// prefers individuals of better fitness while non-strict also considers equal fitness. 
/// @param f1 The first fitness value.
/// @param f2 The second fitness value.
/// @return True if f1 is better than f2, false otherwise.
template<class F>
bool Fitness<F>::is_better(F f1, F f2) {
	if (this->minimize) {
		if (this->strict) {
			return f1 < f2;
		}
		return f1 <= f2;
	} else {
		if (this->strict) {
			return f1 > f2;
		}
		return f1 >= f2;
	}
}

/// @brief Checks for ideal fitness. 
/// @param f fitness to be checked 
/// @return status if fitness is ideal 
template<class F>
bool Fitness<F>::is_ideal(F f) {

	if (minimize) {
		return f <= ideal_fitness;
	} else {
		return f >= ideal_fitness;
	}
}

// Getter and Setter of the fitness class

template<class F>
bool Fitness<F>::is_minimize() const {
	return minimize;
}

template<class F>
void  Fitness<F>::set_minimize(bool p_minimize) {
	this->minimize = p_minimize;
}

template<class F>
bool Fitness<F>::is_strict() const {
	return strict;
}

template<class F>
F Fitness<F>::get_ideal_fitness() const {
	return this->ideal_fitness;
}

template<class F>
void Fitness<F>::set_ideal_fitness(F p_ideal_fitness) {
	this->ideal_fitness = p_ideal_fitness;
}

template<class F>
void Fitness<F>::set_strict(bool strict) {
	this->strict = strict;
}

#endif /* FITNESS_FITNESS_H_ */
