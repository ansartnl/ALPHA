#ifndef PULSDATA_H
#define PULSDATA_H

#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtCore/QVariantHash>

struct PulsData
{
    static const int version = 1;

    enum Status {
        INVALID = -1
        , INACTIVE = 0
        , ACTIVE = 1
        , ACTIVATE = 2
        , PASSIVE = 3
    } status;

    enum Error {
        NO_ERROR = 0
        , STREAM_NOT_OK
        , VERSION_TOO_BIG
        , VERSION_NOT_SUPPORTED
    } lastError;
    inline bool hasError() const
    {
        return (lastError != NO_ERROR);
    }

    QString pulsSourceIp;
    QStringList information;
    QDateTime dateTime;
    QVariantHash additionalInfo;

    PulsData();

    QString errorString() const;

    const QByteArray toByteArray() const;
    static const PulsData fromByteArray(const QByteArray &data);
};

struct PulsDataPrivate
{
    static const PulsData fromByteArrayV1(const QByteArray &data);
};

#endif // PULSDATA_H
