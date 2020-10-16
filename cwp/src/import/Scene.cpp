#include "Scene.h"
#include "Coordinate.h"
#include <math.h>
#include "Master.h"

Scene::Scene()
    : m_bIsMainFIR(false)
{
}

Scene::Scene(const Zone& zone) :
  Zone(zone), m_bIsMainFIR(false)
{
}

Scene::~Scene()
{

}

void Scene::AddWind(const QSharedPointer<core::CWind>& pWind)
{
  m_Wind.append(pWind);
}

QSharedPointer<CRoute> Scene::CreateRoute() const
{
  return QSharedPointer<CRoute> (new CRoute());
}

QSharedPointer<CAirport> Scene::CreateAirport() const
{
  return QSharedPointer<CAirport> (new CAirport());
}

QSharedPointer<CSSA> Scene::CreateSSA() const
{
  return QSharedPointer<CSSA> (new CSSA);
}

QSharedPointer<CRestriction> Scene::CreateRestriction() const
{
  QSharedPointer<CRestriction> pRestriction(new CRestriction());
  return pRestriction;
}

QSharedPointer<CRestrictionUser> Scene::CreateUserRestriction() const
{
  QSharedPointer<CRestrictionUser> pRestriction(new CRestrictionUser());
  return pRestriction;
}

QSharedPointer<CHolding> Scene::CreateHolding() const
{
  QSharedPointer<CHolding> pHolding(new CHolding);
  return pHolding;
}

QSharedPointer<CUserPics> Scene::CreateUserPics() const
{
  QSharedPointer<CUserPics> pUserPics(new CUserPics());
  return pUserPics;
}

QSharedPointer<SvgItem> Scene::CreateSvgItem(const QString& path) const
{
  QSharedPointer<SvgItem> pSvgItem(new SvgItem(path));
  return pSvgItem;
}

void Scene::AddBoundaryPoint(const QSharedPointer<CBoundaryPoint>& pPoint)
{
  if (m_pCentralPoint.isNull()) {
    m_pCentralPoint = QSharedPointer<CPoint> (new CPoint);
    m_pCentralPoint->SetName("Central point");
    if (pPoint->GetType() == CBoundaryPoint::enCircle)
      m_pCentralPoint->SetCoord(pPoint->GetArcCoord());
    else
      m_pCentralPoint->SetCoord(pPoint->GetCoord());
  }

  // Center
  if (pPoint->GetType() != CBoundaryPoint::enCircle
      && !pPoint->GetCoord().IsCenter()) {
    pPoint->GetCoord().Center(m_pCentralPoint->GetCoord());
  }
  if (pPoint->GetType() != CBoundaryPoint::enLine
      && !pPoint->GetArcCoord().IsCenter()) {
    pPoint->GetArcCoord().Center(m_pCentralPoint->GetCoord());
  }

  Zone::AddBoundaryPoint(pPoint);

  if (pPoint->GetCoord().GetX() < m_BoundMin.GetX())
    m_BoundMin.SetX(pPoint->GetCoord().GetX());
  if (pPoint->GetCoord().GetY() < m_BoundMin.GetY())
    m_BoundMin.SetY(pPoint->GetCoord().GetY());

  if (pPoint->GetCoord().GetX() > m_BoundMax.GetX())
    m_BoundMax.SetX(pPoint->GetCoord().GetX());
  if (pPoint->GetCoord().GetY() > m_BoundMax.GetY())
    m_BoundMax.SetY(pPoint->GetCoord().GetY());
}

