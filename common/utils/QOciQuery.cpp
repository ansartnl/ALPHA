#include "QOciQuery.h"
#include <QtCore/QVariant>

QOciQuery::QOciQuery(const QSqlDatabase& DB) :
  QSqlQuery(DB), tempValue(QVariant::UInt)
{
}

void QOciQuery::prepare(const QString& sStatement, const QString& sReturn)
{
  if (!sReturn.isEmpty()) {
    QString sNewStat = (sStatement + " RETURNING " + sReturn
        + " INTO :LastInsertId");
    QSqlQuery::prepare(sNewStat);
    bindValue(":LastInsertId", tempValue);
  } else
    QSqlQuery::prepare(sStatement);
}

QVariant QOciQuery::GetLastInsertID()
{
  return boundValue(":LastInsertId");
}
