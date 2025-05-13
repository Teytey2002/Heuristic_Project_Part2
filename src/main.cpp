#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <map>
#include <algorithm>

#include "pfsp_instance.hpp"
#include "init_methods.hpp"
#include "local_search.hpp"
#include "tabu_search.hpp"
#include "genetic_algorithm.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    unsigned int seed = 0;
    bool saveResults = false;

    string pivoting_rule = "";
    string neighborhood = "";
    string init_method = "";
    string filename = "";
    int populationSize = 50;
    double mutationRate = 0.2;
    int tournamentSize = 3;
    double targetPercent = -1.0;


    // Reading command-line arguments
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];

        if (arg == "--first") pivoting_rule = "first";
        else if (arg == "--best") pivoting_rule = "best";
        else if (arg == "--vnd1") pivoting_rule = "VND1";
        else if (arg == "--vnd2") pivoting_rule = "VND2";
        else if (arg == "--tabu") pivoting_rule = "tabu";
        else if (arg == "--genetic") pivoting_rule = "genetic";
        else if (arg == "--transpose") neighborhood = "transpose";
        else if (arg == "--exchange") neighborhood = "exchange";
        else if (arg == "--insert") neighborhood = "insert";
        else if (arg == "--random-init") init_method = "random";
        else if (arg == "--srz") init_method = "srz";
        else if (arg == "--save") saveResults = true;
        else if (arg.rfind("--seed=", 0) == 0) {
            seed = stoi(arg.substr(7));
        }
        else if (arg[0] != '-') {
            filename = arg;  // argument without a dash = instance file name
        }
        else if (arg.rfind("--pop=", 0) == 0) {
            populationSize = stoi(arg.substr(6));
        }
        else if (arg.rfind("--mut=", 0) == 0) {
            mutationRate = stod(arg.substr(6));
        }
        else if (arg.rfind("--tour=", 0) == 0) {
            tournamentSize = stoi(arg.substr(7));
        }
        else if (arg.rfind("--target=", 0) == 0) {
            targetPercent = stod(arg.substr(9));
        }        
        else {
            cerr << "Error: Unknown argument " << arg << endl;
            return EXIT_FAILURE;
        }
    }

    if (filename.empty()) {
        cerr << "Error: Instance file not specified." << endl;
        return EXIT_FAILURE;
    }

    // Read best known solutions in a map
    map<string, int> bestKnownMap;
    ifstream bk("bestKnownTCT.txt");
    string line;
    getline(bk, line); // skip header
    while (getline(bk, line)) {
        stringstream ss(line);
        string name;
        int val;
        getline(ss, name, ',');
        ss >> val;
        bestKnownMap[name] = val;
    }

    // Extraire le nom de l'instance (ex: TA51)
    string shortName = filename.substr(filename.find_last_of("/") + 1);
    transform(shortName.begin(), shortName.end(), shortName.begin(), ::toupper); // ta051 -> TA051
    if (shortName.substr(0, 2) == "TA" && shortName[2] == '0') shortName = "TA" + shortName.substr(3);

    int bestKnown = bestKnownMap[shortName];

    // Displaying the chosen configuration
    cout << "Configuration:\n";
    cout << " - Instance file: " << filename << endl;
    cout << " - Pivoting: " << pivoting_rule << endl;
    cout << " - Neighborhood: " << neighborhood << endl;
    cout << " - Initialization: " << init_method << endl;
    cout << " - Seed: " << seed << endl;
    if (pivoting_rule == "genetic") {
        cout << " - Population size: " << populationSize << endl;
        cout << " - Mutation rate: " << mutationRate << endl;
        cout << " - Tournament size: " << tournamentSize << endl;
    }
    

    PFSPInstance instance = readInstance(filename);

    vector<int> permutation;
    if (init_method == "random") {
        permutation = generateRandomPermutation(instance.numJobs, seed);
    } else if (init_method == "srz") {
        permutation = generateSRZPermutation(instance);
    } else {
        cerr << "Error: Unknown initialization method." << endl;
        return EXIT_FAILURE;
    }

    // Start timer
    auto start = chrono::high_resolution_clock::now();

    // Choosing the algorithm
    if (pivoting_rule == "first") {
        permutation = localSearch_first_improvement(instance, permutation, neighborhood);
    } else if (pivoting_rule == "best") {
        permutation = localSearch_best_improvement(instance, permutation, neighborhood);
    } else if (pivoting_rule == "VND1") {
        vector<string> vnd1 = {"transpose", "exchange", "insert"};
        permutation = localSearchVND(instance, permutation, vnd1);
    } else if (pivoting_rule == "VND2") {
        vector<string> vnd2 = {"transpose", "insert", "exchange"};
        permutation = localSearchVND(instance, permutation, vnd2);
    } else if (pivoting_rule == "tabu") {
            permutation = tabuSearch(instance, permutation, neighborhood,
                                     bestKnown, targetPercent, shortName, "tabu", seed);
    } else if (pivoting_rule == "genetic") {
        permutation = geneticAlgorithm(instance, populationSize, 500, mutationRate, tournamentSize,
                                       bestKnown, targetPercent, shortName, "genetic", seed);
    } else {
        cerr << "Error: Unknown pivoting rule." << endl;
        return EXIT_FAILURE;
    }

    // End timer
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    // Results
    int completionTime = computeCompletionTime(instance, permutation);
    cout << "Total completion time: " << completionTime << endl;
    cout << "CPU time (s): " << fixed << setprecision(6) << elapsed.count() << endl;

    if (saveResults) {
        ofstream fout("results/test.csv", ios::app);
    
        if (pivoting_rule == "genetic") {
            fout << filename << ","
                 << instance.numJobs << ","
                 << pivoting_rule << ","
                 << init_method << ","
                 << seed << ","
                 << populationSize << ","
                 << mutationRate << ","
                 << tournamentSize << ","
                 << completionTime << ","
                 << fixed << setprecision(6) << elapsed.count() << endl;
        } else {
            fout << filename << ","
                 << instance.numJobs << ","
                 << pivoting_rule << ","
                 << init_method << ","
                 << seed << ",,,,,"
                 << completionTime << ","
                 << fixed << setprecision(6) << elapsed.count() << endl;
        }
    }

    return 0;
}
