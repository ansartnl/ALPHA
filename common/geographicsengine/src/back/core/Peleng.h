#ifndef _PELENG_H__
#define _PELENG_H__

#include "cs/Coordinate.h"
#include "MacroDefs.h"


//! Class stores peleng information.
class Peleng
{
public:
    Peleng()
        : m_ID(0), m_Angle(0.), m_ChannelNumber(0.)
    {}
    virtual ~Peleng() {}

    //! Meteo vector identifier (locator ID)
    GEO_PROPERTY(quint16, ID)

    //! Geo location.
    GEO_PROPERTY(CCoord, Coord)

    //! Scanning angle.
    GEO_PROPERTY(qreal, Angle)

    //! Channel number.
    GEO_PROPERTY(qint8, ChannelNumber)
};

#endif // _PELENG_H__
