/*
 * PointMutation.h
 *
 *  Created on: 20.01.2023
 *      Author: roman
 */

#ifndef VARIATION_POINTMUTATION_H_
#define VARIATION_POINTMUTATION_H_

#include "../UnaryOperator.h"

template <class G, class F>
class ProbabilisticPoint : public UnaryOperator<G,F> {
private:
	float mutation_rate;
public:
	ProbabilisticPoint(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Random> p_random,
			std::shared_ptr<Species<G>> p_species);
	virtual ~ProbabilisticPoint() = default;

	void variate(std::shared_ptr<Individual<G, F>> individual) override;
};

template <class G, class F>
ProbabilisticPoint<G, F>::ProbabilisticPoint(std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Random> p_random,
		std::shared_ptr<Species<G>> p_species) : UnaryOperator<G, F>(p_parameters, p_random, p_species ) {

		this->name = "Probabilistic Point";
		this->mutation_rate = this->parameters->get_mutation_rate();
}

template <class G, class F>
void ProbabilisticPoint<G, F>::variate(std::shared_ptr<Individual<G, F>> individual) {

	std::shared_ptr<G[]> genome =  individual-> get_genome();

	int genome_size = this->parameters->get_genome_size();
	int max_gene;
	int min_gene;

	int num_mutations = this->mutation_rate * genome_size;
	int random_pos;

	for (int i = 0; i < num_mutations; i++) {

		random_pos = this->random->random_integer(0, genome_size - 1);

		if (this->species->is_real_valued()) {
			genome[random_pos] = this->random->random_float(0.0, 1.0);
		} else {
			min_gene = this->species->min_gene(random_pos);
			max_gene = this->species->max_gene(random_pos);
			genome[random_pos] = this->random->random_integer(
					min_gene, max_gene);
		}

	}

}

#endif /* VARIATION_POINTMUTATION_H_ */
