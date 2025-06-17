#pragma once

#include <QQmlApplicationEngine>

#include "components/GamepadAdapter.hpp"
#include "components/IdleManager.hpp"
#include "components/Link.hpp"
#include "components/Settings.hpp"

namespace SeekerStone {
class Main : public QGuiApplication {
    public:
        explicit Main(int argc, char* argv[]);
        ~Main() = default;

    private:
        QQmlApplicationEngine      engine;
        Components::Settings       settings;
        Components::GamepadAdapter gamepadAdapter;
        Components::IdleManager    idleManager;
        Components::Link           link;
};
} // namespace SeekerStone
