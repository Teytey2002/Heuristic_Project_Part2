
#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP

#include "pfsp_instance.hpp"
#include <vector>
#include <string>

std::vector<int> geneticAlgorithm(const PFSPInstance& instance, int populationSize = 50,
                                  int generations = 500, double mutationRate = 0.2,
                                  int tournamentSize = 3);

#endif
