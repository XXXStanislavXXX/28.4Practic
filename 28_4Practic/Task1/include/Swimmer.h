#pragma once
#include <string>

class Swimmer {
public:
    Swimmer(const std::string& name, double speed);

    std::string name;
    double speed;    // m/s
    double position; // m
};
