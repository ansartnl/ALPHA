#ifndef SIMULATOR_TRACK_VAR_H
#define SIMULATOR_TRACK_VAR_H

#include "SimulatorTrack.h"

//! Variables for SimulatorTrack
struct SimulatorTrackVar
{
    typedef SimulatorTrack::TPointList::const_iterator TSimIt;
    TSimIt itSimAllPoint;
    TSimIt itSimAllPointEnd;
    TSimIt itSimBorPoint;
    TSimIt itSimBorPointEnd;
};

#endif // SIMULATOR_TRACK_VAR_H
