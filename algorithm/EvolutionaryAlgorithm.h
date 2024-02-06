//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: EvolutionaryAlgorithm.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef ALGORITHM_EVOLUTIONARYALGORITHM_H_
#define ALGORITHM_EVOLUTIONARYALGORITHM_H_

#include "../parameters/Parameters.h"
#include "../mutation/Mutation.h"
#include "../recombination/Recombination.h"
#include "../random/Random.h"
#include "../representation/Individual.h"
#include "../problems/BlackBoxProblem.h"
#include "../population/AbstractPopulation.h"
#include "../checkpoint/Checkpoint.h"
#include "../composite/Composite.h"

#include <stdexcept>
#include <memory>
#include <thread>
#include <cmath>
#include <mutex>


/// @brief Abstract base class to represent an evolutionary algorithm (EA) 

///	@details 
///	Class will be used to implement various forms of EAs that are provided by CGP++. 
/// Provides the core functionality for EAs such as evaluation but also checkpointing 
/// and CGP decoding of the population. 

/// @tparam E Evaluation Type
/// @tparam G Genotype Type
/// @tparam F Fitness Type 
template<class E, class G, class F>
class EvolutionaryAlgorithm {
protected:
	long long max_fitness_evaluations;
	long long max_generations;
	int report_interval;
	int generation_number;
	int fitness_evaluations;
	int eval_chunk_size;

	bool report_during_job;
	bool is_ideal = false;
	bool checkpointing;
	int checkpoint_modulo;

	F ideal_fitness;
	F best_fitness;

	std::string name;

	std::shared_ptr<Random> random;
	std::shared_ptr<Parameters> parameters;

	std::shared_ptr<AbstractPopulation<G, F>> population;
	std::shared_ptr<Mutation<G, F>> mutation;
	std::shared_ptr<Recombination<G, F>> recombination;
	std::shared_ptr<Species<G>> species;

	std::shared_ptr<Fitness<F>> fitness;
	std::shared_ptr<Functions<E>> functions;
	std::shared_ptr<std::vector<E>> constants;
	std::shared_ptr<BlackBoxProblem<E, G, F>> problem;
	std::shared_ptr<Evaluator<E, G, F>> evaluator;

	std::shared_ptr<Checkpoint<E, G, F>> checkpoint;

	std::shared_ptr<Composite<E, G, F>> composite;
	std::vector<Individual<G, F>> offsprings;


	void report(int generation_number);
	void check_ideal(int generation_number);
	void check_checkpoint();
	void decode();
	void evaluate();
	void evaluate_concurrent();
	void evaluate_chunk(std::vector<std::shared_ptr<Individual<G, F> > > chunk,
			BlackBoxProblem<E, G, F> &p);
	void evaluate_consecutive();

	virtual void breed(int num_offspring) = 0;

public:
	EvolutionaryAlgorithm(std::shared_ptr<Composite<E, G, F>> p_composite);
	virtual ~EvolutionaryAlgorithm() = default;
	void reset();
	virtual std::pair<int, F> evolve()=0;
	virtual const std::string& get_name() const;
	int get_generation_number() const;
	void set_generation_number(int p_generation_number);

};


/// @brief Constructor that takes the element needed to run the EA from the composite.  
/// @param p_composite Pointer to the composite instance. 
template<class E, class G, class F>
EvolutionaryAlgorithm<E, G, F>::EvolutionaryAlgorithm(
		std::shared_ptr<Composite<E, G, F>> p_composite) {

	if (p_composite != nullptr) {
		this->composite = p_composite;
	} else {
		throw std::invalid_argument(
				"Nullpointer exception in algorithm class!");
	}

	parameters = composite->get_parameters();
	random = composite->get_random();
	population = composite->get_population();
	mutation = composite->get_mutation();
	recombination = composite->get_recombination();
	problem = composite->get_problem();
	fitness = composite->get_fitness();
	functions = composite->get_functions();
	constants = composite->get_constants();
	species = composite->get_species();
	evaluator = composite->get_evaluator();
	checkpoint = composite->get_checkpoint();

	max_fitness_evaluations = parameters->get_max_fitness_evaluations();
	max_generations = parameters->get_max_generations();
	ideal_fitness = parameters->get_ideal_fitness();

	report_interval = parameters->get_report_interval();
	report_during_job = parameters->is_report_during_job();

	fitness_evaluations = 0;
	generation_number = 1;

	eval_chunk_size = std::trunc(
			this->parameters->get_population_size()
					/ this->parameters->get_num_eval_threads());

	if (this->parameters->is_checkpointing()) {
		checkpointing = this->parameters->is_checkpointing();
		checkpoint_modulo = this->parameters->get_checkpoint_modulo();
	} else {
		checkpointing = false;
		checkpoint_modulo = -1;
	}
}