bool Scene::AddPoint(const QSharedPointer<CPoint>& pPoint)
{
  if (m_pCentralPoint && !pPoint->GetCoord().IsCenter())
    pPoint->GetCoord().Center(m_pCentralPoint->GetCoord());

  CPointKey Key(pPoint->GetName(), pPoint->GetType(), pPoint->GetICAO(),
    pPoint->GetAirportName());
  if(m_Points.contains(Key))
  {
      LOG_STR(Log4Qt::Level::WARN_INT,
      QObject::tr(QString("Database error. Point key ('"+pPoint->GetName()+"', '"+pPoint->GetType()+"', '"+pPoint->GetICAO()+"', '"+pPoint->GetAirportName()+"')  dublicates!").toUtf8()));
      return false;
  }
  m_Points[Key] = pPoint;
  TSceneList::iterator itScan = m_Childs.begin();
  for (; itScan != m_Childs.end(); ++itScan) {
    if ((*itScan)->IsIncluded(pPoint->GetCoord()))
      (*itScan)->AddPoint(pPoint);
  }
  return true;
}

bool Scene::AddRoute(const QSharedPointer<CRoute>& pRoute)
{
    if(m_Routs.contains(pRoute->GetName()))
    {
        LOG_STR(Log4Qt::Level::WARN_INT, QObject::tr(QString("Database error. Route key ('"+pRoute->GetName()+"') dublicates!").toUtf8()));
        return false;
    }
  m_Routs[pRoute->GetName()] = pRoute;
  TSceneList::iterator itScan = m_Childs.begin();
  for (; itScan != m_Childs.end(); ++itScan) {
    int iPointCount(0);
    CRoute::TPointMap::iterator itPointScan = pRoute->GetRoutPoints().begin();
    for (; itPointScan != pRoute->GetRoutPoints().end(); ++itPointScan) {
      if ((*itScan)->GetPoints().find(CPointKey(
        (*itPointScan)->GetPoint()->GetName(),
        (*itPointScan)->GetPoint()->GetType(),
        (*itPointScan)->GetPoint()->GetICAO())) != (*itScan)->GetPoints().end()) {
        ++iPointCount;
      }
    }
    if (iPointCount > 1)
      (*itScan)->AddRoute(pRoute);
  }
  return true;
}

bool Scene::AddAirport(const QSharedPointer<CAirport>& pAirport)
{
  if (m_pCentralPoint && !pAirport->GetCoord().IsCenter())
    pAirport->GetCoord().Center(m_pCentralPoint->GetCoord());
  if (m_pCentralPoint) {
    CAirport::TRunways::const_iterator itRunwayScan =
        pAirport->GetRunways().begin();
    for (; itRunwayScan != pAirport->GetRunways().end(); ++itRunwayScan)
      if (!(*itRunwayScan)->GetCoord().IsCenter())
        (*itRunwayScan)->GetCoord().Center(m_pCentralPoint->GetCoord());
  }
  if(m_Airports.contains(CAirportKey(pAirport->GetName(), pAirport->GetICAO())))
  {
      LOG_STR(Log4Qt::Level::WARN_INT, QObject::tr(QString("Database error. Airport key ('"+pAirport->GetName()+"', '"+pAirport->GetICAO()+"') dublicates!").toUtf8()));
      return false;
  }
  m_Airports[CAirportKey(pAirport->GetName(), pAirport->GetICAO())] = pAirport;
  TSceneList::iterator itScan = m_Childs.begin();
  for (; itScan != m_Childs.end(); ++itScan) {
    if ((*itScan)->IsIncluded(pAirport->GetCoord()))
      (*itScan)->AddAirport(pAirport);
  }
  return true;
}

void Scene::AddRestriction(const QSharedPointer<CRestriction>& pRestriction)
{
  if (m_pCentralPoint) {
    CRestriction::TPointList::const_iterator itScan =
        pRestriction->GetPoints().begin();
    for (; itScan != pRestriction->GetPoints().end(); ++itScan) {
      if (!(*itScan)->GetCoord().IsNull() && !(*itScan)->GetCoord().IsCenter())
        (*itScan)->GetCoord().Center(m_pCentralPoint->GetCoord());
      if (!(*itScan)->GetArcCoord().IsNull()
          && !(*itScan)->GetArcCoord().IsCenter())
        (*itScan)->GetArcCoord().Center(m_pCentralPoint->GetCoord());
    }
  }

  m_Restriction.append(pRestriction);
  TSceneList::iterator itScan = m_Childs.begin();
  for (; itScan != m_Childs.end(); ++itScan) {
    if ((*itScan)->IsRestrictIncluded(*pRestriction))
      (*itScan)->AddRestriction(pRestriction);
  }
}

