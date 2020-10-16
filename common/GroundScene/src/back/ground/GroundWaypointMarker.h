#ifndef GROUNDWAYPOINTMARKER_H
#define GROUNDWAYPOINTMARKER_H

#include "GroundCoordinate.h"
#include "import/ItemHandler.h"

class GroundWaypointMarker
        :ItemHandler
{
public:

    GroundWaypointMarker()
        :mCoord(), mSN(-1)
    {}

    //! Coordinates
    inline const CCoord& coord() const
    {return mCoord;}

    inline void setCoord(const CCoord& coord)
    {mCoord = coord;}

    //! SN
    inline int SN() const
    {return mSN;}

    inline void setSN(int SN)
    {mSN = SN;}

private:
    CCoord      mCoord;
    int         mSN;
};
#endif // GROUNDWAYPOINTMARKER_H
