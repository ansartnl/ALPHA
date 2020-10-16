#include "Master.h"

#if defined(_MSC_VER) && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "core/View.h"
#include "core/CFPL.h"
#include "common/Calculation.h"
#include "proto/AsterixDB.h"
#include "proto/xt/XtListeners.h"
#include "XtSimCmd.h"
#include "XtStand.h"
#include "XtVehicle.h"

#include "proto/obelix/CmdAirplanaStat.h"
#include "proto/obelix/CmdStandChanged.h"
#include "proto/obelix/CmdVehicleState.h"

#include "AirplaneCollision.h"
#include "RestrictionWarning.h"

#include "main.h"

#include <QStringList>

#include <QSqlDatabase>
#include <QSqlError>

#include <QFile>

#include "front/dialog/ORCAMDlg.h"
#include "front/ground/GUIGroundScene.h"
#include "front/core/GUIView.h"
#include "back/ground/GroundSceneLogic.h"
#include "Coordinate.h"

#include "front/dialog/FMpassDlg.h"

CMaster::CMaster()
    : m_iBeforehandFly(0), m_bStandSend(true),
      m_ObServer(*this), m_ObClient(*this),
      m_Planes(0), m_CalcTimer(), bSectoringInitiator(false),
      mCurrentSectorId(-1), mSIcomboEnable(false),
      mRedisDB(0), mORCAMDialog(0)
{
}

CMaster::~CMaster()
{
}

void CMaster::Init()
{
    // Start Asterix listener (RBS and UVD info parsing) if settings are defined.
    if ( networkSettings()->contains("radar_source/ip") &&
         networkSettings()->contains("radar_source/port") )
    {
        m_pAsterListener = QSharedPointer<IAsterixDataSrc> (new asterix::CSocketListener(
            this,
            networkSettings()->value("radar_source/ip").toString(),
            networkSettings()->value("radar_source/port").toInt(),
            networkSettings()->value("radar_source/mod_s", false).toBool(),
            networkSettings()->value("radar_source/log", false).toBool()));
        m_pAsterListener->Start();
    }

    //Pasha temp
    {
        setSI_FL(getSI_FL());
    }
    //////////////

    // Obelix
    QString sObIP = networkSettings()->value("simulator/obelix/ip").toString();
    int iObPort = networkSettings()->value("simulator/obelix/port").toInt();
    int iObTO = networkSettings()->value("simulator/obelix/timeout").toInt();
    if (networkSettings()->value("simulator/server_type").toString() == "server")
        m_ObServer.Start(sObIP, iObPort);
    m_ObClient.SetOperName(networkSettings()->value("simulator/operator","OP1").toString());
    if (iObTO >= 0)
    {
        m_ObClient.XMLConnect(sObIP, iObPort, iObTO * 1000);
        if(m_ObClient.GetState() != tcp::CXMLConnect::enConnected &&
                networkSettings()->value("simulator/server_type").toString() == "client")
        {
            m_CalcTimer.Connect(tva::bind(&CMaster::ObelixReconnect, this));
            m_CalcTimer.start(10000);
        }
    }
    ///////////////
    m_pSTCAReader = QSharedPointer<IAsterixDataSrc> (new asterix::CSTCAReader(
        this,
        networkSettings()->value("stca/ip").toString(),
        networkSettings()->value("stca/port").toInt()));
    m_pSTCAReader->Start();

    if (networkSettings()->value("simulator/server_type").toString() == "server")
    {
//#if defined(REDIS)
//#else
        m_pOLDIProc = QSharedPointer<OLDIProc> (new OLDIProc(QSqlDatabase::database(),
            networkSettings()->value("oldi/name").toString(),
            networkSettings()->value("oldi/lam_timeout", 10).toUInt()));
//#endif
    }

    helper.reset(new MasterHelper(this));
}

void CMaster::BlockNetworkMessages(bool on)
{
    m_pAsterListener ? m_pAsterListener->BlockMessages(on) : void();
    m_pAsterParser ? m_pAsterParser->BlockMessages(on) : void();
    m_pBearReader ? m_pBearReader->BlockMessages(on) : void();
    m_pSTCAReader ? m_pSTCAReader->BlockMessages(on) : void();
}

void CMaster::ObelixReconnect()
{
    TVA_CATCHALL_TRY
    {
        if(m_ObClient.GetState() != tcp::CXMLConnect::enConnected)
        {
            QString sObIP = networkSettings()->value("simulator/obelix/ip").toString();
            int iObPort = networkSettings()->value("simulator/obelix/port").toInt();
            int iObTO = networkSettings()->value("simulator/obelix/timeout").toInt();
            qDebug() << "Attempting to connect to Obelix server...";
            m_ObClient.XMLConnect(sObIP, iObPort, iObTO * 1000);
            if(m_ObClient.GetState() == tcp::CXMLConnect::enConnected)
                qDebug() << "Attemp succeeded";
            else
                qDebug() << "Attemp failed";
//                m_CalcTimer.stop();
        }
    } TVA_CATCHALL(err)
    {
        Q_UNUSED(err)
    }
}

