#include "asterix004.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>


const int Asterix004Proto::mCategory = 4;
const QString Asterix004Proto::mEdition = "1.9";

const QString Asterix004Proto::MessageType = "a004.MessageType";

const QString Asterix004Proto::DataSourceIdentifier_SAC = ASTERIX_SAC;
const QString Asterix004Proto::DataSourceIdentifier_SIC = ASTERIX_SIC;

const QString Asterix004Proto::SDPSIdentifier = "a004.SDPSIdentifier";

const QString Asterix004Proto::TimeOfMessage = "a004.TimeOfMessage";

const QString Asterix004Proto::TrackNumber1 = "a004.TrackNumber1";
const QString Asterix004Proto::TrackNumber2 = "a004.TrackNumber2";

const QString Asterix004Proto::AlertIdentifier = "a004.AlertIdentifier";

const QString Asterix004Proto::AlertStatus_STAT = "a004.AlertStatus.STAT";

const QString Asterix004Proto::SafetyNetFunctionAndSystemStatus = "a004.SafetyNetFunctionAndSystemStatus";

const QString Asterix004Proto::ConflictTimingAndSeparation_TC = "a004.ConflictTimingAndSeparation.TC";
const QString Asterix004Proto::ConflictTimingAndSeparation_TCA = "a004.ConflictTimingAndSeparation.TCA";
const QString Asterix004Proto::ConflictTimingAndSeparation_CHS = "a004.ConflictTimingAndSeparation.CHS";
const QString Asterix004Proto::ConflictTimingAndSeparation_MHS = "a004.ConflictTimingAndSeparation.MHS";
const QString Asterix004Proto::ConflictTimingAndSeparation_CVS = "a004.ConflictTimingAndSeparation.CVS";
const QString Asterix004Proto::ConflictTimingAndSeparation_MVS = "a004.ConflictTimingAndSeparation.MVS";

const QString Asterix004Proto::LongitudinalDeviation = "a004.LongitudinalDeviation";

const QString Asterix004Proto::TransversalDistanceDeviation = "a004.TransversalDistanceDeviation";

const QString Asterix004Proto::VerticalDeviation = "a004.VerticalDeviation";

const QString Asterix004Proto::AreaDefinitions = "a004.AreaDefinitions";

const QString Asterix004Proto::FDPSSectorControlPositions = "a004.FDPSSectorControlPositions";

const QString Asterix004Proto::ConflictCharacteristics_MAS = "a004.ConflictCharacteristics.MAS";
const QString Asterix004Proto::ConflictCharacteristics_CAS = "a004.ConflictCharacteristics.CAS";
const QString Asterix004Proto::ConflictCharacteristics_FLD = "a004.ConflictCharacteristics.FLD";
const QString Asterix004Proto::ConflictCharacteristics_FVD = "a004.ConflictCharacteristics.FVD";
const QString Asterix004Proto::ConflictCharacteristics_Type = "a004.ConflictCharacteristics.Type";
const QString Asterix004Proto::ConflictCharacteristics_Cross = "a004.ConflictCharacteristics.Cross";
const QString Asterix004Proto::ConflictCharacteristics_Div = "a004.ConflictCharacteristics.Div";
const QString Asterix004Proto::ConflictCharacteristics_RRC = "a004.ConflictCharacteristics.RRC";
const QString Asterix004Proto::ConflictCharacteristics_RTC = "a004.ConflictCharacteristics.RTC";
const QString Asterix004Proto::ConflictCharacteristics_MRVA = "a004.ConflictCharacteristics.MRVA";
const QString Asterix004Proto::ConflictCharacteristics_VRAM_CRM = "a004.ConflictCharacteristics.VRAM.CRM";
const QString Asterix004Proto::ConflictCharacteristics_VRAM_VRM = "a004.ConflictCharacteristics.VRAM.VRM";
const QString Asterix004Proto::ConflictCharacteristics_VRAM_VTM = "a004.ConflictCharacteristics.VRAM.VTM";
const QString Asterix004Proto::ConflictCharacteristics_HAM_HD = "a004.ConflictCharacteristics.HAM.HD";
const QString Asterix004Proto::ConflictCharacteristics_HAM_RD = "a004.ConflictCharacteristics.HAM.RD";
const QString Asterix004Proto::ConflictCharacteristics_HAM_VD = "a004.ConflictCharacteristics.HAM.VD";
const QString Asterix004Proto::ConflictCharacteristics_DBPSM_ARR = "a004.ConflictCharacteristics.DBPSM.ARR";
const QString Asterix004Proto::ConflictCharacteristics_DBPSM_DEP = "a004.ConflictCharacteristics.DBPSM.DEP";
const QString Asterix004Proto::ConflictCharacteristics_DBPSM_TL = "a004.ConflictCharacteristics.DBPSM.TL";
const QString Asterix004Proto::ConflictCharacteristics_AIW = "a004.ConflictCharacteristics.AIW";
const QString Asterix004Proto::ConflictCharacteristics_TableId = "a004.ConflictCharacteristics.TableId";
const QString Asterix004Proto::ConflictCharacteristics_ConflictProperties = "a004.ConflictCharacteristics.ConflictProperties";
const QString Asterix004Proto::ConflictCharacteristics_CS = "a004.ConflictCharacteristics.CS";
const QString Asterix004Proto::ConflictCharacteristics_Probability = "a004.ConflictCharacteristics.Probability";
const QString Asterix004Proto::ConflictCharacteristics_Duration = "a004.ConflictCharacteristics.Duration";

