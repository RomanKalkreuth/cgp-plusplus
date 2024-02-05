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
following command line options 

> DATAFILE PARFILE <options>
> -a <value>          algorithm
> -n <value>          number of function nodes
> -v <value>          number of variables
> -z <value>          number of constants
> -i <value>          number of inputs
> -o <value>          number of outputs
> -f <value>          number of functions
> -r <value>          maximum arity
> -p <value>          mutation rate
> -m <value>          number of parents (mu)
> -l <value>          number of offspring (lambda)
> -e <value>          maximal number of fitness evaluations
> -g <value>          goal (ideal) fitness
> -j <value>          number of jobs
> -s <value>          global seed
> -1 <value>          duplication rate
> -2 <value>          max duplication depth
> -3 <value>          inversion rate
> -4 <value>          max inversion depth


