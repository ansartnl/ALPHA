#include "asterix002.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>

#include <math.h>


const int Asterix002Proto::mCategory = 2;
const QString Asterix002Proto::mEdition = "1.0";

const QString Asterix002Proto::MessageType = "a002.MessageType";

const QString Asterix002Proto::DataSourceIdentifier_SAC = ASTERIX_SAC;
const QString Asterix002Proto::DataSourceIdentifier_SIC = ASTERIX_SIC;

const QString Asterix002Proto::SectorNumber = "a002.SectorNumber";

const QString Asterix002Proto::TimeOfDay = "a002.TimeOfDay";

const QString Asterix002Proto::AntennaRotationSpeed = "a002.AntennaRotationSpeed";

const QString Asterix002Proto::StationConfigurationStatus = "a002.StationConfigurationStatus";

const QString Asterix002Proto::StationProcessingMode = "a002.StationProcessingMode";

const QString Asterix002Proto::PlotCountValues = "a002.PlotCountValues";

const QString Asterix002Proto::WarningErrorConditions = "a002.WarningErrorConditions";

const QString Asterix002Proto::Collimation_ErrorRangeError = "a002.CollimationError.RangeError";
const QString Asterix002Proto::Collimation_ErrorAzimuthError = "a002.CollimationError.AzimuthError";

const QString Asterix002Proto::DynamicWindow_RhoStart = "a002.DynamicWindow.RhoStart";
const QString Asterix002Proto::DynamicWindow_RhoEnd = "a002.DynamicWindow.RhoEnd";
const QString Asterix002Proto::DynamicWindow_ThetaStart = "a002.DynamicWindow.ThetaStart";
const QString Asterix002Proto::DynamicWindow_ThetaEnd = "a002.DynamicWindow.ThetaEnd";

Asterix002Proto::Asterix002Proto()
{}

int Asterix002Proto::category() const
{
    return mCategory;
}

QString Asterix002Proto::edition() const
{
    return mEdition;
}

void Asterix002Proto::parse(QDataStream &stream, AsterixData &asterix)
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

        if (mainMask.isSet(3))
            parse_020(stream, asterix);
        else
            asterix.appendError(mandatory("Sector Number"));

        if (mainMask.isSet(4))
            parse_030(stream, asterix);

        if (mainMask.isSet(5))
            parse_041(stream, asterix);

        if (mainMask.isSet(6))
            parse_050(stream, asterix);

        if (mainMask.isSet(7))
            parse_060(stream, asterix);

        if (mainMask.isSet(8))
            parse_070(stream, asterix);

        if (mainMask.isSet(9))
            parse_100(stream, asterix);
        else if (ActivationBlindZoneFiltering == EMessageType(asterix.value(MessageType).toUInt()))
            asterix.appendError(mandatory("Dynamic Window"));

        if (mainMask.isSet(10))
            parse_090(stream, asterix);

        if (mainMask.isSet(11))
            parse_080(stream, asterix);

        if (mainMask.isSet(13))
            parse_sp(stream, asterix);

        if (mainMask.isSet(14))
            parse_re(stream, asterix);
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

QByteArray Asterix002Proto::generate(AsterixData &map)
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

        if (generate_020(map, stream))
            mainMask.set(3);
        else
            map.appendError(mandatory("Sector Number"));

        if (generate_030(map, stream))
            mainMask.set(4);

        if (generate_041(map, stream))
            mainMask.set(5);

        if (generate_050(map, stream))
            mainMask.set(6);

        if (generate_060(map, stream))
            mainMask.set(7);

        if (generate_070(map, stream))
            mainMask.set(8);

        if (generate_100(map, stream))
            mainMask.set(9);
        else if (ActivationBlindZoneFiltering == EMessageType(map.value(MessageType).toUInt()))
            map.appendError(mandatory("Dynamic Window"));

        if (generate_090(map, stream))
            mainMask.set(10);

        if (generate_080(map, stream))
            mainMask.set(11);

        if (generate_sp(map, stream))
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

