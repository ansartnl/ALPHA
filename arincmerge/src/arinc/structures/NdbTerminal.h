#ifndef NDBTERMINAL_H
#define NDBTERMINAL_H

#include "Ndb.h"
#include "Airport.h"

struct NdbTerminal : public Ndb, public Airport
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }
};

#endif // NDBTERMINAL_H
