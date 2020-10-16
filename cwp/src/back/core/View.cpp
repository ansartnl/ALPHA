#include "View.h"

#include "main.h"
#include "XtGlobal.h"
#include "Master.h"

CView::CView()
    : m_pMaster(0)
{
    //qsrand(QDateTime::currentMSecsSinceEpoch());
}

CView::~CView()
{

}

void CView::AddScene(const QString& sMapName, const TScenePtr& pscene)
{
    if (m_SceneList.empty())
    {
        m_pViewCentralPoint = pscene->GetCentralPoint();
        m_MainFIR = pscene;
    }

    m_SceneList[sMapName] = pscene;
}

CView::TSceneList& CView::GetScenes()
{
    return m_SceneList;
}

QSharedPointer<CAirplane> CView::CreateAirplane() const
{
    return QSharedPointer<CAirplane>(new CAirplane());
}

QSharedPointer<core::CMeteo> CView::CreateMeteo() const
{
    return QSharedPointer<core::CMeteo>(new core::CMeteo);
}

void CView::AddAirplane(const QSharedPointer<CAirplane> &pAirplane)
{
    m_Airplanes[pAirplane->GetID()] = pAirplane;
    QDateTime dt;
    dao()->GetSysdate(dt);
    pAirplane->SetStartTime(dt);
    if (m_pViewCentralPoint && !pAirplane->GetCoord().IsCenter())
        pAirplane->GetCoord().Center(m_pViewCentralPoint->GetCoord());
    pAirplane->SetMaster(m_pMaster);
}

void CView::DelAirplane(QSharedPointer<CAirplane> pAirplane, bool bSend /* = true*/)
{
    Q_UNUSED(bSend)
    m_Airplanes.remove(pAirplane->GetID());
    m_pMaster->removeFormular(pAirplane->GetID());
}

