#ifndef _RE_POINT_H__
#define _RE_POINT_H__

#include "cs/Coordinate.h"

#ifdef REWARN_TEST
#   include "rewarn_global.h"
#endif

namespace rewarn {

//! Structure describes one point of a restriction zone
#ifdef REWARN_TEST
    struct REWARN_EXPORT RePoint
#else
    struct RePoint
#endif
{
    //! Point types
    enum Type
    {
        enLine = 'G', //!< line joining
        enCircle = 'C', //!< circle
        enLeftArc = 'L', //!< left arc
        enRightArc = 'R' //!< right arc
    };

    //! Constructor
    RePoint() : mArcDistance(0),  mType(enLine) {}

public:
    CCoord mArcCoord;
    double mArcDistance;
    CCoord mCoord;
    Type mType;
};

}   // namespace rewarn {

#endif // _RE_POINT_H__
