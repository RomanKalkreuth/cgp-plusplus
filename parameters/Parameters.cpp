//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: Parameters.cpp
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//  Author(s): Anonymous
//
//	License: Academic Free License v. 3.0
// ================================================================================

#include "Parameters.h"

#include <cmath>

Parameters::Parameters() {
	algorithm = -1;
	num_functions = -1;
	num_constants = -1;
	erc_type = -1;
	num_function_nodes = -1;
	num_variables = -1;
	num_inputs = -1;
	num_outputs = -1;
	max_arity = -1;

	population_size = -1;
	num_offspring = -1;

	lambda = -1;
	mutation_rate = -1.0;
	crossover_rate = 0.0;

	inversion_rate = -1.0;
	duplication_rate = -1.0;

	max_inversion_depth = -1;
	max_duplication_depth = -1;

	num_jobs = 1;
	num_eval_threads = 1;
	max_generations = -1;
	max_fitness_evaluations = -1;
	ideal_fitness = 0;

	neutral_genetic_drift = true;

	evaluate_expression = false;
	minimizing_fitness = true;
	report_during_job = false;
	report_after_job = true;
	report_simple = true;
	report_interval = 1;
	print_configuration = false;
	write_statfile = false;
	checkpointing = false;

	simple_report_type = BEST_FITNESS_OF_RUN;

	mutation_operators = std::make_shared<std::vector<MUTATION_TYPE>>();
}

/// @brief Print the detailed configuration of CGP++ 
void Parameters::print() {

	std::cout << "Number of function nodes: " << this->num_function_nodes
			<< std::endl;
	std::cout << "Levels back: " << this->levels_back<< std::endl;
	std::cout << std::endl;
	std::cout << "Number of functions: " << this->num_functions << std::endl;
	std::cout << "Maximum arity: " << this->max_arity << std::endl;
	std::cout << std::endl;
	std::cout << "Number of variables: " << this->num_variables << std::endl;
	std::cout << "Number of constants: " << this->num_constants << std::endl;
	//std::cout << "Constant type: " << this->erc_type << std::endl;
	std::cout << std::endl;
	std::cout << "Number of inputs: " << this->num_inputs << std::endl;
	std::cout << "Number of outputs: " << this->num_outputs << std::endl;

	std::cout << std::endl;

	std::cout << "Crossover rate: " << this->crossover_rate << std::endl;
	std::cout << "Mutation rate: " << this->mutation_rate << std::endl;

	std::cout << std::endl;

	std::cout << "Number of parents (mu): " << this->num_parents << std::endl;
	std::cout << "Number of offspring (lambda): " << this->num_offspring
			<< std::endl;

	std::cout << std::endl;

	std::cout << "Ideal fitness value: " << ideal_fitness << std::endl;

	std::cout << std::endl;

	std::cout << "Number of jobs: " << num_jobs << std::endl;
	std::cout << "Maximum number of fitness evaluations: "
			<< max_fitness_evaluations << std::endl;
	std::cout << "Maximum number of generations: " << max_generations
			<< std::endl;

	std::cout << std::endl;
	std::cout << "Global seed: " << global_seed << std::endl;
	std::cout << std::endl;
}


// Getter and setter of parameter class
// ------------------------------------------------------------------------------------------

void Parameters::set_eval_chunk_size() {
	assert(this->num_eval_threads <= this->population_size);
	this->eval_chunk_size = std::trunc(
			this->population_size / this->num_eval_threads);
}

int Parameters::get_eval_chunk_size() const {
	return this->eval_chunk_size;
}

void Parameters::set_genome_size() {
	assert(this->num_function_nodes > 0);
	assert(this->max_arity > 0);
	assert(this->num_outputs > 0);
	this->genome_size = (this->num_function_nodes * (this->max_arity + 1))
			+ this->num_outputs;
}

int Parameters::get_genome_size() const {
	return this->genome_size;
}

int Parameters::get_max_arity() const {
	return this->max_arity;
}

void Parameters::set_max_arity(int p_max_arity) {
	assert(p_max_arity > 0);
	this->max_arity = p_max_arity;
}

int Parameters::get_num_variables() const {
	return this->num_variables;
}

void Parameters::set_num_variables(int p_num_variables) {
	assert(p_num_variables > 0);
	this->num_variables = p_num_variables;
	this->num_inputs = this->num_constants + this->num_variables;
}

int Parameters::get_num_constants() const {
	return this->num_constants;
}

void Parameters::set_num_constants(int p_num_constants) {
	assert(p_num_constants >= 0);
	this->num_constants = p_num_constants;
	this->num_inputs = this->num_constants + this->num_variables;
}

ERC_TYPE Parameters::get_erc_type() const {
	return this->erc_type;
}

void Parameters::set_erc_type(ERC_TYPE p_erc_type) {
	this->erc_type = p_erc_type;
}

