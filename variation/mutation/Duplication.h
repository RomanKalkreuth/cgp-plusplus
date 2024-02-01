/*
 * PhenotypicDuplication.h
 *
 *  Created on: 24.01.2023
 *      Author: roman
 */

#ifndef VARIATION_MUTATION_DUPLICATION_H_
#define VARIATION_MUTATION_DUPLICATION_H_

#include "Phenotypic.h"

template<class G, class F>
class Duplication: public Phenotypic<G, F> {
public:
	Duplication(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Random> p_random,
			std::shared_ptr<Species<G>> p_species);
	virtual ~Duplication() = default;

	void variate(std::shared_ptr<Individual<G, F>> individual) override;
};

template<class G, class F>
Duplication<G, F>::Duplication(
		std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Random> p_random, std::shared_ptr<Species<G>> p_species) :
		Phenotypic<G, F>(p_parameters, p_random, p_species) {

	this->name = "Duplication";
	this->rate = this->parameters->get_duplication_rate();
	this->max_depth = this->parameters->get_max_duplication_depth();
}
;

template<class G, class F>
void Duplication<G, F>::variate(
		std::shared_ptr<Individual<G, F>> individual) {

	int num_active_function_nodes = individual->num_active_nodes();

	if (num_active_function_nodes <= 1) {
		return;
	}

	int end;
	int position;
	int node;
	int function;

	std::shared_ptr<std::vector<int>> active_nodes = individual->get_active_nodes();
	std::shared_ptr<G[]> genome = individual->get_genome();

	int depth = this->stochastic_depth(this->max_depth,
			num_active_function_nodes);
	int start = this->start_index(num_active_function_nodes, depth);

	end = start + depth;
	node = active_nodes->at(start);

	position = this->species->position_from_node_number(node);
	function = genome[position];

	for (int i = start + 1; i <= end; i++) {
		node = active_nodes->at(i);
		position = this->species->position_from_node_number(node);
		genome[position] = function;
	}

}

#endif /* VARIATION_MUTATION_DUPLICATION_H_ */
