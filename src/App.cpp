#include <QQmlContext>

#include "App.hpp"
#include "common/Macros.hpp"
#include "components/Logger.hpp"

namespace SeekerStone {
App::App(int argc, char* argv[])
    : QGuiApplication(argc, argv)
    , gamepadAdapter(*this)
    , idleManager(*this) {
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, this,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    // Expose backend to QML
    engine.rootContext()->setContextProperty("link", &link);

    engine.loadFromModule(APP_NAME, "Main");

    logInfo << "Initialized";
}
} // namespace SeekerStone
