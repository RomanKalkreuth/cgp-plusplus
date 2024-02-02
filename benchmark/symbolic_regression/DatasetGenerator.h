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



template<class D>
DatasetGenerator<D>::DatasetGenerator(std::shared_ptr<Random> p_random) {

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

template<class D>
std::shared_ptr<std::vector<std::vector<D>>> DatasetGenerator<D>::random_samples(
		D min, D max, int num_instances, int dim) {
	assert(min < max);

	std::shared_ptr<std::vector<std::vector<D>>> samples = std::make_shared<
			std::vector<std::vector<D>>>();

	D rand;
	for (int i = 0; i < num_instances; i++) {
		std::vector<D> sample;
		for (int j = 0; j < dim; j++) {

			rand = this->random->random_integer(min, max);

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

template<class D>
std::shared_ptr<std::vector<float>> DatasetGenerator<D>::evenly_spaced_grid(
		int start, int stop, float step) {
	assert(start < stop);

	int n = std::floor(start + stop / step);

	std::shared_ptr<std::vector<float>> grid = std::make_shared<
			std::vector<float>>(n);

	double val = start;

	for (int i = 0; i < n; i++) {
		grid->push_back(val);
		val += step;
	}
	return grid;
}

template<class D>
std::shared_ptr<std::vector<float>> DatasetGenerator<D>::calculate_function_values(
		std::shared_ptr<std::vector<std::vector<D>>> inputs,
		std::function<float(float*)> objective_function) {

	int num_instances = inputs->size();
	int num_inputs = inputs->at(0).size();

	float xs[num_inputs];
	float y;

	std::shared_ptr<std::vector<float>> res = std::make_shared<
			std::vector<float>>();

	for (int i = 0; i < num_instances; i++) {
		std::vector<D> input_instance = inputs->at(i);
		std::copy(std::begin(input_instance), std::end(input_instance), xs);
		y = objective_function(xs);
		res->push_back(y);
	}
	return res;
}

template<class D>
void DatasetGenerator<D>::write_benchmark_file(
		std::shared_ptr<std::vector<std::vector<D>>> inputs,
		std::shared_ptr<std::vector<float>> outputs, std::string file_path) {

	int num_instances = inputs->size();
	int num_inputs = inputs->at(0).size();
	int num_outputs = 1;

	std::ofstream ofs(file_path.c_str(),
			std::ofstream::out | std::fstream::trunc);

	if (ofs) {
		ofs << ".i " << num_inputs << std::endl;
		ofs << ".o " << num_outputs << std::endl;
		ofs << ".p " << num_instances << std::endl;

		for (int i = 0; i < num_instances; i++) {

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

template<class D>
void DatasetGenerator<D>::generate_benchmark(std::shared_ptr<std::vector<std::vector<D>>> inputs,
		std::function<float(float*)> objective_function, std::string file_path) {

	std::shared_ptr<std::vector<D>> outputs
		= this->calculate_function_values(inputs, objective_function);

	this->write_benchmark_file(inputs, outputs, file_path);

}
#endif /* BENCHMARK_SYMBOLIC_REGRESSION_DATASETGENERATOR_H_ */
