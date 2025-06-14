#pragma once

#include <QDebug>

#include "../common/Logger.hpp"

#ifdef QT_DEBUG
#define logDebug logInfo
#else
#define QT_NO_DEBUG_OUTPUT
#define logDebug qDebug()
#endif
#define logInfo    qInfo()
#define logWarning qWarning()
#define logError   qCritical()

namespace SeekerStone::Components {
class Logger {
    public:
        static Logger& instance();

    private:
        static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

        Common::Logger logger;

        explicit Logger();
        ~Logger();

        std::filesystem::path getLogDir();
};
} // namespace SeekerStone::Components
