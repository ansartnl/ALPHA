#ifndef DATABASESUBSCRIBER_H
#define DATABASESUBSCRIBER_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtSql/QSqlDatabase>

class DatabaseNotifier : public QObject
{
    friend class DatabaseSubscriber;
    Q_OBJECT
    Q_DISABLE_COPY(DatabaseNotifier)

    DatabaseNotifier(QObject *parent = 0) : QObject(parent) {}
    ~DatabaseNotifier() {}

    void emitNotification() {emit notification();}

signals:
    void notification();
};


class DatabaseSubscriber : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DatabaseSubscriber)

    DatabaseSubscriber(const QString &connectionName): mConnectionName(connectionName) {}
    ~DatabaseSubscriber() {}

public:
    static DatabaseSubscriber &instance(const QString &connectionName = QSqlDatabase::defaultConnection);

    DatabaseNotifier *subscribe(const QString &name);
    void unsubscribe(const QString &name);

public slots:
    void onDatabaseConnected();

private slots:
    void onNotification(const QString &name);

private:
    QString mConnectionName;
    QMap<QString, DatabaseNotifier*> mSubscribers;
};

#endif // DATABASESUBSCRIBER_H
