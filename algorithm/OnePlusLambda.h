//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: OnePlusLambda.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef ALGORITHM_ONEPLUSLAMBDA_H_
#define ALGORITHM_ONEPLUSLAMBDA_H_

#include "EvolutionaryAlgorithm.h"
#include <map>
#include <utility>

/// @brief Provides the one+lambda ES adaption that is commonly used in CGP. 

/// @details Uses the neutral genetic drift (NGD) as proposed by Miller. 
/// This adadption is often used as search algorithm since CGP has been predominantly 
/// only with mutation in the past. Considers individuals with equal fitness for selection 
//  if neutral genetic drift is enabled

/// @see Miller, 2019: Cartesian genetic programming: its status and future
/// https://link.springer.com/article/10.1007/s10710-019-09360-6

/// @tparam E Evaluation Type
/// @tparam G Genotype Type
/// @tparam F Fitness Type 
template<class E, class G, class F>
class OnePlusLambda: public EvolutionaryAlgorithm<E, G, F> {
private:
	int lambda;
	int parent_index;

	bool neutral_genetic_drift = true;

	std::multimap<F, int> fitness_map;
	std::shared_ptr<Individual<G, F>> parent;

	int select_parent();
	void create_fitness_map();

	void breed(int num_offspring) override;

public:
	OnePlusLambda(std::shared_ptr<Composite<E, G, F>> p_composite);
	virtual ~OnePlusLambda() = default;

	std::pair<int, F> evolve() override;

};


template<class E, class G, class F>
OnePlusLambda<E, G, F>::OnePlusLambda(
		std::shared_ptr<Composite<E, G, F>> p_composite) :
		EvolutionaryAlgorithm<E, G, F>(p_composite) {

	this->name = "one-plus-lambda";
	lambda = this->parameters->get_lambda();
	this->parameters->set_population_size(1 + lambda);
	neutral_genetic_drift = this->parameters->is_neutral_genetic_drift();
}

/// @brief Maps the position of the individuals and the corresponding fitness.
/// @details Uses std::multimap for the mapping. 
template<class E, class G, class F>
void OnePlusLambda<E, G, F>::create_fitness_map() {

	this->fitness_map.clear();

	F fitness;
	std::shared_ptr<Individual<G, F>> individual;

	for (int i = 0; i < this->population->size(); i++) {
		individual = this->population->get_individual(i);
		fitness = individual->get_fitness();
		this->fitness_map.insert(std::pair<F, int>(fitness, i));
	}
}

/// @brief Selects the parent for the breeding procedure by using elitism
/// and NGD. 
/// @return parent 
template<class E, class G, class F>
int OnePlusLambda<E, G, F>::select_parent() {

	create_fitness_map();

	// Checking if this is the first parent to be selected
	if (this->parent_index == -1) {
		// Setting the best fitness as the first fitness in the map
		this->best_fitness = this->fitness_map.begin()->first;
		// Returning the index of the first individual in the map
		return this->fitness_map.begin()->second;
	}

	bool is_better;
	bool is_equal;

	int rand;

	std::vector<int> better_fitness;
	std::vector<int> equal_fitness;

	// Iterate over the fitness map to categorize individuals based on better or equal fitness
	for (auto it = this->fitness_map.begin(); it != this->fitness_map.end();
			it++) {
		is_better = this->fitness->is_better(it->first, this->best_fitness);
		is_equal = (it->first == this->best_fitness);

		if (is_better) {
			better_fitness.push_back(it->second);
		} else if (is_equal) {
			equal_fitness.push_back(it->second);
		} else {
			break;
		}
	}

	// Select the parent from the individuals with better fitness
	if (better_fitness.size() > 0) {
		rand = this->random->random_integer(0, better_fitness.size() - 1);
		return better_fitness.at(rand);
	// Select the parent from the individuals with equal fitness if neutral genetic drift is enabled
	} else if (this->neutral_genetic_drift && equal_fitness.size() > 0) {
		rand = this->random->random_integer(0, equal_fitness.size() - 1);
		return equal_fitness.at(rand);

	// Return the index of the current parent when no other options are available
	} else {
		return this->parent_index;
	}

}

/// @brief Breed lambda offspring by mutation 
/// @param num_offspring number of offspring 
template<class E, class G, class F>
void OnePlusLambda<E, G, F>::breed(int num_offspring) {

	std::shared_ptr<Individual<G, F>> parent = this->population->get_individual(
			this->parent_index);

	if (this->parent_index != 0) {
		this->population->set_individual(parent, 0);
	}

	std::shared_ptr<G[]> genome_parent = parent->get_genome();

	for (int i = 1; i < this->population->size(); i++) {
		std::shared_ptr<Individual<G, F>> offspring = std::make_shared<
				Individual<G, F>>(parent);
		this->mutation->mutate(offspring);
		offspring->set_evaluated(false);
		this->population->set_individual(offspring, i);
	}
}

/// @brief Evolves the population by means of the one+lambda EA with or without NGD. 
template<class E, class G, class F>
std::pair<int, F> OnePlusLambda<E, G, F>::evolve() {

	this->best_fitness = this->fitness->worst_value();
	this->is_ideal = false;

	if (this->generation_number == 0) {
		this->parent_index = -1;
	}

	while (this->generation_number <= this->max_generations && !this->is_ideal) {

		this->evaluate();

		// Increase the number of fitness evaluations by the number
		// that has been used in the evaluation procedure
		this->fitness_evaluations += this->lambda;

		// Obtain parent with or without considering NGD
		this->parent_index = this->select_parent();

		this->parent = this->population->get_individual(this->parent_index);

		// Obtain best fitness from the sorted population
		this->best_fitness = parent->get_fitness();

		this->report(this->generation_number);

		// Check for ideal fitness
		this->check_ideal(this->generation_number);

		// Check for checkpoint modulo 
		this->check_checkpoint();

		// Breed lambda offspring 
		this->breed(lambda);

		// Reset parent index for the next generation
		this->parent_index = 0;

		this->generation_number++;

	}

	return std::pair<int, F> { this->fitness_evaluations, this->best_fitness };
}

#endif /* ALGORITHM_ONEPLUSLAMBDA_H_ */
