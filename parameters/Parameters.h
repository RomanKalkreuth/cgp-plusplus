//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File Parameters.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef PARAMETERS_PARAMETERS_H_
#define PARAMETERS_PARAMETERS_H_

typedef unsigned int EVAL_METHOD;
typedef unsigned int MUTATION_TYPE;
typedef unsigned int CROSSOVER_TYPE;
typedef unsigned int ALGORITHM;
typedef unsigned int PROBLEM;

#include <stdexcept>
#include <iostream>
#include <cassert>
#include <climits>
#include <memory>
#include <vector>

#include "../template/template_types.h"
#include "../constants/erc_types.h"

/// @brief Class to represent the CGP++ configuration
/// @details Provides getter and setter for the respective parameter and settings and 
/// severtal configuration types defined by typedef. 
class Parameters {
public:
	const EVAL_METHOD FITNESS_EVALUATIONS_TO_TERMINATION = 0;
	const EVAL_METHOD BEST_FITNESS_OF_RUN = 1;

	const MUTATION_TYPE PROBABILISTIC_POINT_MUTATION = 0;
	const MUTATION_TYPE SINGLE_ACTIVE_GENE_MUTATION = 1;
	const MUTATION_TYPE INVERSION_MUTATION = 2;
	const MUTATION_TYPE DUPLICATION_MUTATION = 3;

	const CROSSOVER_TYPE BLOCK_CROSSOVER = 0;
	const CROSSOVER_TYPE DISCRETE_CROSSOVER = 1;

	const ALGORITHM ONE_PLUS_LAMBDA = 0;
	const ALGORITHM MU_PLUS_LAMBDA = 1;

	const PROBLEM SYMBOLIC_REGRESSION = 0;
	const PROBLEM LOGIC_SYNTHESIS = 1;

	const std::string STAT_FILE_DIR = "data/statfiles/";
	const std::string CHECKPOINT_FILE_DIR = "data/checkpoints/";

private:

	int genome_size;
	int num_function_nodes;
	int num_inputs;
	int num_outputs;
	int num_variables;
	int num_functions;
	int num_constants;

	ERC_TYPE erc_type;

	int num_jobs;
	int num_eval_threads;
	int eval_chunk_size;

	int max_arity;

	long long max_fitness_evaluations;
	long long max_generations;

	long long global_seed;

	FITNESS_TYPE ideal_fitness;

	float mutation_rate;
	float crossover_rate;

	MUTATION_TYPE mutation_type;
	CROSSOVER_TYPE crossover_type;

	ALGORITHM algorithm;
	PROBLEM problem;

	std::shared_ptr<std::vector<MUTATION_TYPE>> mutation_operators;

	float inversion_rate;
	float duplication_rate;

	int max_inversion_depth;
	int max_duplication_depth;

	int population_size;
	int mu;
	int lambda;
	int levels_back;
	int num_offspring;
	int num_parents;

	bool neutral_genetic_drift;

	bool evaluate_expression;
	bool minimizing_fitness;
	bool report_during_job;
	bool report_after_job;
	bool report_simple;
	bool print_configuration;
	bool generate_random_seed;
	bool write_statfile;
	bool checkpointing;

	int report_interval;
	int checkpoint_modulo;
	int simple_report_type;

public:
	Parameters();
	virtual ~Parameters() = default;

	void print();

	void set_eval_chunk_size();
	int get_eval_chunk_size() const;

	void set_genome_size();

	int get_genome_size() const;
	void set_genome_size(int p_genome_size);

	int get_max_arity() const;
	void set_max_arity(int p_max_arity);

	int get_num_functions() const;
	void set_num_functions(int p_num_functions);

	int get_num_constants() const;
	void set_num_constants(int p_num_constants);

	ERC_TYPE get_erc_type() const;
	void set_erc_type(ERC_TYPE p_erc_type);

	int get_num_variables() const;
	void set_num_variables(int p_num_variables);

