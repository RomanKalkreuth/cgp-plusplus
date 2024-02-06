//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: DynamicPopulation.h 
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef POPULATION_DYNAMICPOPULATION_H_
#define POPULATION_DYNAMICPOPULATION_H_

#include <stdexcept>
#include <vector>
#include <memory>
#include <cassert>


#include "../parameters/Parameters.h"
#include "../representation/Individual.h"
#include "../random/Random.h"
#include "AbstractPopulation.h"

/// @brief Concept for a dynamic class that can vary in size. 
/// @details Unfinished work and currently not supported
/// @tparam G Genome Type
/// @tparam F Fitness Type 
template <class G, class F>
class DynamicPopulation : public AbstractPopulation<G,F> {
private:
	int population_size;
	std::vector<std::shared_ptr<Individual<G, F>>> individuals;
	std::shared_ptr<Parameters> parameters;
	std::shared_ptr<Random> random;

	void init() override;

public:
	DynamicPopulation(std::shared_ptr<Random> p_random,
			std::shared_ptr<Parameters> p_parameters);
	virtual ~DynamicPopulation();
	void print() override;
	void clear() override;
	void reset() override;
	void add(std::shared_ptr<Individual<G, F>> individual);
	const std::vector<std::shared_ptr<Individual<G,F> > >& get_individuals() const;
	std::shared_ptr<Individual<G, F> > get_individual(int index) const override;
	void set_individual(std::shared_ptr<Individual<G, F> > individual, int index)  override;
};

template <class G, class F>
DynamicPopulation<G, F>::DynamicPopulation(std::shared_ptr<Random> p_random,
		std::shared_ptr<Parameters> p_parameters) : AbstractPopulation<G, F>( p_random,p_parameters) {

	this->init();
}

template <class G, class F>
DynamicPopulation<G, F>::~DynamicPopulation() {
	this->clear();
}


template <class G, class F>
void DynamicPopulation<G, F>::reset() {
	this->clear();
	this->init();
}

template <class G, class F>
void DynamicPopulation<G, F>::init() {
	for (int i = 0; i < this->population_size; i++) {
		std::shared_ptr<Individual<G,F>> ind = std::make_unique<Individual<G,F>>(random,parameters);
		this->individuals.push_back(ind);
	}
}

template <class G, class F>
void DynamicPopulation<G, F>::print() {
	int i=0;
	for (auto it = individuals.begin(); it != individuals.end(); ++it) {

		std::string genome_str = (*it)->to_string();
		std::cout<<"Individual #"<<i<<" :: Fitness: "<<(*it)->get_fitness()<<" :: Genome: "<<genome_str<<std::endl;

		i++;
	}
}

template <class G, class F>
void DynamicPopulation<G, F>::clear() {
	this->individuals.clear();
}

template <class G, class F>
void DynamicPopulation<G, F>::add(std::shared_ptr<Individual<G, F>> individual) {
	assert(individual != nullptr);
	this->individuals.push_back(individual);
}


template <class G, class F>
std::shared_ptr<Individual<G, F> > DynamicPopulation<G, F>::get_individual(int index) const {
	return individuals.at(index);
}

template <class G, class F>
const std::vector<std::shared_ptr<Individual<G, F> > >& DynamicPopulation<G, F>::get_individuals() const {
	return individuals;
}

template<class G, class F>
void DynamicPopulation<G, F>::set_individual(std::shared_ptr<Individual<G, F> > individual, int index) {
	assert(index >= 0 && index < this->population_size);
	this->individuals[index] = individual;
}

#endif /* POPULATION_DYNAMICPOPULATION_H_ */
