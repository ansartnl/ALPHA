#include "asterix063.h"
#include "shareutility.h"
#include "presentdata.h"
#include "asterixdata.h"

#include <QtCore/QTime>

#include <math.h>


const int Asterix063Proto::mCategory = 63;
const QString Asterix063Proto::mEdition = "1.3";

// Data Item I063/010, Data Source Identifier
const QString Asterix063Proto::DataSourceIdentifier_SAC = ASTERIX_SAC;
const QString Asterix063Proto::DataSourceIdentifier_SIC = ASTERIX_SIC;

// Data Item I063/015, Service Identification
const QString Asterix063Proto::ServiceIdentification = "a063.ServiceIdentification";

// Data Item I063/030, Time of Message
const QString Asterix063Proto::TimeOfMessage = "a063.TimeOfMessage";

// Data Item I063/050, Sensor Identifier
const QString Asterix063Proto::SensorIdentifier_SAC = "a063.SensorIdentifier.SAC";
const QString Asterix063Proto::SensorIdentifier_SIC = "a063.SensorIdentifier.SIC";

// Data Item I063/060, Sensor Configuration and Status
const QString Asterix063Proto::SensorConfigurationAndStatus_CON = "a063.SensorConfigurationAndStatus.CON";
const QString Asterix063Proto::SensorConfigurationAndStatus_PSR = "a063.SensorConfigurationAndStatus.PSR";
const QString Asterix063Proto::SensorConfigurationAndStatus_SSR = "a063.SensorConfigurationAndStatus.SSR";
const QString Asterix063Proto::SensorConfigurationAndStatus_MDS = "a063.SensorConfigurationAndStatus.MDS";
const QString Asterix063Proto::SensorConfigurationAndStatus_ADS = "a063.SensorConfigurationAndStatus.ADS";
const QString Asterix063Proto::SensorConfigurationAndStatus_MLT = "a063.SensorConfigurationAndStatus.MLT";
const QString Asterix063Proto::SensorConfigurationAndStatus_FX = "a063.SensorConfigurationAndStatus.FX";

// Data Item I063/070, Time Stamping Bias
const QString Asterix063Proto::TimeStampingBias = "a063.TimeStampingBias";

// Data Item I063/080, SSR / Mode S Range Gain and Bias
const QString Asterix063Proto::SsrModeSRangeGainAndBias_SRG = "a063.SsrModeSRangeGainAndBias.SRG";
const QString Asterix063Proto::SsrModeSRangeGainAndBias_SRB = "a063.SsrModeSRangeGainAndBias.SRB";

// Data Item I063/081, SSR / Mode S Azimuth Bias
const QString Asterix063Proto::SsrModeSAzimuthBias = "a063.SsrModeSAzimuthBias";

// Data Item I063/090, PSR Range Gain and Bias
const QString Asterix063Proto::PsrRangeGainSndBias_PRG = "a063.PsrRangeGainSndBias.PRG";
const QString Asterix063Proto::PsrRangeGainSndBias_PRB = "a063.PsrRangeGainSndBias.PRB";

// Data Item I063/091, PSR Azimuth Bias
const QString Asterix063Proto::PsrAzimuthBias = "a063.PsrAzimuthBias";

// Data Item I063/092, PSR Elevation Bias
const QString Asterix063Proto::PsrElevationBias = "a063.PsrElevationBias";

const QString Asterix063Proto::SpecialPurposeField = "a063.SpecialPurposeField";
const QString Asterix063Proto::ReservedExpansionField = "a063.ReservedExpansionField";

Asterix063Proto::Asterix063Proto()
{}

int Asterix063Proto::category() const
{
    return mCategory;
}

QString Asterix063Proto::edition() const
{
    return mEdition;
}

