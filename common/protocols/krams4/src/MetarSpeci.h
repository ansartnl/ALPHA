#ifndef METARSPECI_H
#define METARSPECI_H

#include "kramsexport.h"
#include "enumstreaming.h"

#include <QtCore>

// The structure describe Metar message
struct KRAMS_EXPORT MetarSpeci
{
    QDateTime arrTime;

    QString source;

    enum Type
    {
        Metar,
        Speci
    } type;
    bool isCor;
    QString airport;
    QDateTime date;
    bool isNil;
    bool isAuto;
    struct Wind
    {
        bool isVariable;
        int direction;
        int averageSpeed;
        QString speedType;
        bool haveGust;
        bool isAbove;
        int gust;
        bool haveVariations;
        int variationFrom;
        int variationTo;
    } wind;

    bool isCavok;

    int visibilityMin;
    QString visibilityMinDirection;
    bool haveMaxVisibility;
    int visibilityMax;
    QString visibilityMaxDirection;

    struct RunwayVisibilityRange
    {
        int runwayId;
        QString runwayPlace;
        QString outOfRange;
        int visibility;
        bool isVariable;
        QString outOfRangeMax;
        int visibilityMax;
        QString tendency;
    };
    QList<RunwayVisibilityRange> rvr;

    struct WeatherEvent
    {
        QString strength;
        QString type;
    };
    QList<WeatherEvent> weatherEvents;

    bool isCloudinessAndVerticalVisibilityAvailable;
    struct CloudinessAndVerticalVisibility
    {
        enum Type
        {
            Clouds,
            Visibility,
            Skc,
            Nsc
        } type;

        struct Cloudiness
        {
            QString cloudAmout;
            int height;
            QString cloudType;
        };
        QList<Cloudiness> clouds;
        QString verticalVisibility;
    } cloudinessAndVerticalVisibility;

    int temperature;
    int dewPoint;

    QString pressureType;
    int pressure;

    QStringList recentWeather;

    struct WindShear
    {
        bool isAll;
        int runwayId;
        QString runwayPlace;
    };
    QList<WindShear> windShear;

    bool isSeaStateAvailable;
    int seaTemperature;
    int seaState;

    struct RunwayState
    {
        RunwayState() : isCLRD(false), isCoveringConditionsSet(false),
            isCoveringRangeSet(false), isCohesionSet(false) {}

        int runwayId;
        bool isCLRD;
        bool isCoveringConditionsSet;
        int coveringConditions;
        bool isCoveringRangeSet;
        int coveringRange;
        int coveringThick;
        bool isCohesionSet;
        int cohesion;
    };
    QList<RunwayState> runwayState;

    bool isThendAvailable;
    struct Thend
    {
        QString type;
        struct Time
        {
            QString type;
            int hours;
            int minits;
        };
        QList<Time> time;

        bool isWindAvailable;
        struct Wind
        {
            bool isVariable;
            int direction;
            int averageSpeed;
            QString speedType;
            bool haveGust;
            bool isGustAbove;
            int gust;
        } wind;

        bool isCavok;

        bool isVisibilityAvailable;
        int visibility;

        bool isNsw;
        QStringList weatherEvents;

        enum CloudlessType
        {
            CloudlessUnAvailable,
            Clouds,
            Visibility,
            Nsc
        } cloudlessType;

        QString cloudAmout;
        int cloudHeight;
        QString cloudType;

        int visibilityHeight;
    } thend;

    QStringList rmk;

    QString qfe;
    QString qbb;

    QList<int> unknownFields;

    enum Error
    {
        Ok,
        NotMetarSpeci,
        InvalidAirport,
        InvalidTime,
        InvalidWind,
        InvalidVisibility,
        InvalidTemperature,
        InvalidPressure,
        AdditionalUnknownFields
    };
};

QDataStream &operator<<(QDataStream &stream, const MetarSpeci &m);
QDataStream &operator>>(QDataStream &stream, MetarSpeci &m);

Q_DECLARE_METATYPE(MetarSpeci)
Q_DECLARE_METATYPE(QList<MetarSpeci>)

QT_DECLARE_EMUM_STREAMING(MetarSpeci::Error)
QT_DECLARE_EMUM_STREAMING(MetarSpeci::Type)
QT_DECLARE_EMUM_STREAMING(MetarSpeci::CloudinessAndVerticalVisibility::Type)
QT_DECLARE_EMUM_STREAMING(MetarSpeci::Thend::CloudlessType)

#endif // METARSPECI_H
