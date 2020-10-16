#ifndef QTDBAUTOCONNECTOR_H
#define QTDBAUTOCONNECTOR_H

#include <QtCore/QMutex>
#include <QtCore/QThread>

#include <QtSql/QSqlDatabase>

struct DbParams
{
    QString driver;
    QString host;
    int port;
    QString databaseName;
    QString user;
    QString password;

    DbParams() :
        port(0)
    {}

    DbParams(const QString &dr, const QString &h, const int p
             , const QString &dn, const QString &u, const QString &pw) :
        driver(dr), host(h), port(p), databaseName(dn), user(u), password(pw)
    {}
};

typedef QList < DbParams > DbParamsList;

class QTimer;

/*!
  * \class QtDbAutoConnector
  * \brief Автоматическое переподключение бд
  *
  * Класс предназначен для автоматического подключения отключившейся базы данных
  * через заданный промежуток времени с заданными параметрами
  */

class QtDbAutoConnector : public QThread
{
    Q_OBJECT
public:
    explicit QtDbAutoConnector(QObject *parent = 0);
    virtual ~QtDbAutoConnector();

    void addDbParams(const DbParams &p);
    void clearDbParams();
    const DbParamsList dbParams() const;

    void setDbAlias(const QString &alias);
    void setInteraval(int msec);

    bool isConnected() const;

    const DbParams currentParams() const;

public slots:
    void start();
    void start(const QString &dbAlias);
    void reconnect();
    void stop();

signals:
    void connected();
    void connected(int index);
    void connected(const DbParams &p);
    void disconnected();
    void error(const QString &err);

protected:
    bool fillDb();
    void removeDb();

    void setActive(bool active);

protected slots:
    void onTimer();
    void onConnected();

protected:
    QTimer *timer;

    DbParamsList mDbParams;
    int mCurrentIndex;

    QString mAlias;
    QSqlDatabase db;

    bool isActive;
    mutable QMutex mutex;
};

#endif // QTDBAUTOCONNECTOR_H
