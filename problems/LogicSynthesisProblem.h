/*
 * ProblemBoolean.h
 *
 *  Created on: 08.11.2022
 *      Author: roman
 */

#ifndef PROBLEMS_LOGICSYNTHESISPROBLEM_H_
#define PROBLEMS_LOGICSYNTHESISPROBLEM_H_

#include <any>
#include <bitset>
#include <algorithm>
#include <cmath>
#include <mutex>

#include "../problems/BlackBoxProblem.h"

template<class E, class G, class F>
class LogicSynthesisProblem: public BlackBoxProblem<E, G, F> {
private:
	int num_bits;
	const int MAX_BITS = 32;

public:
	LogicSynthesisProblem(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Evaluator<E,G,F>> p_evalutor,
			std::shared_ptr<std::vector<std::vector<E>>> p_inputs,
			std::shared_ptr<std::vector<std::vector<E>>> p_outputs,
			std::shared_ptr<std::vector<E>> p_constants,
			int p_num_instances);

	~LogicSynthesisProblem() = default;

	int get_bit(E n, E k);
	F evaluate(E output_real, E output_individual);

	F evaluate(std::shared_ptr<std::vector<E>> outputs_real,
			std::shared_ptr<std::vector<E>> outputs_individual) override;
	LogicSynthesisProblem<E, G, F>* clone() override;

};

template<class E, class G, class F>
LogicSynthesisProblem<E, G, F>::LogicSynthesisProblem(std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Evaluator<E,G,F>> p_evaluator,
		std::shared_ptr<std::vector<std::vector<E>>> p_inputs,
		std::shared_ptr<std::vector<std::vector<E>>> p_outputs,
		std::shared_ptr<std::vector<E>> p_constants,
		int p_num_instances) :
		BlackBoxProblem<E, G, F>(p_parameters, p_evaluator,  p_inputs,  p_outputs , p_constants, p_num_instances) {

	this->name = "Logic Synthesis Problem";
	this->num_bits = std::pow(2, this->num_inputs);

	if(num_bits > MAX_BITS) {
		num_bits = MAX_BITS;
	}

}

template<class E, class G, class F>
int LogicSynthesisProblem<E, G, F>::get_bit(E n, E k) {
	return (n >> k) & 1;
}

template<class E, class G, class F>
F LogicSynthesisProblem<E, G, F>::evaluate(
		std::shared_ptr<std::vector<E>> outputs_real,
		std::shared_ptr<std::vector<E>> outputs_individual) {
	int diff = 0;

	for (int i = 0; i < this->num_outputs; i++) {
		diff += this->evaluate(outputs_real->at(i), outputs_individual->at(i));
	}
	return diff;
}

template<class E, class G, class F>
F LogicSynthesisProblem<E, G, F>::evaluate(E output_real, E output_individual) {
	int diff = 0;

	E compare = output_individual ^ output_real;

	// Calculate bit error
	for (int j = 0; j < num_bits; j++) {
		E temp = compare;
		diff = diff + get_bit(temp, j);
	}
	return diff;
}

template<class E, class G, class F>
LogicSynthesisProblem<E, G, F>* LogicSynthesisProblem<E, G, F>::clone() {
	return new LogicSynthesisProblem<E, G, F>(*this);
}


/*
template<class E, class G, class F>
void LogicSynthesisProblem<E, G, F>::evaluate_individual(
		std::shared_ptr<Individual<G,F>> individual) {

	if(individual->is_evaluated()){
		return;
	}

	int diff = 0;

	std::shared_ptr<std::vector<E>> input_chunk;
	std::shared_ptr<std::vector<E>> output_chunk;
	std::shared_ptr<std::vector<E>> outputs_individual = std::make_shared<std::vector<E>>();

	for (int i = 0; i < this->num_instances; i++) {

		input_chunk = std::make_shared<std::vector<E>>(this->inputs->at(i));
		output_chunk = std::make_shared<std::vector<E>>(this->outputs->at(i));

		outputs_individual->clear();

		mtx.lock();
		this->evaluator->evaluate_iterative(individual, input_chunk, outputs_individual);
		mtx.unlock();

		diff += this->evaluate(output_chunk, outputs_individual);
	}

	individual->set_fitness(diff);
	individual->set_evaluated(true);

}*/


#endif /* PROBLEMS_LOGICSYNTHESISPROBLEM_H_ */
