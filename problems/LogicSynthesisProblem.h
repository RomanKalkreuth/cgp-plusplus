//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: LogicSynthesisProblem.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// -===============================================================================

#ifndef PROBLEMS_LOGICSYNTHESISPROBLEM_H_
#define PROBLEMS_LOGICSYNTHESISPROBLEM_H_

#include <any>
#include <bitset>
#include <algorithm>
#include <cmath>
#include <mutex>

#include "../problems/BlackBoxProblem.h"

/// @brief Class to represent a logic synthesis problem.
/// @details Hamming distance is used to as fitness metric. 
/// Each instance represents one chunk of the compressed truth table. 
/// @tparam E Evalation type 
/// @tparam G Genome type 
/// @tparam F Fitness type
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

/// @brief Return a the bit value at position k from a output mask
/// @param n output mask
/// @param k position of bit balue 
/// @return bit value at positon k 
template<class E, class G, class F>
int LogicSynthesisProblem<E, G, F>::get_bit(E n, E k) {
	return (n >> k) & 1;
}

/// @brief Evaluates the outputs on an individual against the real outputs of the problem.
/// @details Uses hamming distance for the fitness calculation.  
/// @param outputs_real real output values of the truth table
/// @param outputs_individual outputs obtained from the evaluation of the genome 
/// @return 
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

	/// XOR the two outputs to filter out similar bit values 
	E compare = output_individual ^ output_real;

	// Caculate the hamming distance based on the number of bits considered for each chunk 
	for (int j = 0; j < this->num_bits; j++) {
		E temp = compare;
		// Obtain the bit value at jth position and add it to the difference
		diff = diff + get_bit(temp, j);
	}
	return diff;
}

template<class E, class G, class F>
LogicSynthesisProblem<E, G, F>* LogicSynthesisProblem<E, G, F>::clone() {
	return new LogicSynthesisProblem<E, G, F>(*this);
}

#endif /* PROBLEMS_LOGICSYNTHESISPROBLEM_H_ */
