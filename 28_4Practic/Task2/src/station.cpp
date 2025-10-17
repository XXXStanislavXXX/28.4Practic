// File: src/station.cpp
#include "station.h"
#include <iostream>
#include <chrono>

Station::Station()
    : current_train_(-1),
      depart_allowed_(-1),
      departed_count_(0) {}

void Station::arrive_and_wait_depart(int train_id, char label) {
    {
        std::lock_guard<std::mutex> out_lock(cout_mutex_);
        std::cout << "Train " << label << " reached the station area (id=" << train_id << ").\n";
    }

    // Try to acquire platform (blocks other trains)
    {
        std::unique_lock<std::mutex> platform_lock(platform_mutex_, std::defer_lock);
        if (!platform_lock.try_lock()) {
            {
                std::lock_guard<std::mutex> out_lock(cout_mutex_);
                std::cout << "Train " << label << " is waiting for a free platform.\n";
            }
            platform_lock.lock(); // wait until free
        }

        // Platform acquired
        current_train_.store(train_id, std::memory_order_release);
        {
            std::lock_guard<std::mutex> out_lock(cout_mutex_);
            std::cout << "Train " << label << " arrived on the platform.\n";
            std::cout << "Type 'depart' to let train " << label << " depart.\n";
        }

        // Wait for depart signal for this train
        std::unique_lock<std::mutex> dep_lock(depart_mutex_);
        depart_cv_.wait(dep_lock, [&]{ return depart_allowed_ == train_id; });

        // Departing
        {
            std::lock_guard<std::mutex> out_lock(cout_mutex_);
            std::cout << "Train " << label << " is departing from the platform.\n";
        }

        // Clear platform state
        current_train_.store(-1, std::memory_order_release);
        depart_allowed_ = -1;

        // release platform_lock (platform becomes free for waiting trains)
        platform_lock.unlock();

        // increment departed count
        departed_count_.fetch_add(1, std::memory_order_relaxed);
    }
}

bool Station::signal_depart_for_current() {
    int cur = current_train_.load(std::memory_order_acquire);
    if (cur == -1) {
        std::lock_guard<std::mutex> out_lock(cout_mutex_);
        std::cout << "No train currently on the platform to depart.\n";
        return false;
    }

    {
        std::lock_guard<std::mutex> dep_lock(depart_mutex_);
        // re-check to avoid race where train left between load and lock
        if (current_train_.load(std::memory_order_acquire) == -1) {
            std::lock_guard<std::mutex> out_lock(cout_mutex_);
            std::cout << "No train currently on the platform to depart.\n";
            return false;
        }
        depart_allowed_ = cur;
    }
    depart_cv_.notify_one();
    return true;
}

int Station::get_current_train() {
    return current_train_.load(std::memory_order_acquire);
}

int Station::departed_count() const {
    return departed_count_.load(std::memory_order_relaxed);
}
