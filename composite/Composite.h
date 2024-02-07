//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: Composite.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// -===============================================================================
#ifndef COMPOSITE_COMPOSITE_H_
#define COMPOSITE_COMPOSITE_H_

#include <memory>

#include "../random/Random.h"
#include "../functions/Functions.h"
#include "../parameters/Parameters.h"
#include "../representation/Individual.h"
#include "../representation/Species.h"
#include "../population/StaticPopulation.h"
#include "../evaluator/Evaluator.h"
#include "../fitness/Fitness.h"
#include "../mutation/Mutation.h"
#include "../mutation/MutationPipeline.h"
#include "../recombination/Recombination.h"
#include "../problems/BlackBoxProblem.h"
#include "../benchmark/BenchmarkFileReader.h"

 /// @brief A composite class that represents a combination of various components used 
 /// for the execution of the evolutionary algorithm.
 /// @tparam E Evalation type 
 /// @tparam G Genome type 
 /// @tparam F Fitness type
template<class E, class G, class F>
class Composite {
private:
	void init();

	std::shared_ptr<Random> random;
	std::shared_ptr<Parameters> parameters;

	std::shared_ptr<StaticPopulation<G, F>> population;
	std::shared_ptr<Mutation<G,F>> mutation;
	std::shared_ptr<MutationPipeline<G, F>> mutation_pipeline;
	std::shared_ptr<Recombination<G,F>> recombination;
	std::shared_ptr<Species<G>> species;

	std::shared_ptr<Fitness<F>> fitness;

	std::shared_ptr<Functions<E>> functions;
	std::shared_ptr<std::vector<E>> constants;

	std::shared_ptr<BlackBoxProblem<E, G, F>> problem;
	std::shared_ptr<Evaluator<E, G, F>> evaluator;

	std::shared_ptr<Checkpoint<E, G, F>> checkpoint;


public:
	Composite(std::shared_ptr<Parameters> p_parameters, std::shared_ptr<Functions<E>> p_functions);
	virtual ~Composite() = default;

	void print_functions();
	void print_constants();
	void print_operators();

	const std::shared_ptr<Evaluator<E, G, F> >& get_evaluator() const;
	void set_evaluator(const std::shared_ptr<Evaluator<E, G, F> > &p_evaluator);
	const std::shared_ptr<Fitness<F> >& get_fitness() const;
	void set_fitness(const std::shared_ptr<Fitness<F> > &p_fitness);
	const std::shared_ptr<Functions<E> >& get_functions() const;
	void set_functions(const std::shared_ptr<Functions<E> > &p_functions);
	const std::shared_ptr<Mutation<G,F> >& get_mutation() const;
	const std::shared_ptr<Recombination<G,F> >& get_recombination() const;
	void set_mutation(const std::shared_ptr<Mutation<G,F> > &p_mutation);
	const std::shared_ptr<StaticPopulation<G, F> >& get_population() const;
	void set_recombination(
			const std::shared_ptr<Recombination<G,F> > &p_recombination);
	void set_population(
			const std::shared_ptr<StaticPopulation<G, F> > &p_population);
	const std::shared_ptr<BlackBoxProblem<E, G, F> >& get_problem() const;
	void set_problem(const std::shared_ptr<BlackBoxProblem<E, G, F> > &p_problem);
	const std::shared_ptr<Species<G> >& get_species() const;
	void set_species(const std::shared_ptr<Species<G> > &p_species);
	const std::shared_ptr<Parameters>& get_parameters() const;
	const std::shared_ptr<Random>& get_random() const;
	void set_random(const std::shared_ptr<Random> &p_random);
	const std::shared_ptr<std::vector<E> >& get_constants() const;
	void set_constants(const std::shared_ptr<std::vector<E> > &p_constants);
	const std::shared_ptr<Checkpoint<E, G, F> >& get_checkpoint() const;
	void set_checkpoint(const std::shared_ptr<Checkpoint<E, G, F> > &checkpoint);
};

template<class E, class G, class F>
Composite<E, G, F>::Composite(
		std::shared_ptr<Parameters> p_parameters,
		std::shared_ptr<Functions<E>> p_functions) {

	if (p_parameters != nullptr &&  p_functions != nullptr) {
		parameters = p_parameters;
		functions = p_functions;
	} else {
		throw std::invalid_argument("Nullpointer exception in mutation class!");
	}

	init();
}

/// @brief Initializes the Composite object by creating and initializing its member variables. 
template<class E, class G, class F>
void Composite<E, G, F>::init() {

	// Check if the random seed needs to be generated or if a predefined global seed is provided
	if (this->parameters->is_generate_random_seed()) {
		this->random = std::make_shared<Random>(this->parameters);
	} else {
		long long global_seed = this->parameters->get_global_seed();
		this->random = std::make_shared<Random>(global_seed, this->parameters);
	}

	this->species = std::make_shared<Species<G>>(this->random,
			this->parameters);
	this->population = std::make_shared<StaticPopulation<G, F>>(this->random,
			this->parameters);
	this->mutation = std::make_shared<Mutation<G,F>>(this->parameters,
			this->random, this->species);
	this->mutation_pipeline = std::make_shared<MutationPipeline<G,F>>(this->parameters,
				this->random, this->species);
	this->recombination = std::make_shared<Recombination<G,F>>(this->parameters,
				this->random, this->species);

	F ideal_fitness = this->parameters->get_ideal_fitness();

	this->fitness = std::make_shared<Fitness<F>>(this->parameters,
			ideal_fitness);
	this->fitness->set_minimize(parameters->is_minimizing_fitness());

	this->evaluator = std::make_shared<Evaluator<E, G, F>>(this->parameters,
			this->functions, this->species);

	this->constants = std::make_shared<std::vector<E>>();

}

