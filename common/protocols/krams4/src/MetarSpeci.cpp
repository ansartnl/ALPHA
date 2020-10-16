#include "MetarSpeci.h"

QDataStream &operator<<(QDataStream &stream, const MetarSpeci::Wind &data)
{
    stream << data.isVariable;
    stream << data.direction;
    stream << data.averageSpeed;
    stream << data.speedType;
    stream << data.haveGust;
    stream << data.isAbove;
    stream << data.gust;
    stream << data.haveVariations;
    stream << data.variationFrom;
    stream << data.variationTo;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const MetarSpeci::RunwayVisibilityRange &data)
{
    stream << data.runwayId;
    stream << data.runwayPlace;
    stream << data.outOfRange;
    stream << data.visibility;
    stream << data.isVariable;
    stream << data.outOfRangeMax;
    stream << data.visibilityMax;
    stream << data.tendency;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const MetarSpeci::WeatherEvent &data)
{
    stream << data.strength;
    stream << data.type;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const MetarSpeci::CloudinessAndVerticalVisibility::Cloudiness &data)
{
    stream << data.cloudAmout;
    stream << data.height;
    stream << data.cloudType;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const MetarSpeci::CloudinessAndVerticalVisibility &data)
{
    stream << data.type;
    stream << data.clouds;
    stream << data.verticalVisibility;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const MetarSpeci::WindShear &data)
{
    stream << data.isAll;
    stream << data.runwayId;
    stream << data.runwayPlace;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const MetarSpeci::RunwayState &data)
{
    stream << data.runwayId;
    stream << data.isCLRD;
    stream << data.isCoveringConditionsSet;
    stream << data.coveringConditions;
    stream << data.isCoveringRangeSet;
    stream << data.coveringRange;
    stream << data.coveringThick;
    stream << data.isCohesionSet;
    stream << data.cohesion;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const MetarSpeci::Thend::Time &data)
{
    stream << data.type;
    stream << data.hours;
    stream << data.minits;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const MetarSpeci::Thend::Wind &data)
{
    stream << data.isVariable;
    stream << data.direction;
    stream << data.averageSpeed;
    stream << data.speedType;
    stream << data.haveGust;
    stream << data.isGustAbove;
    stream << data.gust;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const MetarSpeci::Thend &data)
{
    stream << data.type;
    stream << data.time;
    stream << data.isWindAvailable;
    stream << data.wind;
    stream << data.isCavok;
    stream << data.isVisibilityAvailable;
    stream << data.visibility;
    stream << data.isNsw;
    stream << data.weatherEvents;
    stream << data.cloudlessType;
    stream << data.cloudAmout;
    stream << data.cloudHeight;
    stream << data.cloudType;
    stream << data.visibilityHeight;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const MetarSpeci &m)
{
    stream << m.arrTime;
    stream << m.source;
    stream << m.type;
    stream << m.isCor;
    stream << m.airport;
    stream << m.date;
    stream << m.isNil;
    stream << m.isAuto;
    stream << m.wind;
    stream << m.isCavok;
    stream << m.visibilityMin;
    stream << m.visibilityMinDirection;
    stream << m.haveMaxVisibility;
    stream << m.visibilityMax;
    stream << m.visibilityMaxDirection;
    stream << m.rvr;
    stream << m.weatherEvents;
    stream << m.isCloudinessAndVerticalVisibilityAvailable;
    stream << m.cloudinessAndVerticalVisibility;
    stream << m.temperature;
    stream << m.dewPoint;
    stream << m.pressureType;
    stream << m.pressure;
    stream << m.recentWeather;
    stream << m.windShear;
    stream << m.isSeaStateAvailable;
    stream << m.seaTemperature;
    stream << m.seaState;
    stream << m.runwayState;
    stream << m.isThendAvailable;
    stream << m.thend;
    stream << m.rmk;
    stream << m.unknownFields;
    stream << m.qfe;
    stream << m.qbb;
    return stream;
}

///////////////////////////////////////////////////////////

