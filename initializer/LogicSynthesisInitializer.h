//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: LogicSynthesisInitializer.h 
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef INITIALIZER_LOGICSYNTHESISINITIALIZER_H_
#define INITIALIZER_LOGICSYNTHESISINITIALIZER_H_

#include <string>

#include "../functions/BooleanFunctions.h"
#include "../problems/LogicSynthesisProblem.h"
#include "../functions/BooleanFunctions.h"
#include "BlackBoxInitializer.h"


/// @brief Derived intializer class for logic synthesis problems. 
/// @details Initializes the defined logic synthesis problem. 
/// @tparam E Evaluation Type
/// @tparam G Genotype Type
/// @tparam F Fitness Type 
template<class E, class G, class F>
class LogicSynthesisInitializer: public  BlackBoxInitializer<E, G, F> {

public:
	LogicSynthesisInitializer(const std::string &p_benchmark_file);
	~LogicSynthesisInitializer() = default;
	void init_problem() override;
	void init_functions() override;
};

template<class E, class G, class F>
LogicSynthesisInitializer<E, G, F>::LogicSynthesisInitializer(
		const std::string &p_benchmark_file) :
		BlackBoxInitializer<E, G, F>(p_benchmark_file) {
}

/// @brief Initializes the logic synthesis problem instance. 
template<class E, class G, class F>
void LogicSynthesisInitializer<E, G, F>::init_problem() {

	std::shared_ptr<LogicSynthesisProblem<E, G, F>> problem = std::make_shared<
			LogicSynthesisProblem<E, G, F>>(this->parameters, this->evaluator,
			this->inputs, this->outputs, this->constants, this->num_instances);

	this->composite->set_problem(problem);
}


/// @brief Initializes the funcion set used for logic synthesis problems. 
template<class E, class G, class F>
void LogicSynthesisInitializer<E, G, F>::init_functions() {
	this->functions = std::make_shared<FunctionsBoolean<E>>(this->parameters);
}


#endif /* INITIALIZER_LOGICSYNTHESISINITIALIZER_H_ */
