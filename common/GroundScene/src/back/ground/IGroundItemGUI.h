#ifndef IGROUNDITEMGUI_H
#define IGROUNDITEMGUI_H

#include "back/ground/GroundPoint.h"

class IGroundItemGUI
{
public:
    virtual ~IGroundItemGUI(){}

    virtual const CCoord& coordinate() const = 0;
    virtual void ShowPathPoints(bool bShow = true) = 0;
    //! Sets alternative path points (for displaying only)
    virtual void setPathPoints(const QList<QSharedPointer<GroundPoint> >& points) = 0;
    //! Clears alternative path points
    virtual void clearPathPoints() = 0;
};

#endif // IGROUNDITEMGUI_H
