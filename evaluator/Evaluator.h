//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: Evaluator.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef EVALUATOR_EVALUATOR_H_
#define EVALUATOR_EVALUATOR_H_

#include "../parameters/Parameters.h"
#include "../functions/Functions.h"
#include "../representation/Species.h"

#include <map>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <memory>
#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>

/// @brief Represents the evaluator for CGP (Cartesian Genetic Programming). 
/// @details Responsible for decoding and evaluating the encoded chromosomes. Provides functions for recursive and iterative evaluation
/// as well as functions for the generation of the symbolic expression which serves as a textual representation of a cadidate program. 
/// @tparam E Evalation type 
/// @tparam G Genome type 
/// @tparam F Fitness type
template<class E, class G, class F>
class Evaluator {
private:

	int num_inputs;
	int num_outputs;
	int max_arity;
	int genome_size;

	std::shared_ptr<Parameters> parameters;
	std::shared_ptr<Functions<E>> functions;
	std::shared_ptr<Species<G>> species;

	std::map<int, E> node_value_map;
	std::map<int, int> node_number_map;
	std::map<int, string> expression_map;

	bool evaluate_expression = true;

	int gene_at(std::shared_ptr<G[]> genome, int position);

public:
	Evaluator(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Functions<E>> p_functions,
			std::shared_ptr<Species<G>> p_species);
	virtual ~Evaluator() = default;
	void evaluate_recursive(std::shared_ptr<Individual<G, F>> individual,
			std::shared_ptr<std::vector<E>> inputs,
			std::shared_ptr<std::vector<E>> outputs);
	std::pair<E, std::string> evaluate_node(
			std::shared_ptr<std::vector<E>> inputs,
			std::stringstream *expression_stream,
			std::shared_ptr<std::vector<int>> active_nodes,
			std::shared_ptr<G[]> genome, int node_num, int num_inputs);

	void evaluate_iterative(std::shared_ptr<Individual<G, F>> individual,
			std::shared_ptr<std::vector<E>> inputs,
			std::shared_ptr<std::vector<E>> outputs);

	void decode_path(std::shared_ptr<Individual<G, F>> individual);
	void visit_node(std::shared_ptr<G[]> genome,
			std::shared_ptr<std::vector<int>> active_nodes, int node_num,
			int num_inputs);

	void decode_expression(std::shared_ptr<G[]> genome,
			std::shared_ptr<std::string[]> expressions);

	std::string decode_node_expression(std::shared_ptr<G[]> genome,
			std::stringstream *expression, int node_num, int num_inputs);

	void decode(int genome[]);

	void determine_active_nodes(std::shared_ptr<Individual<G, F>> individual);

	void clear_maps();
};

template<class E, class G, class F>
Evaluator<E, G, F>::Evaluator(std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Functions<E>> p_functions,
		std::shared_ptr<Species<G>> p_species) {
	if (p_parameters != nullptr && p_functions != nullptr
			&& p_species != nullptr) {
		parameters = p_parameters;
		functions = p_functions;
		species = p_species;
	} else {
		throw std::invalid_argument(
				"Nullpointer exception in evaluator class!");
	}

	num_outputs = parameters->get_num_outputs();
	num_inputs = parameters->get_num_inputs();
	genome_size = parameters->get_genome_size();
	max_arity = parameters->get_max_arity();

	evaluate_expression = parameters->is_evaluate_expression();
}

/// @brief Returns the gene at the specified position in the genome.
/// @details Used to retrieve the gene value at the specified position in the genome. It takes a shared pointer 
/// to the genome array genome and the position of the gene position as input. 
/// If the species is real-valued, it retrieves the gene value as a float from the genome array 
/// and interprets it using the interpret_float function of the species object. If the species is not real-valued,
/// it simply returns the gene value at the specified position in the genome array. 
/// @param genome The genome array.
/// @param position Position of the gene in the genome.
/// @return Gene value at the specified position
template<class E, class G, class F>
int Evaluator<E, G, F>::gene_at(std::shared_ptr<G[]> genome, int position) {
	if (this->species->is_real_valued()) {
		float gene_value = genome[position];
		return this->species->interpret_float(gene_value, position);
	} else {
		return genome[position];
	}
}

/// @brief Clears the maps used for decoding and creating the symbolic expression.
template<class E, class G, class F>
void Evaluator<E, G, F>::clear_maps() {
	this->node_value_map.clear();
	this->expression_map.clear();
	this->node_number_map.clear();
}