/// @brief Prints out the constants
/// @details Only in case that constants exists
template<class E, class G, class F>
void Composite<E, G, F>::print_constants() {
	if(this->parameters->get_num_constants() == 0)
		return;
	std::cout<<"Constants: ";
	int i=1;
	for(auto it : *this->constants.get()) {
		std::cout<<"["<< i <<"] "<<it<<"  ";
		i++;
	}
	std::cout<<std::endl;
}

/// @brief Prints out the fucntion set
template<class E, class G, class F>
void Composite<E, G, F>::print_functions() {
	std::string function_name;
	int num_functions = this->parameters->get_num_functions();

	std::cout<<"Functions: ";
	for(int i=0; i<num_functions; i++) {
		function_name = this->functions->function_name(i);
		std::cout<<"["<< i+1 <<"] "<<function_name<<"  ";
	}

	std::cout<<std::endl;
}

/// @brief Prints out the variation operators
/// @details Crossover operator is only printed
/// when its recombination is used
template<class E, class G, class F>
void Composite<E, G, F>::print_operators() {
	if(this->parameters->get_crossover_rate() > 0.0)
		this->recombination->print();
	this->mutation->print();
	std::cout<<std::endl;
}


// Getters and setters of composite class
//------------------------------------------------------------------------------------------------
template<class E, class G, class F>
const std::shared_ptr<Evaluator<E, G, F> >& Composite<E, G, F>::get_evaluator() const {
	return evaluator;
}

template<class E, class G, class F>
void Composite<E, G, F>::set_evaluator(
		const std::shared_ptr<Evaluator<E, G, F> > &p_evaluator) {
	this->evaluator = p_evaluator;
}

template<class E, class G, class F>
const std::shared_ptr<Fitness<F> >& Composite<E, G, F>::get_fitness() const {
	return this->fitness;
}

template<class E, class G, class F>
void Composite<E, G, F>::set_fitness(
		const std::shared_ptr<Fitness<F> > &p_fitness) {
	this->fitness = p_fitness;
}

template<class E, class G, class F>
const std::shared_ptr<Functions<E> >& Composite<E, G, F>::get_functions() const {
	return this->functions;
}

template<class E, class G, class F>
void Composite<E, G, F>::set_functions(
		const std::shared_ptr<Functions<E> > &p_functions) {
	this->functions = p_functions;
}

template<class E, class G, class F>
const std::shared_ptr<Mutation<G,F> >& Composite<E, G, F>::get_mutation() const {
	return this->mutation;
}

template<class E, class G, class F>
void Composite<E, G, F>::set_mutation(
		const std::shared_ptr<Mutation<G,F> > &p_mutation) {
	this->mutation = p_mutation;
}

template<class E, class G, class F>
const std::shared_ptr<Recombination<G,F> >& Composite<E, G, F>::get_recombination() const {
	return this->recombination;
}

template<class E, class G, class F>
void Composite<E, G, F>::set_recombination(
		const std::shared_ptr<Recombination<G,F> > &p_recombination) {
	this->recombination = p_recombination;
}

template<class E, class G, class F>
const std::shared_ptr<StaticPopulation<G, F> >& Composite<E, G, F>::get_population() const {
	return this->population;
}

template<class E, class G, class F>
void Composite<E, G, F>::set_population(
		const std::shared_ptr<StaticPopulation<G, F> > &p_population) {
	this->population = p_population;
}

template<class E, class G, class F>
const std::shared_ptr<BlackBoxProblem<E, G, F> >& Composite<E, G, F>::get_problem() const {
	return this->problem;
}

template<class E, class G, class F>
void Composite<E, G, F>::set_problem(
		const std::shared_ptr<BlackBoxProblem<E, G, F> > &p_problem) {
	this->problem = p_problem;
}

template<class E, class G, class F>
const std::shared_ptr<Species<G> >& Composite<E, G, F>::get_species() const {
	return this->species;
}

template<class E, class G, class F>
const std::shared_ptr<Random>& Composite<E, G, F>::get_random() const {
	return this->random;
}

template<class E, class G, class F>
void Composite<E, G, F>::set_random(
		const std::shared_ptr<Random> &p_random) {
	this->random = p_random;
}

template<class E, class G, class F>
const std::shared_ptr<Parameters>& Composite<E, G, F>::get_parameters() const {
	return this->parameters;
}

template<class E, class G, class F>
void Composite<E, G, F>::set_species(
		const std::shared_ptr<Species<G> > &p_species) {
	this->species = p_species;
}

template<class E, class G, class F>
const std::shared_ptr<std::vector<E> >& Composite<E, G, F>::get_constants() const {
	return this->constants;
}

template<class E, class G, class F>
const std::shared_ptr<Checkpoint<E, G, F> >& Composite<E, G, F>::get_checkpoint() const {
	return this->checkpoint;
}

template<class E, class G, class F>
void Composite<E, G, F>::set_checkpoint(
		const std::shared_ptr<Checkpoint<E, G, F> > & p_checkpoint) {
	this->checkpoint = p_checkpoint;
}

template<class E, class G, class F>
void Composite<E, G, F>::set_constants(const std::shared_ptr<std::vector<E> > &p_constants) {
	this->constants = p_constants;
}
//------------------------------------------------------------------------------------------------

#endif /* COMPOSITE_COMPOSITE_H_ */
