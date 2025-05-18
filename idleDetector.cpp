#include <QApplication>
#include <QEvent>

#include "idleDetector.h"

namespace {
constexpr QEvent::Type QEVENT_MOUSE_KEYBOARD_MIN = QEvent::Type::MouseButtonPress;
constexpr QEvent::Type QEVENT_MOUSE_KEYBOARD_MAX = QEvent::Type::KeyRelease;
} // namespace

IdleDetector::IdleDetector(QObject *parent)
    : QObject(parent) {
    // Call resetIdleTimer on gamepad inputs
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonPressEvent, this,
            [this](int, QGamepadManager::GamepadButton, double) { resetIdleTimer(); });

    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonReleaseEvent, this,
            [this](int, QGamepadManager::GamepadButton) { resetIdleTimer(); });

    connect(QGamepadManager::instance(), &QGamepadManager::gamepadAxisEvent, this,
            [this](int, QGamepadManager::GamepadAxis, double) { resetIdleTimer(); });

    // Connect gamepad connection signal to wake up the system
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadConnected, this,
            [this](int) { onGamepadConnected(); });

    // Install global event filter for keyboard and mouse
    qApp->installEventFilter(this);
}

IdleDetector::~IdleDetector() {
    qApp->removeEventFilter(this);
}

void IdleDetector::onGamepadConnected() {}

void IdleDetector::resetIdleTimer() {}

bool IdleDetector::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() >= QEVENT_MOUSE_KEYBOARD_MIN && event->type() <= QEVENT_MOUSE_KEYBOARD_MAX) {
        resetIdleTimer();
    }

    // Always return false to allow the event to be processed by other filters
    return false;
}
