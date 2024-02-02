// 					CGP++: Modern C++ Implementation of CGP
// ===============================================================================
//	File
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//  Author(s):
//
//	License:
// -===============================================================================

#ifndef FUNCTIONS_FUNCTIONS_H_
#define FUNCTIONS_FUNCTIONS_H_

#include <string>
#include <any>
#include <memory>
#include <sstream>

#include "../parameters/Parameters.h"

template<class E>
class Functions {
protected:
	Functions(std::shared_ptr<Parameters> p_parameters);
	int num_operators;
public:
	virtual ~Functions() = default;
	virtual E call_function(E inputs[], int function) = 0;
	virtual std::string function_name(int function) = 0;
	virtual std::string input_name(int input) = 0;
	virtual int arity_of(int function) = 0;
};

template<class E>
Functions<E>::Functions(std::shared_ptr<Parameters> p_parameters) {
	if (p_parameters != nullptr) {
		num_operators = p_parameters->get_max_arity();
	} else {
		throw std::invalid_argument(
				"Nullpointer exception in evaluator class!");
	}
}


#endif /* FUNCTIONS_FUNCTIONS_H_ */
