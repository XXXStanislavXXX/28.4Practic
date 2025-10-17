#pragma once
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include "Order.h"
#include "Kitchen.h"

class Restaurant {
public:
    Restaurant();
    void run();

private:
    void waiterLoop();
    void courierLoop();
    Order makeRandomOrder();

    // Shared state
    std::atomic<bool> stop_{false};
    std::mutex ioMtx_;

    std::queue<Order> ready_;
    std::mutex readyMtx_;

    std::atomic<int> delivered_{0};
    std::atomic<int> nextOrderId_{1};

    Kitchen kitchen_;
};
