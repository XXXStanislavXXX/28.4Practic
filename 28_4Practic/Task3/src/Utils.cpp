#include "Utils.h"
#include <cstdlib>

int randRange(int min, int max) {
    if (min > max) {
        int t = min; min = max; max = t;
    }
    return min + std::rand() % (max - min + 1);
}

const char* dishToString(Dish d) {
    switch (d) {
        case Dish::Pizza: return "Pizza";
        case Dish::Soup:  return "Soup";
        case Dish::Steak: return "Steak";
        case Dish::Salad: return "Salad";
        case Dish::Sushi: return "Sushi";
        default:          return "Unknown";
    }
}
