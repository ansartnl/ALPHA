#include "BoundaryPoint.h"

bool CBoundaryPoint::IsType(int iType)
{
  return iType == enLine || iType == enCircle || iType == enLeftArc || iType
      == enRightArc;
}

void CBoundaryPoint::SetType(char cType)
{
  if (IsType((int) cType))
    SetType((Type) cType);
  else
    SetType(enLine);
}

QDataStream &operator<<(QDataStream &out, const CBoundaryPoint &value)
{
    out << value.GetID();
    out << value.GetSN();
    out << quint32(value.GetType());
    out << value.GetCoord();
    out << value.GetArcCoord();
    out << value.GetArcDistance();
    return out;
}

QDataStream &operator>>(QDataStream &in, CBoundaryPoint &value)
{
    quint32 iVal(0);
    in >> iVal;
    value.SetID(iVal);
    in >> iVal;
    value.SetSN(iVal);
    in >> iVal;
    value.SetType(CBoundaryPoint::Type(iVal));
    CCoord coord;
    in >> coord;
    value.SetCoord(coord);
    in >> coord;
    value.SetArcCoord(coord);
    double dVal(0);
    in >> dVal;
    value.SetArcDistance(dVal);
    return in;
}
