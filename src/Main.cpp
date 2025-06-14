#include <QQmlContext>

#include "Main.hpp"
#include "components/Logger.hpp"
#include "common/Macros.hpp"

namespace SeekerStone {
Main::Main(int argc, char* argv[])
    : QGuiApplication(argc, argv) {
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, this,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    // Expose backend to QML
    engine.rootContext()->setContextProperty("link", &link);

    engine.loadFromModule(APP_NAME, "Main");

    installEventFilter(&idleManager);

    logInfo << "Initialized";
}

Main::~Main() {
    removeEventFilter(&idleManager);
    logInfo << "Exited";
}
} // namespace SeekerStone

int main(int argc, char* argv[]) {
    return SeekerStone::Main(argc, argv).exec();
}
