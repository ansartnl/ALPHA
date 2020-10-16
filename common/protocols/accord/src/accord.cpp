#include "accord.h"
#include "accordutility.h"


Accord::Accord()
{}

Accord::~Accord()
{}

int Accord::parseCoordinate(quint16 value) const
{
    int coeff = bool(value & M1) ? -1. : 1.;
    return ( coeff *((value >> 1) & M12) * 100);
}

quint16 Accord::generateCoordinate(int coord) const
{
    quint16 byte2 = 0;

    int xy = coord / 100;
    byte2 |= ( (quint16(xy) & M12) << 1 );
    // Positive sign set.
    if (xy >= 0)
        byte2 |= 0x0001;

    return byte2;
}
