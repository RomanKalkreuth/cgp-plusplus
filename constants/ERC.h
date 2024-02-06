//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: ERC.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef ERC_ERC_H_
#define ERC_ERC_H_

#include <memory>
#include "../random/Random.h"
#include "erc_types.h"

/// @brief Represents an ERC (Ephemeral Random Constant) as used in GP.
class ERC {
public:
	ERC() = delete;

	// Defines the different types of ERC
	static const ERC_TYPE KOZA = 0;
	static const ERC_TYPE ONE = 1;
	static const ERC_TYPE TWO = 2;

	/// @brief Generate an ERC of the specified type
	/// @param p_random Pointer to the random number generator instace
 	/// @param type The type of ERC to generate
 	/// @return The generated ERC value
	static float generate_ERC(std::shared_ptr<Random> p_random, ERC_TYPE type) {
		switch (type) {
		case KOZA:
			return koza(p_random);
			break;

		case ONE:
			return one();
			break;

		case TWO:
			return two();
			break;

		default:
			throw std::invalid_argument("Unknown constant type!");
		}
	}

	/// @brief Get the name of the ERC typ
	/// @param type The type of ERC
	/// @return The name of the ERC type as string.
	static std::string erc_name(ERC_TYPE type) {
		switch (type) {
		case KOZA:
			return "KOZA";
			break;

		case ONE:
			return "ONE";
			break;

		case TWO:
			return "TWO";
			break;

		default:
			throw std::invalid_argument("Unknown constant type!");
		}

	}

	static float koza(std::shared_ptr<Random> p_random) {
		return p_random->random_float(-1.0, 1.0);
	}

	static float one() {
		return 1.0f;
	}

	static float two() {
		return 2.0f;
	}
};

#endif /* ERC_H_ */
