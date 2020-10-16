#ifndef XT_COMMON_DATA_H
#define XT_COMMON_DATA_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtObject.h"
#include <QtGlobal>
#include <QDataStream>

XT_BEGIN_NAMESPACE

//! Coordinate data structure
/*! Useful data structure for representing coordinate in 3 possible types with
    readable and obvious field names:
    - polar coordinate
    - cartesian coordinate
    - geographic coordinate
*/
struct XTPROTO_EXPORT Coord
{
    //! Type this coordinate belongs.
    enum Type
    {
        EInvalid,
        EPolar,
        ECartesian,
        EGeo
    };

    Coord() : mType(EInvalid), mHeight(0.)
    {}

    /** Coordinate type */
    Type    mType;

    /*! 2 coordinates in named form */
    union Data
    {
        Data() { memset(this, 0, sizeof(*this)); }

        struct Polar
        {
            qreal       rho;        // distance meters
            qreal       theta;      // degree
        } mPolar;

        struct Cartesian
        {
            qreal       x;          // x component in meters
            qreal       y;          // y component in meters
        } mCartesian;

        struct Geo
        {
            qreal       latitude;   // degree
            qreal       longitude;  // degree
        } mGeo;
    } data;

    /** Высота в метрах */
    qreal   mHeight;

    typedef Data::Polar         Polar;
    typedef Data::Cartesian     Cartesian;
    typedef Data::Geo           Geo;

    bool operator == (const Coord& crd) const;
    bool operator != (const Coord& crd) const
    { return !operator ==(crd); }

    //! Validness test.
    bool isValid() const { return mType != EInvalid; }

    //! Coordinate data interpretation as polar coordinate.
    Polar & polar() { return data.mPolar; }
    //! Coordinate data interpretation as polar coordinate.
    const Polar & polar() const { return data.mPolar; }

    //! Coordinate data interpretation as cartesian coordinate.
    Cartesian & cartesian() { return data.mCartesian; }
    //! Coordinate data interpretation as cartesian coordinate.
    const Cartesian & cartesian() const { return data.mCartesian; }

    //! Coordinate data interpretation as geographic coordinate.
    Geo & geo() { return data.mGeo; }
    //! Coordinate data interpretation as geographic coordinate.
    const Geo & geo() const { return data.mGeo; }

    //! Define new coordinate as polar coordinate.
    void setPolar(const Polar &v) { mType = EPolar; data.mPolar = v; }
    //! Define new coordinate as cartesian coordinate.
    void setCartesian(const Cartesian &v) { mType = ECartesian; data.mCartesian = v; }
    //! Define new coordinate as geographic coordinate.
    void setGeo(const Geo &v) { mType = EGeo; data.mGeo = v; }
};

XTPROTO_EXPORT QDataStream &operator<<(QDataStream &, const Coord &);
XTPROTO_EXPORT QDataStream &operator>>(QDataStream &, Coord &);

//! Точка в геодезических координатах - градусы
struct XTPROTO_EXPORT GeoPoint
{
    GeoPoint() : mLatitude(0.), mLongitude(0.), mHeight(-10000000.) {}
    GeoPoint(qreal latitude, qreal longitude, qreal height = 0.)
        : mLatitude(latitude), mLongitude(longitude), mHeight(height)
    {}

    bool isValid() const
    { return ( mHeight > -10000000.); }

    bool operator == (const GeoPoint &point) const;
    bool operator != (const GeoPoint &point) const
    { return !operator == (point); }

    qreal   mLatitude;
    qreal   mLongitude;
    //! Высота в метрах
    qreal   mHeight;
};

XTPROTO_EXPORT QDataStream &operator<<(QDataStream &, const GeoPoint &);
XTPROTO_EXPORT QDataStream &operator>>(QDataStream &, GeoPoint &);


//! Velocity data storage
/*! Velocity in meter/sec.
*/
struct XTPROTO_EXPORT Velocity
{
    Velocity() {}
    Velocity(qreal x, qreal y, qreal z)
        : mX(x), mY(y), mZ(z)
    {}

    bool operator == (const Velocity &vel) const;
    bool operator != (const Velocity &vel) const
    { return !operator == (vel); }

    qreal   mX;
    qreal   mY;
    qreal   mZ;
};

XTPROTO_EXPORT QDataStream &operator<<(QDataStream &, const Velocity &);
XTPROTO_EXPORT QDataStream &operator>>(QDataStream &, Velocity &);


enum SignMarkType
{
    SmtPRL  = 0x0001,           //!< плотовая информация, полученная по каналу ПРЛ
    SmtVRL  = 0x0002,           //!< плотовая информация, полученная по каналу ВРЛ
    SmtVRL_S    = 0x0004,       //!< плотовая информация, полученная по каналу ВРЛ в режиме S
    SmtAZN  = 0x0008,           //!< плотовая информация, полученная от АЗН
    SmtFalsePRL  = 0x0010,      //!< ложная плотовая информация, полученная по каналу ПРЛ
    SmtFalseVRL  = 0x0020,      //!< ложная плотовая информация, полученная по каналу ВРЛ
    SmtFalseVRL_S  = 0x0040,    //!< ложная плотовая информация, полученная по каналу ВРЛ в режиме S
    SmtFalseAZN  = 0x0080,      //!< ложная плотовая информация, полученная от АЗН
    SmtMissedPRL  = 0x0100,     //!< пропущенная плотовая информация, полученная по каналу ПРЛ
    SmtMissedVRL  = 0x0200,     //!< пропущенная плотовая информация, полученная по каналу ВРЛ
    SmtMissedVRL_S  = 0x0400,   //!< пропущенная информация, полученная по каналу ВРЛ в режиме S
    SmtMissedAZN  = 0x0800,     //!< пропущенная информация, полученная от АЗН

    SmtPRL_VRL          = SmtPRL | SmtVRL,              //!< // плотовая информация, полученная по каналам ПРЛ и ВРЛ
    SmtFalsePRL_VRL     = SmtFalsePRL | SmtFalseVRL,    //!< ложная плотовая информация, полученная по каналам ПРЛ и ВРЛ
    SmtMissedPRL_VRL    = SmtMissedPRL | SmtMissedVRL   //!< пропущенная информация, полученная по каналам ПРЛ и ВРЛ
};

XT_END_NAMESPACE

#endif // XT_COMMON_DATA_H
