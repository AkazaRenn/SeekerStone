#include <QTimer>

#include "../utils/Logger.hpp"
#include "Link.hpp"

Link::Link(QObject* parent)
    : QObject(parent) {
}

void Link::start() {
    logDebug << "Link start";

    QTimer::singleShot(2000, this, [&]() {
        emit finished(0);
    });
}
