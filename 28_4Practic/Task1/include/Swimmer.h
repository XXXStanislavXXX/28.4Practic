#pragma once

#include <string>

class Swimmer {
public:
    Swimmer(const std::string &name, double speed);

    // Process one tick (interval from tick-1 to tick).
    // 'tick' is the current integer second (1,2,...).
    // Returns true if this tick caused the swimmer to finish.
    bool process_tick(int tick);

    std::string name;
    double speed;        // meters per second
    double distance;     // current distance in meters (<= 100)
    bool finished;
    double finish_time;  // time in seconds (may be fractional)
    int last_processed_tick;
};
