// File: src/Simulation.cpp
#include "Simulation.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <chrono>

static const int TOTAL_SWIMMERS = 6;

struct SharedState {
    std::mutex mtx;
    std::condition_variable cv_tick;      // notify swimmers of new tick
    std::condition_variable cv_processed; // notify main that swimmers processed tick
    int tick = 0;
    std::atomic<int> processed_count{0};
    std::atomic<int> finished_count{0};
    bool stopping = false;
};

Simulation::Simulation(const std::vector<std::string> &names, const std::vector<double> &speeds) {
    num_swimmers = TOTAL_SWIMMERS;
    swimmers.reserve(num_swimmers);
    for (int i = 0; i < num_swimmers; ++i) {
        swimmers.emplace_back(names[i], speeds[i]);
    }
}

Simulation::~Simulation() = default;

void Simulation::start_threads() {
    // not used: threads are managed inside run for clarity
}

void Simulation::stop_threads() {
    // not used
}

void Simulation::run() {
    SharedState state;

    // Launch swimmer threads
    std::vector<std::thread> threads;
    threads.reserve(num_swimmers);
    for (int i = 0; i < num_swimmers; ++i) {
        threads.emplace_back([this, i, &state]() {
            while (true) {
                // wait for new tick or stop
                std::unique_lock<std::mutex> lk(state.mtx);
                state.cv_tick.wait(lk, [&state, i] { return state.tick > 0 || state.stopping; });
                if (state.stopping) break;
                int tick = state.tick;
                lk.unlock();

                // process the tick for swimmer i
                bool just_finished = swimmers[i].process_tick(tick);
                if (just_finished) {
                    state.finished_count.fetch_add(1);
                }

                // signal processed
                state.processed_count.fetch_add(1);
                state.cv_processed.notify_one();
            }
        });
    }

    // Main simulation loop: advance ticks until all finished
    std::cout << std::fixed << std::setprecision(2);
    while (state.finished_count.load() < num_swimmers) {
        // advance tick
        {
            std::lock_guard<std::mutex> lk(state.mtx);
            state.tick += 1;
            // reset processed_count for this tick
            state.processed_count.store(0);
        }
        state.cv_tick.notify_all();

        // wait until all threads processed this tick
        {
            std::unique_lock<std::mutex> lk(state.mtx);
            state.cv_processed.wait(lk, [&state, this] {
                return state.processed_count.load() >= num_swimmers;
            });
        }

        // print progress at this integer second
        int t = state.tick;
        std::cout << "Second " << t << ":\n";
        for (int i = 0; i < num_swimmers; ++i) {
            double dist = swimmers[i].distance;
            if (dist > 100.0) dist = 100.0;
            std::cout << "  Lane " << (i+1) << " (" << swimmers[i].name << "): "
                      << dist << " m\n";
        }
        std::cout << std::flush;

        // wait real 1 second so output appears each second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // stop threads
    {
        std::lock_guard<std::mutex> lk(state.mtx);
        state.stopping = true;
    }
    state.cv_tick.notify_all();
    for (auto &th : threads) if (th.joinable()) th.join();

    // prepare results sorted by finish_time ascending
    std::vector<int> idx(num_swimmers);
    for (int i = 0; i < num_swimmers; ++i) idx[i] = i;
    std::sort(idx.begin(), idx.end(), [this](int a, int b) {
        return swimmers[a].finish_time < swimmers[b].finish_time;
    });

    std::cout << "\nFinal results (sorted by time):\n";
    std::cout << std::setw(4) << "Pos" << std::setw(8) << "Lane" << std::setw(20) << "Name"
              << std::setw(12) << "Time(s)" << "\n";
    for (int pos = 0; pos < num_swimmers; ++pos) {
        int i = idx[pos];
        std::cout << std::setw(4) << (pos+1)
                  << std::setw(8) << (i+1)
                  << std::setw(20) << swimmers[i].name
                  << std::setw(12) << std::fixed << std::setprecision(3) << swimmers[i].finish_time
                  << "\n";
    }
}
