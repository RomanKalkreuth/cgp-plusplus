/*
 * VectorIndividual.cpp
 *
 *  Created on: 26.04.2020
 *      Author: roman
 */

#include "IntegerVectorSpecies.h"
#include <stdexcept>
#include <iostream>

IntegerVectorSpecies::IntegerVectorSpecies(std::shared_ptr<Random> p_random,
		std::shared_ptr<Parameters> p_parameters) {
	if (p_random == nullptr) {
		throw std::invalid_argument("Random object is NULL!");
	} else if (p_parameters == nullptr) {
		throw std::invalid_argument("Parameter object is NULL!");
	}

	random = p_random;
	parameters = p_parameters;

	num_nodes = parameters->get_num_function_nodes();
	num_inputs = parameters->get_num_inputs();
	num_outputs = parameters->get_num_outputs();
	num_functions = parameters->get_num_functions();
	max_arity = parameters->get_max_arity();
	genome_size = calc_genome_size();
}

int IntegerVectorSpecies::calc_genome_size() {

	int genome_size = num_nodes * (max_arity + 1) + num_outputs;
	return genome_size;

}

int IntegerVectorSpecies::calc_max_gene(int position) {
	int max_gene;
	int phenotype = decode_genotype_at(position);

	if (phenotype == output_gene) {
		max_gene = num_inputs + num_nodes - 1;
	} else if (phenotype == function_gene) {
		max_gene = num_functions - 1;
	} else {
		max_gene = node_number_from_position(position) - 1;
	}

	return max_gene;
}

int IntegerVectorSpecies::decode_genotype_at(int position) {
	if (position >= num_nodes * (max_arity + 1)) {
		return output_gene;
	} else if (position % (max_arity + 1) == 0) {
		return function_gene;
	} else {
		return connection_gene;
	}

}

int IntegerVectorSpecies::node_number_from_position(int position) {

	int node_number;
	int phenotype = decode_genotype_at(position);

	if (phenotype == output_gene) {
		node_number = num_inputs + num_nodes
				+ (position - (num_nodes * max_arity));
	} else {
		node_number = num_inputs + (position / (max_arity + 1));
	}
	return node_number;
}

int IntegerVectorSpecies::position_from_node_number(int node_number) {
	return (node_number - num_inputs) * (max_arity + 1);
}

std::shared_ptr<int[]> IntegerVectorSpecies::get_genome() const {
	return genome;
}

void IntegerVectorSpecies::set_genome(std::shared_ptr<int[]> genome) {
	this->genome = genome;
}

