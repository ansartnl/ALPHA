#include "asterix002aero.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>

#include <math.h>


namespace Aerotechnica {

const QString Asterix002::SP_TimeInStorage = "SP_Time";

const QString Asterix002::SP_ADF = "SP_ADF";


Asterix002::Asterix002()
{}


void Asterix002::parse_050(QDataStream &stream, AsterixData &map)
{
    SStationConfigStatus status;
    status.part = read<quint8>(stream);
    if ( status.part & 0x01 )
        status.extents.append( read<quint8>(stream) );

    map.insert(StationConfigurationStatus, QVariant::fromValue(status));
}

void Asterix002::parse_060(QDataStream &stream, AsterixData &map)
{
    SStationProcessingMode mode;
    mode.part = read<quint8>(stream);

    map.insert(StationProcessingMode, QVariant::fromValue(mode));
}

void Asterix002::parse_sp(QDataStream &stream, AsterixData &map)
{
    quint8 len = read<quint8>(stream);
    if ( len < 2 )
        return;

    EMessageType typeMessage = (EMessageType) map.value(MessageType).toUInt();

    if ( typeMessage == NorthMarker )
    {
        map.insert(SP_TimeInStorage, QTime(0,0).addMSecs(double(read<quint8>(stream)) / 128. * 1000.));
    }
    else if ( typeMessage == SectorCrossing )
    {
        map.insert(SP_TimeInStorage, QTime(0,0).addMSecs(double(read<quint8>(stream)) / 128. * 1000.));

        SNorthSDI north;
        north.channels = read24(stream);
        map.insert(SP_ADF, QVariant::fromValue(north));
    }
}

bool Asterix002::generate_050(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, StationConfigurationStatus))
    {
        SStationConfigStatus status = map.value(StationConfigurationStatus).value<SStationConfigStatus>();
        write8(stream, status.part);
        if ( status.part & 0x01 && !status.extents.empty() )
            write8(stream, status.extents[0]);
        return true;
    }
    return false;
}

bool Asterix002::generate_060(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, StationProcessingMode))
    {
        SStationProcessingMode mode = map.value(StationProcessingMode).value<SStationProcessingMode>();
        write8(stream, mode.part);
        return true;
    }
    return false;
}

bool Asterix002::generate_sp(AsterixData &map, QDataStream &stream)
{
    quint8 len = 1;

    EMessageType typeMessage = (EMessageType) map.value(MessageType).toUInt();

    bool isTime = map.contains(SP_TimeInStorage);
    bool isAdf = map.contains(SP_ADF);

    if ( typeMessage == NorthMarker )
        len += isTime ? 1 : 0;
    else if ( typeMessage == SectorCrossing )
        len += isTime && isAdf ? 4 : 0;

    if ( len < 2 )
        return false;

    if ( typeMessage == NorthMarker )
    {
        write8(stream, QTime(0,0).msecsTo(map.value(SP_TimeInStorage).toTime()) / 1000. * 128.);
    }
    else if ( typeMessage == SectorCrossing )
    {
        write8(stream, QTime(0,0).msecsTo(map.value(SP_TimeInStorage).toTime()) / 1000. * 128.);

        SNorthSDI north = map.value(SP_ADF).value<SNorthSDI>();
        write24<quint32>(stream, north.channels);
    }

    return true;
}

QStringList Asterix002::fields() const
{
    static QStringList result = Asterix002Proto::fields()

        << SP_TimeInStorage

        << SP_ADF;

    return result;
}

}   // namespace Aerotechnica {
