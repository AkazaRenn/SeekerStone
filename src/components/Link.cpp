#include <QTimer>

#include "Logger.hpp"
#include "Link.hpp"

namespace SeekerStone::Components {
Link::Link(QObject* parent)
    : QObject(parent) {
}

void Link::start() {
    logDebug << "Link start";

    QTimer::singleShot(2000, this, [&]() {
        emit finished(0);
    });
}
} // namespace SeekerStone::Components