/// @brief Determines the active nodes of an individual.
/// @details Loops over the number of outputs and for each output, it retrieves the output position 
/// in the genome and the corresponding output value using the gene_at function. 
/// It calculates the referenced node position using the position_from_node_number function of the species object.
/// @param individual Poiner to the individual instance 
template<class E, class G, class F>
void Evaluator<E, G, F>::determine_active_nodes(
		std::shared_ptr<Individual<G, F>> individual) {

	// Get the list of active nodes in the individual
	std::shared_ptr<std::vector<int>> active_nodes =
			individual->get_active_nodes();

	// Get the genome of the individual
	std::shared_ptr<G[]> genome = individual->get_genome();

	// Create a map to store the node number and its position in the genome
	std::map<int, int> node_number_map;

	// Initialize variables for evaluating the tree
	int genome_size = this->parameters->get_genome_size();
	int current_node_num;
	int output_value;
	int output_position;
	int referenced_node_position;
	int current_genome_position;

	active_nodes->clear();

	// Iterate over the output nodes
	for (int i = 0; i < this->num_outputs; i++) {
		// Get the position of the output gene in the genome
		output_position = genome_size - i - 1;

		// Get the value of the output gene
		output_value = this->gene_at(genome, output_position);

		// Get the position of the referenced node in the genome
		referenced_node_position = this->species->position_from_node_number(
				output_value);
	}
}

/// @brief Evaluates the node with the given node number.
/// @details Used to evaluate the node with the given node number. First checks if the node value is already stored in the node_value_map. 
/// If it is, it retrieves the value and expression from the maps and creates a pair of the value and expression. 
/// It appends the expression to the expression_stream and returns the pair.
/// @param inputs The input values.
/// @param expression_stream The stringstream to store the expression.
/// @param active_nodes The active nodes vector.
/// @param genome The genome array.
/// @param node_num The node number to evaluate.
/// @param num_inputs Number of inputs.
/// @return  A pair of the evaluated value and the expression.
template<class E, class G, class F>
std::pair<E, std::string> Evaluator<E, G, F>::evaluate_node(
		std::shared_ptr<std::vector<E>> inputs,
		std::stringstream *expression_stream,
		std::shared_ptr<std::vector<int>> active_nodes,
		std::shared_ptr<G[]> genome, int node_num, int num_inputs) {

	// If the node has already been evaluated, retrieve the value and expression
	if (this->node_value_map.count(node_num) == 1) {
		E value = this->node_value_map.at(node_num);
		std::string expression;

		if (this->evaluate_expression) {
			expression = expression_map.at(node_num);
		} else {
			expression = "";
		}

		std::pair<E, std::string> pair { value, expression };

		*expression_stream << expression;

		return pair;
	}

	// If the node is an input node, retrieve the value and input name
	if (node_num < num_inputs) {
		E value = inputs->at(node_num);
		std::string input_name = "";

		// If evaluating expressions, append the input name to the expression
		if (this->evaluate_expression) {
			input_name.append(functions->input_name(node_num));
			this->node_value_map.insert( { node_num, value });
			this->expression_map.insert( { node_num, input_name });
			*expression_stream << input_name;
		}

		std::pair<E, std::string> pair { value, input_name };

		return pair;
	}

	active_nodes->push_back(node_num);

	// Get the metainformation of the current node 
	int connection_gene;
	int position = this->species->position_from_node_number(node_num);
	int function = gene_at(genome, position);
	int num_arguments = this->functions->arity_of(function);
	int max_arity = parameters->get_max_arity();
	std::stringstream node_expression;

	E arguments[max_arity];

	// Evaluate the expression of the node if desired and append it to the string stream
	if (this->evaluate_expression) {
		std::string function_name = functions->function_name(function);
		node_expression << function_name;
		node_expression << "(";
		*expression_stream << node_expression.str();
	}

	// Evaluate the input connections of the node
	for (int i = 1; i <= max_arity; i++) {

		connection_gene = gene_at(genome, position + i);

		// Evaluate the connected node recursively
		std::pair<E, std::string> return_pair = evaluate_node(inputs,
				expression_stream, active_nodes, genome, connection_gene,
				num_inputs);

		arguments[i - 1] = return_pair.first;

		std::string node_string = return_pair.second;

		// Append the sub expression to the sstream
		if (this->evaluate_expression) {
			node_expression << node_string;
			if (num_arguments > 1 && i != num_arguments) {
				node_expression << " ";
				*expression_stream << " ";
			}
		}
	}

	// Finalize the expression 
	if (this->evaluate_expression) {
		node_expression << ")";
		*expression_stream << ")";
		this->expression_map.insert( { node_num, node_expression.str() });
	}

	// Call the function with the evaluated arguments
	E result = functions->call_function(arguments, function);
	node_value_map.insert( { node_num, result });

	std::pair<E, std::string> pair { result, node_expression.str() };

	return pair;
}