void Asterix063Proto::parse(QDataStream &stream, AsterixData &map)
{
    try
    {
        PresentData mainMask(stream);

        if (mainMask.size() > 2)
            throw AsterixErrors::Error_FSPEC;

        if (mainMask.isSet(1))
            parse_010(stream, map);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (mainMask.isSet(2))
            parse_015(stream, map);

        if (mainMask.isSet(3))
            parse_030(stream, map);
        else
            map.appendError(mandatory("Time of Message"));

        if (mainMask.isSet(4))
            parse_050(stream, map);
        else
            map.appendError(mandatory("Sensor Identifier"));

        if (mainMask.isSet(5))
            parse_060(stream, map);

        if (mainMask.isSet(6))
            parse_070(stream, map);

        if (mainMask.isSet(7))
            parse_080(stream, map);

        if (mainMask.isSet(8))
            parse_081(stream, map);

        if (mainMask.isSet(9))
            parse_090(stream, map);

        if (mainMask.isSet(10))
            parse_091(stream, map);

        if (mainMask.isSet(11))
            parse_092(stream, map);

        if (mainMask.isSet(12))
            throw AsterixErrors::Error_FSPEC;

        if (mainMask.isSet(13))
            parse_re(stream, map);

        if (mainMask.isSet(14))
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

QByteArray Asterix063Proto::generate(AsterixData &map)
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

        if (generate_015(map, stream))
            mainMask.set(2);

        if (generate_030(map, stream))
            mainMask.set(3);
        else
            map.appendError(mandatory("Time of Message"));

        if (generate_050(map, stream))
            mainMask.set(4);
        else
            map.appendError(mandatory("Sensor Identifier"));

        if (generate_060(map, stream))
            mainMask.set(5);

        if (generate_070(map, stream))
            mainMask.set(6);

        if (generate_080(map, stream))
            mainMask.set(7);

        if (generate_081(map, stream))
            mainMask.set(8);

        if (generate_090(map, stream))
            mainMask.set(9);

        if (generate_091(map, stream))
            mainMask.set(10);

        if (generate_092(map, stream))
            mainMask.set(11);

        if (generate_re(map, stream))
            mainMask.set(13);

        if (generate_sp(map, stream))
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

void Asterix063Proto::parse_010(QDataStream &stream, AsterixData &map)
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

bool Asterix063Proto::generate_010(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, DataSourceIdentifier_SAC, DataSourceIdentifier_SIC))
    {
        write8(stream, map.value(DataSourceIdentifier_SAC).toUInt());
        write8(stream, map.value(DataSourceIdentifier_SIC).toUInt());
        return true;
    }
    return false;
}

void Asterix063Proto::parse_015(QDataStream &stream, AsterixData &map)
{
    map.insert(ServiceIdentification, read<quint8>(stream));
}

bool Asterix063Proto::generate_015(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ServiceIdentification))
    {
        write8(stream, map.value(ServiceIdentification).toUInt());
        return true;
    }
    return false;
}

void Asterix063Proto::parse_030(QDataStream &stream, AsterixData &map)
{
    map.insert(TimeOfMessage, QTime().addSecs(ceil(double(read24(stream)) / 128)));
}

bool Asterix063Proto::generate_030(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TimeOfMessage))
    {
        write24(stream, double(map.value(TimeOfMessage).toTime().secsTo(QTime(0,0,0,0))) * -128);
        return true;
    }
    return false;
}

void Asterix063Proto::parse_050(QDataStream &stream, AsterixData &map)
{
    map.insert(SensorIdentifier_SAC, read<quint8>(stream));
    map.insert(SensorIdentifier_SIC, read<quint8>(stream));
}

bool Asterix063Proto::generate_050(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, SensorIdentifier_SAC, SensorIdentifier_SIC))
    {
        write8(stream, map.value(SensorIdentifier_SAC).toUInt());
        write8(stream, map.value(SensorIdentifier_SIC).toUInt());
        return true;
    }
    return false;
}

void Asterix063Proto::parse_060(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);
    map.insert(SensorConfigurationAndStatus_CON, data >> 6 & M2);
    map.insert(SensorConfigurationAndStatus_PSR, data >> 5 & M1);
    map.insert(SensorConfigurationAndStatus_SSR, data >> 4 & M1);
    map.insert(SensorConfigurationAndStatus_MDS, data >> 3 & M1);
    map.insert(SensorConfigurationAndStatus_ADS, data >> 2 & M1);
    map.insert(SensorConfigurationAndStatus_MLT, data >> 1 & M1);

    QVariantList extension;
    while (data & M1)
    {
        data = read<quint8>(stream);
        I063_060_FX val = {0};
        val.OPS = data >> 7 & M1;
        val.ODP = data >> 6 & M1;
        val.OXT = data >> 5 & M1;
        val.MSC = data >> 4 & M1;
        val.TSV = data >> 3 & M1;
        val.NPW = data >> 2 & M1;
        extension.push_back(QVariant::fromValue(val));
    }

    if (!extension.isEmpty())
        map.insert(SensorConfigurationAndStatus_FX, extension);
}

bool Asterix063Proto::generate_060(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, SensorConfigurationAndStatus_CON, SensorConfigurationAndStatus_PSR, SensorConfigurationAndStatus_SSR
                  , SensorConfigurationAndStatus_MDS, SensorConfigurationAndStatus_ADS, SensorConfigurationAndStatus_MLT))
    {
        quint8 data = (map.value(SensorConfigurationAndStatus_CON).toUInt() & M2) << 6;
        data |= (map.value(SensorConfigurationAndStatus_PSR).toUInt() & M1) << 5;
        data |= (map.value(SensorConfigurationAndStatus_SSR).toUInt() & M1) << 4;
        data |= (map.value(SensorConfigurationAndStatus_MDS).toUInt() & M1) << 3;
        data |= (map.value(SensorConfigurationAndStatus_ADS).toUInt() & M1) << 2;
        data |= (map.value(SensorConfigurationAndStatus_MLT).toUInt() & M1) << 1;

        if (isContain(map, SensorConfigurationAndStatus_FX))
        {
            QVariantList extension = map.value(SensorConfigurationAndStatus_FX).toList();
            foreach (const QVariant &varinat, extension)
            {
                write8(stream, data | M1);
                I063_060_FX val = varinat.value<I063_060_FX>();
                data = (val.OPS & M1) << 7;
                data |= (val.ODP & M1) << 6;
                data |= (val.OXT & M1) << 5;
                data |= (val.MSC & M1) << 4;
                data |= (val.TSV & M1) << 3;
                data |= (val.NPW & M1) << 2;
            }
        }
        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix063Proto::parse_070(QDataStream &stream, AsterixData &map)
{
    map.insert(TimeStampingBias, read<quint16>(stream));
}

bool Asterix063Proto::generate_070(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, TimeStampingBias))
    {
        write16(stream, map.value(TimeStampingBias).toUInt());
        return true;
    }
    return false;
}

