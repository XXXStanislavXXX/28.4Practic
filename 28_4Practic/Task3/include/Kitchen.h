#pragma once
#include <queue>
#include <mutex>
#include <atomic>
#include "Order.h"

class Kitchen {
public:
    Kitchen(std::queue<Order>& readyQueue,
            std::mutex& readyMutex,
            std::mutex& ioMutex,
            std::atomic<bool>& stopFlag);

    void submitOrder(const Order& order);
    void stop();

private:
    void startCookingLocked(const Order& order);

    // Shared with restaurant
    std::queue<Order>& ready_;
    std::mutex& readyMtx_;
    std::mutex& ioMtx_;
    std::atomic<bool>& stop_;

    // Internal state
    std::mutex mtx_;
    bool busy_{false};
    std::queue<Order> pending_;
};
