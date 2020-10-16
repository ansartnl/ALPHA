#ifndef USERSIGNALHELPER_H
#define USERSIGNALHELPER_H

#ifndef LOG4QT_LINKED_JUDICIOUSLY
#include "logger.h"
#else
#include <Log4Qt/logger.h>
#endif

#include <QtCore/QObject>

// helper for emitting signal

/*
 * Usage:
 * QObject::connect(UserSignalHelper::instance(&app), SIGNAL(userChanged(QString))
 * , dbLayout, SLOT(userChanged(QString)));
 * ...
 * UserSignalHelper::changeUser(userName);
 */

class UserSignalHelper : public QObject {
    Q_OBJECT
public:
    static UserSignalHelper * instance(QObject *parent = 0);

    virtual ~UserSignalHelper();

    static void changeUser(const QString &userName);

signals:
    void userChanged(const QString &userName);

protected:
    void p_changeUser(const QString &userName);

private:
    UserSignalHelper(QObject *parent = 0);
};

// функции, автоматически добавляющие имя пльзователя

inline Log4Qt::LogStream qLogDebug(const QString &name, const QString &userName) {
    UserSignalHelper::changeUser(userName);
    return Log4Qt::Logger::logger(name)->debug();
}

inline Log4Qt::LogStream qLogInfo(const QString &name, const QString &userName) {
    UserSignalHelper::changeUser(userName);
    return Log4Qt::Logger::logger(name)->info();
}

inline Log4Qt::LogStream qLogWarn(const QString &name, const QString &userName) {
    UserSignalHelper::changeUser(userName);
    return Log4Qt::Logger::logger(name)->warn();
}

inline Log4Qt::LogStream qLogError(const QString &name, const QString &userName) {
    UserSignalHelper::changeUser(userName);
    return Log4Qt::Logger::logger(name)->error();
}

#endif // USERSIGNALHELPER_H
