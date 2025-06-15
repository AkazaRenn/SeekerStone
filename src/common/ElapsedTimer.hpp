#pragma once

#include <chrono>

namespace Common {
class ElapsedTimer {
    public:
        void start() {
            startTime = std::chrono::steady_clock::now();
        }

        uintmax_t elapsed() const {
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime)
                .count();
        }

        uintmax_t restart() {
            uintmax_t elapsed = this->elapsed();
            this->start();
            return elapsed;
        }

    private:
        std::chrono::time_point<std::chrono::steady_clock> startTime;
};
} // namespace Common