void CMaster::SendPlaneState(const QSharedPointer<CAirplane> &pPlane)
{
    QString sOperName = pPlane->GetOperName();
    if (m_ObServer.GetOperConnections().find(sOperName) == m_ObServer.GetOperConnections().end())
        sOperName.clear();
    QSharedPointer<obelix::CCmdAirplaneStat> pCmd(new obelix::CCmdAirplaneStat(
            pPlane->GetID(),
            sOperName,
            pPlane));

    obelix::CServer::TOperConnectList::iterator itScanCon = m_ObServer.GetOperConnections().begin();
    for(; itScanCon != m_ObServer.GetOperConnections().end(); ++itScanCon)
    {
        if ((*itScanCon)->GetAirplaneList().find(pPlane->GetID()) !=
            (*itScanCon)->GetAirplaneList().end())
        {
            (*itScanCon)->XMLSend(pCmd.staticCast<CXMLCmd>());
        }
    }
    pPlane->SetIsChanged(false);
}

void CMaster::SetSectoringInitiator(bool bInitiator)
{
    bSectoringInitiator = bInitiator;
}

bool CMaster::GetAirportDataFromXML(GroundScene &scene)
{
    QDomDocument doc;
    //QString xmlPath = m_pConfig->Get(QL1S("Airports[path]"), QString(""));
    QString xmlPath("airports/");
    QString sceneName = scene.name();
    QFile file(QString("airports/%1.xml").arg(sceneName));
    QFile taxiwayFile(QString("airports/%1.csv").arg(sceneName));

    if (!file.open(QIODevice::ReadOnly))
        return false;
    if (!doc.setContent(&file)) {
        file.close();
        return false;
    }

    QDomNodeList dnlAprons = doc.elementsByTagName(QString("Apron"));
    QDomNodeList dnlRunways = doc.elementsByTagName(QString("Runway"));
    QDomNodeList dnlPaths = doc.elementsByTagName(QString("TaxiwayPath"));
    QDomNodeList dnlPoints = doc.elementsByTagName(QString("TaxiwayPoint"));
    QDomNodeList dnlBuildings = doc.elementsByTagName(QString("SceneryObject"));
    QDomNodeList dnlParkings = doc.elementsByTagName(QString("TaxiwayParking"));
    QDomNodeList dnlStarts = doc.elementsByTagName(QString("Start"));
    QDomNodeList dnlTowers = doc.elementsByTagName(QString("Tower"));
    QDomNodeList dnlSigns = doc.elementsByTagName(QString("TaxiwaySign"));
    QDomNodeList dnlRadioRestrictions = doc.elementsByTagName(QString("airportBounds"));

    // Runways
    for(int i = 0; i < dnlRunways.count(); i++)
    {
        QDomNamedNodeMap attributes = dnlRunways.at(i).attributes();
        double lon = attributes.namedItem(QString("lon")).toAttr().value().toDouble();
        double lat = attributes.namedItem(QString("lat")).toAttr().value().toDouble();
        double azimuth = attributes.namedItem(QString("heading")).toAttr().value().toDouble();
        QString size = attributes.namedItem(QString("width")).toAttr().value().toAscii();
        double width = size.left(size.length() - 1).toDouble();
        size = attributes.namedItem(QString("length")).toAttr().value().toAscii();
        double length = size.left(size.length() - 1).toDouble();
        int nNumber = attributes.namedItem(QString("number")).toAttr().value().toInt();
        QString sDesignator = attributes.namedItem(QString("designator")).toAttr().value();
        QString sNumber = attributes.namedItem(QString("number")).toAttr().value();

        if(sDesignator == QString("RIGHT") || sDesignator == QString("LEFT)"))
            sDesignator = sDesignator.left(1);

        QSharedPointer<GroundRunway> pRunway = scene.CreateRunway();
        pRunway->setAzimuth(azimuth);
        pRunway->setLength(length);
        pRunway->setWidth(width);
        CCoord coord;
        coord.SetLatitude(CAngle(lat));
        coord.SetLongitude(CAngle(lon));
        pRunway->setCoord(coord);
        pRunway->setNumber(nNumber);
        pRunway->setDesignator(sDesignator);
        pRunway->setName(sNumber + sDesignator);
        scene.AddRunway(pRunway);
    }

    // Aprons
    for(int i = 0; i < dnlAprons.count(); i++)
    {
        QSharedPointer<GroundApron> pApron = scene.CreateApron();

        QDomNodeList vertex = dnlAprons.at(i).toElement().elementsByTagName(QString("Vertex"));//dnlAprons.at(i).childNodes();
        for(int j = 0; j < vertex.count(); j++)
        {
            QDomNamedNodeMap attributes = vertex.at(j).attributes();
            double lon = attributes.namedItem(QString("lon")).toAttr().value().toDouble();
            double lat = attributes.namedItem(QString("lat")).toAttr().value().toDouble();
            CCoord vert;
            vert.SetLongitude(CAngle(lon));
            vert.SetLatitude(CAngle(lat));
            pApron->addVertex(vert);
        }
        scene.AddApron(pApron);
    }

    // Points
    QMap<int, CCoord> pts;
    for(int i = 0; i < dnlPoints.count(); i++)
    {
        QDomNamedNodeMap attributes = dnlPoints.at(i).attributes();
        double lon = attributes.namedItem(QString("lon")).toAttr().value().toDouble();
        double lat = attributes.namedItem(QString("lat")).toAttr().value().toDouble();
        int index = attributes.namedItem(QString("index")).toAttr().value().toInt();
        QString type = attributes.namedItem(QString("type")).toAttr().value();

        QSharedPointer<GroundPoint> pPoint = scene.CreatePoint();
        CCoord coord;
        coord.SetLatitude(CAngle(lat));
        coord.SetLongitude(CAngle(lon));
        pPoint->setCoord(coord);
        pPoint->setIndex(index);

        if(type == QString("HOLD_SHORT"))
            pPoint->setType(GroundPoint::HOLD_SHORT);
        else if(type == QString("NORMAL"))
            pPoint->setType(GroundPoint::Normal);
        else //if(type == QString("ILS_HOLD_SHORT"))
            pPoint->setType(GroundPoint::START);

        scene.AddPoint(pPoint);
        pts.insert(index, CCoord(CAngle(lon), CAngle(lat)));

        QSharedPointer<GroundCrossroads> pCrossroads = scene.CreateCrossroads();
        coord.SetLatitude(CAngle(lat));
        coord.SetLongitude(CAngle(lon));
        pCrossroads->setCoord(coord);
        pCrossroads->setIndex(index);
        scene.AddCrossroads(pCrossroads);
    }

    // Parkings
    QMap<int, CCoord> parking_pts;
    for(int i = 0; i < dnlParkings.count(); i++)
    {
        QDomNamedNodeMap attributes = dnlParkings.at(i).attributes();
        double lon = attributes.namedItem(QString("lon")).toAttr().value().toDouble();
        double lat = attributes.namedItem(QString("lat")).toAttr().value().toDouble();
        QString size = attributes.namedItem(QString("radius")).toAttr().value();
        QString name = attributes.namedItem(QString("name")).toAttr().value();
        double radius = size.left(size.length() - 1).toDouble();
        int number = attributes.namedItem(QString("number")).toAttr().value().toInt();
        int index = attributes.namedItem(QString("index")).toAttr().value().toInt();

        QSharedPointer<GroundParking> pParking = scene.CreateParking();
        pParking->setRadius(radius);
        pParking->setIndex(index);
        CCoord coord;
        coord.SetLatitude(CAngle(lat));
        coord.SetLongitude(CAngle(lon));
        pParking->setCoord(coord);
        pParking->setNumber(number);
        pParking->setName(name);
        scene.AddParking(pParking);
        parking_pts.insert(index, CCoord(CAngle(lon), CAngle(lat)));
    }

    // Paths & Traces
    for(int i = 0; i < dnlPaths.count(); i++)
    {
        QDomNamedNodeMap attributes = dnlPaths.at(i).attributes();
        QString size = attributes.namedItem(QString("width")).toAttr().value().toAscii();
        double width = size.left(size.length() - 1).toDouble();
        int number = attributes.namedItem(QString("number")).toAttr().value().toInt();
        int start = attributes.namedItem(QString("start")).toAttr().value().toInt();
        int end = attributes.namedItem(QString("end")).toAttr().value().toInt();

        //PATH
        QSharedPointer<GroundPath> pPath = scene.CreatePath();
        pPath->setWidth(width);
        pPath->setNumber(number);
        pPath->setStartPointIndex(start);
        pPath->setEndPointIndex(end);
        //pPath->setName(  scene.logic()->getTaxiways()[name] );

        pPath->setStartPointCoord(pts[start]);
        QString type = attributes.namedItem(QString("type")).toAttr().value().toAscii();
        if(type == QString("PARKING"))
        {
            pPath->setType(GroundPath::Parking);
            pPath->setEndPointCoord(parking_pts[end]);
        }
        else if(type == QString("RUNWAY"))
        {
            pPath->setType(GroundPath::Runway);
            pPath->setEndPointCoord(pts[end]);
        }
        else
        {
            pPath->setType(GroundPath::Taxi);
            pPath->setEndPointCoord(pts[end]);
        }
        scene.AddPath(pPath);

        //TRACE
        QSharedPointer<GroundTrace> pTrace = scene.CreateTrace();
        pTrace->setStartPointIndex(start);
        pTrace->setEndPointIndex(end);
        //pTrace->setName( scene.logic()->getTaxiways()[name] );

        pTrace->setStartPointCoord(pts[start]);
        type = attributes.namedItem(QString("type")).toAttr().value().toAscii();
        if(type == QString("PARKING"))
        {
            pTrace->setType(GroundTrace::Parking);
            pTrace->setEndPointCoord(parking_pts[end]);
        }
        else if(type == QString("RUNWAY"))
        {
            pTrace->setType(GroundTrace::Runway);
            pTrace->setEndPointCoord(pts[end]);
        }
        else
        {
            pTrace->setType(GroundTrace::Taxi);
            pTrace->setEndPointCoord(pts[end]);
        }
        scene.AddTrace(pTrace);

        if(type != QString("PARKING"))
        {
            CCoord from(pts[start]);
            CCoord to(pts[end]);
            from.Center(pts[start]);
            to.Center(pts[start]);

            double dAzim = GetAzimuth(from, to);
            if( GetDistance(to, from) > width)
            {
                if(scene.getCrossroads()[start]->addAzimuth(dAzim * 180/ M_PI))
                {
                    scene.getCrossroads()[start]->addWidth(width);
                }
                if(scene.getCrossroads()[end]->addAzimuth( Azimuth(dAzim + M_PI) * 180 / M_PI ))
                {
                    scene.getCrossroads()[end]->addWidth(width);
                }
            }
        }
    }

    // Start points
    for(int i = 0; i < dnlStarts.count(); i++)
    {
        QDomNamedNodeMap attributes = dnlStarts.at(i).attributes();
        double lon = attributes.namedItem(QString("lon")).toAttr().value().toDouble();
        double lat = attributes.namedItem(QString("lat")).toAttr().value().toDouble();
        double azimuth = attributes.namedItem(QString("heading")).toAttr().value().toDouble();
        int nNumber = attributes.namedItem(QString("number")).toAttr().value().toInt();

        QString sDesignator = attributes.namedItem(QString("designator")).toAttr().value();
        QString sNumber = attributes.namedItem(QString("number")).toAttr().value();

        if(sDesignator == QString("RIGHT") || sDesignator == QString("LEFT)"))
            sDesignator = sDesignator.left(1);

        QSharedPointer<GroundStart> pStart = scene.CreateStart();
        pStart->setAzimuth(azimuth);
        CCoord coord;
        coord.SetLatitude(CAngle(lat));
        coord.SetLongitude(CAngle(lon));
        pStart->setCoord(coord);
        pStart->setNumber(nNumber);
        pStart->setDesignator(sDesignator);
        pStart->setName(sNumber + sDesignator);
        scene.AddStart(pStart);
    }

    foreach(QSharedPointer<GroundPoint> pPoint, scene.getPoints())
    {
        foreach(QSharedPointer<GroundStart> pStart, scene.getStarts())
        if(GetDistance(pPoint->coord(), pStart->coord()) <= 20)
        {
            pStart->setPointIndex(pPoint->index());
        }
    }

    // Towers
    for(int i = 0; i < dnlTowers.count(); i++)
    {
        QDomNamedNodeMap attributes = dnlTowers.at(i).attributes();
        double lon = attributes.namedItem(QString("lon")).toAttr().value().toDouble();
        double lat = attributes.namedItem(QString("lat")).toAttr().value().toDouble();

        QSharedPointer<GroundTower> pTower = scene.CreateTower();
        CCoord coord;
        coord.SetLatitude(CAngle(lat));
        coord.SetLongitude(CAngle(lon));
        pTower->setCoord(coord);
        scene.AddTower(pTower);
    }

    // Buildings
    for(int i = 0; i < dnlBuildings.count(); i++)
    {
        QDomNamedNodeMap attributes = dnlBuildings.at(i).attributes();
        double lon = attributes.namedItem(QString("lon")).toAttr().value().toDouble();
        double lat = attributes.namedItem(QString("lat")).toAttr().value().toDouble();
        double azimuth = attributes.namedItem(QString("heading")).toAttr().value().toDouble();

        double width = 0;
        double length = 0;
        QDomNodeList building = dnlBuildings.at(i).toElement().elementsByTagName(QString("RectangularBuilding"));
        if(!building.isEmpty())
        {
            attributes = building.at(0).attributes();
            width = attributes.namedItem(QString("sizeX")).toAttr().value().toDouble();
            length = attributes.namedItem(QString("sizeZ")).toAttr().value().toDouble();
        }

        QSharedPointer<GroundBuilding> pBuilding = scene.CreateBuilding();
        pBuilding->setAzimuth(azimuth);
        pBuilding->setLength(length);
        pBuilding->setWidth(width);
        CCoord coord;
        coord.SetLatitude(CAngle(lat));
        coord.SetLongitude(CAngle(lon));
        pBuilding->setCoord(coord);
        scene.AddBuilding(pBuilding);
    }

    // Signs
    for(int i = 0; i < dnlSigns.count(); i++)
    {
        QDomNamedNodeMap attributes = dnlSigns.at(i).attributes();
        double lon = attributes.namedItem(QString("lon")).toAttr().value().toDouble();
        double lat = attributes.namedItem(QString("lat")).toAttr().value().toDouble();
        QString label = attributes.namedItem(QString("label")).toAttr().value();

        QSharedPointer<GroundSign> pSign = scene.CreateSign();
        CCoord coord;
        coord.SetLatitude(CAngle(lat));
        coord.SetLongitude(CAngle(lon));
        pSign->setCoord(coord);
        pSign->setLabel(label);

        scene.AddSign(pSign);
    }

    // Radio Restrictions
    for(int i = 0; i < dnlRadioRestrictions.count(); i++)
    {
        QSharedPointer<GroundRadioRestriction> pRestriction = scene.CreateRadioRestriction();

        QDomNodeList vertex = dnlRadioRestrictions.at(i).toElement().elementsByTagName(QString("coordinates"));
        QString buf = vertex.item(0).toElement().text();
        // "\r\n"
        QStringList coords = buf.split(QString("\r\n"), QString::SkipEmptyParts);

        for(int j = 0; j < coords.size(); ++j)
        {   // extracting coordinates
            QStringList buf = coords.at(j).split(QChar(','), QString::SkipEmptyParts);
            if(buf.size() > 1)
            {
                bool bOk1(false);
                bool bOk2(false);
                double lon = buf.at(0).toDouble(&bOk1);
                double lat = buf.at(1).toDouble(&bOk2);
                if(bOk1 && bOk2)
                {
                    CCoord vert;
                    vert.SetLongitude(CAngle(lon));
                    vert.SetLatitude(CAngle(lat));
                    pRestriction->addVertex(vert);
                }
            }
        }
        scene.AddRadioRestriction(pRestriction);
    }

    file.close();
    return true;
}

