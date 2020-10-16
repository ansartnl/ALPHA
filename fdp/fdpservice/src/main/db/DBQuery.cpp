#include "DBQuery.h"
#include <QVariant>

namespace db
{
    CDBQuery::CDBQuery(const QSqlDatabase& DB)
        : QSqlQuery(DB), m_DB(DB)
    {

    }
    CDBQuery::~CDBQuery()
    {

    }

    CDBQuery* CDBQuery::Clone() const
    {
        return new CDBQuery(m_DB);
    }

    void CDBQuery::Prepare(const QString& sStatement, const QString& sReturn)
    {
        prepare(sStatement);
    }

    void CDBQuery::BindValue(const QString& placeholder, const QVariant& val)
    {
        bindValue(placeholder, val);
    }

    QVariant CDBQuery::GetLastInsertID()
    {
        return lastInsertId();
    }
}
