#pragma once

#include <QEvent>
#include <QObject>
#include <QTimer>

#include "../utils/RateLimiter.hpp"

class IdleManager : public QObject {
        Q_OBJECT

    public:
        explicit IdleManager(QObject* parent = nullptr);
        ~IdleManager();

    protected:
        // Event filter to catch all input events
        bool eventFilter(QObject* watched, QEvent* event) override;

    private:
        QTimer            idleTimer;
        RateLimiterByTime idleTimerResetter;

        void onIdle();
        void setupIdleTimer();
};
