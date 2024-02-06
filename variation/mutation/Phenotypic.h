//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: Phenotypic.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef VARIATION_MUTATION_PHENOTYPIC_H_
#define VARIATION_MUTATION_PHENOTYPIC_H_

#include "../UnaryOperator.h"

/// @brief Abstract base class for the implementation of phenotypic mutation in CGP. 
/// @tparam G Genome type 
/// @tparam F Fitness type
template<class G, class F>
class Phenotypic : public UnaryOperator<G,F>  {
protected:
	float rate;
	int max_depth;
public:
	Phenotypic(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Random> p_random,
			std::shared_ptr<Species<G>> p_species) : UnaryOperator<G, F>(p_parameters, p_random, p_species) {};
	virtual ~Phenotypic() = default;

	void variate(std::shared_ptr<Individual<G, F>> individual) = 0;

	int start_index(int num_active_nodes, int depth);
	int stochastic_depth(int max_depth, int num_active_nodes);
};

/// @brief Determines a start position for the mutation within the genome by chance. 
/// @param num_active_nodes number of active function nodes
/// @param depth mutation depth, number of function genes that will be considered
/// @return start position in the genome 
template<class G, class F>
int Phenotypic<G,F>::start_index(int num_active_nodes, int depth) {
		int start_max = num_active_nodes - depth;
		int start;

		if (start_max <= 0) {
			start = 0;
		} else {
			start  = this->random->random_integer(0, start_max - 1);
		}

		return start;
}

/// @brief Determnes the depth of the mutation by chance
/// @param max_depth maximum depth of the mutation
/// @param num_active_nodes number of active function nodes
/// @return mutation depth
template<class G, class F>
int Phenotypic<G,F>::stochastic_depth(int max_depth, int num_active_nodes) {
	int depth;
	int max;

	if (num_active_nodes <= max_depth) {
		max = num_active_nodes - 1;
	} else {
		max = max_depth;
	}

	depth = this->random->random_integer(1, max);

	return depth;
}


#endif /* VARIATION_MUTATION_PHENOTYPIC_H_ */
