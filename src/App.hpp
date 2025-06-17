#pragma once

#include <QQmlApplicationEngine>

#include "components/GamepadAdapter.hpp"
#include "components/IdleManager.hpp"
#include "components/Link.hpp"
#include "components/Settings.hpp"

namespace SeekerStone {
class App : public QGuiApplication {
    public:
        explicit App(int argc, char* argv[]);
        ~App() = default;

    private:
        QQmlApplicationEngine      engine;
        Components::Settings       settings;
        Components::GamepadAdapter gamepadAdapter;
        Components::IdleManager    idleManager;
        Components::Link           link;
};
} // namespace SeekerStone
