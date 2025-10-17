
#pragma once
#include "station.h"
#include <thread>

class Train {
public:
    Train(char label, int travel_seconds, Station& station);
    void start();   // launches internal thread
    void join();    // join internal thread

private:
    void run();

    char label_;
    int travel_seconds_;
    Station& station_;
    std::thread thread_;
};