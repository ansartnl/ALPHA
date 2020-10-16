#ifndef _REWARN_NFZ_CHECKER_H__
#define _REWARN_NFZ_CHECKER_H__

#include <QtCore/QObject>
#include <QtCore/QReadWriteLock>

#include "rewarn_global.h"
#include "AirplaneInfo.h"
#include "orm/ReFir.h"
#include "cs/CoordSystem.h"

namespace rewarn {

//! Main class for managing notifier process.
class NFZchecker : public QObject
{
    Q_OBJECT

public:
    explicit NFZchecker(QObject *parent = 0);

    void setCoordSys(const TCoordSystem &cs) { mCoordSys = cs; }

    //! Implicit shared copy of current fir map with all areas.
    /*! Function will leave only areas that have "No Conflict" type.
    */
    void setNewAreas(TFirMap firs);

    //! Returns true if airplane is inside "No Conflict" restriction zone.
    bool isInside(const AirplaneInfo &ai);

private:
    QReadWriteLock  mLock;

    /*! FIR map with "No Conflict" restriction zones */
    TFirMap         mFirs;

    /*! Reference to coordinate system */
    TCoordSystem    mCoordSys;
};

}   // namespace rewarn {

#endif // _REWARN_NFZ_CHECKER_H__
