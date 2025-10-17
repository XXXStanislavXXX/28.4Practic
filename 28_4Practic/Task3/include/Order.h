#pragma once
#include <cstdint>

enum class Dish {
    Pizza,
    Soup,
    Steak,
    Salad,
    Sushi
};

struct Order {
    std::int32_t id{};
    Dish dish{};
};