const QString Asterix004Proto::AircraftIdentificationAndCharacteristics1 = "a004.";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics2 = "a004.";

const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_AI = "a004.AircraftIdentificationAndCharacteristics.AI";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_M3 = "a004.AircraftIdentificationAndCharacteristics.M3";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_CPW_Latitude = "a004.AircraftIdentificationAndCharacteristics.CPW.Latitude";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_CPW_Longitude = "a004.AircraftIdentificationAndCharacteristics.CPW.Longitude";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_CPW_Altitude = "a004.AircraftIdentificationAndCharacteristics.CPW.Altitude";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_CPC_X = "a004.AircraftIdentificationAndCharacteristics.CPC.X";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_CPC_Y = "a004.AircraftIdentificationAndCharacteristics.CPC.Y";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_CPC_Z = "a004.AircraftIdentificationAndCharacteristics.CPC.Z";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_TT = "a004.AircraftIdentificationAndCharacteristics.TT";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_DT = "a004.AircraftIdentificationAndCharacteristics.DT";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_AC_GAT_OAT = "a004.AircraftIdentificationAndCharacteristics.AC.GAT.OAT";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_AC_FR1_FR2 = "a004.AircraftIdentificationAndCharacteristics.AC.FR1.FR2";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_AC_RVSM = "a004.AircraftIdentificationAndCharacteristics.AC.RVSM";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_AC_HPR = "a004.AircraftIdentificationAndCharacteristics.AC.HPR";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_AC_CDM = "a004.AircraftIdentificationAndCharacteristics.AC.CDM";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_AC_PRI = "a004.AircraftIdentificationAndCharacteristics.AC.PRI";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_AC_GV = "a004.AircraftIdentificationAndCharacteristics.AC.GV";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_MS = "a004.AircraftIdentificationAndCharacteristics.MS";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_FP = "a004.AircraftIdentificationAndCharacteristics.FP";
const QString Asterix004Proto::AircraftIdentificationAndCharacteristics_CF = "a004.AircraftIdentificationAndCharacteristics.CF";


Asterix004Proto::Asterix004Proto()
{}

int Asterix004Proto::category() const
{
    return mCategory;
}

QString Asterix004Proto::edition() const
{
    return mEdition;
}

void Asterix004Proto::parse(QDataStream &stream, AsterixData &asterix)
{
    try
    {
        PresentData mainMask(stream);

        if (mainMask.isSet(1))
            parse_010(stream, asterix);
        else
            asterix.appendError(mandatory("Data Source Identifier"));

        if (mainMask.isSet(2))
            parse_000(stream, asterix);
        else
            throw mandatory("Message Type");

        int messageType = asterix.value(MessageType).toUInt();
        // parse only STCA
        if (messageType == 7)
        {
            if (mainMask.isSet(3))
                parse_015(stream, asterix);

            if (mainMask.isSet(4))
                parse_020(stream, asterix);
            else
                asterix.appendError(mandatory("Time Of Message"));

            if (mainMask.isSet(5))
                parse_040(stream, asterix);
            else
                asterix.appendError(mandatory("Alert Identifier"));

            if (mainMask.isSet(6))
                parse_045(stream, asterix);

            if (mainMask.isSet(7))
                asterix.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(8))
                parse_030(stream, asterix);
            else
                asterix.appendError(mandatory("Track Number 1"));

            if (mainMask.isSet(9))
                parse_170(stream, asterix);

            if (mainMask.isSet(10))
                parse_120(stream, asterix);
            else
                asterix.appendError(mandatory("Conflict Characteristics"));

            if (mainMask.isSet(11))
                parse_070(stream, asterix);

            if (mainMask.isSet(12))
                asterix.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(13))
                asterix.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(14))
                asterix.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(15))
                asterix.appendError(AsterixErrors::Error_FSPEC);

            if (mainMask.isSet(16))
                parse_035(stream, asterix);
            else
                asterix.appendError(mandatory("Track Number 2"));

            if (mainMask.isSet(17))
                parse_171(stream, asterix);

            if (mainMask.isSet(18))
                parse_110(stream, asterix);
        }
    }
    catch (AsterixErrors::ErrorCode error)
    {
        asterix.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        asterix.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            asterix.appendError(AsterixErrors::Error_UnexpectedEndOfData);
    }
}

