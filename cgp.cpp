//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File cgp.cpp
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// -===============================================================================

#include <string>
#include <memory>
#include <unistd.h>
#include <iostream>
#include <getopt.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <stdexcept>

#include "template/template_types.h"
#include "validation/Validation.h"
#include "evolver/Evolver.h"
#include "initializer/SymbolicRegressionInitializer.h"
#include "initializer/LogicSynthesisInitializer.h"
#include "initializer/BlackBoxInitializer.h"
#include "random/Random.h"

typedef unsigned int PROBLEM_TYPE;

void usage(const char *self) {
	std::cout << "usage: DATAFILE PARFILE [opt]CHECKPOINTFILE <options>"
			<< std::endl;
	std::cout
			<< "-a <value>          search algorithm: 0 - one-plus-lambda; 1 = mu-plus-lambda"
			<< std::endl;
	std::cout << "-b <value>          levels back" << std::endl;
	std::cout << "-n <value>          number of function nodes" << std::endl;
	std::cout << "-v <value>          number of variables" << std::endl;
	std::cout << "-z <value>          number of constants" << std::endl;
	std::cout << "-i <value>          number of inputs [opt]" << std::endl;
	std::cout << "-o <value>          number of outputs" << std::endl;
	std::cout << "-f <value>          number of functions" << std::endl;
	std::cout << "-r <value>          maximum arity" << std::endl;
	std::cout << "-p <value>          mutation rate" << std::endl;
	std::cout << "-c <value>          crossover rate" << std::endl;
	std::cout << "-m <value>          number of parents (mu)" << std::endl;
	std::cout << "-l <value>          number of offspring (lambda)"
			<< std::endl;
	std::cout << "-e <value>          maximal number of fitness evaluations"
			<< std::endl;
	std::cout << "-g <value>          goal (ideal) fitness" << std::endl;
	std::cout << "-j <value>          number of jobs" << std::endl;
	std::cout << "-s <value>          global seed" << std::endl;
	std::cout << "-1 <value>          duplication rate" << std::endl;
	std::cout << "-2 <value>          max duplication depth" << std::endl;
	std::cout << "-3 <value>          inversion rate" << std::endl;
	std::cout << "-4 <value>          max inversion depth" << std::endl;
	exit(1);
}