QStringList Asterix002Proto::fields() const
{
    static QStringList result = QStringList()

    << MessageType

    << DataSourceIdentifier_SAC
    << DataSourceIdentifier_SIC

    << SectorNumber

    << TimeOfDay

    << AntennaRotationSpeed

    << StationConfigurationStatus

    << StationProcessingMode

    << PlotCountValues

    << WarningErrorConditions

    << Collimation_ErrorRangeError
    << Collimation_ErrorAzimuthError

    << DynamicWindow_RhoStart
    << DynamicWindow_RhoEnd
    << DynamicWindow_ThetaStart
    << DynamicWindow_ThetaEnd;

    return result;
}

void Asterix002Proto::parse_000(QDataStream &stream, AsterixData &map)
{
    map.insert(MessageType, read<quint8>(stream));
}

bool Asterix002Proto::generate_000(AsterixData &map, QDataStream &stream)
{
    if (map.contains(MessageType))
    {
        write8(stream, map.value(MessageType).toUInt());
        return true;
    }
    return false;
}

void Asterix002Proto::parse_010(QDataStream &stream, AsterixData &map)
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

bool Asterix002Proto::generate_010(AsterixData &map, QDataStream &stream)
{
    if (map.contains(DataSourceIdentifier_SAC) && map.contains(DataSourceIdentifier_SIC))
    {
        write8(stream, map.value(DataSourceIdentifier_SAC).toUInt());
        write8(stream, map.value(DataSourceIdentifier_SIC).toUInt());
        return true;
    }
    return false;
}

void Asterix002Proto::parse_020(QDataStream &stream, AsterixData &map)
{
    map.insert(SectorNumber, double(read<quint8>(stream)) * 360 / 256);
}

bool Asterix002Proto::generate_020(AsterixData &map, QDataStream &stream)
{
    if (map.contains(SectorNumber))
    {
        write8(stream, map.value(SectorNumber).toDouble() * 256 / 360);
        return true;
    }
    return false;
}

void Asterix002Proto::parse_030(QDataStream &stream, AsterixData &map)
{
    map.insert(TimeOfDay, QTime().addSecs(ceil(double(read24(stream)) / 128)));
}

bool Asterix002Proto::generate_030(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TimeOfDay))
    {
        write24(stream, double(map.value(TimeOfDay).toTime().secsTo(QTime(0,0,0,0))) * -128);
        return true;
    }
    return false;
}

void Asterix002Proto::parse_041(QDataStream &stream, AsterixData &map)
{
    map.insert(AntennaRotationSpeed, double(read<quint16>(stream)) / 128);
}

bool Asterix002Proto::generate_041(AsterixData &map, QDataStream &stream)
{
    if (map.contains(AntennaRotationSpeed))
    {
        write16(stream, map.value(AntennaRotationSpeed).toDouble() * 128);
        return true;
    }
    return false;
}

void Asterix002Proto::parse_050(QDataStream &stream, AsterixData &map)
{
    quint8 data;
    QList<quint8> result;
    do
    {
        data = read<quint8>(stream);
        result.push_back(data >> 1 & M7);
    } while(data & M1);
    map.insert(StationConfigurationStatus, QVariant::fromValue(result));
}

