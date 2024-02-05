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
> g++17

### Getting Started

#### Compilation 

We recommend compiling CGP++ using the G++ compiler with the following command: 
> g++ -std=c++17 -O3

#### Build 

A classical makefile is provided in the ```build``` folder. A CMake makefile will be provided soon. 

### Basic Usage 

#### Configuration 

The compiled executable can be used with the parameter file in the ```data/parfile``` folder and the 
following command line options.

##### Commandline configuration:
```
DATAFILE PARFILE options

 -a  - search algorithm: 0 = one-plus-lambda; 1 = mu-plus-lambda 
 -n  - number of function nodes  
 -v  - number of variables  
 -z  - number of constants 
 -i  - number of inputs  
 -o  - number of outputs 
 -f  - number of functions 
 -r  - maximum arity
 -p  - mutation rate
 -c  - crossover rate 
 -m  - number of parents (mu) 
 -l  - number of offspring (lambda) 
 -e  - maximal number of fitness evaluations
 -g  - goal (ideal) fitness 
 -j  - number of jobs
 -s  - global seed
 -1  - duplication rate 
 -2  - max duplication depth 
 -3  - inversion rate 
 -4  - max inversion depth 
```

##### Parfile configuration:
  
```
algorithm  -  0 = one-plus-lambda, 1 = mu-plus-lambda 
levels_back   -  type: integer

num_jobs  - type: integer 
num_function_nodes  - type: integer 
num_variables  - type: integer
num_constants  - type: integer
constant_type - 0 = Koza 
num_outputs  -  type: integer 
num_functions  -  type: integer 
max_arity  -  type: integer 

num_parents  -  type: integer 
num_offspring  -  type: integer

max_fitness_evaluations  -  type: integer  
ideal_fitness   -  type: generic 
minimizing_fitness - 0 = false, 1 = true 

crossover_type  0 = block, 1 = discrete 
crossover_rate   -  type: float,  range: [0.0, 1.0]

probabilistic_point_mutation   - 0 = deactivated, 1 = activated
single_active_gene_mutation   - 0 = deactivated, 1 = activated
inversion_mutation  0 = deactivated, 1 = activated
duplication_mutation  0 = deactivated, 1 = activated
point_mutation_rate   - type: float,  range: [0.0, 1.0]
duplication_rate - type: float,  range: [0.0, 1.0]
inversion_rate  - type: float,  range: [0.0, 1.0]  
max_duplication_depth   - type: integer
max_inversion_depth   - type: integer

neutral_genetic_drift  - 0 = deactivated, 1 = activated
simple_report_type  - 0 = deactivated, 1 = activated
print_configuration  - 0 = deactivated, 1 = activated  
evaluate_expression  - 0 = deactivated, 1 = activated

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

#### Template Settings 

Template types can be set in the ```template/templates_types.h``` file.  

