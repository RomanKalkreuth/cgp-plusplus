//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File MathematicalFunctions.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================


#ifndef FUNCTIONS_MATHEMATICALFUNCTIONS_H_
#define FUNCTIONS_MATHEMATICALFUNCTIONS_H_

#include "Functions.h"

#include <string>
#include <any>
#include <sstream>

/// @brief Represents a minimalistic mathematical function set. 
/// @details Ensures that only data type domains such as integer and floats are used
/// for the template paramter
/// @tparam E Evation type 
template<class E>
class FunctionsMathematical: public Functions<E>  {
public:
		FunctionsMathematical(std::shared_ptr<Parameters> p_parameters);
		virtual ~FunctionsMathematical() = default;

		E call_function(E inputs[], int function) override;
		std::string input_name(int input) override;
		std::string function_name(int function) override;

		int arity_of(int function) override;

};

template<class E>
FunctionsMathematical<E>::FunctionsMathematical(std::shared_ptr<Parameters> p_parameters) :
		Functions<E>(p_parameters) {

	if constexpr (!std::is_same<float, E>::value) {
		if constexpr (!std::is_same<double, E>::value) {
			throw std::invalid_argument(
					"This class only supports float and double!");
		}
	}
}

template<class E>
E FunctionsMathematical<E>::call_function(E inputs[], int function) {

	E result;

	switch (function) {

	case 0:
		result = inputs[0] + inputs[1];
		break;

	case 1:
		result = inputs[0] - inputs[1];
		break;

	case 2:
		result = inputs[0] * inputs[1];
		break;

	case 3:
		if (inputs[1] == 0) {
			result = 1;
		}
		else {
			result = inputs[0] / inputs[1];
		}
		break;

	default:
		throw std::invalid_argument("Illegal function number!");

	}

	return result;

}

template<class E>
std::string FunctionsMathematical<E>::function_name(int function) {

	std::string name = "";

	switch (function) {

	case 0:
		name = "ADD";
		break;

	case 1:
		name = "SUB";
		break;

	case 2:
		name = "MUL";
		break;

	case 3:
		name = "DIV";
		break;

	default:
		throw std::invalid_argument("Illegal function number!");

	}

	return name;
}

template<class E>
std::string FunctionsMathematical<E>::input_name(int input) {
	std::string input_name = "x" + std::to_string(input);
	return input_name;
}

template<class E>
int FunctionsMathematical<E>::arity_of(int function) {
	return 2;
}


#endif /* FUNCTIONS_MATHEMATICALFUNCTIONS_H_ */
