#ifndef _METEO_RUNWAY_H__
#define _METEO_RUNWAY_H__

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVariant>

#include "Runway.h"


//! Meteo information for specified runway
class MeteoRunway
{
public:
    //! Pointer to runway
    typedef QSharedPointer<CRunway>     TRunway;
    typedef QMap<int, QVariant>         TValueMap;

    virtual ~MeteoRunway() {}

    void SetRunwayName(const QString &name) { mRunwayName = name; }
    QString GetRunwayName() const { return mRunwayName; }

    void SetAirportName(const QString &name) { mAirportName = name; }
    QString GetAirportName() const { return mAirportName; }

    //! Returns true if data was not inserted.
    bool empty() const { return mValues.empty(); }

    bool contains(int key) const;
    QVariant value(int key, const QVariant &defValue = QVariant()) const;
    void insert(int key, const QVariant &v);
    void remove(int key);

    bool operator == (const MeteoRunway &v) const;
    bool operator != (const MeteoRunway &v) const;

    TValueMap GetValues() const
    { return mValues; }
    void SetValues(const TValueMap& mV)
    { mValues = mV; }

    // Id list

    /** Время наблюдения */
    static const int ControlTime;
    /** Магнитное направление ветра (градусы) */
    static const int MagneticWind;
    /** Средняя скорость ветра (м/с) */
    static const int AverageSpeed;
    /** Максимальная скорость ветра (м/с) */
    static const int MaxSpeed;
    /** Боковая состоявляющая ветра (м/с) */
    static const int SideWind;
    /** Продольная состовляющая ветра (м/с) */
    static const int LongitudialWind;
    /** Метеорологическая дальность видимости на ВПП (метры) */
    static const int SightDistance;
    /** Метеорологическая дальность видимости ОВИ в начале ВПП (метры) */
    static const int SDistanceStart;
    /** Метеорологическая дальность видимости ОВИ в середине ВПП (метры) */
    static const int SDistanceMiddle;
    /** Метеорологическая дальность видимости ОВИ в конце ВПП (метры) */
    static const int SDistanceEnd;
    /** Высота нижней границы облачности (метры) */
    static const int LowerCloudiness;
    /** Давление на рабочем пороге ВПП (мм. рт. ст.) */
    static const int Pressure;
    /** Давление, приведенное к уровню моря (гПа) */
    static const int PressureSea;
    /** Температура воздуха */
    static const int Temperature;
    /** Точка росы */
    static const int CondensationPoint;

private:
    /** Ссылка на ВПП */
    QString         mRunwayName;
    /** Ссылка на аэропорт */
    QString         mAirportName;
    /** Map with values */
    TValueMap       mValues;
};

QDataStream &operator<<(QDataStream &out, const MeteoRunway &value);
QDataStream &operator>>(QDataStream &in, MeteoRunway &value);

#endif // _METEO_RUNWAY_H__
