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
#ifndef CHECKPOINT_CHECKPOINT_H_
#define CHECKPOINT_CHECKPOINT_H_

#include <fstream>
#include <memory>
#include <string>
#include <filesystem>
#include <chrono>
#include <cstdlib>

#include "../algorithm/EvolutionaryAlgorithm.h"
#include "../parameters/Parameters.h"
#include "../population/AbstractPopulation.h"
#include "../random/Random.h"

using namespace std::chrono;

template<class E, class G, class F>
class Checkpoint {
private:
	std::shared_ptr<Parameters> parameters;
	std::string dir_name;
public:
	Checkpoint(std::shared_ptr<Parameters> p_parameters);
	virtual ~Checkpoint() = default;
	void write(std::shared_ptr<AbstractPopulation<G, F>> population,
			std::shared_ptr<std::vector<E>> constants, int generation_number);
	int load(std::shared_ptr<AbstractPopulation<G, F>> population,
			std::shared_ptr<std::vector<E>> constants,
			std::shared_ptr<Random> random, std::string &checkpoint_file_path);
	void create_dir();
	std::vector<std::string> split_genome(string genome_str);
};

template<class E, class G, class F>
Checkpoint<E, G, F>::Checkpoint(std::shared_ptr<Parameters> p_parameters) {
	if (p_parameters != nullptr) {
		this->parameters = p_parameters;
	} else {
		throw std::invalid_argument(
				"Nullpointer exception in checkpoint class!");
	}
	create_dir();
}

template<class E, class G, class F>
void Checkpoint<E, G, F>::create_dir() {
	std::stringstream ss;
	uint64_t tstamp = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()).count();
	ss << this->parameters->CHECKPOINT_FILE_DIR << tstamp;
	std::filesystem::create_directories(ss.str());
	ss.str("");
	ss << tstamp;
	this->dir_name = ss.str();
}

template<class E, class G, class F>
void Checkpoint<E, G, F>::write(
		std::shared_ptr<AbstractPopulation<G, F>> population,
		std::shared_ptr<std::vector<E>> constants, int generation_number) {
	std::stringstream ss;
	std::unique_ptr<std::ofstream> ofs;
	ss << this->parameters->CHECKPOINT_FILE_DIR << this->dir_name << "/"
			<< "generation-" << generation_number << ".checkpoint";
	ofs = std::make_unique<std::ofstream>(ss.str().c_str(),
			istream::out | ios::binary);
	ss.str("");
	ss << "generation_number " << generation_number << std::endl;
	ss << "global_seed " << this->parameters->get_global_seed() << std::endl;

	for (int i = 0; i < this->parameters->get_population_size(); i++) {
		std::string genome_str = population->get_individual(i)->to_string(",");
		ss << "genome " << genome_str << std::endl;
	}

	for (int i = 0; i < this->parameters->get_num_constants(); i++) {
		E constant = constants->at(i);
		ss << "constant " << constant << std::endl;
	}

	*ofs << ss.rdbuf();
}

template<class E, class G, class F>
int Checkpoint<E, G, F>::load(
		std::shared_ptr<AbstractPopulation<G, F>> population,
		std::shared_ptr<std::vector<E>> constants,
		std::shared_ptr<Random> random, std::string &checkpoint_file_path) {
	std::ifstream ifs;
	ifs.open(checkpoint_file_path, ios_base::in | ios_base::binary);

	std::string parameter;
	std::string value;

	double global_seed;
	int generation_number;

	std::shared_ptr<std::vector<std::vector<std::string>>> genomes =
			std::make_shared<std::vector<std::vector<std::string>>>();

	constants->clear();

	if (ifs.is_open()) {
		while (ifs >> parameter >> value) {
			if (parameter == "generation_number") {
				generation_number = std::stoi(value);
			} else if (parameter == "global_seed") {
				global_seed = std::stod(value);
			} else if (parameter == "genome") {
				std::vector<std::string> genome = this->split_genome(value);
				genomes->push_back(genome);
			} else if (parameter == "constant") {
				double constant = std::stod(value);
				constants->push_back(constant);
			}
		}
	} else {
		throw std::runtime_error("Error opening checkpoint file!");
	}

	random->set_seed(global_seed);
	population->init_from_checkpoint(genomes);

	return generation_number;
}

template<class E, class G, class F>
std::vector<std::string> Checkpoint<E, G, F>::split_genome(string genome_str) {
	std::vector<string> vec;
	int pos = 0;
	while (pos <= genome_str.size()) {
		pos = genome_str.find(",");
		vec.push_back(genome_str.substr(0, pos));
		genome_str.erase(0, pos + 1);
	}
	return vec;
}

#endif /* CHECKPOINT_CHECKPOINT_H_ */
