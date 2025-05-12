#include "genetic_algorithm.hpp"
#include "local_search.hpp"
#include "init_methods.hpp"

#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>

// Individual representation: a job permutation
using Individual = std::vector<int>;

// Evaluate the fitness: lower completion time = better fitness
int evaluateFitness(const PFSPInstance& instance, const Individual& indiv) {
    return computeCompletionTime(instance, indiv);
}

// Tournament selection: pick best among k random individuals
Individual tournamentSelection(const std::vector<Individual>& population,
                               const std::vector<int>& fitness,
                               int tournamentSize,
                               std::mt19937& rng) {
    std::uniform_int_distribution<> dist(0, population.size() - 1);
    int bestIndex = dist(rng);
    for (int i = 1; i < tournamentSize; ++i) {
        int idx = dist(rng);
        if (fitness[idx] < fitness[bestIndex]) {
            bestIndex = idx;
        }
    }
    return population[bestIndex];
}

// Order Crossover (OX) implementation
Individual orderCrossover(const Individual& parent1, const Individual& parent2, std::mt19937& rng) {
    int n = parent1.size();
    Individual child(n, -1);
    std::uniform_int_distribution<> dist(0, n - 1);

    // Random crossover points
    int start = dist(rng);
    int end = dist(rng);
    if (start > end) std::swap(start, end);

    // Copy segment from parent1 into child
    for (int i = start; i <= end; ++i) {
        child[i] = parent1[i];
    }

    // Fill the remaining positions from parent2 in order
    size_t p2_idx = 0;
    for (int i = 0; i < n; ++i) {
        int idx = (end + 1 + i) % n;

        // Skip values already in child
        while (p2_idx < parent2.size() && std::find(child.begin(), child.end(), parent2[p2_idx]) != child.end()) {
            p2_idx++;
        }

        // Assign the next available gene from parent2
        if (p2_idx < parent2.size()) {
            child[idx] = parent2[p2_idx];
        }
    }

    return child;
}


// Insert mutation: pick one job and move it elsewhere
void insertMutation(Individual& indiv, std::mt19937& rng) {
    int n = indiv.size();
    std::uniform_int_distribution<> dist(0, n - 1);
    int i = dist(rng);
    int j = dist(rng);
    while (j == i) j = dist(rng);

    int job = indiv[i];
    indiv.erase(indiv.begin() + i);
    indiv.insert(indiv.begin() + j, job);
}

// Main GA function
std::vector<int> geneticAlgorithm(const PFSPInstance& instance, int populationSize,
                                  int generations, double mutationRate, int tournamentSize) {
    //int n = instance.numJobs;
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<> prob(0.0, 1.0);
    
    // Initialize time limit based on problem size
    double time_limit = 0.0;
    if (instance.numJobs == 50) time_limit = 153.0;
    else if (instance.numJobs == 100) time_limit = 2725.0;
    else if (instance.numJobs == 200) time_limit = 8990.0;
    else {
        std::cerr << "Unsupported problem size for time limit." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "→ Time limit for Genetic Algorithm: " << time_limit << " seconds" << std::endl;

    // Initialize population with SRZ + small perturbations
    std::vector<Individual> population;
    for (int i = 0; i < populationSize; ++i) {
        Individual indiv = generateSRZPermutation(instance);
        insertMutation(indiv, rng);  // diversify slightly
        population.push_back(indiv);
    }

    // Evaluate initial fitness
    std::vector<int> fitness(populationSize);
    for (int i = 0; i < populationSize; ++i) {
        fitness[i] = evaluateFitness(instance, population[i]);
    }

    Individual bestSolution = population[0];
    int bestFitness = fitness[0];

    auto start_time = std::chrono::high_resolution_clock::now();

    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(now - start_time).count();
        if (elapsed >= time_limit) break;
        std::vector<Individual> newPopulation;

        for (int i = 0; i < populationSize; ++i) {
            // Selection
            Individual parent1 = tournamentSelection(population, fitness, tournamentSize, rng);
            Individual parent2 = tournamentSelection(population, fitness, tournamentSize, rng);

            // Crossover
            Individual child = orderCrossover(parent1, parent2, rng);

            // Mutation
            if (prob(rng) < mutationRate) {
                insertMutation(child, rng);
            }
            // Local search for improvement
            child = localSearch_first_improvement(instance, child, "insert");

            newPopulation.push_back(child);
        }

        // Evaluate new population
        fitness.clear();
        for (const auto& indiv : newPopulation) {
            fitness.push_back(evaluateFitness(instance, indiv));
        }

        // Keep track of best
        for (int i = 0; i < populationSize; ++i) {
            if (fitness[i] < bestFitness) {
                bestFitness = fitness[i];
                bestSolution = newPopulation[i];
            }
        }

        population = newPopulation;
    }

    return bestSolution;
}
