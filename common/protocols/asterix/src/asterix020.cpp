#include "asterix020.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QTime>
#include <math.h>


const int Asterix020Proto::mCategory = 20;
const QString Asterix020Proto::mEdition = "1.8";

// Data Item I021/010, Data Source Identification
const QString Asterix020Proto::DataSourceIdentification_SAC = ASTERIX_SAC;
const QString Asterix020Proto::DataSourceIdentification_SIC = ASTERIX_SIC;

// Data Item I020/020, Target Report Descriptor
const QString Asterix020Proto::TargetReportDescriptor = "a020.TargetReportDescriptor";

// Data Item I020/030, Warning/Error Conditions
const QString Asterix020Proto::WarningErrorConditions = "a020.WarningErrorConditions";

// Data Item I020/041, Position in WGS-84 Coordinates
const QString Asterix020Proto::PositionInWGS84Coordinates_Latitude = "a020.PositionInWGS84Coordinates_Latitude";
const QString Asterix020Proto::PositionInWGS84Coordinates_Longitude = "a020.PositionInWGS84Coordinates_Longitude";

// Data Item I020/042, Position in Cartesian Coordinates
const QString Asterix020Proto::PositionInCartesianCoordinates_X = "a020.PositionInCartesianCoordinates_X";
const QString Asterix020Proto::PositionInCartesianCoordinates_Y = "a020.PositionInCartesianCoordinates_Y";

// Data Item I020/050, Mode-2 Code in Octal Representation
const QString Asterix020Proto::Mode2CodeInOctalRepresentation = "a020.Mode2CodeInOctalRepresentation";

// Data Item I020/055, Mode-1 Code in Octal Representation
const QString Asterix020Proto::Mode1CodeInOctalRepresentation = "a020.Mode1CodeInOctalRepresentation";

// Data Item I020/070, Mode-3/A Code in Octal Representation
const QString Asterix020Proto::Mode3ACodeInOctalRepresentation = "a020.Mode3ACodeInOctalRepresentation";

// Data Item I020/090, Flight Level in Binary Representation
const QString Asterix020Proto::FlightLevel = "a020.FlightLevel";

// Data Item I020/100, Mode-C Code
const QString Asterix020Proto::ModeCCode = "a020.ModeCCode";

// Data Item I020/105, Geometric Height (WGS-84)
const QString Asterix020Proto::GeometricHeight = "a020.GeometricHeight";

// Data Item I020/110, Measured Height (Local Cartesian Coordinates)
const QString Asterix020Proto::MeasuredHeight = "a020.MeasuredHeight";

// Data Item I020/140, Time of Day
const QString Asterix020Proto::TimeOfDay = "a020.TimeOfDay";

// Data Item I020/161, Track Number
const QString Asterix020Proto::TrackNumber = "a020.TrackNumber";

// Data Item I020/170, Track Status
const QString Asterix020Proto::TrackStatus = "a020.TrackStatus";

// Data Item I020/202, Calculated Track Velocity in Cartesian Coordinates
const QString Asterix020Proto::CalculatedTrackVelocity_Vx = "a020.CalculatedTrackVelocity_Vx";
const QString Asterix020Proto::CalculatedTrackVelocity_Vy = "a020.CalculatedTrackVelocity_Vy";

// Data Item I020/210, Calculated Acceleration
const QString Asterix020Proto::CalculatedAcceleration_Ax = "a020.CalculatedAcceleration_Ax";
const QString Asterix020Proto::CalculatedAcceleration_Ay = "a020.CalculatedAcceleration_Ay";

// Data Item I020/220, Target Address
const QString Asterix020Proto::TargetAddress = "a020.TargetAddress";

// Data Item I020/230, Communications/ACAS Capability and Flight Status
const QString Asterix020Proto::CommunicationsACASCapabilityAndFlightStatus = "a020.CommunicationsACASCapabilityAndFlightStatus";

// Data Item I020/245, Target Identification
const QString Asterix020Proto::TargetIdentification = "a020.TargetIdentification";

// Data Item I020/250, Mode S MB Data
const QString Asterix020Proto::ModeSMBData = "a020.ModeSMBData";

