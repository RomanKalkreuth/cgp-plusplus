/*
 * IntegerVectorIndividual.cpp
 *
 *  Created on: 13.04.2020
 *      Author: roman
 */


#include "IntegerVectorIndividual.h"

IntegerVectorIndividual::IntegerVectorIndividual(std::shared_ptr<Random> p_random,
		std::shared_ptr<Parameters> p_parameters) :IntegerVectorSpecies(p_random, p_parameters) {

	this->genome_size = parameters->get_genome_size();
	//this->genome = std::make_shared<int[]>(this->genome_size);

	std::shared_ptr<int[]> shared(new int[this->genome_size]());
	this->genome = shared;

	this->reset_genome();
}

int IntegerVectorIndividual::random_value_closed_interval(int min, int max){

	int rand=this->random->random_integer(min, max);
	return rand;
}

void IntegerVectorIndividual::reset_genome() {
	int max_gene;
	int min_gene=0;

	for(int i=0; i<genome_size; i++)
	{
		max_gene=calc_max_gene(i);
		genome[i] = random_value_closed_interval(min_gene, max_gene);
	}
}

void IntegerVectorIndividual::print_genome() {

	for(int i=0; i<genome_size; i++)
	{
		std::cout<< genome[i] << " " ;
	}

	cout<< std::endl;
}

