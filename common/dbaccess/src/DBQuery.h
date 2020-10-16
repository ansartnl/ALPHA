#ifndef DB__QUERY_H
#define DB__QUERY_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

namespace db
{
    //! Base query for database
    class CDBQuery : public QSqlQuery
    {
    public:
        //! Constructor
        /*!
            \param DB reference low-level database structure
        */
        CDBQuery(const QSqlDatabase& DB);
        virtual ~CDBQuery() {}
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
            \param type parameter type
        */
        virtual void BindValue(const QString& placeholder, const QVariant& val, QSql::ParamType type = QSql::In);
        //! Add bind values with prepared SQL statement
        /*!
            \param val value
            \param type parameter type
        */
        virtual void AddBindValue(const QVariant& val, QSql::ParamType type = QSql::In);
        //! Bound value
        /*!
            \param placeholder placeholder
            \return value
        */
        virtual QVariant BoundValue(const QString& placeholder) const;
        //! Get the identifier of a primary key after execution of an INSERT statement
        virtual QVariant GetLastInsertID();

        //! Clauses
        virtual QString GetSystimestampStr() const;
        virtual QString GetFromDualStr() const;

        //! Database environment
        virtual QString GetSelectDbId() const;

    protected:
        //@{
        //! closed QSqlQuery methods
        bool prepare(const QString& query);
        void bindValue(const QString& placeholder, const QVariant& val, QSql::ParamType type = QSql::In);
        void bindValue(int pos, const QVariant& val, QSql::ParamType type = QSql::In);
        void addBindValue(const QVariant& val, QSql::ParamType type = QSql::In);
        QVariant boundValue(const QString& placeholder) const;
        QVariant boundValue(int pos) const;
        QVariant lastInsertId() const;
        //@}

    protected:
        //! Low-level database structure
        QSqlDatabase	m_DB;
    };
}
#endif // DB__QUERY_H
