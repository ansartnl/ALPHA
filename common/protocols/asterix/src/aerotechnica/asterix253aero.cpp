#include "asterix253aero.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>

#include <math.h>


namespace Aerotechnica {

const int Asterix253::mCategory = 253;
const QString Asterix253::mEdition = "A1000";

const QString Asterix253::DataSourceIdentifier_SAC = ASTERIX_SAC;
const QString Asterix253::DataSourceIdentifier_SIC = ASTERIX_SIC;

const QString Asterix253::DirectionToTarget = "DirectionToTarget";

const QString Asterix253::ADFChannel = "ADFChannel";

const QString Asterix253::ADFChannelsOn = "ADFChannelsOn";

const QString Asterix253::ADFChannelsOff = "ADFChannelsOff";


Asterix253::Asterix253()
{}

int Asterix253::category() const
{
    return mCategory;
}

QString Asterix253::edition() const
{
    return mEdition;
}

void Asterix253::parse(QDataStream &stream, AsterixData &asterix)
{
    try
    {
        PresentData mainMask(stream);

        bool isN01 = mainMask.isSet(1);
        bool isN02 = mainMask.isSet(7);
        bool isN03 = mainMask.isSet(5);
        bool isN04 = mainMask.isSet(6);

        if (isN01 && isN02)
        {
            if (isN01)
                parse_N01(stream, asterix);
            else
                asterix.appendError(mandatory("Data Source Identifier"));

            if (isN02)
                parse_N02(stream, asterix);
            else
                asterix.appendError(mandatory("Direction To Target"));
        }
        else if (isN03 && isN04)
        {
            if (isN01)
                parse_N03(stream, asterix);
            else
                asterix.appendError(mandatory("ADF Channels Switch On"));

            if (isN02)
                parse_N04(stream, asterix);
            else
                asterix.appendError(mandatory("ADF Channels Switch Off"));
        }
        else
            throw AsterixErrors::Error_InvalidInput;
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

QByteArray Asterix253::generate(AsterixData &asterix)
{
    QByteArray result;
    PresentData mainMask;
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        bool isN01 = asterix.contains(DataSourceIdentifier_SAC) && asterix.contains(DataSourceIdentifier_SIC);
        bool isN02 = asterix.contains(DirectionToTarget) && asterix.contains(ADFChannel);
        bool isN03 = asterix.contains(ADFChannelsOn);
        bool isN04 = asterix.contains(ADFChannelsOff);

        if (isN01 && isN02)
        {
            if (generate_N01(asterix, stream))
                mainMask.set(1);
            else
                asterix.appendError(mandatory("Data Source Identifier"));

            if (generate_N02(asterix, stream))
                mainMask.set(7);
            else
                asterix.appendError(mandatory("Direction To Target"));
        }
        else if (isN03 && isN04)
        {
            if (generate_N03(asterix, stream))
                mainMask.set(5);
            else
                asterix.appendError(mandatory("ADF Channels Switch On"));

            if (generate_N04(asterix, stream))
                mainMask.set(6);
            else
                asterix.appendError(mandatory("ADF Channels Switch Off"));
        }
        else
            throw AsterixErrors::Error_InvalidInput;
    }
    catch (AsterixErrors::ErrorCode error)
    {
        asterix.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        asterix.appendError(error);
    }
    return result.prepend(mainMask.data());
}

QStringList Asterix253::fields() const
{
    static QStringList result = QStringList()

        << DataSourceIdentifier_SAC
        << DataSourceIdentifier_SIC

        << DirectionToTarget
        << ADFChannel

        << ADFChannelsOn
        << ADFChannelsOff;

    return result;
}

void Asterix253::parse_N01(QDataStream &stream, AsterixData &map) // Data Item I001/010, Data Source Identifier
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

void Asterix253::parse_N02(QDataStream &stream, AsterixData &map)
{
    map.insert(ADFChannel, read<quint8>(stream));
    map.insert(DirectionToTarget, double(read<quint16>(stream)) * 0.0055);
}

void Asterix253::parse_N03(QDataStream &stream, AsterixData &map)
{
    SChannel ch;
    ch.channels = read<quint16>(stream);
    map.insert(ADFChannelsOn, QVariant::fromValue(ch));
}

void Asterix253::parse_N04(QDataStream &stream, AsterixData &map)
{
    SChannel ch;
    ch.channels = read<quint16>(stream);
    map.insert(ADFChannelsOff, QVariant::fromValue(ch));
}

bool Asterix253::generate_N01(AsterixData &map, QDataStream &stream)
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

bool Asterix253::generate_N02(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ADFChannel))
    {
        write8(stream, map.value(ADFChannel).toUInt());
        write16(stream, map.value(DirectionToTarget).toDouble() / 0.0055);
        return true;
    }
    return false;
}

bool Asterix253::generate_N03(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ADFChannelsOn))
    {
        SChannel ch = map.value(ADFChannelsOn).value<SChannel>();
        write16(stream, ch.channels);
        return true;
    }
    return false;
}

bool Asterix253::generate_N04(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ADFChannelsOff))
    {
        SChannel ch = map.value(ADFChannelsOff).value<SChannel>();
        write16(stream, ch.channels);
        return true;
    }
    return false;
}

}   // namespace Aerotechnica {
