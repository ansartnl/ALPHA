#ifndef HOLDING_H
#define HOLDING_H

#include "Point.h"
#include "ItemHandler.h"

#include <QtCore/QMap>

class Dao;

//! Class describes the holding pattern
class CHolding: public ItemHandler
{
  public:
    //! Smart pointer to holding point
    typedef QSharedPointer<CPoint> TPoint;
    //! Map of holding points.Key - serial number
    typedef QMap<int, TPoint> TPointList;
    //! Type of a holding pattern
    enum Type
    {
      enUndef = '0', //!< Undefine
        enHighAlt = '1', //!< High altitude
        enLowAlt = '2', //!< Low altitude
        enSID = '3', //!< SID
        enSTAR = '4', //!< STAR
        enAppr = '5', //!< Approach
        enMisAppr = '6', //!< Missed approach
        enUndef2 = '7'
    //!< Undefined 2
    };
    //! Turn direction
    enum Turn
    {
      enLeft = 'L', //!< Left
        enRight = 'R'
    //!< Right
    };
    //! Constructor
    CHolding() :
      m_ID(0), m_Type(enUndef), m_InCourse(0), m_Turn(enLeft), m_LegLength(0),
          m_LegTime(0), m_Speed(0), m_ZoneID(0)
    {
    }
    //! Destructor
    virtual ~CHolding()
    {
    }
    //! Identifier of holding pattern

  public:
    const quint32 &GetID() const
    {
      return m_ID;
    }
    void SetID(const quint32 &V)
    {
      m_ID = V;
    }
  private:
    quint32 m_ID;
    //! Point

  public:
    const TPoint &GetPoint() const
    {
      return m_Point;
    }
    void SetPoint(const TPoint &V)
    {
      m_Point = V;
    }
  private:
    TPoint m_Point;
    //! Type of holding pattern

  public:
    const Type &GetType() const
    {
      return m_Type;
    }
    void SetType(const Type &V)
    {
      m_Type = V;
    }
  private:
    Type m_Type;
    //! Inbound course on point

  public:
    const double &GetInCourse() const
    {
      return m_InCourse;
    }
    void SetInCourse(const double &V)
    {
      m_InCourse = V;
    }
  private:
    double m_InCourse;
    //! Turn direction

  public:
    const Turn &GetTurn() const
    {
      return m_Turn;
    }
    void SetTurn(const Turn &V)
    {
      m_Turn = V;
    }
  private:
    Turn m_Turn;
    //! Leg length

  public:
    const double &GetLegLength() const
    {
      return m_LegLength;
    }
    void SetLegLength(const double &V)
    {
      m_LegLength = V;
    }
  private:
    double m_LegLength;
    //! Leg time

  public:
    const int &GetLegTime() const
    {
      return m_LegTime;
    }
    void SetLegTime(const int &V)
    {
      m_LegTime = V;
    }
  private:
    int m_LegTime;
    //! Speed limit in holding pattern

  public:
    const double &GetSpeed() const
    {
      return m_Speed;
    }
    void SetSpeed(const double &V)
    {
      m_Speed = V;
    }
  private:
    double m_Speed;
    //! Zone identifier, whom belongs this pattern

  public:
    const quint32 &GetZoneID() const
    {
      return m_ZoneID;
    }
    void SetZoneID(const quint32 &V)
    {
      m_ZoneID = V;
    }
  private:
    quint32 m_ZoneID;
    //! Current point in master list

  public:
    const TPointList &GetMasterPoints() const
    {
      return m_MasterPoints;
    }
    void SetMasterPoints(const TPointList &V)
    {
      m_MasterPoints = V;
    }
  private:
    TPointList m_MasterPoints;

  public:
    //! Set type of holding pattern
    /*!
     \param cType type of holding pattern
     */
    void SetType(char cType);
    //! Set turn direction
    /*!
     \param cTurn turn direction
     */
    void SetTurn(char cTurn);
    //! Get master point list
    TPointList& GetMasterPoints()
    {
      return m_MasterPoints;
    }

  public:
    const int &GetCurrentSN() const
    {
      return m_CurrentSN;
    }
    void SetCurrentSN(const int &V)
    {
      m_CurrentSN = V;
    }
  private:
    int m_CurrentSN;

  public:
    //! Reset current sequence number
    void ResetAlbanianPoint();
    //! Get master points
    QSharedPointer<CPoint> GetAlbanianPoint();
    //! Get next master point from list
    QSharedPointer<CPoint> GetNextAlbanianPoint();
    //! Calculate next master point from list
    void NextAlbanianPoint();
  public:
    //! Default speed in holding pattern
    static const quint32 m_qDefaultSpeed;
  protected:
    

  public:
    const TPointList::const_iterator &GetAlbanianIterator() const
    {
      return m_AlbanianIterator;
    }
    void SetAlbanianIterator(const TPointList::const_iterator &V)
    {
      m_AlbanianIterator = V;
      MarkAlbanianIterator(true);
    }
    bool IsAlbanianIterator() const
    {
      return m_bAlbanianIterator;
    }
    void MarkAlbanianIterator(bool bAlbanianIterator)
    {
      m_bAlbanianIterator = bAlbanianIterator;
    }
  private:
    TPointList::const_iterator m_AlbanianIterator;
    bool m_bAlbanianIterator;
};

#endif // HOLDING_H
