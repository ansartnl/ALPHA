#include "DBSettings.h"

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

DBSettings::DBSettings(const QSqlDatabase &db, QObject *parent) :
    QObject(parent), mDb(db)
{
    setErrorPattern("%e");
}

void DBSettings::setErrorPattern(const QString &pattern)
{
    errorPattern = pattern;
}

QVariant DBSettings::value(const QString &key, const QVariant &defaultValue)
{
    QSqlQuery query(mDb);
    query.prepare("SELECT value FROM settings WHERE key = :key");
    query.bindValue(":key", key);
    if (!query.exec())
    {
        processError(query);
        return defaultValue;
    }
    if (!query.next())
    {
        return defaultValue;
    }
    return b2v(query.value(0).toByteArray());
}

void DBSettings::setValue(const QString &key, const QVariant &value)
{
    if (contains(key))
        updateValue(key, value);
    else
        insertValue(key, value);
}

bool DBSettings::contains(const QString &key)
{
    QSqlQuery query(mDb);
    query.prepare("SELECT * FROM settings WHERE key = :key");
    query.bindValue(":key", key);
    if (!query.exec())
    {
        processError(query);
        return false;
    }
    return query.next();
}

void DBSettings::insertValue(const QString &key, const QVariant &value)
{
    QSqlQuery query(mDb);
    query.prepare("INSERT INTO settings (key, value) VALUES (:key, :value)");
    query.bindValue(":key", key);
    query.bindValue(":value", v2b(value));
    if (!query.exec())
        processError(query);
}

void DBSettings::updateValue(const QString &key, const QVariant &value)
{
    QSqlQuery query(mDb);
    query.prepare("UPDATE settings SET value = :value WHERE key = :key");
    query.bindValue(":key", key);
    query.bindValue(":value", v2b(value));
    if (!query.exec())
        processError(query);
}

void DBSettings::processError(const QSqlQuery &q)
{
    QString errorString = errorPattern;
    errorString.replace("%e", q.lastError().text());
    errorString.replace("%q", q.lastQuery());

    emit error(errorString);
    emit error(q);
}

QByteArray DBSettings::v2b(const QVariant &v)
{
    QByteArray ret;
    QDataStream stream(&ret, QIODevice::WriteOnly);
    stream << v;
    return ret;
}

QVariant DBSettings::b2v(const QByteArray &b)
{
    QVariant ret;
    QDataStream stream (b);
    stream >> ret;
    return ret;
}
