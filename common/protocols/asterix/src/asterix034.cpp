#include "asterix034.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>

#include <math.h>


const int Asterix034Proto::mCategory = 34;
const QString Asterix034Proto::mEdition = "1.27";

const QString Asterix034Proto::MessageType = "a034.MessageType";

const QString Asterix034Proto::DataSourceIdentifier_SAC = ASTERIX_SAC;
const QString Asterix034Proto::DataSourceIdentifier_SIC = ASTERIX_SIC;

const QString Asterix034Proto::SectorAzimuth = "a034.SectorAzimuth";

const QString Asterix034Proto::TimeOfDay = "a034.TimeOfDay";

const QString Asterix034Proto::AntennaRotationPeriod = "a034.AntennaRotationPeriod";

const QString Asterix034Proto::StationConfigurationStatus_COM_NOGO = "a034.StationConfigurationStatus.CommonPart.perational Release Status of the System";
const QString Asterix034Proto::StationConfigurationStatus_COM_RDPC = "a034.StationConfigurationStatus.CommonPart.Radar Data Processor Chain Selection Status";
const QString Asterix034Proto::StationConfigurationStatus_COM_RDPR = "a034.StationConfigurationStatus.CommonPart.RDPR";
const QString Asterix034Proto::StationConfigurationStatus_COM_OVLRDP = "a034.StationConfigurationStatus.CommonPart.Radar Data Processor Overload Indicator";
const QString Asterix034Proto::StationConfigurationStatus_COM_OVLXMT = "a034.StationConfigurationStatus.CommonPart.Transmission Subsystem Overload Status";
const QString Asterix034Proto::StationConfigurationStatus_COM_MSC = "a034.StationConfigurationStatus.CommonPart.Monitoring System Connected Status";
const QString Asterix034Proto::StationConfigurationStatus_COM_TSV = "a034.StationConfigurationStatus.CommonPart.Time Source Validity";

const QString Asterix034Proto::StationConfigurationStatus_PSR_ANT = "a034.StationConfigurationStatus.Specific Status for PSR Sensor.Selected antenna";
const QString Asterix034Proto::StationConfigurationStatus_PSR_CHAB = "a034.StationConfigurationStatus.Specific Status for PSR Sensor.Channel A/B selection status";
const QString Asterix034Proto::StationConfigurationStatus_PSR_OVL = "a034.StationConfigurationStatus.Specific Status for PSR Sensor.Overload condition";
const QString Asterix034Proto::StationConfigurationStatus_PSR_MSC = "a034.StationConfigurationStatus.Specific Status for PSR Sensor.Monitoring System Connected Status";

const QString Asterix034Proto::StationConfigurationStatus_SSR_ANT = "a034.StationConfigurationStatus.Specific Status for SSR Sensor.Selected antenna";
const QString Asterix034Proto::StationConfigurationStatus_SSR_CHAB = "a034.StationConfigurationStatus.Specific Status for SSR Sensor.Channel A/B selection status";
const QString Asterix034Proto::StationConfigurationStatus_SSR_OVL = "a034.StationConfigurationStatus.Specific Status for SSR Sensor.Overload condition";
const QString Asterix034Proto::StationConfigurationStatus_SSR_MSC = "a034.StationConfigurationStatus.Specific Status for SSR Sensor.Monitoring System Connected Status";

const QString Asterix034Proto::StationConfigurationStatus_MDS_ANT = "a034.StationConfigurationStatus.Specific Status for Mode S Sensor.Selected antenna";
const QString Asterix034Proto::StationConfigurationStatus_MDS_CHAB = "a034.StationConfigurationStatus.Specific Status for Mode S Sensor.Channel A/B selection status for surveillance";
const QString Asterix034Proto::StationConfigurationStatus_MDS_OVLSUR = "a034.StationConfigurationStatus.Specific Status for Mode S Sensor.Overload condition";
const QString Asterix034Proto::StationConfigurationStatus_MDS_MSC = "a034.StationConfigurationStatus.Specific Status for Mode S Sensor.Monitoring System Connected Status";
const QString Asterix034Proto::StationConfigurationStatus_MDS_SCF = "a034.StationConfigurationStatus.Specific Status for Mode S Sensor.Channel A/B selection status for Surveillance Co-ordination Function";
const QString Asterix034Proto::StationConfigurationStatus_MDS_DLF = "a034.StationConfigurationStatus.Specific Status for Mode S Sensor.Channel A/B selection status for Data Link Function";
const QString Asterix034Proto::StationConfigurationStatus_MDS_OVLSCF = "a034.StationConfigurationStatus.Specific Status for Mode S Sensor.Overload in Surveillance Co-ordination Function";
const QString Asterix034Proto::StationConfigurationStatus_MDS_OVLDLF = "a034.StationConfigurationStatus.Specific Status for Mode S Sensor.Overload in Data Link Function";