void Asterix063Proto::parse_080(QDataStream &stream, AsterixData &map)
{
    map.insert(SsrModeSRangeGainAndBias_SRG, double(read<qint16>(stream)) / 100000);
    map.insert(SsrModeSRangeGainAndBias_SRB, double(read<qint16>(stream)) / 128);
}

bool Asterix063Proto::generate_080(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, SsrModeSRangeGainAndBias_SRG, SsrModeSRangeGainAndBias_SRB))
    {
        write16(stream, map.value(SsrModeSRangeGainAndBias_SRG).toDouble() * 100000);
        write16(stream, map.value(SsrModeSRangeGainAndBias_SRB).toDouble() * 128);
        return true;
    }
    return false;
}

void Asterix063Proto::parse_081(QDataStream &stream, AsterixData &map)
{
    map.insert(SsrModeSAzimuthBias, double(read<qint16>(stream)) * 360 / 65536);
}

bool Asterix063Proto::generate_081(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, SsrModeSAzimuthBias))
    {
        write16(stream, map.value(SsrModeSAzimuthBias).toDouble() * 65536 / 360);
        return true;
    }
    return false;
}

void Asterix063Proto::parse_090(QDataStream &stream, AsterixData &map)
{
    map.insert(PsrRangeGainSndBias_PRG, double(read<qint16>(stream)) / 100000);
    map.insert(PsrRangeGainSndBias_PRB, double(read<qint16>(stream)) / 128);
}

bool Asterix063Proto::generate_090(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, PsrRangeGainSndBias_PRG, PsrRangeGainSndBias_PRB))
    {
        write16(stream, map.value(PsrRangeGainSndBias_PRG).toDouble() * 100000);
        write16(stream, map.value(PsrRangeGainSndBias_PRB).toDouble() * 128);
        return true;
    }
    return false;
}

void Asterix063Proto::parse_091(QDataStream &stream, AsterixData &map)
{
    map.insert(PsrAzimuthBias, double(read<qint16>(stream)) * 360 / 65536);
}

bool Asterix063Proto::generate_091(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, PsrAzimuthBias))
    {
        write16(stream, map.value(PsrAzimuthBias).toDouble() * 65536 / 360);
        return true;
    }
    return false;
}

void Asterix063Proto::parse_092(QDataStream &stream, AsterixData &map)
{
    map.insert(PsrElevationBias, double(read<qint16>(stream)) * 360 / 65536);
}

bool Asterix063Proto::generate_092(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, PsrElevationBias))
    {
        write16(stream, map.value(PsrElevationBias).toDouble() * 65536 / 360);
        return true;
    }
    return false;
}

QStringList Asterix063Proto::fields() const
{
    static QStringList result = QStringList()
    // Data Item I063/010, Data Source Identifier
    << DataSourceIdentifier_SAC
    << DataSourceIdentifier_SIC

    // Data Item I063/015, Service Identification
    << ServiceIdentification

    // Data Item I063/030, Time of Message
    << TimeOfMessage

    // Data Item I063/050, Sensor Identifier
    << SensorIdentifier_SAC
    << SensorIdentifier_SIC

    // Data Item I063/060, Sensor Configuration and Status
    << SensorConfigurationAndStatus_CON
    << SensorConfigurationAndStatus_PSR
    << SensorConfigurationAndStatus_SSR
    << SensorConfigurationAndStatus_MDS
    << SensorConfigurationAndStatus_ADS
    << SensorConfigurationAndStatus_MLT
    << SensorConfigurationAndStatus_FX

    // Data Item I063/070, Time Stamping Bias
    << TimeStampingBias

    // Data Item I063/080, SSR / Mode S Range Gain and Bias
    << SsrModeSRangeGainAndBias_SRG
    << SsrModeSRangeGainAndBias_SRB

    // Data Item I063/081, SSR / Mode S Azimuth Bias
    << SsrModeSAzimuthBias

    // Data Item I063/090, PSR Range Gain and Bias
    << PsrRangeGainSndBias_PRG
    << PsrRangeGainSndBias_PRB

    // Data Item I063/091, PSR Azimuth Bias
    << PsrAzimuthBias

    // Data Item I063/092, PSR Elevation Bias
    << PsrElevationBias

    << SpecialPurposeField
    << ReservedExpansionField;
    return result;
}
