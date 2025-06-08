#include <QQmlContext>

#include "Main.hpp"

Main::Main(int argc, char* argv[])
    : QGuiApplication(argc, argv)
    , logger(Logger::instance()) {
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, this,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    // Expose backend to QML
    engine.rootContext()->setContextProperty("link", &link);

    engine.loadFromModule("SeekerStone", "Main");

    installEventFilter(&idleManager);
}

Main::~Main() {
    removeEventFilter(&idleManager);
    logInfo << "Goodbye";
}

int main(int argc, char* argv[]) {
    return Main(argc, argv).exec();
}
