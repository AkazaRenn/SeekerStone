#include <QDir>
#include <QStandardPaths>
#include <iostream>

#include "Logger.hpp"

namespace {
const qint64   MAX_LOG_FILE_SIZE           = 10 * 1024 * 1024;
const uint64_t LOG_RORATION_CHECK_INTERVAL = 100;
const uint8_t  MAX_LOG_FILES_TO_KEEP       = 5;
const Logger&  LOGGER                      = Logger::instance();
} // namespace

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::Logger()
    : logDir(getLogDir())
    , logRotator(LOG_RORATION_CHECK_INTERVAL, std::bind(&Logger::rotateLogFile, this)) {
    openLogFile();
    qInstallMessageHandler(messageHandler);
}

Logger::~Logger() {
    qInstallMessageHandler(nullptr);
    std::lock_guard<std::mutex> lock(logTextStreamMutex);
    logFile.close();
}

std::filesystem::path Logger::getLogDir() {
    std::filesystem::path statePath;

    const QString statePathStr = QStandardPaths::writableLocation(QStandardPaths::StateLocation);
    if (statePathStr.isEmpty()) {
        statePath = std::filesystem::path(QDir::homePath().toStdString()) / ".local" / "state";
        std::cout << "QStandardPaths::writableLocation(QStandardPaths::StateLocation) returned empty path" << std::endl;
    } else {
        statePath = statePathStr.toStdString();
    }

    std::filesystem::path logDirPath = statePath / APP_NAME / "logs";
    std::filesystem::create_directories(logDirPath);
    return logDirPath;
}

void Logger::openLogFile() {
    const std::string logFileName = std::string(APP_NAME) + ' ' +
                                    QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString() + ".log";
    logFile.setFileName(logDir / logFileName);

    {
        std::lock_guard<std::mutex> lock(logTextStreamMutex);
        if (!logFile.isOpen()) {
            if (logFile.open(QIODevice::Append | QIODevice::Text)) {
                logTextStream.setDevice(&logFile);
                std::cout << "Writing logs to: \"" << logFile.fileName().toStdString() << '\"' << std::endl;
            }
        }
    }

    // Remove old logs
    QDir qLogDir(logDir);
    qLogDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    qLogDir.setNameFilters(QStringList("*.log"));
    qLogDir.setSorting(QDir::Time);

    QFileInfoList logFiles = qLogDir.entryInfoList();

    if (logFiles.size() > MAX_LOG_FILES_TO_KEEP) {
        int filesToDelete = logFiles.size() - MAX_LOG_FILES_TO_KEEP;
        for (int i = logFiles.size() - 1; i >= MAX_LOG_FILES_TO_KEEP; i--) {
            qLogDir.remove(logFiles[i].fileName());
        }
    }
}

QString Logger::nanosecondsNow() {
    const auto now         = std::chrono::system_clock::now();
    const auto duration    = now.time_since_epoch();
    const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % 1'000'000'000;

    return QString::number(nanoseconds).rightJustified(9, '0');
}

void Logger::rotateLogFile() {
    if (logFile.size() > MAX_LOG_FILE_SIZE) {
        openLogFile();
    }
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    instance().writeLogToFile(type, context, msg);
}

void Logger::writeLogToFile(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    const char* level;
    switch (type) {
        case QtDebugMsg:
            level = "DBG";
            break;
        case QtInfoMsg:
            level = "INF";
            break;
        case QtWarningMsg:
            level = "WRN";
            break;
        case QtCriticalMsg:
            level = "ERR";
            break;
        default:
            level = "UKN";
            break;
    }

    {
        std::lock_guard<std::mutex> lock(logTextStreamMutex);
        if (logTextStream.device()) {
            logTextStream << "[" << QDateTime::currentDateTime().toString("hh:mm:ss") << '.' << nanosecondsNow() << "]"
                          << "[" << level << "]"
                          << "[" << std::filesystem::path(context.file).filename().string().c_str() << ":"
                          << context.line << "]" << msg << '\n';
            logTextStream.flush();
        }
    }

    logRotator.execute();
}
