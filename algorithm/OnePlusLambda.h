/*
 * OnePlusLambda.h
 *
 *  Created on: 05.01.2023
 *      Author: roman
 */

#ifndef ALGORITHM_ONEPLUSLAMBDA_H_
#define ALGORITHM_ONEPLUSLAMBDA_H_

#include "EvolutionaryAlgorithm.h"
#include <map>
#include <utility>

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

	lambda = this->parameters->get_lambda();
	this->parameters->set_population_size(1 + lambda);
	neutral_genetic_drift = this->parameters->is_neutral_genetic_drift();
}

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

template<class E, class G, class F>
int OnePlusLambda<E, G, F>::select_parent() {

	create_fitness_map();

	if (this->parent_index == -1) {
		this->best_fitness = this->fitness_map.begin()->first;
		return this->fitness_map.begin()->second;
	}

	bool is_better;
	bool is_equal;

	int rand;

	std::vector<int> better_fitness;
	std::vector<int> equal_fitness;

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

	if (better_fitness.size() > 0) {
		rand = this->random->random_integer(0, better_fitness.size() - 1);
		return better_fitness.at(rand);
	} else if (this->neutral_genetic_drift && equal_fitness.size() > 0) {
		rand = this->random->random_integer(0, equal_fitness.size() - 1);
		return equal_fitness.at(rand);
	} else {
		return this->parent_index;
	}

}

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

template<class E, class G, class F>
std::pair<int, F> OnePlusLambda<E, G, F>::evolve() {

	this->best_fitness = this->fitness->worst_value();
	this->is_ideal = false;
	this->parent_index = -1;

	while (this->generation_number <= this->max_generations && !this->is_ideal) {

		this->evaluate();

		this->fitness_evaluations += this->lambda;

		this->parent_index = this->select_parent();

		this->parent = this->population->get_individual(this->parent_index);

		this->best_fitness = parent->get_fitness();

		this->report(this->generation_number);

		this->check_ideal(this->generation_number);

		this->check_checkpoint();

		this->breed(lambda);

		this->parent_index = 0;

		this->generation_number++;

	}

	return std::pair<int, F> { this->fitness_evaluations, this->best_fitness };
}

#endif /* ALGORITHM_ONEPLUSLAMBDA_H_ */
