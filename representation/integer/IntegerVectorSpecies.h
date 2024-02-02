/*
 * VectorIndividual.h
 *
 *  Created on: 26.04.2020
 *      Author: roman
 */

#ifndef REPRESENTATION_INTEGERVECTORSPECIES_H_
#define REPRESENTATION_INTEGERVECTORSPECIES_H_

#include <iostream>
#include <map>
#include <memory>

#include "../../random/Random.h"
#include "../../parameters/Parameters.h"

class IntegerVectorSpecies {
public:
	IntegerVectorSpecies(std::shared_ptr<Random> p_random,
			std::shared_ptr<Parameters> p_parameters);
	virtual ~IntegerVectorSpecies() = default;

protected:

	const int connection_gene = 0;
	const int function_gene = 1;
	const int output_gene = 2;

	int num_nodes;
	int num_inputs;
	int num_outputs;
	int num_functions;
	int max_arity;
	int min_argument;
	int max_argument;
	int genome_size;
	int chromosome_size;

	std::shared_ptr<int[]> genome;
	std::shared_ptr<Random> random;
	std::shared_ptr<Parameters> parameters;

public:

	int calc_genome_size();
	int calc_max_gene(int position);
	int decode_genotype_at(int position);
	int node_number_from_position(int position);
	int position_from_node_number(int node_number);

	std::shared_ptr<int[]> get_genome() const;
	void set_genome(std::shared_ptr<int[]> genome);

};

#endif /* REPRESENTATION_INTEGERVECTORSPECIES_H_ */