void CView::UpdateAirplane(CAirplaneState &AirplaneState)
{
    if (AirplaneState.GetTrackNumber() == CAirplaneState::kClearAllTracks)
    {
        // Clear
        foreach (const QSharedPointer<CAirplane> &a, m_Airplanes)
            DelAirplane(a);
        return;
    }

    CCoord CoordTemp(AirplaneState.GetCoordinate());
    CoordTemp.Center(m_pViewCentralPoint->GetCoord());
    //if (IsSquareIncluded(CoordTemp, 100000))
    //{
        QSharedPointer<CAirplane> pAirplane;
        TAirplanesMap::iterator itFind = m_Airplanes.find(AirplaneState.GetTrackNumber());
        if (itFind == m_Airplanes.end())
        {
            // Create
            pAirplane = QSharedPointer<CAirplane>(CreateAirplane());
            pAirplane->SetID(AirplaneState.GetTrackNumber());
            pAirplane->SetOperName(QObject::tr("NS"));
            pAirplane->SetMode3AReplyInit(AirplaneState.GetMode3AReply());
        }
        else
        {
            // Update
            pAirplane = *itFind;
            if ((AirplaneState.GetTrackStatus() == CAirplaneState::BeginTrack) || (AirplaneState.GetTrackStatus() == CAirplaneState::EndTrack))
            {
                //STCA (podpora)
                DelAirplane(*itFind);
                itFind = m_Airplanes.end();
                if (AirplaneState.GetTrackStatus() == CAirplaneState::EndTrack)
                    return;

                pAirplane = QSharedPointer<CAirplane>(CreateAirplane());
                pAirplane->SetID(AirplaneState.GetTrackNumber());
                pAirplane->SetOperName(QObject::tr("NS"));
                pAirplane->SetMode3AReplyInit(AirplaneState.GetMode3AReply());
            }
        }

        pAirplane->setIsUvd(AirplaneState.isUvd());
        pAirplane->setLastEmg(AirplaneState.lastEmg());
        pAirplane->setCfl(AirplaneState.cfl());
        pAirplane->setSfl(AirplaneState.sfl());
        pAirplane->setHeading(AirplaneState.heading());
        pAirplane->setVerticalRate(AirplaneState.verticalRate());
        pAirplane->setTrueAirspeed(AirplaneState.trueAirspeed());
        pAirplane->setGroundAirspeed(AirplaneState.groundAirspeed());
        pAirplane->setIndicatedAirspeed(AirplaneState.indicatedAirspeed());
        pAirplane->setIndicatedAirspeedMach(AirplaneState.indicatedAirspeedMach());

        checkDblCode(AirplaneState, pAirplane);

        pAirplane->SetPlaneSPI(AirplaneState.GetSPI());
        pAirplane->SetMode3AReply(AirplaneState.GetMode3AReply());
        pAirplane->SetACID(AirplaneState.GetCallsign().trimmed());

        /*if (pAirplane->dbl()) {
            CAngle lat = CoordTemp.GetLatitude();
            lat.SetDegrees(35);
            CoordTemp.SetLatitude(lat);
            CoordTemp.Center(m_pViewCentralPoint->GetCoord());
            pAirplane->SetCoord(CoordTemp);
        }*/
        pAirplane->SetCoord(CoordTemp);

        pAirplane->SetFL(AirplaneState.GetFlightLevel());
        pAirplane->SetVelocity(AirplaneState.GetVelocity());
        pAirplane->SetVz(AirplaneState.GetZVelocity());
        pAirplane->SetTrackNumberUvd(AirplaneState.GetTrackNumberUvd());
        if (AirplaneState.IsHeightQNH())
            pAirplane->SetHeightQNH(AirplaneState.GetHeightQNH());
        else
            pAirplane->MarkHeightQNH(false);

        if (AirplaneState.IsBPS())
            pAirplane->SetBPS(AirplaneState.GetBPS());
        else
            pAirplane->MarkBPS(false);

        if (AirplaneState.IsACAS())
            pAirplane->SetACAS();

        pAirplane->setUpdated(true);
        if (pAirplane->GetSFPL() && pAirplane->GetSFPL()->GetFPLFrag())
        {
            int nextPoint = pAirplane->GetSFPL()->GetFPLFrag()->GetNextPoint();
            pAirplane->GetSFPL()->GetFPLFrag()->Update(pAirplane->GetCoord(), pAirplane->GetVelocity().GetV());
            if (pAirplane->GetSFPL()->GetFPLFrag()->GetNextPoint() != nextPoint)
            {
                if (master()->getRedisDB())
                    master()->getRedisDB()->saveSFPL(pAirplane->GetSFPL());
            }

            QStringList sectors = pAirplane->GetSFPL()->GetAllSectors().split("-");
            if (sectors.size() > 1)
            {
                const QString nextSector = sectors.at(1);

                for (auto itFIR = GetScenes().constBegin(); itFIR != GetScenes().constEnd(); ++itFIR)
                {
                    auto itScene = (*itFIR)->GetSectorList().constBegin();
                    for (; itScene != (*itFIR)->GetSectorList().constEnd(); ++itScene)
                    {
                        if ((*itScene)->GetShortName() == nextSector)
                        {
                            int lowLimit = (*itScene)->GetLowLimit();
                            int upLimit = (*itScene)->GetUpLimit();

                            // check if airplane crossed sectors border
                            double altitide = pAirplane->GetFLinMeters();
                            if ((altitide >= lowLimit && (altitide <= upLimit || upLimit == 0)))
                            {
                                if ((*itScene)->IsIncluded(pAirplane->GetCoord(), 1))
                                {
                                    pAirplane->CalculateSectors(pAirplane->GetSFPL());
                                    logOperation(QObject::tr("%1: sector changed, new sectors list: %2")
                                                 .arg(pAirplane->GetSFPL()->GetACID())
                                                 .arg(pAirplane->GetSFPL()->GetAllSectors()));

                                    if (master()->getRedisDB()) {
                                        master()->getRedisDB()->saveSFPL(pAirplane->GetSFPL());
                                    }
                                }
                            }
                            
                            // check autotransfer
                            if (pAirplane->GetPlaneType() == CAirplaneData::ASSUME)
                            {
                                const QStringList cs = networkSettings()->value("users/current_sectors_short_name").toString().split("-", QString::SkipEmptyParts);
                                if (!cs.contains(nextSector))
                                {
                                    int secs = networkSettings()->value("airplane/auto_transfer_time", 30).toInt();

                                    CCoord coord;
                                    coord.SetX(pAirplane->GetCoord().GetX() + secs * pAirplane->GetVelocity().GetVx());
                                    coord.SetY(pAirplane->GetCoord().GetY() + secs * pAirplane->GetVelocity().GetVy());
                                    coord.DeCenter(m_pViewCentralPoint->GetCoord());

                                    if ((*itScene)->IsIncluded(coord, 1))
                                    {
                                        pAirplane->transfer(nextSector);
                                    }
                                }
                            }

                            break;
                        }
                    }
                }
            }

            if ((!pAirplane->GetSFPL()->GetIsCOPin() && !pAirplane->GetSFPL()->GetCOPin().isEmpty())
                || (!pAirplane->GetSFPL()->GetIsCOPout() && !pAirplane->GetSFPL()->GetCOPout().isEmpty()))
            {
                CProcFrag::TPointFragList listPoints = pAirplane->GetSFPL()->GetFPLFrag()->GetAllPoint();
                CProcFrag::TPointFragList::const_iterator itPnt =
                        listPoints.constBegin();
                for (; itPnt != listPoints.constEnd(); ++itPnt)
                {
                    if ((*itPnt)->IsPassed())
                    {
                        if (!pAirplane->GetSFPL()->GetIsCOPin() && !pAirplane->GetSFPL()->GetCOPin().isEmpty())
                        {
                            if ((*itPnt == pAirplane->GetSFPL()->GetCOPinPoint())
                                || ((*itPnt)->GetPoint()->GetName().contains(pAirplane->GetSFPL()->GetCOPin())))
                            {
                                pAirplane->GetSFPL()->SetIsCOPin(true);
                                pAirplane->GetSFPL()->SetETOin((*itPnt)->GetPassedTime().time());
                            }
                        }
                        if (!pAirplane->GetSFPL()->GetIsCOPout() && !pAirplane->GetSFPL()->GetCOPout().isEmpty())
                        {
                            if ((*itPnt == pAirplane->GetSFPL()->GetCOPoutPoint())
                                || ((*itPnt)->GetPoint()->GetName().contains(pAirplane->GetSFPL()->GetCOPout())))
                            {
                                pAirplane->GetSFPL()->SetIsCOPout(true);
                                pAirplane->GetSFPL()->SetETOout((*itPnt)->GetPassedTime().time());
                            }
                        }
                    }
                }
            }
        }
        if (itFind == m_Airplanes.end())
            AddAirplane(pAirplane);

        m_pMaster->addFormular(pAirplane->GetID());
}

