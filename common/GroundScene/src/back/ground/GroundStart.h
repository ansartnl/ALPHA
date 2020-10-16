#ifndef GROUNDSTART_H
#define GROUNDSTART_H

#include "GroundCoordinate.h"
#include "import/ItemHandler.h"

class GroundStart
        :ItemHandler
{
public:
    GroundStart()
        : mCoord(),
          mAzimuth(0),
          mNumber(-1),
          mDesignator(),
          mName(),
          mPointIndex(-1)
    {}

    //! Coordinates
    inline const CCoord& coord() const
    {return mCoord;}

    inline void setCoord(CCoord& coord)
    {mCoord = coord;}

    //! Azimuth
    inline const double& azimuth() const
    {return mAzimuth;}

    inline void setAzimuth(double val)
    {mAzimuth = val;}

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

    //! Point Index
    inline int pointIndex() const
    {return mPointIndex;}

    inline void setPointIndex(int val)
    {mPointIndex = val;}

private:
    CCoord  mCoord;
    double  mAzimuth;
    int     mNumber;
    QString mDesignator;
    QString mName;
    int     mPointIndex;
};

#endif // GROUNDSTART_H