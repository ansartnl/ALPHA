#ifndef METEOTYPES_H
#define METEOTYPES_H

namespace rewarn {

//! Meteo type
enum MeteoType
{
    Cloudburst	= 0,    // LVN
    LoStorm		= 1,	// (R)
    MedStorm	= 2,	// R)
    HiStorm		= 3,	// R
    Hail		= 4,	// G
    Squall		= 5, 	// H00
    Snow        = 6,    //S
    Rain        = 7,    //N
    Fog         = 8     //F
};

}

#endif // METEOTYPES_H
