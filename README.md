## CGP++: Modern C++ Implementation of Cartesian Genetic Programming

CGP++ is a modern and versatile C++ implementation of Cartesian Genetic Programming (CGP). CGP++ provides a generic framework that enables flexible and versatile usage. 
The general motivation and philosophy behind CGP++ is to facilitate the discovery of new problem domains and the reproducibility of existing results. 
CGP itself is a graph-based representation model for genetic programming, a search heuristic used for program synthesis. Based on object-oriented design, CGP++ provides 
a suitable framework that can be used to integrate new sophisticated methods that entail the implementation of complex architecture. 
CGP++ is not only intended to be a implementation of CGP but rather to serve as a blueprint for a modern and general genetic programming toolkit based on C++.


### Key Features and Properties

- Generic design
- Checkpointing
- Concurrency support
- Mutation pipelining
- Crossover support
- Parameter interfaces for hyperparameter tuning 
- Benchmarking utilities
  - Benchmark generator for symbolic regression
  - PLU reader for logic synthesis benchmarks
  
### Requirements

Since CGP++ uses modern techniques of C++, it has to be compiled with a version of GCC that supports the C++17 standard. 
> g++17

### Getting Started

#### User Guide

The user guide provides supporting details for the usage of CGP++.

#### Compilation 

We recommend compiling CGP++ using the G++ compiler with the following command: 
> g++ -std=c++17 -O3

#### Build 

For Linux, a classical Unix ```makefile``` is provided in the build folder. The command```make all``` can be used to create the executable.  

