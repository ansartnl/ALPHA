#ifndef LOG_H
#define LOG_H

#ifndef LOG4QT_LINKED_JUDICIOUSLY
#include "logger.h"
#else
#include <Log4Qt/logger.h>
#endif

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

/*!
  * \file Log.h
  * \brief Набор встроенных функций для облегчения работы с Log4Qt
  */

inline Log4Qt::Logger * qRootLogger() {
    return Log4Qt::Logger::rootLogger();
}

inline Log4Qt::Logger * qLogger(const QString &name) {
    return Log4Qt::Logger::logger(name);
}

inline Log4Qt::LogStream qLogTrace() {
    return Log4Qt::Logger::rootLogger()->trace();
}

inline Log4Qt::LogStream qLogDebug() {
    return Log4Qt::Logger::rootLogger()->debug();
}

inline Log4Qt::LogStream qLogInfo() {
    return Log4Qt::Logger::rootLogger()->info();
}

inline Log4Qt::LogStream qLogWarn() {
    return Log4Qt::Logger::rootLogger()->warn();
}

inline Log4Qt::LogStream qLogError() {
    return Log4Qt::Logger::rootLogger()->error();
}

inline Log4Qt::LogStream qLogDebug(const QString &name) {
    return Log4Qt::Logger::logger(name)->debug();
}

inline Log4Qt::LogStream qLogInfo(const QString &name) {
    return Log4Qt::Logger::logger(name)->info();
}

inline Log4Qt::LogStream qLogWarn(const QString &name) {
    return Log4Qt::Logger::logger(name)->warn();
}

inline Log4Qt::LogStream qLogError(const QString &name) {
    return Log4Qt::Logger::logger(name)->error();
}


inline QString dbgAddressPort(const QHostAddress &a, quint16 b) {
    return QString("'%1:%2'").arg(a.toString()).arg(b);
}

inline QString dbgAddressPort(const QTcpSocket *s) {
    return dbgAddressPort(s->peerAddress(), s->peerPort());
}

inline QString dbgAddressPort(const QTcpServer *s) {
    return dbgAddressPort(s->serverAddress(), s->serverPort());
}

#endif // LOG_H
