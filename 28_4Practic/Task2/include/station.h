// File: include/station.h
#pragma once
// Station: manages a single platform where only one train can be present at a time.
// Trains call arrive_and_wait_depart(...) to occupy the platform and then wait
// for a user-issued depart command. The class is thread-safe.
#include <condition_variable>
#include <mutex>
#include <atomic>

class Station {
public:
    Station();
    // Called by a train thread after travel time.
    // Blocks until the platform is free, then waits for a depart signal for this train.
    // Returns when the train departs.
    void arrive_and_wait_depart(int train_id, char label);

    // Signal the currently occupying train to depart. Returns true if a train was signaled.
    bool signal_depart_for_current();

    // Returns the id of the train currently on the platform, or -1 if none.
    int get_current_train();

    // Number of trains that already departed from the station.
    int departed_count() const;

private:
    std::mutex platform_mutex_;      // protects platform acquisition by trains
    std::atomic<int> current_train_; // id of train on platform, -1 if none

    std::mutex depart_mutex_;        // protects depart condition and depart_allowed_
    std::condition_variable depart_cv_;
    int depart_allowed_;             // id that is allowed to depart (protected by depart_mutex_)

    std::mutex cout_mutex_;          // protect console output

    std::atomic<int> departed_count_;
};
