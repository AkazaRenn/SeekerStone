#pragma once

#include <QDebug>
#include <QFile>
#include <QMessageLogger>
#include <QTextStream>
#include <mutex>

#include "utils/RateLimiter.hpp"

#define __FILENAME__ (std::filesystem::path(__FILE__).filename().string().c_str())

#ifdef QT_DEBUG
#define logDebug QMessageLogger(__FILENAME__, __LINE__, Q_FUNC_INFO).debug()
#else
#define logDebug \
    if (false)   \
    qDebug() // Prevents execution in release
#endif
#define logInfo    QMessageLogger(__FILENAME__, __LINE__, Q_FUNC_INFO).info()
#define logWarning QMessageLogger(__FILENAME__, __LINE__, Q_FUNC_INFO).warning()
#define logError   QMessageLogger(__FILENAME__, __LINE__, Q_FUNC_INFO).critical()

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
