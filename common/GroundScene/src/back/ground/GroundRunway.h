#ifndef GROUNDRUNWAY_H
#define GROUNDRUNWAY_H

#include "GroundCoordinate.h"
#include "import/ItemHandler.h"

class GroundRunway
        : ItemHandler
{
public:
    GroundRunway()
        : mCoord(),
          mAzimuth(0),
          mWidth(0),
          mLength(0),
          mNumber(-1),
          mDesignator(),
          mName()
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

    //! Width
    inline double width() const
    {return mWidth;}

    inline void setWidth(double val)
    {mWidth = val;}

    //! Length
    inline double length() const
    {return mLength;}

    inline void setLength(double val)
    {mLength = val;}

    //! Number
    inline int number() const
    {return mNumber;}

    inline void setNumber(int val)
    {mNumber = val;}

    //! Designator
    inline const QString& designator() const
    {return mDesignator;}

    inline void setDesignator(QString& val)
    {mDesignator = val;}

    //! Name
    inline const QString& name() const
    {return mName;}

    inline void setName(const QString& val)
    {mName = val;}

private:
    CCoord  mCoord;
    double  mAzimuth;
    double  mWidth;
    double  mLength;
    int     mNumber;
    QString mDesignator;
    QString mName;

};

#endif // GROUNDRUNWAY_H
