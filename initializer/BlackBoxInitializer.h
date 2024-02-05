//	CGP++: Modern C++ Implementation of Cartesian Genetic Programming
// ===============================================================================
//	File: BlackBoxInitializer.h 
// ===============================================================================
//
// ===============================================================================
//  Copyright (C) 2024
//
//  Author(s): Anonymous
//
//	License: Academic Free License v. 3.0
// ================================================================================

#ifndef INITIALIZER_BLACKBOXINITIALIZER_H_
#define INITIALIZER_BLACKBOXINITIALIZER_H_

#include <string>
#include "Initializer.h"

template<class E, class G, class F>
class BlackBoxInitializer: public Initializer<E, G, F> {
protected:
	std::shared_ptr<std::vector<std::vector<E>> > inputs;
	std::shared_ptr<std::vector<std::vector<E>> > outputs;
	int num_instances;
public:
	BlackBoxInitializer(const std::string &p_benchmark_file);
	virtual ~BlackBoxInitializer() = default;
	virtual void init_problem() = 0;
	virtual void init_functions() = 0;
	void read_data();
};

template<class E, class G, class F>
BlackBoxInitializer<E, G, F>::BlackBoxInitializer(
		const std::string &p_benchmark_file) :
		Initializer<E, G, F>(p_benchmark_file) {
}

template<class E, class G, class F>
void BlackBoxInitializer<E, G, F>::read_data() {

	std::shared_ptr<BenchmarkFileReader<E>> bechmark_reader = std::make_shared<
			BenchmarkFileReader<E>>();

	bechmark_reader->read_benchmark_file(this->benchmark_file);

	this->inputs = bechmark_reader->get_input_data();
	this->outputs = bechmark_reader->get_output_data();

	int num_inputs = bechmark_reader->get_num_inputs();
	int num_outputs = bechmark_reader->get_num_outputs();

	this->parameters->set_num_variables(num_inputs);
	this->parameters->set_num_outputs(num_outputs);
	this->num_instances = bechmark_reader->get_num_instances();
}

#endif /* INITIALIZER_BLACKBOXINITIALIZER_H_ */
