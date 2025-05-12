#ifndef LOCAL_SEARCH_HPP
#define LOCAL_SEARCH_HPP

#include "pfsp_instance.hpp"
#include <vector>
#include <string>

int computeCompletionTime(const PFSPInstance& instance, const std::vector<int>& permutation);

std::vector<int> localSearch_first_improvement(const PFSPInstance& instance, std::vector<int> permutation, const std::string& neighborhood);
std::vector<int> localSearch_best_improvement(const PFSPInstance& instance, std::vector<int> permutation, const std::string& neighborhood);
std::vector<int> localSearchVND(const PFSPInstance& instance, const std::vector<int>& initialSolution, const std::vector<std::string>& neighborhoods);

#endif
