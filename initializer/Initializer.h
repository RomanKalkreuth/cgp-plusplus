//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: Initializer.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef INITIALIZER_INITIALIZER_H_
#define INITIALIZER_INITIALIZER_H_

#include <iostream>
#include <stdexcept>

#include "../random/Random.h"
#include "../parameters/Parameters.h"
#include "../representation/Individual.h"
#include "../representation/Species.h"
#include "../evaluator/Evaluator.h"
#include "../fitness/Fitness.h"
#include "../mutation/Mutation.h"
#include "../problems/BlackBoxProblem.h"
#include "../algorithm/EvolutionaryAlgorithm.h"
#include "../algorithm/OnePlusLambda.h"
#include "../algorithm/MuPlusLambda.h"
#include "../composite/Composite.h"
#include "../population/StaticPopulation.h"
#include "../functions/Functions.h"
#include "../constants/ERC.h"
#include "../checkpoint/Checkpoint.h"

/// @brief Initializer class for CGP++ that sets up central elements requred to set 
/// up a run 
/// @details Used to initialize elements such as composite, algorithm, constants, \
/// checkpointer, problem and functions. The objects are instantiated with the make_shared function
/// to wrap them as shared pointers 
/// @tparam E Evaluation Type
/// @tparam G Genotype Type
/// @tparam F Fitness Type 
template<class E, class G, class F>
class Initializer {
protected:
	std::string benchmark_file;
	std::shared_ptr<Parameters> parameters;
	std::shared_ptr<Composite<E, G, F>> composite;
	std::shared_ptr<Evaluator<E, G, F>> evaluator;
	std::shared_ptr<EvolutionaryAlgorithm<E, G, F>> algorithm;
	std::shared_ptr<Checkpoint<E, G, F>> checkpoint;
	std::shared_ptr<BlackBoxProblem<E, G, F>> problem;
	std::shared_ptr<Functions<E>> functions;
	std::shared_ptr<std::vector<E>> constants;

public:
	Initializer();
	Initializer(const std::string &p_benchmark_file);
	virtual ~Initializer() = default;
	const std::shared_ptr<Composite<E, G, F> >& get_composite() const;
	const std::shared_ptr<EvolutionaryAlgorithm<E, G, F> >& get_algorithm() const;
	const std::shared_ptr<Parameters>& get_parameters() const;

	void init_comandline_parameters(int p_algorithm, int p_num_nodes,
			int p_num_variables, int p_num_constants, int p_num_outputs,
			int p_num_functions, int p_max_arity, int p_num_parents,
			int p_num_offspring, float p_mutation_rate,
			long long p_max_fitness_evaluations, F p_ideal_fitness,
			int p_num_jobs, long long p_global_seed, float p_duplication_rate,
			int p_max_duplication_depth, float p_inversion_rate,
			int p_max_inversion_depth, float p_crossover_rate,
			int p_levels_back);

	void init_parfile_parameters(std::string parfile_path);
	void init_composite();
	void init_erc();
	void init_algorithm();
	void init_checkpoint();
	void init_checkpoint_file(std::string &checkpoint_file);
	virtual void init_problem() = 0;
	virtual void init_functions() = 0;
};

/// @brief Constructor that instantiates the parameter object. 
template<class E, class G, class F>
Initializer<E, G, F>::Initializer() {
	parameters = std::make_shared<Parameters>();
}

/// @brief Overloaded constructor that sets the passed bnechmark file. 
template<class E, class G, class F>
Initializer<E, G, F>::Initializer(const std::string &p_benchmark_file) :
		Initializer() {

	if (!p_benchmark_file.empty()) {
		this->benchmark_file = p_benchmark_file;
	} else {
		throw std::invalid_argument(
				"Empty file path string in initializer class!");
	}

}

