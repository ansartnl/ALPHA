#ifndef GROUNDBUILDING_H
#define GROUNDBUILDING_H

#include "GroundCoordinate.h"
#include "import/ItemHandler.h"

class GroundBuilding
        : ItemHandler
{
public:
    GroundBuilding()
        : mCoord(),
          mAzimuth(0),
          mWidth(0),
          mLength(0)
    {}

    //! Coordinates
    inline const CCoord& coord() const
    {return mCoord;}

    inline void setCoord(CCoord& coord)
    {mCoord = coord;}

    //! Azimuth
    inline double azimuth() const
    {return mAzimuth;}

    inline void setAzimuth(double val)
    {mAzimuth = val;}

    //! Length
    inline double length() const
    {return mLength;}

    inline void setLength(double val)
    {mLength = val;}

    //! Width
    inline double width() const
    {return mWidth;}

    inline void setWidth(double val)
    {mWidth = val;}

private:
    CCoord  mCoord;
    double mAzimuth;
    double mWidth;
    double mLength;
};

#endif // GROUNDBUILDING_H
