#ifndef DBOCI_H
#define DBOCI_H

#include <QVariant>
#include "DBQuery.h"

namespace db
{
    //! Query for ORACLE database
    class CDBOCIQuery
        : public CDBQuery
    {
    public:
        //! Constructor
        /*!
            \param DB reference low-level database structure
        */
        CDBOCIQuery(const QSqlDatabase& DB);
        virtual CDBQuery* Clone() const;
        virtual void Prepare(const QString& sStatement, const QString& sReturn = QString());
        virtual QVariant GetLastInsertID();
    private:
        QVariant    m_LastID;
    };
}

#endif // DBOCI_H
