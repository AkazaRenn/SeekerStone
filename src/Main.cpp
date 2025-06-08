#include <QQmlContext>

#include "Main.hpp"
#include "utils/Logger.hpp"

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

int main(int argc, char* argv[]) {
    return Main(argc, argv).exec();
}
