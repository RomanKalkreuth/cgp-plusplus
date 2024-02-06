//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: SymbolicRegressionProblem.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef PROBLEMS_SYMBOLICREGRESSIONPROBLEM_H_
#define PROBLEMS_SYMBOLICREGRESSIONPROBLEM_H_

#include "../problems/BlackBoxProblem.h"

#include <cmath>

/// @brief Class to represent of a sybolic regression problem.
/// @details Absolute difference is used fitness calculation.
/// @tparam E Evalation type 
/// @tparam G Genome type 
/// @tparam F Fitness type
template<class E, class G, class F>
class SymbolicRegressionProblem: public BlackBoxProblem<E, G, F> {
private:

public:
	SymbolicRegressionProblem(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Evaluator<E, G, F>> p_evaluator,
			std::shared_ptr<std::vector<std::vector<E>>> p_inputs,
			std::shared_ptr<std::vector<std::vector<E>>> p_outputs,
			std::shared_ptr<std::vector<E>> p_constants, int p_num_instances);

	~SymbolicRegressionProblem() = default;

	SymbolicRegressionProblem<E, G, F>* clone() override;
	F evaluate(std::shared_ptr<std::vector<E>> outputs_real,
			std::shared_ptr<std::vector<E>> outputs_individual) override;
};

template<class E, class G, class F>
SymbolicRegressionProblem<E, G, F>::SymbolicRegressionProblem(
		std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Evaluator<E, G, F>> p_evaluator,
		std::shared_ptr<std::vector<std::vector<E>>> p_inputs,
		std::shared_ptr<std::vector<std::vector<E>>> p_outputs,
		std::shared_ptr<std::vector<E>> p_constants, int p_num_instances) :
		BlackBoxProblem<E, G, F>(p_parameters, p_evaluator, p_inputs, p_outputs,
				p_constants, p_num_instances) {

	this->name = "Symbolic Regression Problem";

}

/// @brief Evaluates the outputs on an individual against the real outputs of the problem.
/// @details Fitness is obtained by calculation the sum of the absolute difference between the real 
/// function values and values obtained after evaluation of the individual. 
/// @param outputs_real 
/// @param outputs_individual 
/// @return 
template<class E, class G, class F>
F SymbolicRegressionProblem<E, G, F>::evaluate(
		std::shared_ptr<std::vector<E>> outputs_real,
		std::shared_ptr<std::vector<E>> outputs_individual) {
	float diff = 0;

	for (int i = 0; i < this->num_outputs; i++) {
		diff += abs(outputs_individual->at(i) - outputs_real->at(i));
	}

	return diff;
}

template<class E, class G, class F>
SymbolicRegressionProblem<E, G, F>* SymbolicRegressionProblem<E, G, F>::clone() {
	return new SymbolicRegressionProblem<E, G, F>(*this);
}


#endif /* PROBLEMS_SYMBOLICREGRESSIONPROBLEM_H_ */
