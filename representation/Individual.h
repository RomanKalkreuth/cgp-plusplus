//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: Individual.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================


#ifndef REPRESENGAGION_INDIVIDUAL_H_
#define REPRESENGAGION_INDIVIDUAL_H_

#include "Species.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>

/// @brief Class to represent integer-based and real-encoded CGP individuals.
/// @details Provides methods to handle the genome, active nodes and symbolic expressions.
/// @tparam G Genome type 
/// @tparam F Fitness type
template<class G, class F>
class Individual: public Species<G> {
private:
	std::shared_ptr<std::vector<int>> active_nodes;
	std::shared_ptr<std::vector<std::string>> expressions;

	F fitness;
	bool evaluated = false;
public:
	Individual(std::shared_ptr<Random> p_random,
			std::shared_ptr<Parameters> p_parameters);
	Individual(std::shared_ptr<Individual<G, F>> individual);
	virtual ~Individual() = default;

	void reset_genome();
	void reset();
	Individual<G,F>* clone();

	void clear_active_nodes();
	void add_active_node(int node_num);
	void print_active_nodes();

	void clear_expressions();
	void add_expression(std::string expression);
	void print_expression();

	void print_genome();
	int random_value_closed_interval(int min, int max);
	std::string generate_expression();
	F get_fitness() const;

	void set_fitness(F p_fitness);
	bool is_evaluated() const;
	void set_evaluated(bool p_evaluated);

	void copy_genome(std::shared_ptr<G[]> p_genome);

	const std::string to_string(std::string delimiter=" ") const;
	const std::shared_ptr<std::vector<int> >& get_active_nodes() const;
	void set_active_nodes(const std::shared_ptr<std::vector<int> > &p_active_nodes);

	int num_active_nodes();

	const std::shared_ptr<std::vector<std::string>>& get_expressions() const;
	void set_expressions(const std::shared_ptr<std::vector<std::string>> &p_expressions);
};

/// @brief Constructor to intialize the expression and active node vectors
/// @param p_random shared pointer to random generator instance
/// @param p_parameters shared pointer to parameter object
template<class G, class F>
Individual<G,F>::Individual(std::shared_ptr<Random> p_random,
		std::shared_ptr<Parameters> p_parameters) :
		Species<G>(p_random, p_parameters) {

	std::shared_ptr<G[]> new_genome_ptr(new G[this->genome_size]());
	this->genome = new_genome_ptr;

	this->expressions = std::make_shared<std::vector<std::string>>();
	this->active_nodes = std::make_shared<std::vector<int>>();

	this->reset_genome();
}


/// @brief Copy constructor for deep cloning
/// @param individual individual to clone
template<class G, class F>
Individual<G,F>::Individual(std::shared_ptr<Individual<G, F>> individual) :
		Species<G>(individual->random, individual->parameters) {

	std::shared_ptr<G[]> new_genome_ptr(new G[this->genome_size](), std::default_delete<G[]>());
	this->genome = new_genome_ptr;

	std::shared_ptr<G[]> genome_ptr = individual->get_genome();
	std::copy( genome_ptr.get(), genome_ptr.get() + this->genome_size , this->genome.get());

	this->active_nodes =  std::make_shared<std::vector<int>>(*individual->active_nodes);
	this->expressions = std::make_shared<std::vector<std::string>>(*individual->expressions);
}

/// @brief Returns a random integer in a open interval 
/// @details Used for initialization of the genotype 
/// @param min lower bound
/// @param max upper bound
/// @return random integer in open interval 
template<class G, class F>
int Individual<G,F>::random_value_closed_interval(int min, int max) {
	int rand = this->random->random_integer(min, max);
	return rand;
}

/// @brief Reset genome, active nodes and evaluation status 
template<class G, class F>
void Individual<G,F>::reset() {
	this->reset_genome();
	this->active_nodes->clear();
	this->evaluated = false;
}

