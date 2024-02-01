/*
 * constants.h
 *
 *  Created on: Jan 17, 2024
 *      Author: kalkreuth
 */

#ifndef ERC_ERC_H_
#define ERC_ERC_H_

#include <memory>
#include "../random/Random.h"
#include "erc_types.h"

class ERC {
public:
	ERC();

	static const ERC_TYPE KOZA = 0;
	static const ERC_TYPE ONE = 1;
	static const ERC_TYPE TWO = 2;

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
