#ifndef QOCIQUERY_H
#define QOCIQUERY_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtCore/QVariant>

//! Base query for database
class QOciQuery: public QSqlQuery
{
  public:
    //! Constructor
    /*!
     \param DB reference low-level database structure
     */
    QOciQuery(const QSqlDatabase& database = QSqlDatabase::database());
    //! Create new query
    //! prepare SQL statement
    /*!
     \param sStatement SQL statement for preparation
     \param sReturn if statement is INSERT, than name of a primary key
     */
    virtual void prepare(const QString& sStatement, const QString& sReturn =
        QString::null);
    //! Get the identifier of a primary key after execution of an INSERT statement
    virtual QVariant GetLastInsertID();
  private:
    QVariant tempValue;
};

#endif // QOCIQUERY_H
