#include "train.h"
#include <iostream>
#include <chrono>

Train::Train(char label, int travel_seconds, Station& station)
    : label_(label), travel_seconds_(travel_seconds), station_(station) {}

void Train::start() {
    thread_ = std::thread(&Train::run, this);
}

void Train::join() {
    if (thread_.joinable()) thread_.join();
}

void Train::run() {
    // Simulate travel time
    std::this_thread::sleep_for(std::chrono::seconds(travel_seconds_));

    // Use integer id for the train (simple mapping from label)
    int id = static_cast<int>(label_);

    // Arrive and wait for depart
    station_.arrive_and_wait_depart(id, label_);
}