void Scene::AddUserRestriction(const QSharedPointer<CRestrictionUser> &restriction)
{
    if (m_pCentralPoint) {
        CRestrictionUser::TPointList::const_iterator itScan = restriction->GetPoints().begin();
        for (; itScan != restriction->GetPoints().end(); ++itScan) {
            if (!(*itScan)->GetCoord().IsNull() && !(*itScan)->GetCoord().IsCenter())
                (*itScan)->GetCoord().Center(m_pCentralPoint->GetCoord());
            if (!(*itScan)->GetArcCoord().IsNull() && !(*itScan)->GetArcCoord().IsCenter())
                (*itScan)->GetArcCoord().Center(m_pCentralPoint->GetCoord());
        }
    }

    m_RestrictionUser.append(restriction);
    TSceneList::iterator itScan = m_Childs.begin();
    for (; itScan != m_Childs.end(); ++itScan) {
        if ((*itScan)->IsRestrictUserIncluded(*restriction))
            (*itScan)->AddUserRestriction(restriction);
    }
}

void Scene::AddHolding(const QSharedPointer<CHolding>& pHolding)
{
  if (m_pCentralPoint && !pHolding->GetPoint()->GetCoord().IsCenter())
    pHolding->GetPoint()->GetCoord().Center(m_pCentralPoint->GetCoord());

  m_Holding.append(pHolding);
  TSceneList::iterator itScan = m_Childs.begin();
  for (; itScan != m_Childs.end(); ++itScan) {
    Scene::TPointsMap::iterator itPointFind((*itScan)->GetPoints().end());
    if (pHolding->GetPoint()->GetType() == CPoint::enAirportPoint
        || pHolding->GetPoint()->GetType() == CPoint::enAirportNDB) {
      itPointFind = (*itScan)->GetPoints().find(
        CPointKey(pHolding->GetPoint()->GetName(),
          pHolding->GetPoint()->GetType(), pHolding->GetPoint()->GetICAO(),
          pHolding->GetPoint()->GetAirportName()));
    } else
      itPointFind = (*itScan)->GetPoints().find(CPointKey(
        pHolding->GetPoint()->GetName(), pHolding->GetPoint()->GetType(),
        pHolding->GetPoint()->GetICAO()));
    if (itPointFind != (*itScan)->GetPoints().end())
      (*itScan)->AddHolding(pHolding);
  }
}

void Scene::AddSector(const QSharedPointer<Scene>& pSector)
{
  pSector->SetCentralPoint(GetCentralPoint());
  if (m_pCentralPoint) {
    Scene::TBoundary::iterator itScan = pSector->GetBoundary().begin();
    for (; itScan != pSector->GetBoundary().end(); ++itScan)
      if (!(*itScan)->GetCoord().IsCenter())
        (*itScan)->GetCoord().Center(m_pCentralPoint->GetCoord());
  }
  m_Childs.append(pSector);
}

void Scene::AddNote(const QSharedPointer<MapNote> &pNote)
{
    m_Notes.insert(pNote->id(), pNote);
    if (m_pCentralPoint && !pNote->coord().IsCenter())
        pNote->coord().Center(m_pCentralPoint->GetCoord());
}

void Scene::AddUserPics(const QSharedPointer<CUserPics>& pUserPics)
{
  if (m_pCentralPoint) {
    CUserPics::TPointList::const_iterator itScan =
        pUserPics->GetPoints().begin();
    for (; itScan != pUserPics->GetPoints().end(); ++itScan) {
      if (!(*itScan)->GetCoord().IsNull() && !(*itScan)->GetCoord().IsCenter())
        (*itScan)->GetCoord().Center(m_pCentralPoint->GetCoord());
      if (!(*itScan)->GetArcCoord().IsNull()
          && !(*itScan)->GetArcCoord().IsCenter())
        (*itScan)->GetArcCoord().Center(m_pCentralPoint->GetCoord());
    }
  }

  m_UserPics.append(pUserPics);

  /* ???
  TSceneList::iterator itScan = m_Childs.begin();
  for (; itScan != m_Childs.end(); ++itScan) {
    //if ((*itScan)->IsRestrictIncluded(*pRestriction))
      (*itScan)->AddUserPics(pUserPics);
  }
  */
}

