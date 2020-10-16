#include "qtautotransaction.h"
#include <QtCore/QThread>
#include <QSqlDriver>
#include <QDebug>

namespace qtautotrans {
struct Sleep : public QThread { using QThread::msleep;
                                using QThread::sleep; };
}

QtAutoTransaction::QtAutoTransaction(const QSqlDatabase &database) :
    db(database)
{
    needCommit = db.transaction();
    if(!needCommit)
    {
        qDebug() << "QtAutoTransaction circle connection start";
        QSqlDriver* drv = db.driver();
        if(drv && drv->hasFeature(QSqlDriver::Transactions))
        {
            int i = 0;
            while(!needCommit && (i < 4))
            {
                qtautotrans::Sleep::msleep(i*i*i*10 + 1);
                ++i;
                needCommit = db.transaction();
            }
            if(needCommit)
                qDebug() << "QtAutoTransaction circle connection success at i =" << i-1;

            else
                qDebug() << "QtAutoTransaction circle connection failed";
        }
        qDebug() << "QtAutoTransaction circle connection finish";
    }
}

QtAutoTransaction::~QtAutoTransaction()
{
    if (needCommit)
        db.rollback();
    db = QSqlDatabase();
}

bool QtAutoTransaction::commit()
{
    if (needCommit)
    {
        needCommit = false;
        return db.commit();
    }
    return true;
}
