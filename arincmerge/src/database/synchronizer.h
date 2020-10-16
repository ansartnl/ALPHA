#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include <QDataStream>
#include <QtCore/QMap>
#include <QtCore/QMetaType>
#include <QtCore/QObject>

#include <QtSql/QSqlDatabase>

const QString randName();

struct ServerInfo
{
    QString name;

    QString driver;
    QString host;
    quint16 port;
    QString databaseName;
    QString user;
    QString password;

    QString connectionName;

    QSqlDatabase db;

    ServerInfo() :
        port(0)
      , connectionName(randName())
    {}
    ServerInfo(const QString &name_, const QString &driver_, const QString &host_, quint16 port_
               , const QString &databaseName_, const QString &user_, const QString &password_) :
        name(name_)
      , driver(driver_)
      , host(host_)
      , port(port_)
      , databaseName(databaseName_)
      , user(user_)
      , password(password_)
      , connectionName(randName())
    {
    }

    inline const QString identifier() const
    {
        return connectionName;
    }

    inline const QString humanReadable() const
    {
        return QString("%1@%2:%3/%4").arg(user).arg(host).arg(port).arg(databaseName);
    }
};
Q_DECLARE_METATYPE(ServerInfo)
inline QDataStream & operator <<(QDataStream &stream, const ServerInfo &msg)
{
    stream << msg.name;
    stream << msg.driver;
    stream << msg.host;
    stream << msg.port;
    stream << msg.databaseName;
    stream << msg.user;
    stream << msg.password;
    stream << msg.connectionName;
    return stream;
}
inline QDataStream & operator >>(QDataStream &stream, ServerInfo &msg)
{
    stream >> msg.name;
    stream >> msg.driver;
    stream >> msg.host;
    stream >> msg.port;
    stream >> msg.databaseName;
    stream >> msg.user;
    stream >> msg.password;
    stream >> msg.connectionName;
    return stream;
}

class Synchronizer : public QObject
{
    Q_OBJECT
public:
    explicit Synchronizer(QObject *parent = 0, const QSqlDatabase &db_ = QSqlDatabase());
    ~Synchronizer();

public slots:
    void addServer(const ServerInfo &info);
    void synchronize(const QString &id);

signals:
    void changeLastSynchronization(const QString &id, const QString &lastSynchronization);
    void changeStatus(const QString &id, const QString &status);

private:
    const QStringList allTables(const QSqlDatabase &sdb);
    bool disableTriggers(const QSqlDatabase &sdb, const QString &tableName);
    bool enableTriggers(const QSqlDatabase &sdb, const QString &tableName);
    bool clearTable(const QSqlDatabase &sdb, const QString &tableName);
    bool copyTable(const QSqlDatabase &sdb, const QString &tableName, const QStringList &fields);
    bool updateLastSynchronization(const QSqlDatabase &sdb, const QDateTime &time);

    bool execute(const QSqlDatabase &sdb, const QString &queryString);

    void emitError(const QString &id, int counter, int total);
    void emitError(const QString &id);

private slots:
    void onError(const QSqlQuery &query);
    void onError(const QString &text);

private:
    static const QString kConnectionName;

    QSqlDatabase db;
    QMap < QString, ServerInfo > mServers;
};

#endif // SYNCHRONIZER_H
