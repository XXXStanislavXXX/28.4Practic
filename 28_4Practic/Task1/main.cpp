// File: src/main.cpp
#include "Simulation.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>

int main() {
    const int N = 6;
    std::vector<std::string> names;
    std::vector<double> speeds;
    names.reserve(N);
    speeds.reserve(N);

    // Read 6 names
    for (int i = 0; i < N; ++i) {
        std::cout << "Enter name of swimmer " << (i + 1) << ": ";
        std::string s;
        if (!(std::cin >> s)) return 0;
        names.push_back(s);
    }

    // Read 6 speeds (m/s)
    for (int i = 0; i < N; ++i) {
        std::cout << "Enter speed " << (i + 1) << ": ";
        double v;
        if (!(std::cin >> v)) return 0;
        speeds.push_back(v);
    }

    Simulation sim(names, speeds);
    sim.run();

    return 0;
}
