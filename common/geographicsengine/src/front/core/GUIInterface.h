#ifndef _GUI_INTERFACE_H__
#define _GUI_INTERFACE_H__

#include <QTransform>
#include "cs/CoordSystem.h"

class CCoord;

//! Geodesy graphics engine
namespace geoGE {

//! Interface to graphics objects
class IGUIItemHandler
{
public:
    virtual ~IGUIItemHandler() {}
    //! Set graphics item new position
    virtual void SetPosition(const TCoordSystem &cs) = 0;
    //! Returns true if manual transformation is required for the item.
    virtual bool IsTransformNeeded() const { return false; }
    //! Set SI
    virtual void SetSI(qint16 SI) {}
};

}   // namespace geoGE

#endif // _GUI_INTERFACE_H__
