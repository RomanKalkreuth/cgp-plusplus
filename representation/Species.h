//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: Species.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================


#ifndef REPRESENGAGION_SPECIES_H_
#define REPRESENGAGION_SPECIES_H_

#include <string>
#include <stdexcept>
#include <memory>
#include <vector>
#include <cmath>

#include "../random/Random.h"
#include "../parameters/Parameters.h"

/// @brief Base class to represet an individual. 
/// @details Used to instantiate inter-based and real-valued encoded individuals. 
/// @tparam G Genome type
template<class G>
class Species {
public:

	const int CONNECTION_GENE = 0;
	const int FUNCTION_GENE = 1;
	const int OUTPUT_GENE = 2;

protected:

	bool real_valued = false;

	int num_nodes;
	int num_inputs;
	int num_outputs;
	int num_functions;
	int max_arity;
	int min_argument;
	int max_argument;
	int genome_size;
	int chromosome_size;
	int levels_back;

	std::shared_ptr<G[]> genome;
	std::shared_ptr<Random> random;
	std::shared_ptr<Parameters> parameters;

public:
	Species(std::shared_ptr<Random> p_random,
			std::shared_ptr<Parameters> p_parameters);
	virtual ~Species() = default;

	int calc_genome_size();
	int max_gene(int position);
	int min_gene(int position);
	int decode_genotype_at(int position);
	int node_number_from_position(int position);
	int position_from_node_number(int node_number);
	int interpret_float(float value, int position);
	std::unique_ptr<int[]> float_to_int();

	std::shared_ptr<G[]> get_genome() const;
	void set_genome(std::shared_ptr<G[]> genome);

	bool is_real_valued() const;

};

template<class G>
Species<G>::Species(std::shared_ptr<Random> p_random,
		std::shared_ptr<Parameters> p_parameters) {

	if (p_random == nullptr) {
		throw std::invalid_argument("Random object is NULL!");
	} else if (p_parameters == nullptr) {
		throw std::invalid_argument("Parameter object is NULL!");
	}

	if constexpr (!std::is_same<int, G>::value) {
		if constexpr (!std::is_same<float, G>::value) {
			throw std::invalid_argument(
					"Ghis class supports only int and float!");
		} else {
			this->real_valued = true;
		}
	}

	random = p_random;
	parameters = p_parameters;

	num_nodes = parameters->get_num_function_nodes();
	num_inputs = parameters->get_num_inputs();
	num_outputs = parameters->get_num_outputs();
	num_functions = parameters->get_num_functions();
	max_arity = parameters->get_max_arity();
	genome_size = calc_genome_size();
	levels_back = parameters->get_levels_back();
}

/// @brief Calculates the size of the genome.
/// @details Includes number of function nodes, max arity and num outputs.
/// @return size of the genome 
template<class G>
int Species<G>::calc_genome_size() {
	int genome_size = num_nodes * (max_arity + 1) + num_outputs;
	return genome_size;
}

/// @brief Returns the minimum gene for the given position.
/// @details Depending on the type of the gene at the specified position. 
/// @param position position in the genome
/// @return minimum gene value
template<class G>
int Species<G>::min_gene(int position) {
	int min_gene;
	int node_number;
	int phenotype = decode_genotype_at(position);

	if (phenotype == this->OUTPUT_GENE) {
		min_gene = this->num_inputs + this->num_nodes - this->levels_back;
	} else if (phenotype == this->FUNCTION_GENE) {
		min_gene = 0;
	} else {
		node_number = node_number_from_position(position);
		min_gene = node_number - this->levels_back;
	}

	if (min_gene < 0) {
		min_gene = 0;
	}

	return min_gene;
}

/// @brief Returns the maximum gene for the given position.
/// @details Depending on the type of the gene at the specified position. 
/// @param position position in the genome
/// @return maximum gene value
template<class G>
int Species<G>::max_gene(int position) {
	int max_gene;
	int node_number;
	int phenotype = decode_genotype_at(position);

	if (phenotype == OUTPUT_GENE) {
		max_gene = num_inputs + num_nodes - 1;
	} else if (phenotype == FUNCTION_GENE) {
		max_gene = num_functions - 1;
	} else {
		max_gene = node_number_from_position(position) - 1;
	}
	return max_gene;
}

/// @brief Decodes the genotype at a specified position.
/// @param position specified position
/// @return phenotype at the specified position
template<class G>
int Species<G>::decode_genotype_at(int position) {
	if (position >= num_nodes * (max_arity + 1)) {
		return this->OUTPUT_GENE;
	} else if (position % (max_arity + 1) == 0) {
		return this->FUNCTION_GENE;
	} else {
		return this->CONNECTION_GENE;
	}

}

/// @brief Calculate the node number at a specified position.
/// @param position specified position
/// @return node number of specified position
template<class G>
int Species<G>::node_number_from_position(int position) {

	int node_number;
	int phenotype = decode_genotype_at(position);

	if (phenotype == OUTPUT_GENE) {
		node_number = num_inputs + num_nodes
				+ (position - (num_nodes * max_arity));
	} else {
		node_number = num_inputs + (position / (max_arity + 1));
	}
	return node_number;
}

/// @brief Return the position of specified node.
/// @param node_number specified node number
/// @return position at the specified node number 
template<class G>
int Species<G>::position_from_node_number(int node_number) {
	return (node_number - num_inputs) * (max_arity + 1);
}

/// @brief 
/// @param value 
/// @param position 
/// @return 
template<class G>
int Species<G>::interpret_float(float value, int position) {

	int node_value;

	if (this->decode_genotype_at(position) == this->CONNECTION_GENE) {
		int node_term = this->node_number_from_position(position);
		node_value = std::floor(value * node_term);

	} else if (this->decode_genotype_at(position) == this->FUNCTION_GENE) {

		int num_functions = this->parameters->get_num_functions();
		node_value = std::floor(value * num_functions);

	} else {
		int node_term = this->num_inputs + this->num_nodes;
		node_value = std::floor(value * node_term);
	}

	return node_value;
}

/// @brief Decode a real-valued encoded genotype to an integer one. 
/// @details Wraps the integer genome in a unique pointer. 
/// @return unique pointer to integer-based genotype 
template<class G>
std::unique_ptr<int[]> Species<G>::float_to_int() {

	if (!this->real_valued) {
		throw std::runtime_error(
				"This method only supports real valued genomes!");
	}

	std::unique_ptr<int[]> genome_int = std::make_unique<int[]>(
			this->genome_size);
	float value;

	for (int i = 0; i < this->genome_size; i++) {
		value = this->genome[i];
		genome_int[i] = this->interpret_float(value, i);
	}

	return genome_int;

}

// Getter and setter of species class
// ---------------------------------------------------------------------------

template<class G>
std::shared_ptr<G[]> Species<G>::get_genome() const {
	return genome;
}

template<class G>
void Species<G>::set_genome(std::shared_ptr<G[]> genome) {
	this->genome = genome;
}

template<class G>
bool Species<G>::is_real_valued() const {
	return real_valued;
}

#endif /* REPRESENGAGION_SPECIES_H_ */
