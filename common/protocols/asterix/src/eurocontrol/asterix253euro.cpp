#include "asterix253euro.h"
#include "shareutility.h"

#include "presentdata.h"
#include "asterixdata.h"

namespace Eurocontrol
{

const int Asterix253::mCategory = 253;
const QString Asterix253::mEdition = "";

const QString Asterix253::DataSourceIdentifier_SAC = "a253.DataSourceIdentifier.SAC";
const QString Asterix253::DataSourceIdentifier_SIC = "a253.DataSourceIdentifier.SIC";
const QString Asterix253::ServiceIdentifier = "a253.ServiceIdentifier";
const QString Asterix253::ApplicationIdentifier = "a253.ApplicationIdentifier";
const QString Asterix253::State = "a253.State";
const QString Asterix253::MessageType = "a253.MessageType";
const QString Asterix253::AdapterState_First = "a253.AdapterState.First";
const QString Asterix253::AdapterState_Second = "a253.AdapterState.Second";
const QString Asterix253::Frequency = "a253.Frequency";
const QString Asterix253::Bearing = "a253.Bearing";

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
            parse_040(stream, map);
        else
            map.appendError(mandatory("Message Type"));

        if (mainMask.isSet(3))
            parse_030(stream, map);

        if (mainMask.isSet(4))
            parse_015(stream, map);

        if (mainMask.isSet(5))
            parse_036(stream, map);

        if (mainMask.isSet(6))
            parse_101(stream, map);

        if (mainMask.isSet(7))
        {
            if (static_cast<Type>(map.value(MessageType).toInt()) == BearingType)
                parse_111(stream, map);
        }
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

void Asterix253::parse_010(QDataStream &stream, AsterixData &map)
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

void Asterix253::parse_015(QDataStream &stream, AsterixData &map)
{
    map.insert(ServiceIdentifier, read<quint8>(stream));
}

void Asterix253::parse_030(QDataStream &stream, AsterixData &map)
{
    map.insert(ApplicationIdentifier, read<quint16>(stream));
}

void Asterix253::parse_036(QDataStream &stream, AsterixData &map)
{
    map.insert(State, read<quint8>(stream));
}

void Asterix253::parse_040(QDataStream &stream, AsterixData &map)
{
    int value = read<quint8>(stream);

    Type type = UnknownType;
    if (value == 9)
        type = ChannelStateType;
    else if (value == 10)
        type = RadioStateType;
    else if (value == 22)
        type = BearingType;

    map.insert(MessageType, type);
}

void Asterix253::parse_101(QDataStream &stream, AsterixData &map)
{
    map.insert(AdapterState_First, read<quint8>(stream));
    map.insert(AdapterState_Second, read<quint8>(stream));
}

void Asterix253::parse_111(QDataStream &stream, AsterixData &map)
{
    map.insert(Frequency, read<quint32>(stream));
    map.insert(Bearing, read<quint16>(stream) * 0.0055);
}

QStringList Asterix253::fields() const
{
    static QStringList result = QStringList()
    << DataSourceIdentifier_SAC
    << DataSourceIdentifier_SIC
    << ServiceIdentifier
    << ApplicationIdentifier
    << State
    << MessageType
    << AdapterState_First
    << AdapterState_Second
    << Frequency
    << Bearing;

    return result;
}

}