const QString Asterix034Proto::StationProcessingMode_COM_REDRDP = "a034.System Processing Mode.Common Part.Reduction Steps in use for an overload of the RDP";
const QString Asterix034Proto::StationProcessingMode_COM_REDXMT = "a034.System Processing Mode.Common Part.Reduction Steps in use for an overload of the Transmission subsystem";

const QString Asterix034Proto::StationProcessingMode_PSR_POL = "a034.System Processing Mode.Specific Processing Mode information for PSR Sensor.Polarization in use by PSR";
const QString Asterix034Proto::StationProcessingMode_PSR_REDRAD = "a034.System Processing Mode.Specific Processing Mode information for PSR Sensor.Reduction Steps in use as result of an overload within the PSR subsystem";
const QString Asterix034Proto::StationProcessingMode_PSR_STC = "a034.System Processing Mode.Specific Processing Mode information for PSR Sensor.Sensitivity Time Control Map in use";

const QString Asterix034Proto::StationProcessingMode_SSR_REDRAD = "a034.System Processing Mode.Specific Processing Mode information for SSR Sensor.Reduction Steps in use as result of an overload within the SSR subsystem";

const QString Asterix034Proto::StationProcessingMode_MDS_REDRAD = "a034.System Processing Mode.Specific Processing Mode information for Mode S Sensor.Reduction Steps in use as result of an overload within the Mode S subsystem";
const QString Asterix034Proto::StationProcessingMode_MDS_CLU = "a034.System Processing Mode.Specific Processing Mode information for Mode S Sensor.Cluster State";

const QString Asterix034Proto::MessageCountValues = "a034.MessageCountValues";

const QString Asterix034Proto::CollimationError_RangeError = "a034.CollimationError.RangeError";
const QString Asterix034Proto::CollimationError_AzimuthError = "a034.CollimationError.AzimuthError";

const QString Asterix034Proto::GenericPolarWindow_RhoStart = "a034.GenericPolarWindow.RhoStart";
const QString Asterix034Proto::GenericPolarWindow_RhoEnd = "a034.GenericPolarWindow.RhoEnd";
const QString Asterix034Proto::GenericPolarWindow_ThetaStart = "a034.GenericPolarWindow.ThetaStart";
const QString Asterix034Proto::GenericPolarWindow_ThetaEnd = "a034.GenericPolarWindow.ThetaEnd";

const QString Asterix034Proto::DataFilter = "a034.DataFilter";

const QString Asterix034Proto::PositionOfSource3D_Height = "a034.PositionOfSource3D.Height";
const QString Asterix034Proto::PositionOfSource3D_Latitude = "a034.PositionOfSource3D.Latitude";
const QString Asterix034Proto::PositionOfSource3D_Longitude = "a034.PositionOfSource3D.Longitude";


Asterix034Proto::Asterix034Proto()
{}

int Asterix034Proto::category() const
{
    return mCategory;
}

QString Asterix034Proto::edition() const
{
    return mEdition;
}

