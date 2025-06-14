#pragma once

#include <QDebug>

namespace Common {
const std::string_view toString(QtMsgType type) {
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

template <typename Duration>
std::string nowString() {
    const auto now          = std::chrono::system_clock::now();
    const auto nowTruncated = std::chrono::time_point_cast<Duration>(now);
    const auto localTime    = std::chrono::zoned_time{std::chrono::current_zone(), nowTruncated};
    return std::format("{:%H:%M:%S}", localTime);
}
} // namespace Common
