#ifndef XT_METEO_DATA_H
#define XT_METEO_DATA_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtObject.h"
#include <QtGlobal>
#include <QPair>
#include <QPointF>
#include <QVector>
#include <QMap>
#include <QVariant>
#include <QDateTime>

XT_BEGIN_NAMESPACE

//! Array of meteo vectors - data storage
struct XTPROTO_EXPORT MeteoVector : public CObject
{
    virtual void Serialize(QDataStream& out) const
    {
        CObject::Serialize(out);
        out << mId << mMeteo << mIntencity;
    }
    virtual void Deserialize(QDataStream& in)
    {
        CObject::Deserialize(in);
        in >> mId >> mMeteo >> mIntencity;
    }

    QByteArray toArray() const;

    bool operator == (const MeteoVector& obj) const
    {
        return ( mId == obj.mId && mMeteo == obj.mMeteo &&
                 mIntencity == obj.mIntencity );
    }

    bool operator != (const MeteoVector& obj) const
    { return !operator ==(obj); }

    XT_OBJECT_TYPE_DECL

public:
    /** QPointf: x - latitude, y - longitude (degree) */
    typedef QPair<QPointF, QPointF>     TVector;
    typedef QVector<TVector>            TVectors;

    /** Locator identifier */
    quint16     mId;
    /** Array of meteovectors */
    TVectors    mMeteo;
    /** Intensity (0-7) */
    qint16      mIntencity;
};


//! Meteo contour - data storage
struct XTPROTO_EXPORT MeteoContour : public CObject
{
    MeteoContour()
        : mId(0), mIntencity(0), mDBID(0) {}

    virtual void Serialize(QDataStream& out) const
    {
        CObject::Serialize(out);
        out << mId << mContour << mIntencity << mName << mDBID;
    }
    virtual void Deserialize(QDataStream& in)
    {
        CObject::Deserialize(in);
        in >> mId >> mContour >> mIntencity >> mName >> mDBID;
    }

    QByteArray toArray() const;

    bool operator == (const MeteoContour& obj) const
    {
        return ( mId == obj.mId && mContour == obj.mContour &&
                 mIntencity == obj.mIntencity && mName == obj.mName  &&
                 mDBID == obj.mDBID );
    }

    bool operator != (const MeteoContour& obj) const
    { return !operator ==(obj); }

    XT_OBJECT_TYPE_DECL

public:
    /** Array of points: x - latitude, y - longitude (degree) */
    typedef QVector<QPointF>    TPoints;

    /** Locator identifier */
    quint16     mId;
    /** Array of contour points */
    TPoints     mContour;
    /** Intensity (0-7) */
    qint16      mIntencity;
    /** Meteo contour name */
    QString     mName;
    /** Database identifier */
    quint32     mDBID;
};


//! Метео КРАМС
struct XTPROTO_EXPORT MeteoInfo : public CObject
{
    MeteoInfo()
        : mDBID(0) {}

    virtual void Serialize(QDataStream& out) const
    {
        CObject::Serialize(out);
        out << mAirport << mNumberVPP << mDateTime << mValues << mDBID;
    }
    virtual void Deserialize(QDataStream& in)
    {
        CObject::Deserialize(in);
        in >> mAirport >> mNumberVPP >> mDateTime >> mValues >> mDBID;
    }

    QByteArray toArray() const;

    bool operator == (const MeteoInfo& obj) const
    {
        return ( mAirport == obj.mAirport && mNumberVPP == obj.mNumberVPP &&
                 mDateTime == obj.mDateTime && mValues == obj.mValues &&
                 mDBID == obj.mDBID );
    }

    bool operator != (const MeteoInfo& obj) const
    { return !operator ==(obj); }

    XT_OBJECT_TYPE_DECL

public:
    typedef QMap<quint8, QVariant>  TValuesMap;

    //! ИКАО аэропорта.
    QString     mAirport;
    //! Номер ВПП
    QString     mNumberVPP;
    //! Дата и время наблюдения.
    QDateTime   mDateTime;
    //! Список значений.
    TValuesMap  mValues;
    /** Database identifier */
    quint32     mDBID;

    // Группа идентификаторов.
public:
    //! Магнитное направление ветра (градусы)
    static const quint8 WindCourse_ID;
    //! Средняя скорость ветра (м/с)
    static const quint8 WindAverage_ID;
    //! Максимальная скорость ветра (м/с)
    static const quint8 WindMax_ID;
    //! Боковая состовляющая ветра относительно ВПП (м/с)
    static const quint8 WindSide_ID;
    //! Продольная состовляющая ветра относительно ВПП (м/с)
    static const quint8 WindLong_ID;
    //! Temperature
    static const quint8 Temperature_ID;
    //! Точка росы
    static const quint8 DewPoint_ID;
    //! Метеорологическая дальность видимости (м/с)
    static const quint8 Vis_ID;
    //! Метеорологическая дальность видимости ОВИ в начале ВПП (м/с)
    static const quint8 DistVisBegin_ID;
    //! Метеорологическая дальность видимости ОВИ в середине ВПП (м/с)
    static const quint8 DistVisMiddle_ID;
    //! Метеорологическая дальность видимости ОВИ в конце ВПП (м/с)
    static const quint8 DistVisEnd_ID;
    //! Высота нижней границы облачности (метры)
    static const quint8 HeightCloud_ID;
    //! Давление на рабочем пороге ВПП (мм.рт.ст.)
    static const quint8 QFE_ID;
    //! Давление на рабочем пороге ВПП (гПа)
    static const quint8 QFEh_ID;
    //! Давление, приведенное к уровню моря (гПа)
    static const quint8 QNHh_ID;
};

XT_END_NAMESPACE

QDataStream &operator<<(QDataStream &out, const Xt::MeteoVector &meteoVector);
QDataStream &operator<<(QDataStream &out, const Xt::MeteoContour &meteoContour);
QDataStream &operator<<(QDataStream &out, const Xt::MeteoInfo &meteoInfo);

#endif // XT_METEO_DATA_H
