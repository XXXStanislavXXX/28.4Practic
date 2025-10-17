// File: src/main.cpp
#include "train.h"
#include "station.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    Station station;

    std::vector<int> times(3);
    char labels[3] = {'A','B','C'};

    for (int i = 0; i < 3; ++i) {
        std::cout << "Enter travel time in seconds for train " << labels[i] << ": ";
        while (!(std::cin >> times[i]) || times[i] < 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a non-negative integer for train " << labels[i] << ": ";
        }
    }
    // consume leftover newline before getline loop
    std::string line;
    std::getline(std::cin, line);

    // Create and start trains
    std::vector<Train> trains;
    trains.reserve(3);
    for (int i = 0; i < 3; ++i) {
        trains.emplace_back(labels[i], times[i], station);
        trains.back().start();
    }

    // Main loop: accept user commands until all trains have departed
    while (station.departed_count() < 3) {
        std::cout << "(type 'depart' to release the train on the platform) > ";
        if (!std::getline(std::cin, line)) break;
        if (line == "depart") {
            station.signal_depart_for_current();
        } else if (line.empty()) {
            continue;
        } else {
            std::cout << "Unknown command. Only 'depart' is expected.\n";
        }
    }

    // Join train threads
    for (auto &t : trains) t.join();

    std::cout << "All trains visited the station. Program exiting.\n";
    return 0;
}
