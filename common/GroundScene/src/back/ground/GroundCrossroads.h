#ifndef GROUNDCROSSROADS_H
#define GROUNDCROSSROADS_H

#include <QVector>
#include "import/ItemHandler.h"
#include "GroundCoordinate.h"

//! Used for displaying crossroads rounding areas
class GroundCrossroads :
        public ItemHandler
{
public:
    GroundCrossroads()
       : mCoord(),
        mIndex(-1),
        mAzimuths(),
        mWidths()
    {}

    //! Coordinates
    inline const CCoord& coord() const
    {return mCoord;}

    inline void setCoord(CCoord& coord)
    {mCoord = coord;}

    //! Azimuths
    bool addAzimuth(double val)
    {
        if(!mAzimuths.contains(val))
        {
            mAzimuths.append(val);
            return true;
        }
        else
            return false;
    }

    //! Index
    inline int index() const
    {return mIndex;}

    inline void setIndex(int val)
    {mIndex = val;}

    inline const QList<double>& azimuths() const
    {return mAzimuths;}

    //! Widths
    inline void addWidth(double val)
    {mWidths.append(val);}

    inline const QList<double>& getWidths() const
    {return mWidths;}

private:
    CCoord  mCoord;
    int     mIndex;
    QList<double> mAzimuths;
    QList<double> mWidths;
};

#endif // GROUNDCROSSROADS_H
