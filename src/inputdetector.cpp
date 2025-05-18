#include <QGuiApplication>

#include "inputdetector.h"
#include "logger.h"

namespace {
constexpr QEvent::Type  QEVENT_MOUSE_KEYBOARD_MIN_INCLUDE = QEvent::Type::MouseButtonPress;
constexpr QEvent::Type  QEVENT_MOUSE_KEYBOARD_MAX_INCLUDE = QEvent::Type::KeyRelease;
constexpr SDL_EventType SDL_EVENT_GAMEPAD_MIN_INCLUDE     = SDL_EVENT_GAMEPAD_AXIS_MOTION;
constexpr SDL_EventType SDL_EVENT_GAMEPAD_MAX_EXCLUDE     = SDL_EVENT_FINGER_DOWN;
} // namespace

InputDetector::InputDetector(QObject* parent)
    : QObject(parent)
    , sdlEventThread(SDL_CreateThread(sdlEventHandler, "EventThread", nullptr)) {
    // Initialize SDL subsystems
    if (SDL_Init(SDL_INIT_GAMEPAD) < 0) {
        logError() << "SDL could not initialize! SDL Error: " << SDL_GetError();
    }

    // Install global event filter for keyboard and mouse
    qApp->installEventFilter(this);
}

InputDetector::~InputDetector() {
    qApp->removeEventFilter(this);
    SDL_Quit();
}

void InputDetector::onGamepadConnected() {}

void InputDetector::resetIdleTimer() {}

int InputDetector::sdlEventHandler(void* data) {
    auto inputDetector = static_cast<InputDetector*>(data);
    if (!inputDetector) {
        logError() << "InputDetector is null in SDL event handler";
        return -1;
    }

    SDL_Event event;
    bool      running = true;

    logDebug() << "Starting SDL_WaitEvent loop";
    while (running && SDL_WaitEvent(&event)) {
        logDebug() << "SDL event: " << event.type;
        if (event.type >= SDL_EVENT_GAMEPAD_MIN_INCLUDE && event.type < SDL_EVENT_GAMEPAD_MAX_EXCLUDE) {
            inputDetector->resetIdleTimer();
            if (event.type == SDL_EVENT_GAMEPAD_ADDED) {
                inputDetector->onGamepadConnected();
            }
        } else if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
    }
    logDebug() << "Stopping SDL_WaitEvent loop";
    return 0;
}

bool InputDetector::eventFilter(QObject* watched, QEvent* event) {
    logDebug() << "QEvent: " << event->type();
    if (event->type() >= QEVENT_MOUSE_KEYBOARD_MIN_INCLUDE && event->type() <= QEVENT_MOUSE_KEYBOARD_MAX_INCLUDE) {
        resetIdleTimer();
    }

    // Always return false to allow the event to be processed by other filters
    return false;
}
