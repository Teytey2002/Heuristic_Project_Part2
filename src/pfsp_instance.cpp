#include "pfsp_instance.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>

PFSPInstance readInstance(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error can't open file " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    PFSPInstance instance;
    file >> instance.numJobs >> instance.numMachines;
    instance.processingTimes.resize(instance.numJobs, std::vector<int>(instance.numMachines));

    for (int i = 0; i < instance.numJobs; ++i) {
        for (int j = 0; j < instance.numMachines; ++j) {
            int machine_id, processing_time;
            file >> machine_id >> processing_time;
            if (machine_id != j + 1) {
                std::cerr << "Erreur: identifiant de machine inattendu à la ligne " << i + 2 << std::endl;
                exit(EXIT_FAILURE);
            }
            instance.processingTimes[i][j] = processing_time;
        }
    }

    return instance;
}