QByteArray Asterix004Proto::generate(AsterixData &map)
{
    return QByteArray();
}

QStringList Asterix004Proto::fields() const
{
    return QStringList();
}

void Asterix004Proto::parse_000(QDataStream &stream, AsterixData &map)
{
    map.insert(MessageType, read<quint8>(stream));
}

void Asterix004Proto::parse_010(QDataStream &stream, AsterixData &map)
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

void Asterix004Proto::parse_015(QDataStream &stream, AsterixData &map)
{
    QList<SDPS> result;
    for (quint8 i = read<quint8>(stream); i; --i)
    {
        SDPS id;
        id.sac = read<quint8>(stream);
        id.sic = read<quint8>(stream);

        result.append(id);
    }

    map.insert(SDPSIdentifier, QVariant::fromValue(result));
}

void Asterix004Proto::parse_020(QDataStream &stream, AsterixData &map)
{
    map.insert(TimeOfMessage, QTime().addSecs(qRound(read24(stream) / 128.0)));
}

void Asterix004Proto::parse_030(QDataStream &stream, AsterixData &map)
{
    map.insert(TrackNumber1, read<quint16>(stream));
}

void Asterix004Proto::parse_035(QDataStream &stream, AsterixData &map)
{
    map.insert(TrackNumber2, read<quint16>(stream));
}

void Asterix004Proto::parse_040(QDataStream &stream, AsterixData &map)
{
    map.insert(AlertIdentifier, read<quint16>(stream));
}

void Asterix004Proto::parse_045(QDataStream &stream, AsterixData &map)
{
    quint8 data = read<quint8>(stream);

    map.insert(AlertStatus_STAT, data >> 1 & M3);
}

void Asterix004Proto::parse_070(QDataStream &stream, AsterixData &map)
{
    PresentData mask(stream);

    if (mask.isSet(1))
    {
        map.insert(ConflictTimingAndSeparation_TC, read24(stream) / 128.0);
    }
    if (mask.isSet(2))
    {
        map.insert(ConflictTimingAndSeparation_TCA, read24(stream) / 128.0);
    }
    if (mask.isSet(3))
    {
        map.insert(ConflictTimingAndSeparation_CHS, read24(stream) * 0.5);
    }
    if (mask.isSet(4))
    {
        map.insert(ConflictTimingAndSeparation_MHS, read<quint16>(stream) * 0.5);
    }
    if (mask.isSet(5))
    {
        map.insert(ConflictTimingAndSeparation_CVS, read<quint16>(stream) * 25);
    }
    if (mask.isSet(6))
    {
        map.insert(ConflictTimingAndSeparation_MVS, read<quint16>(stream) * 25);
    }
}

void Asterix004Proto::parse_110(QDataStream &stream, AsterixData &map)
{
    QList<FDPS> result;
    for (quint8 i = read<quint8>(stream); i; --i)
    {
        FDPS id;
        id.centre = read<quint8>(stream);
        id.position = read<quint8>(stream);

        result.append(id);
    }

    map.insert(SDPSIdentifier, QVariant::fromValue(result));
}

void Asterix004Proto::parse_120(QDataStream &stream, AsterixData &map)
{
    PresentData mask(stream);

    if (mask.isSet(1))
    {
        quint8 data = read<quint8>(stream);
        map.insert(ConflictCharacteristics_MAS, data >> 7 & M1);
        map.insert(ConflictCharacteristics_CAS, data >> 6 & M1);
        map.insert(ConflictCharacteristics_FLD, data >> 5 & M1);
        map.insert(ConflictCharacteristics_FVD, data >> 4 & M1);
        map.insert(ConflictCharacteristics_Type, data >> 3 & M1);
        map.insert(ConflictCharacteristics_Cross, data >> 2 & M1);
        map.insert(ConflictCharacteristics_Div, data >> 1 & M1);

        if (data & M1)
        {
            data = read<quint8>(stream);
            map.insert(ConflictCharacteristics_RRC, data >> 7 & M1);
            map.insert(ConflictCharacteristics_RTC, data >> 6 & M1);
            map.insert(ConflictCharacteristics_MRVA, data >> 5 & M1);
            map.insert(ConflictCharacteristics_VRAM_CRM, data >> 4 & M1);
            map.insert(ConflictCharacteristics_VRAM_VRM, data >> 3 & M1);
            map.insert(ConflictCharacteristics_VRAM_VTM, data >> 2 & M1);
            map.insert(ConflictCharacteristics_HAM_HD, data >> 1 & M1);

            if (data & M1)
            {
                data = read<quint8>(stream);
                map.insert(ConflictCharacteristics_HAM_RD, data >> 7 & M1);
                map.insert(ConflictCharacteristics_HAM_VD, data >> 6 & M1);
                map.insert(ConflictCharacteristics_DBPSM_ARR, data >> 5 & M1);
                map.insert(ConflictCharacteristics_DBPSM_DEP, data >> 4 & M1);
                map.insert(ConflictCharacteristics_DBPSM_TL, data >> 3 & M1);
                map.insert(ConflictCharacteristics_AIW, data >> 2 & M1);

                while (data & M1)
                    data = read<quint8>(stream);
            }
        }
    }
    if (mask.isSet(2))
    {
        quint8 data = read<quint8>(stream);
        map.insert(ConflictCharacteristics_TableId, data >> 4 & M4);
        map.insert(ConflictCharacteristics_ConflictProperties, data >> 1 & M3);
        map.insert(ConflictCharacteristics_CS, data & M1);
    }
    if (mask.isSet(3))
    {
        map.insert(ConflictCharacteristics_Probability, read<quint8>(stream) * 0.5);
    }
    if (mask.isSet(4))
    {
        map.insert(ConflictCharacteristics_Duration, read24(stream) / 128.0);
    }
}

