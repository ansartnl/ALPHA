#include "SmartSender.h"

SmartRequst::~SmartRequst()
{
    if (data.isEmpty() || !s)
        return;

    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);
    foreach (const QVariant &v, data)
        stream << v;
    s->send(arr);
}

SmartSender::SmartSender()
{
}

void SmartSender::sendVariant(const QVariant &data)
{
    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);
    stream << data;
    send(arr);
}
