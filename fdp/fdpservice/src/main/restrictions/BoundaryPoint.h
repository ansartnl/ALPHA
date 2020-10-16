#ifndef BOUNDARYPOINT_H
#define BOUNDARYPOINT_H

#include "Coordinate.h"
#include "ItemHandler.h"

#include <QtCore/QSharedPointer>
#include <QtCore/QMap>

class CBoundaryPoint
{
  public:
    typedef QSharedPointer<CBoundaryPoint> TPointPtr;
    typedef QMap<int, TPointPtr> TBoundaryList;
    //! Point types
    enum Type
    {
      enLine = 'G', //!< line joining
        enCircle = 'C', //!< circle
        enLeftArc = 'L', //!< left arc
        enRightArc = 'R'
    //!< right arc
    };

    //! Constructor
    CBoundaryPoint() :
      m_ArcDistance(0),  m_Type(enLine), m_SN(0), m_ID(0)
    {
    }
    //! Destructor
    virtual ~CBoundaryPoint()
    {
    }
    //! Identifier from database

    const quint32 &GetID() const
    {
      return m_ID;
    }
    void SetID(const quint32 &V)
    {
      m_ID = V;
    }
    //! Serial number of the point

    const int &GetSN() const
    {
      return m_SN;
    }
    void SetSN(const int &V)
    {
      m_SN = V;
    }
    //! Type of the point

    const Type &GetType() const
    {
      return m_Type;
    }
    void SetType(const Type &V)
    {
      m_Type = V;
    }
    //! Coordinate of the point

    const CCoord &GetCoord() const
    {
      return m_Coord;
    }
    void SetCoord(const CCoord &V)
    {
      m_Coord = V;
    }

    const CCoord &GetArcCoord() const
    {
      return m_ArcCoord;
    }
    void SetArcCoord(const CCoord &V)
    {
      m_ArcCoord = V;
    }

    //! Arc radius

    const double &GetArcDistance() const
    {
      return m_ArcDistance;
    }
    void SetArcDistance(const double &V)
    {
      m_ArcDistance = V;
    }

    //! Get coordinate
    /*!
     \return    coordinate
     */
    CCoord& GetCoord()
    {
      return m_Coord;
    }
    //! Get arc coordinate
    /*!
     \return    coordinate
     */
    CCoord& GetArcCoord()
    {
      return m_ArcCoord;
    }
    //! Is type supported or not
    /*!
     \param iType type
     \return    true if supported
     */
    bool IsType(int iType);
    //! Set new type
    /*!
     \param cType type
     \return    true if supported
     */
    void SetType(char cType);

  private:
    CCoord m_ArcCoord;
    double m_ArcDistance;
    CCoord m_Coord;
    Type m_Type;
    int m_SN;
    quint32 m_ID;
};

QDataStream &operator<<(QDataStream &out, const CBoundaryPoint &value);
QDataStream &operator>>(QDataStream &in, CBoundaryPoint &value);

#endif // BOUNDARYPOINT_H
