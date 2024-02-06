//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: UnaryOperator.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef VARIATION_UNARYOPERATOR_H_
#define VARIATION_UNARYOPERATOR_H_

#include "GeneticOperator.h"
#include "../representation/Individual.h"
#include <memory>

/// @brief Abstract base class to represent a unary genetic operator such as mutation. 
/// @tparam G Genome type 
/// @tparam F Fitness type
template <class G, class F>
class UnaryOperator : public GeneticOperator<G,F> {
public:
	UnaryOperator(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Random> p_random,
			std::shared_ptr<Species<G>> p_species) : GeneticOperator<G,F>(p_parameters, p_random, p_species) {}
	virtual ~UnaryOperator() = default;
	virtual void variate(std::shared_ptr<Individual<G, F>> individual) = 0;
};

#endif /* VARIATION_UNARYOPERATOR_H_ */
