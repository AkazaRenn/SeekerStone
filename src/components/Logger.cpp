#include <QDir>
#include <QStandardPaths>
#include <iostream>

#include "../common/Macros.hpp"
#include "../common/Utils.hpp"
#include "Logger.hpp"

namespace {
constexpr uintmax_t        MAX_LOG_FILE_SIZE_BYTES = 10 * 1024 * 1024;
constexpr size_t           MAX_LOG_FILES_TO_KEEP   = 5;
constexpr std::string_view LOG_FILE_NAME_FORMAT    = APP_NAME " {:%Y-%m-%d %H:%M:%S}.log";
const auto&                LOGGER                  = SeekerStone::Components::Logger::instance();
} // namespace

namespace SeekerStone::Components {
Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::Logger()
    : logger(getLogDir(), LOG_FILE_NAME_FORMAT, MAX_LOG_FILE_SIZE_BYTES, MAX_LOG_FILES_TO_KEEP) {
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
    instance().logger << std::format("[{}][{}][{}:{}]{}", Common::nowString<std::chrono::nanoseconds>(),
                                     Common::toString(type), std::filesystem::path(context.file).filename().string(),
                                     context.line, msg.toStdString());
}
} // namespace SeekerStone::Components