// Data Item I020/260, ACAS Resolution Advisory Report
const QString Asterix020Proto::ACASResolutionAdvisoryReport = "a020.ACASResolutionAdvisoryReport";

// Data Item I020/300, Vehicle Fleet Identification
const QString Asterix020Proto::VehicleFleetIdentification = "a020.VehicleFleetIdentification";

// Data Item I020/310, Pre-programmed Message
const QString Asterix020Proto::PreProgrammedMessage = "a020.PreProgrammedMessage";

// Data Item I020/400, Contributing Receivers
const QString Asterix020Proto::ContributingReceivers = "a020.ContributingReceivers";

// Data Item I020/500, Position Accuracy
// Deviation of Position
const QString Asterix020Proto::PositionAccuracy_DOP = "a020.PositionAccuracy_DOP";
// Standard Deviation of Position
const QString Asterix020Proto::PositionAccuracy_SDP = "a020.PositionAccuracy_SDP";
// Standard Deviation of Geometric Height
const QString Asterix020Proto::PositionAccuracy_SDH = "a020.PositionAccuracy_SDH";


Asterix020Proto::Asterix020Proto()
{}

int Asterix020Proto::category() const
{
    return mCategory;
}

QString Asterix020Proto::edition() const
{
    return mEdition;
}

void Asterix020Proto::parse(QDataStream &stream, AsterixData &map)
{
    try
    {
        PresentData mainMask(stream);

        if (mainMask.isSet(1))
            parse_010(stream, map);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (mainMask.isSet(2))
            parse_020(stream, map);
        else
            map.appendError(mandatory("Target Report Descriptor"));

        if (mainMask.isSet(3))
            parse_140(stream, map);
        else
            map.appendError(mandatory("Time of Day"));

        if (mainMask.isSet(4))
            parse_041(stream, map);

        if (mainMask.isSet(5))
            parse_042(stream, map);

        if (!mainMask.isSet(4) && !mainMask.isSet(5))
            map.appendError(mandatory("Position in WGS-84 Coordinates or Position in Cartesian Coordinates"));

        if (mainMask.isSet(6))
            parse_161(stream, map);

        if (mainMask.isSet(7))
            parse_170(stream, map);

        if (mainMask.isSet(8))
            parse_070(stream, map);

        if (mainMask.isSet(9))
            parse_202(stream, map);

        if (mainMask.isSet(10))
            parse_090(stream, map);

        if (mainMask.isSet(11))
            parse_100(stream, map);

        if (mainMask.isSet(12))
            parse_220(stream, map);

        if (mainMask.isSet(13))
            parse_245(stream, map);

        if (mainMask.isSet(14))
            parse_110(stream, map);

        if (mainMask.isSet(15))
            parse_105(stream, map);

        if (mainMask.isSet(16))
            parse_210(stream, map);

        if (mainMask.isSet(17))
            parse_300(stream, map);

        if (mainMask.isSet(18))
            parse_310(stream, map);

        if (mainMask.isSet(19))
            parse_500(stream, map);

        if (mainMask.isSet(20))
            parse_400(stream, map);

        if (mainMask.isSet(21))
            parse_250(stream, map);

        if (mainMask.isSet(22))
            parse_230(stream, map);

        if (mainMask.isSet(23))
            parse_260(stream, map);

        if (mainMask.isSet(24))
            parse_030(stream, map);

        if (mainMask.isSet(25))
            parse_055(stream, map);

        if (mainMask.isSet(26))
            parse_050(stream, map);

        if (mainMask.isSet(27))
            parse_re(stream, map);

        if (mainMask.isSet(28))
            parse_sp(stream, map);
    }
    catch (AsterixErrors::ErrorCode error)
    {
        map.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        map.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            map.appendError(AsterixErrors::Error_UnexpectedEndOfData);
    }
}

