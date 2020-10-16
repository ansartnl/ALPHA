#include "asterix001aero.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>

#include <math.h>


namespace Aerotechnica {

const QString Asterix001::SP_TimeInStorage = "SP_Time";

const QString Asterix001::SP_BoardNumber = "SP_BoardNumber";

const QString Asterix001::SP_Altitude = "SP_Altitude";

const QString Asterix001::SP_Fuel = "SP_Fuel";

const QString Asterix001::SP_Emergency_S = "SP_Emergency_S";


Asterix001::Asterix001()
{}

void Asterix001::parse_sp(QDataStream &stream, AsterixData &map)
{
    quint8 len = read<quint8>(stream);
    if ( len < 2 )
        return;

    PresentData fspec(stream);

    // Read "Board Number"
    if (fspec.isSet(1))
        map.insert(SP_BoardNumber, read24(stream));

    // Read "Altitude"
    if (fspec.isSet(2))
    {
        SAltitude altitude;
        altitude.code = read<quint16>(stream);
        map.insert(SP_Altitude, QVariant::fromValue(altitude));
    }

    // Read "Fuel Reserve"
    if (fspec.isSet(3))
        map.insert(SP_Fuel, read<quint8>(stream));

    // Read "Emergency"
    if (fspec.isSet(4))
        map.insert(SP_Emergency_S, read<quint8>(stream) >> 7);

    // Read "Time In Storage"
    if (fspec.isSet(5))
        map.insert(SP_TimeInStorage, QTime(0,0).addMSecs(double(read<quint8>(stream)) / 128. * 1000.));
}

bool Asterix001::generate_sp(AsterixData &map, QDataStream &stream)
{
    quint8 len = 1;

    bool isBoardNumber = map.contains(SP_BoardNumber);
    bool isAltitude = map.contains(SP_Altitude);
    bool isFuel = map.contains(SP_Fuel);
    bool isEmergency = map.contains(SP_Emergency_S);
    bool isTime = map.contains(SP_TimeInStorage);

    if ( !(isBoardNumber || isAltitude || isFuel || isEmergency || isTime) )
        return false;

    len += isBoardNumber ? 3 : 0;
    len += isAltitude ? 2 : 0;
    len += isFuel ? 1 : 0;
    len += isEmergency ? 1 : 0;
    len += isTime ? 1 : 0;

    // Save lenngth of the field
    write8(stream, len);

    QByteArray result;
    {
        PresentData mainMask;
        QDataStream streamSP(&result, QIODevice::WriteOnly);

        // Save "Board Number"
        if (isBoardNumber)
            write24(streamSP, map.value(SP_BoardNumber).toUInt());

        // Save "Altitude"
        if (isAltitude)
        {
            SAltitude altitude = map.value(SP_Altitude).value<SAltitude>();
            write16(streamSP, altitude.code);
        }

        // Save "Fuel Reserve"
        if (isFuel)
            write8(streamSP, map.value(SP_Fuel).toUInt());

        // Save "Emergency"
        if (isEmergency)
            write8(streamSP, quint8(map.value(SP_Emergency_S).toUInt()) << 7);

        // Save "Time In Storage"
        if (isTime)
            write8(streamSP, QTime(0,0).msecsTo(map.value(SP_TimeInStorage).toTime()) / 1000. * 128.);
    }
    writeArray(stream, result);

    return true;
}

QStringList Asterix001::fields() const
{
    static QStringList result = QStringList()

        << SP_TimeInStorage
        << SP_BoardNumber
        << SP_Altitude
        << SP_Fuel
        << SP_Emergency_S;

    return result;
}

}   // namespace Aerotechnica {
