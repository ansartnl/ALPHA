#include "View.h"
#include <time.h>
#include "cs/CoordSystemGK.h"
#include "cs/CoordSystemStereo.h"

#include <QDebug>


namespace geoGE {

View::View()
{
    m_pCS = TCoordSystem(new CCoordSystemStereo());
}

View::~View()
{
}

void View::AddScene(const QString& sMapName, const TScenePtr& pScene)
{
    RemoveScene(sMapName);
	m_SceneList[sMapName] = pScene;
}

void View::RemoveScene(const QString& sKey)
{
    TSceneList::iterator it = m_SceneList.find(sKey);
    if ( it != m_SceneList.end() )
        m_SceneList.erase(it);
}

View::TSceneList& View::GetScenes()
{
	return m_SceneList;
}

QSharedPointer<MeteoVector> View::CreateMeteoVec() const
{
    return QSharedPointer<MeteoVector>(new MeteoVector);
}

QSharedPointer<MeteoContour> View::CreateMeteoZone() const
{
    return QSharedPointer<MeteoContour>(new MeteoContour);
}

QSharedPointer<Peleng> View::CreatePeleng() const
{
    return QSharedPointer<Peleng>(new Peleng);
}

void View::UpdateMeteo(const MeteoVector& meteoVector)
{
    QSharedPointer<MeteoVector> pMeteoVec;
    // Find meteo in the already existed list
    TMeteoVecMap::iterator it = mMeteoVectors.find(meteoVector.GetID());
    if ( it != mMeteoVectors.end() )
        pMeteoVec = *it;

    bool isNewMeteo = false;
    // If nothing found - create new meteo vector list
    if ( !pMeteoVec )
    {
        pMeteoVec = QSharedPointer<MeteoVector>(CreateMeteoVec());
        pMeteoVec->SetID(meteoVector.GetID());
        isNewMeteo = true;
    }

    pMeteoVec->SetIntencity(meteoVector.GetIntencity());

    // Update vector coordinates
    pMeteoVec->ClearVectors();
    for ( MeteoVector::TVectors::const_iterator itc = meteoVector.GetVectors().constBegin();
        itc != meteoVector.GetVectors().constEnd(); ++itc )
    {
        CCoord coordS = itc->first,
            coordE = itc->second;

        m_pCS->toModel( coordS );
        m_pCS->toModel( coordE );

        pMeteoVec->AddVector(MeteoVector::TVector(coordS, coordE));
    }

    if ( isNewMeteo && pMeteoVec )
        AddMeteo(pMeteoVec);
}

void View::UpdateMeteo(const MeteoContour& meteoZone)
{
    QSharedPointer<MeteoContour> pMeteoZone;
    // Find meteo in the already existed list
    TMeteoZoneMap::iterator it = mMeteoZones.find(meteoZone.GetID());
    if ( it != mMeteoZones.end() )
        pMeteoZone = *it;

    bool isNewMeteo = false;
    // If nothing found - create new meteo zone
    if ( !pMeteoZone )
    {
        pMeteoZone = QSharedPointer<MeteoContour>(CreateMeteoZone());
        pMeteoZone->SetID(meteoZone.GetID());
        isNewMeteo = true;
    }

    pMeteoZone->SetIntencity(meteoZone.GetIntencity());

    // Update vector coordinates
    pMeteoZone->ClearContour();
    for ( MeteoContour::TPoints::const_iterator itc = meteoZone.GetContour().constBegin();
        itc != meteoZone.GetContour().constEnd(); ++itc )
    {
        CCoord coord = *itc;

        m_pCS->toModel( coord );

        pMeteoZone->AddPoint(coord);
    }

    if ( isNewMeteo && pMeteoZone )
        AddMeteo(pMeteoZone);
}

void View::AddMeteo(const QSharedPointer<MeteoVector>& pMeteoVec)
{
    mMeteoVectors.insert(pMeteoVec->GetID(), pMeteoVec);
}

void View::AddMeteo(const QSharedPointer<MeteoContour>& pMeteoZone)
{
    mMeteoZones.insert(pMeteoZone->GetID(), pMeteoZone);
}

void View::UpdatePeleng(const Peleng& peleng)
{
    QSharedPointer<Peleng> pPeleng;
    // Find peleng in the already existed list
    TPelengMap::iterator it = mPelengs.find(peleng.GetID());
    if ( it != mPelengs.end() )
        pPeleng = *it;

    bool isNewPeleng = false;
    // If nothing found - create new peleng object
    if ( !pPeleng )
    {
        pPeleng = QSharedPointer<Peleng>(CreatePeleng());
        pPeleng->SetID(peleng.GetID());
        isNewPeleng = true;
    }


    CCoord coordTemp = peleng.GetCoord();
    m_pCS->toModel( coordTemp );

    pPeleng->SetCoord( coordTemp );
    pPeleng->SetAngle(peleng.GetAngle());
    pPeleng->SetChannelNumber(peleng.GetChannelNumber());

    if ( isNewPeleng && pPeleng )
        AddPeleng(pPeleng);
}

void View::AddPeleng(const QSharedPointer<Peleng>& pPeleng)
{
    mPelengs.insert(pPeleng->GetID(), pPeleng);
}

void View::DefineCenterPoint(qreal latitude, qreal longitude, qreal angle)
{
    // Set center point depending what coordinate system is used.
    CCoordSystemStereo* pCSStereo = dynamic_cast<CCoordSystemStereo*>( m_pCS.data() );
    if ( pCSStereo )
        pCSStereo->defineCenterPoint(latitude, longitude);

    else
    {
        CCoordSystemGK* pCSGK = dynamic_cast<CCoordSystemGK*>( m_pCS.data() );
        if ( pCSGK )
            pCSGK->defineCenterPoint(latitude, longitude);

//        CCoordSystemProj* pCSProj = dynamic_cast<CCoordSystemProj*>( m_pCS.data() );
//        if ( pCSProj )
//            pCSProj->defineCenterPoint(latitude, longitude);
    }

    m_pCS->defineProjectionRotation(angle);
}

void View::SetCenterPoint(qreal latitude, qreal longitude, qreal angle)
{
    // Set center point depending what coordinate system is used.
    DefineCenterPoint(latitude, longitude, angle);

    // Recalculate model coordinates if coordinate system is binded to the center point
    // (performance reason).
    if ( !m_pCS->isModelCentered() )
        return;

    UpdateModelCoords();
}

void View::UpdateModelCoords()
{
    for ( TSceneList::iterator it = m_SceneList.begin(); it != m_SceneList.end(); ++it )
    {
        (*it)->UpdateModelCoords(m_pCS);
    }

    // Update meteo coordinates
    for ( TMeteoVecMap::iterator itMeteo = mMeteoVectors.begin();
        itMeteo != mMeteoVectors.end(); ++itMeteo )
    {
        QSharedPointer<MeteoVector> meteoVector = *itMeteo;
        if ( meteoVector )
        {
            for ( MeteoVector::TVectors::iterator it = meteoVector->GetVectors().begin();
                it != meteoVector->GetVectors().end(); ++it )
            {
                MeteoVector::TVector& vec = *it;
                m_pCS->toModel( vec.first );
                m_pCS->toModel( vec.second );
            }
        }
    }

    // Update meteo zone coordinates
    for ( TMeteoZoneMap::iterator itMeteo = mMeteoZones.begin();
        itMeteo != mMeteoZones.end(); ++itMeteo )
    {
        QSharedPointer<MeteoContour> meteoZone = *itMeteo;
        if ( meteoZone )
        {
            for ( MeteoContour::TPoints::iterator it = meteoZone->GetContour().begin();
                it != meteoZone->GetContour().end(); ++it )
                m_pCS->toModel( *it );
        }
    }
}

void View::UpdateProjection(const QString &nameProj, qreal a, qreal e, qreal k0)
{
    // Do not update projection if it is identical to current.
    if ( !DefineProjection(nameProj, a, e, k0) )
        return;

    // Recreate object coordinates.
    SetCenterPoint(m_pCS->getCenterLatitude(), m_pCS->getCenterLongitude(),
                   m_pCS->getProjectionRotation());
}

bool View::DefineProjection(const QString &nameProj, qreal a, qreal e, qreal k0)
{
    Q_ASSERT(m_pCS);

    if ( nameProj == m_pCS->GetProjectionName() )
    {
        CCoordSystemStereo *csStereo = dynamic_cast<CCoordSystemStereo *>(m_pCS.data());
        if ( csStereo )
        {
            qreal ac, ec, k0c;
            csStereo->getCoefficients(ac, ec, k0c);
            // Parameters of current coordinate system is identical to new - do nothing - quit.
            if ( qFuzzyCompare(a, ac) && qFuzzyCompare(e, ec) && qFuzzyCompare(k0, k0c) )
                return false;

            csStereo->setCoefficients(a, e, k0);
        }
    }
    // Create new projection and apply new model parameters.
    else
    {
        // TODO: new projection is created here ...
    }

    return true;
}

void View::Clear()
{
    m_SceneList.clear();
    mMeteoZones.clear();
    mMeteoVectors.clear();
}

}   // namespace geoGE
