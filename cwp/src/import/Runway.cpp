#include "Runway.h"

CRunway::TSSA::iterator CRunway::AddSSA(const TSSAPtr& pSSA)
{
    return m_SSA.insert(CSSAKey(pSSA->GetName(), pSSA->GetType()), pSSA);
}

const CCoord& CRunway::GetPlace() const
{
    if (GetCoord().IsNull())
        return m_pParent->GetPlace();
    return GetCoord();
}

double CRunway::GetRunwayAlt() const
{
    return m_pParent->GetAirportAlt();
}

double CRunway::GetDirection() const
{
    return GetAzimuth() + GetParent()->GetDeclination();    //+ declination
}

double CRunway::GetRunwayLength() const
{
    return GetLength();
}

const QString& CRunway::GetRunwayName() const
{
    return GetName();
}

void CRunway::MarkItemAllActive(bool bActive)
{
    MarkItemActive(bActive);

    TSSA::iterator itScan = m_SSA.begin();
    for (; itScan != m_SSA.end(); ++itScan)
        (*itScan)->MarkItemActive(bActive);
}
