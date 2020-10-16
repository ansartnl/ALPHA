#include "Airport.h"

const CCoord& CAirport::GetPlace() const
{
    return GetCoord();
}

double CAirport::GetAirportAlt() const
{
    return GetAlt();
}

CAirport::TRunways::iterator CAirport::AddRunway(const TRunwayPtr& pPtr)
{
    return m_Runways.insert(pPtr->GetName(), pPtr);
}

void CAirport::MarkItemAllActive(bool bActive)
{
    MarkItemActive(bActive);

    TRunways::iterator itScan = m_Runways.begin();
    for (; itScan != m_Runways.end(); ++itScan)
        (*itScan)->MarkItemActive(bActive);
}

QSharedPointer<CSSA> CAirport::Find(const QString& sSSA, CSSA::Type SSAType)
{
    TRunways::iterator itScan = GetRunways().begin();
    for (; itScan != GetRunways().end(); ++itScan) {
        CRunway::TSSA::iterator itFindSSA = (*itScan)->GetSSA().find(CSSAKey(sSSA,
                                                                             SSAType));
        if (itFindSSA != (*itScan)->GetSSA().end())
            return itFindSSA->dynamicCast<CSSA> ();
    }
    return QSharedPointer<CSSA> ();
}
