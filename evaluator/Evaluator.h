// 					CGP++: Modern C++ Implementation of CGP
// ===============================================================================
//	File
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2023
//
//  Author(s):
//
//	License:
// -===============================================================================

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

template<class E, class G, class F>
int Evaluator<E, G, F>::gene_at(std::shared_ptr<G[]> genome, int position) {
	if (this->species->is_real_valued()) {
		float gene_value = genome[position];
		return this->species->interpret_float(gene_value, position);
	} else {
		return genome[position];
	}
}

template<class E, class G, class F>
void Evaluator<E, G, F>::clear_maps() {
	this->node_value_map.clear();
	this->expression_map.clear();
	this->node_number_map.clear();
}

template<class E, class G, class F>
void Evaluator<E, G, F>::determine_active_nodes(
		std::shared_ptr<Individual<G, F>> individual) {

	std::shared_ptr<std::vector<int>> active_nodes =
			individual->get_active_nodes();

	std::shared_ptr<G[]> genome = individual->get_genome();
	std::map<int, int> node_number_map;

	int genome_size = this->parameters->get_genome_size();
	int current_node_num;
	int output_value;
	int output_position;
	int referenced_node_position;
	int current_genome_position;

	active_nodes->clear();

	for (int i = 0; i < this->num_outputs; i++) {

		output_position = genome_size - i - 1;
		output_value = this->gene_at(genome, output_position);
		referenced_node_position = this->species->position_from_node_number(
				output_value);
	}
}

template<class E, class G, class F>
std::pair<E, std::string> Evaluator<E, G, F>::evaluate_node(
		std::shared_ptr<std::vector<E>> inputs,
		std::stringstream *expression_stream,
		std::shared_ptr<std::vector<int>> active_nodes,
		std::shared_ptr<G[]> genome, int node_num, int num_inputs) {

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

	if (node_num < num_inputs) {
		E value = inputs->at(node_num);
		std::string input_name = "";

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

	int connection_gene;
	int position = this->species->position_from_node_number(node_num);
	int function = gene_at(genome, position);
	int num_arguments = this->functions->arity_of(function);
	int max_arity = parameters->get_max_arity();
	std::stringstream node_expression;

	E arguments[max_arity];

	if (this->evaluate_expression) {
		std::string function_name = functions->function_name(function);
		node_expression << function_name;
		node_expression << "(";
		*expression_stream << node_expression.str();
	}

	for (int i = 1; i <= max_arity; i++) {

		connection_gene = gene_at(genome, position + i);

		std::pair<E, std::string> return_pair = evaluate_node(inputs,
				expression_stream, active_nodes, genome, connection_gene,
				num_inputs);

		arguments[i - 1] = return_pair.first;

		std::string node_string = return_pair.second;

		if (this->evaluate_expression) {
			node_expression << node_string;
			if (num_arguments > 1 && i != num_arguments) {
				node_expression << " ";
				*expression_stream << " ";
			}
		}
	}

	if (this->evaluate_expression) {
		node_expression << ")";
		*expression_stream << ")";
		this->expression_map.insert( { node_num, node_expression.str() });
	}

	E result = functions->call_function(arguments, function);
	node_value_map.insert( { node_num, result });

	std::pair<E, std::string> pair { result, node_expression.str() };

	return pair;
}

template<class E, class G, class F>
void Evaluator<E, G, F>::evaluate_recursive(std::shared_ptr<Individual<G, F>> individual,
		std::shared_ptr<std::vector<E>> inputs,
		std::shared_ptr<std::vector<E>> outputs) {

	this->clear_maps();

	std::shared_ptr<std::vector<int>> active_nodes =
			individual->get_active_nodes();
	std::shared_ptr<G[]> genome = individual->get_genome();

	std::stringstream expression_stream;

	active_nodes->clear();

	std::pair<E, std::string> result_pair;

	if (this->evaluate_expression) {
		individual->clear_expressions();
	}

	int output;
	for (int i = 0; i < this->num_outputs; i++) {

		int position = this->genome_size - i - 1;
		output = gene_at(genome, position);

		result_pair = evaluate_node(inputs, &expression_stream, active_nodes,
				genome, output, num_inputs);

		outputs->push_back(result_pair.first);

		if (this->evaluate_expression) {
			individual->add_expression(result_pair.second);
			expression_stream.str("");
		}
	}
}

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

	for (auto it = active_nodes->begin(); it != active_nodes->end(); it++) {

		node_num = *it;

		node_pos = this->species->position_from_node_number(node_num);
		function = this->gene_at(genome, node_pos);

		for (int i = 0; i < this->max_arity; i++) {
			node_arg = this->gene_at(genome, node_pos + i + 1);

			if(node_arg < this->num_inputs) {
				arguments[i] = inputs->at(node_arg);
			} else {
				arguments[i] = this->node_value_map.at(node_arg);
			}
		}

		E result = this->functions->call_function(arguments, function);

		this->node_value_map.insert( { node_num, result });
	}


	E value;

	for (int i = 0; i < this->num_outputs; i++) {
		output_pos = this->genome_size - i - 1;
		output_val = gene_at(genome, output_pos);


		if (output_val < this->num_inputs) {
			value = inputs->at(output_val);
		} else {
			value = this->node_value_map.at(output_val);
		}

		outputs->push_back(value);
	}

}

template<class E, class G, class F>
void Evaluator<E, G, F>::visit_node(std::shared_ptr<G[]> genome,
		std::shared_ptr<std::vector<int>> active_nodes, int node_num,
		int num_inputs) {

	if (this->node_number_map.count(node_num) == 1) {
		return;
	}

	if (node_num < num_inputs) {
		return;
	}

	active_nodes->push_back(node_num);
	this->node_number_map.insert( { node_num, 1 });

	int connection_gene;
	int position = this->species->position_from_node_number(node_num);

	for (int i = 1; i <= this->max_arity; i++) {
		connection_gene = gene_at(genome, position + i);
		visit_node(genome, active_nodes, connection_gene, num_inputs);
	}
}

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

template<class E, class G, class F>
std::string Evaluator<E, G, F>::decode_node_expression(
		std::shared_ptr<G[]> genome, std::stringstream *expression_stream,
		int node_num, int num_inputs) {

	if (expression_map.count(node_num) == 1) {
		std::string expression = expression_map.at(node_num);
		*expression_stream << expression;
		return expression;
	}

	if (node_num < num_inputs) {
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

	std::string function_name = functions->function_name(function);
	std::string input_name;

	std::stringstream node_expression;

	node_expression << function_name;
	node_expression << "(";

	*expression_stream << node_expression.str();

	for (int i = 1; i <= max_arity; i++) {

		connection_gene = gene_at(genome, position + i);

		std::string node_string = decode_node(genome, expression_stream,
				connection_gene, num_inputs);

		node_expression << node_string;

		if (num_arguments > 1 && i != num_arguments) {
			node_expression << " ";
			*expression_stream << " ";
		}
	}

	node_expression << ")";
	*expression_stream << ")";

	expression_map.insert( { node_num, node_expression.str() });

	return node_expression.str();

}

#endif /* EVALUATOR_EVALUATOR_H_ */
