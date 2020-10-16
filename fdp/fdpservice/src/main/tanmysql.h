#ifndef TANMYSQL_H_
#define TANMYSQL_H_

#include "aftnworkerbase.h"

#include <QtCore/QDateTime>
#include <QtSql/QSqlDatabase>

#include "fpl/fpltan.h"

class DbConnectionKeeper;


class TANMySql : public AFTNWorkerBase
{
    Q_OBJECT
public:
    explicit TANMySql(QObject* parent = 0);
    virtual void run();
};


class TANMySqlBase: public WorkerBase
{
    Q_OBJECT

    static const QString TBL_MSG_FROM_TAN;
    static const QString TBL_MSG_FROM_FDP;

public:
    TANMySqlBase(QObject* parent = 0);

    bool start();
    void stop();

    bool isDisabled() const { return disable; }

protected:
    void loadSettings();
    void initDefaultSettings();

    virtual void timerEvent(QTimerEvent *event);

private slots:
    void onMysqlReconnected();
    void onMysqlDisconnected();

private:
    void readTAN();
    void readFDP();
    void writeTAN();
    void writeFDP();

    void checkIDinFDP(FPL& Fpl);
    void deleteFromFDP(FPL& Fpl);

    void setServerState(bool bMain);

    bool beginQuery(const QSqlDatabase &base = QSqlDatabase::database());
    bool endQuery(const QSqlDatabase &base = QSqlDatabase::database());

private:
    int timerId;
    int interval;

    DbConnectionKeeper *mMysqlConnKeeper;

    bool disable;

    QList<int> listOfErrorId;

    typedef QList<FPLfromTAN> TTANList;
    typedef QList<FPLfromFDP> TFDPList;
    TTANList  mListFromTAN;
    TFDPList  mListFromFDP;
};

#endif /* TANMYSQL_H_ */
