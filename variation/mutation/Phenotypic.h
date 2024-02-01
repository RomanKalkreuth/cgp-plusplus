/*
 * PhenotypicMutation.h
 *
 *  Created on: 26.01.2023
 *      Author: roman
 */

#ifndef VARIATION_MUTATION_PHENOTYPIC_H_
#define VARIATION_MUTATION_PHENOTYPIC_H_

#include "../UnaryOperator.h"

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

	int start_index(int num_active_function_nodes, int depth);
	int stochastic_depth(int max_depth, int num_active_function_nodes);
};

template<class G, class F>
int Phenotypic<G,F>::start_index(int num_active_function_nodes, int depth) {
		int start_max = num_active_function_nodes - depth;
		int start;

		if (start_max <= 0) {
			start = 0;
		} else {
			start  = this->random->random_integer(0, start_max);
		}

		return start;
}

template<class G, class F>
int Phenotypic<G,F>::stochastic_depth(int max_depth, int num_active_function_nodes) {
	int depth;
	int max;

	if (num_active_function_nodes <= max_depth) {
		max = num_active_function_nodes - 1;
	} else {
		max = max_depth;
	}

	depth = this->random->random_integer(1, max);

	return depth;
}


#endif /* VARIATION_MUTATION_PHENOTYPIC_H_ */
