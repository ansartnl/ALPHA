#include <QtSql/QSqlDriver>

#include "databasesubscriber.h"

DatabaseSubscriber &DatabaseSubscriber::instance(const QString &connectionName)
{
    static DatabaseSubscriber object(connectionName);
    return object;
}

DatabaseNotifier *DatabaseSubscriber::subscribe(const QString &name)
{
    if (mSubscribers.contains(name))
        return mSubscribers.value(name);

    DatabaseNotifier *notifier = new DatabaseNotifier(this);
    mSubscribers.insert(name, notifier);

    QSqlDatabase::database(mConnectionName).driver()->subscribeToNotification(name);
    return notifier;
}

void DatabaseSubscriber::unsubscribe(const QString &name)
{
    if (mSubscribers.contains(name))
    {
        DatabaseNotifier *notifier = mSubscribers.take(name);
        notifier->deleteLater();

        QSqlDatabase::database(mConnectionName).driver()->unsubscribeFromNotification(name);
    }
}

void DatabaseSubscriber::onDatabaseConnected()
{
    QSqlDriver *driver = QSqlDatabase::database(mConnectionName).driver();
    connect(driver, SIGNAL(notification(QString)), SLOT(onNotification(QString)));

    foreach (const QString &name, mSubscribers.keys())
    {
        driver->subscribeToNotification(name);
        mSubscribers.value(name)->emitNotification();
    }
}

void DatabaseSubscriber::onNotification(const QString &name)
{
    DatabaseNotifier *notifier = mSubscribers.value(name, 0);

    if (notifier)
    {
        notifier->emitNotification();
    }
}