/// @brief Reads the parameters from the parameter file. 
/// @details The parameters read from the file are either set as values or states. 
/// Sets the respective parameters in the parameter object. 
/// @param parfile_path path to the parameter file 
template<class E, class G, class F>
void Initializer<E, G, F>::init_parfile_parameters(std::string parfile_path) {

	if (parfile_path.size() == 0) {
		throw std::runtime_error("File path is an empty string!");
	}

	std::string extension = std::filesystem::path(parfile_path).extension();

	if (extension != ".params") {
		throw std::runtime_error("Method only accepts param files!");
	}

	std::shared_ptr<std::vector<MUTATION_TYPE>> mutation_operators =
			parameters->get_mutation_operators();

	std::ifstream ifs;
	ifs.open(parfile_path, ios_base::in | ios_base::binary);

	if (ifs.is_open()) {

		std::string parameter;
		double value;

		while (ifs >> parameter >> value) {

			if (!ifs.good()) {
				throw std::runtime_error("Error while reading parameter file!");
			}

			bool state = (value == 1 ? true : false);

			if (parameter == "algorithm") {
				this->parameters->set_algorithm(value);
			} else if (parameter == "num_function_nodes") {
				this->parameters->set_num_function_nodes(value);
			} else if (parameter == "num_variables") {
				this->parameters->set_num_variables(value);
			} else if (parameter == "num_outputs") {
				this->parameters->set_num_outputs(value);
			} else if (parameter == "num_functions") {
				this->parameters->set_num_functions(value);
			} else if (parameter == "num_constants") {
				this->parameters->set_num_constants(value);
			} else if (parameter == "constant_type") {
				this->parameters->set_erc_type(value);
			} else if (parameter == "max_arity") {
				this->parameters->set_max_arity(value);
			} else if (parameter == "max_fitness_evaluations") {
				this->parameters->set_max_fitness_evaluations(value);
			} else if (parameter == "num_jobs") {
				this->parameters->set_num_jobs(value);
			} else if (parameter == "num_offspring") {
				this->parameters->set_num_offspring(value);
			} else if (parameter == "num_parents") {
				this->parameters->set_num_parents(value);
			} else if (parameter == "ideal_fitness") {
				this->parameters->set_ideal_fitness(value);
			} else if (parameter == "minimizing_fitness") {
				this->parameters->set_minimizing_fitness(state);
			} else if (parameter == "probabilistic_point_mutation"
					&& state == true) {
				mutation_operators->push_back(
						parameters->PROBABILISTIC_POINT_MUTATION);
			} else if (parameter == "single_active_gene_mutation"
					&& state == true) {
				mutation_operators->push_back(
						parameters->SINGLE_ACTIVE_GENE_MUTATION);
			} else if (parameter == "inversion_mutation" && state == true) {
				mutation_operators->push_back(parameters->INVERSION_MUTATION);
			} else if (parameter == "duplication_mutation" && state == true) {
				mutation_operators->push_back(parameters->DUPLICATION_MUTATION);
			} else if (parameter == "point_mutation_rate") {
				this->parameters->set_mutation_rate(value);
			} else if (parameter == "crossover_type") {
				this->parameters->set_crossover_type(value);
			} else if (parameter == "crossover_rate") {
				this->parameters->set_crossover_rate(value);
			} else if (parameter == "duplication_rate") {
				this->parameters->set_duplication_rate(value);
			} else if (parameter == "inversion_rate") {
				this->parameters->set_inversion_rate(value);
			} else if (parameter == "max_duplication_depth") {
				this->parameters->set_max_duplication_depth(value);
			} else if (parameter == "max_inversion_depth") {
				this->parameters->set_max_inversion_depth(value);
			} else if (parameter == "print_configuration") {
				this->parameters->set_print_configuration(state);
			} else if (parameter == "evaluate_expression") {
				this->parameters->set_evaluate_expression(state);
			} else if (parameter == "report_during_job") {
				this->parameters->set_report_during_job(state);
			} else if (parameter == "report_after_job") {
				this->parameters->set_report_after_job(state);
			} else if (parameter == "report_simple") {
				this->parameters->set_report_simple(state);
			} else if (parameter == "report_interval") {
				this->parameters->set_report_interval(value);
			} else if (parameter == "simple_report_type") {
				this->parameters->set_simple_report_type(value);
			} else if (parameter == "generate_random_seed") {
				this->parameters->set_generate_random_seed(state);
			} else if (parameter == "write_statfile") {
				this->parameters->set_write_statfile(state);
			} else if (parameter == "global_seed") {
				this->parameters->set_global_seed(value);
			} else if (parameter == "num_eval_threads") {
				this->parameters->set_num_eval_threads(value);
			} else if (parameter == "checkpointing") {
				this->parameters->set_checkpointing(state);
			} else if (parameter == "checkpoint_modulo") {
				this->parameters->set_checkpoint_modulo(value);
			}
		}
	} else {
		throw std::runtime_error("Error opening PAR file!");
	}
}

