//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: DataSetGenerator.h
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef BENCHMARK_SYMBOLIC_REGRESSION_DATASETGENERATOR_H_
#define BENCHMARK_SYMBOLIC_REGRESSION_DATASETGENERATOR_H_

#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include <cmath>
#include <memory>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <functional>
#include <type_traits>

#include "../../random/Random.h"

/// @brief Class for generating datasets for symbolic regression benchmarks.
/// @details Provides a generator for the datasets of the benchmarks proposed by McDermott et al. (2012) 
/// @see https://doi.org/10.1145/2330163.2330273
/// @tparam D Datatype for the generated sets. 
template<class D>
class DatasetGenerator {
private:
	std::shared_ptr<Random> random;
	const std::string BENCHMARK_DIR = "../data/datfiles/";

	std::ofstream ofs;

public:
	DatasetGenerator(std::shared_ptr<Random> p_random);

	std::shared_ptr<std::vector<std::vector<D>>> random_samples(D min, D max,
			int num_instances, int dim = 1);

	std::shared_ptr<std::vector<float>> evenly_spaced_grid(int start, int stop,
			float step);

	std::shared_ptr<std::vector<float>> calculate_function_values(
			std::shared_ptr<std::vector<std::vector<D>>> inputs,
			std::function<float(float*)> objective_function);

	void write_benchmark_file(
			std::shared_ptr<std::vector<std::vector<D>>> inputs,
			std::shared_ptr<std::vector<float>> outputs, std::string file_path);

	void generate_benchmark(std::shared_ptr<std::vector<std::vector<D>>> inputs,
			std::function<float(float*)> objective_function, std::string file_path);

};


/// @brief Constructor for the DatasetGenerator class
/// @param p_random Random genreator instance 
template<class D>
DatasetGenerator<D>::DatasetGenerator(std::shared_ptr<Random> p_random) {

	// Check wether the template type is valid
	if constexpr (!std::is_same<int, D>::value) {
		if constexpr (!std::is_same<float, D>::value) {
			throw std::invalid_argument(
					"Illegal template type in DatasetGeneratorClass");
		}
	}

	if (p_random == nullptr) {
		throw std::invalid_argument("Random object is NULL!");
	}

	this->random = p_random;

}

/// @brief Function to generate random samples within a given range
/// @param min left bound of the interval 
/// @param max right bound of the interval 
/// @param num_instances number of samples
/// @param dim number of variables 
/// @return samples stored in a vector
template<class D>
std::shared_ptr<std::vector<std::vector<D>>> DatasetGenerator<D>::random_samples(
		D min, D max, int num_instances, int dim) {
	assert(min < max);

	// Creating a shared pointer instance for the vector of samples
	std::shared_ptr<std::vector<std::vector<D>>> samples = std::make_shared<
			std::vector<std::vector<D>>>();

	D rand;
	for (int i = 0; i < num_instances; i++) {
		std::vector<D> sample;
		for (int j = 0; j < dim; j++) {

			rand = this->random->random_integer(min, max);
			// Generating a random value based on the template type
			if constexpr (std::is_same<int, D>::value) {
				rand = this->random->random_integer(min, max);
			} else {
				rand = this->random->random_float(min, max);
			}

			sample.push_back(rand);
		}
		samples->push_back(sample);
	}
	return samples;
}

/// @brief Function to generate evenly spaced grid 
/// @param min left bound of the interval 
/// @param max right bound of the interval 
/// @param step step size 
/// @return grid stored in a vector
template<class D>
std::shared_ptr<std::vector<float>> DatasetGenerator<D>::evenly_spaced_grid(
		int start, int stop, float step) {
	assert(start < stop);

	// Calculate the number of elements in the grid
	int n = std::floor(start + stop / step);

	// Create a shared pointer to a vector of floats
	std::shared_ptr<std::vector<float>> grid = std::make_shared<
			std::vector<float>>(n);

	double val = start;

	// Fill the grid with evenly spaced values
	for (int i = 0; i < n; i++) {
		grid->push_back(val);
		val += step;
	}
	return grid;
}


/// @brief Calculating the function values for a set of input values
/// and a given objective function
/// @param inputs set of data points
/// @param objective_function pointer to the objective function
/// @return calculated function values
template<class D>
std::shared_ptr<std::vector<float>> DatasetGenerator<D>::calculate_function_values(
		std::shared_ptr<std::vector<std::vector<D>>> inputs,
		std::function<float(float*)> objective_function) {

	// Get the number of instances and inputs
	int num_instances = inputs->size();
	int num_inputs = inputs->at(0).size();

	float xs[num_inputs];
	float y;

	// Create a shared pointer to a vector of floats to store the result
	std::shared_ptr<std::vector<float>> res = std::make_shared<
			std::vector<float>>();

	// Iterate over each instance and calculate the function value
	for (int i = 0; i < num_instances; i++) {
		std::vector<D> input_instance = inputs->at(i);
		std::copy(std::begin(input_instance), std::end(input_instance), xs);
		y = objective_function(xs);
		res->push_back(y);
	}
	return res;
}

/// @brief Writes a benchmark file with the given inputs and outputs
/// @param inputs: a shared pointer to a vector of vectors representing the input data
/// @param outputs: a shared pointer to a vector of floats representing the output data
/// @return file path where the benchmark file has been written
template<class D>
void DatasetGenerator<D>::write_benchmark_file(
		std::shared_ptr<std::vector<std::vector<D>>> inputs,
		std::shared_ptr<std::vector<float>> outputs, std::string file_path) {

	// Get the number of instances, inputs, and outputs
	int num_instances = inputs->size();
	int num_inputs = inputs->at(0).size();
	int num_outputs = 1;


	// Open the file for writing
	std::ofstream ofs(file_path.c_str(),
			std::ofstream::out | std::fstream::trunc);

	if (ofs) {
		// Write the meta information to the file
		ofs << ".i " << num_inputs << std::endl;
		ofs << ".o " << num_outputs << std::endl;
		ofs << ".p " << num_instances << std::endl;

		// Iterate over each instance
		for (int i = 0; i < num_instances; i++) {

			// Write the input values to the file
			for (int j = 0; j < num_inputs; j++) {
				ofs << inputs->at(i).at(j) << " ";
			}

			ofs << "  ";
			ofs << outputs->at(i) << "";
			ofs << std::endl;
		}
		ofs << ".e";
		ofs.close();
	} else {
		throw std::runtime_error("Could not open or create benchmark file!");
	}

}

/// @brief Generates a benchmark file by the output values for the given input data 
/// using the provided objective function
/// @param inputs pointer to a vector of vectors representing the input data
/// @param objective_function pointer to the objective function
/// @param file_path file path where the benchmark file will be written
template<class D>
void DatasetGenerator<D>::generate_benchmark(std::shared_ptr<std::vector<std::vector<D>>> inputs,
		std::function<float(float*)> objective_function, std::string file_path) {

	std::shared_ptr<std::vector<D>> outputs
		= this->calculate_function_values(inputs, objective_function);

	this->write_benchmark_file(inputs, outputs, file_path);

}
#endif /* BENCHMARK_SYMBOLIC_REGRESSION_DATASETGENERATOR_H_ */
