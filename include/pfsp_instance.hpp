#ifndef PFSP_INSTANCE_HPP
#define PFSP_INSTANCE_HPP

#include <vector>
#include <string>

struct PFSPInstance {
    int numJobs;
    int numMachines;
    std::vector<std::vector<int>> processingTimes;
};

PFSPInstance readInstance(const std::string& filename);

#endif
