#include "prior008.h"
#include "shareutility.h"
#include "priordata.h"
#include "presentdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>
#include <QtCore/qmath.h>

#include <math.h>


/*!
    ПРИМЕЧАНИЕ :

        Контур  конкретного  опасного  метеоявления  описывается
        векторами,  образуемыми  при  пересечении  границ
        (контура)  метеоявления  лучами,     исходящими  из  точки
        стояния  радиолокатора.   Дискретность  векторных  лучей
        по  углу  азимута  -  1,40625;  2,8125  или  5,625  градуса.
        По  каждому  вектору  передается  отдельное  сообщение с
        прямоугольными  координатами  X, Y  начальной  и
        конечной точек пересечения луча с границами метеоявления.

*/

const int Prior008::mCategory = 8;
const QString Prior008::mEdition = "1.0";

const QString Prior008::MessageType = "pr008.MessageType";

const QString Prior008::DataSourceIdentifier_SAC = PRIOR_SAC;
const QString Prior008::DataSourceIdentifier_SIC = PRIOR_SIC;

const QString Prior008::WeatherVector = "pr008.WeatherVector";

const QString Prior008::TimeOfDay = "pr008.TimeOfDay";


Prior008::Prior008()
{}

int Prior008::category() const
{
    return mCategory;
}

QString Prior008::edition() const
{
    return mEdition;
}

void Prior008::parse(QDataStream &stream, PriorData &map)
{
    try
    {
        PresentData mainMask(stream);

        if (mainMask.isSet(1))
            parse_DataSourceIdentifier(stream, map);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (mainMask.isSet(2))
            parse_MessageType(stream, map);
        else
            throw mandatory("Message Type");

        uint messageType = map.value(MessageType).toUInt();

        // Parse "weather vector" message
        if ( messageType == uint(MTWeatherVector) )
        {
            if (mainMask.isSet(3))
                parse_WeatherVector(stream, map);
            else
                map.appendError(mandatory("Weather Vector"));

            if (mainMask.isSet(4))
                parse_TimeOfDay(stream, map);
        }
        else
            throw PriorErrors::Error_InvalidInput;

    }
    catch (PriorErrors::ErrorCode error)
    {
        map.appendError(error);
    }
    catch (PriorErrors *error)
    {
        map.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            map.appendError(PriorErrors::Error_UnexpectedEndOfData);
    }
}

QByteArray Prior008::generate(PriorData &map)
{
    QByteArray result;
    PresentData mainMask;
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        if (generate_DataSourceIdentifier(map, stream))
            mainMask.set(1);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (generate_MessageType(map, stream))
            mainMask.set(2);
        else
            throw mandatory("Message Type");

        uint messageType = map.value(MessageType).toUInt();

        // Generate "weather vector" message
        if ( messageType == uint(MTWeatherVector) )
        {
            if (generate_WeatherVector(map, stream))
                mainMask.set(3);
            else
                map.appendError(mandatory("Weather Vector"));

            if (generate_TimeOfDay(map, stream))
                mainMask.set(4);
        }
        else
            throw PriorErrors::Error_InvalidInput;

    }
    catch (PriorErrors::ErrorCode error)
    {
        map.appendError(error);
    }
    catch (PriorErrors *error)
    {
        map.appendError(error);
    }
    return result.prepend(mainMask.data());
}

QStringList Prior008::fields() const
{
    static QStringList result = QStringList()

    << MessageType

    << DataSourceIdentifier_SAC
    << DataSourceIdentifier_SIC

    << WeatherVector

    << TimeOfDay;

    return result;
}

void Prior008::parse_MessageType(QDataStream &stream, PriorData &map)
{
    map.insert(MessageType, read<quint8>(stream));
}

void Prior008::parse_DataSourceIdentifier(QDataStream &stream, PriorData &map)
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

void Prior008::parse_WeatherVector(QDataStream &stream, PriorData &map)
{
    SWeatherVector weatherVector;

    weatherVector.x1 = read<qint16>(stream) * 15.;
    weatherVector.y1 = read<qint16>(stream) * 15.;
    weatherVector.x2 = read<qint16>(stream) * 15.;
    weatherVector.y2 = read<qint16>(stream) * 15.;

    map.insert(WeatherVector, QVariant::fromValue(weatherVector));
}

void Prior008::parse_TimeOfDay(QDataStream &stream, PriorData &map)
{
    quint32 timeOfDay = read24(stream) / 128 * 1000.;
    map.insert(TimeOfDay, QTime().addMSecs(timeOfDay));
}

bool Prior008::generate_MessageType(PriorData &map, QDataStream &stream)
{
    if (map.contains(MessageType))
    {
        write8(stream, map.value(MessageType).toUInt());
        return true;
    }
    return false;
}

bool Prior008::generate_DataSourceIdentifier(PriorData &map, QDataStream &stream)
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

bool Prior008::generate_WeatherVector(PriorData &map, QDataStream &stream)
{
    if (map.contains(WeatherVector))
    {
        SWeatherVector weatherVector = map.value(WeatherVector).value<SWeatherVector>();

        write16<qint16>(stream, weatherVector.x1 / 15.);
        write16<qint16>(stream, weatherVector.y1 / 15.);
        write16<qint16>(stream, weatherVector.x2 / 15.);
        write16<qint16>(stream, weatherVector.y2 / 15.);
        return true;
    }
    return false;
}

bool Prior008::generate_TimeOfDay(PriorData &map, QDataStream &stream)
{
    if (map.contains(TimeOfDay))
    {
        int secs = QTime(0,0).msecsTo(map.value(TimeOfDay).toTime()) * 128 / 1000.;
        write24(stream, secs);
        return true;
    }
    return false;
}
