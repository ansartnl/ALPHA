#include "PulsData.h"

#include <QtCore/QObject>

PulsData::PulsData() :
    status(INVALID), lastError(NO_ERROR)
{
}

QString PulsData::errorString() const
{
    switch (lastError)
    {
    case NO_ERROR:
        return QObject::tr("No error");
    case STREAM_NOT_OK:
        return QObject::tr("Stream not OK");
    case VERSION_TOO_BIG:
        return QObject::tr("Version too big");
    case VERSION_NOT_SUPPORTED:
        return QObject::tr("Version not supported");
    }
    return QObject::tr("Unhandled error");
}

const QByteArray PulsData::toByteArray() const
{
    QByteArray ret;
    QDataStream stream(&ret, QIODevice::WriteOnly);

    stream << version;
    stream << int(status);
    stream << pulsSourceIp;
    stream << information;
    stream << dateTime;
    stream << additionalInfo;

    return ret;
}

const PulsData PulsData::fromByteArray(const QByteArray &data)
{
    PulsData ret;

    QDataStream stream(data);

    int v;
    stream >> v;
    if (stream.status() != QDataStream::Ok)
    {
        ret.lastError = STREAM_NOT_OK;
        return ret;
    }
    if (v > version)
    {
        ret.lastError = VERSION_TOO_BIG;
        return ret;
    }

    switch (v)
    {
    case 1:
        ret = PulsDataPrivate::fromByteArrayV1(data);
        break;
    default:
        ret.lastError = VERSION_NOT_SUPPORTED;
    }

    return ret;
}

const PulsData PulsDataPrivate::fromByteArrayV1(const QByteArray &data)
{
    PulsData ret;

    QDataStream stream(data);

    int v;
    int s;
    stream >> v;
    Q_ASSERT_X(v == 1, "PulsData::fromByteArrayV1", "version not equal 1");
    stream >> s;
    ret.status = PulsData::Status(s);
    stream >> ret.pulsSourceIp;
    stream >> ret.information;
    stream >> ret.dateTime;
    stream >> ret.additionalInfo;
    if (stream.status() != QDataStream::Ok)
        ret.lastError = PulsData::STREAM_NOT_OK;

    return ret;
}
