// File: src/Swimmer.cpp
#include "Swimmer.h"
#include <algorithm>

Swimmer::Swimmer(const std::string &n, double s)
    : name(n), speed(s), distance(0.0), finished(false),
      finish_time(0.0), last_processed_tick(0) {}

bool Swimmer::process_tick(int tick) {
    if (last_processed_tick >= tick) return false; // already processed
    last_processed_tick = tick;

    if (finished) return false;

    double before = distance;
    // interval is 1.0 seconds: from (tick-1) to tick
    double after = before + speed;

    if (after >= 100.0) {
        // compute fractional time inside the interval when reached 100m
        double needed = 100.0 - before;
        double frac = (speed > 0.0) ? (needed / speed) : 1.0;
        finish_time = (tick - 1) + frac;
        distance = 100.0;
        finished = true;
        return true;
    } else {
        distance = after;
        return false;
    }
}
