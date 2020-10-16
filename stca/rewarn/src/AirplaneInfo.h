#ifndef _AIRPLANE_INFO_H__
#define _AIRPLANE_INFO_H__

#include "rewarn_global.h"

#include <QtGlobal>

namespace rewarn {

//! Airplane data structure.
struct REWARN_EXPORT AirplaneInfo
{
    AirplaneInfo()
        : boardNumber(0), x(0.), y(0.), height(0.), dx(0.), dy(0.), dz(0.)
    {}

    quint16 boardNumber;
    /*! Latitude or X projection */
    qreal   x;
    /*! Longitude or Y projection */
    qreal   y;
    /*! Airplane altitude */
    qreal   height;
    /*! Velocity vector meter per second */
    qreal   dx, dy, dz;
};

}

#endif // _AIRPLANE_INFO_H__
