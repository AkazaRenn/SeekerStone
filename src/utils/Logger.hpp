#pragma once

#include <QDebug>
#include <QFile>
#include <QMessageLogger>
#include <QTextStream>
#include <filesystem>
#include <mutex>

#include "RateLimiter.hpp"

#ifdef QT_DEBUG
#define logDebug logInfo
#else
#define QT_NO_DEBUG_OUTPUT
#define logDebug qDebug()
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

        QFile                       logFile;
        const std::filesystem::path logDir;
        QTextStream                 logTextStream;
        std::mutex                  logTextStreamMutex;
        RateLimiterByCount          logRotator;

        void                  openLogFile();
        std::filesystem::path getLogDir();
        QString               nanosecondsNow();
        void                  rotateLogFile();
        void                  writeLogToFile(QtMsgType type, const QMessageLogContext& context, const QString& msg);
};
