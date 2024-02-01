/*
 * Evolver.h
 *
 *  Created on: 14.01.2023
 *      Author: roman
 */

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

template<class E, class G, class F>
void Evolver<E, G, F>::resume(std::string &checkpoint) {
	if (this->parameters->is_print_configuration())
		this->print_configuration();

	int generation_number = this->algorithm->get_generation_number();

	std::cout << "Resuming job from checkpoint: " << checkpoint
			<< "at generation:" << generation_number << std::endl << std::endl;

	std::shared_ptr<std::ofstream> ofs;

	if (this->parameters->is_write_statfile()) {
		std::string statfile = this->statfile_name();
		ofs = std::make_shared<std::ofstream>(statfile,
				std::ofstream::out | std::fstream::trunc);
	}

	this->execute_job(1, ofs);

	ofs->close();

}

template<class E, class G, class F>
void Evolver<E, G, F>::run() {

	if (this->parameters->is_print_configuration())
		this->print_configuration();

	std::shared_ptr<std::ofstream> ofs;

	if (this->parameters->is_write_statfile()) {
		std::string statfile = this->statfile_name();
		ofs = std::make_shared<std::ofstream>(statfile,
				std::ofstream::out | std::fstream::trunc);
	}

	for (int job = 1; job <= this->num_jobs; job++) {

		if (job > 1) {
			this->population->reset();
			this->algorithm->reset();
		}

		this->execute_job(job, ofs);

		if (this->parameters->is_generate_random_seed()) {
			this->random->set_random_seed();
		}

	}

	ofs->close();
}

template<class E, class G, class F>
void Evolver<E, G, F>::execute_job(int job,
		std::shared_ptr<std::ofstream> ofs) {

	std::pair<int, F> result;
	std::stringstream ss;
	std::chrono::duration<double> duration;

	auto start = std::chrono::high_resolution_clock::now();
	result = algorithm->evolve();
	auto end = std::chrono::high_resolution_clock::now();

	duration = end - start;

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

	if (this->parameters->is_write_statfile()) {
		*ofs << ss.rdbuf();
	}

	if (!ofs->good()) {
		throw std::runtime_error("Error while writing benchmark file!");
	}

}

#endif /* EVOLVER_EVOLVER_H_ */
