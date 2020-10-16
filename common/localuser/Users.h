#ifndef USERS_H
#define USERS_H

#include <QtCore/QCryptographicHash>
#include <QtCore/QVariantHash>

// key - username
extern QVariantHash users;
extern QVariantHash currentUser;
extern QString currentUserLogin;
extern QStringList groups;
inline QString passwordHash(QString password)
{
    password.append("SOMESALTFROMFILONENKO\xAB\xCD\xEF");
    return QCryptographicHash::hash(password.toUtf8().data(), QCryptographicHash::Md5).toHex();
}

#ifdef LOG4QT_VERSION
#include "Log.h"

inline Log4Qt::LogStream qUserLogDebug() { return Log4Qt::Logger::rootLogger()->debug()
            << "\"" << currentUserLogin << "\" "; }
inline Log4Qt::LogStream qUserLogInfo() { return Log4Qt::Logger::rootLogger()->info()
            << "\"" << currentUserLogin << "\" "; }
inline Log4Qt::LogStream qUserLogWarn() { return Log4Qt::Logger::rootLogger()->warn()
            << "\"" << currentUserLogin << "\" "; }
inline Log4Qt::LogStream qUserLogError() { return Log4Qt::Logger::rootLogger()->error()
            << "\"" << currentUserLogin << "\" "; }
inline Log4Qt::LogStream qUserLogDebug(const QString &name) {
    return Log4Qt::Logger::logger(name)->debug() << "\"" << currentUserLogin << "\" "; }
inline Log4Qt::LogStream qUserLogInfo(const QString &name) {
    return Log4Qt::Logger::logger(name)->info() << "\"" << currentUserLogin << "\" "; }
inline Log4Qt::LogStream qUserLogWarn(const QString &name) {
    return Log4Qt::Logger::logger(name)->warn() << "\"" << currentUserLogin << "\" "; }
inline Log4Qt::LogStream qUserLogError(const QString &name) {
    return Log4Qt::Logger::logger(name)->error() << "\"" << currentUserLogin << "\" "; }
#else
#include <QtDebug>

inline QDebug qUserLogInfo() { return qDebug(); }
inline QDebug qUserLogError() { return qDebug(); }
inline QDebug qUserLogInfo(const QString &name) { return (qDebug() << "[" << name << "]"); }
inline QDebug qUserLogError(const QString &name) { return (qDebug() << "[" << name << "]"); }
#endif

#endif // USERS_H
