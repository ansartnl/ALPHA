#ifndef ROUTE_H
#define ROUTE_H

#include "Point.h"
#include "ItemHandler.h"
#include <QtCore/QMap>

//! Class describes the route
class CRoute: public ItemHandler
{
  public:
    //! Map of route points. Key - point identifier
    typedef QMap<int, QSharedPointer<CRoutePoint> > TPointMap;
    //! Constructor
    CRoute()
    {
    }
    ;
    //! Destructor
    virtual ~CRoute()
    {
    }
    ;

    //! Set route identifier
    /*!
     \param uiID route identifier
     */
    void SetID(unsigned int uiID)
    {
      m_uiID = uiID;
    }
    //! Get route identifier
    /*!
     \return route identifier
     */
    unsigned int GetID() const
    {
      return m_uiID;
    }
    //! Set route name
    /*!
     \param qsName route name
     */
    void SetName(const QString& qsName)
    {
      m_qsName = qsName;
    }
    //! Get route name
    /*!
     \return route name
     */
    const QString& GetName() const
    {
      return m_qsName;
    }
    //! Add route point to point list
    /*!
     \param pPoint pointer to route point
     */
    void AddRoutPoint(const QSharedPointer<CRoutePoint>& pPoint)
    {
      m_Points[pPoint->GetSN()] = pPoint;
    }
    //! Set map of route points
    /*!
     \param Points map of route points
     */
    void SetRoutPoints(const TPointMap& Points)
    {
      m_Points = Points;
    }
    //! Get route points
    /*!
     \return route points
     */
    const TPointMap& GetRoutPoints() const
    {
      return m_Points;
    }
    //! Get route points
    /*!
     \return route points
     */
    TPointMap& GetRoutPoints()
    {
      return m_Points;
    }
    //! Set identifier of the zone, whom belongs this route
    /*!
     \param uiID zone identifier
     */
    void SetZoneID(unsigned int uiID)
    {
      m_uiZoneID = uiID;
    }
    //! Get identifier of the zone, whom belongs this route
    /*!
     \return route identifier
     */
    unsigned int GetZoneID() const
    {
      return m_uiZoneID;
    }
    //! Get area code
    /*!
     \return area code
     */
    const QString& GetAreaCode() const
    {
      return m_sAreaCode;
    }
    //! Set area code
    /*!
     \param sAreaCode area code
     */
    void SetAreaCode(const QString& sAreaCode)
    {
      m_sAreaCode = sAreaCode;
    }

    //! Find a point in route point list
  /*!
    \param pPoint pointer to point
    \return iterator to route point map
  */
  TPointMap::iterator FindPoint(const QSharedPointer<CPoint>& pPoint);

  private:
    QString m_qsName;
    TPointMap m_Points;
    unsigned int m_uiZoneID;
    unsigned int m_uiID;
    QString m_sAreaCode;

};
#endif // ROUTE_H
