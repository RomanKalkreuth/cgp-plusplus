/*
 * RecobinationOperator.h
 *
 *  Created on: 20.01.2023
 *      Author: roman
 */

#ifndef VARIATION_BINARYOPERATOR_H_
#define VARIATION_BINARYOPERATOR_H_

#include "GeneticOperator.h"
#include "../representation/Individual.h"

#include <memory>

template <class G, class F>
class BinaryOperator : public GeneticOperator<G,F> {
public:
	BinaryOperator(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Random> p_random,
			std::shared_ptr<Species<G>> p_species) : GeneticOperator<G,F>(p_parameters, p_random, p_species) {}
	virtual ~BinaryOperator() = default;

	virtual void variate(std::shared_ptr<Individual<G, F>> p1, std::shared_ptr<Individual<G, F>> p2) = 0;
};

#endif /* VARIATION_BINARYOPERATOR_H_ */
