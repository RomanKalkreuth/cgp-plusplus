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

