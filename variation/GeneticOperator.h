//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: GeneticOperator.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef VARIATION_GENETICOPERATOR_H_
#define VARIATION_GENETICOPERATOR_H_

#include "../parameters/Parameters.h"
#include "../random/Random.h"
#include "../representation/Species.h"

/// @brief Abstract base class to represent a genetic operator 
/// @tparam G Genome type 
/// @tparam F Fitness type
template<class G, class F>
class GeneticOperator {
protected:
	std::shared_ptr<Parameters> parameters;
	std::shared_ptr<Random> random;
	std::shared_ptr<Species<G>> species;
	std::string name;

public:
	GeneticOperator(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Random> p_random,
			std::shared_ptr<Species<G>> p_species);
	virtual ~GeneticOperator() = default;

	const std::string& get_name() const;
	void set_name(const std::string &name);
};

template<class G, class F>
const std::string& GeneticOperator<G, F>::get_name() const {
	return this->name;
}

template<class G, class F>
void GeneticOperator<G, F>::set_name(const std::string &name) {
	this->name = name;
}

template<class G, class F>
GeneticOperator<G, F>::GeneticOperator(std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Random> p_random,
		std::shared_ptr<Species<G>> p_species) {

	if (p_parameters != nullptr) {
		this->parameters = p_parameters;
	} else {
		throw std::invalid_argument(
				"p_parameters is null in genetic operator class!");
	}

	if (p_random != nullptr) {
		this->random = p_random;
	} else {
		throw std::invalid_argument(
				"p_random is null in genetic operator class!");
	}

	if (p_species != nullptr) {
		this->species = p_species;
	} else {
		throw std::invalid_argument(
				"p_species is null in genetic operator class!");
	}

}

#endif /* VARIATION_GENETICOPERATOR_H_ */
