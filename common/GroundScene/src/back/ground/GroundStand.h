#ifndef GROUND_STAND_H
#define GROUND_STAND_H

#include <QtGlobal>

struct GroundStand
{
    GroundStand()
        : mId(0), mStandId(0), mAirportId(0), mExerciseId(0), mFplId(0), mStateId(0) {}

    quint32     mId;
    quint32     mStandId;
    QString     mStandName;
    quint32     mAirportId;
    QString     mAirportName;
    quint32     mExerciseId;
    QString     mExerciseName;
    quint32     mFplId;
    QString     mFplName;
    quint16     mStateId;
    QString     mStateName;
};

#endif // GROUND_STAND_H
