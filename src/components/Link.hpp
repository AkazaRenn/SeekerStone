#pragma once

#include <QObject>

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