/*
 * MuPlusLambda.h
 *
 *  Created on: 03.02.2023
 *      Author: roman
 */

#ifndef ALGORITHM_MUPLUSLAMBDA_H_
#define ALGORITHM_MUPLUSLAMBDA_H_

#include "EvolutionaryAlgorithm.h"

template<class E, class G, class F>
class MuPlusLambda: public EvolutionaryAlgorithm<E, G, F> {
private:
	int mu;
	int lambda;

	int select_parent();
	void breed(int num_offspring) override;
public:
	MuPlusLambda(std::shared_ptr<Composite<E, G, F>> p_composite);
	virtual ~MuPlusLambda() = default;

	std::pair<int, F> evolve() override;

};

template<class E, class G, class F>
MuPlusLambda<E, G, F>::MuPlusLambda(
		std::shared_ptr<Composite<E, G, F>> p_composite) :
		EvolutionaryAlgorithm<E, G, F>(p_composite) {
	this->name = "mu-plus-lambda";
	mu = this->parameters->get_mu();
	lambda = this->parameters->get_lambda();
	this->parameters->set_population_size(mu + lambda);
}

template<class E, class G, class F>
int MuPlusLambda<E, G, F>::select_parent() {
	return this->random->random_integer(0, this->mu - 1);
}

template<class E, class G, class F>
void MuPlusLambda<E, G, F>::breed(int num_offspring) {

	for (int i = 0; i < num_offspring; i++) {

		int idx1 = this->select_parent();
		int idx2 = this->select_parent();

		std::shared_ptr<Individual<G, F>> p1 = this->population->get_individual(
				idx1);
		std::shared_ptr<Individual<G, F>> p2 = this->population->get_individual(
				idx2);

		std::shared_ptr<Individual<G, F>> o1 =
				std::make_shared<Individual<G, F>>(p1);

		std::shared_ptr<Individual<G, F>> o2 =
				std::make_shared<Individual<G, F>>(p2);

		this->recombination->crossover(o1, o2);

		this->mutation->mutate(o1);
		o1->set_evaluated(false);

		this->population->set_individual(o1, this->mu + i);
	}
}

template<class E, class G, class F>
std::pair<int, F> MuPlusLambda<E, G, F>::evolve() {

	this->best_fitness = this->fitness->worst_value();
	this->is_ideal = false;

	while (this->generation_number <= this->max_generations && !this->is_ideal) {

		this->evaluate();

		this->fitness_evaluations += this->lambda;

		this->population->sort();

		this->best_fitness = this->population->get_individual(0)->get_fitness();

		this->report(this->generation_number);

		this->check_ideal(this->generation_number);

		this->check_checkpoint();

		this->breed(lambda);

		this->generation_number++;

	}

	return std::pair<int, F> { this->fitness_evaluations, this->best_fitness };
}

#endif /* ALGORITHM_MUPLUSLAMBDA_H_ */
