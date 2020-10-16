#ifndef QTDATABASECONNECTOR_H
#define QTDATABASECONNECTOR_H

#include <QtCore/QThread>
#include <QtSql/QSqlDatabase>

/*!
  * \class QtDatabaseConnector
  * \brief Класс подключения к базе данных
  *
  * Данный класс пытается подключиться к базе данных.
  */

class QtDatabaseConnector : public QThread
{
    Q_OBJECT
public:
    explicit QtDatabaseConnector(const QString &connectionName, QObject *parent = 0);
    virtual ~QtDatabaseConnector();

    QSqlDatabase db;

    QString driverName;
    QString databaseName;
    QString login;
    QString password;
    QString host;
    int port;

    void clear();

protected:
    virtual void run();

private:
    QString dbConnectionName;
};

#endif // QTDATABASECONNECTOR_H
