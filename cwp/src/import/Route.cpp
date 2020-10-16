#include "Route.h"

CRoute::TPointMap::iterator CRoute::FindPoint(const QSharedPointer<CPoint>& pPoint)
{
  TPointMap::iterator itResult = GetRoutPoints().begin();
  for(; itResult != GetRoutPoints().end(); ++itResult)
  {
    if (*(*itResult)->GetPoint() == *pPoint)
      break;
  }
  return itResult;
}
