#include "ListACData.h"

const int CListACData::GetDeltaIPS_Small(int iDelta/* = 0*/)
{
    static int delta = 10;
    if(iDelta) delta = iDelta;
    return delta;
}

const int CListACData::GetDeltaIPS_Big(int iDelta/* = 0*/)
{
    static int delta = 30;
    if(iDelta) delta = iDelta;
    return delta;
}

CListACData::CListACData()
	: m_IPS_Small(0), m_IPS_Big(0)
{
}
