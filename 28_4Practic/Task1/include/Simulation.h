#pragma once
#include <vector>
#include <mutex>
#include "Swimmer.h"

class Simulation {
public:
    Simulation(double distance_meters, double dt_seconds = 0.1);

    void add_swimmer(const Swimmer& swimmer);
    void run();
    const std::vector<Swimmer>& results() const;

private:
    void swim_race(Swimmer swimmer);

    double distance_;
    double dt_;
    std::vector<Swimmer> swimmers_;

    // populated in finish order by threads
    std::vector<Swimmer> results_;
    std::mutex results_mutex_;
    std::mutex cout_mutex_;
};
