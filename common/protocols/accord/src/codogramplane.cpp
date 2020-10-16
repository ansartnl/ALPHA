#include "codogramplane.h"


bool CodogramPlane::testInformationType(quint8 value) const
{
    return !( value < quint8(IT_XY) || value > quint8(IT_XYIBNB) );
}

bool CodogramPlane::testInformationSource(quint8 value) const
{
    if ( value != quint8(IS_PRL) && value != quint8(IS_VRL) && value != quint8(IS_PRL_VRL) )
        return false;
    return true;
}

bool CodogramPlane::testProhibition(quint8 value) const
{
    return ( value != 3 );
}
