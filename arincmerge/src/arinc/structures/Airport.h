#ifndef AIRPORT_H
#define AIRPORT_H

#include "ArincBaseStruct.h"

struct Airport : public ArincBaseStruct
{
    static const char SectionCode;
    inline virtual char sectionCode() const { return SectionCode; }
};

#endif // AIRPORT_H
