#ifndef GROUNDPATH_H
#define GROUNDPATH_H

#include "GroundCoordinate.h"
#include "import/ItemHandler.h"

class GroundPath
        : ItemHandler
{
public:
    GroundPath()
        :mWidth(0),
          mNumber(-1),
          mStartPointIndex(-1),
          mEndPointIndex(-1),
          mType(None),
          mStartPointCoord(),
          mEndPointCoord(),
          mName()
    {}

    enum Type{None, Taxi, Path, Closed, Vehicle, Parking, Runway};

    //! Start coord
    inline const CCoord& startPointCoord() const
    {return mStartPointCoord;}

    inline void setStartPointCoord(CCoord& coord)
    {mStartPointCoord = coord;}

    //! End coord
    inline const CCoord& endPointCoord() const
    {return mEndPointCoord;}

    inline void setEndPointCoord(CCoord& coord)
    {mEndPointCoord = coord;}

    //! Width
    inline double width() const
    {return mWidth;}

    inline void setWidth(double val)
    {mWidth = val;}

    //! Number
    inline int number() const
    {return mNumber;}

    inline void setNumber(int val)
    {mNumber = val;}

    //! Start index
    inline int startPointIndex() const
    {return mStartPointIndex;}

    inline void setStartPointIndex(int val)
    {mStartPointIndex = val;}

    //! End Index
    inline int endPointIndex() const
    {return mEndPointIndex;}

    inline void setEndPointIndex(int val)
    {mEndPointIndex = val;}

    //! Type
    inline Type type() const
    {return mType;}

    inline void setType(Type val)
    {mType = val;}

    //! Name
    inline const QString& name() const
    {return mName;}

    inline void setName(QString& val)
    {mName = val;}

private:
    double mWidth;
    int    mNumber;
    int    mStartPointIndex;
    int    mEndPointIndex;
    Type   mType;
    CCoord mStartPointCoord;
    CCoord mEndPointCoord;
    QString mName;
};

#endif // GROUNDPATH_H
