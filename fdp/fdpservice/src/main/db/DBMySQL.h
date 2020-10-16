#ifndef DBMYSQL_H
#define DBMYSQL_H

#include <QMap>
#include "DBQuery.h"

namespace db
{
    //! Query for MySQL database
    class CDBMYSQLQuery
        : public CDBQuery
    {
    public:
        //! Map for placeholders
        typedef QMap<QString, int> TPlaceMap;
        //! Constructor
        /*!
            \param DB reference low-level database structure
        */
        CDBMYSQLQuery(const QSqlDatabase& DB);
        virtual CDBQuery* Clone() const;
        virtual void Prepare(const QString& sStatement, const QString& sReturn = QString());
        virtual void BindValue(const QString& placeholder, const QVariant& val);

    protected:
        //! Map with placeholder from the query
        TPlaceMap    m_PlaceHolders;
    };
}
#endif // DBMYSQL_H
