/*
 * BenchmarkReader.h
 *
 *  Created on: 11.11.2022
 *      Author: roman
 */

#ifndef BENCHMARKS_BOOL_BENCHMARKREADER_H_
#define BENCHMARKS_BOOL_BENCHMARKREADER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>
#include <vector>
#include <any>
#include <memory>

template<class E>
class BenchmarkFileReader {
private:
	std::shared_ptr<std::vector<std::vector<E>>> inputs;
	std::shared_ptr<std::vector<std::vector<E>>> outputs;

	std::ifstream ifs;
	std::string line;

	int num_inputs;
	int num_outputs;
	int num_instances;

public:
	BenchmarkFileReader();
	~BenchmarkFileReader() = default;
	void read_benchmark_file(std::string file_path);
	void print_data();

	std::shared_ptr<std::vector<std::vector<E>>> get_input_data() const;
	std::shared_ptr<std::vector<std::vector<E>>> get_output_data() const;
	int get_num_instances() const;
	int get_num_inputs() const;
	int get_num_outputs() const;
};

template<class E>
BenchmarkFileReader<E>::BenchmarkFileReader() {
	inputs = std::make_shared<std::vector<std::vector<E>>>();
	outputs = std::make_shared<std::vector<std::vector<E>>>();
}

template<class E>
void BenchmarkFileReader<E>::print_data() {

	if (inputs.size() == 0) {
		throw std::runtime_error("No input data available!");
		return;
	}

	if (outputs.size() == 0) {
		throw std::runtime_error("No output data available!");
		return;
	}

	for (auto vec = inputs.begin(); vec != inputs.end();
			++vec) {
		for (auto it = vec->begin(); it != vec->end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

	for (auto vec = outputs.begin(); vec != outputs.end();
			++vec) {
		for (auto it = vec->begin(); it != vec->end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}

}

template<class E>
void BenchmarkFileReader<E>::read_benchmark_file(std::string file_path) {

	if (file_path.size() == 0) {
		throw std::runtime_error("File path is an empty string!");
	}

	std::string extension = std::filesystem::path(file_path).extension();

	if (extension != ".plu" && extension != ".dat") {
		throw std::runtime_error("Method only accepts PLU or DAT files!");
	}

	ifs.open(file_path, std::ifstream::in);

	char c;

	if (ifs.is_open()) {

		std::string str;
		E input;
		E output;

		ifs >> str >> num_inputs;
		ifs >> str >> num_outputs;
		ifs >> str >> num_instances;

		std::vector<E> input_chunk;
		std::vector<E> output_chunk;

		for (int i = 0; i < num_instances; i++) {

			for (int j = 0; j < num_inputs; j++) {
				ifs >> input;
				input_chunk.push_back(input);
			}

			// Inputs and outputs are seperated with whitespaces
			// Therefore, we skip this whitespace
			do {
				ifs.get(c);
			} while (ifs.peek() == ' ');

			for (int j = 0; j < num_outputs; j++) {
				ifs >> output;
				output_chunk.push_back(output);
			}


			inputs->push_back(input_chunk);
			outputs->push_back(output_chunk);

			input_chunk.clear();
			output_chunk.clear();
		}

		if (!ifs.good()) {
			throw std::runtime_error("Error while reading benchmark file!");
		}

		ifs.close();

	} else {
		throw std::runtime_error("Error opening benchmark file!");
	}

}

template<class E>
std::shared_ptr<std::vector<std::vector<E>>> BenchmarkFileReader<E>::get_input_data() const {
	return inputs;
}

template<class E>
std::shared_ptr<std::vector<std::vector<E>>> BenchmarkFileReader<E>::get_output_data() const {
	return outputs;
}

template<class E>
int BenchmarkFileReader<E>::get_num_instances() const {
	return num_instances;
}

template<class E>
int BenchmarkFileReader<E>::get_num_inputs() const {
	return num_inputs;
}

template<class E>
int BenchmarkFileReader<E>::get_num_outputs() const {
	return num_outputs;
}

#endif /* BENCHMARKS_BOOL_BENCHMARKREADER_H_ */
