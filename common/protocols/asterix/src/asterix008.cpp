#include "asterix008.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>
#include <QtCore/qmath.h>

#include <math.h>


const int Asterix008Proto::mCategory = 8;
const QString Asterix008Proto::mEdition = "1.1";

qint8 Asterix008Proto::scaleFactor = 0;

const QString Asterix008Proto::MessageType = "a008.MessageType";

const QString Asterix008Proto::DataSourceIdentifier_SAC = ASTERIX_SAC;
const QString Asterix008Proto::DataSourceIdentifier_SIC = ASTERIX_SIC;

const QString Asterix008Proto::VectorQualifier = "a008.VectorQualifier";

const QString Asterix008Proto::SequencePolarVectors = "a008.SequencePolarVectors";

const QString Asterix008Proto::SequenceCartesianVectors = "a008.SequenceCartesianVectors";

const QString Asterix008Proto::SequenceWeatherVectors = "a008.SequenceWeatherVectors";

const QString Asterix008Proto::ContourIdentifier = "a008.ContourIdentifier";

const QString Asterix008Proto::SequenceContourPoints = "a008.SequenceContourPoints";

const QString Asterix008Proto::TimeOfDay = "a008.TimeOfDay";

const QString Asterix008Proto::ProcessingStatus = "a008.ProcessingStatus";

const QString Asterix008Proto::StationConfigurationStatus = "a008.StationConfigurationStatus";

const QString Asterix008Proto::TotalNumberItems = "a008.TotalNumberItems";

Asterix008Proto::Asterix008Proto()
{}

int Asterix008Proto::category() const
{
    return mCategory;
}

QString Asterix008Proto::edition() const
{
    return mEdition;
}

