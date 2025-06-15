#pragma once

#include <QEvent>
#include <QGuiApplication>
#include <QObject>
#include <QTimer>

#include "../common/RateLimiter.hpp"

namespace SeekerStone::Components {
class IdleManager : public QObject {
        Q_OBJECT

    public:
        explicit IdleManager(QGuiApplication& _main, QObject* parent = nullptr);
        ~IdleManager();

    protected:
        // Event filter to catch all input events
        bool eventFilter(QObject* watched, QEvent* event) override;

    private:
        QTimer                    idleTimer;
        Common::RateLimiterByTime idleTimerResetter;
        QGuiApplication&          main;

        void onIdle();
        void setupIdleTimer();
};
} // namespace SeekerStone::Components
