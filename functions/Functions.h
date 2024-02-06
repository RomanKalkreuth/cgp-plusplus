//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File Function.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef FUNCTIONS_FUNCTIONS_H_
#define FUNCTIONS_FUNCTIONS_H_

#include <string>
#include <any>
#include <memory>
#include <sstream>

#include "../parameters/Parameters.h"

/// @brief Abstract base class to represent function set.
/// @details Ensures that every function set that inherits from this class 
/// has member fucntions to call the functions and to obtain the function and input names. 
/// @tparam E Evaluation type
template<class E>
class Functions {
protected:
	Functions(std::shared_ptr<Parameters> p_parameters);
	int num_operators;
public:
	virtual ~Functions() = default;
	virtual E call_function(E inputs[], int function) = 0;

	/// @brief Returns the function name 
	/// @param function index of the functions 
	/// @return function name
	virtual std::string function_name(int function) = 0;

	/// @brief Returns the input name
	/// @param input index of the input
	/// @return name of the input
	virtual std::string input_name(int input) = 0;

	/// @brief Returns the arity of a function
	/// @param function index of the function
	/// @return funcion arity 
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
