//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: StaticPopulation.h 
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef POPULATION_STATICPOPULATION_H_
#define POPULATION_STATICPOPULATION_H_

#include <stdexcept>
#include <sstream>
#include <vector>
#include <memory>
#include <cassert>
#include <cstdlib>

#include "../population/AbstractPopulation.h"
#include "../parameters/Parameters.h"
#include "../representation/Individual.h"
#include "../random/Random.h"

/// @brief Represents a static population 
/// @details Static array is used to store the individuals
/// @tparam G Genome Type
/// @tparam F Fitness Type 
template<class G, class F>
class StaticPopulation: public AbstractPopulation<G, F> {
private:
	std::shared_ptr<std::shared_ptr<Individual<G, F>>[]> individuals;

	void init() override;

public:
	StaticPopulation(std::shared_ptr<Random> p_random,
			std::shared_ptr<Parameters> p_parameters);
	virtual ~StaticPopulation() = default;
	void print() override;
	void reset() override;
	int size() override;
	void sort() override;
	void init_from_checkpoint(
			std::shared_ptr<std::vector<std::vector<std::string>>> genomes);
	std::shared_ptr<Individual<G, F> > get_individual(int index) const override;
	std::shared_ptr<std::shared_ptr<Individual<G, F>>[]> init_population_ptr();
	void set_individual(std::shared_ptr<Individual<G, F> > individual,
			int index) override;
};

template<class G, class F>
StaticPopulation<G, F>::StaticPopulation(std::shared_ptr<Random> p_random,
		std::shared_ptr<Parameters> p_parameters) :
		AbstractPopulation<G, F>(p_random, p_parameters) {

	this->individuals = this->init_population_ptr();

	this->init();
}

/// @brief Resets the population by iterating over the individuals and 
/// resetting the genomes 
template<class G, class F>
void StaticPopulation<G, F>::reset() {
	for (int i = 0; i < this->population_size; i++) {
		std::shared_ptr<Individual<G, F>> ind = this->get_individual(i);
		ind->reset();
	}
}

/// @brief Initializes the population by instantiating the individuals
/// that are reference by a shared pointer 
template<class G, class F>
void StaticPopulation<G, F>::init() {
	for (int i = 0; i < this->population_size; i++) {
		std::shared_ptr<Individual<G, F>> ind = std::make_shared<
				Individual<G, F>>(this->random, this->parameters);
		this->individuals[i] = ind;
	}
}

/// @brief Initializes the population and the correspndonding shared pointer that
/// is used for referencing 
/// @return shared pointer to the population instance 
template<class G, class F>
std::shared_ptr<std::shared_ptr<Individual<G, F>>[]> StaticPopulation<G, F>::init_population_ptr() {

	std::shared_ptr<std::shared_ptr<Individual<G, F>>[]> shared_population_ptr(
			new std::shared_ptr<Individual<G, F>>[this->population_size](),
			std::default_delete<std::shared_ptr<Individual<G, F>>[]>());

	//std::shared_ptr<std::shared_ptr<Individual<G, F>>[]> shared_population_ptr =
	//		std::make_shared<std::shared_ptr<Individual<G, F>>[]>(
	//				this->population_size);

	return shared_population_ptr;

}

/// @brief Initializes the population from the genomes provided by a checkpoint. 
/// @param genomes genomes read from the checkpoint 
template<class G, class F>
void StaticPopulation<G, F>::init_from_checkpoint(
		std::shared_ptr<std::vector<std::vector<std::string>>> genomes) {

	bool real_valued = this->individuals[0]->is_real_valued();
	int size = genomes->at(0).size();

	for (int i = 0; i < this->population_size; i++) {

		std::vector<std::string>& genome_vec = genomes->at(i);
		std::shared_ptr<G[]> genome(new G[size](), std::default_delete<G[]>());

		for (int j = 0; j < size; j++) {
			std::string s = genome_vec.at(j);
			if (real_valued) {
				genome[j] = std::stoi(s);
			} else {
				genome[j] = std::stof(s);
			}
		}
		this->individuals[i]->set_genome(genome);
	}
}

/// @brief Sorts the population on this basis of the respective fitness
/// @details Uses lambda a lambda function for the comparison
template<class G, class F>
void StaticPopulation<G, F>::sort() {
std::sort(this->individuals.get(), this->individuals.get() + this->size(),
		[](auto const ind1, auto const ind2) {
			return ind1->get_fitness() < ind2->get_fitness();
		});
}

/// @brief Print out the fitness and genome of the individuals
template<class G, class F>
void StaticPopulation<G, F>::print() {
for (int i = 0; i < this->population_size; i++) {
	std::string genome_str = individuals[i]->to_string();
	std::cout << "Individual #" << i << " :: Fitness: "
			<< individuals[i]->get_fitness() << " :: Genome: " << genome_str
			<< std::endl;
}
}

/// @brief Return the population size 
/// @return number of individuals
template<class G, class F>
int StaticPopulation<G, F>::size() {
return this->population_size;
}

template<class G, class F>
std::shared_ptr<Individual<G, F> > StaticPopulation<G, F>::get_individual(
	int index) const {
assert(index >= 0 && index < this->population_size);
return individuals[index];
}

template<class G, class F>
void StaticPopulation<G, F>::set_individual(
	std::shared_ptr<Individual<G, F> > individual, int index) {
assert(index >= 0 && index < this->population_size);
this->individuals[index] = individual;
}

#endif /* POPULATION_STATICPOPULATION_H_ */