int Parameters::get_num_inputs() const {
	return this->num_inputs;
}

void Parameters::set_num_inputs(int p_num_inputs) {
	this->num_inputs = p_num_inputs;
}

int Parameters::get_num_function_nodes() const {
	return this->num_function_nodes;
}

void Parameters::set_num_function_nodes(int p_num_function_nodes) {
	assert(p_num_function_nodes > 1);
	num_function_nodes = p_num_function_nodes;
}

int Parameters::get_num_outputs() const {
	return this->num_outputs;
}

void Parameters::set_num_outputs(int p_num_outputs) {
	this->num_outputs = p_num_outputs;
}

int Parameters::get_num_functions() const {
	return this->num_functions;
}

void Parameters::set_num_functions(int p_num_functions) {
	assert(p_num_functions > 0);
	this->num_functions = p_num_functions;
}

void Parameters::set_genome_size(int p_genome_size) {
	assert(p_genome_size > 0);
	this->genome_size = p_genome_size;
}

float Parameters::get_mutation_rate() const {
	return this->mutation_rate;
}

void Parameters::set_mutation_rate(float p_mutation_rate) {
	assert(p_mutation_rate > 0.0);
	this->mutation_rate = p_mutation_rate;
}

float Parameters::get_crossover_rate() const {
	return this->crossover_rate;
}

void Parameters::set_crossover_rate(float p_crossover_rate) {
	assert(p_crossover_rate >= 0.0);
	this->crossover_rate = p_crossover_rate;
}

CROSSOVER_TYPE Parameters::get_crossover_type() const {
	return this->crossover_type;
}

void Parameters::set_crossover_type(CROSSOVER_TYPE p_crossover_type) {
	assert(p_crossover_type >= 0);
	crossover_type = p_crossover_type;
}

FITNESS_TYPE Parameters::get_ideal_fitness() const {
	return ideal_fitness;
}

void Parameters::set_ideal_fitness(FITNESS_TYPE p_ideal_fitness) {
	this->ideal_fitness = p_ideal_fitness;
}

long long Parameters::get_max_fitness_evaluations() const {
	return max_fitness_evaluations;
}

void Parameters::set_max_fitness_evaluations(
		long long p_max_fitness_evaluations) {
	assert(
			p_max_fitness_evaluations > 0 && p_max_fitness_evaluations <= LLONG_MAX);
	assert(this->num_offspring > 0);

	this->max_fitness_evaluations = p_max_fitness_evaluations;
	this->max_generations = this->max_fitness_evaluations / this->num_offspring;
}

long long Parameters::get_max_generations() const {
	return this->max_generations;
}

void Parameters::set_max_generations(long long p_max_generations) {
	assert(p_max_generations > 0 && p_max_generations <= LLONG_MAX);
	assert(this->num_offspring > 0);
	this->max_generations = p_max_generations;
	this->max_fitness_evaluations = p_max_generations * this->num_offspring;
}

int Parameters::get_population_size() const {
	return this->population_size;
}

void Parameters::set_population_size(int p_population_size) {
	assert(p_population_size > 0);
	population_size = p_population_size;
}

int Parameters::get_lambda() const {
	return this->lambda;
}

void Parameters::set_lambda(int p_lambda) {
	assert(p_lambda > 0);
	this->lambda = p_lambda;
}

int Parameters::get_mu() const {
	return this->mu;
}

void Parameters::set_mu(int p_mu) {
	assert(p_mu > 0);
	this->mu = p_mu;
}

int Parameters::get_num_offspring() const {
	return this->num_offspring;
}

void Parameters::set_num_offspring(int p_num_offspring) {
	assert(p_num_offspring > 0);
	this->num_offspring = p_num_offspring;
	this->lambda = this->num_offspring;
}

int Parameters::get_num_parents() const {
	return this->num_parents;
}

void Parameters::set_num_parents(int p_num_parents) {
	assert(p_num_parents > 0);
	this->num_parents = p_num_parents;
	this->mu = p_num_parents;
}

bool Parameters::is_print_configuration() const {
	return this->print_configuration;
}

void Parameters::set_print_configuration(bool printParameters) {
	this->print_configuration = printParameters;
}

int Parameters::get_levels_back() const {
	return this->levels_back;
}

void Parameters::set_levels_back(int p_levels_back) {
	assert(p_levels_back > 0);
	this->levels_back = p_levels_back;
}

bool Parameters::is_minimizing_fitness() const {
	return this->minimizing_fitness;
}

void Parameters::set_minimizing_fitness(bool p_miniming_fitness) {
	this->minimizing_fitness = p_miniming_fitness;
}

int Parameters::get_num_jobs() const {
	return this->num_jobs;
}

int Parameters::get_num_eval_threads() const {
	return this->num_eval_threads;
}

int Parameters::get_report_interval() const {
	return this->report_interval;
}

