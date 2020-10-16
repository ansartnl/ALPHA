#ifndef NDBNAVAID_H
#define NDBNAVAID_H

#include "Ndb.h"
#include "Navaid.h"

struct NdbNavaid : public Ndb, public Navaid
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }
};

#endif // NDBNAVAID_H
