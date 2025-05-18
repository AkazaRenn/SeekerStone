#pragma once

#include <QDebug>
#include <filesystem>

#define __FILENAME__ (std::filesystem::path(__FILE__).filename().string().c_str())

#ifdef QT_DEBUG
#define logDebug() QMessageLogger(__FILENAME__, __LINE__, Q_FUNC_INFO).debug()
#else
#define logDebug() \
    if (false)     \
    qDebug() // Prevents execution in release
#endif
#define logInfo()    QMessageLogger(__FILENAME__, __LINE__, Q_FUNC_INFO).info()
#define logWarning() QMessageLogger(__FILENAME__, __LINE__, Q_FUNC_INFO).warning()
#define logError()   QMessageLogger(__FILENAME__, __LINE__, Q_FUNC_INFO).critical()
