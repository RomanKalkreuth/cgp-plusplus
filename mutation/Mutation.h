/*
 * Mutation.h
 *
 *  Created on: 08.07.2021
 *      Author: roman
 */

#ifndef MUTATION_H_
#define MUTATION_H_

#include "../parameters/Parameters.h"
#include "../random/Random.h"
#include "../representation/Species.h"
#include "../representation/Individual.h"
#include "MutationPipeline.h"

#include <stdexcept>
#include <iostream>
#include <memory>

template<class G, class F>
class Mutation {
private:
	std::shared_ptr<Parameters> parameters;
	std::shared_ptr<Random> random;
	std::shared_ptr<Species<G>> species;
	float mutation_rate;
	MUTATION_TYPE mutation_type;

	std::shared_ptr<MutationPipeline<G,F>> pipeline;

public:
	Mutation(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Random> p_random,
			std::shared_ptr<Species<G>> p_species);
	virtual ~Mutation() = default;
	void mutate(std::shared_ptr<Individual<G, F>> parent);
	void print();
	const std::shared_ptr<MutationPipeline<G, F> >& get_pipeline() const;
	void set_pipeline(const std::shared_ptr<MutationPipeline<G, F> > &pipeline);
};

template<class G, class F>
Mutation<G, F>::Mutation(std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Random> p_random,
		std::shared_ptr<Species<G>> p_species) {

	if (p_parameters != nullptr) {
		this->parameters = p_parameters;
		this->mutation_rate = p_parameters->get_mutation_rate();
		this->mutation_type = p_parameters->get_mutation_type();
	} else {
		throw std::invalid_argument("p_parameters is null in mutation class!");
	}

	if (p_random != nullptr) {
		this->random = p_random;
	} else {
		throw std::invalid_argument("p_random is null in mutation class!");
	}

	if (p_species != nullptr) {
		this->species = p_species;
	} else {
		throw std::invalid_argument("p_species is null in mutation class!");
	}

	this->pipeline = std::make_shared<MutationPipeline<G,F>>(p_parameters, p_random, p_species);
}

template<class G, class F>
void Mutation<G, F>::mutate(std::shared_ptr<Individual<G, F>> parent) {
	this->pipeline->breed(parent);
}

template<class G, class F>
inline const std::shared_ptr<MutationPipeline<G, F> >& Mutation<G, F>::get_pipeline() const {
	return this->pipeline;
}

template<class G, class F>
inline void Mutation<G, F>::set_pipeline(
		const std::shared_ptr<MutationPipeline<G, F> > &p_pipeline) {
	this->pipeline = p_pipeline;
}

template<class G, class F>
void Mutation<G, F>::print() {
	std::cout<<"Mutation: ";
	this->pipeline->print();
}



#endif /* MUTATION_H_ */
