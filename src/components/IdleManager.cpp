#include "IdleManager.hpp"
#include "Logger.hpp"

namespace {
constexpr QEvent::Type QEVENT_MOUSE_KEYBOARD_MIN_INCLUDE = QEvent::Type::MouseButtonPress;
constexpr QEvent::Type QEVENT_MOUSE_KEYBOARD_MAX_INCLUDE = QEvent::Type::KeyRelease;
constexpr uintmax_t    IDLE_TIMER_INTERVAL_NS            = 1'000'000'000;
} // namespace

namespace SeekerStone::Components {
IdleManager::IdleManager(QGuiApplication& _main, QObject* parent)
    : QObject(parent)
    , idleTimer(this)
    , idleTimerResetter(IDLE_TIMER_INTERVAL_NS, std::bind(&IdleManager::onIdle, this))
    , main(_main) {
    connect(&idleTimer, &QTimer::timeout, this, &IdleManager::onIdle);
    setupIdleTimer();
    main.installEventFilter(this);
}

IdleManager::~IdleManager() {
    main.removeEventFilter(this);
    idleTimer.stop();
}

bool IdleManager::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() >= QEVENT_MOUSE_KEYBOARD_MIN_INCLUDE && event->type() <= QEVENT_MOUSE_KEYBOARD_MAX_INCLUDE) {
        logDebug << "QEvent:" << event->type();
        idleTimerResetter.execute();
    }

    // Always return false to allow the event to be processed by other filters
    return false;
}

void IdleManager::onIdle() {
    // TODO
    logDebug << "onIdle";
}

void IdleManager::setupIdleTimer() {
    idleTimer.stop();
    // TODO: setInterval based on settings
    idleTimer.setInterval(30 * 60 * 1000);
    idleTimer.setSingleShot(true);
    idleTimer.start();
}
} // namespace SeekerStone::Components