void Asterix034Proto::parse(QDataStream &stream, AsterixData &map)
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

        if (messageType == NorthMarker)
        {
            if (mainMask.isSet(3))
                parse_030(stream, map);
            else
                map.appendError(mandatory("Time of Day"));

            if (mainMask.isSet(4))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(5))
                parse_041(stream, map);

            if (mainMask.isSet(6))
                parse_050(stream, map);

            if (mainMask.isSet(7))
                parse_060(stream, map);

            if (mainMask.isSet(8))
                parse_070(stream, map);

            if (mainMask.isSet(9))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(10))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(11))
                parse_120(stream, map);

            if (mainMask.isSet(12))
                parse_090(stream, map);
        }
        else if (messageType == SectorCrossing)
        {
            if (mainMask.isSet(3))
                parse_030(stream, map);
            else
                map.appendError(mandatory("Time of Day"));

            if (mainMask.isSet(4))
                parse_020(stream, map);
            else
                map.appendError(mandatory("Sector Number"));

            if (mainMask.isSet(5))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(6))
                parse_050(stream, map);

            if (mainMask.isSet(7))
                parse_060(stream, map);

            if (mainMask.isSet(8))
                parse_070(stream, map);

            if (mainMask.isSet(9))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(10))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(11))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(12))
                parse_090(stream, map);
        }
        else if (messageType == GeographicalFiltering)
        {
            if (mainMask.isSet(3))
                parse_030(stream, map);

            if (mainMask.isSet(4))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(5))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(6))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(7))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(8))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(9))
                parse_100(stream, map);

            if (mainMask.isSet(10))
                parse_110(stream, map);
            else
                map.appendError(mandatory("DataFilter"));

            if (mainMask.isSet(11))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(12))
                map.appendError(AsterixErrors::Error_FSPEC);
        }
        else if (messageType == JammingStrobe)
        {
            if (mainMask.isSet(3))
                parse_030(stream, map);

            if (mainMask.isSet(4))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(5))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(6))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(7))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(8))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(9))
                parse_100(stream, map);
            else
                map.appendError(mandatory("Generic Polar Window"));

            if (mainMask.isSet(10))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(11))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(12))
                map.appendError(AsterixErrors::Error_FSPEC);
        }
        else
            throw AsterixErrors::Error_InvalidInput;

        // Reserved Expansion Field
        if (mainMask.isSet(13))
            parse_re(stream, map);

        // Special Purpose Field
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

