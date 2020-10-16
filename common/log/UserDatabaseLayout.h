#ifndef USERDATABASELAYOUT_H
#define USERDATABASELAYOUT_H

#ifndef LOG4QT_LINKED_JUDICIOUSLY
#include "databaselayout.h"
#else
#include <Log4Qt/databaselayout.h>
#endif

namespace Log4Qt {

class UserDatabaseLayout : public DatabaseLayout {
    Q_OBJECT
public:
    explicit UserDatabaseLayout(QObject *parent = 0);

public:
    virtual QSqlRecord formatRecord(const LoggingEvent &rEvent);
    virtual QString format(const LoggingEvent &rEvent);

    QString userColumn() const;
    void setUserColumn(const QString &columnName);

public slots:
    void userChanged(const QString &userName);

private:
    QString mUser;
    QString mUserName;
};

} // namespace Log4Qt

#endif // USERDATABASELAYOUT_H
