#pragma once

#include <QQmlApplicationEngine>

#include "components/GamepadAdapter.hpp"
#include "components/IdleManager.hpp"
#include "components/Link.hpp"

namespace SeekerStone {
class Main : public QGuiApplication {
    public:
        explicit Main(int argc, char* argv[]);
        ~Main() = default;

    private:
        QQmlApplicationEngine      engine;
        Components::GamepadAdapter gamepadAdapter;
        Components::IdleManager    idleManager;
        Components::Link           link;
};
} // namespace SeekerStone
