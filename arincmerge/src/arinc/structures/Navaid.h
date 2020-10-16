#ifndef NAVAID_H
#define NAVAID_H

#include "ArincBaseStruct.h"

struct Navaid : public ArincBaseStruct
{
    static const char SectionCode;
    inline virtual char sectionCode() const { return SectionCode; }
};

#endif // NAVAID_H
