#include "asterix020euro.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QTime>
#include <math.h>


namespace Eurocontrol {

// Reserved Expansion Field: Deviation of Position
const QString Asterix020::RE_PositionAccuracy_DOP = "a020.RE_PositionAccuracy_DOP";
// Reserved Expansion Field: Standard Deviation of Position (Cartesian)
const QString Asterix020::RE_PositionAccuracy_SDC = "a020.RE_PositionAccuracy_SDC";
// Reserved Expansion Field: Standard Deviation of Position (WGS-84)
const QString Asterix020::RE_PositionAccuracy_SDW = "a020.RE_PositionAccuracy_SDW";
// Reserved Expansion Field: Standard Deviation of Geometric Height (WGS-84)
const QString Asterix020::RE_PositionAccuracy_SDH = "a020.RE_PositionAccuracy_SDH";
// Reserved Expansion Field: Ground Velocity Vector
const QString Asterix020::RE_GroundSpeed = "a020.RE_GroundSpeed";
// Reserved Expansion Field: Track Angle clockwise reference to “True North”
const QString Asterix020::RE_TrackAngle = "a020.RE_TrackAngle";
// Reserved Expansion Field: Track Angle clockwise reference to “True North”
const QString Asterix020::RE_GroundVelocityAccuracy = "a020.RE_GroundVelocityAccuracy";
// Reserved Expansion Field: Time of ASTERIX Report Transmission
const QString Asterix020::RE_TimeReportTransmission = "a020.RE_TimeReportTransmission";
// Reserved Expansion Field: Special Position Identification Age
const QString Asterix020::RE_SPI = "a020.RE_SPI";
// Reserved Expansion Field: Target Identification age
const QString Asterix020::RE_TI = "a020.RE_TI";
// Reserved Expansion Field: Mode S MB age
const QString Asterix020::RE_MBD = "a020.RE_MBD";
// Reserved Expansion Field: Mode S MB age
const QString Asterix020::RE_M3A = "a020.RE_M3A";
// Reserved Expansion Field: Flight Level age
const QString Asterix020::RE_FL = "a020.RE_FL";
// Reserved Expansion Field: Flight Status age
const QString Asterix020::RE_FS = "a020.RE_FS";
// Reserved Expansion Field: Geometric / Measured Height age
const QString Asterix020::RE_GH = "a020.RE_GH";
// Reserved Expansion Field: Target Address age
const QString Asterix020::RE_TA = "a020.RE_TA";
// Reserved Expansion Field: Mode C code age
const QString Asterix020::RE_MC = "a020.RE_MC";
// Reserved Expansion Field: Mode-S Specific Service Capability age
const QString Asterix020::RE_MSS = "a020.RE_MSS";
// Reserved Expansion Field: Altitude reporting capability age
const QString Asterix020::RE_ARC = "a020.RE_ARC";
// Reserved Expansion Field: Aircraft identification capability age
const QString Asterix020::RE_AIC = "a020.RE_AIC";
// Reserved Expansion Field: Mode-2 Code age
const QString Asterix020::RE_M2 = "a020.RE_M2";
// Reserved Expansion Field: Mode-1 Code age
const QString Asterix020::RE_M1 = "a020.RE_M1";
// Reserved Expansion Field: ACAS Resolution Advisory age
const QString Asterix020::RE_ARA = "a020.RE_ARA";
// Reserved Expansion Field: Vehicle Fleet Identification age
const QString Asterix020::RE_VI = "a020.RE_VI";
// Reserved Expansion Field: Pre-programmed message age
const QString Asterix020::RE_MSG = "a020.RE_MSG";


Asterix020::Asterix020()
{}


void Asterix020::parse_re(QDataStream &stream, AsterixData &map)
{
    quint8 len = read<quint8>(stream);
    if ( len < 2 )
        return;

    quint8 indicator = read<quint8>(stream);

    // Read "Position Accuracy"
    if ( indicator & 0x80 )
    {
        quint8 primary = read<quint8>(stream);

        // Read "Deviation of Position"
        if ( primary & 0x80 )
        {
            SDeviationPosition dp;
            dp.x = double(read<qint16>(stream)) / 4.;
            dp.y = double(read<qint16>(stream)) / 4.;
            dp.rho = double(read<qint16>(stream)) / 4.;

            map.insert(RE_PositionAccuracy_DOP, QVariant::fromValue(dp));
        }
        // Read "Standard Deviation of Position (Cartesian)"
        if ( primary & 0x40 )
        {
            SDeviationPosition dp;
            dp.x = double(read<qint16>(stream)) / 4.;
            dp.y = double(read<qint16>(stream)) / 4.;
            dp.rho = double(read<qint16>(stream)) / 4.;

            map.insert(RE_PositionAccuracy_SDC, QVariant::fromValue(dp));
        }
        // Read "Standard Deviation of Geometric Height (WGS-84)"
        if ( primary & 0x20 )
        {
            map.insert(RE_PositionAccuracy_SDH, double(read<qint16>(stream)));
        }
        // Read "Standard Deviation of Position (WGS-84)"
        if ( primary & 0x10 )
        {
            SDeviationPosition dp;
            dp.x = double(read<qint16>(stream)) *180. / 33554432.;
            dp.y = double(read<qint16>(stream)) * 180. / 33554432.;
            dp.rho = double(read<qint16>(stream)) * 180. / 33554432.;

            map.insert(RE_PositionAccuracy_SDW, QVariant::fromValue(dp));
        }
    }

    // Read "Ground Velocity Vector"
    if ( indicator & 0x40 )
    {
        quint16 byte2 = read<quint16>(stream);

        SGroundSpeed gs;
        gs.speed = (byte2 & 0x7FFF) * 0.00006103515625;
        gs.isValueExceedsRange = byte2 & 0x8000;

        map.insert(RE_GroundSpeed, QVariant::fromValue(gs));
        map.insert(RE_TrackAngle, double(read<quint16>(stream)) * 0.0055);
    }

    // Read "Ground Velocity Accuracy"
    if ( indicator & 0x20 )
    {
        SGroundVelocityAccuracy gva;
        gva.groundSpeed = double(read<quint8>(stream)) * 0.00006103515625;
        gva.trackAngle = double(read<quint8>(stream)) * 0.08789;

        map.insert(RE_GroundVelocityAccuracy, QVariant::fromValue(gva));
    }

    // Read "Time of ASTERIX Report Transmission"
    if ( indicator & 0x10 )
    {
        map.insert(RE_TimeReportTransmission, QTime().addMSecs(ceil(double(read24(stream)) * 7.8125)));
    }

    // Read "Data-Ages"
    if ( indicator & 0x08 )
    {
        PresentData mainMask(stream);

        // SPI
        if (mainMask.isSet(1))
            map.insert(RE_SPI, double(read<quint8>(stream)) * 0.1);
        // TI
        if (mainMask.isSet(2))
            map.insert(RE_TI, double(read<quint8>(stream)) * 0.1);
        // MBD
        if (mainMask.isSet(3))
        {
            quint8 rep = read<quint8>(stream);

            TModeSMBDataAgeList data;
            for ( int i = 0; i < rep; i++ )
            {
                SModeSMBDataAge modeMB;
                modeMB.BDS2 = read<quint8>(stream);
                modeMB.BDS1 = modeMB.BDS2 >> 4;
                modeMB.BDS2 &= 0x0F;
                modeMB.MBA = double(read<quint8>(stream))*0.1;
                data.append( QVariant::fromValue(modeMB) );
            }

            map.insert(RE_MBD, data);
        }
        // M3A
        if (mainMask.isSet(4))
            map.insert(RE_M3A, double(read<quint8>(stream)) * 0.1);
        // FL
        if (mainMask.isSet(5))
            map.insert(RE_FL, double(read<quint8>(stream)) * 0.1);
        // FS
        if (mainMask.isSet(6))
            map.insert(RE_FS, double(read<quint8>(stream)) * 0.1);
        // GH
        if (mainMask.isSet(7))
            map.insert(RE_GH, double(read<quint8>(stream)) * 0.1);
        // TA
        if (mainMask.isSet(8))
            map.insert(RE_TA, double(read<quint8>(stream)) * 0.1);
        // MC
        if (mainMask.isSet(9))
            map.insert(RE_MC, double(read<quint8>(stream)) * 0.1);
        // MSS
        if (mainMask.isSet(10))
            map.insert(RE_MSS, double(read<quint8>(stream)) * 0.1);
        // ARC
        if (mainMask.isSet(11))
            map.insert(RE_ARC, double(read<quint8>(stream)) * 0.1);
        // AIC
        if (mainMask.isSet(12))
            map.insert(RE_AIC, double(read<quint8>(stream)) * 0.1);
        // M2
        if (mainMask.isSet(13))
            map.insert(RE_M2, double(read<quint8>(stream)) * 0.1);
        // M1
        if (mainMask.isSet(14))
            map.insert(RE_M1, double(read<quint8>(stream)) * 0.1);
        // ARA
        if (mainMask.isSet(15))
            map.insert(RE_ARA, double(read<quint8>(stream)) * 0.1);
        // VI
        if (mainMask.isSet(16))
            map.insert(RE_VI, double(read<quint8>(stream)) * 0.1);
        // MSG
        if (mainMask.isSet(17))
            map.insert(RE_MSG, double(read<quint8>(stream)) * 0.1);
    }
}

bool Asterix020::generate_re(AsterixData &map, QDataStream &stream)
{
    bool isPA = map.contains(RE_PositionAccuracy_DOP) || map.contains(RE_PositionAccuracy_SDC)
                || map.contains(RE_PositionAccuracy_SDW) || map.contains(RE_PositionAccuracy_SDH);
    bool isGVV = map.contains(RE_GroundSpeed) && map.contains(RE_TrackAngle);
    bool isGVA = map.contains(RE_GroundVelocityAccuracy);
    bool isTRT = map.contains(RE_TimeReportTransmission);
    bool isDA = map.contains(RE_SPI) || map.contains(RE_TI) || map.contains(RE_MBD) ||
                map.contains(RE_M3A) || map.contains(RE_FL) || map.contains(RE_FS) ||
                map.contains(RE_GH) || map.contains(RE_TA) || map.contains(RE_MC) ||
                map.contains(RE_MSS) || map.contains(RE_ARC) || map.contains(RE_AIC) ||
                map.contains(RE_M2) || map.contains(RE_M1) || map.contains(RE_ARA) ||
                map.contains(RE_VI) || map.contains(RE_MSG);

    quint8 len = 1;
    // Calculate length of the field.
    if ( isPA )
    {
        len += map.contains(RE_PositionAccuracy_DOP) ? 6 : 0;
        len += map.contains(RE_PositionAccuracy_SDC) ? 6 : 0;
        len += map.contains(RE_PositionAccuracy_SDW) ? 6 : 0;
        len += map.contains(RE_PositionAccuracy_SDH) ? 2 : 0;
    }
    if ( isGVV ) len += 4;
    if ( isGVA ) len += 2;
    if ( isTRT ) len += 3;
    if ( isDA )
    {
        len += map.contains(RE_SPI) ? 1 : 0;
        len += map.contains(RE_TI) ? 1 : 0;
        if ( map.contains(RE_MBD) )
        {
            TModeSMBDataAgeList mbd = map.value(RE_MBD).toList();
            len += 1 + mbd.size()*2;
        }
        len += map.contains(RE_M3A) ? 1 : 0;
        len += map.contains(RE_FL) ? 1 : 0;
        len += map.contains(RE_FS) ? 1 : 0;
        len += map.contains(RE_GH) ? 1 : 0;
        len += map.contains(RE_TA) ? 1 : 0;
        len += map.contains(RE_MC) ? 1 : 0;
        len += map.contains(RE_MSS) ? 1 : 0;
        len += map.contains(RE_ARC) ? 1 : 0;
        len += map.contains(RE_AIC) ? 1 : 0;
        len += map.contains(RE_M2) ? 1 : 0;
        len += map.contains(RE_M1) ? 1 : 0;
        len += map.contains(RE_ARA) ? 1 : 0;
        len += map.contains(RE_VI) ? 1 : 0;
        len += map.contains(RE_MSG) ? 1 : 0;
    }

    if ( isPA || isGVV || isGVA || isTRT || isDA )
    {
        // Save length of the field.
        write8(stream, len);

        quint8 indicator = 0;
        if ( isPA )
            indicator |= 0x80;
        if ( isGVV )
            indicator |= 0x40;
        if ( isGVA )
            indicator |= 0x20;
        if ( isTRT )
            indicator |= 0x10;
        if ( isDA )
            indicator |= 0x08;

        write8(stream, indicator);

        // Save "Position Accuracy"
        if ( isPA )
        {
            bool isDOP = map.contains(RE_PositionAccuracy_DOP);
            bool isSDC = map.contains(RE_PositionAccuracy_SDC);
            bool isSDW = map.contains(RE_PositionAccuracy_SDW);
            bool isSDH = map.contains(RE_PositionAccuracy_SDH);

            quint8 primary = 0;
            if ( isDOP )
                primary |= 0x80;
            if ( isSDC )
                primary |= 0x40;
            if ( isSDW )
                primary |= 0x20;
            if ( isSDH )
                primary |= 0x10;

            write8(stream, primary);

            // Save "Deviation of Position"
            if ( isDOP )
            {
                SDeviationPosition dp =
                        map.value(RE_PositionAccuracy_DOP).value<SDeviationPosition>();

                write16(stream, qint16(dp.x * 4.));
                write16(stream, qint16(dp.y * 4.));
                write16(stream, qint16(dp.rho * 4.));
            }
            // Save "Standard Deviation of Position (Cartesian)"
            if ( isSDC )
            {
                SDeviationPosition dp =
                        map.value(RE_PositionAccuracy_SDC).value<SDeviationPosition>();

                write16(stream, qint16(dp.x * 4.));
                write16(stream, qint16(dp.y * 4.));
                write16(stream, qint16(dp.rho * 4.));
            }
            // Read "Standard Deviation of Geometric Height (WGS-84)"
            if ( isSDW )
            {
                write16(stream, map.value(RE_PositionAccuracy_SDH).toUInt());
            }
            // Read "Standard Deviation of Position (WGS-84)"
            if ( isSDH )
            {
                SDeviationPosition dp =
                        map.value(RE_PositionAccuracy_SDW).value<SDeviationPosition>();

                write16(stream, qint16(dp.x / 180. * 33554432.));
                write16(stream, qint16(dp.y / 180. * 33554432.));
                write16(stream, qint16(dp.rho / 180. * 33554432.));
            }
        }

        // Save "Ground Velocity Vector"
        if ( isGVV )
        {
            quint16 byte2 = 0;

            SGroundSpeed gs = map.value(RE_GroundSpeed).value<SGroundSpeed>();
            gs.isValueExceedsRange = gs.speed > 0x7FFF;
            byte2 |= quint16(gs.speed) & 0x7FFF;
            byte2 |= gs.isValueExceedsRange ? 0x8000 : 0x0000;

            write16(stream, byte2);
            write16(stream, map.value(RE_TrackAngle).toDouble() / 0.0055);
        }

        // Save "Ground Velocity Accuracy"
        if ( isGVA )
        {
            SGroundVelocityAccuracy gva =
                    map.value(RE_GroundVelocityAccuracy).value<SGroundVelocityAccuracy>();
            gva.groundSpeed = double(read<quint8>(stream)) * 0.00006103515625;
            gva.trackAngle = double(read<quint8>(stream)) * 0.08789;

            write8(stream, gva.groundSpeed / 0.00006103515625);
            write8(stream, gva.trackAngle / 0.08789);
        }

        // Save "Time of ASTERIX Report Transmission"
        if ( isTRT )
        {
            write24(stream, QTime(0,0).msecsTo(
                    map.value(RE_TimeReportTransmission).toTime()) / 7.8125);
        }

        // Read "Data-Ages"
        if ( isDA )
        {
            // SPI
            if ( map.contains(RE_SPI) )
                write8(stream, map.value(RE_SPI).toDouble() * 10.);
            // TI
            if ( map.contains(RE_TI) )
                write8(stream, map.value(RE_TI).toDouble() * 10.);
            // MBD
            if ( map.contains(RE_MBD) )
            {
                TModeSMBDataAgeList data = map.value(RE_MBD).toList();
                write8(stream, data.size());    // rep

                for ( int i = 0; i < data.size(); i++ )
                {
                    SModeSMBDataAge modeMB = data[i].value<SModeSMBDataAge>();
                    quint8 byte = modeMB.BDS2;
                    byte |= modeMB.BDS1 << 4;

                    write8(stream, byte);
                    write8(stream, modeMB.MBA * 10);
                }
            }
            // M3A
            if ( map.contains(RE_M3A) )
                write8(stream, map.value(RE_M3A).toDouble() * 10.);
            // FL
            if ( map.contains(RE_FL) )
                write8(stream, map.value(RE_FL).toDouble() * 10.);
            // FS
            if ( map.contains(RE_FS) )
                write8(stream, map.value(RE_FS).toDouble() * 10.);
            // GH
            if ( map.contains(RE_GH) )
                write8(stream, map.value(RE_GH).toDouble() * 10.);
            // TA
            if ( map.contains(RE_TA) )
                write8(stream, map.value(RE_TA).toDouble() * 10.);
            // MC
            if ( map.contains(RE_MC) )
                write8(stream, map.value(RE_MC).toDouble() * 10.);
            // MSS
            if ( map.contains(RE_MSS) )
                write8(stream, map.value(RE_MSS).toDouble() * 10.);
            // ARC
            if ( map.contains(RE_ARC) )
                write8(stream, map.value(RE_ARC).toDouble() * 10.);
            // AIC
            if ( map.contains(RE_AIC) )
                write8(stream, map.value(RE_AIC).toDouble() * 10.);
            // M2
            if ( map.contains(RE_M2) )
                write8(stream, map.value(RE_M2).toDouble() * 10.);
            // M1
            if ( map.contains(RE_M1) )
                write8(stream, map.value(RE_M1).toDouble() * 10.);
            // ARA
            if ( map.contains(RE_ARA) )
                write8(stream, map.value(RE_ARA).toDouble() * 10.);
            // VI
            if ( map.contains(RE_VI) )
                write8(stream, map.value(RE_VI).toDouble() * 10.);
            // MSG
            if ( map.contains(RE_MSG) )
                write8(stream, map.value(RE_MSG).toDouble() * 10.);
        }
        return true;
    }
    return false;
}

QStringList Asterix020::fields() const
{
    static QStringList result = Asterix020Proto::fields()

            // Reserved Expansion Field: Deviation of Position
            << RE_PositionAccuracy_DOP
            // Reserved Expansion Field: Standard Deviation of Position (Cartesian)
            << RE_PositionAccuracy_SDC
            // Reserved Expansion Field: Standard Deviation of Position (WGS-84)
            << RE_PositionAccuracy_SDW
            // Reserved Expansion Field: Standard Deviation of Geometric Height (WGS-84)
            << RE_PositionAccuracy_SDH
            // Reserved Expansion Field: Ground Velocity Vector
            << RE_GroundSpeed
            // Reserved Expansion Field: Track Angle clockwise reference to “True North”
            << RE_TrackAngle
            // Reserved Expansion Field: Track Angle clockwise reference to “True North”
            << RE_GroundVelocityAccuracy
            // Reserved Expansion Field: Time of ASTERIX Report Transmission
            << RE_TimeReportTransmission
            // Reserved Expansion Field: Special Position Identification Age
            << RE_SPI
            // Reserved Expansion Field: Target Identification age
            << RE_TI
            // Reserved Expansion Field: Mode S MB age
            << RE_MBD
            // Reserved Expansion Field: Mode S MB age
            << RE_M3A
            // Reserved Expansion Field: Flight Level age
            << RE_FL
            // Reserved Expansion Field: Flight Status age
            << RE_FS
            // Reserved Expansion Field: Geometric / Measured Height age
            << RE_GH
            // Reserved Expansion Field: Target Address age
            << RE_TA
            // Reserved Expansion Field: Mode C code age
            << RE_MC
            // Reserved Expansion Field: Mode-S Specific Service Capability age
            << RE_MSS
            // Reserved Expansion Field: Altitude reporting capability age
            << RE_ARC
            // Reserved Expansion Field: Aircraft identification capability age
            << RE_AIC
            // Reserved Expansion Field: Mode-2 Code age
            << RE_M2
            // Reserved Expansion Field: Mode-1 Code age
            << RE_M1
            // Reserved Expansion Field: ACAS Resolution Advisory age
            << RE_ARA
            // Reserved Expansion Field: Vehicle Fleet Identification age
            << RE_VI
            // Reserved Expansion Field: Pre-programmed message age
            << RE_MSG;

    return result;
}

}   // Eurocontrol