/// @brief Recursive evaluation of a CGP individual.
/// @details Evaluates an individual by recursively by triggering the recursive evaluation for each output node. 
/// @param individual CGP individual to evaluate
/// @param inputs input values used for evaluation
/// @param outputs outputs vector to store evalation results
template<class E, class G, class F>
void Evaluator<E, G, F>::evaluate_recursive(
		std::shared_ptr<Individual<G, F>> individual,
		std::shared_ptr<std::vector<E>> inputs,
		std::shared_ptr<std::vector<E>> outputs) {

	// Reset the internal maps before evaluation
	this->clear_maps();

	// Obtain the active nodes and genome from the individual object.
	std::shared_ptr<std::vector<int>> active_nodes =
			individual->get_active_nodes();
	std::shared_ptr<G[]> genome = individual->get_genome();

	// A string stream object is instantiated to store expressions during evaluation
	std::stringstream expression_stream;

	active_nodes->clear();

	// Pair to store the result value and expression of a node
	std::pair<E, std::string> result_pair;

	// If the evaluate_expression flag is set, the expressions stored in the individual object is cleared.
	// Clear the expression if the expression is evaluated.
	if (this->evaluate_expression) {
		individual->clear_expressions();
	}

	// A loop is executed for each output value 
	int output;
	// This method is used to evaluate the output nodes.
	for (int i = 0; i < this->num_outputs; i++) {

		int position = this->genome_size - i - 1;
		output = gene_at(genome, position);

		// Evaluate the path of the output node recursively 
		result_pair = evaluate_node(inputs, &expression_stream, active_nodes,
				genome, output, num_inputs);

		outputs->push_back(result_pair.first);

		// Add the evaluated expression to the individual if evaluating expressions
		// Add expression to the expression stream.
		if (this->evaluate_expression) {
			individual->add_expression(result_pair.second);
			expression_stream.str("");
		}
	}
}


/// @brief Iterative evaluation of a CGP individual.
/// @details Iterates over the active nodes, evaluates and stores the immediate results
/// @param individual CGP individual to evaluate
/// @param inputs input values used for evaluation
/// @param outputs outputs vector to store evalation results
template<class E, class G, class F>
void Evaluator<E, G, F>::evaluate_iterative(
		std::shared_ptr<Individual<G, F>> individual,
		std::shared_ptr<std::vector<E>> inputs,
		std::shared_ptr<std::vector<E>> outputs) {

	std::shared_ptr<std::vector<int>> active_nodes =
			individual->get_active_nodes();

	std::shared_ptr<G[]> genome = individual->get_genome();

	this->clear_maps();

	int output_val;
	int output_pos;
	int node_num;
	int node_pos;
	int node_arg;
	int function;

	E arguments[this->max_arity];

	// call all function functions in the active_nodes.
	for (auto it = active_nodes->begin(); it != active_nodes->end(); it++) {

		node_num = *it;

		node_pos = this->species->position_from_node_number(node_num);
		function = this->gene_at(genome, node_pos);

		// Evaluate each input connection of the nodes 
		for (int i = 0; i < this->max_arity; i++) {
			node_arg = this->gene_at(genome, node_pos + i + 1);

			//  Get the input values from the node value mapping or directly from the inputs
			if (node_arg < this->num_inputs) {
				arguments[i] = inputs->at(node_arg);
			} else {
				arguments[i] = this->node_value_map.at(node_arg);
			}
		}

		E result = this->functions->call_function(arguments, function);

		this->node_value_map.insert( { node_num, result });
	}

	E value;

	// Pushes the output nodes to the output vector.
	for (int i = 0; i < this->num_outputs; i++) {
		output_pos = this->genome_size - i - 1;
		output_val = gene_at(genome, output_pos);

		// Get the output value directly from the inputs or from the 
		// node -  value mapping
		if (output_val < this->num_inputs) {
			value = inputs->at(output_val);
		} else {
			value = this->node_value_map.at(output_val);
		}

		outputs->push_back(value);
	}

}

