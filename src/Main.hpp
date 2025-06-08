#pragma once

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "GamepadAdapter.hpp"
#include "IdleManager.hpp"
#include "Link.hpp"

class Main : public QGuiApplication {
    public:
        explicit Main(int argc, char* argv[]);
        ~Main();

    private:
        QQmlApplicationEngine engine;
        GamepadAdapter        gamepadAdapter;
        IdleManager           idleManager;
        Link                  link;
};