QByteArray Asterix020Proto::generate(AsterixData &map)
{
    QByteArray result;
    PresentData mainMask;
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        if (generate_010(map, stream))
            mainMask.set(1);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (generate_020(map, stream))
            mainMask.set(2);
        else
            map.appendError(mandatory("Target Report Descriptor"));

        if (generate_140(map, stream))
            mainMask.set(3);
        else
            map.appendError(mandatory("Time of Day"));

        bool is041 = generate_041(map, stream);
        if (is041)
            mainMask.set(4);

        bool is042 = generate_042(map, stream);
        if (is042)
            mainMask.set(5);

        if (!is041 && !is042)
            map.appendError(mandatory("Position in WGS-84 Coordinates or Position in Cartesian Coordinates"));

        if (generate_161(map, stream))
            mainMask.set(6);

        if (generate_170(map, stream))
            mainMask.set(7);

        if (generate_070(map, stream))
            mainMask.set(8);

        if (generate_202(map, stream))
            mainMask.set(9);

        if (generate_090(map, stream))
            mainMask.set(10);

        if (generate_100(map, stream))
            mainMask.set(11);

        if (generate_220(map, stream))
            mainMask.set(12);

        if (generate_245(map, stream))
            mainMask.set(13);

        if (generate_110(map, stream))
            mainMask.set(14);

        if (generate_105(map, stream))
            mainMask.set(15);

        if (generate_210(map, stream))
            mainMask.set(16);

        if (generate_300(map, stream))
            mainMask.set(17);

        if (generate_310(map, stream))
            mainMask.set(18);

        if (generate_500(map, stream))
            mainMask.set(19);

        if (generate_400(map, stream))
            mainMask.set(20);

        if (generate_250(map, stream))
            mainMask.set(21);

        if (generate_230(map, stream))
            mainMask.set(22);

        if (generate_260(map, stream))
            mainMask.set(23);

        if (generate_030(map, stream))
            mainMask.set(24);

        if (generate_055(map, stream))
            mainMask.set(25);

        if (generate_050(map, stream))
            mainMask.set(26);

        if (generate_re(map, stream))
            mainMask.set(48);

        if (generate_sp(map, stream))
            mainMask.set(49);
    }
    catch (AsterixErrors::ErrorCode error)
    {
        map.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        map.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            map.appendError(AsterixErrors::Error_UnexpectedEndOfData);
    }
    return result.prepend(mainMask.data());
}

void Asterix020Proto::parse_010(QDataStream &stream, AsterixData &map)
{
    map.insert(DataSourceIdentification_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentification_SIC, read<quint8>(stream));
}

bool Asterix020Proto::generate_010(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, DataSourceIdentification_SAC, DataSourceIdentification_SIC))
    {
        write8(stream, map.value(DataSourceIdentification_SAC).toUInt());
        write8(stream, map.value(DataSourceIdentification_SIC).toUInt());
        return true;
    }
    return false;
}

void Asterix020Proto::parse_020(QDataStream &stream, AsterixData &map)
{
    STargetReportDescriptor trd;
    trd.part = read<quint8>(stream);
    if ( trd.part & 0x01 )
    {
        quint8 byte = read<quint8>(stream);
        trd.extents.append(byte);
    }
    map.insert(TargetReportDescriptor, QVariant::fromValue(trd));
}

bool Asterix020Proto::generate_020(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TargetReportDescriptor))
    {
        STargetReportDescriptor trd = map.value(TargetReportDescriptor).value<STargetReportDescriptor>();
        write8(stream, trd.part);
        if ( !trd.extents.empty() )
            write8(stream, trd.extents[0]);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_030(QDataStream &stream, AsterixData &map)
{
    map.insert(WarningErrorConditions, read<quint8>(stream));
}

bool Asterix020Proto::generate_030(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, WarningErrorConditions))
    {
        write8(stream, map.value(WarningErrorConditions).toUInt());
        return true;
    }
    return false;
}

void Asterix020Proto::parse_041(QDataStream &stream, AsterixData &map)
{
    map.insert(PositionInWGS84Coordinates_Latitude, double(read<qint32>(stream)) * 180 / 33554432);
    map.insert(PositionInWGS84Coordinates_Longitude, double(read<qint32>(stream)) * 180 / 33554432);
}

