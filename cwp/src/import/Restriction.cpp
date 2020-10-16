#include "Restriction.h"

CRestriction::TDescription CRestriction::TYPE_DESCRIPTION = []{
    CRestriction::TDescription result;

    result.insert(CRestriction::enAlert, QString("Alert"));
    result.insert(CRestriction::enCaution, QString("Caution"));
    result.insert(CRestriction::enDanger, QString("Danger"));
    result.insert(CRestriction::enMilitary, QString("Military"));
    result.insert(CRestriction::enProhibited, QString("Prohibited"));
    result.insert(CRestriction::enRestricted, QString("Restricted"));
    result.insert(CRestriction::enTraining, QString("Training"));
    result.insert(CRestriction::enWarning, QString("Warning"));
    result.insert(CRestriction::enUnknown, QString("Unknown"));

    return result;
}();

CRestrictionUser::TDescription CRestrictionUser::TYPE_DESCRIPTION = []{
    CRestrictionUser::TDescription result;

    result.insert(CRestrictionUser::enObstacle, QString("Obstacle"));
    result.insert(CRestrictionUser::enNFZ, QString("No Conflict Zone"));
    result.insert(CRestrictionUser::enNRI, QString("Zone without RI"));

    result.insert(CRestrictionUser::enAlert, QString("Alert"));
    result.insert(CRestrictionUser::enCaution, QString("Caution"));
    result.insert(CRestrictionUser::enDanger, QString("Danger"));
    result.insert(CRestrictionUser::enMilitary, QString("Military"));
    result.insert(CRestrictionUser::enProhibited, QString("Prohibited"));
    result.insert(CRestrictionUser::enRestricted, QString("Restricted"));
    result.insert(CRestrictionUser::enTraining, QString("Training"));
    result.insert(CRestrictionUser::enWarning, QString("Warning"));
    result.insert(CRestrictionUser::enUnknown, QString("Unknown"));

    result.insert(CRestrictionUser::enMap, QString("Map"));

    return result;
}();


CRestriction::CRestriction() :
  m_ID(0), m_ZoneID(0), m_Type(enUnknown)
{
}

void CRestriction::AddPoint(const TPoint& pPoint)
{
  m_Points[pPoint->GetSN()] = pPoint;
}

bool CRestriction::IsType(int iType)
{
  return iType == enAlert || iType == enCaution || iType == enDanger || iType
      == enMilitary || iType == enProhibited || iType == enRestricted || iType
      == enTraining || iType == enWarning || iType == enUnknown;
}

void CRestriction::SetType(char cType)
{
  if (IsType((int) cType))
    SetType((Type) cType);
  else
    SetType(enUnknown);
}

CRestrictionUser::CRestrictionUser() :
    m_ID(0), m_ZoneID(0), m_Type(enDanger), m_HMin(-1000000000), m_HMax(1000000000), m_qGUID(QUuid::createUuid()), actual_(Qt::Unchecked)
{
}

void CRestrictionUser::AddPoint(const TPoint& pPoint)
{
    m_Points[pPoint->GetSN()] = pPoint;
}

bool CRestrictionUser::isValid(const QDateTime &current) const
{
    if (m_Type == enMap)
        return true;

    if ((m_TimeBegin.isNull() || m_TimeBegin <= current)
         && (m_TimeEnd.isNull() || current <= m_TimeEnd))
        return true;
    return false;
}

QDataStream &operator<<(QDataStream &out, const CRestrictionUser &value)
{
    out << value.GetID();
    out << value.GetZoneID();
    out << quint32(value.GetType());
    out << value.GetName();
    out << value.GetHMin();
    out << value.GetHMax();
    out << value.GetTimeBegin();
    out << value.GetTimeEnd();
    out << value.GetGUID();
    out << value.GetPoints();
    return out;
}

QDataStream &operator>>(QDataStream &in, CRestrictionUser &value)
{
    quint32 iVal(0);
    in >> iVal;
    value.SetID(iVal);
    in >> iVal;
    value.SetZoneID(iVal);
    in >> iVal;
    value.SetType(CRestrictionUser::Type(iVal));
    QString sVal;
    in >> sVal;
    value.SetName(sVal);
    double dVal(0);
    in >> dVal;
    value.SetHMin(dVal);
    in >> dVal;
    value.SetHMax(dVal);
    QDateTime dtVal;
    in >> dtVal;
    value.SetTimeBegin(dtVal);
    in >> dtVal;
    value.SetTimeEnd(dtVal);
    QUuid uid;
    in >> uid;
    value.SetGUID(uid);
    in >> value.GetPoints();
    return in;
}

QDataStream &operator<<(QDataStream &out, const CRestrictionUser::TPointList &value)
{
    out << quint32(value.size());
    CRestrictionUser::TPointList::ConstIterator it = value.constEnd();
    CRestrictionUser::TPointList::ConstIterator begin = value.constBegin();
    while (it != begin)
    {
        --it;
        out << it.key() << *it.value();
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, CRestrictionUser::TPointList &value)
{
    QDataStream::Status oldStatus = in.status();
    in.resetStatus();
    value.clear();

    quint32 n;
    in >> n;

    value.detach();
//    value.setInsertInOrder(true);
    for (quint32 i = 0; i < n; ++i)
    {
        if (in.status() != QDataStream::Ok)
            break;

        CRestrictionUser::TPointList::key_type mapKey;
        CRestrictionUser::TPointList::mapped_type mapValue(new CRestrictionUser::TPointList::mapped_type::element_type);
        in >> mapKey >> *mapValue;
        value.insertMulti(mapKey, mapValue);
    }
//    value.setInsertInOrder(false);
    if (in.status() != QDataStream::Ok)
        value.clear();
    if (oldStatus != QDataStream::Ok)
        in.setStatus(oldStatus);
    return in;
}

QDataStream &operator<<(QDataStream &out, const CRestrictPoint &value)
{
    out << (const CBoundaryPoint &)value;
    out << value.GetRestrictID();
    return out;
}

QDataStream &operator>>(QDataStream &in, CRestrictPoint &value)
{
    in >> (CBoundaryPoint &)value;
    quint32 iVal(0);
    in >> iVal;
    value.SetRestrictID(iVal);
    return in;
}
