#pragma once

#include <vector>
#include <algorithm>

template <typename T>
class Genetic_basic {
public:
	int population_size;
	int select_size;

	std::vector<std::pair<T, double>> populations;

	Genetic_basic(int psize, int ssize);

	virtual void Initialize_Population() = 0;
	virtual double FitnessFunction(const T& GENE) = 0;
	void Select();
	virtual std::pair<T, T> Crossover(T parent1, T parent2) = 0;
	virtual void Mutation(T& child) = 0;
};

/*class Genetic : public Genetic_basic<int> {

};*/