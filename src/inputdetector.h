#pragma once

#include <QEvent>
#include <QObject>
#include <SDL3/SDL.h>

class InputDetector : public QObject {
        Q_OBJECT
    public:
        explicit InputDetector(QObject *parent = nullptr);
        ~InputDetector();

    protected:
        // Event filter to catch all input events
        bool eventFilter(QObject *watched, QEvent *event) override;

    private:
        std::unique_ptr<SDL_Thread> sdlEventThread;

        void onGamepadConnected();
        void resetIdleTimer();
        static int sdlEventHandler(void *data);
};