bool Asterix020Proto::generate_041(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, PositionInWGS84Coordinates_Latitude,
                  PositionInWGS84Coordinates_Longitude))
    {
        write32(stream, map.value(PositionInWGS84Coordinates_Latitude).toDouble() * 33554432 / 180);
        write32(stream, map.value(PositionInWGS84Coordinates_Longitude).toDouble() * 33554432 / 180);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_042(QDataStream &stream, AsterixData &map)
{
    map.insert(PositionInCartesianCoordinates_X, double(read24(stream)) / 2.);
    map.insert(PositionInCartesianCoordinates_Y, double(read24(stream)) / 2.);
}

bool Asterix020Proto::generate_042(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, PositionInCartesianCoordinates_X,
                  PositionInCartesianCoordinates_Y))
    {
        write24(stream, map.value(PositionInCartesianCoordinates_X).toDouble() * 2);
        write24(stream, map.value(PositionInCartesianCoordinates_Y).toDouble() * 2);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_050(QDataStream &stream, AsterixData &map)
{
    SMode23ACodeInOctalRepresentation mode2;
    mode2.code = read<quint16>(stream);
    map.insert(Mode2CodeInOctalRepresentation, QVariant::fromValue(mode2));
}

bool Asterix020Proto::generate_050(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, Mode2CodeInOctalRepresentation))
    {
        SMode23ACodeInOctalRepresentation mode2 =
                map.value(Mode2CodeInOctalRepresentation).value<SMode23ACodeInOctalRepresentation>();
        write16(stream, mode2.code);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_055(QDataStream &stream, AsterixData &map)
{
    SMode1CodeInOctalRepresentation mode1;
    mode1.code = read<quint8>(stream);
    map.insert(Mode1CodeInOctalRepresentation, QVariant::fromValue(mode1));
}

bool Asterix020Proto::generate_055(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, Mode1CodeInOctalRepresentation))
    {
        SMode1CodeInOctalRepresentation mode1 =
                map.value(Mode1CodeInOctalRepresentation).value<SMode1CodeInOctalRepresentation>();
        write8(stream, mode1.code);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_070(QDataStream &stream, AsterixData &map)
{
    SMode23ACodeInOctalRepresentation mode3A;
    mode3A.code = read<quint16>(stream);
    map.insert(Mode3ACodeInOctalRepresentation, QVariant::fromValue(mode3A));
}

bool Asterix020Proto::generate_070(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, Mode3ACodeInOctalRepresentation))
    {
        SMode23ACodeInOctalRepresentation mode3A =
                map.value(Mode3ACodeInOctalRepresentation).value<SMode23ACodeInOctalRepresentation>();
        write16(stream, mode3A.code);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_090(QDataStream &stream, AsterixData &map)
{
    SFlightLevel flightLevel;
    flightLevel.code = read<quint16>(stream);
    map.insert(FlightLevel, QVariant::fromValue(flightLevel));
}

bool Asterix020Proto::generate_090(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, FlightLevel))
    {
        SFlightLevel flightLevel = map.value(FlightLevel).value<SFlightLevel>();
        write16(stream, flightLevel.code);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_100(QDataStream &stream, AsterixData &map)
{
    SModeCCode modeC;
    modeC.code = read<quint32>(stream);
    map.insert(ModeCCode, QVariant::fromValue(modeC));
}

bool Asterix020Proto::generate_100(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ModeCCode))
    {
        SModeCCode modeC = map.value(ModeCCode).value<SModeCCode>();
        write32(stream, modeC.code);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_105(QDataStream &stream, AsterixData &map)
{
    map.insert(GeometricHeight, double(read<qint16>(stream)) * 6.25);
}

bool Asterix020Proto::generate_105(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, GeometricHeight))
    {
        write16(stream, map.value(GeometricHeight).toDouble() / 6.25);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_110(QDataStream &stream, AsterixData &map)
{
    map.insert(MeasuredHeight, double(read<qint16>(stream)) * 6.25);
}

bool Asterix020Proto::generate_110(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, MeasuredHeight))
    {
        write16(stream, qint16(map.value(MeasuredHeight).toDouble() / 6.25));
        return true;
    }
    return false;
}

void Asterix020Proto::parse_140(QDataStream &stream, AsterixData &map)
{
    map.insert(TimeOfDay, QTime().addMSecs(ceil(double(read24(stream)) * 7.8125)));
}

