#ifndef GROUNDPOINT_H
#define GROUNDPOINT_H

#include <QVector>
#include "GroundCoordinate.h"
#include "import/ItemHandler.h"

namespace ground_point{
const int HOVER_RADIUS = 20;
}

class GroundPoint
        :ItemHandler
{
public:
    //static const int HOVER_RADIUS;


    GroundPoint()
        :mCoord(),
          mIndex(-1),
          mOrientation(Forward),
          mType(Normal)
    {}

    enum Orientation{Forward, Reverse};
    enum Type{Normal, HOLD_SHORT, START};

    //! Coordinates
    inline const CCoord& coord() const
    {return mCoord;}

    inline void setCoord(const CCoord& coord)
    {mCoord = coord;}

    //! Type
    inline Type type() const
    {return mType;}

    virtual void setType(Type val)
    {mType = val;}

    //! Orientation
    inline Orientation orientation() const
    {return mOrientation;}

    inline void setOrientation(Orientation val)
    {mOrientation = val;}

    //! Index
    inline int index() const
    {return mIndex;}

    inline void setIndex(int val)
    {mIndex = val;}

private:
    CCoord      mCoord;
    int         mIndex;
    Orientation mOrientation;
    Type        mType;
};

#endif // GROUNDPOINT_H
