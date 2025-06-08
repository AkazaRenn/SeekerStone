#include <QDir>
#include <filesystem>

#include "Logger.hpp"

namespace {
const std::filesystem::path logDirPath = std::filesystem::absolute(
    std::filesystem::path(QDir::homePath().toStdString()) / ".local" / "share" / APP_NAME / "logs");
const qint64   maxLogFileSize           = 10 * 1024 * 1024;
const uint64_t logRotationCheckInterval = 100;
const uint8_t  maxLogFilesToKeep        = 5;
} // namespace

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::Logger()
    : logRotator(logRotationCheckInterval, std::bind(&Logger::rotateLogFile, this)) {
    openLogFile();
    qInstallMessageHandler(messageHandler);
}

Logger::~Logger() {
    qInstallMessageHandler(nullptr);
    std::lock_guard<std::mutex> lock(logTextStreamMutex);
    logFile.close();
}

void Logger::openLogFile() {
    std::string logFileName = std::string(APP_NAME) + ' ' +
                              QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString() + ".log";
    logFile.setFileName(logDirPath / logFileName);

    {
        std::lock_guard<std::mutex> lock(logTextStreamMutex);
        std::filesystem::create_directories(logDirPath);
        if (!logFile.isOpen()) {
            if (logFile.open(QIODevice::Append | QIODevice::Text)) {
                logTextStream.setDevice(&logFile);
            }
        }
    }
    
    // Remove old logs
    QDir logDir(logDirPath);
    logDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    logDir.setNameFilters(QStringList("*.log"));
    logDir.setSorting(QDir::Time);

    QFileInfoList logFiles = logDir.entryInfoList();

    if (logFiles.size() > maxLogFilesToKeep) {
        int filesToDelete = logFiles.size() - maxLogFilesToKeep;
        for (int i = logFiles.size() - 1; i >= maxLogFilesToKeep; i--) {
            logDir.remove(logFiles[i].fileName());
        }
    }
}

QString Logger::nanosecondsNow() {
    auto now         = std::chrono::system_clock::now();
    auto duration    = now.time_since_epoch();
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % 1'000'000'000;

    return QString::number(nanoseconds).rightJustified(9, '0');
}

void Logger::rotateLogFile() {
    if (logFile.size() > maxLogFileSize) {
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

    std::lock_guard<std::mutex> lock(logTextStreamMutex);
    if (logTextStream.device()) {
        logTextStream << "[" << QDateTime::currentDateTime().toString("hh:mm:ss") << '.' << nanosecondsNow() << "]"
                      << "[" << level << "]"
                      << "[" << std::filesystem::path(context.file).filename().string().c_str() << ":" << context.line
                      << "]" << msg << '\n';
        logTextStream.flush();
    }
}
