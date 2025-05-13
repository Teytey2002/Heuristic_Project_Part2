
#ifndef TABU_SEARCH_HPP
#define TABU_SEARCH_HPP

#include "pfsp_instance.hpp"
#include <vector>
#include <string>

// Tabu Search function
std::vector<int> tabuSearch(const PFSPInstance& instance, const std::vector<int>& initialSolution,
                            const std::string& neighborhoodint, int bestKnown, double targetPercent, const std::string& instanceName,
                            const std::string& algoName, int seed);

#endif