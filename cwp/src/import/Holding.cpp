#include "Holding.h"

const quint32 CHolding::m_qDefaultSpeed = 118.322222222222;

void CHolding::SetType(char cType)
{
  if (cType >= enUndef && cType <= enUndef2)
    SetType((Type) (int) cType);
  else
    SetType(enUndef);

}

void CHolding::SetTurn(char cTurn)
{
  if (cTurn == enLeft || cTurn == enRight)
    SetTurn((Turn) (int) cTurn);
}


void CHolding::ResetAlbanianPoint()
{
  SetAlbanianIterator(GetMasterPoints().begin());
  if (GetAlbanianIterator() != GetMasterPoints().end())
    SetCurrentSN(GetAlbanianIterator().key());
}

QSharedPointer<CPoint> CHolding::GetAlbanianPoint()
{
  if (GetAlbanianIterator() != GetMasterPoints().end())
    return *GetAlbanianIterator();
  return QSharedPointer<CPoint>();
}

QSharedPointer<CPoint> CHolding::GetNextAlbanianPoint()
{
  TPointList::const_iterator itResult = GetAlbanianIterator() + 1;
  if (itResult == GetMasterPoints().end())
    itResult = GetMasterPoints().begin();

  if (itResult != GetMasterPoints().end())
    return *itResult;
  return QSharedPointer<CPoint>();
}

void CHolding::NextAlbanianPoint()
{
  SetAlbanianIterator(GetAlbanianIterator() + 1);
  if (GetAlbanianIterator() == GetMasterPoints().end())
    SetAlbanianIterator(GetMasterPoints().begin());
  if (GetAlbanianIterator() != GetMasterPoints().end())
    SetCurrentSN(GetAlbanianIterator().key());
}

