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

#ifndef VARIATION_SINGLEACTIVEGENEMUTATION_H_
#define VARIATION_SINGLEACTIVEGENEMUTATION_H_

#include "../UnaryOperator.h"

template<class G, class F>
class SingleActiveGene: public UnaryOperator<G, F> {
public:
	SingleActiveGene(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Random> p_random,
			std::shared_ptr<Species<G>> p_species);

	virtual ~SingleActiveGene() = default;

	void variate(std::shared_ptr<Individual<G, F>> individual) override;
};

template <class G, class F>
SingleActiveGene<G, F>::SingleActiveGene(std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Random> p_random,
		std::shared_ptr<Species<G>> p_species) : UnaryOperator<G, F>(p_parameters, p_random, p_species ) {
		this->name = "Single Active Gene";
}

template<class G, class F>
void SingleActiveGene<G, F>::variate(
		std::shared_ptr<Individual<G, F>> individual) {

	std::shared_ptr<std::vector<int>> active_nodes = individual->get_active_nodes();
	std::shared_ptr<G[]> genome =  individual-> get_genome();

	int num_active_nodes = active_nodes->size();

	if (num_active_nodes == 0) {
		return;
	}

	int max_arity = this->parameters->get_max_arity();

	int rand_node_index = this->random->random_integer(0, num_active_nodes - 1);
	int rand_node_number = active_nodes->at(rand_node_index);
	int rand_gene_index = this->random->random_integer(0, max_arity);

	int mutation_pos = this->species->position_from_node_number(
			rand_node_number) + rand_gene_index;

	int min_gene = this->species->min_gene(mutation_pos);
	int max_gene = this->species->max_gene(mutation_pos);

	genome[mutation_pos] = this->random->random_integer(min_gene,
			max_gene);

}

#endif /* VARIATION_SINGLEACTIVEGENEMUTATION_H_ */