void Scene::AddSvgItem(const QSharedPointer<SvgItem>& pSvgItem)
{
  if (m_pCentralPoint)
  {
      CCoord &start = pSvgItem->GetCoordStart();
      if (!start.IsNull() && !start.IsCenter())
      {
          start.Center(m_pCentralPoint->GetCoord());
      }
      CCoord &finish = pSvgItem->GetCoordFinish();
      if (!finish.IsNull() && !finish.IsCenter())
      {
          finish.Center(m_pCentralPoint->GetCoord());
      }
      QPointF first;
      first.setX(start.GetX());
      first.setY(start.GetY());
      QPointF second;
      second.setX(finish.GetX());
      second.setY(finish.GetY());
      qreal dif_x = second.x() - first.x();
      qreal dif_y = second.y() - first.y();
      qreal angle = atan(dif_y/dif_x)* (180./M_PI);
      pSvgItem->SetAngle(angle);
  }

  m_SvgItems.append(pSvgItem);
}

QSharedPointer<CRestrictionUser> Scene::DelUserRestriction(quint32 id)
{
    QSharedPointer<CRestrictionUser> restriction;

    typedef QSharedPointer<CRestrictionUser>    TRestrictionUser;
    for ( TRestrictionUserList::iterator it = m_RestrictionUser.begin();
          it != m_RestrictionUser.end(); ++it )
    {
        TRestrictionUser &rest = *it;
        if ( id == rest->GetID() )
        {
            restriction = rest;

            // Delete reference to restriction zone to children scene.
            for ( TSceneList::iterator itChild = m_Childs.begin();
                  itChild != m_Childs.end(); ++itChild )
            {
                (*itChild)->DelUserRestriction(id);
            }

            // Remove restriction zone reference.
            m_RestrictionUser.erase(it);
            break;
        }
    }

    return restriction;
}

QSharedPointer<CUserPics> Scene::DelUserPics(quint32 id)
{
    QSharedPointer<CUserPics> userpics;
    for(TUserPicsList::iterator itList = m_UserPics.begin(); itList != m_UserPics.end(); ++itList)
    {
        if(id == (*itList)->GetID())
        {
            userpics = *itList;
            m_UserPics.erase(itList);
            break;
        }
    }
    return userpics;
}

QSharedPointer<SvgItem> Scene::DelSVG(quint32 id)
{
    QSharedPointer<SvgItem> svgitem;
    for(TSvgItemList::iterator itList = m_SvgItems.begin(); itList != m_SvgItems.end(); ++itList)
    {
        if(id == (*itList)->GetSvgItemID())
        {
            svgitem = *itList;
            m_SvgItems.erase(itList);
            break;
        }
    }
    return svgitem;
}

void Scene::SetCentralPoint(const QSharedPointer<CPoint>& pCentral)
{
  m_pCentralPoint = pCentral;
}

const QSharedPointer<CPoint>& Scene::GetCentralPoint() const
{
  return m_pCentralPoint;
}

QSharedPointer<CPoint> Scene::CreatePoint(CPoint::Type type) const
{
  return QSharedPointer<CPoint> (new CPoint());
}

QSharedPointer<CBoundaryPoint> Scene::CreateBoundaryPoint() const
{
  return QSharedPointer<CBoundaryPoint> (new CBoundaryPoint());
}


QSharedPointer<CRunway> Scene::CreateRunway() const
{
  return QSharedPointer<CRunway> (new CRunway);

}

QSharedPointer<Scene> Scene::CreateSector() const
{
  return QSharedPointer<Scene> (new Scene);

}

