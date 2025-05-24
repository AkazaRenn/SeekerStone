#pragma once

#include <QObject>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>

class GamepadAdapter : public QObject {
        Q_OBJECT
    public:
        explicit GamepadAdapter(QObject* parent = nullptr);
        ~GamepadAdapter();

    private:
        std::unique_ptr<SDL_Thread, void (*)(SDL_Thread*)> sdlEventThread;

        std::unordered_map<SDL_GamepadButton, Qt::Key> gamepadButtonMapping = {
            {SDL_GamepadButton::SDL_GAMEPAD_BUTTON_DPAD_UP,        Qt::Key::Key_Up       },
            {SDL_GamepadButton::SDL_GAMEPAD_BUTTON_DPAD_DOWN,      Qt::Key::Key_Down     },
            {SDL_GamepadButton::SDL_GAMEPAD_BUTTON_DPAD_LEFT,      Qt::Key::Key_Left     },
            {SDL_GamepadButton::SDL_GAMEPAD_BUTTON_DPAD_RIGHT,     Qt::Key::Key_Right    },
            {SDL_GamepadButton::SDL_GAMEPAD_BUTTON_SOUTH,          Qt::Key::Key_Return   },
            {SDL_GamepadButton::SDL_GAMEPAD_BUTTON_EAST,           Qt::Key::Key_Escape   },
            {SDL_GamepadButton::SDL_GAMEPAD_BUTTON_WEST,           Qt::Key::Key_Backspace},
            {SDL_GamepadButton::SDL_GAMEPAD_BUTTON_NORTH,          Qt::Key::Key_Space    },
            {SDL_GamepadButton::SDL_GAMEPAD_BUTTON_LEFT_SHOULDER,  Qt::Key::Key_Home     },
            {SDL_GamepadButton::SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, Qt::Key::Key_End      },
        };
        std::unordered_map<SDL_GamepadAxis, std::pair<Qt::Key, QEvent::Type>> gamepadAxisAsButtonMapping = {
            {SDL_GamepadAxis::SDL_GAMEPAD_AXIS_LEFT_TRIGGER,  {Qt::Key::Key_Shift, QEvent::KeyRelease}},
            {SDL_GamepadAxis::SDL_GAMEPAD_AXIS_RIGHT_TRIGGER, {Qt::Key::Key_Enter, QEvent::KeyRelease}},
        };

        void onGamepadButton(bool buttonDown, SDL_GamepadButton button);
        void onGamepadAxis(Sint16 value, SDL_GamepadAxis axis);

        static int  sdlEventHandler(void* data);
        static void sdlThreadCleanup(SDL_Thread* thread);

    signals:
        void signalGamepadAction();
        void signalGamepadConnected();
};
