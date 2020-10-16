#ifndef RESTRICTION_H
#define RESTRICTION_H

#include "Coordinate.h"
#include "BoundaryPoint.h"
#include "ItemHandler.h"
#include <QtCore/QMap>
#include <QtCore/QDateTime>
#include <QtCore/QDataStream>
#include <QtCore/QUuid>
#include <QHash>

//! Class describes the point of a restriction zone
class CRestrictPoint: public CBoundaryPoint
{
public:
    CRestrictPoint() :
      m_RestrictID(0)
    {}

    //! Restriction identifier from database
    const quint32 &GetRestrictID() const { return m_RestrictID; }
    void SetRestrictID(const quint32 &V) { m_RestrictID = V; }

private:
    quint32 m_RestrictID;
};

QDataStream &operator<<(QDataStream &out, const CRestrictPoint &value);
QDataStream &operator>>(QDataStream &in, CRestrictPoint &value);

//! Class describes a restriction zone
class CRestriction: public ItemHandler
{
  public:
    //! Smart pointer to restricted point
    typedef QSharedPointer<CRestrictPoint> TPoint;
    //! Map of restricted points.Key - serial number
    typedef QMap<int, TPoint> TPointList;

    typedef QMap<int, QString> TDescription;

    static TDescription  TYPE_DESCRIPTION;
    //! Restriction type
    enum Type
    {
        enAlert = 'A', //!< Alert
        enCaution = 'C', //!< Caution
        enDanger = 'D', //!< Danger
        enMilitary = 'M', //!< Military
        enProhibited = 'P', //!< Prohibited
        enRestricted = 'R', //!< Restricted
        enTraining = 'T', //!< Training
        enWarning = 'W', //!< Warning
        enUnknown = 'U' //!< Unknown
    };

    //! Constructor
    CRestriction();

    //@{
    //! Identifier from database
    const quint32 &GetID() const { return m_ID; }
    void SetID(const quint32 &V) { m_ID = V; }
    //@}

    //! The name of a restriction zone
    const QString &GetName() const { return m_Name; }
    void SetName(const QString &V) { m_Name = V; }

    //! Multicode
    const QString &GetMultiCode() const { return m_MultiCode; }
    void SetMultiCode(const QString &V) { m_MultiCode = V; }

    //! The type of a restriction zone
    const Type &GetType() const { return m_Type; }
    void SetType(const Type &V) { m_Type = V; }
    //! Is type supported or not
    /*!
         \param iType type
         \return    true if supported
     */
    bool IsType(int iType);
    //! Set type
    /*!
         \param cType type
     */
    void SetType(char cType);

    //! The designation of a restriction zone
    const QString &GetDesignation() const { return m_Designation; }
    void SetDesignation(const QString &V) { m_Designation = V; }

    //! The identifier of a zone , whom belongs this restriction
    const quint32 &GetZoneID() const { return m_ZoneID; }
    void SetZoneID(const quint32 &V) { m_ZoneID = V; }

    //! Restricted point list
    const TPointList &GetPoints() const { return m_Points; }
    TPointList& GetPoints() { return m_Points; }
    void SetPoints(const TPointList &V) { m_Points = V; }
    //! Add restricted point
    /*!
     \param pPoint pointer to restricted point
     */
    void AddPoint(const TPoint& pPoint);

    //! List points to master
    const TPointList &GetMasterPoints() const { return m_MasterPoints; }
    void SetMasterPoints(const TPointList &V) { m_MasterPoints = V; }

private:
    quint32 m_ID;
    quint32 m_ZoneID;
    Type m_Type;
    QString m_Name;
    QString m_MultiCode;
    QString m_Designation;
    TPointList m_Points;
    TPointList m_MasterPoints;
};


