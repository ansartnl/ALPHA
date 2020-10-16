#include "asterix253peleng.h"
#include "shareutility.h"

#include "presentdata.h"
#include "asterixdata.h"

namespace Peleng {

const int Asterix253::mCategory = 253;
const QString Asterix253::mEdition = "";

const QString Asterix253::MessageType = "a253.MessageType";
const QString Asterix253::DataSourceIdentifier_SAC = "a253.DataSourceIdentifier.SAC";
const QString Asterix253::DataSourceIdentifier_SIC = "a253.DataSourceIdentifier.SIC";
const QString Asterix253::ServiceIdentifier = "a253.ServiceIdentifier";
const QString Asterix253::State = "a253.State";
const QString Asterix253::Mode = "a253.Mode";
const QString Asterix253::ChannelState_SAC = "a253.ChannelState.SAC";
const QString Asterix253::ChannelState_SIC = "a253.ChannelState.SIC";
const QString Asterix253::ChannelState_First = "a253.ChannelState.First";
const QString Asterix253::ChannelState_Second = "a253.ChannelState.Second";
const QString Asterix253::AdapterState_First = "a253.AdapterState.First";
const QString Asterix253::AdapterState_Second = "a253.AdapterState.Second";

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

void Asterix253::parse(QDataStream &stream, AsterixData &map)
{
    try
    {
        PresentData mainMask(stream);

        if (mainMask.size() > 1)
            throw AsterixErrors::Error_FSPEC;

        if (mainMask.isSet(1))
            parse_010(stream, map);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (mainMask.isSet(2))
            parse_000(stream, map);
        else
            map.appendError(mandatory("Message Type"));

        if (mainMask.isSet(3))
            parse_015(stream, map);

        if (mainMask.isSet(4))
            parse_020(stream, map);
        else
            map.appendError(mandatory("State"));

        if (mainMask.isSet(5))
            parse_021(stream, map);
        else
            map.appendError(mandatory("Mode"));

        if (mainMask.isSet(6))
            parse_030(stream, map);
        else
            map.appendError(mandatory("Channel State"));

        if (mainMask.isSet(7))
            parse_031(stream, map);
        else
            map.appendError(mandatory("Network Adapter State"));
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
    catch (...)
    {
        qCritical() << "Unknown exception:" << Q_FUNC_INFO;
    }
}

QByteArray Asterix253::generate(AsterixData &map)
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
            map.appendError(mandatory("Message Type"));

        if (generate_015(map, stream))
            mainMask.set(3);

        if (generate_020(map, stream))
            mainMask.set(4);
        else
            map.appendError(mandatory("State"));

        if (generate_021(map, stream))
            mainMask.set(5);
        else
            map.appendError(mandatory("Mode"));

        if (generate_030(map, stream))
            mainMask.set(6);
        else
            map.appendError(mandatory("Channel State"));

        if (generate_031(map, stream))
            mainMask.set(7);
        else
            map.appendError(mandatory("Network Adapter State"));
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

void Asterix253::parse_000(QDataStream &stream, AsterixData &map)
{
    map.insert(MessageType, read<quint8>(stream));
}

void Asterix253::parse_010(QDataStream &stream, AsterixData &map)
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

void Asterix253::parse_015(QDataStream &stream, AsterixData &map)
{
    map.insert(ServiceIdentifier, read<quint8>(stream));
}

void Asterix253::parse_020(QDataStream &stream, AsterixData &map)
{
    map.insert(State, read<quint8>(stream));
}

void Asterix253::parse_021(QDataStream &stream, AsterixData &map)
{
    map.insert(Mode, read<quint8>(stream));
}

void Asterix253::parse_030(QDataStream &stream, AsterixData &map)
{
    int count = read<quint8>(stream);

    QVariantList sac, sic, first, second;

    for (int i = 0; i < count; ++i)
    {
        sac.append(read<quint8>(stream));
        sic.append(read<quint8>(stream));
        first.append(read<quint8>(stream));
        second.append(read<quint8>(stream));
    }

    map.insert(ChannelState_SAC, sac);
    map.insert(ChannelState_SIC, sic);
    map.insert(ChannelState_First, first);
    map.insert(ChannelState_Second, second);
}

void Asterix253::parse_031(QDataStream &stream, AsterixData &map)
{
    map.insert(AdapterState_First, read<quint8>(stream));
    map.insert(AdapterState_Second, read<quint8>(stream));
}

bool Asterix253::generate_000(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, MessageType))
    {
        write8(stream, map.value(MessageType).toUInt());
        return true;
    }
    return false;
}

bool Asterix253::generate_010(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, DataSourceIdentifier_SAC, DataSourceIdentifier_SIC))
    {
        write8(stream, map.value(DataSourceIdentifier_SAC).toUInt());
        write8(stream, map.value(DataSourceIdentifier_SIC).toUInt());
        return true;
    }
    return false;
}

bool Asterix253::generate_015(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ServiceIdentifier))
    {
        write8(stream, map.value(ServiceIdentifier).toUInt());
        return true;
    }
    return false;
}

bool Asterix253::generate_020(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, State))
    {
        write8(stream, map.value(State).toUInt());
        return true;
    }
    return false;
}

bool Asterix253::generate_021(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, Mode))
    {
        write8(stream, map.value(Mode).toUInt());
        return true;
    }
    return false;
}

bool Asterix253::generate_030(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, ChannelState_SAC, ChannelState_SIC, ChannelState_First, ChannelState_Second))
    {
        QVariantList sac = map.value(ChannelState_SAC).toList();
        QVariantList sic = map.value(ChannelState_SIC).toList();
        QVariantList first = map.value(ChannelState_First).toList();
        QVariantList second = map.value(ChannelState_Second).toList();

        int count = sac.size();

        if (count != sic.size() || count != first.size() || count != second.size())
            return false;

        write8(stream, count);
        for (int i = 0; i < count; ++i)
        {
            write8(stream, sac[i].toUInt());
            write8(stream, sic[i].toUInt());
            write8(stream, first[i].toUInt());
            write8(stream, second[i].toUInt());
        }
    }
    return false;
}

bool Asterix253::generate_031(AsterixData &map, QDataStream &stream)
{
    if (isContain(map, AdapterState_First, AdapterState_Second))
    {
        write8(stream, map.value(AdapterState_First).toUInt());
        write8(stream, map.value(AdapterState_Second).toUInt());
        return true;
    }
    return false;
}

QStringList Asterix253::fields() const
{
    static QStringList result = QStringList()
    << MessageType
    << DataSourceIdentifier_SAC
    << DataSourceIdentifier_SIC
    << ServiceIdentifier
    << State
    << Mode
    << ChannelState_SAC
    << ChannelState_SIC
    << ChannelState_First
    << ChannelState_Second
    << AdapterState_First
    << AdapterState_Second;

    return result;
}

}

