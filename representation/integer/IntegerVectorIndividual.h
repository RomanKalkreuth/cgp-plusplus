/*
 * IntegerVectorIndividual.h
 *
 *  Created on: 13.04.2020
 *      Author: roman
 */

#ifndef REPRESENTATION_INTEGERVECTORINDIVIDUAL_H_
#define REPRESENTATION_INTEGERVECTORINDIVIDUAL_H_

#include <string>
#include <memory>

#include "../../random/Random.h"
#include "../../parameters/Parameters.h"
#include "IntegerVectorSpecies.h"


class IntegerVectorIndividual : public IntegerVectorSpecies {

private:
	std::string expression;

public:

	IntegerVectorIndividual(std::shared_ptr<Random> p_random,
			std::shared_ptr<Parameters> p_parameters);
	virtual ~IntegerVectorIndividual() = default;

	void reset_genome();
	void print_genome();
	std::string generate_expression();
	void print_expression();
	int random_value_closed_interval(int min, int max);
};
#endif /* REPRESENTATION_INTEGERVECTORINDIVIDUAL_H_ */

