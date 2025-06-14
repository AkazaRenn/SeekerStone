#pragma once

#include <filesystem>
#include <fstream>
#include <mutex>

#include "RateLimiter.hpp"

namespace Common {
class Logger {
    public:
        explicit Logger(const std::filesystem::path& _logDir, const std::string_view& _logFileNameFormat,
                        uintmax_t _maxLogFileSizeBytes, size_t _maxLogFilesToKeep);
        ~Logger();

        Logger& operator<<(const std::string& msg);

    private:
        const std::string_view&      logFileNameFormat;
        const std::filesystem::path& logDir;
        std::filesystem::path        logFile;
        std::ofstream                logTextStream;
        std::mutex                   logTextStreamMutex;
        RateLimiterByCount           logRotator;
        uintmax_t                    maxLogFileSizeBytes;
        size_t                       maxLogFilesToKeep;

        void openLogFile();
        void rotateLogFile();
        void removeOldLogFiles();
};
} // namespace Common
