#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Main.hpp"

int Main::run(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.loadFromModule("SeekerStone", "Main");

    return app.exec();
}

int main(int argc, char* argv[]) {
    return Main().run(argc, argv);
}