/// @brief Function that visits a function node to obtain the active nodes
/// @details The function recursively visits connected nodes and updates the map of visited active nodes.
/// @param genome genome of the CGP individual
/// @param active_nodes vector for the storage of active nodes
/// @param node_num current node number
/// @param num_inputs number of inputs
template<class E, class G, class F>
void Evaluator<E, G, F>::visit_node(std::shared_ptr<G[]> genome,
		std::shared_ptr<std::vector<int>> active_nodes, int node_num,
		int num_inputs) {

	// Check if the node has already been visited, and return if it has.
	if (this->node_number_map.count(node_num) == 1) {
		return;
	}

	// Check if the node is within the input nodes, and return if it is.
	if (node_num < num_inputs) {
		return;
	}


	// Add the current node to the list of active nodes.
	active_nodes->push_back(node_num);
	this->node_number_map.insert( { node_num, 1 });

	int connection_gene;
	int position = this->species->position_from_node_number(node_num);


	// Visit all connected nodes up to the maximum arity.
	for (int i = 1; i <= this->max_arity; i++) {
		connection_gene = gene_at(genome, position + i);
		visit_node(genome, active_nodes, connection_gene, num_inputs);
	}
}

/// @brief Recursive decoding of the path of the corresponding graph of an CGP indvidual.
/// @details The path is decoded by using recursion. 
/// @see Recursive evaluation in evaluate_recursive()
/// @param individual CGP individual to decode
template<class E, class G, class F>
void Evaluator<E, G, F>::decode_path(
		std::shared_ptr<Individual<G, F>> individual) {

	std::shared_ptr<std::vector<int>> active_nodes =
			individual->get_active_nodes();

	std::shared_ptr<G[]> genome = individual->get_genome();

	active_nodes->clear();
	this->node_number_map.clear();

	int output;
	for (int i = 0; i < num_outputs; i++) {
		int position = genome_size - i - 1;
		output = gene_at(genome, position);
		visit_node(genome, active_nodes, output, num_inputs);
	}

	std::sort(active_nodes->begin(), active_nodes->end());
}


/// @brief Recursive decoding of the expression of a CGP indvidual 
/// @details Decodes the path of of an CGP indidual and creates the corresponding symbolic expression. 
/// @see Recursive evaluation in evaluate_recursive()
/// @param individual CGP individual 
template<class E, class G, class F>
void Evaluator<E, G, F>::decode_expression(std::shared_ptr<G[]> genome,
		std::shared_ptr<std::string[]> expressions) {

	int num_outputs = parameters->get_num_outputs();
	int num_inputs = parameters->get_num_inputs();
	int genome_size = parameters->get_genome_size();

	std::stringstream expression_stream;
	expression_map.clear();

	int output;

	for (int i = 0; i < num_outputs; i++) {

		int position = genome_size - i - 1;
		output = gene_at(genome, position);

		decode_node(genome, &expression_stream, output, num_inputs);
		expressions[i] = expression_stream.str();
		expression_stream.str("");
	}

}

/// @brief This function decodes the expression of a function node in the genome.
/// @details Recursive evaluation of function node to obtain and generate the symbolic expression. 
/// @see Recursive evaluation of a function node in evaluate_node()
/// @param genome The genome containing the node its connection genes.
/// @param expression_stream The stringstream to store the decoded expression.
/// @param node_num The number of the node to decode.
/// @param num_inputs The number of input nodes.
/// @return The decoded expression of the node.
template<class E, class G, class F>
std::string Evaluator<E, G, F>::decode_node_expression(
		std::shared_ptr<G[]> genome, std::stringstream *expression_stream,
		int node_num, int num_inputs) {

	// Check if the expression for the node has already been decoded
	if (expression_map.count(node_num) == 1) {
		std::string expression = expression_map.at(node_num);
		*expression_stream << expression;
		return expression;
	}

	// Check if the node is an input node
	if (node_num < num_inputs) {
		// Obtain the input name and return it directly 
		std::string input_name = functions->input_name(node_num);
		expression_map.insert( { node_num, input_name });
		*expression_stream << input_name;
		return input_name;
	}

	int connection_gene;
	int function;

	int max_arity = parameters->get_max_arity();
	int position = this->species->position_from_node_number(node_num);

	function = gene_at(genome, position);

	int num_arguments = this->functions->arity_of(function);

	// Get the function name 
	std::string function_name = functions->function_name(function);
	std::string input_name;

	std::stringstream node_expression;

	node_expression << function_name;
	node_expression << "(";

	*expression_stream << node_expression.str();


	// Consider all connection genes 
	for (int i = 1; i <= max_arity; i++) {

		connection_gene = gene_at(genome, position + i);

		// Decode the connected node recursively and add it to the string stream 
		std::string node_string = decode_node(genome, expression_stream,
				connection_gene, num_inputs);

		node_expression << node_string;

		if (num_arguments > 1 && i != num_arguments) {
			node_expression << " ";
			*expression_stream << " ";
		}
	}

	// Finisih the generation of a node term expression 
	node_expression << ")";
	*expression_stream << ")";

	expression_map.insert( { node_num, node_expression.str() });

	return node_expression.str();

}

#endif /* EVALUATOR_EVALUATOR_H_ */
