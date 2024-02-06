//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: BlackBoxProblem.h 
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================


#ifndef PROBLEMS_BLACKBOXPROBLEM_H_
#define PROBLEMS_BLACKBOXPROBLEM_H_

#include <stdexcept>
#include <memory>
#include <mutex>

#include "../parameters/Parameters.h"
#include "../evaluator/Evaluator.h"
#include "../representation/Individual.h"

std::mutex mtx;

/// @brief Base class to represent a black box problem.
/// @details Provides core functionality for the evaluation of the black box problem. 
/// @tparam E Evalation type 
/// @tparam G Genome type 
/// @tparam F Fitness type
template<class E, class G, class F>
class BlackBoxProblem {
protected:
	std::shared_ptr<Parameters> parameters;
	std::shared_ptr<Evaluator<E, G, F>> evaluator;

	std::shared_ptr<std::vector<std::vector<E>>> inputs;
	std::shared_ptr<std::vector<std::vector<E>>> outputs;

	std::shared_ptr<std::vector<E>> constants;

	std::shared_ptr<std::vector<E>> outputs_individual;

	std::string name;

	int num_variables;
	int num_constants;
	int num_inputs;
	int num_outputs;
	int num_instances;

public:
	BlackBoxProblem(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Evaluator<E, G, F>> p_evaluator,
			std::shared_ptr<std::vector<E>> p_constants);

	BlackBoxProblem(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Evaluator<E, G, F>> p_evaluator,
			std::shared_ptr<std::vector<E>> p_constants, int p_num_instances);

	BlackBoxProblem(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Evaluator<E, G, F>> p_evaluator,
			std::shared_ptr<std::vector<std::vector<E>>> p_inputs,
			std::shared_ptr<std::vector<std::vector<E>>> p_outputs,
			std::shared_ptr<std::vector<E>> p_constants, int p_num_instances);

	BlackBoxProblem(const BlackBoxProblem &problem);

	virtual void evaluate_individual(
				std::shared_ptr<Individual<G, F>> individual);

	virtual BlackBoxProblem<E, G, F>* clone() = 0;
	virtual F evaluate(std::shared_ptr<std::vector<E>> outputs_real,
			std::shared_ptr<std::vector<E>> outputs_individual) = 0;
	const std::string& get_name() const;

	virtual ~BlackBoxProblem() = default;
};

/// @brief Constructor that instantiates the vector for the input/output mappings
///	@details Dimension of the problem is based on the number of considered instances 
/// @param p_parameters shared pointer to parameter object
/// @param p_evaluator shared pointer to evaluator object
/// @param p_constants shared pointer to constants vector
/// @param p_num_instances number of instances considered by the problem
template<class E, class G, class F>
BlackBoxProblem<E, G, F>::BlackBoxProblem(
		std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Evaluator<E, G, F>> p_evaluator,
		std::shared_ptr<std::vector<E>> p_constants, int p_num_instances) {

	if (p_parameters != nullptr && p_evaluator != nullptr
			&& p_constants != nullptr) {
		parameters = p_parameters;
		evaluator = p_evaluator;
		constants = p_constants;
	} else {
		throw std::invalid_argument("Nullpointer exception in problem class!");
	}

	num_variables = parameters->get_num_variables();
	num_constants = parameters->get_num_constants();

	num_inputs = parameters->get_num_inputs();
	num_outputs = parameters->get_num_outputs();

	num_instances = p_num_instances;

	inputs = std::make_shared<std::vector<std::vector<E>>>(num_instances);
	outputs = std::make_shared<std::vector<std::vector<E>>>(num_instances);

	outputs_individual = std::make_shared<std::vector<E>>(num_outputs);

}

/// @brief Overloaded constructor that initializes the input/output vectors
/// @param p_inputs shared pointer to input data vector
/// @param p_outputs shared pointer to output data vector
/// @param p_parameters shared pointer to parameter object
/// @param p_evaluator shared pointer to evaluator object
/// @param p_constants shared pointer to constants vector
/// @param p_num_instances number of instances considered by the problem 
template<class E, class G, class F>
BlackBoxProblem<E, G, F>::BlackBoxProblem(
		std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Evaluator<E, G, F>> p_evaluator,
		std::shared_ptr<std::vector<std::vector<E>>> p_inputs,
		std::shared_ptr<std::vector<std::vector<E>>> p_outputs,
		std::shared_ptr<std::vector<E>> p_constants, int p_num_instances) :
		BlackBoxProblem<E, G, F>(p_parameters, p_evaluator, p_constants,
				p_num_instances) {


	if (p_inputs == nullptr || p_outputs == nullptr) {
		throw std::invalid_argument(
				"Nullpointer exception in BlackBoxProblem class!");
	}

	if (p_inputs->size() == 0 || p_outputs->size() == 0) {
		throw std::invalid_argument("Empty vector in BlackBoxProblem class!");
	}

	E value;

	for (int i = 0; i < this->num_instances; i++) {
		for (int j = 0; j < this->num_variables; j++) {
			value = (*p_inputs)[i][j];
			this->inputs->at(i).push_back(value);
		}

		for (int j = 0; j < this->num_outputs; j++) {
			value = (*p_outputs)[i][j];
			this->outputs->at(i).push_back(value);
		}
	}

}

/// @brief Copy constructor for deep cloning 
/// @param problem problem instance to clone
template<class E, class G, class F>
BlackBoxProblem<E, G, F>::BlackBoxProblem(const BlackBoxProblem &problem) {

	parameters = problem.parameters;
	evaluator = problem.evaluator;

	num_variables = problem.num_variables;
	num_constants = problem.num_constants;

	num_inputs = problem.num_inputs;
	num_outputs = problem.num_outputs;
	num_instances = problem.num_instances;

	inputs = std::make_shared<std::vector<std::vector<E>>>(*problem.inputs);
	outputs = std::make_shared<std::vector<std::vector<E>>>(*problem.outputs);

	constants = std::make_shared<std::vector<E>>(*problem.constants);

	outputs_individual = std::make_shared<std::vector<E>>(num_outputs);
}

template<class E, class G, class F>
const std::string& BlackBoxProblem<E, G, F>::get_name() const {
	return this->name;
}


/// @brief Evaluates an individual against the given input/output matching
/// @details Iterative evaluation procedure for each instance of the problem.
/// @param individual individual to evaluate
template<class E, class G, class F>
void BlackBoxProblem<E, G, F>::evaluate_individual(
		std::shared_ptr<Individual<G, F>> individual) {

	if (individual->is_evaluated()) {
		return;
	}

	F diff = 0;

	std::shared_ptr<std::vector<E>> input_instance;
	std::shared_ptr<std::vector<E>> output_instace;
	std::shared_ptr<std::vector<E>> outputs_ind = std::make_shared<
			std::vector<E>>();

	for (int i = 0; i < this->num_instances; i++) {
		input_instance = std::make_shared<std::vector<E>>(this->inputs->at(i));
		output_instace = std::make_shared<std::vector<E>>(this->outputs->at(i));

		if (this->num_constants > 0) {
			input_instance->insert(std::end(*input_instance),
					std::begin(*constants), std::end(*constants));
		}

		outputs_ind->clear();

		mtx.lock();
		this->evaluator->evaluate_iterative(individual, input_instance,
				outputs_ind);
		mtx.unlock();

		diff += this->evaluate(output_instace, outputs_ind);
	}

	individual->set_fitness(diff);
	individual->set_evaluated(true);
}

#endif /* PROBLEMS_BLACKBOXPROBLEM_H_ */
