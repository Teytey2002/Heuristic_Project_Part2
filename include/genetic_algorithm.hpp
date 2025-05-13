
#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP

#include "pfsp_instance.hpp"
#include <vector>
#include <string>

std::vector<int> geneticAlgorithm(const PFSPInstance& instance,
    int populationSize,
    int generations,
    double mutationRate,
    int tournamentSize,
    int bestKnown,
    double targetPercent,
    const std::string& instanceName,
    const std::string& algoName,
    int seed);


#endif
