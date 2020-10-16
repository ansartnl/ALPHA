#include "DBQuery.h"

namespace db
{
    CDBQuery::CDBQuery(const QSqlDatabase& DB)
        : QSqlQuery(DB), m_DB(DB)
    {

    }

    CDBQuery* CDBQuery::Clone() const
    {
        return new CDBQuery(m_DB);
    }

    void CDBQuery::Prepare(const QString& sStatement, const QString& sReturn)
    {
        Q_UNUSED(sReturn)
        prepare(sStatement);
    }

    void CDBQuery::BindValue(const QString& placeholder, const QVariant& val, QSql::ParamType type)
    {
        bindValue(placeholder, val, type);
    }

    void CDBQuery::AddBindValue(const QVariant& val, QSql::ParamType type)
    {
        addBindValue(val, type);
    }

    QVariant CDBQuery::BoundValue(const QString& placeholder) const
    {
        return boundValue(placeholder);
    }

    QVariant CDBQuery::GetLastInsertID()
    {
        return lastInsertId();
    }

    QString CDBQuery::GetSystimestampStr() const
    {
        return "systimestamp";
    }

    QString CDBQuery::GetFromDualStr() const
    {
        return "FROM dual";
    }

    QString CDBQuery::GetSelectDbId() const
    {
        return "";
    }



    //! closed QSqlQuery methods
    bool CDBQuery::prepare(const QString& query)
    {
        return QSqlQuery::prepare(query);
    }

    void CDBQuery::bindValue(const QString& placeholder, const QVariant& val, QSql::ParamType type)
    {
        QSqlQuery::bindValue(placeholder, val, type);
    }

    void CDBQuery::bindValue(int pos, const QVariant& val, QSql::ParamType type)
    {
        QSqlQuery::bindValue(pos, val, type);
    }

    void CDBQuery::addBindValue(const QVariant& val, QSql::ParamType type)
    {
        QSqlQuery::addBindValue(val, type);
    }

    QVariant CDBQuery::boundValue(const QString& placeholder) const
    {
        return QSqlQuery::boundValue(placeholder);
    }

    QVariant CDBQuery::boundValue(int pos) const
    {
        return QSqlQuery::boundValue(pos);
    }

    QVariant CDBQuery::lastInsertId() const
    {
        return QSqlQuery::lastInsertId();
    }
}
