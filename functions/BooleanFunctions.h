/*
 * FunctionsBoolean.h
 *
 *  Created on: 25.06.2021
 *  Author: Roman Kalkreuth
 */

#ifndef FUNCTIONS_BOOLEANFUNCTIONS_H_
#define FUNCTIONS_BOOLEANFUNCTIONS_H_

#include "Functions.h"
#include "../parameters/Parameters.h"

#include <string>
#include <any>
#include <sstream>

template<class E>
class FunctionsBoolean: public Functions<E> {
public:
	FunctionsBoolean(std::shared_ptr<Parameters> p_parameters);
	virtual ~FunctionsBoolean() = default;

	E call_function(E inputs[], int function) override;
	std::string input_name(int input) override;
	std::string function_name(int function) override;

	int arity_of(int function) override;

};

template<class E>
FunctionsBoolean<E>::FunctionsBoolean(std::shared_ptr<Parameters> p_parameters) :
		Functions<E>(p_parameters) {

	if constexpr (!std::is_same<int, E>::value) {
		if constexpr (!std::is_same<long, E>::value) {
			if constexpr (!std::is_same<unsigned int, E>::value) {
				if constexpr (!std::is_same<unsigned long, E>::value) {
					throw std::invalid_argument(
							"This class only supports (signed/unsigned) long and int!");
				}
			}
		}
	}

}

template<class E>
E FunctionsBoolean<E>::call_function(E inputs[], int function) {

	E result;

	switch (function) {

	case 0:
		result = inputs[0] & inputs[1];
		break;

	case 1:
		result = inputs[0] | inputs[1];
		break;

	case 2:
		result = ~(inputs[0] & inputs[1]);
		break;

	case 3:
		result = ~(inputs[0] | inputs[1]);
		break;

	default:
		throw std::invalid_argument("Illegal function number!");

	}

	return result;

}

template<class E>
std::string FunctionsBoolean<E>::FunctionsBoolean::function_name(int function) {

	std::string name = "";

	switch (function) {

	case 0:
		name = "AND";
		break;

	case 1:
		name = "OR";
		break;

	case 2:
		name = "NAND";
		break;

	case 3:
		name = "NOR";
		break;

	default:
		throw std::invalid_argument("Illegal function number!");

	}

	return name;
}

template<class E>
std::string FunctionsBoolean<E>::input_name(int input) {
	std::string input_name = "x" + std::to_string(input);
	return input_name;
}

template<class E>
int FunctionsBoolean<E>::arity_of(int function) {
	return 2;
}

#endif /* FUNCTIONS_BOOLEANFUNCTIONS_H_ */
