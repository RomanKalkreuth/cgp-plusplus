//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: BlockCrossover.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================


#ifndef VARIATION_CROSSOVER_BLOCKCROSSOVER_H_
#define VARIATION_CROSSOVER_BLOCKCROSSOVER_H_

#include <cmath>
#include "../BinaryOperator.h"

/// @brief Class for the representation of the block crossover that has proposed for CGP. 
/// @details Block crossover swaps blocks of active function genes between two individuals.
/// The block crossover uses a parameter blockSize which defines the maximum block size.
/// Reasonable results were obtained on several symbolic regression benchmarks (Kalkreuth (2021)).
/// @see Husa, J., Kalkreuth, R. (2018). A Comparative Study on Crossover in Cartesian Genetic Programming
/// https://doi.org/10.1007/978-3-319-77553-1_13
/// @see Kalkreuth R. (2021). Reconsideration and extension of Cartesian genetic programming
/// http://dx.doi.org/10.17877/DE290R-22504
/// @tparam G Genome type 
/// @tparam F Fitness type
template<class G, class F>
class BlockCrossover: public BinaryOperator<G, F> {
public:

	int block_size;

	BlockCrossover(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Random> p_random,
			std::shared_ptr<Species<G>> p_species) :
			BinaryOperator<G, F>(p_parameters, p_random, p_species) {
		this->name = "Block Crossover";
		this->block_size = 2;
	}

	virtual ~BlockCrossover() = default;

	void determine_swap_nodes(int block_size, std::vector<int> &active_nodes,
			std::vector<int> &swap_nodes);
	int calc_swap_index(int swap_node_number);

	void variate(std::shared_ptr<Individual<G, F>> p1,
			std::shared_ptr<Individual<G, F>> p2) override;
};

/// @brief Determines bunch of active nodes that are used for the swap by chance. 
/// @details Number of nodes depends on the block size. 
/// @param block_size block size parameter.
/// @param active_nodes active nodes of the parent. 
/// @param swap_nodes nodes that are used for the swap 
template<class G, class F>
void BlockCrossover<G, F>::determine_swap_nodes(int block_size,
		std::vector<int> &active_nodes, std::vector<int> &swap_nodes) {

	int j = 0;
	int rand_index;
	int node_number;

	std::vector<int> possible_nodes(active_nodes);

	swap_nodes.clear();

	while (j < block_size) {

		rand_index = this->random->random_integer(0, possible_nodes.size() - 1);
		node_number = possible_nodes.at(rand_index);

		swap_nodes.push_back(node_number);

		possible_nodes.erase(possible_nodes.begin() + rand_index);

		j++;
	}

}

template<class G, class F>
int BlockCrossover<G, F>::calc_swap_index(int swap_node_number) {

	int num_inputs = this->parameters->get_num_inputs();
	int max_arity = this->parameters->get_max_arity();

	return (swap_node_number - num_inputs) * (1 + max_arity);

}

template<class G, class F>
void BlockCrossover<G, F>::variate(std::shared_ptr<Individual<G, F>> p1,
		std::shared_ptr<Individual<G, F>> p2) {

	if (p1->num_active_nodes() == 0 || p2->num_active_nodes() == 0) {
		return;
	}

	int swap_node1 = 0;
	int swap_node2 = 0;
	int swap_index1 = 0;
	int swap_index2 = 0;
	int block_size;

	std::vector<int> swap_nodes1;
	std::vector<int> swap_nodes2;

	if (p1->num_active_nodes() < this->block_size
			|| p2->num_active_nodes() < this->block_size) {
		block_size = std::min(p1->num_active_nodes(), p2->num_active_nodes());
	} else {
		block_size = this->block_size;
	}

	//
	std::shared_ptr<std::vector<int>> active_nodes1 = p1->get_active_nodes();
	std::shared_ptr<std::vector<int>> active_nodes2 = p2->get_active_nodes();

	std::shared_ptr<G[]> g1 = p1->get_genome();
	std::shared_ptr<G[]> g2 = p2->get_genome();

	determine_swap_nodes(block_size, *active_nodes1, swap_nodes1);
	determine_swap_nodes(block_size, *active_nodes2, swap_nodes2);

	int j = 0;
	int temp = 0; // used to store values during swapping

	for (j = 0; j < block_size; j++) {

		swap_node1 = swap_nodes1.at(j);
		swap_node2 = swap_nodes2.at(j);

		swap_index1 = calc_swap_index(swap_node1);
		swap_index2 = calc_swap_index(swap_node2);

		temp = g1[swap_index1];

		g1[swap_index1] = g2[swap_index2];
		g2[swap_index2] = temp;

	}

}

#endif /* VARIATION_CROSSOVER_BLOCKCROSSOVER_H_ */