bool Parameters::is_report_during_job() const {
	return this->report_during_job;
}

bool Parameters::is_report_after_job() const {
	return this->report_after_job;
}

bool Parameters::is_report_simple() const {
	return this->report_simple;
}

int Parameters::get_simple_report_type() const {
	return this->simple_report_type;
}

bool Parameters::is_evaluate_expression() const {
	return this->evaluate_expression;
}

bool Parameters::is_neutral_genetic_drift() const {
	return this->neutral_genetic_drift;
}

void Parameters::set_neutral_genetic_drift(bool p_neutral_genetic_drift) {
	this->neutral_genetic_drift = p_neutral_genetic_drift;
}

void Parameters::set_evaluate_expression(bool p_evaluate_expression) {
	this->evaluate_expression = p_evaluate_expression;
}

void Parameters::set_simple_report_type(int p_simple_report_type) {
	this->simple_report_type = p_simple_report_type;
}

void Parameters::set_report_simple(bool P_report_simple) {
	this->report_simple = P_report_simple;
}

void Parameters::set_report_after_job(bool p_report_after_job) {
	this->report_after_job = p_report_after_job;
}

void Parameters::set_report_during_job(bool p_report_during_job) {
	this->report_during_job = p_report_during_job;
}

void Parameters::set_report_interval(int p_report_interval) {
	assert(p_report_interval > 0);
	this->report_interval = p_report_interval;
}

void Parameters::set_num_jobs(int p_num_jobs) {
	assert(p_num_jobs > 0);
	this->num_jobs = p_num_jobs;
}

void Parameters::set_num_eval_threads(int p_num_eval_threads) {
	assert(p_num_eval_threads > 0);
	this->num_eval_threads = p_num_eval_threads;
}

long long Parameters::get_global_seed() const {
	return this->global_seed;
}

const std::shared_ptr<std::vector<MUTATION_TYPE> >& Parameters::get_mutation_operators() const {
	return this->mutation_operators;
}

MUTATION_TYPE Parameters::get_mutation_type() const {
	return this->mutation_type;
}

ALGORITHM Parameters::get_algorithm() const {
	return this->algorithm;
}

void Parameters::set_algorithm(ALGORITHM p_algorithm) {
	assert(p_algorithm >= 0);
	this->algorithm = p_algorithm;
}

void Parameters::set_mutation_type(MUTATION_TYPE p_mutation_type) {
	this->mutation_type = p_mutation_type;
}

bool Parameters::is_generate_random_seed() const {
	return this->generate_random_seed;
}

void Parameters::set_generate_random_seed(bool p_generate_random_seed) {
	generate_random_seed = p_generate_random_seed;
}

bool Parameters::is_write_statfile() const {
	return this->write_statfile;
}

void Parameters::set_write_statfile(bool p_write_stat_file) {
	this->write_statfile = p_write_stat_file;
}

void Parameters::set_mutation_operators(
		const std::shared_ptr<std::vector<MUTATION_TYPE> > &p_mutations_operators) {
	this->mutation_operators = p_mutations_operators;
}

void Parameters::set_global_seed(long long p_global_seed) {
	assert(p_global_seed > 0);
	this->global_seed = p_global_seed;
}

float Parameters::get_inversion_rate() const {
	return this->inversion_rate;
}

float Parameters::get_duplication_rate() const {
	return this->duplication_rate;
}

int Parameters::get_max_inversion_depth() const {
	return this->max_inversion_depth;
}

int Parameters::get_max_duplication_depth() const {
	return this->max_duplication_depth;
}

int Parameters::get_checkpoint_modulo() const {
	return this->checkpoint_modulo;
}

void Parameters::set_checkpoint_modulo(int p_checkpoint_modulo) {
	this->checkpoint_modulo = p_checkpoint_modulo;
}

void Parameters::set_max_duplication_depth(int p_max_duplication_depth) {
	assert(p_max_duplication_depth > 0);
	this->max_duplication_depth = p_max_duplication_depth;
}

void Parameters::set_max_inversion_depth(int p_max_inversion_depth) {
	assert(p_max_inversion_depth > 0);
	this->max_inversion_depth = p_max_inversion_depth;
}

void Parameters::set_duplication_rate(float p_duplication_rate) {
	assert(p_duplication_rate >= 0);
	this->duplication_rate = p_duplication_rate;
}

void Parameters::set_inversion_rate(float p_inversion_rate) {
	assert(p_inversion_rate >= 0);
	this->inversion_rate = p_inversion_rate;
}

bool Parameters::is_checkpointing() const {
	return this->checkpointing;
}

void Parameters::set_checkpointing(bool p_checkpointing) {
	this->checkpointing = p_checkpointing;
}

PROBLEM Parameters::get_problem() const {
	return problem;
}

void Parameters::set_problem(PROBLEM problem) {
	this->problem = problem;
}
// ------------------------------------------------------------------------------------------