/// @brief Resets the genome according to the representation type 
/// @details Distinguishes between integer and real-valued encoding
template<class G, class F>
void Individual<G,F>::reset_genome() {
	int max_gene;
	int min_gene;

	for (int i = 0; i < this->genome_size; i++) {
		if (std::is_same<int, G>::value) {
			min_gene = this->min_gene(i);
			max_gene = this->max_gene(i);
			this->genome[i] = this->random_value_closed_interval(min_gene,
					max_gene);
		} else {
			this->genome[i] = this->random->random_float(0.0, 1.0);
		}
	}
}

// Handling of the active node vector 
// ---------------------------------------------
template<class G, class F>
void Individual<G,F>::clear_active_nodes() {
	this->active_nodes->clear();
}

template<class G, class F>
void Individual<G,F>::add_active_node(int node_num) {
	this->active_nodes->push_back(node_num);
}

template<class G, class F>
void Individual<G,F>::print_active_nodes() {
	for (int node : *this->active_nodes){
		std::cout<<node<<" ";
	}
	std::cout<<std::endl;
}
// ---------------------------------------------


// Handling of the expression vector
// ---------------------------------------------
template<class G, class F>
void Individual<G,F>::clear_expressions() {
	this->expressions->clear();
}

template<class G, class F>
void Individual<G,F>::add_expression(std::string expression) {
	this->expressions->push_back(expression);
}
// ---------------------------------------------


/// @brief Print out the genome 
template<class G, class F>
void Individual<G,F>::print_genome() {
	for (int i = 0; i < this->genome_size; i++) {
		std::cout << this->genome[i] << " ";
	}
	std::cout << std::endl;
}

/// @brief Concert the genome to a string.
/// @details Optional use of a delimter to seperate the genes. 
/// @param delimiter 
/// @return 
template<class G, class F>
const std::string Individual<G, F>::to_string(std::string delimiter) const {
	std::stringstream ss;
	for(int i=0; i < this->genome_size; i++) {
		G g = this->genome[i];

		ss << g;

		if(i < this->genome_size - 1) {
			ss << delimiter;
		}
	}
	return ss.str();
}

/// @brief Copy the genome that has been passed to the function
/// @param p_genome shared pointer to genome array
template<class G, class F>
void Individual<G, F>::copy_genome(std::shared_ptr<G[]> p_genome) {

	std::shared_ptr<G[]> genome_copy = std::make_shared<G[]>(this->genome_size);

	std::copy(p_genome.get(), p_genome.get() + this->genome_size, genome_copy.get());
	this->genome = genome_copy;
}

/// @brief Trigger deep cloning via copy constructor
/// @return cloned CGP individual
template<class G, class F>
Individual<G,F>* Individual<G, F>::clone() {
	return new Individual<G,F>(*this);
}


/// @brief Returns the number of actie nodes
/// @return number of active nodes 
template<class G, class F>
int Individual<G, F>::num_active_nodes() {
	return active_nodes->size();
}


// Getter and setter of indvidual class
// ---------------------------------------------------------------------------

template<class G, class F>
F Individual<G,F>::get_fitness() const {
	return fitness;
}

template<class G, class F>
void Individual<G,F>::set_fitness(F p_fitness) {
	this->fitness = p_fitness;
}

template<class G, class F>
bool Individual<G, F>::is_evaluated() const {
	return evaluated;
}

template<class G, class F>
void Individual<G, F>::set_evaluated(bool p_evaluated) {
	this->evaluated = p_evaluated;
}

template<class G, class F>
const std::shared_ptr<std::vector<int> >& Individual<G, F>::get_active_nodes() const {
	return active_nodes;
}

template<class G, class F>
const std::shared_ptr<std::vector<std::string>>& Individual<G, F>::get_expressions() const {
	return expressions;
}

template<class G, class F>
void Individual<G, F>::set_expressions(
		const std::shared_ptr<std::vector<std::string>> &p_expressions) {
	this->expressions = p_expressions;
}

template<class G, class F>
void Individual<G, F>::set_active_nodes(
		const std::shared_ptr<std::vector<int> > &activeNodes) {
	active_nodes = activeNodes;
}

// ---------------------------------------------------------------------------


#endif /* REPRESENGAGION_INDIVIDUAL_H_ */