QSharedPointer<MapNote> Scene::CreateNote() const
{
    return QSharedPointer<MapNote>(new MapNote);
}

void Scene::MarkItemAllActive(bool bActive)
{
  MarkItemActive(bActive);
  TPointsMap::iterator itPntScan = m_Points.begin();
  for (; itPntScan != m_Points.end(); ++itPntScan)
    (*itPntScan)->MarkItemActive(bActive);

  TRoutsMap::iterator itRouteScan = m_Routs.begin();
  for (; itRouteScan != m_Routs.end(); ++itRouteScan)
    (*itRouteScan)->MarkItemActive(bActive);

  TAirportsMap::iterator itAirportScan = m_Airports.begin();
  for (; itAirportScan != m_Airports.end(); ++itAirportScan)
    (*itAirportScan)->MarkItemActive(bActive);

  TRestrictionList::iterator itRestScan = m_Restriction.begin();
  for (; itRestScan != m_Restriction.end(); ++itRestScan)
    (*itRestScan)->MarkItemActive(bActive);

  THoldingList::iterator itHoldScan = m_Holding.begin();
  for (; itHoldScan != m_Holding.end(); ++itHoldScan)
    (*itHoldScan)->MarkItemActive(bActive);

  if (bActive == false) {
    TSceneList::iterator isSceneScan = m_Childs.begin();
    for (; isSceneScan != m_Childs.end(); ++isSceneScan)
      (*isSceneScan)->MarkItemActive(bActive);
  }
}

bool Scene::IsAirportExistAndSvgAdded(const QString& code, const QString& name, QSharedPointer<SvgItem>& ptr)
{
    QMap<CAirportKey, QSharedPointer<CAirport> >::Iterator air_iterator = m_Airports.begin();
    for(; air_iterator != m_Airports.end(); air_iterator++)
    {
        const CAirportKey& key = air_iterator.key();
        //qDebug() << key.GetName();
        if(key.GetName() == code)
        {
            CAirport* airport = air_iterator.value().data();
            if(airport)
            {
                if(airport->IsSvgItemExist(name))
                {
                    return true;
                }
                else
                {
                    airport->AppendSvgItem(name, ptr);
                    return false;
                }
            }
        }
    }
    return false;
}

bool Scene::IsIncluded(CCoord& Coord, const int iDeltaDistance/* = 6000*/)
{
    if ( !m_pCentralPoint ) return false;
  bool bInter(false);
  Coord.Center(m_pCentralPoint->GetCoord());
  // Intersection with segment
  TBoundary::const_iterator itTemp = m_Boundary.end();
  --itTemp;
  qint64 xold = (*itTemp)->GetCoord().GetX();
  qint64 yold = (*itTemp)->GetCoord().GetY();
  TBoundary::const_iterator itScan = m_Boundary.begin();
  for (; itScan != m_Boundary.end(); ++itScan) {
    qint64 xnew = (*itScan)->GetCoord().GetX();
    qint64 ynew = (*itScan)->GetCoord().GetY();

    // Check vertical edges & peaks
    /*if (Coord.GetX() == xold)
     {
     if (Coord.GetY == yold)
     return true;
     else if (xold == xnew &&
     ((Coord.GetY() >= yold && Coord.GetY() <= ynew ) ||
     (Coord.GetY() >= ynew && Coord.GetY() <= yold )))
     return true;
     }*/

    qint64 x1, x2, y1, y2;
    if (xnew > xold) {
      x1 = xold;
      x2 = xnew;
      y1 = yold;
      y2 = ynew;
    } else {
      x1 = xnew;
      x2 = xold;
      y1 = ynew;
      y2 = yold;
    }

    // Delta
    // Coordinate rotation xn = x*cos(a)+y*sin(a); yn = -x*sin(a)+y*cos(a);
    qint64 DeltaX = x2 - x1;
    qint64 DeltaY = y2 - y1;
    qint64 LineX = pow(DeltaX * DeltaX + DeltaY * DeltaY, 0.5);
    if (LineX != 0) {
      qint64 PointX = ((Coord.GetX() - x1) * DeltaX + (Coord.GetY() - y1)
          * DeltaY) / LineX;
      if (PointX >= -iDeltaDistance && PointX <= LineX + iDeltaDistance) {
        qint64 PointY = ((x1 - Coord.GetX()) * DeltaY + (Coord.GetY() - y1)
            * DeltaX) / LineX;
        if (PointY >= -iDeltaDistance && PointY <= iDeltaDistance)
          return true;
      }
    }

    if ((xnew < Coord.GetX()) == (Coord.GetX() <= xold) && (Coord.GetY() - y1)
        * (x2 - x1) <= (y2 - y1) * (Coord.GetX() - x1))
      bInter = !bInter;
    xold = xnew;
    yold = ynew;
  }
  return bInter;
}

