/*
 * SymbolicRegressionProblem.h
 *
 *  Created on: Jan 11, 2024
 *      Author: kalkreuth
 */

#ifndef PROBLEMS_SYMBOLICREGRESSIONPROBLEM_H_
#define PROBLEMS_SYMBOLICREGRESSIONPROBLEM_H_

#include "../problems/BlackBoxProblem.h"

#include <cmath>

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