bool Asterix002Proto::generate_050(AsterixData &map, QDataStream &stream)
{
    if (map.contains(StationConfigurationStatus))
    {
        const QList<quint8> extension = map.value(StationConfigurationStatus).value< QList<quint8> >();
        if (extension.isEmpty())
            return false;

        quint8 data = (extension.first() & M7) << 1;
        foreach (quint8 val, extension)
        {
            write8(stream, data | M1);
            data = (val & M7) << 1;
        }
        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix002Proto::parse_060(QDataStream &stream, AsterixData &map)
{
    quint8 data;
    QList<quint8> result;
    do
    {
        data = read<quint8>(stream);
        result.push_back(data >> 1 & M7);
    } while(data & M1);
    map.insert(StationProcessingMode, QVariant::fromValue(result));
}

bool Asterix002Proto::generate_060(AsterixData &map, QDataStream &stream)
{
    if (map.contains(StationProcessingMode))
    {
        const QList<quint8> extension = map.value(StationProcessingMode).value< QList<quint8> >();
        if (extension.isEmpty())
            return false;

        quint8 data = (extension.first() & M7) << 1;
        foreach (quint8 val, extension)
        {
            write8(stream, data | M1);
            data = (val & M7) << 1;
        }
        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix002Proto::parse_070(QDataStream &stream, AsterixData &map)
{
    QList<I002_070> repetitionField;
    for (quint8 rep = read<quint8>(stream); 0 != rep; --rep)
    {
        quint16 data = read<quint16>(stream);
        I002_070 val;
        val.A = data >> 15 & M1;
        val.IDENT = data >> 10 & M5;
        val.COUNTER = data & M10;
        repetitionField.push_back(val);
    }
    if (!repetitionField.isEmpty())
        map.insert(PlotCountValues, QVariant::fromValue(repetitionField));
}

bool Asterix002Proto::generate_070(AsterixData &map, QDataStream &stream)
{
    if (map.contains(PlotCountValues))
    {
        QList<I002_070> extension = map.value(PlotCountValues).value< QList<I002_070> >();
        quint8 rep = calculateRepetition(extension);
        if (0 == rep)
        {
            map.appendError(AsterixErrors::Error_REP);
            return false;
        }

        write8(stream, rep);
        foreach (const I002_070 &val, extension)
        {
            quint16 data = (val.A & M1) << 15;
            data |= (val.IDENT & M5) << 10;
            data |= val.COUNTER & M10;
            write16(stream, data);
        }
        return true;
    }
    return false;
}

void Asterix002Proto::parse_080(QDataStream &stream, AsterixData &map)
{
    quint8 data;
    QList<quint8> result;
    do
    {
        data = read<quint8>(stream);
        result.push_back(data >> 1 & M7);
    } while(data & M1);
    map.insert(WarningErrorConditions, QVariant::fromValue(result));
}

bool Asterix002Proto::generate_080(AsterixData &map, QDataStream &stream)
{
    if (map.contains(WarningErrorConditions))
    {
        const QList<quint8> extension = map.value(WarningErrorConditions).value< QList<quint8> >();
        if (extension.isEmpty())
            return false;

        quint8 data = (extension.first() & M7) << 1;
        foreach (quint8 val, extension)
        {
            write8(stream, data | M1);
            data = (val & M7) << 1;
        }
        write8(stream, data);
        return true;
    }
    return false;
}

void Asterix002Proto::parse_090(QDataStream &stream, AsterixData &map)
{
    map.insert(Collimation_ErrorRangeError, double(read<qint8>(stream)) / 128);
    map.insert(Collimation_ErrorAzimuthError, double(read<qint8>(stream)) * 360 / 65536);
}

bool Asterix002Proto::generate_090(AsterixData &map, QDataStream &stream)
{
    if (map.contains(Collimation_ErrorRangeError) && map.contains(Collimation_ErrorAzimuthError))
    {
        write8(stream, map.value(Collimation_ErrorRangeError).toDouble() * 128);
        write8(stream, map.value(Collimation_ErrorAzimuthError).toDouble() * 65536 / 360);
        return true;
    }
    return false;
}

void Asterix002Proto::parse_100(QDataStream &stream, AsterixData &map)
{
    map.insert(DynamicWindow_RhoStart, double(read<quint16>(stream)) / 128);
    map.insert(DynamicWindow_RhoEnd, double(read<quint16>(stream)) / 128);
    map.insert(DynamicWindow_ThetaStart, double(read<quint16>(stream)) * 360 / 65536);
    map.insert(DynamicWindow_ThetaEnd, double(read<quint16>(stream)) * 360 / 65536);
}

bool Asterix002Proto::generate_100(AsterixData &map, QDataStream &stream)
{
    if (map.contains(DynamicWindow_RhoStart) && map.contains(DynamicWindow_RhoEnd)
        && map.contains(DynamicWindow_ThetaStart) && map.contains(DynamicWindow_ThetaEnd))
    {
        write16(stream, map.value(DynamicWindow_RhoStart).toDouble() * 128);
        write16(stream, map.value(DynamicWindow_RhoEnd).toDouble() * 128);
        write16(stream, map.value(DynamicWindow_ThetaStart).toDouble() * 65536 / 360);
        write16(stream, map.value(DynamicWindow_ThetaEnd).toDouble() * 65536 / 360);
        return true;
    }
    return false;
}