bool Asterix020Proto::generate_140(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TimeOfDay))
    {
        write24(stream, double(QTime(0,0).msecsTo(map.value(TimeOfDay).toTime())) / 7.8125);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_161(QDataStream &stream, AsterixData &map)
{
    quint16 trackNumber = read<quint16>(stream) & 0x0FFF;
    map.insert(TrackNumber, trackNumber);
}

bool Asterix020Proto::generate_161(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TrackNumber))
    {
        write16(stream, map.value(TrackNumber).toUInt() & 0x0FFF);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_170(QDataStream &stream, AsterixData &map)
{
    STrackStatus ts;
    ts.part = read<quint8>(stream);
    if ( ts.part & 0x01 )
        ts.extents.append( read<quint8>(stream) );
    map.insert(TrackStatus, QVariant::fromValue(ts));
}

bool Asterix020Proto::generate_170(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TrackStatus))
    {
        STrackStatus ts = map.value(TrackStatus).value<STrackStatus>();
        write8(stream, ts.part);
        if ( !ts.extents.empty() )
            write8(stream, ts.extents[0]);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_202(QDataStream &stream, AsterixData &map)
{
    map.insert(CalculatedTrackVelocity_Vx, double(read<qint16>(stream)) / 4.);
    map.insert(CalculatedTrackVelocity_Vy, double(read<qint16>(stream)) / 4.);
}

bool Asterix020Proto::generate_202(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, CalculatedTrackVelocity_Vx, CalculatedTrackVelocity_Vy))
    {
        write16(stream, qint16(map.value(CalculatedTrackVelocity_Vx).toDouble() * 4.));
        write16(stream, qint16(map.value(CalculatedTrackVelocity_Vy).toDouble() * 4.));
        return true;
    }
    return false;
}

void Asterix020Proto::parse_210(QDataStream &stream, AsterixData &map)
{
    map.insert(CalculatedAcceleration_Ax, double(read<qint8>(stream)) / 4.);
    map.insert(CalculatedAcceleration_Ay, double(read<qint8>(stream)) / 4.);
}

bool Asterix020Proto::generate_210(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, CalculatedAcceleration_Ax, CalculatedAcceleration_Ay))
    {
        write8(stream, qint8(map.value(CalculatedAcceleration_Ax).toDouble() * 4));
        write8(stream, qint8(map.value(CalculatedAcceleration_Ay).toDouble() * 4));
        return true;
    }
    return false;
}

void Asterix020Proto::parse_220(QDataStream &stream, AsterixData &map)
{
    map.insert(TargetAddress, read24(stream));
}

bool Asterix020Proto::generate_220(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TargetAddress))
    {
        write24(stream, map.value(TargetAddress).toUInt());
        return true;
    }
    return false;
}

void Asterix020Proto::parse_230(QDataStream &stream, AsterixData &map)
{
    SCommunicationsACASCapabilityAndFlightStatus status;
    status.item = read<quint16>(stream);
    map.insert(CommunicationsACASCapabilityAndFlightStatus, QVariant::fromValue(status));
}

bool Asterix020Proto::generate_230(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, CommunicationsACASCapabilityAndFlightStatus))
    {
        SCommunicationsACASCapabilityAndFlightStatus status =
                map.value(CommunicationsACASCapabilityAndFlightStatus).value<SCommunicationsACASCapabilityAndFlightStatus>();
        write16(stream, status.item);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_245(QDataStream &stream, AsterixData &map)
{
    STargetIdentification ti;
    ti.sti = STargetIdentification::STI((read<quint8>(stream) >> 6) & 0x3);
    QByteArray buffer = readCharacters(stream, 2);
    memcpy(ti.characters, buffer.data(), 8);

    map.insert(TargetIdentification, QVariant::fromValue(ti));
}

bool Asterix020Proto::generate_245(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TargetIdentification))
    {
        STargetIdentification ti =
                map.value(TargetIdentification).value<STargetIdentification>();
        QByteArray buffer(8, Qt::Uninitialized);
        memcpy(buffer.data(), ti.characters, 8);

        write8(stream, quint8(ti.sti) << 6);
        writeCharacters(stream, buffer);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_250(QDataStream &stream, AsterixData &map)
{
    TModeSMBDataList data;
    quint8 rep = read<quint8>(stream);
    for ( int i = 0; i < rep; i++ )
    {
        SModeSMBData modeS;
        QByteArray buffer = readArray(stream, 7);
        memcpy(modeS.data, buffer.data(), 7);

        quint8 bds = read<quint8>(stream);
        modeS.BDS1 = bds >> 4;
        modeS.BDS2 = bds & 0x0F;

        data.append( QVariant::fromValue(modeS) );
    }
    map.insert(ModeSMBData, data);
}

bool Asterix020Proto::generate_250(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ModeSMBData))
    {
        TModeSMBDataList data = map.value(ModeSMBData).toList();

        write8(stream, data.size());

        foreach ( const QVariant &v, data )
        {
            SModeSMBData modeS = v.value<SModeSMBData>();
            QByteArray buffer(7, Qt::Uninitialized);
            memcpy(buffer.data(), modeS.data, 7);

            quint8 bds = modeS.BDS2;
            bds |= modeS.BDS1 << 4;

            writeCharacters(stream, buffer);
            write8(stream, bds);
        }
        return true;
    }
    return false;
}

