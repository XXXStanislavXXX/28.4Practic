#include "Kitchen.h"
#include "Utils.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

Kitchen::Kitchen(std::queue<Order>& readyQueue,
                 std::mutex& readyMutex,
                 std::mutex& ioMutex,
                 std::atomic<bool>& stopFlag)
    : ready_(readyQueue),
      readyMtx_(readyMutex),
      ioMtx_(ioMutex),
      stop_(stopFlag) {}

void Kitchen::stop() {
    std::lock_guard<std::mutex> lk(mtx_);
    // Do not start new dishes after stop; let running cook finish silently.
}

void Kitchen::submitOrder(const Order& order) {
    std::lock_guard<std::mutex> lk(mtx_);
    if (stop_.load()) return;

    if (!busy_) {
        startCookingLocked(order);
    } else {
        pending_.push(order);
        std::lock_guard<std::mutex> out(ioMtx_);
        std::cout << "[Kitchen] Queued order #" << order.id
                  << " (" << dishToString(order.dish) << ")\n";
    }
}

void Kitchen::startCookingLocked(const Order& order) {
    busy_ = true;
    const int cookSec = randRange(5, 15);

    {
        std::lock_guard<std::mutex> out(ioMtx_);
        std::cout << "[Kitchen] Start cooking order #" << order.id
                  << " (" << dishToString(order.dish) << "), time " << cookSec << "s\n";
    }

    std::thread([this, order, cookSec]() {
        std::this_thread::sleep_for(std::chrono::seconds(cookSec));

        if (!stop_.load()) {
            {
                std::lock_guard<std::mutex> rm(readyMtx_);
                ready_.push(order);
            }
            std::lock_guard<std::mutex> out(ioMtx_);
            std::cout << "[Kitchen] Ready order #" << order.id
                      << " (" << dishToString(order.dish) << ")\n";
        }

        // Decide next cooking task
        std::lock_guard<std::mutex> lk(mtx_);
        busy_ = false;
        if (stop_.load()) {
            // Drop pending silently on stop
            std::queue<Order> empty;
            std::swap(pending_, empty);
            return;
        }
        if (!pending_.empty()) {
            Order next = pending_.front();
            pending_.pop();
            startCookingLocked(next); // mtx_ is held; this is the locked variant
        }
    }).detach();
}
