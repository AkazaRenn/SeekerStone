#pragma once

#include <QObject>

namespace SeekerStone::Components {
class Link : public QObject {
        Q_OBJECT

    public:
        explicit Link(QObject* parent = nullptr);
        ~Link() = default;

    public slots:
        void start();

    signals:
        void finished(int exitCode);
};
} // namespace SeekerStone::Components