void Asterix020Proto::parse_260(QDataStream &stream, AsterixData &map)
{
    map.insert(ACASResolutionAdvisoryReport, readArray(stream, 7));
}

bool Asterix020Proto::generate_260(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ACASResolutionAdvisoryReport))
    {
        writeArray(stream, map.value(ACASResolutionAdvisoryReport).toByteArray());
        return true;
    }
    return false;
}

void Asterix020Proto::parse_300(QDataStream &stream, AsterixData &map)
{
    map.insert(VehicleFleetIdentification, read<quint8>(stream));
}

bool Asterix020Proto::generate_300(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, VehicleFleetIdentification))
    {
        write8(stream, map.value(VehicleFleetIdentification).toUInt());
        return true;
    }
    return false;
}

void Asterix020Proto::parse_310(QDataStream &stream, AsterixData &map)
{
    SPreProgrammedMessage msg;
    msg.item = read<quint8>(stream);
    map.insert(PreProgrammedMessage, QVariant::fromValue(msg));
}

bool Asterix020Proto::generate_310(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, PreProgrammedMessage))
    {
        SPreProgrammedMessage msg = map.value(PreProgrammedMessage).value<SPreProgrammedMessage>();
        write8(stream, msg.item);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_400(QDataStream &stream, AsterixData &map)
{
    quint8 rep = read<quint8>(stream);

    QByteArray buffer(rep, Qt::Uninitialized);
    for ( int i = 0; i < rep; i++ )
        buffer[i] = read<quint8>(stream);

    map.insert(ContributingReceivers, buffer);
}

bool Asterix020Proto::generate_400(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ContributingReceivers))
    {
        QByteArray buffer = map.value(ContributingReceivers).toByteArray();

        write8(stream, buffer.size());
        for ( int i = 0; i < buffer.size(); i++ )
            write8(stream, buffer[i]);
        return true;
    }
    return false;
}

void Asterix020Proto::parse_500(QDataStream &stream, AsterixData &map)
{
    quint8 primary = read<quint8>(stream);

    // Read DOP
    if ( primary & 0x80 )
    {
        SDeviationPosition dp;
        dp.x = double(read<qint16>(stream)) / 4.;
        dp.y = double(read<qint16>(stream)) / 4.;
        dp.rho = double(read<qint16>(stream)) / 4.;

        map.insert(PositionAccuracy_DOP, QVariant::fromValue(dp));
    }
    // Read SDP
    if ( primary & 0x40 )
    {
        SDeviationPosition dp;
        dp.x = double(read<qint16>(stream)) / 4.;
        dp.y = double(read<qint16>(stream)) / 4.;
        dp.rho = double(read<qint16>(stream)) / 4.;

        map.insert(PositionAccuracy_SDP, QVariant::fromValue(dp));
    }
    // Read SDH
    if ( primary & 0x20 )
    {
        map.insert(PositionAccuracy_SDH, double(read<qint16>(stream)) / 2.);
    }
}

