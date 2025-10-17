#include "Restaurant.h"
#include "Utils.h"
#include <iostream>
#include <vector>
#include <chrono>

using namespace std::chrono_literals;

Restaurant::Restaurant()
    : kitchen_(ready_, readyMtx_, ioMtx_, stop_) {}

void Restaurant::run() {
    std::thread waiter(&Restaurant::waiterLoop, this);
    std::thread courier(&Restaurant::courierLoop, this);

    waiter.join();
    courier.join();

    std::lock_guard<std::mutex> out(ioMtx_);
    std::cout << "[System] Simulation finished.\n";
}

void Restaurant::waiterLoop() {
    while (!stop_.load()) {
        const int pauseSec = randRange(5, 10);
        std::this_thread::sleep_for(std::chrono::seconds(pauseSec));
        if (stop_.load()) break;

        Order order = makeRandomOrder();
        {
            std::lock_guard<std::mutex> out(ioMtx_);
            std::cout << "[Waiter] New order #" << order.id
                      << " (" << dishToString(order.dish) << ")\n";
        }
        kitchen_.submitOrder(order);
    }
}

void Restaurant::courierLoop() {
    while (!stop_.load()) {
        std::this_thread::sleep_for(30s);
        if (stop_.load()) break;

        std::vector<Order> batch;
        {
            std::lock_guard<std::mutex> rm(readyMtx_);
            while (!ready_.empty() && !stop_.load()) {
                batch.push_back(ready_.front());
                ready_.pop();
            }
        }

        {
            std::lock_guard<std::mutex> out(ioMtx_);
            std::cout << "[Courier] Arrived. Ready items: " << batch.size() << "\n";
        }

        if (batch.empty()) continue;

        int remaining = 10 - delivered_.load();
        int toDeliver = remaining > 0 ? std::min<int>(remaining, static_cast<int>(batch.size())) : 0;

        for (int i = 0; i < toDeliver; ++i) {
            {
                std::lock_guard<std::mutex> out(ioMtx_);
                std::cout << "[Courier] Delivered order #" << batch[i].id
                          << " (" << dishToString(batch[i].dish) << ")\n";
            }
            delivered_.fetch_add(1);
        }

        if (delivered_.load() >= 10) {
            stop_.store(true);
            kitchen_.stop();
            std::lock_guard<std::mutex> out(ioMtx_);
            std::cout << "[System] Delivered 10 orders. Shutting down.\n";
            break;
        }
    }
}

Order Restaurant::makeRandomOrder() {
    Order o;
    o.id = nextOrderId_.fetch_add(1);
    int r = randRange(0, 4);
    switch (r) {
        case 0: o.dish = Dish::Pizza; break;
        case 1: o.dish = Dish::Soup;  break;
        case 2: o.dish = Dish::Steak; break;
        case 3: o.dish = Dish::Salad; break;
        default: o.dish = Dish::Sushi; break;
    }
    return o;
}
