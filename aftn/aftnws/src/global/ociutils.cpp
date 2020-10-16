#include "ociutils.h"

#include <QtDebug>
#include <QtCore/QVariant>

#include <QtSql/QSqlDriver>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlIndex>

QString OciInsertClause(const QSqlDriver* driver, const QString& tableName,
  const QSqlRecord& record)
{
  if (tableName.isEmpty() || record.isEmpty())
    return QString::null;

  QString result(driver->sqlStatement(QSqlDriver::InsertStatement, tableName,
    record, false));

  QStringList names;
  QStringList values;
  for (int i = 0; i < record.count(); ++i) {
    names << record.fieldName(i);
    values << ":" + record.fieldName(i);
  };
  result.append(" RETURNING " + names.join(", ") + " INTO " + values.join(", "));

  return result;
}

QSqlError OciInsert(const QSqlDatabase& database, const QString& tableName,
  QSqlRecord& record)
{
  QString insertClause = OciInsertClause(database.driver(), tableName, record);
  QSqlQuery query(database);

  query.prepare(insertClause);
  for (int i = 0; i < record.count(); ++i)
    query.bindValue(record.fieldName(i), QString(1024, ' '), QSql::Out);

  if (query.exec()) {
    QMap<QString, QVariant> bounded = query.boundValues();
    foreach(const QString& key, bounded.keys())
        record.setValue(key, bounded.value(key));
    return QSqlError();
  }

  return query.lastError();
}

QString OciUpdateClause(QSqlDriver* driver, const QString& tableName,
  const QSqlRecord& record, const QSqlRecord& primaryKey)
{
  if (tableName.isEmpty() || record.isEmpty() || primaryKey.isEmpty())
    return QString::null;

  QString result(driver->sqlStatement(QSqlDriver::UpdateStatement, tableName,
    record, false));

  if (result.isEmpty())
    return QString::null;

  result.append(' ');
  result.append(driver->sqlStatement(QSqlDriver::WhereStatement, tableName,
    primaryKey, false));

  QStringList names;
  QStringList values;
  for (int i = 0; i < record.count(); ++i) {
    names << record.fieldName(i);
    values << ":" + record.fieldName(i);
  };
  result.append(" RETURNING " + names.join(", ") + " INTO " + values.join(", "));

  return result;
}

QSqlError OciUpdate(const QSqlDatabase& database, const QString& tableName,
  QSqlRecord& record, const QSqlRecord& primaryKey)
{
  QString updateClause = OciUpdateClause(database.driver(), tableName, record,
    primaryKey);
  QSqlQuery query(database);
  query.prepare(updateClause);

  for (int i = 0; i < record.count(); ++i)
    query.bindValue(record.fieldName(i), QString(1024, ' '), QSql::Out);


  if (query.exec()) {
  } else {
    return query.lastError();
  }

  QString selectClause = OciSelectClause(database.driver(), tableName, record,
    primaryKey);
  if (query.exec(selectClause))
    if (query.next()) {
      QMap<QString, QVariant> bounded = query.boundValues();
      foreach(const QString& key, bounded.keys())
          record.setValue(key, bounded.value(key));
      return QSqlError();
    }

  return query.lastError();
}

/*!
 */
QString OciDeleteClause(QSqlDriver* driver, const QString& tableName,
  const QSqlRecord& primaryKey)
{
  if (tableName.isEmpty() || primaryKey.isEmpty())
    return QString::null;

  QString result(driver->sqlStatement(QSqlDriver::DeleteStatement, tableName,
    primaryKey, false));
  result.append(' ');
  result.append(driver->sqlStatement(QSqlDriver::WhereStatement, tableName,
    primaryKey, false));
  return result;
}

/*!
 */
QStringList OciDeleteClauses(QSqlDriver* driver, const QString& tableName,
  const QList<QSqlRecord>& primaryKeys, int rowsInOneQuery)
{
  if (tableName.isEmpty() || primaryKeys.isEmpty())
    return QStringList();

  QString escapedTableName(driver->escapeIdentifier(tableName,
    QSqlDriver::TableName));
  QStringList resultList;
  QString result("DELETE FROM %1 WHERE %2");
  QString currentColumnValuePair;
  QStringList primaryKeyValues;
  QStringList whereList;
  int pkeyValuesCount = primaryKeys.at(0).count();

  if (!pkeyValuesCount)
    return QStringList();

  QList<QSqlRecord>::const_iterator currentPkey = primaryKeys.begin();
  QList<QSqlRecord>::const_iterator endPkey = primaryKeys.end();
  int g = 0;

  while (currentPkey != endPkey) {
    for (int i = 0; i < pkeyValuesCount; ++i) {
      currentColumnValuePair.append(driver->escapeIdentifier(
        (*currentPkey).fieldName(i), QSqlDriver::FieldName));

      if ((*currentPkey).value(i).isNull()) {
        currentColumnValuePair.append(" IS NULL ");
      } else {
        currentColumnValuePair.append('=');
        currentColumnValuePair.append(driver->formatValue((*currentPkey).field(
          i)));
      }

      primaryKeyValues.append(currentColumnValuePair);
      currentColumnValuePair.clear();
    }

    whereList.append("(" + primaryKeyValues.join(" AND ") + ")");
    primaryKeyValues.clear();
    ++currentPkey;
    ++g;

    if (!(g % rowsInOneQuery)) {
      result = result.arg(escapedTableName, whereList.join(" OR "));
      resultList.append(result);
      whereList.clear();
      result = QString("DELETE FROM %1 WHERE %2 ");
    }
  }

  if (g % rowsInOneQuery) {
    result = result.arg(escapedTableName, whereList.join(" OR "));
    resultList.append(result);
  }

  return resultList;
}

QString OciSelectClause(QSqlDriver* driver, const QString& tableName,
  const QSqlRecord& record, const QSqlRecord& primaryKey)
{
  QString result(driver->sqlStatement(QSqlDriver::SelectStatement, tableName,
    record, false));

  if (primaryKey.isEmpty())
    return result;

  result.append(' ');
  result.append(driver->sqlStatement(QSqlDriver::WhereStatement, tableName,
    primaryKey, false));
  return result;
}
