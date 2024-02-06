//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: Recombination.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================


#ifndef RECOMBINATION_RECOMBINATION_H_
#define RECOMBINATION_RECOMBINATION_H_

#include "../parameters/Parameters.h"
#include "../random/Random.h"
#include "../representation/Species.h"
#include "../representation/Individual.h"

#include "../variation/crossover/BlockCrossover.h"
#include "../variation/crossover/DiscreteCrossover.h"

#include <stdexcept>
#include <memory>


/// @brief Class to represent and handle the recombination procedure.
/// @tparam G Genome type 
/// @tparam F Fitness type
template<class G, class F>
class Recombination {
private:
	std::shared_ptr<Parameters> parameters;
	std::shared_ptr<Random> random;
	std::shared_ptr<Species<G>> species;

	float crossover_rate;
	MUTATION_TYPE crossover_type;

	std::unique_ptr<BlockCrossover<G, F>> block_crossover;
	std::unique_ptr<DiscreteCrossover<G, F>> discrete_crossover;

	std::unique_ptr<BinaryOperator<G,F>> op;

public:
	Recombination(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Random> p_random,
			std::shared_ptr<Species<G>> p_species);
	virtual ~Recombination() = default;
	void crossover(std::shared_ptr<Individual<G, F>> p1,
			std::shared_ptr<Individual<G, F>> p2);
	void print();
};

/// @brief Constructor to instantiate the crossover operator.
/// @details Crossover is chosen based on the setting in the parameter object. 
/// @param p_parameters shared pointer to parameter object
/// @param p_random shared pointer to random generator instance
/// @param p_species shared pointer to species instance 
template<class G, class F>
Recombination<G, F>::Recombination(std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Random> p_random,
		std::shared_ptr<Species<G>> p_species) {

	if (p_parameters != nullptr) {
		this->parameters = p_parameters;
		this->crossover_rate = p_parameters->get_crossover_rate();
		this->crossover_type = p_parameters->get_crossover_type();
	} else {
		throw std::invalid_argument("p_parameters is null in mutation class!");
	}

	if (p_random != nullptr) {
		this->random = p_random;
	} else {
		throw std::invalid_argument("p_random is null in mutation class!");
	}

	if (p_species != nullptr) {
		this->species = p_species;
	} else {
		throw std::invalid_argument("p_species is null in mutation class!");
	}


	if (crossover_type == this->parameters->BLOCK_CROSSOVER) {
		this->op = std::make_unique<BlockCrossover<G, F>>(p_parameters,
				p_random, p_species);
	} else if (crossover_type == this->parameters->DISCRETE_CROSSOVER) {
		this->op = std::make_unique<DiscreteCrossover<G, F>>(p_parameters,
				p_random, p_species);
	}
}

/// @brief Trigger the recombination procedure. 
/// @param p1 first parent 
/// @param p2 second parent
template<class G, class F>
void Recombination<G, F>::crossover(std::shared_ptr<Individual<G, F>> p1,
		std::shared_ptr<Individual<G, F>> p2) {
	this->op->variate(p1, p2);
}

/// @brief Print the recobination name. 
template<class G, class F>
void Recombination<G, F>::print() {
	std::cout<<"Recombination: ";
	if(this->op != nullptr) {
		std::cout<<this->op->get_name()<<std::endl;
	}
	else {
		std::cout<<"None"<<std::endl;
	}
}

#endif /* RECOMBINATION_RECOMBINATION_H_ */
