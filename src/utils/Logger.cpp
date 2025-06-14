#include <QDir>
#include <QStandardPaths>
#include <iostream>

#include "Logger.hpp"
#include "Macros.hpp"

namespace {
const uintmax_t            MAX_LOG_FILE_SIZE_BYTES = 10 * 1024 * 1024;
const size_t               MAX_LOG_FILES_TO_KEEP   = 5;
const SeekerStone::Logger& LOGGER                  = SeekerStone::Logger::instance();
} // namespace

namespace SeekerStone {
Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::Logger()
    : logger(getLogDir(), "'SeekerStone' yyyy-MM-dd hh:mm:ss'.log'", MAX_LOG_FILE_SIZE_BYTES, MAX_LOG_FILES_TO_KEEP) {
    qInstallMessageHandler(messageHandler);
}

Logger::~Logger() {
    qInstallMessageHandler(nullptr);
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

void Logger::messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    instance().logger << std::format("[{}][{}][{}:{}]{}", nowString(), toString(type),
                                     std::filesystem::path(context.file).filename().string(), context.line,
                                     msg.toStdString());
}
} // namespace SeekerStone

const char* toString(QtMsgType type) {
    switch (type) {
        case QtDebugMsg:
            return "DBG";
        case QtInfoMsg:
            return "INF";
        case QtWarningMsg:
            return "WRN";
        case QtCriticalMsg:
            return "ERR";
        default:
            return "UKN";
    }
}

std::string nowString() {
    const auto now       = std::chrono::system_clock::now();
    const auto localTime = std::chrono::zoned_time{std::chrono::current_zone(), now};
    return std::format("{:%H:%M:%S}", localTime);
}
