#ifndef GROUNDPARKING_H
#define GROUNDPARKING_H

#include "GroundCoordinate.h"
#include "import/ItemHandler.h"

namespace ground_parking{
const int HOVER_RADIUS = 20;
}

class GroundParking
        : ItemHandler
{
public:
    GroundParking()
        : mCoord(),
          mRadius(0),
          mNumber(-1),
          mIndex(-1),
          mName()
    {}

    //! Coordinates
    inline const CCoord& coord() const
    {return mCoord;}

    inline void setCoord(CCoord& coord)
    {mCoord = coord;}

    //! Radius
    inline double radius() const
    {return mRadius;}

    inline void setRadius(double val)
    {mRadius = val;}

    //! Number
    inline int number() const
    {return mNumber;}

    inline void setNumber(int val)
    {mNumber = val;}

    //! Index
    inline int index() const
    {return mIndex;}

    inline void setIndex(int val)
    {mIndex = val;}

    //! Name
    inline const QString& name() const
    {return mName;}

    inline void setName(QString sName)
    {mName = sName;}

private:
    CCoord  mCoord;
    double mRadius;
    int    mNumber;
    int    mIndex;
    QString mName;
};

#endif // GROUNDPARKING_H
