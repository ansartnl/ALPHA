#include "usTransport.h"

namespace UserSystem
{

Transport::Transport(QObject *parent) :
    QObject(parent)
{
}

void Transport::sendData(const QVariant &data)
{
    emit send(data);
}

QString Transport::userName() const
{
    return mUserName;
}

void Transport::setUserName(const QString &u)
{
    mUserName = u;
}

void Transport::onDataReceived(const QVariant &data)
{
    emit receive(data);
}

} // namespace
