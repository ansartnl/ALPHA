#include "DBOCI.h"

namespace db
{
    CDBOCIQuery::CDBOCIQuery(const QSqlDatabase& DB)
        : CDBQuery(DB), m_LastID(QVariant::UInt)
    {

    }

    CDBQuery* CDBOCIQuery::Clone() const
    {
        return new CDBOCIQuery(m_DB);
    }

    void CDBOCIQuery::Prepare(const QString& sStatement, const QString& sReturn)
    {
        if (!sReturn.isEmpty())
        {
            QString sNewStat = (sStatement + " RETURNING " + sReturn + " INTO :LastInsertId") ;
            CDBQuery::Prepare(sNewStat);
            bindValue(":LastInsertId", m_LastID);
        } else
            CDBQuery::Prepare(sStatement);

    }

    QVariant CDBOCIQuery::GetLastInsertID()
    {
        return boundValue(":LastInsertId");
    }
}
