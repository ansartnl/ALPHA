#include "Point.h"

CRoutePoint::CRoutePoint(unsigned int iSN,
  const QSharedPointer<CPoint>& pPoint, unsigned int uiRoutID,
  unsigned int uiID) :
  m_bSpeed(false), m_bLevel(false), m_Custom(false), m_IsSTAR(false), m_IsSID(false),
  m_iSN(iSN), m_pPoint(pPoint), m_uiRoutID(uiRoutID), m_uiID(uiID), m_bEnd(false)

{
}

CRoutePoint::CRoutePoint() :
  m_bSpeed(false), m_bLevel(false), m_Custom(false), m_IsSTAR(false), m_IsSID(false),
  m_iSN(0), m_uiRoutID(0), m_uiID(0), m_bEnd(false)
{
}

CPointKey::CPointKey(const QString& qsName)
{
  Init();
  SetName(qsName);
}

CPointKey::CPointKey(const QString& qsName, CPoint::Type Type,
  const QString& qsICAO)
{
  Init();
  SetName(qsName);
  SetType(Type);
  SetICAO(qsICAO);
}

CPointKey::CPointKey(const QString& qsName, CPoint::Type Type,
  const QString& qsICAO, const QString& AirportName)
{
  Init();
  SetName(qsName);
  SetType(Type);
  SetICAO(qsICAO);
  SetAirportName(AirportName);
}

bool CPointKey::operator<(const CPointKey& ref2nd) const
{
  if (GetName() != ref2nd.GetName())
    return GetName() < ref2nd.GetName();

  // patch
  if (!IsType() && ref2nd.IsType() && (ref2nd.GetType()
      == CPoint::enAirportPoint || ref2nd.GetType() == CPoint::enAirportNDB))
    return true;
  if (IsType() && !ref2nd.IsType() && (GetType() == CPoint::enAirportPoint
      || GetType() == CPoint::enAirportNDB))
    return false;

  if (IsType() && ref2nd.IsType() && GetType() != ref2nd.GetType())
    return GetType() < ref2nd.GetType();
  if (IsICAO() && ref2nd.IsICAO() && GetICAO() != ref2nd.GetICAO())
    return GetICAO() < ref2nd.GetICAO();
  if (IsAirportName() && ref2nd.IsAirportName() && GetAirportName()
      != ref2nd.GetAirportName())
    return GetAirportName() < ref2nd.GetAirportName();

  return false;
}

void CPointKey::Init()
{
  m_bName = false;
  m_bType = false;
  m_bICAO = false;
  m_bAirportName = false;
}