void Asterix008Proto::parse(QDataStream &stream, AsterixData &map)
{
    try
    {
        PresentData mainMask(stream);

        if (mainMask.isSet(1))
            parse_010(stream, map);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (mainMask.isSet(2))
            parse_000(stream, map);
        else
            throw mandatory("Message Type");

        EMessageType messageType = (EMessageType) map.value(MessageType).toUInt();

        // SOP message restrictions.
        if ( messageType == SOPMessage )
        {
            if (mainMask.isSet(3))
                parse_020(stream, map);

            if (mainMask.isSet(4))
                parse_036(stream, map);

            if (mainMask.isSet(5))
                parse_034(stream, map);

            if (mainMask.isSet(6))
                parse_040(stream, map);

            if (mainMask.isSet(7))
                parse_050(stream, map);

            if (mainMask.isSet(8))
                parse_090(stream, map);
            else
                map.appendError(mandatory("Time of Day"));

            if (mainMask.isSet(9))
                parse_100(stream, map);
            else
                map.appendError(mandatory("Processing Status"));

            if (mainMask.isSet(10))
                parse_110(stream, map);
            else
                map.appendError(mandatory("Station Configuration Status"));

            if (mainMask.isSet(11))
                parse_120(stream, map);

            if (mainMask.isSet(12))
                parse_038(stream, map);
        }
        // EOP message restrictions.
        else if ( messageType == EOPMessage )
        {
            if (mainMask.isSet(3))
                parse_020(stream, map);

            if (mainMask.isSet(4))
                parse_036(stream, map);

            if (mainMask.isSet(5))
                parse_034(stream, map);

            if (mainMask.isSet(6))
                parse_040(stream, map);

            if (mainMask.isSet(7))
                parse_050(stream, map);

            if (mainMask.isSet(8))
                parse_090(stream, map);
            else
                map.appendError(mandatory("Time of Day"));

            if (mainMask.isSet(9))
                parse_100(stream, map);

            if (mainMask.isSet(10))
                parse_110(stream, map);
            else
                map.appendError(mandatory("Station Configuration Status"));

            if (mainMask.isSet(11))
                parse_120(stream, map);
            else
                map.appendError(mandatory("Total number of Items"));

            if (mainMask.isSet(12))
                parse_038(stream, map);
        }
        // Cartesian vector restrictions.
        else if ( messageType == CartesianVector )
        {
            if (mainMask.isSet(3))
                parse_020(stream, map);
            else
                map.appendError(mandatory("Vector Qualifier"));

            if (mainMask.isSet(4))
                parse_036(stream, map);
            else
                map.appendError(mandatory("Sequence of Cartesian Vectors"));

            if (mainMask.isSet(5))
                parse_034(stream, map);

            if (mainMask.isSet(6))
                parse_040(stream, map);

            if (mainMask.isSet(7))
                parse_050(stream, map);

            if (mainMask.isSet(8))
                parse_090(stream, map);

            if (mainMask.isSet(9))
                parse_100(stream, map);

            if (mainMask.isSet(10))
                parse_110(stream, map);

            if (mainMask.isSet(11))
                parse_120(stream, map);

            if (mainMask.isSet(12))
                parse_038(stream, map);
        }
        // Weather vector restrictions.
        else if ( messageType == WeatherVector )
        {
            if (mainMask.isSet(3))
                parse_020(stream, map);
            else
                map.appendError(mandatory("Vector Qualifier"));

            if (mainMask.isSet(4))
                parse_036(stream, map);

            if (mainMask.isSet(5))
                parse_034(stream, map);

            if (mainMask.isSet(6))
                parse_040(stream, map);

            if (mainMask.isSet(7))
                parse_050(stream, map);

            if (mainMask.isSet(8))
                parse_090(stream, map);

            if (mainMask.isSet(9))
                parse_100(stream, map);

            if (mainMask.isSet(10))
                parse_110(stream, map);

            if (mainMask.isSet(11))
                parse_120(stream, map);

            if (mainMask.isSet(12))
                parse_038(stream, map);
            else
                map.appendError(mandatory("Sequence of Weather Vectors"));
        }
        // Polar vector restrictions.
        else if ( messageType == PolarVector )
        {
            if (mainMask.isSet(3))
                parse_020(stream, map);
            else
                map.appendError(mandatory("Vector Qualifier"));

            if (mainMask.isSet(4))
                parse_036(stream, map);

            if (mainMask.isSet(5))
                parse_034(stream, map);
            else
                map.appendError(mandatory("Sequence of Polar Vectors"));

            if (mainMask.isSet(6))
                parse_040(stream, map);

            if (mainMask.isSet(7))
                parse_050(stream, map);

            if (mainMask.isSet(8))
                parse_090(stream, map);

            if (mainMask.isSet(9))
                parse_100(stream, map);

            if (mainMask.isSet(10))
                parse_110(stream, map);

            if (mainMask.isSet(11))
                parse_120(stream, map);

            if (mainMask.isSet(12))
                parse_038(stream, map);
        }
        // Contour points restrictions.
        else if ( messageType == ContourPoints )
        {
            if (mainMask.isSet(3))
                parse_020(stream, map);

            if (mainMask.isSet(4))
                parse_036(stream, map);

            if (mainMask.isSet(5))
                parse_034(stream, map);

            if (mainMask.isSet(6))
                parse_040(stream, map);
            else
                map.appendError(mandatory("Contour Identifier"));

            if (mainMask.isSet(7))
                parse_050(stream, map);
            else
                map.appendError(mandatory("Sequence of Contour Points"));

            if (mainMask.isSet(8))
                parse_090(stream, map);

            if (mainMask.isSet(9))
                parse_100(stream, map);

            if (mainMask.isSet(10))
                parse_110(stream, map);

            if (mainMask.isSet(11))
                parse_120(stream, map);

            if (mainMask.isSet(12))
                parse_038(stream, map);
        }
        else
            throw AsterixErrors::Error_InvalidInput;

        if (mainMask.isSet(13))
            parse_re(stream, map);

        if (mainMask.isSet(14))
            parse_re(stream, map);

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

QByteArray Asterix008Proto::generate(AsterixData &map)
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

        if (generate_000(map, stream))
            mainMask.set(2);
        else
            throw mandatory("Message Type");

        EMessageType messageType = (EMessageType) map.value(MessageType).toUInt();

        // SOP message restrictions.
        if ( messageType == SOPMessage )
        {
            if (generate_090(map, stream))
                mainMask.set(8);
            else
                map.appendError(mandatory("Time of Day"));

            if (generate_100(map, stream))
                mainMask.set(9);
            else
                map.appendError(mandatory("Processing Status"));

            if (generate_110(map, stream))
                mainMask.set(10);
            else
                map.appendError(mandatory("Station Configuration Status"));
        }
        // EOP message restrictions.
        else if ( messageType == EOPMessage )
        {
            if (generate_090(map, stream))
                mainMask.set(8);
            else
                map.appendError(mandatory("Time of Day"));

            if (generate_110(map, stream))
                mainMask.set(10);
            else
                map.appendError(mandatory("Station Configuration Status"));

            if (generate_120(map, stream))
                mainMask.set(11);
            else
                map.appendError(mandatory("Total number of Items"));
        }
        // Cartesian vector restrictions.
        else if ( messageType == CartesianVector )
        {
            if (generate_020(map, stream))
                mainMask.set(3);
            else
                map.appendError(mandatory("Vector Qualifier"));

            if (generate_036(map, stream))
                mainMask.set(4);
            else
                map.appendError(mandatory("Sequence of Cartesian Vectors"));
        }
        // Weather vector restrictions.
        else if ( messageType == WeatherVector )
        {
            if (generate_020(map, stream))
                mainMask.set(3);
            else
                map.appendError(mandatory("Vector Qualifier"));

            if (generate_038(map, stream))
                mainMask.set(12);
            else
                map.appendError(mandatory("Sequence of Weather Vectors"));
        }
        // Polar vector restrictions.
        else if ( messageType == PolarVector )
        {
            if (generate_020(map, stream))
                mainMask.set(3);
            else
                map.appendError(mandatory("Vector Qualifier"));

            if (generate_034(map, stream))
                mainMask.set(5);
            else
                map.appendError(mandatory("Sequence of Polar Vectors"));
        }
        // Contour points restrictions.
        else if ( messageType == ContourPoints )
        {
            if (generate_040(map, stream))
                mainMask.set(6);
            else
                map.appendError(mandatory("Contour Identifier"));

            if (generate_050(map, stream))
                mainMask.set(7);
            else
                map.appendError(mandatory("Sequence of Contour Points"));
        }
        else
            throw AsterixErrors::Error_InvalidInput;

        if (generate_re(map, stream))
            mainMask.set(13);

        if (generate_re(map, stream))
            mainMask.set(14);

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

QStringList Asterix008Proto::fields() const
{
    static QStringList result = QStringList()

    << MessageType

    << DataSourceIdentifier_SAC
    << DataSourceIdentifier_SIC

    << VectorQualifier

    << SequencePolarVectors

    << SequenceCartesianVectors

    << SequenceWeatherVectors

    << ContourIdentifier

    << SequenceContourPoints

    << TimeOfDay

    << ProcessingStatus

    << StationConfigurationStatus

    << TotalNumberItems;

    return result;
}

void Asterix008Proto::parse_000(QDataStream &stream, AsterixData &map)
{
    map.insert(MessageType, int(read<quint8>(stream)));
}

void Asterix008Proto::parse_010(QDataStream &stream, AsterixData &map)
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

void Asterix008Proto::parse_020(QDataStream &stream, AsterixData &map)
{
    SVectorQualifier vq;

    // First part
    quint8 byte = read<quint8>(stream);

    vq.org = byte & 0x80 ? SystemCoordinates : LocalCoordinates;
    vq.intensity = (byte >> 4) & M3;
    vq.shadingOrientation = ((byte >> 1) & M3) * 22.5f;

    // First extension.
    if ( byte & M1 )
    {
        byte = read<quint8>(stream);

        vq.isTestVector = byte & 0x04;
        vq.isErrorCondition = byte & 0x02;
    }

    // Read next extension fields - not defined in current spec.
    while ( byte & M1 )
        byte = read<quint8>(stream);

    map.insert(VectorQualifier, QVariant::fromValue(vq));
}

void Asterix008Proto::parse_034(QDataStream &stream, AsterixData &map)
{
    quint8 rep = read<quint8>(stream);

    TSequencePolarVectorList list;
    const qreal rangeFactor = qPow(2, -7+scaleFactor);

    for ( int i = 0; i < rep; i++ )
    {
        quint8 start = read<quint8>(stream);
        quint8 end = read<quint8>(stream);
        quint16 azimuth = read<quint16>(stream);

        SPolarVector polarVector;
        polarVector.startRange = rangeFactor * start;
        polarVector.endRange = rangeFactor * end;
        polarVector.azimuth = 0.0055 * azimuth;

        list.append( QVariant::fromValue(polarVector) );
    }

    if ( !list.empty() )
        map.insert(SequencePolarVectors, list);
}

void Asterix008Proto::parse_036(QDataStream &stream, AsterixData &map)
{
    quint8 rep = read<quint8>(stream);

    TSequenceCartesianVectorList list;
    const qreal lsbFactor = qPow(2, -6+scaleFactor);

    for ( int i = 0; i < rep; i++ )
    {
        quint8 x = read<quint8>(stream);
        quint8 y = read<quint8>(stream);
        quint8 length = read<quint8>(stream);

        SCartesianVector cartVector;
        cartVector.x = lsbFactor * x;
        cartVector.y = lsbFactor * y;
        cartVector.length = lsbFactor * length;

        list.append( QVariant::fromValue(cartVector) );
    }

    if ( !list.empty() )
        map.insert(SequenceCartesianVectors, list);
}

void Asterix008Proto::parse_038(QDataStream &stream, AsterixData &map)
{
    quint8 rep = read<quint8>(stream);

    TSequenceWeatherVectorList list;
    const qreal lsbFactor = qPow(2, -6+scaleFactor);

    for ( int i = 0; i < rep; i++ )
    {
        quint8 x1 = read<quint8>(stream);
        quint8 y1 = read<quint8>(stream);
        quint8 x2 = read<quint8>(stream);
        quint8 y2 = read<quint8>(stream);

        SWeatherVector weatherVector;
        weatherVector.x1 = lsbFactor * x1;
        weatherVector.y1 = lsbFactor * y1;
        weatherVector.x2 = lsbFactor * x2;
        weatherVector.y2 = lsbFactor * y2;

        list.append( QVariant::fromValue(weatherVector) );
    }

    if ( !list.empty() )
        map.insert(SequenceWeatherVectors, list);
}

void Asterix008Proto::parse_040(QDataStream &stream, AsterixData &map)
{
    quint16 byte2 = read<quint16>(stream);

    SContourId ci;
    ci.org = byte2 & 0x8000 ? SystemCoordinates : LocalCoordinates;
    ci.intensity = (byte2 >> 12) & M3;
    ci.fstLst = SContourId::FST_LST( (byte2 >> 8) & M2 );
    ci.serialNumber = byte2 & M8;

    map.insert(ContourIdentifier, QVariant::fromValue( ci ));
}

void Asterix008Proto::parse_050(QDataStream &stream, AsterixData &map)
{
    quint8 rep = read<quint8>(stream);

    TSequenceContourPointsList list;
    const qreal lsbFactor = qPow(2, -6+scaleFactor);

    for ( int i = 0; i < rep; i++ )
    {
        quint8 x = read<quint8>(stream);
        quint8 y = read<quint8>(stream);

        SPoint point;
        point.x = lsbFactor * x;
        point.y = lsbFactor * y;

        list.append( QVariant::fromValue(point) );
    }

    if ( !list.empty() )
        map.insert(SequenceContourPoints, list);
}

void Asterix008Proto::parse_090(QDataStream &stream, AsterixData &map)
{
    quint32 timeOfDay = read24(stream) / 128;
    map.insert(TimeOfDay, QTime().addMSecs(timeOfDay*1000));
}

void Asterix008Proto::parse_100(QDataStream &stream, AsterixData &map)
{
    quint32 byte4 = quint32(read<quint8>(stream)) << 16 | read<quint16>(stream);

    SProcessingStatus ps;
    ps.scaleFactor = ((byte4 >> 19) & M4) * (byte4 & 0x00800000 ? -1 : 1);
    ps.reductionStage = (byte4 >> 16) & M3;
    ps.parameters = (byte4 >> 1) & M15;

    // Dummy read to follow the protocol extent octets if exist.
    while ( byte4 & M1 )
        read<quint8>(stream);

    map.insert(ProcessingStatus, QVariant::fromValue(ps));
}

void Asterix008Proto::parse_110(QDataStream &stream, AsterixData &map)
{
    TStationConfigurationStatusList list;

    quint8 byte = 0;
    do
    {
        byte = read<quint8>(stream);
        list.append(byte);
    }
    while ( byte & M1 );

    if ( !list.empty() )
        map.insert(StationConfigurationStatus, list);
}

void Asterix008Proto::parse_120(QDataStream &stream, AsterixData &map)
{
    map.insert(TotalNumberItems, read<quint16>(stream));
}

bool Asterix008Proto::generate_000(AsterixData &map, QDataStream &stream)
{
    if (map.contains(MessageType))
    {
        write8(stream, map.value(MessageType).toUInt());
        return true;
    }
    return false;
}

bool Asterix008Proto::generate_010(AsterixData &map, QDataStream &stream)
{
    if (map.contains(DataSourceIdentifier_SAC)
        && map.contains(DataSourceIdentifier_SIC))
    {
        write8(stream, map.value(DataSourceIdentifier_SAC).toUInt());
        write8(stream, map.value(DataSourceIdentifier_SIC).toUInt());
        return true;
    }
    return false;
}

bool Asterix008Proto::generate_020(AsterixData &map, QDataStream &stream)
{
    if (map.contains(VectorQualifier))
    {
        quint8 byte = 0;
        SVectorQualifier vq = map.value(VectorQualifier).value<SVectorQualifier>();

        byte |= quint8( vq.org ) << 7;
        byte |= (quint8(vq.intensity) & M3) << 4;
        byte |= (quint8(qRound(vq.shadingOrientation / 22.5f)) & M3) << 1;

        quint8 byteExt = 0;
        if ( vq.isTestVector || vq.isErrorCondition )
        {
            byte |= 0x01;

            byteExt |= vq.isTestVector ? 0x04 : 0;
            byteExt |= vq.isErrorCondition ? 0x02 : 0;
        }

        write8(stream, byte);
        if ( byteExt )
            write8(stream, byteExt);

        return true;
    }
    return false;
}

bool Asterix008Proto::generate_034(AsterixData &map, QDataStream &stream)
{
    if (map.contains(SequencePolarVectors))
    {
        TSequencePolarVectorList list = map.value(SequencePolarVectors).toList();
        write8(stream, list.size());

        const qreal rangeFactor = qPow(2, -7+scaleFactor);

        foreach ( const QVariant &fields, list )
        {
            SPolarVector polarVector = fields.value<SPolarVector>();

            write8(stream, qRound(polarVector.startRange / rangeFactor));
            write8(stream, qRound(polarVector.endRange / rangeFactor));
            write16(stream, qRound(polarVector.azimuth / 0.0055));
        }

        return true;
    }
    return false;
}

bool Asterix008Proto::generate_036(AsterixData &map, QDataStream &stream)
{
    if (map.contains(SequenceCartesianVectors))
    {
        TSequenceCartesianVectorList list = map.value(SequenceCartesianVectors).toList();
        write8(stream, list.size());

        const qreal lsbFactor = qPow(2, -6+scaleFactor);

        foreach ( const QVariant &fields, list )
        {
            SCartesianVector cartVector = fields.value<SCartesianVector>();

            write8(stream, qRound(cartVector.x / lsbFactor));
            write8(stream, qRound(cartVector.y / lsbFactor));
            write8(stream, qRound(cartVector.length / lsbFactor));
        }

        return true;
    }
    return false;
}

bool Asterix008Proto::generate_038(AsterixData &map, QDataStream &stream)
{
    if (map.contains(SequenceWeatherVectors))
    {
        TSequenceWeatherVectorList list = map.value(SequenceWeatherVectors).toList();
        write8(stream, list.size());

        const qreal lsbFactor = qPow(2, -6+scaleFactor);

        foreach ( const QVariant &fields, list )
        {
            SWeatherVector weatherVector = fields.value<SWeatherVector>();

            write8(stream, qRound(weatherVector.x1 / lsbFactor));
            write8(stream, qRound(weatherVector.y1 / lsbFactor));
            write8(stream, qRound(weatherVector.x2 / lsbFactor));
            write8(stream, qRound(weatherVector.y2 / lsbFactor));
        }

        return true;
    }
    return false;
}

bool Asterix008Proto::generate_040(AsterixData &map, QDataStream &stream)
{
    if (map.contains(ContourIdentifier))
    {
        quint16 byte2 = 0;
        SContourId ci = map.value(ContourIdentifier).value<SContourId>();

        byte2 |= quint16(ci.serialNumber);
        byte2 |= quint16(ci.org) << 15;
        byte2 |= (quint16(ci.intensity) & M3) << 12;
        byte2 |= quint16(ci.fstLst) << 8;

        write16(stream, byte2);
        return true;
    }
    return false;
}

bool Asterix008Proto::generate_050(AsterixData &map, QDataStream &stream)
{
    if (map.contains(SequenceContourPoints))
    {
        TSequenceContourPointsList list = map.value(SequenceContourPoints).toList();
        write8(stream, list.size());

        const qreal lsbFactor = qPow(2, -6+scaleFactor);

        foreach ( const QVariant &fields, list )
        {
            SPoint point = fields.value<SPoint>();

            write8(stream, qRound(point.x / lsbFactor));
            write8(stream, qRound(point.y / lsbFactor));
        }

        return true;
    }
    return false;
}

bool Asterix008Proto::generate_090(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TimeOfDay))
    {
        int secs = QTime(0,0,0,0).msecsTo(map.value(TimeOfDay).toTime()) * 128 / 1000;
        write24(stream, secs);
        return true;
    }
    return false;
}