void Asterix004Proto::parse_170(QDataStream &stream, AsterixData &map)
{
    map.insert(AircraftIdentificationAndCharacteristics1, parseAircraftIdentificationAndCharacteristics(stream));
}

void Asterix004Proto::parse_171(QDataStream &stream, AsterixData &map)
{
    map.insert(AircraftIdentificationAndCharacteristics2, parseAircraftIdentificationAndCharacteristics(stream));
}

QVariantMap Asterix004Proto::parseAircraftIdentificationAndCharacteristics(QDataStream &stream)
{
    QVariantMap map;

    PresentData mask(stream);
    if (mask.isSet(1))
    {
        map.insert(AircraftIdentificationAndCharacteristics_AI, readArray(stream, 7));
    }
    if (mask.isSet(2))
    {
        map.insert(AircraftIdentificationAndCharacteristics_M3, read<quint16>(stream));
    }
    if (mask.isSet(3))
    {
        map.insert(AircraftIdentificationAndCharacteristics_CPW_Latitude, read<qint32>(stream) * 180.0 / 33554432);
        map.insert(AircraftIdentificationAndCharacteristics_CPW_Longitude, read<qint32>(stream) * 180.0 / 33554432);
        map.insert(AircraftIdentificationAndCharacteristics_CPW_Altitude, read<qint16>(stream) * 25);
    }
    if (mask.isSet(4))
    {
        map.insert(AircraftIdentificationAndCharacteristics_CPC_X, read24s(stream) * 0.5);
        map.insert(AircraftIdentificationAndCharacteristics_CPC_Y, read24s(stream) * 0.5);
        map.insert(AircraftIdentificationAndCharacteristics_CPC_Z, read<qint16>(stream) * 25);
    }
    if (mask.isSet(5))
    {
        map.insert(AircraftIdentificationAndCharacteristics_TT, read24(stream) / 128.0);
    }
    if (mask.isSet(6))
    {
        map.insert(AircraftIdentificationAndCharacteristics_DT, read<quint16>(stream) * 0.5);
    }
    if (mask.isSet(7))
    {
        quint8 data = read<quint8>(stream);
        map.insert(AircraftIdentificationAndCharacteristics_AC_GAT_OAT, data >> 6 & M2);
        map.insert(AircraftIdentificationAndCharacteristics_AC_FR1_FR2, data >> 4 & M2);
        map.insert(AircraftIdentificationAndCharacteristics_AC_RVSM, data >> 2 & M2);
        map.insert(AircraftIdentificationAndCharacteristics_AC_HPR, data >> 1 & M1);

        if (data & M1)
        {
            data = read<quint8>(stream);
            map.insert(AircraftIdentificationAndCharacteristics_AC_CDM, data >> 6 & M2);
            map.insert(AircraftIdentificationAndCharacteristics_AC_PRI, data >> 5 & M1);
            map.insert(AircraftIdentificationAndCharacteristics_AC_GV, data >> 4 & M1);

            while (data & M1)
                data = read<quint8>(stream);
        }
    }
    if (mask.isSet(8))
    {
        map.insert(AircraftIdentificationAndCharacteristics_MS, readCharacters(stream, 2));
    }
    if (mask.isSet(9))
    {
        quint32 data = read<quint32>(stream);

        map.insert(AircraftIdentificationAndCharacteristics_FP, data & M27);
    }
    if (mask.isSet(10))
    {
        map.insert(AircraftIdentificationAndCharacteristics_CF, read<quint16>(stream) / 4.0);
    }

    return map;
}
