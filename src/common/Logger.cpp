#include <format>
#include <iostream>

#include "Logger.hpp"

namespace {
const uint64_t LOG_RORATION_CHECK_INTERVAL = 20;
} // namespace

namespace Common {
Logger::Logger(const std::filesystem::path& _logDir, const std::string_view& _logFileNameFormat,
               uintmax_t _maxLogFileSizeBytes, size_t _maxLogFilesToKeep)
    : logFileNameFormat(_logFileNameFormat)
    , logDir(_logDir)
    , logRotator(LOG_RORATION_CHECK_INTERVAL, std::bind(&Logger::rotateLogFile, this))
    , maxLogFileSizeBytes(_maxLogFileSizeBytes)
    , maxLogFilesToKeep(_maxLogFilesToKeep) {
    openLogFile();
}

Logger::~Logger() {
    std::lock_guard<std::mutex> lock(logTextStreamMutex);
    if (logTextStream.is_open()) {
        logTextStream.flush();
    }
}

Logger& Logger::operator<<(const std::string& msg) {
    {
        std::lock_guard<std::mutex> lock(logTextStreamMutex);
        if (logTextStream.is_open()) {
            logTextStream << msg << std::endl;
        } else {
            std::cerr << "Log file not open" << std::endl;
        }
    }

    logRotator.execute();
    return *this;
}

void Logger::openLogFile() {
    const auto                    now        = std::chrono::system_clock::now();
    const auto                    nowSeconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
    const std::chrono::zoned_time localTime{std::chrono::current_zone(), nowSeconds};
    // TODO: Only available in C++ 26
    // const std::string             logFileName = std::format(std::runtime_format(logFileNameFormat), localTime);
    const std::string             logFileName = std::format("SeekerStone {:%Y-%m-%d %H:%M:%S}.log", localTime);

    logFile = logDir / logFileName;

    {
        std::lock_guard<std::mutex> lock(logTextStreamMutex);
        logTextStream.open(logFile, std::ios::out | std::ios::app);
        if (logTextStream.is_open()) {
            std::cout << "Log file path: " << logFile << std::endl;
            removeOldLogFiles();
        } else {
            std::cerr << "Failed to open log file: " << logFile << std::endl;
        }
    }
}

void Logger::removeOldLogFiles() {
    if (!std::filesystem::exists(logDir)) {
        std::cerr << "Error: Log directory '" << logDir << "' does not exist" << std::endl;
        return;
    }
    if (!std::filesystem::is_directory(logDir)) {
        std::cerr << "Error: '" << logDir << "' is not a directory" << std::endl;
        return;
    }

    std::vector<std::filesystem::directory_entry> logFiles;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(logDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".log") {
                logFiles.push_back(entry);
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error iterating directory '" << logDir << "': " << e.what() << std::endl;
        return;
    }

    if (logFiles.size() <= maxLogFilesToKeep) {
        return;
    }

    std::sort(logFiles.begin(), logFiles.end(), std::greater<>());

    if (logFiles.size() > maxLogFilesToKeep) {
        for (std::size_t i = maxLogFilesToKeep; i < logFiles.size(); i++) {
            std::filesystem::remove(logFiles[i]);
        }
    }
}

void Logger::rotateLogFile() {
    if (std::filesystem::file_size(logFile) >= maxLogFileSizeBytes) {
        openLogFile();
    }
}
} // namespace Common
