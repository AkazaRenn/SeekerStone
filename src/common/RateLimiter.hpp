#pragma once

#include <functional>

#include "ElapsedTimer.hpp"

namespace Common {
class RateLimiterByTime {
    public:
        explicit RateLimiterByTime(uintmax_t _intervalNs, std::function<void()> _function)
            : intervalNs(_intervalNs)
            , function(_function) {
            timer.start();
        }

        void execute() {
            if (timer.elapsed() < intervalNs) {
                return;
            }
            function();
            timer.restart();
        }

    private:
        uintmax_t             intervalNs;
        ElapsedTimer          timer;
        std::function<void()> function;
};

class RateLimiterByCount {
    public:
        explicit RateLimiterByCount(uint64_t _callInterval, std::function<void()> _function)
            : callInterval(_callInterval)
            , function(_function) {
        }

        void execute() {
            if (callCount++ < callInterval) {
                return;
            }
            function();
            callCount = 0;
        }

    private:
        uint64_t              callCount;
        uint64_t              callInterval;
        std::function<void()> function;
};
} // namespace Common