For building CGP++ on Windows, we recommend, using popular choices such as: 
- Mingw-w64 (https://www.mingw-w64.org/)
- MSVC (https://visualstudio.microsoft.com/vs/features/cplusplus/)
- VSCode (https://code.visualstudio.com/docs/languages/cpp)

On MacOS, the use of clang (https://clang.llvm.org/) in combination with VSCode can be considered as an alternative to using G++:
https://code.visualstudio.com/docs/setup/mac

### Basic Usage

#### Configuration 

The compiled executable can be used with the parameter file in the ```data/parfile``` folder and the 
following commandline options. The data file, .plu file for logic synthesis and .dat file for symbolic regression, are
located in the ```data/``` folder. The data and parameter file are passed to CGP via commandline. Optionally, a checkpoint file
can be passed to CGP this way. The CGP and run specific options can be the passed to CGP via the commandline and parameterfile configuration. 

##### Commandline configuration:
```
usage: DATAFILE PARFILE CHECKPOINTFILE <options>

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
algorithm                        -  0 = one-plus-lambda, 1 = mu-plus-lambda 
levels_back                      -  type: integer

num_jobs                         -  type: integer 
num_function_nodes               -  type: integer 
num_variables                    -  type: integer
num_constants                    -  type: integer
constant_type                    -  0 = Koza 
num_outputs                      -  type: integer 
num_functions                    -  type: integer 
max_arity                        -  type: integer 

num_parents                      -  type: integer 
num_offspring                    -  type: integer

max_fitness_evaluations          -  type: integer  
ideal_fitness                    -  type: generic 
minimizing_fitness               -  0 = false, 1 = true 

crossover_type                   -  0 = block, 1 = discrete 
crossover_rate                   -  type: float,  range: [0.0, 1.0]

probabilistic_point_mutation     -  0 = deactivated, 1 = activated
single_active_gene_mutation      -  0 = deactivated, 1 = activated
inversion_mutation               -  0 = deactivated, 1 = activated
duplication_mutation             -  0 = deactivated, 1 = activated
point_mutation_rate              -  type: float,  range: [0.0, 1.0]
duplication_rate                 -  type: float,  range: [0.0, 1.0]
inversion_rate                   -  type: float,  range: [0.0, 1.0]  
max_duplication_depth            -  type: integer
max_inversion_depth              -  type: integer

neutral_genetic_drift            -  0 = deactivated, 1 = activated
simple_report_type               -  0 = deactivated, 1 = activated
print_configuration              -  0 = deactivated, 1 = activated  
evaluate_expression              -  0 = deactivated, 1 = activated

num_eval_threads                 -  type: integer   
generate_random_seed             -  0 = deactivated, 1 = activated
global_seed                      -  type: long long

report_during_job                -  0 = deactivated, 1 = activated
report_after_job                 -  0 = deactivated, 1 = activated
report_simple                    -  0 = deactivated, 1 = activated
report_interval	                 -  0 = deactivated, 1 = activated

checkpointing                    -  0 = deactivated, 1 = activated
checkpoint_modulo                -  type: integer

write_statfile                   -  0 = deactivated, 1 = activated
```

#### Examples

##### Logic Synthesis
The following call to CGP++ triggers a run with a simple 1-Bit adder benchmark by using a 1+1 ES as search algorithm
and a simple CGP with 10 function nodes and point mutation:

```./cgp data/plufiles/add1c.plu data/parfiles/cgp.params -a 0 -b 10 -n 10 -v 1 -z 0 -o 1 -f 4 -r 2 -m 1 -l 1 -p 0.1 -c 0.0 -e 1000000 -j 10 -g 0```

Template settings: 
```
typedef int EVALUATION_TYPE;
typedef int GENOME_TYPE;
typedef int FITNESS_TYPE;
```

##### Symbolic Regression
This call triggers a run with the simple Koza1 benchmark (quartic polynomial) by using a 4+8 ES as search algorithm and discrete recombination:

```./cgp data/datfiles/koza1.dat data/parfiles/cgp.params  -a 1 -b 10 -n 10  -v 1 -z 0 -o 1 -f 4 -r 2 -m 4 -l 8 -p 0.1 -c 0.5 -e 10000000  -j 10  -g 0.01```

Template settings: 
```
typedef float EVALUATION_TYPE;
typedef int GENOME_TYPE;
typedef float FITNESS_TYPE;
```

#### Template Settings 

Template types can be set in the ```template/templates_types.h``` file.  

### Tutorial and Publication

A corresponding paper that describes CGP++ in more detail is currently under review. The CGP++ team also works tirelessly
on a tutorial which will be released soon. 

### Exemplary Program Output

A program output could be, for example, the overview of the configuration of the parameters and the metrics of the respective instances 
such as number of fitness evaluations, best fitness and runtime.  

```
		CGP++ CONFIGURATION
-------------------------------------------------
Number of function nodes: 10
Levels back: 10

Number of functions: 4
Maximum arity: 2

Number of variables: 1
Number of constants: 1

Number of inputs: 2
Number of outputs: 1

Crossover rate: 0.5
Mutation rate: 0.1

Number of parents (mu): 4
Number of offspring (lambda): 8

Ideal fitness value: 0.01

Number of jobs: 100
Maximum number of fitness evaluations: 1000000000
Maximum number of generations: 125000000

Global seed: 1707214445176910306

Functions: [1] ADD  [2] SUB  [3] MUL  [4] DIV  
Constants: [1] -0.416173  

Recombination: Discrete Crossover
Mutation: [1] Probabilistic Point  [2] Inversion  [3] Duplication  

Algorithm: mu-plus-lambda
-------------------------------------------------

Job # 1 :: Evaluations: 1980264 :: Best Fitness: 2.08206e-05 :: Runtime (s): 40.5062
Job # 2 :: Evaluations: 167528 :: Best Fitness: 2.11298e-05 :: Runtime (s): 2.47478
Job # 3 :: Evaluations: 113000 :: Best Fitness: 2.10702e-05 :: Runtime (s): 2.098
Job # 4 :: Evaluations: 49800 :: Best Fitness: 2.11298e-05 :: Runtime (s): 0.590241
Job # 5 :: Evaluations: 8832 :: Best Fitness: 2.08057e-05 :: Runtime (s): 0.128364
```
### License 
CGP++ is free open-source with a BSD-style academic license (AFL 3.0).

### Acknowledgement

CGP++ is dedicated to Dr. Julian Francis Miller (1955 - 2022), who as
the founder of Cartesian Genetic Programming devoted a large part
of his scientific life to its proposal, development and analysis. With
the introduction of CGP++ we pay tribute to Julian’s pioneering
effort in the field of graph-based GP and acknowledge his lifework.

