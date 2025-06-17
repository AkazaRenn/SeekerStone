#include <QKeyEvent>

#include "GamepadAdapter.hpp"
#include "Logger.hpp"

namespace {
constexpr SDL_EventType SDL_EVENT_GAMEPAD_MIN_INCLUDE = SDL_EVENT_GAMEPAD_AXIS_MOTION;
constexpr SDL_EventType SDL_EVENT_GAMEPAD_MAX_EXCLUDE = SDL_EVENT_FINGER_DOWN;
constexpr Sint16        SDL_TRIGGER_AXIS_VALUE_MIN    = 0;
constexpr Sint16        SDL_TRIGGER_AXIS_VALUE_MAX    = SDL_JOYSTICK_AXIS_MAX;
constexpr Sint16        SDL_TRIGGER_AXIS_VALUE_CENTER = (SDL_TRIGGER_AXIS_VALUE_MAX + SDL_TRIGGER_AXIS_VALUE_MIN) / 2;
} // namespace

namespace SeekerStone::Components {
GamepadAdapter::GamepadAdapter(QGuiApplication& _main, QObject* parent)
    : QObject(parent)
    , main(_main)
    , sdlEventThread(nullptr, sdlThreadCleanup) {
    // Initialize SDL subsystems
    if (!SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1")) {
        logWarning << "Failed setting SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS hint";
    }
    if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_GAMEPAD)) {
        logError << "SDL could not initialize! SDL Error:" << SDL_GetError();
    } else {
        sdlEventThread.reset(SDL_CreateThread(sdlEventHandler, "EventThread", this));
    }
}

void GamepadAdapter::sdlThreadCleanup(SDL_Thread* thread) {
    if (!thread) {
        return;
    }

    logDebug << "SDL thread cleanup";

    SDL_Event event;
    event.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&event);
    SDL_WaitThread(thread, nullptr);

    logDebug << "SDL quit";
    SDL_Quit();
}

int GamepadAdapter::sdlEventHandler(void* data) {
    auto gamepadAdapter = static_cast<GamepadAdapter*>(data);
    if (!gamepadAdapter) {
        logError << "GamepadAdapter is null in SDL event handler";
        return -1;
    }

    SDL_Event event;
    bool      running = true;

    logDebug << "Starting SDL_WaitEvent loop";
    while (running && SDL_WaitEvent(&event)) {
        logDebug << "SDL event:" << event.type;
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
    logDebug << "Stopping SDL_WaitEvent loop:" << SDL_GetError();
    return 0;
}

void GamepadAdapter::onGamepadButton(bool buttonDown, SDL_GamepadButton button) {
    logDebug << "Gamepad button" << buttonDown << ":" << button;

    Qt::Key      qtKey;
    QEvent::Type qtEvent = buttonDown ? QEvent::KeyPress : QEvent::KeyRelease;

    const auto it = gamepadButtonMapping.find(button);
    if (it == gamepadButtonMapping.end()) {
        return;
    } else {
        qtKey = it->second;
    }

    main.postEvent(main.focusObject(), new QKeyEvent(qtEvent, qtKey, Qt::NoModifier));
}

void GamepadAdapter::onGamepadAxis(Sint16 axisValue, SDL_GamepadAxis axis) {
    logDebug << "Gamepad axis" << axis << ":" << axisValue;

    QEvent::Type qtEvent = (axisValue >= SDL_TRIGGER_AXIS_VALUE_CENTER) ? QEvent::KeyPress : QEvent::KeyRelease;
    Qt::Key      qtKey;

    if (gamepadAxisAsButtonMapping.contains(axis) && gamepadAxisAsButtonMapping.at(axis).second != qtEvent) {
        qtKey = gamepadAxisAsButtonMapping.at(axis).first;
    } else {
        return;
    }

    gamepadAxisAsButtonMapping.at(axis).second = qtEvent;
    main.postEvent(main.focusObject(), new QKeyEvent(qtEvent, qtKey, Qt::NoModifier));
}
} // namespace SeekerStone::Components
