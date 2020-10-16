#ifndef DBQUERY_H
#define DBQUERY_H

#include <QSqlDatabase>
#include <QSqlQuery>

namespace db
{
    //! Base query for database
    class CDBQuery
        : public QSqlQuery
    {
    public:
        //! Constructor
        /*!
            \param DB reference low-level database structure
        */
        CDBQuery(const QSqlDatabase& DB);
        virtual ~CDBQuery();
        //! Create new query
        /*!
            \return query
        */
        virtual CDBQuery* Clone() const;
        //! Prepare SQL statement
        /*!
            \param sStatement SQL statement for preparation
            \param sReturn if statement is INSERT, than name of a primary key
        */
        virtual void Prepare(const QString& sStatement, const QString& sReturn = QString());
        //! Bind values with prepared SQL statement
        /*!
            \param placeholder placeholder
            \param val value
        */
        virtual void BindValue(const QString& placeholder, const QVariant& val);
        //! Get the identifier of a primary key after execution of an INSERT statement
        virtual QVariant GetLastInsertID();

    protected:
        //! Low-level database structure
        QSqlDatabase    m_DB;
    };
}
#endif // DBQUERY_H