//! Class describes a restriction user zone
class CRestrictionUser: public ItemHandler
{
public:
    typedef QMap<int, QString> TDescription;
    static TDescription TYPE_DESCRIPTION;
    //! Restriction type
    enum Type
    {
        enObstacle      = 'O',  //!< Obstacle
        enNFZ           = 'Z',  //!< No Conflict Zone
        enNRI           = '[',  //!< Zone without RI

        enAlert         = 'A',  //!< Alert
        enCaution       = 'C',  //!< Caution
        enDanger        = 'D',  //!< Danger
        enMilitary      = 'M',  //!< Military
        enProhibited    = 'P',  //!< Prohibited
        enRestricted    = 'R',  //!< Restricted
        enTraining      = 'T',  //!< Training
        enWarning       = 'W',  //!< Warning
        enUnknown       = 'U',  //!< Unknown

        enMap           = 'X'   //!< Map
    };

    //! Constructor
    CRestrictionUser();

    //! Smart pointer to restricted point
    typedef QSharedPointer<CRestrictPoint> TPoint;
    //! Map of restricted points.Key - serial number
    typedef QMap<int, TPoint> TPointList;

    //@{
    //! Identifier from database
    const quint32 &GetID() const { return m_ID; }
    void SetID(const quint32 &V) { m_ID = V; }
    //@}

    //@{
    //! Type of restriction.
    const Type &GetType() const { return m_Type; }
    void SetType(const Type &V) { m_Type = V; }
    //@}

    //@{
    //! The identifier of a zone , whom belongs this restriction
    const quint32 &GetZoneID() const { return m_ZoneID; }
    void SetZoneID(const quint32 &V) { m_ZoneID = V; }
    //@}

    //@{
    //! The name of a restriction zone
    const QString &GetName() const { return m_Name; }
    void SetName(const QString &V) { m_Name = V; }
    //@}

    //@{
    //! Minimum zone height
    double GetHMin() const { return m_HMin; }
    void SetHMin(double V) { m_HMin = V; }
    //@}

    //@{
    //! Maximum zone height
    double GetHMax() const { return m_HMax; }
    void SetHMax(double V) { m_HMax = V; }
    //@}

    //@{
    //! Start time when zone is available.
    const QDateTime& GetTimeBegin() const { return m_TimeBegin; }
    void SetTimeBegin(const QDateTime &V) { m_TimeBegin = V; }
    //@}

    //@{
    //! Stop time when zone is available.
    const QDateTime& GetTimeEnd() const { return m_TimeEnd; }
    void SetTimeEnd(const QDateTime &V) { m_TimeEnd = V; }
    //@}

    //@{
    //! GUID
    const QUuid& GetGUID() const { return m_qGUID; }
    void SetGUID(const QUuid &V) { m_qGUID = V; }
    //@}

    //! Returns true if current time is between start and stop time.
    bool isValid(const QDateTime &current) const;

    //@{
    //! Restricted point list
    void AddPoint(const TPoint& pPoint);
    TPointList& GetPoints() { return m_Points; }
    const TPointList &GetPoints() const { return m_Points; }
    void SetPoints(const TPointList &V) { m_Points = V; }
    //@}

    Qt::CheckState actual() const { return actual_; };
    void setActual(Qt::CheckState actual) { actual_ = actual; };

private:
    quint32 m_ID;
    quint32 m_ZoneID;
    Type m_Type;
    QString m_Name;
    double m_HMin;
    double m_HMax;
    QDateTime m_TimeBegin;
    QDateTime m_TimeEnd;
    QUuid m_qGUID;
    TPointList m_Points;

    Qt::CheckState actual_;
};

QDataStream &operator<<(QDataStream &out, const CRestrictionUser::TPointList &value);
QDataStream &operator>>(QDataStream &in, CRestrictionUser::TPointList &value);

QDataStream &operator<<(QDataStream &out, const CRestrictionUser &value);
QDataStream &operator>>(QDataStream &in, CRestrictionUser &value);

#endif // RESTRICTION_H