QByteArray Asterix034Proto::generate(AsterixData &map)
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

        // North marker message restrictions.
        if ( messageType == NorthMarker )
        {
            if (generate_030(map, stream))
                mainMask.set(3);
            else
                map.appendError(mandatory("Time of Day"));

            if (generate_020(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_041(map, stream))
                mainMask.set(5);

            if (generate_050(map, stream))
                mainMask.set(6);

            if (generate_060(map, stream))
                mainMask.set(7);

            if (generate_070(map, stream))
                mainMask.set(8);

            if (generate_100(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_110(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_120(map, stream))
                mainMask.set(11);

            if (generate_090(map, stream))
                mainMask.set(12);
        }
        else if ( messageType == SectorCrossing ) // Sector crossing message restrictions.
        {
            if (generate_030(map, stream))
                mainMask.set(3);
            else
                map.appendError(mandatory("Time of Day"));

            if (generate_020(map, stream))
                mainMask.set(4);
            else
                map.appendError(mandatory("Sector Number"));

            if (generate_041(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_050(map, stream))
                mainMask.set(6);

            if (generate_060(map, stream))
                mainMask.set(7);

            if (generate_070(map, stream))
                mainMask.set(8);

            if (generate_100(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_110(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_120(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_090(map, stream))
                mainMask.set(12);
        }
        else if ( messageType == GeographicalFiltering ) // Geographical filtering message restrictions.
        {
            if (generate_030(map, stream))
                mainMask.set(3);

            if (generate_020(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_041(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_050(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_060(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_070(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_100(map, stream))
                mainMask.set(9);

            if (generate_110(map, stream))
                mainMask.set(10);
            else
                map.appendError(mandatory("DataFilter"));

            if (generate_120(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_090(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);
        }
        else if ( messageType == JammingStrobe ) // Jamming strobe message restrictions.
        {
            if (generate_030(map, stream))
                mainMask.set(3);

            if (generate_020(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_041(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_050(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_060(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_070(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_100(map, stream))
                mainMask.set(9);
            else
                map.appendError(mandatory("Generic Polar Window"));

            if (generate_110(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_120(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

            if (generate_090(map, stream))
                map.appendError(AsterixErrors::Error_FSPEC);

        }
        else
            throw AsterixErrors::Error_InvalidInput;

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

QStringList Asterix034Proto::fields() const
{
    static QStringList result = QStringList()

    // Data Item I034/000, Message Type
    << MessageType

    // Data Item I034/010, Data Source Identifier
    << DataSourceIdentifier_SAC
    << DataSourceIdentifier_SIC

    // Data Item I034/020, Sector Number
    << SectorAzimuth

    // Data Item I034/030, Time of Day
    << TimeOfDay

    // Data Item I034/041, Antenna Rotation Speed
    << AntennaRotationPeriod

    // Data Item I034/050, System Configuration and Status
    // COM Subfield:
    << StationConfigurationStatus_COM_NOGO
    << StationConfigurationStatus_COM_RDPC
    << StationConfigurationStatus_COM_RDPR
    << StationConfigurationStatus_COM_OVLRDP
    << StationConfigurationStatus_COM_OVLXMT
    << StationConfigurationStatus_COM_MSC
    << StationConfigurationStatus_COM_TSV
    // PSR Subfield:
    << StationConfigurationStatus_PSR_ANT
    << StationConfigurationStatus_PSR_CHAB
    << StationConfigurationStatus_PSR_OVL
    << StationConfigurationStatus_PSR_MSC
    // SSR Subfield:
    << StationConfigurationStatus_SSR_ANT
    << StationConfigurationStatus_SSR_CHAB
    << StationConfigurationStatus_SSR_OVL
    << StationConfigurationStatus_SSR_MSC
    // MDS Subfield:
    << StationConfigurationStatus_MDS_ANT
    << StationConfigurationStatus_MDS_CHAB
    << StationConfigurationStatus_MDS_OVLSUR
    << StationConfigurationStatus_MDS_MSC
    << StationConfigurationStatus_MDS_SCF
    << StationConfigurationStatus_MDS_DLF
    << StationConfigurationStatus_MDS_OVLSCF
    << StationConfigurationStatus_MDS_OVLDLF

    // Data Item I034/060, System Processing Mode
    // COM Subfield:
    << StationProcessingMode_COM_REDRDP
    << StationProcessingMode_COM_REDXMT
    // PSR Subfield:
    << StationProcessingMode_PSR_POL
    << StationProcessingMode_PSR_REDRAD
    << StationProcessingMode_PSR_STC
    // SSR Subfield:
    << StationProcessingMode_SSR_REDRAD
    // MDS Subfield:
    << StationProcessingMode_MDS_REDRAD
    << StationProcessingMode_MDS_CLU

    //Data Item I034/070, Message Count Values
    << MessageCountValues

    // Data Item I034/090, Collimation Error
    << CollimationError_RangeError
    << CollimationError_AzimuthError

    // Data Item I034/100, Generic Polar Window
    << GenericPolarWindow_RhoStart
    << GenericPolarWindow_RhoEnd
    << GenericPolarWindow_ThetaStart
    << GenericPolarWindow_ThetaEnd

    // Data Item I034/110 Data Filter
    << DataFilter

    // Data Item I034/120 3D-Position Of Data Source.
    << PositionOfSource3D_Height
    << PositionOfSource3D_Latitude
    << PositionOfSource3D_Longitude

    // sp and re
    << SpecialPurposeField
    << ReservedExpansionField;

    return result;
}

void Asterix034Proto::parse_000(QDataStream &stream, AsterixData &map)
{
    map.insert(MessageType, read<quint8>(stream));
}

bool Asterix034Proto::generate_000(AsterixData &map, QDataStream &stream)
{
    if (map.contains(MessageType))
    {
        write8(stream, map.value(MessageType).toUInt());
        return true;
    }
    return false;
}

void Asterix034Proto::parse_010(QDataStream &stream, AsterixData &map)
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

bool Asterix034Proto::generate_010(AsterixData &map, QDataStream &stream)
{
    if (map.contains(DataSourceIdentifier_SAC) && map.contains(DataSourceIdentifier_SIC))
    {
        write8(stream, map.value(DataSourceIdentifier_SAC).toUInt());
        write8(stream, map.value(DataSourceIdentifier_SIC).toUInt());
        return true;
    }
    return false;
}

void Asterix034Proto::parse_020(QDataStream &stream, AsterixData &map)
{
    map.insert(SectorAzimuth, double(read<quint8>(stream)) * 360 / 256);
}

bool Asterix034Proto::generate_020(AsterixData &map, QDataStream &stream)
{
    if (map.contains(SectorAzimuth))
    {
        write8(stream, map.value(SectorAzimuth).toDouble() * 256 / 360);
        return true;
    }
    return false;
}

void Asterix034Proto::parse_030(QDataStream &stream, AsterixData &map)
{
    map.insert(TimeOfDay, QTime().addSecs(ceil(double(read24(stream)) / 128)));
}

bool Asterix034Proto::generate_030(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TimeOfDay))
    {
        write24(stream, double(map.value(TimeOfDay).toTime().secsTo(QTime(0,0,0,0))) * -128);
        return true;
    }
    return false;
}

void Asterix034Proto::parse_041(QDataStream &stream, AsterixData &map)
{
    map.insert(AntennaRotationPeriod, double(read<quint16>(stream)) / 128);
}

bool Asterix034Proto::generate_041(AsterixData &map, QDataStream &stream)
{
    if (map.contains(AntennaRotationPeriod))
    {
        write16(stream, map.value(AntennaRotationPeriod).toDouble() * 128);
        return true;
    }
    return false;
}

void Asterix034Proto::parse_050(QDataStream &stream, AsterixData &map)
{
    PresentData mask(stream);

    if (mask.size() > 1)
        map.appendError(AsterixErrors::Error_FX);

    if (mask.isSet(1))
    {
        quint8 data = read<quint8>(stream);
        map.insert(StationConfigurationStatus_COM_NOGO, data >> 7 & M1);
        map.insert(StationConfigurationStatus_COM_RDPC, data >> 6 & M1);
        map.insert(StationConfigurationStatus_COM_RDPR, data >> 5 & M1);
        map.insert(StationConfigurationStatus_COM_OVLRDP, data >> 4 & M1);
        map.insert(StationConfigurationStatus_COM_OVLXMT, data >> 3 & M1);
        map.insert(StationConfigurationStatus_COM_MSC, data >> 2 & M1);
        map.insert(StationConfigurationStatus_COM_TSV, data >> 1 & M1);
    }

    if (mask.isSet(2))
        map.appendError(AsterixErrors::Error_FX);

    if (mask.isSet(3))
        map.appendError(AsterixErrors::Error_FX);

    if (mask.isSet(4))
    {
        quint8 data = read<quint8>(stream);
        map.insert(StationConfigurationStatus_PSR_ANT, data >> 7 & M1);
        map.insert(StationConfigurationStatus_PSR_CHAB, data >> 5 & M2);
        map.insert(StationConfigurationStatus_PSR_OVL, data >> 4 & M1);
        map.insert(StationConfigurationStatus_PSR_MSC, data >> 3 & M1);
    }

    if (mask.isSet(5))
    {
        quint8 data = read<quint8>(stream);
        map.insert(StationConfigurationStatus_SSR_ANT, data >> 7 & M1);
        map.insert(StationConfigurationStatus_SSR_CHAB, data >> 5 & M2);
        map.insert(StationConfigurationStatus_SSR_OVL, data >> 4 & M1);
        map.insert(StationConfigurationStatus_SSR_MSC, data >> 3 & M1);
    }

    if (mask.isSet(6))
    {
        quint16 data = read<quint16>(stream);
        map.insert(StationConfigurationStatus_MDS_ANT, data >> 15 & M1);
        map.insert(StationConfigurationStatus_MDS_CHAB, data >> 13 & M2);
        map.insert(StationConfigurationStatus_MDS_OVLSUR, data >> 12 & M1);
        map.insert(StationConfigurationStatus_MDS_MSC, data >> 11 & M1);
        map.insert(StationConfigurationStatus_MDS_SCF, data >> 10 & M1);
        map.insert(StationConfigurationStatus_MDS_DLF, data >> 9 & M1);
        map.insert(StationConfigurationStatus_MDS_OVLSCF, data >> 8 & M1);
        map.insert(StationConfigurationStatus_MDS_OVLDLF, data >> 7 & M1);
    }

    if (mask.isSet(7))
        map.appendError(AsterixErrors::Error_FX);
}

bool Asterix034Proto::generate_050(AsterixData &map, QDataStream &stream)
{
    PresentData subMask;
    QByteArray subArray;
    QDataStream subStream(&subArray, QIODevice::WriteOnly);

    if (isContain(map, StationConfigurationStatus_COM_NOGO, StationConfigurationStatus_COM_RDPC, StationConfigurationStatus_COM_RDPR, StationConfigurationStatus_COM_OVLRDP
                  , StationConfigurationStatus_COM_OVLXMT, StationConfigurationStatus_COM_MSC, StationConfigurationStatus_COM_TSV))
    {
        subMask.set(1);
        quint8 data = (map.value(StationConfigurationStatus_COM_NOGO).toUInt() & M1) << 7;
        data |= (map.value(StationConfigurationStatus_COM_RDPC).toUInt() & M1) << 6;
        data |= (map.value(StationConfigurationStatus_COM_RDPR).toUInt() & M1) << 5;
        data |= (map.value(StationConfigurationStatus_COM_OVLRDP).toUInt() & M1) << 4;
        data |= (map.value(StationConfigurationStatus_COM_OVLXMT).toUInt() & M1) << 3;
        data |= (map.value(StationConfigurationStatus_COM_MSC).toUInt() & M1) << 2;
        data |= (map.value(StationConfigurationStatus_COM_TSV).toUInt() & M1) << 1;
        write8(subStream, data);
    }

    if (isContain(map, StationConfigurationStatus_PSR_ANT, StationConfigurationStatus_PSR_CHAB
                  , StationConfigurationStatus_PSR_OVL, StationConfigurationStatus_PSR_MSC))
    {
        subMask.set(4);
        quint8 data = 0;
        data |= (map.value(StationConfigurationStatus_PSR_ANT).toUInt() & M1) << 7;
        data |= (map.value(StationConfigurationStatus_PSR_CHAB).toUInt() & M2) << 5;
        data |= (map.value(StationConfigurationStatus_PSR_OVL).toUInt() & M1) << 4;
        data |= (map.value(StationConfigurationStatus_PSR_MSC).toUInt() & M1) << 3;
        write8(subStream, data);
    }

    if (isContain(map, StationConfigurationStatus_SSR_ANT, StationConfigurationStatus_SSR_CHAB
                  , StationConfigurationStatus_SSR_OVL, StationConfigurationStatus_SSR_MSC))
    {
        subMask.set(5);
        quint8 data = 0;
        data |= (map.value(StationConfigurationStatus_SSR_ANT).toUInt() & M1) << 7;
        data |= (map.value(StationConfigurationStatus_SSR_CHAB).toUInt() & M2) << 5;
        data |= (map.value(StationConfigurationStatus_SSR_OVL).toUInt() & M1) << 4;
        data |= (map.value(StationConfigurationStatus_SSR_MSC).toUInt() & M1) << 3;
        write8(subStream, data);
    }

    if (isContain(map, StationConfigurationStatus_MDS_ANT, StationConfigurationStatus_MDS_CHAB, StationConfigurationStatus_MDS_OVLSUR
                  , StationConfigurationStatus_MDS_MSC, StationConfigurationStatus_MDS_SCF, StationConfigurationStatus_MDS_DLF
                  , StationConfigurationStatus_MDS_OVLSCF, StationConfigurationStatus_MDS_OVLDLF))
    {
        subMask.set(6);
        quint16 data = 0;
        data |= (map.value(StationConfigurationStatus_MDS_ANT).toUInt() & M1) << 15;
        data |= (map.value(StationConfigurationStatus_MDS_CHAB).toUInt() & M2) << 13;
        data |= (map.value(StationConfigurationStatus_MDS_OVLSUR).toUInt() & M1) << 12;
        data |= (map.value(StationConfigurationStatus_MDS_MSC).toUInt() & M1) << 11;
        data |= (map.value(StationConfigurationStatus_MDS_SCF).toUInt() & M1) << 10;
        data |= (map.value(StationConfigurationStatus_MDS_DLF).toUInt() & M2) << 9;
        data |= (map.value(StationConfigurationStatus_MDS_OVLSCF).toUInt() & M1) << 8;
        data |= (map.value(StationConfigurationStatus_MDS_OVLDLF).toUInt() & M1) << 7;
        write16(subStream, data);
    }

    if (!subMask.isEmpty())
    {
        writeArray(stream, subMask.data());
        writeArray(stream, subArray);
        return true;
    }
    return false;
}

void Asterix034Proto::parse_060(QDataStream &stream, AsterixData &map)
{
    PresentData mask(stream);

    if (mask.size() > 1)
        map.appendError(AsterixErrors::Error_FX);

    if (mask.isSet(1))
    {
        quint8 data = read<quint8>(stream);
        map.insert(StationProcessingMode_COM_REDRDP, data >> 4 & M3);
        map.insert(StationProcessingMode_COM_REDXMT, data >> 1 & M3);
    }

    if (mask.isSet(2))
        map.appendError(AsterixErrors::Error_FX);

    if (mask.isSet(3))
        map.appendError(AsterixErrors::Error_FX);

    if (mask.isSet(4))
    {
        quint8 data = read<quint8>(stream);
        map.insert(StationProcessingMode_PSR_POL, data >> 7 & M1);
        map.insert(StationProcessingMode_PSR_REDRAD, data >> 4 & M3);
        map.insert(StationProcessingMode_PSR_STC, data >> 2 & M2);
    }

    if (mask.isSet(5))
    {
        quint8 data = read<quint8>(stream);
        map.insert(StationProcessingMode_SSR_REDRAD, data >> 5 & M3);
    }

    if (mask.isSet(6))
    {
        quint8 data = read<quint8>(stream);
        map.insert(StationProcessingMode_MDS_REDRAD, data >> 5 & M3);
        map.insert(StationProcessingMode_MDS_CLU, data >> 4 & M1);
    }

    if (mask.isSet(7))
        map.appendError(AsterixErrors::Error_FX);
}

bool Asterix034Proto::generate_060(AsterixData &map, QDataStream &stream)
{
    PresentData subMask;
    QByteArray subArray;
    QDataStream subStream(&subArray, QIODevice::WriteOnly);

    if (isContain(map, StationProcessingMode_COM_REDRDP, StationProcessingMode_COM_REDXMT))
    {
        subMask.set(1);
        quint8 data = 0;
        data |= (map.value(StationProcessingMode_COM_REDRDP).toUInt() & M3) << 4;
        data |= (map.value(StationProcessingMode_COM_REDXMT).toUInt() & M3) << 1;
        write8(subStream, data);
    }

    if (isContain(map, StationProcessingMode_PSR_POL, StationProcessingMode_PSR_REDRAD
                  , StationProcessingMode_PSR_STC))
    {
        subMask.set(4);
        quint8 data = 0;
        data |= (map.value(StationProcessingMode_PSR_POL).toUInt() & M1) << 7;
        data |= (map.value(StationProcessingMode_PSR_REDRAD).toUInt() & M3) << 4;
        data |= (map.value(StationProcessingMode_PSR_STC).toUInt() & M2) << 2;
        write8(subStream, data);
    }

    if (isContain(map, StationProcessingMode_SSR_REDRAD))
    {
        subMask.set(5);
        write8(subStream, (map.value(StationProcessingMode_SSR_REDRAD).toUInt() & M3) << 5);
    }

    if (isContain(map, StationProcessingMode_MDS_REDRAD, StationProcessingMode_MDS_CLU))
    {
        subMask.set(6);
        quint8 data = 0;
        data |= (map.value(StationProcessingMode_MDS_REDRAD).toUInt() & M3) << 5;
        data |= (map.value(StationProcessingMode_MDS_CLU).toUInt() & M1) << 4;
        write8(subStream, data);
    }

    if (!subMask.isEmpty())
    {
        writeArray(stream, subMask.data());
        writeArray(stream, subArray);
        return true;
    }
    return false;
}

void Asterix034Proto::parse_070(QDataStream &stream, AsterixData &map)
{
    QVariantList repetitionField;
    for (quint8 rep = read<quint8>(stream); 0 != rep; --rep)
    {
        quint16 data = read<quint16>(stream);
        I034_070 val = {0};
        val.TYP = data >> 11 & M5;
        val.COUNTER = data & M11;
        repetitionField.push_back(QVariant::fromValue(val));
    }
    if (!repetitionField.isEmpty())
        map.insert(MessageCountValues, repetitionField);
}

bool Asterix034Proto::generate_070(AsterixData &map, QDataStream &stream)
{
    if (map.contains(MessageCountValues))
    {
        QVariantList data = map.value(MessageCountValues).toList();
        if (!data.isEmpty())
        {
            write8(stream, calculateRepetition(data));
            foreach (const QVariant &variantData, data)
            {
                I034_070 data = variantData.value<I034_070>();
                quint16 result = 0;
                result |= (data.TYP & M5) << 11;
                result |= data.COUNTER & M11;
                write16(stream, result);
            }
            return true;
        }
    }
    return false;
}

void Asterix034Proto::parse_090(QDataStream &stream, AsterixData &map)
{
    map.insert(CollimationError_RangeError, double(read<qint8>(stream)) / 128);
    map.insert(CollimationError_AzimuthError, double(read<qint8>(stream)) * 360 / 16384);
}

bool Asterix034Proto::generate_090(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, CollimationError_RangeError, CollimationError_AzimuthError ))
    {
        write8(stream, map.value(CollimationError_RangeError).toDouble() * 128);
        write8(stream, map.value(CollimationError_AzimuthError).toDouble() * 16384 / 360);
        return true;
    }
    return false;
}

void Asterix034Proto::parse_100(QDataStream &stream, AsterixData &map)
{
    map.insert(GenericPolarWindow_RhoStart, double(read<quint16>(stream)) / 256);
    map.insert(GenericPolarWindow_RhoEnd, double(read<quint16>(stream)) / 256);
    map.insert(GenericPolarWindow_ThetaStart, double(read<quint16>(stream)) * 360 / 65536);
    map.insert(GenericPolarWindow_ThetaEnd, double(read<quint16>(stream)) * 360 / 65536);
}

bool Asterix034Proto::generate_100(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, GenericPolarWindow_RhoStart, GenericPolarWindow_RhoEnd, GenericPolarWindow_ThetaStart, GenericPolarWindow_ThetaEnd))
    {
        write16(stream, map.value(GenericPolarWindow_RhoStart).toDouble() * 256);
        write16(stream, map.value(GenericPolarWindow_RhoEnd).toDouble() * 256);
        write16(stream, map.value(GenericPolarWindow_ThetaStart).toDouble() * 65536 / 360);
        write16(stream, map.value(GenericPolarWindow_ThetaEnd).toDouble() * 65536 / 360);
        return true;
    }
    return false;
}

void Asterix034Proto::parse_110(QDataStream &stream, AsterixData &map)
{
    map.insert(DataFilter, read<quint8>(stream));
}

bool Asterix034Proto::generate_110(AsterixData &map, QDataStream &stream)
{
    if (map.contains(DataFilter))
    {
        write8(stream, map.value(DataFilter).toUInt());
        return true;
    }
    return false;
}

void Asterix034Proto::parse_120(QDataStream &stream, AsterixData &map)
{
    map.insert(PositionOfSource3D_Height, read<quint16>(stream));
    map.insert(PositionOfSource3D_Latitude, double(read24s(stream)) * 180 / 8388608);
    map.insert(PositionOfSource3D_Longitude, double(read24s(stream)) * 180 / 8388608);
}

bool Asterix034Proto::generate_120(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, PositionOfSource3D_Height, PositionOfSource3D_Latitude, PositionOfSource3D_Longitude))
    {
        write16(stream, map.value(PositionOfSource3D_Height).toUInt());
        write24(stream, map.value(PositionOfSource3D_Latitude).toDouble() * 8388608 / 180);
        write24(stream, map.value(PositionOfSource3D_Longitude).toDouble() * 8388608 / 180);
        return true;
    }
    return false;
}
