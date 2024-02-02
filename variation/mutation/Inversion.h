// 					CGP++: Modern C++ Implementation of CGP
// ===============================================================================
//	File
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//  Author(s): Anonymous
//
//	License:
// -===============================================================================

#ifndef VARIATION_MUTATION_INVERSION_H_
#define VARIATION_MUTATION_INVERSION_H_

#include <cmath>

#include "Phenotypic.h"

template<class G, class F>
class Inversion: public Phenotypic<G, F> {
public:
	Inversion(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Random> p_random,
			std::shared_ptr<Species<G>> p_species);
	virtual ~Inversion() = default;

	void variate(std::shared_ptr<Individual<G, F>> individual) override;
};

template<class G, class F>
Inversion<G, F>::Inversion(std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Random> p_random,
		std::shared_ptr<Species<G>> p_species) : Phenotypic<G,F>(p_parameters, p_random, p_species) {

	this->name = "Inversion";

	this->rate = this->parameters->get_inversion_rate();
	this->max_depth = this->parameters->get_max_inversion_depth();

};

template<class G, class F>
void Inversion<G, F>::variate(
		std::shared_ptr<Individual<G, F>> individual) {

	int num_active_function_nodes =  individual->num_active_nodes();

	if (num_active_function_nodes <= 1) {
			return;
	}

	int end;
	int left_node;
	int left_position;
	int right_node;
	int right_position;
	int middle;
	int tmp;

	std::shared_ptr<std::vector<int>> active_nodes =
			individual->get_active_nodes();

	std::shared_ptr<G[]> genome = individual->get_genome();

	int depth = this->stochastic_depth(this->max_depth, num_active_function_nodes);
	int start = this->start_index(num_active_function_nodes,depth);

	end = start + depth;
	middle = round(depth / 2.0);

	for (int i = 0; i < middle; i++) {
		left_node = active_nodes->at(start + i);
		right_node = active_nodes->at(end - i);

		left_position = this->species->position_from_node_number(left_node);
		right_position = this->species->position_from_node_number(right_node);

		tmp = genome[left_position];
		genome[left_position] = genome[right_position];
		genome[right_position] = tmp;
	}

}

#endif /* VARIATION_MUTATION_INVERSION_H_ */