void CMaster::VehicleChanged(const Xt::Vehicle *xtVehicle)
{
    QSharedPointer<obelix::CCmdVehicleState> pCmd(new obelix::CCmdVehicleState());
    pCmd->InsertVehicle(xtVehicle->mId,
                        xtVehicle->mName,
                        xtVehicle->mVelocity,
                        CCoord(xtVehicle->mLongitude, xtVehicle->mLatitude),
                        xtVehicle->mHidden);
    if(xtVehicle->mDelete)
        pCmd->SetDel(xtVehicle->mDelete);
    GetObServer().SendToAllClients(pCmd);
}

void CMaster::GetZonesFromDB(QList<Zone>& Zones) const
{
    TVA_CATCHALL_TRY
    {
        dao()->LoadZones(Zones);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
}

void CMaster::GetSectorAirports(QStringList *airports)
{
    if(currentSectorId() != -1)
    {
        TVA_CATCHALL_TRY
        {
            QHash<quint32, QStringList> data;
            dao()->LoadSectorAirports(data);
            *airports = data[currentSectorId()];
        }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
    }
}

void CMaster::GetSectorAirportRunways(const QString& airportName, QStringList *runways)
{
    TVA_CATCHALL_TRY
    {
        dao()->LoadAirportRunwaysByName(airportName, runways);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
}

void CMaster::OnAsterixData(CAirplaneState& Data)
{
    // check STCA (podpora)
    if ((Data.GetTrackStatus() == CAirplaneState::BeginTrack)  || (Data.GetTrackStatus() == CAirplaneState::EndTrack))
        m_listSTCA.deleteSTCA(Data.GetTrackNumber());

    if (m_ZonesView)
        m_ZonesView->UpdateAirplane(Data);
    //UpdateAirplane(Data);   // <--
    m_listSTCA.decreaseSTCA(Data.GetTrackNumber());
    //qDebug() << QString("Asterix data id=%1 coord=%2").arg(Data.GetTrackNumber()).arg(Data.GetCoordinate().GetAsString());
    if (mORCAMDialog)
        mORCAMDialog->airplaneData(Data);
}

void CMaster::OnBearingData(CBearingData& Data)
{
    if (m_ZonesView)
        m_ZonesView->BearingData(Data);
}

void CMaster::OnSTCAData(const QVariant &v)
{
    if (m_ZonesView)
    {
        if (v.canConvert<AirplaneCollision>())
        {
            AirplaneCollision Data = v.value<AirplaneCollision>();
            if (Data.identifier.isEmpty() || (networkSettings()->value("users/current_sector_type", "").toString() == Data.identifier))
            {
                CView::TAirplanesMap::iterator itPlane1 = m_ZonesView->GetAirplanes().find(Data.id1);
                CView::TAirplanesMap::iterator itPlane2 = m_ZonesView->GetAirplanes().find(Data.id2);
                if ((itPlane1 != m_ZonesView->GetAirplanes().end()) && (itPlane2 != m_ZonesView->GetAirplanes().end()))
                {
                    // check STCA (podpora)
                    bool isOur    = (*itPlane1)->IsOur() || (*itPlane2)->IsOur();
                    bool isFlying = (*itPlane1)->isFlying() && (*itPlane2)->isFlying();
                    bool isOnAir  = (*itPlane1)->isOnAir() && (*itPlane2)->isOnAir();
                    bool flTest   = (*itPlane1)->GetFL() < 700 || (*itPlane2)->GetFL() < 700;
                    if (isOur && isFlying && isOnAir && flTest && GetDistance((*itPlane1)->GetCoord(), (*itPlane2)->GetCoord()) < 250000)
                    {
                        bool bNonRVSM(false);
                        if (!Data.RVSM)
                        {
                            if ((*itPlane1)->GetSFPL())
                                bNonRVSM |= !(*itPlane1)->GetSFPL()->isRVSM();
                            else
                                bNonRVSM |= true;
                            if ((*itPlane2)->GetSFPL())
                                bNonRVSM |= !(*itPlane2)->GetSFPL()->isRVSM();
                            else
                                bNonRVSM |= true;
                        }
                        if (bNonRVSM || Data.RVSM)
                        {
                            /*bool bRes = */m_listSTCA.addSTCA(Data);
                            //if (bRes) // optimization, not sure
                            {
                                (*itPlane1)->SetMapSTCA(m_listSTCA.mapSTCA(Data.id1));
                                (*itPlane2)->SetMapSTCA(m_listSTCA.mapSTCA(Data.id2));
                            }
                        }
                        else
                        {
                            Data.type = AirplaneCollision::NoCollision;
                        }
                    }
                    else
                    {
                        Data.type = AirplaneCollision::NoCollision;
                    }

                    // Reset STCA type.
                    if (Data.type == AirplaneCollision::NoCollision)
                    {
                        m_listSTCA.addSTCA(Data);
                    }

                    m_ZonesView->SetSTCA(Data);
                }
                else
                {
                    // Reset STCA type.
                    Data.type = AirplaneCollision::NoCollision;
                    m_listSTCA.addSTCA(Data);
                }
            }
        }

        else if (v.canConvert<RestrictionWarning>())
        {
            RestrictionWarning Data = v.value<RestrictionWarning>();
            if (Data.identifier.isEmpty() || (networkSettings()->value("users/current_sector_type", "").toString() == Data.identifier))
            {
                CView::TAirplanesMap::iterator itPlane = m_ZonesView->GetAirplanes().find(Data.boardNumber);
                if (itPlane != m_ZonesView->GetAirplanes().end()/* && (*itPlane)->IsOur()*/)
                    (*itPlane)->SetRestrictionWarning(Data);
            }
        }
        else if (v.canConvert<MeteoWarning>())
        {
            MeteoWarning Data = v.value<MeteoWarning>();
            if (Data.identifier.isEmpty() || (networkSettings()->value("users/current_sector_type", "").toString() == Data.identifier))
            {
                CView::TAirplanesMap::iterator itPlane = m_ZonesView->GetAirplanes().find(Data.boardNumber);
                if (itPlane != m_ZonesView->GetAirplanes().end()/* && (*itPlane)->IsOur()*/)
                    (*itPlane)->SetMeteoWarning(Data);
            }
        }
    }
}

void CMaster::OnObjectData(const Xt::TObject& obj)
{
    // Simulator command
    if (obj->GetType() == Xt::SimCmd::GetClassType())
    {
        Xt::SimCmd *cmd = static_cast<Xt::SimCmd *>( obj.data() );
        if(cmd->mType == Xt::SimCmd::StartExercise)
        {
            LOG_STR(Log4Qt::Level::DEBUG_INT, QObject::tr("Xt object: Xt::SimCmd::StartExercise"));
        }
        else if(cmd->mType == Xt::SimCmd::StopExercise)
        {
            LOG_STR(Log4Qt::Level::DEBUG_INT, QObject::tr("Xt object: Xt::SimCmd::StopExercise"));
            StopExercise(cmd->mDBID == dao()->GetDBID());
        }
        else if(cmd->mType == Xt::SimCmd::BeginMeteo)
        {
            LOG_STR(Log4Qt::Level::DEBUG_INT, QObject::tr("Xt object: Xt::SimCmd::BeginMeteo"));
            BeginSimMeteo(cmd->mIdFir, cmd->mDBID == dao()->GetDBID());
        }
        else if(cmd->mType == Xt::SimCmd::EndMeteo)
        {
            LOG_STR(Log4Qt::Level::DEBUG_INT, QObject::tr("Xt object: Xt::SimCmd::EndMeteo"));
            EndSimMeteo(cmd->mIdFir, cmd->mDBID == dao()->GetDBID());
        }
        else if(cmd->mType == Xt::SimCmd::BeginStand)
        {
            LOG_STR(Log4Qt::Level::DEBUG_INT, QObject::tr("Xt object: Xt::SimCmd::BeginStand"));
            m_mapStand.clear();
            m_bStandSend = false;
        }
        else if(cmd->mType == Xt::SimCmd::EndStand)
        {
            LOG_STR(Log4Qt::Level::DEBUG_INT, QObject::tr("Xt object: Xt::SimCmd::EndStand"));
            m_bStandSend = true;
            StandEnd();
        }
    }
    if ( obj->GetType() == Xt::Stand::GetClassType() )
    {
        Xt::Stand *xtstand = static_cast<Xt::Stand *>( obj.data() );
        StandChanged(xtstand, m_bStandSend);
    }
    if ( obj->GetType() == Xt::Vehicle::GetClassType() )
    {
        Xt::Vehicle *xtVehicle = static_cast<Xt::Vehicle *>(obj.data());
        VehicleChanged(xtVehicle);
    }

    // Retranslate the call.
    if (m_ZonesView)
    {
        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView);
        if (view)
            view->OnObjectReceived(obj);
    }
}

void CMaster::loadFromDB(uint uiID, Scene& scene, IScene& View)
{
    TVA_CATCHALL_TRY
    {
        dao()->Load(uiID, scene);
        dao()->LoadMeteo(uiID, View);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
}

void CMaster::saveToDB(Scene &scene)
{
    TVA_CATCHALL_TRY
    {
        dao()->Save(scene);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
}

void MasterHelper::onCheckAirplane()
{
    if (mMaster->m_ZonesView)
    {
        foreach (const QSharedPointer<CAirplane>& plane, mMaster->m_ZonesView->GetAirplanes())
        {
            if (!plane->check())
                mMaster->m_ZonesView->DelAirplane(plane);
        }
    }

}

void CMaster::StopExercise(bool bSameDB)
{
    if(!bSameDB)
    {
        TVA_CATCHALL_TRY
        {
            dao()->InitOLDI(QStringList(), false, true);
        }TVA_CATCHALL(err){
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
    }
    m_mapStand.clear();
    StandEnd();
    mORCAMDialog->reset();
}

void CMaster::BeginSimMeteo(QString sIdFIR, bool bSameDB)
{
    if(!bSameDB)
    {
        TVA_CATCHALL_TRY
        {
            dao()->DeleteFIRMeteo(sIdFIR);
        }TVA_CATCHALL(err){
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
    }
}

void CMaster::UpdateAirplane(CAirplaneState &AirplaneState)
{
    if(!m_ZonesView) return;
    CCoord CoordTemp(AirplaneState.GetCoordinate());
    CoordTemp.Center(m_ZonesView->GetCentralPoint()->GetCoord());
    //if (IsSquareIncluded(CoordTemp, 100000))
    //{
        QSharedPointer<CAirplane> pAirplane;
        TAirplanesMap::iterator itFind = m_Planes->find(AirplaneState.GetTrackNumber());
        if (itFind == m_Planes->end())
        {
            // Create
            pAirplane = QSharedPointer<CAirplane>(m_ZonesView->CreateAirplane());
            pAirplane->SetID(AirplaneState.GetTrackNumber());
            pAirplane->SetOperName(QObject::tr("NS"));
            pAirplane->SetMode3AReplyInit(AirplaneState.GetMode3AReply());
        }
        else
        {
            // Update
            pAirplane = *itFind;
            if((AirplaneState.GetTrackStatus() == CAirplaneState::BeginTrack) || (AirplaneState.GetTrackStatus() == CAirplaneState::EndTrack))
            {
                //STCA (podpora)
                m_Planes->remove(pAirplane->GetID());
                itFind = m_Planes->end();
                if(AirplaneState.GetTrackStatus() == CAirplaneState::EndTrack)
                    return;

                pAirplane = QSharedPointer<CAirplane>(m_ZonesView->CreateAirplane());
                pAirplane->SetID(AirplaneState.GetTrackNumber());
                pAirplane->SetOperName(QObject::tr("NS"));
                pAirplane->SetMode3AReplyInit(AirplaneState.GetMode3AReply());
            }
        }

        pAirplane->SetPlaneSPI(AirplaneState.GetSPI());
        pAirplane->SetMode3AReply(AirplaneState.GetMode3AReply());
        pAirplane->SetCoord(CoordTemp);
        pAirplane->SetFL(AirplaneState.GetFlightLevel());
        pAirplane->SetVelocity(AirplaneState.GetVelocity());
        pAirplane->SetVz(AirplaneState.GetZVelocity());
        pAirplane->SetTrackNumberUvd(AirplaneState.GetTrackNumberUvd());
        if(AirplaneState.IsHeightQNH())
            pAirplane->SetHeightQNH(AirplaneState.GetHeightQNH());
        else
            pAirplane->MarkHeightQNH(false);
        pAirplane->setUpdated(true);
        if (pAirplane->GetSFPL() && pAirplane->GetSFPL()->GetFPLFrag())
        {
            pAirplane->GetSFPL()->GetFPLFrag()->Update(
                    pAirplane->GetCoord(),
                    pAirplane->GetVelocity().GetV());
            if((!pAirplane->GetSFPL()->GetIsCOPin() && !pAirplane->GetSFPL()->GetCOPin().isEmpty())
                || (!pAirplane->GetSFPL()->GetIsCOPout() && !pAirplane->GetSFPL()->GetCOPout().isEmpty()))
            {
                CProcFrag::TPointFragList listPoints = pAirplane->GetSFPL()->GetFPLFrag()->GetAllPoint();
                CProcFrag::TPointFragList::const_iterator itPnt =
                        listPoints.constBegin();
                for(; itPnt != listPoints.constEnd(); ++itPnt)
                {
                    if((*itPnt)->IsPassed())
                    {
                        if(!pAirplane->GetSFPL()->GetIsCOPin() && !pAirplane->GetSFPL()->GetCOPin().isEmpty())
                        {
                            if((*itPnt == pAirplane->GetSFPL()->GetCOPinPoint())
                                || ((*itPnt)->GetPoint()->GetName().contains(pAirplane->GetSFPL()->GetCOPin())))
                            {
                                pAirplane->GetSFPL()->SetIsCOPin(true);
                                pAirplane->GetSFPL()->SetETOin((*itPnt)->GetPassedTime().time());
                            }
                        }
                        if(!pAirplane->GetSFPL()->GetIsCOPout() && !pAirplane->GetSFPL()->GetCOPout().isEmpty())
                        {
                            if((*itPnt == pAirplane->GetSFPL()->GetCOPoutPoint())
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
        if (itFind == m_Planes->end())
            AddAirplane(pAirplane);
}

void CMaster::AddAirplane(const QSharedPointer<CAirplane> &pAirplane)
{
    (*m_Planes)[pAirplane->GetID()] = pAirplane;
    QDateTime dt;
    dao()->GetSysdate(dt);
    pAirplane->SetStartTime(dt);
    pAirplane->SetMaster(this);
}

void CMaster::StandChanged(const Xt::Stand* xtstand, bool bSend/* = true*/)
{
    if (xtstand)
    {
        GroundStand stand;
        stand.mId = xtstand->mId;
        stand.mStandId = xtstand->mStandId;
        stand.mStandName = xtstand->mStandName;
        stand.mAirportId = xtstand->mAirportId;
        stand.mAirportName = xtstand->mAirportName;
        stand.mFplId = xtstand->mFplId;
        stand.mFplName = xtstand->mFplName;
        stand.mStateId = xtstand->mStateId;
        stand.mStateName = xtstand->mStateName;

        m_mapStand[stand.mId] = stand;

        if (bSend)
        {
            QSharedPointer<obelix::CCmdStandChanged> pCmd(new obelix::CCmdStandChanged(
                    stand.mId,
                    stand.mAirportName,
                    stand.mStandName,
                    stand.mStateId,
                    stand.mFplName));
            m_ObServer.SendToAllClients(pCmd);
        }
    }
}

void CMaster::StandEnd()
{
    // send update to clients
    m_bStandSend = true;
    QSharedPointer<obelix::CCmdStandChanged> pCmd(new obelix::CCmdStandChanged(
            0,
            "",
            "",
            0,
            ""));
    m_ObServer.SendToAllClients(pCmd);

    QMap<quint32, GroundStand>::const_iterator itMap = m_mapStand.constBegin();
    for(; itMap != m_mapStand.constEnd(); ++itMap)
    {
        pCmd = QSharedPointer<obelix::CCmdStandChanged>(new obelix::CCmdStandChanged(
                itMap->mId,
                itMap->mAirportName,
                itMap->mStandName,
                itMap->mStateId,
                itMap->mFplName));
        m_ObServer.SendToAllClients(pCmd);
    }

    pCmd = QSharedPointer<obelix::CCmdStandChanged>(new obelix::CCmdStandChanged(
            UINT_MAX,
            "",
            "",
            0,
            ""));
    m_ObServer.SendToAllClients(pCmd);
}

QString CMaster::getSI_FL() const
{
    QString sRes = networkSettings()->value("airplane/FL_SI", "FM").toString();
    FMpassDlg::getFMvalues(sRes);
    return sRes;
}

void CMaster::setSI_FL(QString val)
{
    QRegExp rexp("([F,M]{1,1})([F,M]{0,1})");
    if(rexp.exactMatch(val))
    {
        if(!rexp.cap(1).isEmpty())
        {
            if(!rexp.cap(2).isEmpty())
                mSIcomboEnable = true;
            else
                mSIcomboEnable = false;
            if(!mSIcomboEnable)
            {
                int tempSI = AppSettings::SIMeters;
                if("F" == rexp.cap(1))
                    tempSI |= AppSettings::SIFeet | AppSettings::SIKnot;
                AppSettings::SetSI(tempSI);
            }
        }
    }
}

void CMaster::showORCAMDialog()
{
    if (mORCAMDialog)
        mORCAMDialog->show();
}

QSharedPointer<CPoint> CMaster::findPointByName(const QString &name) const
{
    if (m_ZonesView && m_ZonesView.dynamicCast<CGUIView>())
    {
        QSharedPointer<CGUIView> view = m_ZonesView.dynamicCast<CGUIView>();
        CPointKey key(name);
        foreach (QSharedPointer<Scene> scene ,view->GetScenes())
        {
            if (scene->GetPoints().contains(key))
                return scene->GetPoints().value(key);
        }
    }
    return QSharedPointer<CPoint>();
}