/// @brief Triggers the CGP decoding and starts the evaluation 
/// either in a consecutive or concurrent fashion  
template<class E, class G, class F>
void EvolutionaryAlgorithm<E, G, F>::evaluate() {

	this->decode();

	if (parameters->get_num_eval_threads() == 1) {
		this->evaluate_consecutive();
	} else {
		this->evaluate_concurrent();
	}
}

/// @brief Decodes the individuals of the CGP by calling the decoder of the evaluator 
template<class E, class G, class F>
void EvolutionaryAlgorithm<E, G, F>::decode() {
	std::shared_ptr<Individual<G, F>> individual;

	for (int i = 0; i < this->population->size(); i++) {
		individual = this->population->get_individual(i);
		this->evaluator->decode_path(individual);
	}
}

/// @brief Evaluates the individuals by using conurrency 
/// @details A thread pool is created by chunking the population 
/// and evaluating each chunk in each thread. The thread pool is then
/// synchronized. 
template<class E, class G, class F>
void EvolutionaryAlgorithm<E, G, F>::evaluate_concurrent() {

	std::shared_ptr<Individual<G, F>> individual;
	std::vector<std::thread> threads;

	int num_eval_threads = parameters->get_num_eval_threads();
	int num_individuals = this->population->size();

	int start;
	int n;

	for (int i = 0; i < num_eval_threads; i++) {

		start = i * this->eval_chunk_size;
		n = this->eval_chunk_size;

		if (i == num_eval_threads - 1 && (start + n) != num_individuals) {
			n = num_individuals - start;
		}

		std::vector<std::shared_ptr<Individual<G, F> > > chunk;
		BlackBoxProblem<E, G, F> *p = this->problem->clone();

		for (int i = start; i < start + n; i++) {
			individual = this->population->get_individual(i);
			chunk.push_back(individual);
		}

		std::thread t = std::thread([=]() {
			this->evaluate_chunk(chunk, *p);
		});
		threads.push_back(std::move(t));
	}

	for (auto &t : threads) {
		t.join();
	}
}

/// @brief Evaluates a chunk of individuals within a thread. 
template<class E, class G, class F>
void EvolutionaryAlgorithm<E, G, F>::evaluate_chunk(
		std::vector<std::shared_ptr<Individual<G, F> > > chunk,
		BlackBoxProblem<E, G, F> &p) {

	for (auto it = chunk.begin(); it != chunk.end(); ++it) {
		p.evaluate_individual(*it);
	}
}

/// @details Evaluates the population in a consectutive fashion. 
template<class E, class G, class F>
void EvolutionaryAlgorithm<E, G, F>::evaluate_consecutive() {
	std::shared_ptr<Individual<G, F>> individual;
	for (int i = 0; i < this->population->size(); i++) {
		individual = this->population->get_individual(i);
		this->problem->evaluate_individual(individual);
	}
}

/// @details Resets the number of generation and fitness evaluations. 
template<class E, class G, class F>
void EvolutionaryAlgorithm<E, G, F>::reset() {
	this->generation_number = 1;
	this->fitness_evaluations = 0;
}

/// @brief Reports the current status of the EA 
/// (genration number and best fitness found so far). 
template<class E, class G, class F>
void EvolutionaryAlgorithm<E, G, F>::report(int generation_number) {

	if (this->report_during_job) {
		if (generation_number % this->report_interval == 0) {
			std::cout << "Generation # " << this->generation_number
					<< " :: Best Fitness: " << this->best_fitness << std::endl;
		}
	}
}

/// @brief Checks for the predefined ideal fitness.  
template<class E, class G, class F>
void EvolutionaryAlgorithm<E, G, F>::check_ideal(int generation_number) {
	this->is_ideal = this->fitness->is_ideal(this->best_fitness);

	if (this->is_ideal) {
		if (this->report_during_job) {
			std::cout << "Ideal fitness has been reached in generation # "
					<< this->generation_number << std::endl;
		}
	}
}

/// @brief Checks whether a new checkpoing should be triggered. 
/// @details Checkpoint interval is predefined by a checkpoint modulo. 
template<class E, class G, class F>
void EvolutionaryAlgorithm<E, G, F>::check_checkpoint() {
	if (this->checkpointing) {
		if (this->generation_number % this->checkpoint_modulo == 0) {
			this->checkpoint->write(this->population, this->constants, this->generation_number);

			if (this->report_during_job) {
				std::cout << "Checkpoint written at generation # "
						<< this->generation_number << std::endl;
			}
		}
	}
}

// Getter and setter of EA class
// ------------------------------------------------------------------------------------------

template<class E, class G, class F>
const std::string& EvolutionaryAlgorithm<E, G, F>::get_name() const {
	return this->name;
}

template<class E, class G, class F>
int EvolutionaryAlgorithm<E, G, F>::get_generation_number() const {
	return this->generation_number;
}


template<class E, class G, class F>
void EvolutionaryAlgorithm<E, G, F>::set_generation_number(int p_generation_number) {
	generation_number = p_generation_number;
}

// ------------------------------------------------------------------------------------------


#endif /* ALGORITHM_EVOLUTIONARYALGORITHM_H_ */