int main(int argcc, char **argvv, char **envp) {

	if (argcc < 3)
		usage(*argvv);

	const PROBLEM LOGIC_SYNTHESIS = 0;
	const PROBLEM SYMBOLIC_REGRESSION = 1;

	std::string data_file = argvv[1];
	std::string param_file = argvv[2];

	std::string checkpoint_file;
	std::string s;

	int num_nodes = -1;
	int num_inputs = -1;
	int num_outputs = -1;

	int num_variables = -1;
	int num_constants = -1;

	int num_parents = -1;
	int num_offspring = -1;
	int num_jobs = -1;
	int num_functions = -1;
	int max_arity = -1;

	int levels_back = -1;

	int algorithm = -1;

	float mutation_rate = -1;
	float crossover_rate = 0.0f;

	float duplication_rate = -1.0;
	int max_duplication_depth = -1;

	float inversion_rate = -1.0;
	int max_inversion_depth = -1;

	long long max_fitness_evaluations = -1;
	long long global_seed = -1;

	FITNESS_TYPE ideal_fitness = -1;

	int problem_type;

	s = argvv[1];

	// Validate the type of the passed datafile
	// ---------------------------------------------------------------------------------------
	if (s.find(".plu") != std::string::npos) {
		problem_type = LOGIC_SYNTHESIS;
	} else if (s.find(".dat") != std::string::npos) {
		problem_type = SYMBOLIC_REGRESSION;
	} else {
		throw std::invalid_argument("Datatype is not supported!");
	}

	if (argvv[3]) {
		s = argvv[3];
		if (s.find(".checkpoint") != std::string::npos) {
			checkpoint_file = s;
		}
	}


	// Get parameters from command line
	// ---------------------------------------------------------------------------------------
	char opt;
	while ((opt = getopt(argcc, argvv,
			"a:n:v:z:c:i:o:f:r:m:p:l:b:e:g:j:s:1:2:3:4")) != -1) {
		switch (opt) {

		case 'a':
			algorithm = atoi(optarg);
			break;

		case 'n':
			num_nodes = atoi(optarg);
			break;

		case 'v':
			num_variables = atoi(optarg);
			break;

		case 'z':
			num_constants = atoi(optarg);
			break;

		case 'i':
			num_inputs = atoi(optarg);
			break;

		case 'o':
			num_outputs = atoi(optarg);
			break;

		case 'f':
			num_functions = atoi(optarg);
			break;

		case 'r':
			max_arity = atoi(optarg);
			break;

		case 'p':
			mutation_rate = atof(optarg);
			break;

		case 'c':
			crossover_rate = atof(optarg);
			break;

		case 'm':
			num_parents = atoi(optarg);
			break;

		case 'l':
			num_offspring = atoi(optarg);
			break;

		case 'b':
			levels_back = atoi(optarg);
			break;

		case 'e':
			max_fitness_evaluations = atol(optarg);
			break;

		case 'g':
			ideal_fitness = atof(optarg);
			break;

		case 'j':
			num_jobs = atoi(optarg);
			break;

		case 's':
			global_seed = atol(optarg);
			break;

		case '1':
			duplication_rate = atof(optarg);
			break;

		case '2':
			max_duplication_depth = atoi(optarg);
			break;

		case '3':
			inversion_rate = atof(optarg);
			break;

		case '4':
			max_inversion_depth = atoi(optarg);
			break;

		default:
			usage(*argvv);
			exit(1);
		}

	}
	// ---------------------------------------------------------------------------------------


	// Validate the evaluation type for the problem domain
	// and create the initializer for the corresponding problems
	// ---------------------------------------------------------------------------------------
	std::shared_ptr<
			BlackBoxInitializer<EVALUATION_TYPE, GENOME_TYPE, FITNESS_TYPE>> initializer;

	if (problem_type == LOGIC_SYNTHESIS) {
		if constexpr (Validation::validate_ls_type()) {
			initializer = std::make_shared<
					LogicSynthesisInitializer<EVALUATION_TYPE, GENOME_TYPE,
							FITNESS_TYPE>>(data_file);
		} else {
			throw std::invalid_argument(
					"Evaluation type is not supported for logic synthesis!");
		}
	} else if (problem_type == SYMBOLIC_REGRESSION) {
		if constexpr (Validation::validate_sr_type()) {
			initializer = std::make_shared<
					SymbolicRegressionInitializer<EVALUATION_TYPE, GENOME_TYPE,
							FITNESS_TYPE>>(data_file);
		} else {
			throw std::invalid_argument(
					"Evaluation type is not supported for symbolic regression!");
		}
	}


	// Initialize the parameters
	// ---------------------------------------------------------------------------------------
	initializer->init_parfile_parameters(param_file);

	initializer->init_comandline_parameters(algorithm, num_nodes, num_variables,
			num_constants, num_outputs, num_functions, max_arity, num_parents,
			num_offspring, mutation_rate, max_fitness_evaluations,
			ideal_fitness, num_jobs, global_seed, duplication_rate,
			max_duplication_depth, inversion_rate, max_inversion_depth,
			crossover_rate, levels_back);


	// ---------------------------------------------------------------------------------------

	// Initialize the data and the elements used to run CGP
	// ---------------------------------------------------------------------------------------
	initializer->read_data();
	initializer->init_functions();
	initializer->init_composite();
	initializer->init_erc();
	initializer->init_problem();
	initializer->init_checkpoint();
	initializer->init_algorithm();


	// Check for a checkpoint file
	// ---------------------------------------------------------------------------------------
	if (checkpoint_file != "")
		initializer->init_checkpoint_file(checkpoint_file);

	// Create the evolver
	// ---------------------------------------------------------------------------------------
	std::shared_ptr<Evolver<EVALUATION_TYPE, GENOME_TYPE, FITNESS_TYPE>> evolver =
			std::make_shared<Evolver<EVALUATION_TYPE, GENOME_TYPE, FITNESS_TYPE>>(
					initializer);

	if (checkpoint_file == "") {
		evolver->run();
	} else {
		evolver->resume(checkpoint_file);
	}
}
