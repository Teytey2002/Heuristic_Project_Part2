
#include "tabu_search.hpp"
#include "local_search.hpp"
#include "utils.hpp"

#include <unordered_map>
#include <algorithm>
#include <climits>
#include <chrono>
#include <iostream>
#include <fstream>

// Structure to represent a move (swap or insert operation)
struct Move {
    int i;
    int j;
    Move(int i, int j) : i(i), j(j) {}
    bool operator==(const Move& other) const {
        return (i == other.i && j == other.j);
    }
};

// Hash function for Move (needed for unordered_map)
namespace std {
    template <>
    struct hash<Move> {
        size_t operator()(const Move& m) const {
            return hash<int>()(m.i) ^ hash<int>()(m.j);
        }
    };
}


std::vector<int> tabuSearch(const PFSPInstance& instance, const std::vector<int>& initialSolution,
                            const std::string& neighborhood,
                            int bestKnown, double targetPercent, const std::string& instanceName,
                            const std::string& algoName, int seed) {
    
    int n = instance.numJobs;

    // Step 1: improve initial solution with first-improvement and insert
    std::vector<int> current = localSearch_first_improvement(instance, initialSolution, "insert");
    std::vector<int> bestSolution = current;
    int bestCost = computeCompletionTime(instance, bestSolution);

    // Step 2: determine time limit based on instance size
    double time_limit = 0.0;
    if (n == 50) {
        time_limit = 3.1 ; //153.0;
    } else if (n == 100) {
        time_limit = 54.5; //2725.0;
    } else if (n == 200) {
        time_limit = 9000.0;
    } else {
        std::cerr << "Unsupported problem size for time limit: " << n << " jobs." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "→ Time limit for Tabu Search: " << time_limit << " seconds" << std::endl;

    int tabuTenure = 7;
    std::unordered_map<Move, int> tabuList;

    auto start_time = std::chrono::high_resolution_clock::now();

    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(now - start_time).count();
        if (elapsed > time_limit) break;

        std::vector<int> bestNeighbor = current;
        int bestNeighborCost = INT_MAX;
        Move bestMove(-1, -1);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == j) continue;

                std::vector<int> neighbor = current;

                if (neighborhood == "exchange" && i < j) {
                    std::swap(neighbor[i], neighbor[j]);
                } else if (neighborhood == "transpose" && j == i + 1) {
                    std::swap(neighbor[i], neighbor[j]);
                } else if (neighborhood == "insert") {
                    int job = neighbor[i];
                    neighbor.erase(neighbor.begin() + i);
                    neighbor.insert(neighbor.begin() + j, job);
                } else {
                    continue;
                }

                int cost = computeCompletionTime(instance, neighbor);
                Move move(i, j);
                bool isTabu = tabuList.find(move) != tabuList.end();
                bool aspiration = cost < bestCost;

                if (!isTabu || aspiration) {
                    if (cost < bestNeighborCost) {
                        bestNeighbor = neighbor;
                        bestNeighborCost = cost;
                        bestMove = move;
                    }
                }
            }
        }

        current = bestNeighbor;

        if (bestNeighborCost < bestCost) {
            bestCost = bestNeighborCost;
            bestSolution = bestNeighbor;
            
            //std::cout << "Test seuil : current = " << bestNeighborCost << " vs target = " << bestKnown * (1.0 + targetPercent / 100.0) << std::endl;

            // Verify if the solution meets the desired quality target
            if (isTargetReached(bestCost, bestKnown, targetPercent)) {
                std::ofstream fout("results/rtd_results.csv", std::ios::app);
                fout << instanceName << "," << algoName << "," << seed << "," << targetPercent << "," << elapsed << "\n";
                break;
            }
        }

        for (auto it = tabuList.begin(); it != tabuList.end();) {
            it->second--;
            if (it->second <= 0) {
                it = tabuList.erase(it);
            } else {
                ++it;
            }
        }

        if (bestMove.i != -1 && bestMove.j != -1) {
            tabuList[bestMove] = tabuTenure;
        }
    }

    return bestSolution;
}