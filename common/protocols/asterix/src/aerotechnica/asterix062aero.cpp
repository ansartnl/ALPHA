#include "asterix062aero.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>

#include <math.h>


namespace Aerotechnica {

const QString Asterix062::SP_BRF = "a62.SP_BRF";
const QString Asterix062::SP_BoardNumber = "a62.SP_BoardNumber";
const QString Asterix062::SP_Altitude = "a62.SP_Altitude";
const QString Asterix062::SP_Fuel = "a62.SP_Fuel";
const QString Asterix062::SP_Emergency = "a62.SP_Emergency";
const QString Asterix062::SP_FplStatus = "a62.SP_FplStatus";
const QString Asterix062::SP_HeightQNH = "a62.SP_HeightQNH";
const QString Asterix062::SP_ValidStatus = "a62.SP_ValidStatus";


Asterix062::Asterix062()
{}

void Asterix062::parse_sp(QDataStream &stream, AsterixData &map)
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
        map.insert(SP_Emergency, read<quint8>(stream) >> 7);

    // Read "BRF"
    if (fspec.isSet(5))
        map.insert(SP_BRF, read<quint8>(stream));

    // Read "FPL Status"
    if (fspec.isSet(6))
        map.insert(SP_FplStatus, read<quint8>(stream));

    // Read "Valid status" structure
    if (fspec.isSet(7))
    {
        quint8 status = read<quint8>(stream);

        SValidStatus vs;
        vs.isBoardNumberValid = !(status & 0x01);
        vs.isHeightValid = !(status & 0x02);
        map.insert(SP_FplStatus, QVariant::fromValue(vs));
    }

    // Read "QNH corrected height"
    if (fspec.isSet(8))
    {
        double h = (double)read<qint16>(stream) * 25.;
        if ( h > -10000. )
            map.insert(SP_HeightQNH, h);
    }
}

bool Asterix062::generate_sp(AsterixData &map, QDataStream &stream)
{
    quint8 len = 1;

    bool isBoardNumber = map.contains(SP_BoardNumber);
    bool isAltitude = map.contains(SP_Altitude);
    bool isFuel = map.contains(SP_Fuel);
    bool isEmergency = map.contains(SP_Emergency);
    bool isTime = map.contains(SP_BRF);
    bool isHeightQNH = map.contains(SP_BRF);
    bool isValidStatus = map.contains(SP_ValidStatus);
    bool isFplStatus = map.contains(SP_FplStatus);

    if ( !(isBoardNumber || isAltitude || isFuel || isEmergency || isTime ||
           isHeightQNH || isValidStatus || isFplStatus) )
        return false;

    len += isBoardNumber ? 3 : 0;
    len += isAltitude ? 2 : 0;
    len += isFuel ? 1 : 0;
    len += isEmergency ? 1 : 0;
    len += isTime ? 1 : 0;
    len += isHeightQNH ? 2 : 0;
    len += isValidStatus ? 1 : 0;
    len += isFplStatus ? 1 : 0;

    // Save lenngth of the field
    write8(stream, len);

    QByteArray result;
    {
        PresentData mainMask;
        QDataStream streamSP(&result, QIODevice::WriteOnly);

        // Save "Board Number"
        if (isBoardNumber)
        {
            write24(streamSP, map.value(SP_BoardNumber).toUInt());
            mainMask.set(1);
        }

        // Save "Altitude"
        if (isAltitude)
        {
            SAltitude altitude = map.value(SP_Altitude).value<SAltitude>();
            write16(streamSP, altitude.code);
            mainMask.set(2);
        }

        // Save "Fuel Reserve"
        if (isFuel)
        {
            write8(streamSP, map.value(SP_Fuel).toUInt());
            mainMask.set(3);
        }

        // Save "Emergency"
        if (isEmergency)
        {
            write8(streamSP, quint8(map.value(SP_Emergency).toUInt()) << 7);
            mainMask.set(4);
        }

        // Save "BRF"
        if (isTime)
        {
            write8(streamSP, map.value(SP_BRF).toUInt());
            mainMask.set(5);
        }

        // Save "FPL Status"
        if (isTime)
        {
            write8(streamSP, map.value(SP_FplStatus).toUInt());
            mainMask.set(6);
        }

        // Save "Valid status" structure
        if (isTime)
        {
            SValidStatus vs = map.value(SP_BRF).value<SValidStatus>();
            quint8 byte = 0;
            if ( !vs.isBoardNumberValid ) byte |= 0x01;
            if ( !vs.isHeightValid ) byte |= 0x02;

            write8(streamSP, byte);
            mainMask.set(7);
        }

        // Save "QNH corrected height"
        if (isTime)
        {
            write16<qint16>(streamSP, map.value(SP_HeightQNH).toUInt() / 25);
            mainMask.set(8);
        }

        result.prepend(mainMask.data());
    }
    writeArray(stream, result);

    return true;
}

QStringList Asterix062::fields() const
{
    static QStringList result = Asterix062Proto::fields()

        << SP_BRF
        << SP_BoardNumber
        << SP_Altitude
        << SP_Fuel
        << SP_Emergency
        << SP_FplStatus
        << SP_HeightQNH
        << SP_ValidStatus;

    return result;
}

}   // namespace Aerotechnica {
