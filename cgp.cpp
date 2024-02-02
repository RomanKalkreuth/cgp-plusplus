// 					CGP++: Modern C++ Implementation of CGP
// ===============================================================================
//	File
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2023
//
//  Author(s):
//
//	License:
// -===============================================================================

#include <string>
#include <memory>
#include <unistd.h>
#include <iostream>
#include <getopt.h>
#include <fstream>
#include <string>
#include <filesystem>

#include "template/template_types.h"
#include "evolver/Evolver.h"
#include "initializer/SymbolicRegressionInitializer.h"

#include "benchmark/symbolic_regression/DatasetGenerator.h"
#include "benchmark/symbolic_regression/ObjectiveFunctions.h"
#include "random/Random.h"

void usage(const char *self) {
	std::cout << "usage: DATAFILE PARFILE <options>" << std::endl;
	std::cout << "-a <value>          algorithm" << std::endl;
	std::cout << "-n <value>          number of function nodes" << std::endl;
	std::cout << "-v <value>          number of variables" << std::endl;
	std::cout << "-z <value>          number of constants" << std::endl;
	std::cout << "-i <value>          number of inputs" << std::endl;
	std::cout << "-o <value>          number of outputs" << std::endl;
	std::cout << "-f <value>          number of functions" << std::endl;
	std::cout << "-r <value>          maximum arity" << std::endl;
	std::cout << "-p <value>          mutation rate" << std::endl;
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

	std::string data_file = argvv[1];
	std::string param_file = argvv[2];

	std::string checkpoint_file;

	if (argvv[3]) {
		std::string s = argvv[3];
		if (s.find(".checkpoint") != std::string::npos) {
			checkpoint_file = s;
		}
	}

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

	//std::shared_ptr<
	//		LogicSynthesisInitializer<EVALUATION_TYPE, GENOME_TYPE,
	//				FITNESS_TYPE>> initializer = std::make_shared<
	//		LogicSynthesisInitializer<EVALUATION_TYPE, GENOME_TYPE,
	//				FITNESS_TYPE>>(data_file);

	std::shared_ptr<
			SymbolicRegressionInitializer<EVALUATION_TYPE, GENOME_TYPE,
					FITNESS_TYPE>> initializer = std::make_shared<
			SymbolicRegressionInitializer<EVALUATION_TYPE, GENOME_TYPE,
					FITNESS_TYPE>>(data_file);

	initializer->init_parfile_parameters(param_file);

	initializer->init_comandline_parameters(algorithm, num_nodes, num_variables,
			num_constants, num_outputs, num_functions, max_arity, num_parents,
			num_offspring, mutation_rate, max_fitness_evaluations,
			ideal_fitness, num_jobs, global_seed, duplication_rate,
			max_duplication_depth, inversion_rate, max_inversion_depth,
			crossover_rate, levels_back);

	initializer->read_data();
	initializer->init_functions();
	initializer->init_composite();
	initializer->init_erc();
	initializer->init_problem();
	initializer->init_checkpoint();
	initializer->init_algorithm();

	if(checkpoint_file != "")
		initializer->init_checkpoint_file(checkpoint_file);


	std::shared_ptr<Evolver<EVALUATION_TYPE, GENOME_TYPE, FITNESS_TYPE>> evolver =
			std::make_shared<Evolver<EVALUATION_TYPE, GENOME_TYPE, FITNESS_TYPE>>(
					initializer);

	if(checkpoint_file == "") {
		evolver->run();
	}
	else {
		evolver->resume(checkpoint_file);
	}
}