/// @brief Initializes the commandline parameters that have been read in the main function. 
/// @details Sets the respective parameters in the parameter object. 
template<class E, class G, class F>
void Initializer<E, G, F>::init_comandline_parameters(int p_algorithm,
		int p_num_nodes, int p_num_variables, int p_num_constants,
		int p_num_outputs, int p_num_functions, int p_max_arity,
		int p_num_parents, int p_num_offspring, float p_mutation_rate,
		long long p_max_fitness_evaluations, F p_ideal_fitness, int p_num_jobs,
		long long p_global_seed, float p_duplication_rate,
		int p_max_duplication_depth, float p_inversion_rate,
		int p_max_inversion_depth, float p_crossover_rate, int p_levels_back) {

	this->parameters->set_num_function_nodes(p_num_nodes);
	this->parameters->set_num_variables(p_num_variables);
	this->parameters->set_num_constants(p_num_constants);
	this->parameters->set_num_outputs(p_num_outputs);
	this->parameters->set_num_functions(p_num_functions);
	this->parameters->set_max_arity(p_max_arity);

	this->parameters->set_num_parents(p_num_parents);
	this->parameters->set_num_offspring(p_num_offspring);
	this->parameters->set_mutation_rate(p_mutation_rate);
	this->parameters->set_crossover_rate(p_crossover_rate);

	this->parameters->set_levels_back(p_levels_back);
	this->parameters->set_algorithm(p_algorithm);

	this->parameters->set_max_fitness_evaluations(p_max_fitness_evaluations);
	this->parameters->set_ideal_fitness(p_ideal_fitness);
	this->parameters->set_num_jobs(p_num_jobs);

	this->parameters->set_genome_size();
	this->parameters->set_population_size(p_num_parents + p_num_offspring);
	this->parameters->set_global_seed(p_global_seed);
	this->parameters->set_eval_chunk_size();

	if (p_duplication_rate >= 0) {
		this->parameters->set_duplication_rate(p_duplication_rate);
	}

	if (p_inversion_rate >= 0) {
		this->parameters->set_inversion_rate(p_inversion_rate);
	}

	if (p_max_duplication_depth > 0) {
		this->parameters->set_max_duplication_depth(p_max_duplication_depth);
	}

	if (p_max_inversion_depth > 0) {
		this->parameters->set_max_inversion_depth(p_max_inversion_depth);
	}

}

/// @brief Inits the number of ERC's according to the predefined type. 
template<class E, class G, class F>
void Initializer<E, G, F>::init_erc() {
	int num_constants = this->parameters->get_num_constants();
	ERC_TYPE type = this->parameters->get_erc_type();

	std::shared_ptr<Random> random = this->composite->get_random();
	std::shared_ptr<std::vector<E>> constants =
			this->composite->get_constants();

	for (int i = 0; i < num_constants; i++) {
		E erc = ERC::generate_ERC(random, type);
		constants->push_back(erc);
	}

	this->constants = constants;
}

/// @brief Inits the composite class 
template<class E, class G, class F>
void Initializer<E, G, F>::init_composite() {
	this->composite = std::make_shared<Composite<E, G, F>>(parameters,
			functions);
	this->composite->set_problem(problem);
	this->evaluator = composite->get_evaluator();
}

/// @brief Inits the evolutionary algorithm 
/// @details The algorithm is instantiated according to the type setting 
/// in the parameter object. 
template<class E, class G, class F>
void Initializer<E, G, F>::init_algorithm() {
	if (this->parameters->get_algorithm()
			== this->parameters->ONE_PLUS_LAMBDA) {
		this->algorithm = std::make_shared<OnePlusLambda<E, G, F>>(
				this->composite);
	} else if (this->parameters->get_algorithm()
			== this->parameters->MU_PLUS_LAMBDA) {
		this->algorithm = std::make_shared<MuPlusLambda<E, G, F>>(
				this->composite);
	} else {
		throw std::invalid_argument("Unknown algorithm!");
	}
}

/// @brief Inits the checkpointer.
/// @details Sets the checkpointer in the composite. 
template<class E, class G, class F>
void Initializer<E, G, F>::init_checkpoint() {
	this->checkpoint = std::make_shared<Checkpoint<E, G, F>>(parameters);
	this->composite->set_checkpoint(this->checkpoint);
}

/// @brief Init a checkpoint from a the corresponding file in case its passed  to CGP++
/// @param checkpoint_file path to the checkpoint file 
template<class E, class G, class F>
void Initializer<E, G, F>::init_checkpoint_file(std::string &checkpoint_file) {
	int generation_number = this->checkpoint->load(
			this->composite->get_population(), this->composite->get_constants(),
			this->composite->get_random(), checkpoint_file);
	this->algorithm->set_generation_number(generation_number);
	std::cout << "Loaded chekpoint file " << checkpoint_file << std::endl
			<< std::endl;
}

// Getter of the initializer class 

template<class E, class G, class F>
const std::shared_ptr<Composite<E, G, F> >& Initializer<E, G, F>::get_composite() const {
	return composite;
}

template<class E, class G, class F>
const std::shared_ptr<EvolutionaryAlgorithm<E, G, F> >& Initializer<E, G, F>::get_algorithm() const {
	return algorithm;
}

template<class E, class G, class F>
const std::shared_ptr<Parameters>& Initializer<E, G, F>::get_parameters() const {
	return parameters;
}

#endif /* INITIALIZER_INITIALIZER_H_ */
