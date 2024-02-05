//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: Evolver.h 
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//  Author(s): Anonymous
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef EVOLVER_EVOLVER_H_
#define EVOLVER_EVOLVER_H_

#include "../algorithm/EvolutionaryAlgorithm.h"
#include "../initializer/LogicSynthesisInitializer.h"
#include "../random/Random.h"

#include <fstream>
#include <sstream>
#include <chrono>
#include <memory>

using namespace std::chrono;

/// @brief Class to reprsent the evolver that manages the execution of an experiment
/// with multiple instances
/// @details Provides the functionality to execute and resume instances as well as for handling the 
/// output of immediate and final results
/// @tparam E Evalation type 
/// @tparam G Genome type 
/// @tparam F Fitness type
template<class E, class G, class F>
class Evolver {
private:

	int num_jobs;

	bool report_after_job;
	bool report_during_job;
	bool report_simple;
	EVAL_METHOD simple_report_type;

	std::shared_ptr<Initializer<E, G, F>> initializer;
	std::shared_ptr<Parameters> parameters;
	std::shared_ptr<Random> random;
	std::shared_ptr<Composite<E, G, F>> composite;

	std::shared_ptr<EvolutionaryAlgorithm<E, G, F>> algorithm;
	std::shared_ptr<AbstractPopulation<G, F>> population;

	void print_configuration();

public:
	Evolver(std::shared_ptr<Initializer<E, G, F>> p_initializer);
	virtual ~Evolver() = default;
	std::string statfile_name();
	void run();
	void execute_job(int job, std::shared_ptr<std::ofstream> ofs);
	void resume(std::string &checkpoint);
};

template<class E, class G, class F>
Evolver<E, G, F>::Evolver(std::shared_ptr<Initializer<E, G, F>> p_initializer) {

	if (p_initializer != nullptr) {
		initializer = p_initializer;
		composite = p_initializer->get_composite();
		random = composite->get_random();
	} else {
		throw std::invalid_argument("Nullpointer exception in evolver class!");
	}

	parameters = composite->get_parameters();
	num_jobs = parameters->get_num_jobs();
	report_after_job = this->parameters->is_report_after_job();
	report_during_job = this->parameters->is_report_during_job();
	report_simple = this->parameters->is_report_simple();

	if (report_simple) {
		simple_report_type = this->parameters->get_simple_report_type();
	}

	this->population = this->composite->get_population();
	this->algorithm = this->initializer->get_algorithm();
}

/// @brief Prints the CGP++ configuration that has been defined for the experiment
template<class E, class G, class F>
void Evolver<E, G, F>::print_configuration() {
	std::cout << "		CGP++ CONFIGURATION" << std::endl;
	std::cout << "-------------------------------------------------"
			<< std::endl;
	this->parameters->print();
	this->composite->print_functions();
	this->composite->print_constants();
	std::cout << std::endl;
	this->composite->print_operators();
	std::cout << std::endl;
	std::cout << "Algorithm: " << this->algorithm->get_name();
	std::cout << std::endl;
	std::cout << "-------------------------------------------------"
			<< std::endl;
	std::cout << std::endl;
}

/// @brief Generates an unique filename for the statfile with a timestamp 
/// @return Filename of the statfile
template<class E, class G, class F>
std::string Evolver<E, G, F>::statfile_name() {
	std::stringstream ss;
	if (this->parameters->is_write_statfile()) {
		uint64_t tstamp = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()).count();
		ss << this->parameters->STAT_FILE_DIR << tstamp << ".stat";
	}
	return ss.str();
}

/// @brief Resumes a run instance from a checkpoint 
/// @param checkpoint name of checkpoint file
template<class E, class G, class F>
void Evolver<E, G, F>::resume(std::string &checkpoint) {
	if (this->parameters->is_print_configuration())
		this->print_configuration();

	int generation_number = this->algorithm->get_generation_number();

	std::cout << "Resuming job from checkpoint: " << checkpoint
			<< "at generation:" << generation_number << std::endl << std::endl;

	std::shared_ptr<std::ofstream> ofs;

	// If statfile output is enabled, open the file for writing
	if (this->parameters->is_write_statfile()) {
		std::string statfile = this->statfile_name();
		ofs = std::make_shared<std::ofstream>(statfile,
				std::ofstream::out | std::fstream::trunc);
	}

	this->execute_job(1, ofs);

	ofs->close();

}

/// @brief  Runs the a experiment by executing the specified number of jobs.
template<class E, class G, class F>
void Evolver<E, G, F>::run() {

	// Print the configuration if specified
	if (this->parameters->is_print_configuration())
		this->print_configuration();

	std::shared_ptr<std::ofstream> ofs;

	// If statfile is enabled, open the file for writing
	if (this->parameters->is_write_statfile()) {
		std::string statfile = this->statfile_name();
		ofs = std::make_shared<std::ofstream>(statfile,
				std::ofstream::out | std::fstream::trunc);
	}

	// Run the evolutionary algorithm for each job
	for (int job = 1; job <= this->num_jobs; job++) {

		// Reset the population and algorithm for each job after the first
		if (job > 1) {
			this->population->reset();
			this->algorithm->reset();
		}

		this->execute_job(job, ofs);

		// Generate a new random seed if specified
		if (this->parameters->is_generate_random_seed()) {
			this->random->set_random_seed();
		}

	}

	ofs->close();
}

/// @brief Executes a specific job
/// @details Meausres the runtime of the execution of the EA. 
/// @param job The job number.
/// @param ofs A shared pointer to the output file stream.
template<class E, class G, class F>
void Evolver<E, G, F>::execute_job(int job,
		std::shared_ptr<std::ofstream> ofs) {

	std::pair<int, F> result;
	std::stringstream ss;
	std::chrono::duration<double> duration;

	// Start EA and measures the runtime
	auto start = std::chrono::high_resolution_clock::now();
	result = algorithm->evolve();
	auto end = std::chrono::high_resolution_clock::now();

	duration = end - start;

	// Do reporting after the job if desired. 
	if (this->report_after_job) {
		if (this->report_simple) {
			if (this->simple_report_type
					== this->parameters->FITNESS_EVALUATIONS_TO_TERMINATION) {
				ss << result.first << std::endl;
			} else {
				ss << result.second << std::endl;
			}
		} else {
			ss << "Job # " << job << " :: Evaluations: " << result.first
					<< " :: Best Fitness: " << result.second
					<< " :: Runtime (s): " << duration.count() << std::endl;
		}
	}

	if (this->report_during_job)
		std::cout << std::endl;

	std::cout << ss.str();

	if (this->report_during_job)
		std::cout << std::endl << std::endl;

	// Write to statfile if desired. 
	if (this->parameters->is_write_statfile()) {
		*ofs << ss.rdbuf();
	}

	if (!ofs->good()) {
		throw std::runtime_error("Error while writing benchmark file!");
	}

}

#endif /* EVOLVER_EVOLVER_H_ */