bool Asterix020Proto::generate_500(AsterixData &map, QDataStream &stream)
{
    bool isPositionAccuracy_DOP = map.contains(PositionAccuracy_DOP);
    bool isPositionAccuracy_SDP = map.contains(PositionAccuracy_SDP);
    bool isPositionAccuracy_SDH = map.contains(PositionAccuracy_SDH);

    if (isPositionAccuracy_DOP || isPositionAccuracy_SDP || isPositionAccuracy_SDH)
    {
        quint8 primary = 0;
        if ( isPositionAccuracy_DOP )
            primary |= 0x80;
        if ( isPositionAccuracy_SDP )
            primary |= 0x40;
        if ( isPositionAccuracy_SDH )
            primary |= 0x20;

        write8(stream, primary);

        // Save DOP
        if ( isPositionAccuracy_DOP )
        {
            SDeviationPosition dp = map.value(PositionAccuracy_DOP).value<SDeviationPosition>();

            write16(stream, qint16(dp.x * 4.));
            write16(stream, qint16(dp.y * 4.));
            write16(stream, qint16(dp.rho * 4.));
        }
        // Save SDP
        if ( isPositionAccuracy_SDP )
        {
            SDeviationPosition dp = map.value(PositionAccuracy_SDP).value<SDeviationPosition>();

            write16(stream, qint16(dp.x * 4.));
            write16(stream, qint16(dp.y * 4.));
            write16(stream, qint16(dp.rho * 4.));
        }
        // Save SDH
        if ( isPositionAccuracy_SDH )
        {
            write16(stream, qint16(map.value(PositionAccuracy_SDH).toDouble() * 2.));
        }

        return true;
    }
    return false;
}

QStringList Asterix020Proto::fields() const
{
    static QStringList result = QStringList()

            // Data Item I020/010, Data Source Identification
            << DataSourceIdentification_SAC
            << DataSourceIdentification_SIC

            // Data Item I020/020, Target Report Descriptor
            << TargetReportDescriptor

            // Data Item I020/030, Warning/Error Conditions
            << WarningErrorConditions

            // Data Item I020/041, Position in WGS-84 Coordinates
            << PositionInWGS84Coordinates_Latitude
            << PositionInWGS84Coordinates_Longitude

            // Data Item I020/042, Position in Cartesian Coordinates
            << PositionInCartesianCoordinates_X
            << PositionInCartesianCoordinates_Y

            // Data Item I020/050, Mode-2 Code in Octal Representation
            << Mode2CodeInOctalRepresentation

            // Data Item I020/055, Mode-1 Code in Octal Representation
            << Mode1CodeInOctalRepresentation

            // Data Item I020/070, Mode-3/A Code in Octal Representation
            << Mode3ACodeInOctalRepresentation

            // Data Item I020/090, Flight Level in Binary Representation
            << FlightLevel

            // Data Item I020/100, Mode-C Code
            << ModeCCode

            // Data Item I020/105, Geometric Height (WGS-84)
            << GeometricHeight

            // Data Item I020/110, Measured Height (Local Cartesian Coordinates)
            << MeasuredHeight

            // Data Item I020/140, Time of Day
            << TimeOfDay

            // Data Item I020/161, Track Number
            << TrackNumber

            // Data Item I020/170, Track Status
            << TrackStatus

            // Data Item I020/202, Calculated Track Velocity in Cartesian Coordinates
            << CalculatedTrackVelocity_Vx
            << CalculatedTrackVelocity_Vy

            // Data Item I020/210, Calculated Acceleration
            << CalculatedAcceleration_Ax
            << CalculatedAcceleration_Ay

            // Data Item I020/220, Target Address
            << TargetAddress

            // Data Item I020/230, Communications/ACAS Capability and Flight Status
            << CommunicationsACASCapabilityAndFlightStatus

            // Data Item I020/245, Target Identification
            << TargetIdentification

            // Data Item I020/250, Mode S MB Data
            << ModeSMBData

            // Data Item I020/260, ACAS Resolution Advisory Report
            << ACASResolutionAdvisoryReport

            // Data Item I020/300, Vehicle Fleet Identification
            << VehicleFleetIdentification

            // Data Item I020/310, Pre-programmed Message
            << PreProgrammedMessage

            // Data Item I020/400, Contributing Receivers
            << ContributingReceivers

            // Data Item I020/500, Position Accuracy
            << PositionAccuracy_DOP
            << PositionAccuracy_SDP
            << PositionAccuracy_SDH;

    return result;
}
