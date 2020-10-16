#ifndef USERPICS_H
#define USERPICS_H

#include "Coordinate.h"
#include "BoundaryPoint.h"
#include "ItemHandler.h"
#include <QtCore/QMap>

//! Class describes the point of a user pics
class CUserPicsPoint: public CBoundaryPoint
{
public:
    CUserPicsPoint() :
      m_UserPicsID(0)
    {}

    //! UserPics identifier from database
    const quint32 &GetUserPicsID() const { return m_UserPicsID; }
    void SetUserPicsID(const quint32 &V) { m_UserPicsID = V; }

private:
    quint32 m_UserPicsID;
};

QDataStream &operator<<(QDataStream &out, const CUserPicsPoint &value);
QDataStream &operator>>(QDataStream &in, CUserPicsPoint &value);

//! Class describes a user pics
class CUserPics: public ItemHandler
{
  public:
    //! Smart pointer to user pics point
    typedef QSharedPointer<CUserPicsPoint> TPoint;
    //! Map of user pics points. Key - serial number
    typedef QMap<int, TPoint> TPointList;

    //! Constructor
    CUserPics();

    //@{
    //! Identifier from database
    const quint32 &GetID() const { return m_ID; }
    void SetID(const quint32 &V) { m_ID = V; }
    //@}

    //! The name of a user pics
    const QString &GetName() const { return m_Name; }
    void SetName(const QString &V) { m_Name = V; }

    //! The identifier of a zone , whom belongs this user pics
    const quint32 &GetZoneID() const { return m_ZoneID; }
    void SetZoneID(const quint32 &V) { m_ZoneID = V; }

    //! User pics point list
    const TPointList &GetPoints() const { return m_Points; }
    TPointList& GetPoints() { return m_Points; }
    void SetPoints(const TPointList &V) { m_Points = V; }
    //! Add user pics point
    /*!
     \param pPoint pointer to user pics point
     */
    void AddPoint(const TPoint& pPoint);

private:
    quint32 m_ID;
    quint32 m_ZoneID;
    QString m_Name;
    TPointList m_Points;
};

#endif // USERPICS_H
