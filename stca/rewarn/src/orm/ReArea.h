#ifndef _RE_AREA_H__
#define _RE_AREA_H__

#include <QtCore/QMap>
#include <QtCore/QRectF>
#include <QtCore/QDateTime>
#include <QtGui/QPolygonF>

#ifdef REWARN_TEST
#   include "rewarn_global.h"
#endif

#include "Area.h"
#include "ReTypes.h"
#include "RePoint.h"

namespace rewarn {

//! Structure describes a restriction zone
#ifdef REWARN_TEST
    struct REWARN_EXPORT ReArea
#else
    struct ReArea : public Area
#endif
{
    //! Restriction zone state
    enum State
    {
        Disabled,       //!< Zone is disabled
        Enabled,        //!< Zone is enabled if current time is inside working interval
        AlwaysEnabled   //!< Zone is always enabled
    };

    //! Constructor
    ReArea() : mType(enUnknown), mState(Enabled) {}

    //! Returns true if input datetime is inside working interval.
    bool isValid(const QDateTime &dt) const;

    //! Returns true restriction zone is user zone.
    bool isUser() const;

public:
    QString mMultiCode;
    ReType  mType;
    State   mState;
    QString mDesignation;
    QDateTime mTimeBegin;
    QDateTime mTimeEnd;
};

}   // namespace rewarn {

#endif // _RE_AREA_H__
