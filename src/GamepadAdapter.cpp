#include <QGuiApplication>
#include <QKeyEvent>

#include "GamepadAdapter.hpp"
#include "utils/Logger.hpp"

namespace {
constexpr SDL_EventType SDL_EVENT_GAMEPAD_MIN_INCLUDE = SDL_EVENT_GAMEPAD_AXIS_MOTION;
constexpr SDL_EventType SDL_EVENT_GAMEPAD_MAX_EXCLUDE = SDL_EVENT_FINGER_DOWN;
constexpr Sint16        SDL_TRIGGER_AXIS_VALUE_MIN    = 0;
constexpr Sint16        SDL_TRIGGER_AXIS_VALUE_MAX    = SDL_JOYSTICK_AXIS_MAX;
constexpr Sint16        SDL_TRIGGER_AXIS_VALUE_CENTER = (SDL_TRIGGER_AXIS_VALUE_MAX + SDL_TRIGGER_AXIS_VALUE_MIN) / 2;
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
                    gamepadAdapter->onGamepadButton(true, static_cast<SDL_GamepadButton>(event.gbutton.button));
                    break;
                case SDL_EVENT_GAMEPAD_BUTTON_UP:
                    gamepadAdapter->onGamepadButton(false, static_cast<SDL_GamepadButton>(event.gbutton.button));
                    break;
                case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                    gamepadAdapter->onGamepadAxis(event.gaxis.value, static_cast<SDL_GamepadAxis>(event.gaxis.axis));
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

void GamepadAdapter::onGamepadButton(bool buttonDown, SDL_GamepadButton button) {
    logDebug() << "Gamepad button " << buttonDown << ": " << button;

    Qt::Key      qtKey;
    QEvent::Type qtEvent = buttonDown ? QEvent::KeyPress : QEvent::KeyRelease;

    if (gamepadButtonMapping.contains(button)) {
        qtKey = gamepadButtonMapping.at(button);
    } else {
        return;
    }

    QGuiApplication::postEvent(QGuiApplication::focusObject(), new QKeyEvent(qtEvent, qtKey, Qt::NoModifier));
}

void GamepadAdapter::onGamepadAxis(Sint16 axisValue, SDL_GamepadAxis axis) {
    logDebug() << "Gamepad axis " << axis << ": " << axisValue;

    QEvent::Type qtEvent = (axisValue >= SDL_TRIGGER_AXIS_VALUE_CENTER) ? QEvent::KeyPress : QEvent::KeyRelease;
    Qt::Key      qtKey;

    if (gamepadAxisAsButtonMapping.contains(axis) && gamepadAxisAsButtonMapping.at(axis).second != qtEvent) {
        qtKey = gamepadAxisAsButtonMapping.at(axis).first;
    } else {
        return;
    }

    gamepadAxisAsButtonMapping.at(axis).second = qtEvent;
    QGuiApplication::postEvent(QGuiApplication::focusObject(), new QKeyEvent(qtEvent, qtKey, Qt::NoModifier));
}
