#ifndef DBSETTINGS_H
#define DBSETTINGS_H

#include <QtCore/QVariant>

#include <QtSql/QSqlDatabase>

class DBSettings : public QObject
{
    Q_OBJECT
public:
    DBSettings(const QSqlDatabase &db, QObject *parent = 0);

    /*
     * %e - error text
     * %q - query
     */
    void setErrorPattern(const QString &pattern);

    QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    void setValue(const QString &key, const QVariant &value);

    bool contains(const QString &key);

signals:
    void error(const QString &error);
    void error(const QSqlQuery &query);

protected:
    void insertValue(const QString &key, const QVariant &value);
    void updateValue(const QString &key, const QVariant &value);

    void processError(const QSqlQuery &q);

    QByteArray v2b(const QVariant &v);
    QVariant b2v(const QByteArray &b);

protected:
    QSqlDatabase mDb;
    QString errorPattern;
};

#endif // DBSETTINGS_H
