#ifndef GROUNDTOWER_H
#define GROUNDTOWER_H

#include "GroundCoordinate.h"
#include "import/ItemHandler.h"

class GroundTower
        :ItemHandler
{
public:
    GroundTower()
    : mCoord()
    {}

    //! Coordinates
    inline const CCoord& coord() const
    {return mCoord;}

    inline void setCoord(CCoord& coord)
    {mCoord = coord;}

private:
    CCoord  mCoord;
};

#endif // GROUNDTOWER_H
