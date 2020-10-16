#ifndef GROUNDSIGN_H
#define GROUNDSIGN_H

#include <QVector>
#include "GroundCoordinate.h"
#include "import/ItemHandler.h"

namespace ground_sign{
const int HOVER_RADIUS = 20;
}

class GroundSign
        :ItemHandler
{
public:

    GroundSign()
        : mCoord(),
          mLabel()
    {}

    //! Coordinates
    inline const CCoord& coord() const
    {return mCoord;}

    inline void setCoord(CCoord& coord)
    {mCoord = coord;}

    //! Label
    inline const QString& label() const
    {return mLabel;}

    inline void setLabel(QString& label)
    {mLabel = label;}

private:
    CCoord  mCoord;
    QString mLabel;
};

#endif // GROUNDSIGN_H
