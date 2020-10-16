#ifndef QTAUTOTRANSACTION_H
#define QTAUTOTRANSACTION_H

#include <QtSql/QSqlDatabase>

/*!
  * \class QtAutoTransaction
  * \brief Автоматическа транзакция
  *
  * Данный класс упрощает использование транзакций баз данных.
  */

class QtAutoTransaction
{
public:
    QtAutoTransaction(const QSqlDatabase &db = QSqlDatabase::database());
    virtual ~QtAutoTransaction();
    virtual bool commit();
    inline bool isTransactionStarted() const { return needCommit; }

private:
    QSqlDatabase db;
    bool needCommit;
};

#endif // QTAUTOTRANSACTION_H
