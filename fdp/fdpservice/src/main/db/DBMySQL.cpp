#include "DBMySQL.h"

#include <QRegExp>

namespace db
{
    CDBMYSQLQuery::CDBMYSQLQuery(const QSqlDatabase& DB)
        : CDBQuery(DB)
    {

    }

    CDBQuery* CDBMYSQLQuery::Clone() const
    {
        return new CDBMYSQLQuery(m_DB);
    }

    void CDBMYSQLQuery::Prepare(const QString& sStatement, const QString& sReturn)
    {
        QString sNewStatement(sStatement);
        m_PlaceHolders.clear();
        int iIndex(0);
        int iBegin(0), iEnd(0);
        while (true)
        {
            iBegin = sStatement.indexOf(':', iEnd);
            if(iBegin == -1)
                break;
            iEnd = sStatement.indexOf(QRegExp("\\W+"), iBegin+1);
            if (iEnd == -1)
                iEnd = sStatement.size();
            m_PlaceHolders[sStatement.mid(iBegin, iEnd - iBegin)] = iIndex++;
            sNewStatement.replace(iBegin, iEnd - iBegin, QString("?").rightJustified(iEnd - iBegin));
        }

        CDBQuery::Prepare(sNewStatement);
    }

    void CDBMYSQLQuery::BindValue(const QString& placeholder, const QVariant& val)
    {
        TPlaceMap::iterator itFind = m_PlaceHolders.find(placeholder);
        if (itFind != m_PlaceHolders.end())
            bindValue(itFind.value(), val);
    }
}
