#include "UserDatabaseLayout.h"

#ifndef LOG4QT_LINKED_JUDICIOUSLY
#include "loggingevent.h"
#else
#include <Log4Qt/loggingevent.h>
#endif

#include <QtSql/QSqlField>
#include <QtSql/QSqlRecord>

namespace Log4Qt {

UserDatabaseLayout::UserDatabaseLayout(QObject *parent) :
    DatabaseLayout(parent) {
}

QSqlRecord UserDatabaseLayout::formatRecord(const LoggingEvent &rEvent) {
    QSqlField field;
    QSqlRecord record = DatabaseLayout::formatRecord(rEvent);

    if (!mUser.isEmpty()) {
        field.setName(mUser);
        field.setType(QVariant::String);
        field.setGenerated(true);
        field.setValue(mUserName);
        record.append(field);
    }

    mUserName.clear();

    return record;
}

QString UserDatabaseLayout::format(const LoggingEvent &rEvent) {
    QString result = DatabaseLayout::format(rEvent);

    if (!mUser.isEmpty()) {
        result.append(mUser);
        result.append(":");
        result.append(mUser);
        result.append("; ");
    }
    return result;
}

QString UserDatabaseLayout::userColumn() const {
    return mUser;
}

void UserDatabaseLayout::setUserColumn(const QString &columnName) {
    mUser = columnName;
}

void UserDatabaseLayout::userChanged(const QString &userName) {
    mUserName = userName;
}

} // namespace Log4Qt