	int get_num_inputs() const;
	void set_num_inputs(int p_num_inputs);

	int get_num_function_nodes() const;
	void set_num_function_nodes(int p_num_nodes);

	int get_num_outputs() const;
	void set_num_outputs(int p_num_outputs);

	float get_mutation_rate() const;
	void set_mutation_rate(float p_mutation_rate);

	float get_crossover_rate() const;
	void set_crossover_rate(float p_crossover_rate);

	CROSSOVER_TYPE get_crossover_type() const;
	void set_crossover_type(CROSSOVER_TYPE p_crossover_type);

	MUTATION_TYPE get_mutation_type() const;
	void set_mutation_type(MUTATION_TYPE p_mutation_type);


	FITNESS_TYPE get_ideal_fitness() const;
	void set_ideal_fitness(FITNESS_TYPE p_ideal_fitness);

	long long get_max_fitness_evaluations() const;
	void set_max_fitness_evaluations(long long p_max_fitness_evaluations);

	long long get_max_generations() const;
	void set_max_generations(long long p_max_generations);

	int get_population_size() const;
	void set_population_size(int p_population_size);

	void set_mu(int p_mu);
	int get_mu() const;

	void set_lambda(int p_lambda);
	int get_lambda() const;

	int get_num_parents() const;
	void set_num_parents(int p_num_parents);

	int get_num_offspring() const;
	void set_num_offspring(int p_num_offspring);

	int get_levels_back() const;
	void set_levels_back(int p_levels_back);

	bool is_minimizing_fitness() const;
	void set_minimizing_fitness(bool p_miniming_fitness);

	int get_num_jobs() const;
	void set_num_jobs(int p_num_jobs);

	int get_num_eval_threads() const;
	void set_num_eval_threads(int p_num_eval_threads);

	int get_report_interval() const;
	void set_report_interval(int p_report_interval);

	bool is_report_during_job() const;
	void set_report_during_job(bool p_report_during_job);

	bool is_report_after_job() const;
	void set_report_after_job(bool p_report_after_job);

	bool is_report_simple() const;
	void set_report_simple(bool p_report_simple);

	int get_simple_report_type() const;
	void set_simple_report_type(int p_simple_report_type);

	bool is_evaluate_expression() const;
	void set_evaluate_expression(bool p_evaluate_expression);

	bool is_neutral_genetic_drift() const;
	void set_neutral_genetic_drift(bool p_neutral_genetic_drift);

	bool is_print_configuration() const;
	void set_print_configuration(bool p_print_parameters);

	long long get_global_seed() const;
	void set_global_seed(long long globalSeed);

	const std::shared_ptr<std::vector<MUTATION_TYPE> >& get_mutation_operators() const;
	void set_mutation_operators(const std::shared_ptr<std::vector<MUTATION_TYPE> > &p_mutation_operators);

	ALGORITHM get_algorithm() const;
	void set_algorithm(ALGORITHM p_algorithm);

	bool is_generate_random_seed() const;
	void set_generate_random_seed(bool p_generate_random_seed);

	bool is_write_statfile() const;
	void set_write_statfile(bool p_write_output_file);

	bool is_checkpointing() const;
	void set_checkpointing(bool p_checkpointing);

	int get_checkpoint_modulo() const;
	void set_checkpoint_modulo(int p_checkpoint_modulo);

	float get_inversion_rate() const;
	void set_inversion_rate(float p_inversion_rate);

	float get_duplication_rate() const;
	void set_duplication_rate(float p_duplication_rate);

	int get_max_inversion_depth() const;
	void set_max_inversion_depth(int p_max_inversion_depth);

	int get_max_duplication_depth() const;
	void set_max_duplication_depth(int p_max_duplication_depth);
	PROBLEM get_problem() const;
	void set_problem(PROBLEM problem);
};

#endif /* PARAMETERS_PARAMETERS_H_ */