bool Scene::IsSquareIncluded(CCoord& Coord, int iDelta)
{
    if ( !m_pCentralPoint ) return false;
  Coord.Center(m_pCentralPoint->GetCoord());
  if (Coord.GetX() >= m_BoundMin.GetX() - iDelta && Coord.GetX()
      <= m_BoundMax.GetX() + iDelta && Coord.GetY() >= m_BoundMin.GetY()
      - iDelta && Coord.GetY() <= m_BoundMax.GetY() + iDelta)
    return true;
  return false;
}

bool Scene::IsRestrictIncluded(CRestriction& Restriction)
{
  CRestriction::TPointList::const_iterator itScan =
      Restriction.GetPoints().begin();
  for (; itScan != Restriction.GetPoints().end(); ++itScan) {
    if ((*itScan)->GetType() != CRestrictPoint::enCircle && IsIncluded(
      (*itScan)->GetCoord()))
      return true;
    if ((*itScan)->GetType() == CRestrictPoint::enCircle && IsIncluded(
      (*itScan)->GetArcCoord()))
      return true;
  }
  return false;
}

bool Scene::IsRestrictUserIncluded(CRestrictionUser& Restriction)
{
  CRestrictionUser::TPointList::const_iterator itScan =
      Restriction.GetPoints().begin();
  for (; itScan != Restriction.GetPoints().end(); ++itScan) {
    if ((*itScan)->GetType() != CRestrictPoint::enCircle && IsIncluded(
      (*itScan)->GetCoord()))
      return true;
    if ((*itScan)->GetType() == CRestrictPoint::enCircle && IsIncluded(
      (*itScan)->GetArcCoord()))
      return true;
  }
  return false;
}

Scene::TSceneList& Scene::GetSectorList()
{
  return m_Childs;
}

Scene::TScenePtr Scene::FindSector(uint uiSectorID)
{
  TScenePtr pResult;
  TSceneList::iterator itFind = m_Childs.begin();
  for (; itFind != m_Childs.end(); ++itFind) {
    if ((*itFind)->GetID() == uiSectorID) {
      pResult = *itFind;
      break;
    }
  }
  return pResult;
}

Scene::TSceneList Scene::FindSectorsByPoint(QString sPointName, double dAltitudeMeters/* = 0*/, bool bInside/* = false*/)
{
    TSceneList pResult;
    QSharedPointer<CPoint> pPoint = GetPoints().value(CPointKey(sPointName), QSharedPointer<CPoint>());
    if(!pPoint) return pResult;
    TSceneList::iterator itSector = m_Childs.begin();
    for(; itSector != m_Childs.end(); ++itSector)
    {
        if(dAltitudeMeters)
        {
            if((*itSector)->GetLowLimit() - dAltitudeMeters <= 152  &&
               (dAltitudeMeters - (*itSector)->GetUpLimit() <= 152 || (*itSector)->GetUpLimit() == 0))
            {
                if((*itSector)->GetPoints().values().contains(pPoint))
                    pResult.append(*itSector);
            }
        }
        else
        {
            if((*itSector)->GetPoints().values().contains(pPoint))
                pResult.append(*itSector);
        }
    }
    return pResult;
}
