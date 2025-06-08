#pragma once

#include <QElapsedTimer>

class RateLimiterByTime {
    public:
        explicit RateLimiterByTime(qint64 _intervalMs, std::function<void()> _function)
            : intervalMs(_intervalMs)
            , function(_function) {
            timer.start();
        }

        void execute() {
            if (timer.elapsed() < intervalMs) {
                return;
            }
            function();
            timer.restart();
        }

    private:
        qint64                intervalMs;
        QElapsedTimer         timer;
        std::function<void()> function;
};

class RateLimiterByCount {
    public:
        explicit RateLimiterByCount(uint64_t _callInterval, std::function<void()> _function)
            : callInterval(_callInterval)
            , function(_function) {
        }

        void execute() {
            if ((callCount++) < callInterval) {
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