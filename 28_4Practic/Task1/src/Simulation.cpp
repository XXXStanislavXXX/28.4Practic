#include "Simulation.h"

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

Simulation::Simulation(double distance_meters, double dt_seconds)
    : distance_(distance_meters), dt_(dt_seconds) {}

void Simulation::add_swimmer(const Swimmer& swimmer) {
    swimmers_.push_back(swimmer);
}

void Simulation::run() {
    std::vector<std::thread> threads;
    threads.reserve(swimmers_.size());

    for (const auto& s : swimmers_) {
        threads.emplace_back(&Simulation::swim_race, this, s);
    }

    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
}

const std::vector<Swimmer>& Simulation::results() const {
    return results_;
}

void Simulation::swim_race(Swimmer swimmer) {
    using namespace std::chrono;
    auto sleep_duration = duration<double>(dt_);

    {
        std::scoped_lock lk(cout_mutex_);
        std::cout << swimmer.name << " started. speed: " << swimmer.speed << " m/s\n";
    }

    while (swimmer.position < distance_) {
        std::this_thread::sleep_for(sleep_duration);

        swimmer.position += swimmer.speed * dt_;
        if (swimmer.position > distance_) swimmer.position = distance_;

        {
            std::scoped_lock lk(cout_mutex_);
            std::cout << std::fixed << std::setprecision(1)
                      << swimmer.name << " position: " << swimmer.position << "/" << distance_ << " m\n";
        }
    }

    {
        std::scoped_lock lk(results_mutex_);
        results_.push_back(swimmer);
    }

    {
        std::scoped_lock lk(cout_mutex_);
        std::cout << swimmer.name << " finished!\n";
    }
}
