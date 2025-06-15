#include <QQmlContext>

#include "Main.hpp"
#include "common/Macros.hpp"
#include "components/Logger.hpp"

namespace SeekerStone {
Main::Main(int argc, char* argv[])
    : QGuiApplication(argc, argv)
    , idleManager(*this) {
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, this,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    initialize();
}

Main::~Main() {
    removeEventFilter(&idleManager);
    logInfo << "Exiting";
}

void Main::initialize() {
    // Expose backend to QML
    engine.rootContext()->setContextProperty("link", &link);

    engine.loadFromModule(APP_NAME, "Main");

    logInfo << "Initialized";
}
} // namespace SeekerStone

int main(int argc, char* argv[]) {
    return SeekerStone::Main(argc, argv).exec();
}
