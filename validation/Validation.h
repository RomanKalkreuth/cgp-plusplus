//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: Validation.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef VALIDATION_VALIDATION_H_
#define VALIDATION_VALIDATION_H_

#include <any>
#include "../template/template_types.h"

class Validation {
public:
	Validation();

	static bool constexpr validate_ls_type() {
		if constexpr (!std::is_same<int, EVALUATION_TYPE>::value) {
			if constexpr (!std::is_same<long, EVALUATION_TYPE>::value) {
				if constexpr (!std::is_same<unsigned int, EVALUATION_TYPE>::value) {
					if constexpr (!std::is_same<unsigned long, EVALUATION_TYPE>::value) {
						return false;
					}
				}
			}
		}
		return true;
	}

	static bool constexpr validate_sr_type() {
		if constexpr (!std::is_same<float, EVALUATION_TYPE>::value) {
			if constexpr (!std::is_same<double, EVALUATION_TYPE>::value) {
				return false;
			}
		}
		return true;
	}
};

#endif /* VALIDATION_VALIDATION_H_ */
