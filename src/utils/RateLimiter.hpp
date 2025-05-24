#include <QElapsedTimer>

#include "Logger.hpp"

class RateLimiter {
    public:
        explicit RateLimiter(qint64 _intervalMs, std::function<void()> _function)
            : intervalMs(_intervalMs)
            , function(_function) {
            timer.start();
        }

        void execute() {
            if (timer.elapsed() < intervalMs) {
                logDebug() << "Rate limited: Ignored";
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