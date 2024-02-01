/*
 * MutationPipeline.h
 *
 *  Created on: 20.01.2023
 *      Author: roman
 */

#ifndef MUTATION_MUTATIONPIPELINE_H_
#define MUTATION_MUTATIONPIPELINE_H_

#include <vector>
#include <memory>
#include <exception>

#include "../parameters/Parameters.h"
#include "../variation/UnaryOperator.h"
#include "../variation/mutation/Duplication.h"
#include "../variation/mutation/Inversion.h"
#include "../variation/mutation/ProbabilisticPoint.h"
#include "../variation/mutation/SingleActiveGene.h"

template<class G, class F>
class MutationPipeline {
private:
	std::shared_ptr<std::vector<std::shared_ptr<UnaryOperator<G, F>>>> pipeline;
	std::shared_ptr<std::vector<MUTATION_TYPE>> operators;
	std::shared_ptr<Parameters> parameters;
	std::shared_ptr<Random> random;
	std::shared_ptr<Species<G>> species;

public:

	MutationPipeline(std::shared_ptr<Parameters> p_parameters,
			std::shared_ptr<Random> p_random,
			std::shared_ptr<Species<G>> p_species);
	virtual ~MutationPipeline() = default;

	void init();
	void breed(std::shared_ptr<Individual<G, F>> ind);
	void print();

};

template<class G, class F>
MutationPipeline<G, F>::MutationPipeline(
		std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Random> p_random,
		std::shared_ptr<Species<G>> p_species) {

	if (p_parameters != nullptr && p_random != nullptr
			&& p_species != nullptr) {
		parameters = p_parameters;
		random = p_random;
		species = p_species;
		operators = p_parameters->get_mutation_operators();
	} else {
		throw std::invalid_argument(
				"Nullpointer exception in mutation pipeline class!");
	}

	pipeline = std::make_shared<
			std::vector<std::shared_ptr<UnaryOperator<G, F>>>>();

	this->init();

}

template<class G, class F>
void MutationPipeline<G, F>::init() {

	std::shared_ptr<UnaryOperator<G, F>> op;

	for (MUTATION_TYPE type : *operators) {

		if (type == parameters->PROBABILISTIC_POINT_MUTATION) {
			op = std::make_shared<ProbabilisticPoint<G, F>>(
					this->parameters, this->random, this->species);

			pipeline->push_back(op);

		} else if (type == parameters->SINGLE_ACTIVE_GENE_MUTATION) {
			op = std::make_shared<SingleActiveGene<G, F>>(
					this->parameters, this->random, this->species);
			pipeline->push_back(op);

		} else if (type == parameters->DUPLICATION_MUTATION) {
			op = std::make_shared<Duplication<G, F>>(
					this->parameters, this->random, this->species);
			pipeline->push_back(op);

		} else if (type == parameters->INVERSION_MUTATION) {
			op = std::make_shared<Inversion<G, F>>(
					this->parameters, this->random, this->species);
			pipeline->push_back(op);
		}
	}
}

template<class G, class F>
void MutationPipeline<G, F>::print() {
	int i = 1;
	for (auto it = this->pipeline->begin(); it != this->pipeline->end(); ++it) {
		std::cout<<"["<< i <<"] "<<(*it)->get_name()<<"  ";
		i++;
	}

}

template<class G, class F>
void MutationPipeline<G, F>::breed(std::shared_ptr<Individual<G, F>> ind) {
	for (auto it = this->pipeline->begin(); it != this->pipeline->end(); ++it) {
		it->get()->variate(ind);
	}
}



#endif /* MUTATION_MUTATIONPIPELINE_H_ */