bool Asterix008Proto::generate_100(AsterixData &map, QDataStream &stream)
{
    if (map.contains(ProcessingStatus))
    {
        SProcessingStatus ps = map.value(ProcessingStatus).value<SProcessingStatus>();

        quint8 byte = 0;
        byte |= ps.reductionStage & M3;
        byte |= (ps.scaleFactor < 0 ? (0x10 | (quint8(0xFF-ps.scaleFactor+1) & M4)) :
                (ps.scaleFactor & M4)) << 3;
        byte |= ps.scaleFactor < 0 ? 0x80 : 0x00;

        quint16 byte2 = 0;
        byte2 |= ps.parameters << 1;

        write8(stream, byte);
        write16(stream, byte2);
        return true;
    }
    return false;
}

bool Asterix008Proto::generate_110(AsterixData &map, QDataStream &stream)
{
    if (map.contains(StationConfigurationStatus))
    {
        TStationConfigurationStatusList list = map.value(StationConfigurationStatus).toList();

        int i = 0;
        quint8 byte = 0;
        foreach ( const QVariant &scs, list )
        {
            byte = quint8( scs.toUInt() );

            if ( i+1 < list.size() )
                byte |= M1;

            write8(stream, byte);
            i++;
        }

        return true;
    }
    return false;
}

bool Asterix008Proto::generate_120(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TotalNumberItems))
    {
        write16(stream, map.value(TotalNumberItems).toUInt());
        return true;
    }
    return false;
}
