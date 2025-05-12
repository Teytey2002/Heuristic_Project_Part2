#ifndef INIT_METHODS_HPP
#define INIT_METHODS_HPP

#include "pfsp_instance.hpp"
#include <vector>

std::vector<int> generateRandomPermutation(int n, unsigned int seed = 0);
std::vector<int> generateSRZPermutation(const PFSPInstance& instance);

#endif
