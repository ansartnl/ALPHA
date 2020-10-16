#include "UserSignalHelper.h"

UserSignalHelper * UserSignalHelper::instance(QObject *parent)
{
    static UserSignalHelper *h = new UserSignalHelper(parent);
    return h;
}

UserSignalHelper::~UserSignalHelper()
{
}

void UserSignalHelper::changeUser(const QString &userName)
{
    instance()->p_changeUser(userName);
}


void UserSignalHelper::p_changeUser(const QString &userName)
{
    emit userChanged(userName);
}

UserSignalHelper::UserSignalHelper(QObject *parent) :
    QObject(parent)
{
}
