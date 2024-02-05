## CGP++: Modern C++ Implementation of Cartesian Genetic Programming

CGP++ is a modern C++ implementation of Cartesian Genetic Programming (CGP). CGP is a graph-based
representation model for genetic programming. CGP++ implementation focuses on the standard acyclic graph representation of CGP
and provides a generic framework that enables flexible usage. The main motivation and philosophy behind CGP++ 
is to facilitate the discovery of new problem domains and the reproducibility of existing results. Based on object-oriented design,
CGP++ also provides a suitable framework that can be used to integrate new sophisticated methods that entail the implementation of complex architecture. 

### Key Features and Properties

- Generic design
- Checkpointing
- Concurrency support
- Mutation pipelining
- Crossover support
- Parameter interfaces for hyperparameter tuning 
- Benchmark utilities for symbolic regression and logic synthesis
  

### Requirements

Since CGP++ uses modern techniques of C++, it has to be compiled with a version of GCC that supports the C++17 standard. 

### Getting Started

#### Compilation 

We recommend compiling CGP++ using the G++ compiler with the following command: 
> g++ -std=c++17 -O3

### Basic Usage 

The compiled file can be used with the parameter file in the data/parfile folder and the 
following command line options.

- Commandline parametrization:
```
DATAFILE PARFILE options

 -a  - search algorithm: 0 = one-plus-lambda; 1 = mu-plus-lambda <br />
 -n  - number of function nodes  <br />
 -v  - number of variables  <br />
 -z  - number of constants  <br />
 -i  - number of inputs  <br />
 -o  - number of outputs  <br />
 -f  - number of functions  <br />
 -r  - maximum arity  <br />
 -p  - mutation rate  <br />
 -m  - number of parents (mu)  <br />
 -l  - number of offspring (lambda)  <br />
 -e  - maximal number of fitness evaluations  <br />
 -g  - goal (ideal) fitness  <br />
 -j  - number of jobs  <br />
 -s  - global seed  <br />
 -1  - duplication rate  <br />
 -2  - max duplication depth  <br />
 -3  - inversion rate  <br />
 -4  - max inversion depth <br />
```

- Parfile parametrization:
  
```
algorithm  -  0 = one-plus-lambda, 1 = mu-plus-lambda <br />

num_jobs  - type: integer <br />
num_function_nodes  - type: integer <br />
num_variables  - type: integer <br />
num_constants  - type: integer <br />
constant_type - 0 = Koza true <br />
num_outputs  -  type: integer <br />
num_functions  -  type: integer <br />
max_arity  -  type: integer <br />

num_parents  -  type: integer <br /> 
num_offspring  -  type: integer <br />

max_fitness_evaluations  -  type: integer  
ideal_fitness   -  type: generic <br />
minimizing_fitness  0 = false, 1 = true <br />

crossover_type  0 = false, 1 = true <br />
crossover_rate   -  type: float,  range: [0.0, 1.0]<br />

mutation_type  -   type: integer <br />
levels_back   -  type: integer <br />
probabilistic_point_mutation   - type: float, range: [0.0, 1.0]  <br />
single_active_gene_mutation   - type: float, range: [0.0, 1.0]  <br />
inversion_mutation  0 = deactivated, 1 = activated <br />
duplication_mutation  0 = deactivated, 1 = activated <br />
point_mutation_rate   - type: float,  range: [0.0, 1.0]
duplication_rate - type: float,  range: [0.0, 1.0]
inversion_rate  - type: float,  range: [0.0, 1.0]  
max_duplication_depth   - type: integer <br />
max_inversion_depth   - type: integer <br />

neutral_genetic_drift  0 = deactivated, 1 = activated
simple_report_type  - 0 = deactivated, 1 = activated
print_configuration  - 0 = deactivated, 1 = activated  
evaluate_expression	 - 0 = deactivated, 1 = activated

num_eval_threads  - type: integer   
generate_random_seed   -  0 = deactivated, 1 = activated
global_seed   -  type: long long

report_during_job  0 = deactivated, 1 = activated
report_after_job  0 = deactivated, 1 = activated
report_simple  0 = deactivated, 1 = activated
report_interval	 0 = deactivated, 1 = activated

checkpointing  0 = deactivated, 1 = activated
checkpoint_modulo  -  type: integer

write_statfile 0 = deactivated, 1 = activated
```
