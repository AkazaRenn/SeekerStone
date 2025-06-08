#pragma once

#include <QDebug>
#include <QFile>
#include <QMessageLogger>
#include <QTextStream>
#include <mutex>

#include "RateLimiter.hpp"

#ifdef QT_DEBUG
#define logDebug qDebug()
#else
#define logDebug \
    if (false)   \
    qDebug() // Prevents execution in release
#endif
#define logInfo    qInfo()
#define logWarning qWarning()
#define logError   qCritical()

class Logger {
    public:
        static Logger& instance();

    private:
        static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

        explicit Logger();
        ~Logger();

        QFile              logFile;
        QTextStream        logTextStream;
        std::mutex         logTextStreamMutex;
        RateLimiterByCount logRotator;

        void    openLogFile();
        QString nanosecondsNow();
        void    rotateLogFile();
        void    writeLogToFile(QtMsgType type, const QMessageLogContext& context, const QString& msg);
};