void CView::AddMeteo(const QSharedPointer<core::CMeteo>& pMeteo)
{
    m_Meteo.append(pMeteo);
    if (m_pViewCentralPoint)
    {
        core::CMeteo::TPointList::const_iterator itScan = pMeteo->GetPoints().begin();
        for(; itScan != pMeteo->GetPoints().end(); ++itScan)
        {
            if (!(*itScan)->GetCoord().IsNull() && !(*itScan)->GetCoord().IsCenter())
                (*itScan)->GetCoord().Center(m_pViewCentralPoint->GetCoord());
        }
    }
}

void CView::DelMeteo(uint uiZoneID)
{
    TMeteoList::iterator itMeteo = GetMeteo().begin();
    while(itMeteo != GetMeteo().end())
    {
        if(uiZoneID == (*itMeteo)->GetZoneID())
            itMeteo = GetMeteo().erase(itMeteo);
        else
            ++itMeteo;
    }
}

void CView::SetMaster(CMaster* pMaster)
{
    m_pMaster = pMaster;
    m_pMaster->SetPlanes(&m_Airplanes);
}

CView::TScenePtr CView::GetMainFIR() const
{
    return m_MainFIR;
}

void CView::OnObjectReceived(const Xt::TObject& obj)
{
    static const qreal gMetersToFL = 3.2808399 / 100.;

    // Received track mark.
    if ( obj->GetType() == Xt::Track::GetClassType() )
    {
        Xt::Track *track = static_cast<Xt::Track *>( obj.data() );

        if ( track->mLocation.mType != Xt::Coord::EGeo )
        {
            LOG_STR(Log4Qt::Level::WARN_INT,
                    QObject::tr("Received non geodesic coordinate. Skip this track. Coord type: '%1'")
                    .arg(track->mLocation.mType));
            return;
        }

        const Xt::Coord::Geo &geoCoord = track->mLocation.geo();

        CAirplaneState airplane;
        airplane.SetTrackNumber( track->mNumber );
        airplane.SetCoordinate( CCoord(geoCoord.longitude,
                                       geoCoord.latitude) );
        airplane.SetFlightLevel( track->mLocation.mHeight * gMetersToFL );
        airplane.SetVelocity( CVelocity(track->mVelocity.mX,
                                        track->mVelocity.mY) );
        airplane.SetMode3AReply(0);

        quint16 sacsic = track->mRlsId;
        airplane.SetSAC( sacsic >> 8 );
        airplane.SetSIC( sacsic & 0x00FF );

        UpdateAirplane( airplane );
    }
    // Received plot mark.
    else if ( obj->GetType() == Xt::Plot::GetClassType() )
    {
        Xt::Plot *plot = static_cast<Xt::Plot *>( obj.data() );

        if ( plot->mLocation.mType != Xt::Coord::EGeo )
        {
            LOG_STR(Log4Qt::Level::WARN_INT,
                    QObject::tr("Received non geodesic coordinate. Skip this plot. Coord type: '%1'")
                    .arg(plot->mLocation.mType));
            return;
        }

        const Xt::Coord::Geo &geoCoord = plot->mLocation.geo();

        CAirplaneState airplane;
        airplane.SetTrackNumber( plot->mNumber );
        airplane.SetCoordinate( CCoord(geoCoord.longitude,
                                       geoCoord.latitude) );
        airplane.SetFlightLevel( plot->mLocation.mHeight * gMetersToFL );
        airplane.SetVelocity( CVelocity(0,0) );
        airplane.SetMode3AReply(0);

        quint16 sacsic = plot->mRlsId;
        airplane.SetSAC( sacsic >> 8 );
        airplane.SetSIC( sacsic & 0x00FF );

        UpdateAirplane( airplane );
    }
    // Received meteo contour.
    else if ( obj->GetType() == Xt::MeteoContour::GetClassType() )
    {
        Xt::MeteoContour *meteo = static_cast<Xt::MeteoContour *>( obj.data() );
        if(meteo->mDBID != dao()->GetDBID())
        {
            if(GetMainFIR())
            {
                QSharedPointer<core::CMeteo> pMeteo(CreateMeteo());
                pMeteo->SetZoneID(GetMainFIR()->GetID());
                pMeteo->SetName(meteo->mName);
                pMeteo->SetType(core::CMeteo::Type(meteo->mIntencity));
                for(int i = 0; i < meteo->mContour.size(); ++i)
                {
                    QSharedPointer<core::CMeteoPoint> pPoint(new core::CMeteoPoint);
                    pPoint->SetSN((i+1)*10);
                    pPoint->SetCoord(CCoord(meteo->mContour[i].y(), meteo->mContour[i].x()));
                    pMeteo->AddPoint(pPoint);
                }
                // Save in DB
                TVA_CATCHALL_TRY
                {
                    dao()->InsertMeteo(pMeteo);
                }TVA_CATCHALL(err){
                    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

                AddMeteo(pMeteo);
            }
        }
    }
}

Scene::TSceneList CView::FindSectorsByPoint(const QString &point_name, double dAltitudeMeters/* = 0*/, bool bInside/* = false*/)
{
    Scene::TSceneList sectors;
    foreach (const TScenePtr &fir, GetScenes()) {
        sectors += fir->FindSectorsByPoint(point_name, dAltitudeMeters, bInside);
    }
    return sectors;
}

Scene::TSceneList CView::FindSectorsByPoint(const QStringList &fir_names_list, const QString &point_name, double dAltitudeMeters/* = 0*/, bool bInside/* = false*/)
{
    Scene::TSceneList sectors;
    foreach (const TScenePtr &fir, GetScenes()) {
        if (fir_names_list.contains(fir->GetIDName()))
            sectors += fir->FindSectorsByPoint(point_name, dAltitudeMeters, bInside);
    }
    return sectors;
}

void CView::SetFormularDefaultPos()
{
    TAirplanesMap::iterator itAirplans = GetAirplanes().begin();
    for (; itAirplans != GetAirplanes().end(); ++itAirplans)
        (*itAirplans)->SetFormularDefaultPos();
}

void CView::setFormularsPos(qreal angle, int length)
{
    foreach (const QSharedPointer<CAirplane> &airplane, m_Airplanes.values()) {
        if (!airplane->fixedFormular())
            airplane->setFormularPos(angle, length);
    }
}

void CView::checkDblCode(CAirplaneState &airplane_state, QSharedPointer<CAirplane> &airplane)
{
    const quint16 mode3a = airplane_state.GetMode3AReply();
    const quint16 track_num = airplane_state.GetTrackNumber();

    static const quint16 invalid_mode3a = 0;
    if (invalid_mode3a == mode3a)
        return;

    if (mode3a_to_track_num_.end() == mode3a_to_track_num_.find(mode3a, track_num))
        mode3a_to_track_num_.insert(mode3a, track_num);

    TrackNumToMode3A::iterator it_tn_to_m3a = track_num_to_mode3a_.find(track_num);
    if (track_num_to_mode3a_.end() == it_tn_to_m3a) {
        track_num_to_mode3a_.insert(track_num, mode3a);
    } else {
        const quint16 old_mode3a = *it_tn_to_m3a;
        if (mode3a != old_mode3a) {
            airplane->setDbl(false);

            mode3a_to_track_num_.remove(old_mode3a, track_num);
            if (mode3a_to_track_num_.count(old_mode3a) < 2)
                updateDblFlag(old_mode3a, false/*, QColor()*/);

            track_num_to_mode3a_.erase(it_tn_to_m3a);
            track_num_to_mode3a_.insert(track_num, mode3a);
        }
    }

    Mode3AToTrackInfo::iterator it = mode3a_to_track_info_.find(mode3a);
    if (it == mode3a_to_track_info_.end()) {
        TrackInfo ti;
        ti.dbl = false;
        ti.appearance_time = QDateTime::currentDateTime();
        ti.counter = 0;
        ti.track_num = track_num;
        mode3a_to_track_info_.insert(mode3a, ti);

        airplane->setDbl(false);
    } else {
        TrackInfo ti = *it;

        if (ti.dbl) {
            const int time_diff = ti.appearance_time.secsTo(QDateTime::currentDateTime());
            if (time_diff > 4) {
                ti.dbl = false;
                ti.appearance_time = QDateTime::currentDateTime();

                updateDblFlag(mode3a, false/*, QColor()*/);
            } else {
                if (track_num != ti.track_num)
                    ti.appearance_time = QDateTime::currentDateTime();

                airplane->setDbl(true);
            }

            ti.counter = 0;
            ti.track_num = track_num;
        }

        if (track_num != ti.track_num) {
            ++ti.counter;
            ti.track_num = track_num;

            if (3 == ti.counter) {
                const int diff = ti.appearance_time.secsTo(QDateTime::currentDateTime());
                if (diff < 8) {
                    /*QColor color(qrand() % 255, qrand() % 255, qrand() % 255);
                    Mode3AToDblColor::iterator it = mode3a_to_dbl_color_.find(mode3a);
                    if (it == mode3a_to_dbl_color_.end())
                        mode3a_to_dbl_color_.insert(mode3a, color);
                    else
                        color = *mode3a_to_dbl_color_.find(mode3a);*/

                    ti.dbl = true;

                    updateDblFlag(mode3a, true/*, color*/);
                }

                ti.counter = 0;
                ti.appearance_time = QDateTime::currentDateTime();
            }
        }

        mode3a_to_track_info_.erase(it);
        mode3a_to_track_info_.insert(mode3a, ti);
    }
}

void CView::updateDblFlag(quint16 mode3a, bool dbl/*, const QColor &color*/)
{
    foreach (quint16 track_num, mode3a_to_track_num_.values(mode3a)) {
        TAirplanesMap::iterator it = m_Airplanes.find(track_num);
        if (it != m_Airplanes.end()) {
            (*it)->setDbl(dbl);
            //(*it)->setDblColor(color);
        }
    }
}

void CView::assumeAirplanes(const QString &current_sector)
{
    foreach (const QSharedPointer<CAirplane> &airplane, m_Airplanes) {
        const QSharedPointer<CSFPL> &sfpl = airplane->GetSFPL();
        const QStringList controlled_sectors = networkSettings()->value("users/current_sectors_short_name").toString().split("-", QString::SkipEmptyParts);
        //if (sfpl && (sfpl->GetCurSector() == current_sector)) {
        if (sfpl && (controlled_sectors.contains(sfpl->GetCurSector()))) {
            if (CAirplaneData::ACT == airplane->GetPlaneType()) {
                airplane->SetOur(true);
                airplane->SetPlaneType(CAirplaneData::ASSUME);
            }
        }
    }
}

void CView::releaseAirplanes(const QString &current_sector)
{
    foreach (const QSharedPointer<CAirplane> &airplane, m_Airplanes) {
        if (CAirplaneData::ASSUME == airplane->GetPlaneType()) {
            const QSharedPointer<CSFPL> &sfpl = airplane->GetSFPL();
            const QStringList controlled_sectors = networkSettings()->value("users/current_sectors_short_name").toString().split("-", QString::SkipEmptyParts);
            //if (sfpl && (sfpl->GetCurSector() != current_sector)) {
            if (sfpl && !(controlled_sectors.contains(sfpl->GetCurSector()))) {
                airplane->SetOur(false);
                airplane->SetPlaneType(CAirplaneData::ACT);
            }
        }
    }
}
