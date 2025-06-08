#pragma once

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "GamepadAdapter.hpp"
#include "IdleManager.hpp"
#include "Link.hpp"
#include "Logger.hpp"

class Main : public QGuiApplication {
    public:
        explicit Main(int argc, char* argv[]);
        ~Main();

    private:
        const Logger& logger;

        QQmlApplicationEngine engine;
        GamepadAdapter        gamepadAdapter;
        IdleManager           idleManager;
        Link                  link;
};
