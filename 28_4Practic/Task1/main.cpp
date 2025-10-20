#include <iostream>
#include <limits>
#include "Simulation.h"
#include "Swimmer.h"

int main() {
    const double distance = 100.0;
    const double dt = 1.0; // обновление раз в секунду
    Simulation sim(distance, dt);

    std::cout << "How many swimmers? ";
    int n = 0;
    if (!(std::cin >> n) || n <= 0) return 0;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < n; ++i) {
        std::string name;
        double speed = 0.0;

        std::cout << "Enter name for swimmer " << (i + 1) << ": ";
        std::getline(std::cin, name);
        if (name.empty()) {
            name = "Swimmer" + std::to_string(i + 1);
            std::cout << "Empty name -> using default: " << name << "\n";
        }

        std::cout << "Enter speed (m/s) for " << name << ": ";
        while (!(std::cin >> speed)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid number, please enter speed (m/s): ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        sim.add_swimmer(Swimmer(name, speed));
    }

    sim.run();

    std::cout << "\nFinish order:\n";
    const auto& res = sim.results();
    for (size_t i = 0; i < res.size(); ++i) {
        std::cout << i + 1 << ". " << res[i].name << " (speed " << res[i].speed << " m/s)\n";
    }

    return 0;
}
