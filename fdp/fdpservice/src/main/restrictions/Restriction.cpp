#include "Restriction.h"

void CRestrictionUser::AddPoint(const TPoint& pPoint)
{
    m_Points[pPoint->GetSN()] = pPoint;
}

bool CRestrictionUser::isValid(const QDateTime &current) const
{
    if ( (m_TimeBegin.isNull() || m_TimeBegin <= current)
         && (m_TimeEnd.isNull() || current <= m_TimeEnd) )
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
    value.setInsertInOrder(true);
    for (quint32 i = 0; i < n; ++i)
    {
        if (in.status() != QDataStream::Ok)
            break;

        CRestrictionUser::TPointList::key_type mapKey;
        CRestrictionUser::TPointList::mapped_type mapValue(new CRestrictionUser::TPointList::mapped_type::element_type);
        in >> mapKey >> *mapValue;
        value.insertMulti(mapKey, mapValue);
    }
    value.setInsertInOrder(false);
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
