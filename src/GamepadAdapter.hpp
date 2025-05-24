#pragma once

#include <QObject>
#include <SDL3/SDL.h>

class GamepadAdapter : public QObject {
        Q_OBJECT
    public:
        explicit GamepadAdapter(QObject* parent = nullptr);
        ~GamepadAdapter();

    private:
        std::unique_ptr<SDL_Thread, void (*)(SDL_Thread*)> sdlEventThread;

        void onGamepadButton(bool buttonDown, Uint8 button);

        static int  sdlEventHandler(void* data);
        static void sdlThreadCleanup(SDL_Thread* thread);

    signals:
        void signalGamepadAction();
        void signalGamepadConnected();
};
