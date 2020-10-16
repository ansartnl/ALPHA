#include <QtCore/QCryptographicHash>
#include <QtCore/QDataStream>
#include <QtCore/QRegExp>
#include <QtDebug>

#include "Base32.h"
#include "HardwareId.h"

#define MINIMUM_ID_COUNT 2
#define MINIMUM_MATCH_COUNT 2

HardwareId::HardwareId()
{
}

HardwareId::HardwareId(const QByteArray &data)
{
    fromByteArray(data);
}

bool HardwareId::setId(HardwareType type, QString id)
{
    // remove non alphanumeric characters
    id.remove(QRegExp("[^a-zA-Z\\d]"));

    if (!id.isEmpty())
    {
        this->id[type] = QCryptographicHash::hash(id.toLatin1(), QCryptographicHash::Sha1);
        return true;
    }

    return false;
}

bool HardwareId::isValid() const
{
    return id.size() >= MINIMUM_ID_COUNT;
}

QByteArray HardwareId::toRegKey() const
{
    QByteArray result;
    if (isValid())
    {
        QByteArray hash = QCryptographicHash::hash(toByteArray(), QCryptographicHash::Sha1);
        QByteArray data = Base32::encode(hash);
        for (int i = 0; i < 15; ++i)
        {
            result.append(data[i * 2]);
        }
    }

    return result;
}

bool HardwareId::verify(const HardwareId &currentId) const
{
    int validCount = 0;
    for (QMap<HardwareType, QByteArray>::const_iterator it = id.begin(); it != id.end(); ++it)
    {
        if (it.value() == currentId.id.value(it.key()))
        {
            ++validCount;
        }
    }

    return validCount >= MINIMUM_MATCH_COUNT;
}

void HardwareId::print() const
{
    qDebug() << "Hardware ID";
    for (QMap<HardwareType, QByteArray>::const_iterator it = id.begin(); it != id.end(); ++it)
    {
        QString type;
        switch (it.key())
        {
        case Board:
            type = "Motherboard";
            break;
        case Drive:
            type = "Drive";
            break;
        case Ethernet:
            type = "Ethernet";
            break;
        case Memory:
            type = "Memory";
            break;
        }

        qDebug() << QString("%1: %2").arg(type).arg(QString(it.value().toHex()));
    }
}

QByteArray HardwareId::toByteArray() const
{
    QByteArray result;
    QDataStream ds(&result, QIODevice::WriteOnly);
    ds.setVersion(QDataStream::Qt_4_6);
    ds << id;

    return result;
}

void HardwareId::fromByteArray(const QByteArray &data)
{
    id.clear();

    QDataStream ds(data);
    ds.setVersion(QDataStream::Qt_4_6);
    ds >> id;
}

QDataStream &operator>>(QDataStream &stream, HardwareId::HardwareType &type)
{
    int value;
    stream >> value;
    type = (HardwareId::HardwareType)value;
    return stream;
}
