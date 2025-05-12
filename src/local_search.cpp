#include "local_search.hpp"
#include <algorithm>
#include <climits>

// Calcul of cost (sum of completion times)
int computeCompletionTime(const PFSPInstance& instance, const std::vector<int>& permutation) {
    int n = instance.numJobs;
    int m = instance.numMachines;

    std::vector<int> machineTime(m, 0);
    int totalCompletionTime = 0;

    for (int i = 0; i < n; ++i) {
        int job = permutation[i];
        machineTime[0] += instance.processingTimes[job][0];

        for (int j = 1; j < m; ++j) {
            machineTime[j] = std::max(machineTime[j], machineTime[j - 1]) + instance.processingTimes[job][j];
        }

        totalCompletionTime += machineTime[m - 1];
    }

    return totalCompletionTime;
}

// First Improvement
std::vector<int> localSearch_first_improvement(const PFSPInstance& instance, std::vector<int> permutation, const std::string& neighborhood) {
    int n = instance.numJobs;
    bool improved = true;

    while (improved) {
        improved = false;
        int currentCost = computeCompletionTime(instance, permutation);

        for (int i = 0; i < n && !improved; ++i) {
            for (int j = 0; j < n && !improved; ++j) {
                if (i == j) continue;

                std::vector<int> neighbor = permutation;

                if (neighborhood == "exchange" && i < j)
                    std::swap(neighbor[i], neighbor[j]);
                else if (neighborhood == "transpose" && j == i + 1)
                    std::swap(neighbor[i], neighbor[j]);
                else if (neighborhood == "insert") {
                    int job = neighbor[i];
                    neighbor.erase(neighbor.begin() + i);
                    neighbor.insert(neighbor.begin() + j, job);
                } else continue;

                int newCost = computeCompletionTime(instance, neighbor);
                if (newCost < currentCost) {
                    permutation = neighbor;
                    improved = true;
                }
            }
        }
    }

    return permutation;
}

// Best Improvement
std::vector<int> localSearch_best_improvement(const PFSPInstance& instance, std::vector<int> permutation, const std::string& neighborhood) {
    int n = instance.numJobs;
    bool improved = true;

    while (improved) {
        improved = false;
        int currentCost = computeCompletionTime(instance, permutation);
        std::vector<int> bestNeighbor = permutation;
        int bestCost = currentCost;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == j) continue;

                std::vector<int> neighbor = permutation;

                if (neighborhood == "exchange" && i < j)
                    std::swap(neighbor[i], neighbor[j]);
                else if (neighborhood == "transpose" && j == i + 1)
                    std::swap(neighbor[i], neighbor[j]);
                else if (neighborhood == "insert") {
                    int job = neighbor[i];
                    neighbor.erase(neighbor.begin() + i);
                    neighbor.insert(neighbor.begin() + j, job);
                } else continue;

                int newCost = computeCompletionTime(instance, neighbor);
                if (newCost < bestCost) {
                    bestCost = newCost;
                    bestNeighbor = neighbor;
                }
            }
        }

        if (bestCost < currentCost) {
            permutation = bestNeighbor;
            improved = true;
        }
    }

    return permutation;
}

// VND
std::vector<int> localSearchVND(const PFSPInstance& instance, const std::vector<int>& initialSolution, const std::vector<std::string>& neighborhoods) {
    std::vector<int> current = initialSolution;
    bool improvement = true;

    while (improvement) {
        improvement = false;

        for (const std::string& neigh : neighborhoods) {
            std::vector<int> newSol = localSearch_first_improvement(instance, current, neigh);

            if (computeCompletionTime(instance, newSol) < computeCompletionTime(instance, current)) {
                current = newSol;
                improvement = true;
                break;
            }
        }
    }

    return current;
}
