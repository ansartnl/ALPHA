#ifndef OCIUTILS_H
#define OCIUTILS_H

#include <QtCore/QString>
class QSqlDriver;

#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtCore/QVariant>

/*!
 Funtions for work with oracle database.
 \file ociutils.h
 */
QString OciInsertClause(const QSqlDriver* driver, const QString& tableName,
  const QSqlRecord& record);

QSqlError OciInsert(const QSqlDatabase& database, const QString& tableName,
  QSqlRecord& record);

QString OciUpdateClause(QSqlDriver* driver, const QString& tableName,
  const QSqlRecord& record, const QSqlRecord& primaryKey);

QSqlError OciUpdate(const QSqlDatabase& database, const QString& tableName,
  QSqlRecord& record, const QSqlRecord& primaryKey);

QString OciDeleteClause(QSqlDriver* driver, const QString& tableName,
  const QSqlRecord& primaryKey);

QStringList OciDeleteClauses(QSqlDriver* driver, const QString& tableName,
  const QList<QSqlRecord>& primaryKeys, int rowsInOneQuery = 10);

QString OciSelectClause(QSqlDriver* driver, const QString& tableName,
  const QSqlRecord& record, const QSqlRecord& primaryKey = QSqlRecord());

inline QDataStream& operator >>(QDataStream& stream, QSqlField& field)
{
  QVariant::Type type;
  QVariant value;
  QString name;
  bool isAuto;
  bool isGenerated;
  bool isReadOnly;
  stream >> type >> isAuto >> isGenerated >> isReadOnly >> name >> value;
  field.setName(name);
  field.setAutoValue(isAuto);
  field.setGenerated(isGenerated);
  field.setReadOnly(isReadOnly);
  field.setType(type);
  field.setValue(value);
  return stream;
}

inline QDataStream& operator <<(QDataStream& stream, const QSqlField& field)
{
  stream << field.type() << field.isAutoValue() << field.isGenerated()
      << field.isReadOnly() << field.name() << field.value();
  return stream;
}

inline QDataStream& operator >>(QDataStream& stream, QSqlRecord& record)
{
  int count;
  QSqlField field;
  stream >> count;
  for (int i = 0; i < count; ++i) {
    stream >> field;
    record.append(field);
  }
  return stream;
}

inline QDataStream& operator <<(QDataStream& stream, const QSqlRecord& record)
{
  int count = record.count();
  stream << count;
  for (int i = 0; i < count; ++i)
    stream << record.field(i);
  return stream;
}

#endif // SQLUTILS_H