QDataStream &operator>>(QDataStream &stream, MetarSpeci::Wind &data)
{
    stream >> data.isVariable;
    stream >> data.direction;
    stream >> data.averageSpeed;
    stream >> data.speedType;
    stream >> data.haveGust;
    stream >> data.isAbove;
    stream >> data.gust;
    stream >> data.haveVariations;
    stream >> data.variationFrom;
    stream >> data.variationTo;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, MetarSpeci::RunwayVisibilityRange &data)
{
    stream >> data.runwayId;
    stream >> data.runwayPlace;
    stream >> data.outOfRange;
    stream >> data.visibility;
    stream >> data.isVariable;
    stream >> data.outOfRangeMax;
    stream >> data.visibilityMax;
    stream >> data.tendency;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, MetarSpeci::WeatherEvent &data)
{
    stream >> data.strength;
    stream >> data.type;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, MetarSpeci::CloudinessAndVerticalVisibility::Cloudiness &data)
{
    stream >> data.cloudAmout;
    stream >> data.height;
    stream >> data.cloudType;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, MetarSpeci::CloudinessAndVerticalVisibility &data)
{
    stream >> data.type;
    stream >> data.clouds;
    stream >> data.verticalVisibility;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, MetarSpeci::WindShear &data)
{
    stream >> data.isAll;
    stream >> data.runwayId;
    stream >> data.runwayPlace;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, MetarSpeci::RunwayState &data)
{
    stream >> data.runwayId;
    stream >> data.isCLRD;
    stream >> data.isCoveringConditionsSet;
    stream >> data.coveringConditions;
    stream >> data.isCoveringRangeSet;
    stream >> data.coveringRange;
    stream >> data.coveringThick;
    stream >> data.isCohesionSet;
    stream >> data.cohesion;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, MetarSpeci::Thend::Time &data)
{
    stream >> data.type;
    stream >> data.hours;
    stream >> data.minits;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, MetarSpeci::Thend::Wind &data)
{
    stream >> data.isVariable;
    stream >> data.direction;
    stream >> data.averageSpeed;
    stream >> data.speedType;
    stream >> data.haveGust;
    stream >> data.isGustAbove;
    stream >> data.gust;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, MetarSpeci::Thend &data)
{
    stream >> data.type;
    stream >> data.time;
    stream >> data.isWindAvailable;
    stream >> data.wind;
    stream >> data.isCavok;
    stream >> data.isVisibilityAvailable;
    stream >> data.visibility;
    stream >> data.isNsw;
    stream >> data.weatherEvents;
    stream >> data.cloudlessType;
    stream >> data.cloudAmout;
    stream >> data.cloudHeight;
    stream >> data.cloudType;
    stream >> data.visibilityHeight;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, MetarSpeci &m)
{
    stream >> m.arrTime;
    stream >> m.source;
    stream >> m.type;
    stream >> m.isCor;
    stream >> m.airport;
    stream >> m.date;
    stream >> m.isNil;
    stream >> m.isAuto;
    stream >> m.wind;
    stream >> m.isCavok;
    stream >> m.visibilityMin;
    stream >> m.visibilityMinDirection;
    stream >> m.haveMaxVisibility;
    stream >> m.visibilityMax;
    stream >> m.visibilityMaxDirection;
    stream >> m.rvr;
    stream >> m.weatherEvents;
    stream >> m.isCloudinessAndVerticalVisibilityAvailable;
    stream >> m.cloudinessAndVerticalVisibility;
    stream >> m.temperature;
    stream >> m.dewPoint;
    stream >> m.pressureType;
    stream >> m.pressure;
    stream >> m.recentWeather;
    stream >> m.windShear;
    stream >> m.isSeaStateAvailable;
    stream >> m.seaTemperature;
    stream >> m.seaState;
    stream >> m.runwayState;
    stream >> m.isThendAvailable;
    stream >> m.thend;
    stream >> m.rmk;
    stream >> m.unknownFields;
    stream >> m.qfe;
    stream >> m.qbb;
    return stream;
}

