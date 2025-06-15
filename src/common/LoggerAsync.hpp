#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "Logger.hpp"

namespace Common {
class LoggerAsync : private Logger {
    public:
        using Logger::Logger;

        ~LoggerAsync() {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                running = false;
                queueAvailable.notify_all();
            }
            if (thread.joinable()) {
                thread.join();
            }
        }

        LoggerAsync& operator<<(const std::string& msg) {
            std::lock_guard<std::mutex> lock(queueMutex);
            queue.emplace(msg);
            queueAvailable.notify_one();
            return *this;
        }

    private:
        std::queue<std::string> queue;
        std::mutex              queueMutex;
        std::condition_variable queueAvailable;
        std::thread             thread{&LoggerAsync::run, this};
        std::atomic<bool>       running = true;

        void run() {
            while (true) {
                std::unique_lock<std::mutex> lock(queueMutex);
                queueAvailable.wait(lock, [&]() {
                    return (!running) || (!queue.empty());
                });
                if (queue.empty()) {
                    if (running) {
                        continue;
                    } else {
                        return;
                    }
                }
                const std::string& msg = queue.front();
                Logger::           operator<<(msg);
                queue.pop();
            }
        }
};
} // namespace Common
