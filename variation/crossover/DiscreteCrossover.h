//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: DiscreteCrossover.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================


#ifndef VARIATION_CROSSOVER_DISCRETECROSSOVER_H_
#define VARIATION_CROSSOVER_DISCRETECROSSOVER_H_

#include <cmath>

#include "../BinaryOperator.h"

/// @brief Class for the representation of discrete recombination that has been proposed for CGP. 
/// @details Adaption of discrete/uniform recombination in CGP. This is a phenotypic variation 
/// method for discrete recombination in CGP.  It adapts discrete recombination in CGP by means of 
/// phenotypic functional variation which is performed through the exchange of function genes of active function nodes.
/// Reasonable results were obtained on several symbolic regression benchmarks (Kalkreuth (2021))
/// @see Kalkreuth, R. (2022). Towards Discrete Phenotypic Recombination in Cartesian Genetic Programming.
/// https://doi.org/10.1007/978-3-031-14721-0_5
/// @see Kalkreuth R. (2021). Reconsideration and extension of Cartesian genetic programming
/// http://dx.doi.org/10.17877/DE290R-22504
/// @tparam G Genome type 
/// @tparam F Fitness type
template<class G, class F>
class DiscreteCrossover: public BinaryOperator<G, F> {
public:
	DiscreteCrossover(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Random> p_random,
			std::shared_ptr<Species<G>> p_species) :
			BinaryOperator<G, F>(p_parameters, p_random, p_species) {
		this->name = "Discrete Crossover";
	}

	virtual ~DiscreteCrossover() = default;

	void variate(std::shared_ptr<Individual<G, F>> p1,
			std::shared_ptr<Individual<G, F>> p2) override;
};

template<class G, class F>
void DiscreteCrossover<G, F>::variate(std::shared_ptr<Individual<G, F>> p1,
		std::shared_ptr<Individual<G, F>> p2) {

	if (p1->num_active_nodes() == 0 || p2->num_active_nodes() == 0) {
		return;
	}

	int tmp = 0;
	int num_inputs = this->parameters->get_num_inputs();
	int max_arity = this->parameters->get_max_arity();

	// Boundary extension is activated by default
	bool boundary_extension = true;

	std::shared_ptr<G[]> g1 = p1->get_genome();
	std::shared_ptr<G[]> g2 = p2->get_genome();

	// Node numbers are stored if two nodes are selected for the swap of the function gene
	int swap_node1 = 0;
	int swap_node2 = 0;

	// Indices for the function genes
	int index1 = 0;
	int index2 = 0;

	// Determine the phenotypic length
	std::shared_ptr<std::vector<int>> active_nodes1 = p1->get_active_nodes();
	std::shared_ptr<std::vector<int>> active_nodes2 = p2->get_active_nodes();

	// Determine the phenotypic length
	int len1 = active_nodes1->size();
	int len2 = active_nodes2->size();

	// check that the chromosomes are equally long (true in most cases)
	int min = std::min(len1, len2);
	int max = std::max(len1, len2);

	// Iterate over the minimum phenotypic length
	for (int x = 0; x < min; x++) {

		// Decide uniformly at random whether a gene swap will be performed or not
		if (this->random->random_bool()) {

			// If boundary extension is activated, we select a swap node for
			// the phenotypically larger parent beyond the minimum number of
			// active nodes of both parents
			if (boundary_extension && x == (min - 1) && len1 != len2) {

				// Choose the extension by chance
				int r = this->random->random_integer(0, max - x - 1);

				if (len1 < len2) {
					swap_node1 = active_nodes1->at(x);
					swap_node2 = active_nodes2->at(x + r);
				} else {
					swap_node1 = active_nodes1->at(x + r);
					swap_node2 = active_nodes2->at(x);
				}

			} else { // Just choose the swap nodes "in-line" without any extension
				swap_node1 = active_nodes1->at(x);
				swap_node2 = active_nodes2->at(x);
			}

			// calculate the swap indexes
			index1 = (swap_node1 - num_inputs) * (1 + max_arity);
			index2 = (swap_node2 - num_inputs) * (1 + max_arity);

			// perform the swaps
			tmp = g1[index1];
			g1[index1] = g2[index2];
			g2[index2] = tmp;
		}
	}

}

#endif /* VARIATION_CROSSOVER_DISCRETECROSSOVER_H_ */
