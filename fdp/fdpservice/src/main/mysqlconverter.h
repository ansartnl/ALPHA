/*
 * MysqlConverter.h
 *
 *  Created on: Oct 27, 2011
 *      Author: filonenko
 */
#ifndef MYSQLCONVERTER_H_
#define MYSQLCONVERTER_H_

#include "aftnworkerbase.h"

#include <QtCore/QDateTime>
#include <QtSql/QSqlDatabase>

class DbConnectionKeeper;


class MysqlConverter : public AFTNWorkerBase
{
    Q_OBJECT
public:
    explicit MysqlConverter(QObject* parent = 0);
    virtual void run();
};


class MysqlConverterBase: public WorkerBase
{
    Q_OBJECT
public:
    MysqlConverterBase(QObject* parent = 0);

    bool start();
    void stop();

    bool isDisabled() const { return disable; }

protected:
    void loadSettings();
    void initDefaultSettings();
    QStringList isValidRecord(const QSqlRecord &mysqlrec);

    virtual void timerEvent(QTimerEvent *event);

private slots:
    void onMysqlReconnected();
    void onMysqlDisconnected();

private:
    int timerId;
    int interval;

    DbConnectionKeeper *mMysqlConnKeeper;

    bool disable;

    QSqlDatabase mysqldb;
    QSqlDatabase oradb;

    QList<int> listOfErrorId;
};

#endif /* MYSQLCONVERTER_H_ */
