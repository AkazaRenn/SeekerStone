#include <QGuiApplication>
#include <QKeyEvent>
#include <SDL3/SDL_gamepad.h>

#include "GamepadAdapter.hpp"
#include "utils/Logger.hpp"

namespace {
constexpr SDL_EventType SDL_EVENT_GAMEPAD_MIN_INCLUDE = SDL_EVENT_GAMEPAD_AXIS_MOTION;
constexpr SDL_EventType SDL_EVENT_GAMEPAD_MAX_EXCLUDE = SDL_EVENT_FINGER_DOWN;
} // namespace

GamepadAdapter::GamepadAdapter(QObject* parent)
    : QObject(parent)
    , sdlEventThread(SDL_CreateThread(sdlEventHandler, "EventThread", this), sdlThreadCleanup) {
    // Initialize SDL subsystems
    if (SDL_Init(SDL_INIT_GAMEPAD) < 0) {
        logError() << "SDL could not initialize! SDL Error: " << SDL_GetError();
    }
}

GamepadAdapter::~GamepadAdapter() {
    SDL_Quit();
}

void GamepadAdapter::sdlThreadCleanup(SDL_Thread* thread) {
    logDebug() << "SDL thread cleanup";
    SDL_WaitThread(thread, nullptr);
    logDebug() << "SDL thread cleanup done";
}

int GamepadAdapter::sdlEventHandler(void* data) {
    auto gamepadAdapter = static_cast<GamepadAdapter*>(data);
    if (!gamepadAdapter) {
        logError() << "GamepadAdapter is null in SDL event handler";
        return -1;
    }

    SDL_Event event;
    bool      running = true;

    logDebug() << "Starting SDL_WaitEvent loop";
    while (running && SDL_WaitEvent(&event)) {
        logDebug() << "SDL event: " << event.type;
        if (event.type >= SDL_EVENT_GAMEPAD_MIN_INCLUDE && event.type < SDL_EVENT_GAMEPAD_MAX_EXCLUDE) {
            emit gamepadAdapter->signalGamepadAction();

            switch (event.type) {
                case SDL_EVENT_GAMEPAD_ADDED:
                    emit gamepadAdapter->signalGamepadConnected();
                    break;
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                    gamepadAdapter->onGamepadButton(true, event.gbutton.button);
                    break;
                case SDL_EVENT_GAMEPAD_BUTTON_UP:
                    gamepadAdapter->onGamepadButton(false, event.gbutton.button);
                    break;
                default:
                    break;
            }
        } else if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
    }
    logDebug() << "Stopping SDL_WaitEvent loop";
    return 0;
}

void GamepadAdapter::onGamepadButton(bool buttonDown, Uint8 button) {
    logDebug() << "Gamepad button " << buttonDown << ": " << button;

    Qt::Key qtKey;
    switch (button) {
        case SDL_GamepadButton::SDL_GAMEPAD_BUTTON_DPAD_UP:
            qtKey = Qt::Key_Up;
            break;
        case SDL_GamepadButton::SDL_GAMEPAD_BUTTON_DPAD_DOWN:
            qtKey = Qt::Key_Down;
            break;
        case SDL_GamepadButton::SDL_GAMEPAD_BUTTON_DPAD_LEFT:
            qtKey = Qt::Key_Left;
            break;
        case SDL_GamepadButton::SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
            qtKey = Qt::Key_Right;
            break;
        case SDL_GamepadButton::SDL_GAMEPAD_BUTTON_SOUTH:
            qtKey = Qt::Key_Return;
            break;
        case SDL_GamepadButton::SDL_GAMEPAD_BUTTON_EAST:
            qtKey = Qt::Key_Escape;
            break;
        case SDL_GamepadButton::SDL_GAMEPAD_BUTTON_WEST:
            qtKey = Qt::Key_Backspace;
            break;
        case SDL_GamepadButton::SDL_GAMEPAD_BUTTON_NORTH:
            qtKey = Qt::Key_Space;
            break;
        default:
            return;
    }

    auto keyEvent = new QKeyEvent(buttonDown ? QEvent::KeyPress : QEvent::KeyRelease, qtKey, Qt::NoModifier);
    QGuiApplication::postEvent(QGuiApplication::focusObject(), keyEvent);
}
