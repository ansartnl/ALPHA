#ifndef AIRSPACE_H
#define AIRSPACE_H

#include "ArincBaseStruct.h"

struct Airspace : public ArincBaseStruct
{
    static const char SectionCode;
    inline virtual char sectionCode() const { return SectionCode; }
};

#endif // AIRSPACE_H
