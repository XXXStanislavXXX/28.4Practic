#pragma once

#include "Swimmer.h"
#include <vector>
#include <string>

class Simulation {
public:
    Simulation(const std::vector<std::string> &names, const std::vector<double> &speeds);
    ~Simulation();

    // Run the simulation (reads nothing). This method performs the
    // per-second loop, prints progress and final results to stdout.
    void run();

private:
    void start_threads();
    void stop_threads();

    std::vector<Swimmer> swimmers;
    int num_swimmers;
};
