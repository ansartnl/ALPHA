#ifndef ENROUTE_H
#define ENROUTE_H

#include "ArincBaseStruct.h"

struct Enroute : public ArincBaseStruct
{
    static const char SectionCode;
    inline virtual char sectionCode() const { return SectionCode; }
};

#endif // ENROUTE_H
