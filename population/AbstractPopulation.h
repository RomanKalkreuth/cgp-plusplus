//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: AbstractPopulation.h 
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef POPULATION_ABSTRACTPOPULATION_H_
#define POPULATION_ABSTRACTPOPULATION_H_

#include <memory>

#include "../parameters/Parameters.h"
#include "../representation/Individual.h"
#include "../random/Random.h"

/// @brief Abstract base class to represent static and dynamic populations.
/// @details Provides core functions for the handling of the invidiuals and initiliaziation from checkspoints.
/// @tparam G Genome Type
/// @tparam F Fitness Type 
template<class G, class F>
class AbstractPopulation {
protected:
	int population_size;

	std::shared_ptr<Parameters> parameters;
	std::shared_ptr<Random> random;

	virtual void init() = 0;

public:
	AbstractPopulation(std::shared_ptr<Random> p_random,
			std::shared_ptr<Parameters> p_parameters);

	virtual ~AbstractPopulation() = default;

	virtual void print() = 0;
	virtual void reset() = 0;
	virtual int size() = 0;
	virtual void sort() = 0;

	virtual void init_from_checkpoint(std::shared_ptr<std::vector<std::vector<std::string>>> genomes) = 0;

	virtual std::shared_ptr<Individual<G, F> > get_individual(int index) const = 0;
	virtual void set_individual(std::shared_ptr<Individual<G, F> > individual, int index) = 0;

};

template<class G, class F>
AbstractPopulation<G, F>::AbstractPopulation(std::shared_ptr<Random> p_random,
		std::shared_ptr<Parameters> p_parameters) {

	if (p_parameters != nullptr || p_random != nullptr) {
		parameters = p_parameters;
		random = p_random;
		population_size = p_parameters->get_population_size();
	} else {
		throw std::invalid_argument("Nullpointer exception in abstract population class!");
	}
}

#endif /* POPULATION_ABSTRACTPOPULATION_H_ */
