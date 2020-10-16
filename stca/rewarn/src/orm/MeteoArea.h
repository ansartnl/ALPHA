#ifndef METEOAREA_H
#define METEOAREA_H

#include "Area.h"
#include "MeteoTypes.h"

namespace rewarn {

struct MeteoArea : public Area
{
    MeteoType mType;
};

}
#endif // METEOAREA_H
