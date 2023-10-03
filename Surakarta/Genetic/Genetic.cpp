#include "Genetic.h"

template <typename T>
Genetic_basic<T>::Genetic_basic(int psize, int ssize) : population_size(psize), select_size(ssize) {}

template <typename T>
void Genetic_basic<T>::Select() {
    std::sort(populations.begin(), populations.end(), [](const std::pair<T, double>& A, const std::pair<T, double>& B) {
        return A.second > B.second;
    });
}