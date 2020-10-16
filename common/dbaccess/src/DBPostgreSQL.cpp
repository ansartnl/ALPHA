#include "DBPostgreSQL.h"

namespace db
{
    CDBPostgreSQLQuery::CDBPostgreSQLQuery(const QSqlDatabase& DB)
		: CDBQuery(DB), m_LastID(QVariant::UInt)
	{

	}

    CDBQuery* CDBPostgreSQLQuery::Clone() const
	{
        return new CDBPostgreSQLQuery(m_DB);
	}

    void CDBPostgreSQLQuery::Prepare(const QString& sStatement, const QString& sReturn)
	{
		if (!sReturn.isEmpty())
		{
            QString sNewStat = (sStatement + " RETURNING " + sReturn + " AS LastInsertId") ;
			CDBQuery::Prepare(sNewStat);
            //bindValue(":LastInsertId", m_LastID, QSql::InOut);
		} else
			CDBQuery::Prepare(sStatement);

	}

    QVariant CDBPostgreSQLQuery::GetLastInsertID()
	{
        QVariant var;
        //var = boundValue(":LastInsertId");
        if(next())
        {
            var = value(0);
        }
        return var;
	}

    QString CDBPostgreSQLQuery::GetSystimestampStr() const
    {
        return "now()";
    }

    QString CDBPostgreSQLQuery::GetFromDualStr() const
    {
        return "";
    }

    QString CDBPostgreSQLQuery::GetSelectDbId() const
    {
        return QString("SELECT datid FROM pg_stat_database WHERE datname = '%1'").arg(m_DB.databaseName());
    }
}
