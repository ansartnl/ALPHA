#include "ReArea.h"

namespace rewarn {

bool ReArea::isUser() const
{
    return mType == enUser || mType == enUserObstacle || mType == enUserNFZ;
}

bool ReArea::isValid(const QDateTime &dt) const
{
    if ( mState == Disabled )
        return false;
    if ( mState == AlwaysEnabled )
        return true;
    if ( mTimeBegin.isNull() && mTimeEnd.isNull() )
        return true;
    else if ( mTimeBegin.isNull() && dt <= mTimeEnd )
        return true;
    else if ( mTimeEnd.isNull() && mTimeBegin <= dt )
        return true;
    else if ( mTimeBegin <= dt && dt <= mTimeEnd )
        return true;
    return false;
}

}   // namespace rewarn {
