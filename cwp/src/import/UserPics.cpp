#include "UserPics.h"

CUserPics::CUserPics()
    : m_ID(0), m_ZoneID(0)
{
}

void CUserPics::AddPoint(const TPoint& pPoint)
{
    m_Points[pPoint->GetSN()] = pPoint;
}

QDataStream &operator<<(QDataStream &out, const CUserPicsPoint &value)
{
    out << (const CBoundaryPoint &)value;
    out << value.GetUserPicsID();
    return out;
}

QDataStream &operator>>(QDataStream &in, CUserPicsPoint &value)
{
    in >> (CBoundaryPoint &)value;
    quint32 iVal(0);
    in >> iVal;
    value.SetUserPicsID(iVal);
    return in;
}
