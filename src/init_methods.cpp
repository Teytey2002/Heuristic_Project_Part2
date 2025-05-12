#include "init_methods.hpp"
#include <algorithm>
#include <random>
#include <chrono>
#include <numeric>
#include <climits>

std::vector<int> generateRandomPermutation(int n, unsigned int seed) {
    std::vector<int> permutation(n);
    for (int i = 0; i < n; ++i)
        permutation[i] = i;

    if (seed == 0)
        seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::shuffle(permutation.begin(), permutation.end(), std::default_random_engine(seed));
    return permutation;
}

std::vector<int> generateSRZPermutation(const PFSPInstance& instance) {
    int n = instance.numJobs;
    int m = instance.numMachines;
    std::vector<std::pair<int, int>> jobSum;

    for (int i = 0; i < n; ++i) {
        int sum = std::accumulate(instance.processingTimes[i].begin(), instance.processingTimes[i].end(), 0);
        jobSum.emplace_back(sum, i);
    }

    std::stable_sort(jobSum.begin(), jobSum.end());
    std::vector<int> solution;

    for (const auto& job : jobSum) {
        int jobIndex = job.second;
        int bestPos = 0;
        int bestCompletion = INT_MAX;

        for (size_t pos = 0; pos <= solution.size(); ++pos) {
            std::vector<int> temp = solution;
            temp.insert(temp.begin() + pos, jobIndex);

            std::vector<int> machineTime(m, 0);
            int totalCompletion = 0;
            for (size_t i = 0; i < temp.size(); ++i) {
                int jID = temp[i];
                machineTime[0] += instance.processingTimes[jID][0];
                for (int j = 1; j < m; ++j) {
                    machineTime[j] = std::max(machineTime[j], machineTime[j - 1]) + instance.processingTimes[jID][j];
                }
                totalCompletion += machineTime[m - 1];
            }

            if (totalCompletion < bestCompletion) {
                bestCompletion = totalCompletion;
                bestPos = pos;
            }
        }

        solution.insert(solution.begin() + bestPos, jobIndex);
    }

    return solution;
}
