// 					CGP++: Modern C++ Implementation of CGP
// ===============================================================================
//	File
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//  Author(s): Anonymous
//
//	License:
// -===============================================================================

#ifndef INITIALIZER_SYMBOLICREGRESSIONINITIALIZER_H_
#define INITIALIZER_SYMBOLICREGRESSIONINITIALIZER_H_

#include "../functions/BooleanFunctions.h"
#include "../problems/SymbolicRegressionProblem.h"
#include "../functions/MathematicalFunctions.h"
#include "BlackBoxInitializer.h"

template<class E, class G, class F>
class SymbolicRegressionInitializer: public BlackBoxInitializer<E, G, F> {
public:
	SymbolicRegressionInitializer(const std::string &p_benchmark_file);
	~SymbolicRegressionInitializer()  = default;
	void init_problem() override;
	void init_functions() override;
};

template<class E, class G, class F>
SymbolicRegressionInitializer<E, G, F>::SymbolicRegressionInitializer(
		const std::string &p_benchmark_file) :
		BlackBoxInitializer<E, G, F>(p_benchmark_file) {
}

template<class E, class G, class F>
void SymbolicRegressionInitializer<E, G, F>::init_problem() {
	std::shared_ptr<SymbolicRegressionProblem<E, G, F>> problem = std::make_shared<
			SymbolicRegressionProblem<E, G, F>>(this->parameters, this->evaluator,
			this->inputs, this->outputs, this->constants, this->num_instances);

	this->composite->set_problem(problem);
}

template<class E, class G, class F>
void SymbolicRegressionInitializer<E, G, F>::init_functions() {
		this->functions = std::make_shared<FunctionsMathematical<E>>(this->parameters);
}



#endif /* INITIALIZER_SYMBOLICREGRESSIONINITIALIZER_H_ */
