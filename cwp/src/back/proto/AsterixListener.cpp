#include "AsterixListener.h"
#include "main.h"

#include <QDir>
#include <QFile>

#include "asterixdata.h"
#include "asterixerrors.h"
#include "aerotechnica/asterix062aero.h"
#include "eurocontrol/asterix004euro.h"
#include "eurocontrol/asterix253euro.h"
#include "peleng/asterix062peleng.h"

#include "GUIView.h"
#include "AirplaneCollision.h"
#include "BearingData.h"
#include "RestrictionWarning.h"

namespace asterix
{

static const QString AsterixLogFile()
{
    return QDir(QDir("bin:").absoluteFilePath("asterix")).absoluteFilePath(QDateTime::currentDateTimeUtc().toString("yy-MM-dd_hh"));
}

CSocketListener::CSocketListener(IParent* pParent, const QString& qsIP, quint16 quiPort, bool modeS, bool log)
    : m_pParent(pParent), m_Host(qsIP), m_ushPort(quiPort), mIsMessagesBlocked(false), m_ModeS(modeS), m_Logging(log)

{}

CSocketListener::~CSocketListener()
{
    Stop();
}

void CSocketListener::Start()
{
    Stop();

    TVA_CHECK(m_Socket.bind(m_Host, m_ushPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint),
              (tr("Unable to listen adress") + (": %1 : %2")).arg(m_Host.toString()).arg(m_ushPort),
              E_FAIL);
    connect(&m_Socket, SIGNAL(readyRead()), this, SLOT(OnUDPData()));
}

void CSocketListener::Stop()
{
    disconnect(&m_Socket, SIGNAL(readyRead()), 0, 0);
}

void CSocketListener::OnUDPData()
{
    while (m_Socket.hasPendingDatagrams()) {
        QByteArray qabBuffer;
        qabBuffer.resize(m_Socket.pendingDatagramSize());
        QHostAddress SenderIP;
        quint16 quiSenderPort(0);
        m_Socket.readDatagram(qabBuffer.data(), qabBuffer.size(),
                              &SenderIP, &quiSenderPort);

        if (mIsMessagesBlocked || !m_pParent)
            continue;

        if (m_ModeS)
        {
            //addToLog("datagram ", qabBuffer);

            if (m_Logging)
            {
                addToLog("Datagram (hexadecimal representation):");
                QString str_l="";
                char a[10];
                for(int i = 0;i < qabBuffer.size(); i++ )
                {
                    str_l = str_l + " " + QString("%1").arg((unsigned char)qabBuffer.at(i),2,16,QLatin1Char('0'));
                    if ( (( (i+1) % 16) == 0 ) )
                    {
                        addToLog(str_l);
                        str_l = "";
                    }
                }
                addToLog(str_l);
            }
            qabBuffer.remove(0, 25); // skip header
            if (m_Logging)
            {
                addToLog("Datagram without header (hexadecimal representation):");
                QString str_l="";
                for(int i = 0;i < qabBuffer.size(); i++ )
                {
                    str_l = str_l + " " + QString("%1").arg((unsigned char)qabBuffer.at(i),2,16,QLatin1Char('0'));
                    if ( (( (i+1) % 16) == 0 ) )
                    {
                        addToLog(str_l);
                        str_l = "";
                    }
                }
                addToLog(str_l);
            }
        }

        TVA_CATCHALL_TRY
        {
            int readedBytes = 0;
            forever
            {
                if (qabBuffer.isEmpty())
                    break;

                AsterixData aData;
                const int category = AsterixData::categoryFromArray(qabBuffer);
                if (category == 4)
                {
                    aData.category = 4;
                    aData.variety = AsterixEurocontrol;
                }
                else if (category == 62)
                {
                    aData.category = 62;
                    aData.variety = (m_ModeS ? AsterixPeleng : AsterixAerotechnica);
                }
                else if (category == 253)
                {
                    aData.category = 253;
                    aData.variety = AsterixEurocontrol;
                }
                else
                {
                    break;
                }

                AsterixErrors::ErrorCode errorCode = aData.fromArray(qabBuffer, &readedBytes);
                if (errorCode == AsterixErrors::Error_MessageIsShort)
                    break;

                bool isErrorExists = false;
                bool isProtocolNotSupported = false;
                // Dump errors to log.
                foreach (AsterixErrors *error, aData.errors())
                {
                    if (*error == AsterixErrors::OK || *error == AsterixErrors::Error_Mandatory)
                        continue;

                    QString errorString = tr("Asterix listener: ") + error->toString();

                    if (error->code() == AsterixErrors::Error_ProtocolNotSupported)
                    {
                        errorString += tr(". Asterix category: ") + QString("%1").arg(int(aData.category), 3, 10, QChar('0'));
                        isProtocolNotSupported = true;
                    }

                    if (error->code() == AsterixErrors::Error_UnexpectedEndOfData ||
                            error->code() == AsterixErrors::Error_ProtocolNotSupported)
                    {
                        errorString += tr(". Buffer dump: ") + qabBuffer.toHex();
                    }

                    if (error->code() == AsterixErrors::Error_UnexpectedEndOfData)
                        continue;

                    if (error->code() != AsterixErrors::Error_Mandatory)
                        isErrorExists = true;

                    LOG_STR(INFO, errorString);
                }

                // If error is occurred -> break datagram dump.
                if (isErrorExists && !isProtocolNotSupported)
                    break;

                qabBuffer.remove(0, readedBytes);
                if (qabBuffer == QByteArray(qabBuffer.size(), 0))
                    qabBuffer.clear();

                switch (category)
                {
                case 4:
                    processCat004(aData);
                    break;
                case 62:
                    processCat062(aData);
                    break;
                case 253:
                    processCat253(aData);
                    break;
                }
            }

        } TVA_TRAPALL{}
    }
}

void CSocketListener::addToLog(const QString &key, const QVariant &value)
{
    if (m_Logging)
    {
        QFile file(AsterixLogFile());
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
            return;
        QTextStream(&file) << key << " " << value.toString() << endl;
    }
}

void CSocketListener::addToLog(const QString &key)
{
    if (m_Logging)
    {
        QFile file(AsterixLogFile());
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
            return;
        QTextStream(&file) << key << endl;
    }
}
void CSocketListener::processCat004(const AsterixData &aData)
{
    using namespace Eurocontrol;

    if (aData.value(Asterix004::MessageType).toUInt() == Asterix004::MSAW)
    {
        addToLog("TrackNumber 1", aData.value(Asterix004::TrackNumber1));

        RestrictionWarning warning(aData.value(Asterix004::TrackNumber1).toUInt(), RestrictionWarning::UserObstacle);

        addToLog("parsing completed", true);
        m_pParent->OnSTCAData(QVariant::fromValue(warning));
    }
    else if (aData.value(Asterix004::MessageType).toUInt() == Asterix004::STCA)
    {
        addToLog("TrackNumber 1", aData.value(Asterix004::TrackNumber1));
        addToLog("TrackNumber 2", aData.value(Asterix004::TrackNumber2));

        AirplaneCollision collision(aData.value(Asterix004::TrackNumber1).toUInt(),
                                    aData.value(Asterix004::TrackNumber2).toUInt(),
                                    1, AirplaneCollision::Crash);

        addToLog("parsing completed", true);
        m_pParent->OnSTCAData(QVariant::fromValue(collision));
    }
}

void CSocketListener::processCat062(const AsterixData &aData)
{
    CAirplaneState airplaneState;

    // RBS information receiving ...

    addToLog("SAC", aData.value(Asterix::SAC));
    airplaneState.SetSAC(aData.value(Asterix::SAC).toUInt());
    addToLog("SIC", aData.value(Asterix::SIC));
    airplaneState.SetSIC(aData.value(Asterix::SIC).toUInt());
    addToLog("TrackNumber", aData.value(Asterix062Proto::TrackNumber));
    airplaneState.SetTrackNumber(aData.value(Asterix062Proto::TrackNumber).toUInt());

    addToLog("Callsign", aData.value(Asterix062Proto::AircraftDerivedData_ID));
    airplaneState.SetCallsign(aData.value(Asterix062Proto::AircraftDerivedData_ID).toString());

    addToLog("Cfl", aData.value(Asterix062Proto::AircraftDerivedData_FSS_Altitude));
    airplaneState.setCfl(aData.value(Asterix062Proto::AircraftDerivedData_FSS_Altitude).toDouble());
    addToLog("SFL(SAL)", aData.value(Asterix062Proto::AircraftDerivedData_SAL_Altitude));
    airplaneState.setSfl(aData.value(Asterix062Proto::AircraftDerivedData_SAL_Altitude).toDouble());
    addToLog("Heading", aData.value(Asterix062Proto::AircraftDerivedData_TAN));
    airplaneState.setHeading(aData.value(Asterix062Proto::AircraftDerivedData_TAN).toDouble());
    addToLog("TrueAirspeed", aData.value(Asterix062Proto::AircraftDerivedData_TAS));
    airplaneState.setTrueAirspeed(aData.value(Asterix062Proto::AircraftDerivedData_TAS).toInt());
    addToLog("Groundspeed", aData.value(Asterix062Proto::AircraftDerivedData_GSP));
    airplaneState.setGroundAirspeed(aData.value(Asterix062Proto::AircraftDerivedData_GSP).toDouble());
    addToLog("VerticalRate", aData.value(Asterix062Proto::AircraftDerivedData_BVR));
    airplaneState.setVerticalRate(aData.value(Asterix062Proto::AircraftDerivedData_BVR).toDouble());
    if (aData.contains(Asterix062Proto::AircraftDerivedData_IAR))
    {
        addToLog("IndicatedAirspeed", aData.value(Asterix062Proto::AircraftDerivedData_IAR));
        airplaneState.setIndicatedAirspeed(aData.value(Asterix062Proto::AircraftDerivedData_IAR).toInt());
    }
    else if (!aData.value(Asterix062Proto::AircraftDerivedData_IAS_IM).toInt())
    {
        addToLog("IndicatedAirspeed/IndicatedAirspeed",aData.value(Asterix062Proto::AircraftDerivedData_IAS).toDouble()*3600);
        airplaneState.setIndicatedAirspeed(aData.value(Asterix062Proto::AircraftDerivedData_IAS).toDouble()*3600);
    }
    if (aData.contains(Asterix062Proto::AircraftDerivedData_MAC)) {
        addToLog("Mach Number", aData.value(Asterix062Proto::AircraftDerivedData_MAC));
        airplaneState.setIndicatedAirspeedMach(aData.value(Asterix062Proto::AircraftDerivedData_MAC).toDouble());
    } else if (aData.value(Asterix062Proto::AircraftDerivedData_IAS_IM).toInt()) {
        addToLog("IndicatedAirspeed/MachNumber", aData.value(Asterix062Proto::AircraftDerivedData_IAS));
        airplaneState.setIndicatedAirspeedMach(aData.value(Asterix062Proto::AircraftDerivedData_IAS).toDouble());
    }

    addToLog("longitude", aData.value(Asterix062Proto::CalculatedTrackPositionWGS_Longitude));
    addToLog("latitude", aData.value(Asterix062Proto::CalculatedTrackPositionWGS_Latitude));
    CCoord coord(
                aData.value(Asterix062Proto::CalculatedTrackPositionWGS_Longitude).toDouble(),
                aData.value(Asterix062Proto::CalculatedTrackPositionWGS_Latitude).toDouble());

    if (gActiveView && gActiveView->GetCentralPoint())
        coord.Center(gActiveView->GetCentralPoint()->GetCoord());

    airplaneState.SetCoordinate(coord);

    // Because Asterix062 returns feet instead of flightLevel
    addToLog("FlightLevel", aData.value(Asterix062Proto::MeasuredFlightLevel));
    airplaneState.SetFlightLevel(aData.value(Asterix062Proto::MeasuredFlightLevel).toDouble()/100.);

    addToLog("Vx", aData.value(Asterix062Proto::CalculatedTrackVelocityCartesian_Vx));
    addToLog("Vy", aData.value(Asterix062Proto::CalculatedTrackVelocityCartesian_Vy));
    airplaneState.SetVelocity(CVelocity(
                                  aData.value(Asterix062Proto::CalculatedTrackVelocityCartesian_Vx).toDouble(),
                                  aData.value(Asterix062Proto::CalculatedTrackVelocityCartesian_Vy).toDouble()));

    if (aData.contains(Asterix062Proto::TimeOfTrackInformation)) {
        addToLog("time", aData.value(Asterix062Proto::TimeOfTrackInformation));
        airplaneState.SetTime(aData.value(Asterix062Proto::TimeOfTrackInformation).toTime());
    }

    addToLog("ZVelocity", aData.value(Asterix062Proto::CalculatedRateOfClimbDescent));
    airplaneState.SetZVelocity(aData.value(Asterix062Proto::CalculatedRateOfClimbDescent).toDouble());

    addToLog("Mode3AReply", aData.value(Asterix062Proto::TrackMode3ACode_reply));
    airplaneState.SetMode3AReply(aData.value(Asterix062Proto::TrackMode3ACode_reply).toUInt());

    // Because of CAirplaneState and Asterix062 implementation need to SPI sign -> shift 6 bits left.
    addToLog("spi", aData.value(Asterix062Proto::TrackStatus_SPI));
    airplaneState.SetSPI(aData.value(Asterix062Proto::TrackStatus_SPI).toUInt() << 6);

    // Track status
    Asterix062Proto::TTrackStatusMap trackStatus = aData.value(Asterix062Proto::TrackStatus).value<Asterix062Proto::TTrackStatusMap>();
    if (trackStatus.contains(Asterix062Proto::ETrackStatus_TSB) && trackStatus.value(Asterix062Proto::ETrackStatus_TSB) == 1)
        airplaneState.SetTrackStatus( CAirplaneState::BeginTrack );
    if (trackStatus.contains(Asterix062Proto::ETrackStatus_TSE) && trackStatus.value(Asterix062Proto::ETrackStatus_TSE) == 1)
        airplaneState.SetTrackStatus( CAirplaneState::EndTrack );

    if (aData.contains(Asterix062Proto::AircraftDerivedData_BPS)) {
        addToLog("BPS", aData.value(Asterix062Proto::AircraftDerivedData_BPS));
        airplaneState.SetBPS(aData.value(Asterix062Proto::AircraftDerivedData_BPS).toDouble());
    }

    if (aData.contains(Asterix062Proto::AircraftDerivedData_ACS)) {
        addToLog("ACAS", aData.value(Asterix062Proto::AircraftDerivedData_ACS));
        airplaneState.SetACAS(true);
    }

    // UVD information receiving ...

    if (m_ModeS)
    {
        using namespace Peleng;

        if (aData.contains(Asterix062::SP_BoardNumber))
        {
            airplaneState.setIsUvd(true);
            Asterix062::BoardNumber bn = aData.value(Asterix062::SP_BoardNumber).value<Asterix062::BoardNumber>();
            airplaneState.SetMode3AReply(bn.number);
        }
        if (aData.contains(Asterix062::SP_FuelHeight))
        {
            airplaneState.setIsUvd(true);
            Asterix062::FuelHeight fh = aData.value(Asterix062::SP_FuelHeight).value<Asterix062::FuelHeight>();
            if (fh.A)
            {
                // Convert from 10m to m
                airplaneState.SetHeightQNH(fh.height * 10);
            }
            else
            {
                // Convert from 10m to FL
                airplaneState.SetFlightLevel(fh.height / 3.048);
            }
            if (fh.B)
                airplaneState.setEmg();
        }
    }
    else
    {
        using namespace Aerotechnica;

        if (aData.contains(Asterix062::SP_BoardNumber))
            airplaneState.SetTrackNumberUvd( aData.value(Asterix062::SP_BoardNumber).toUInt() );

        if (aData.contains(Asterix062::SP_Altitude))
        {
            Asterix062::SAltitude h = aData.value(Asterix062::SP_Altitude).value<Asterix062::SAltitude>();
            // Convert from feet to flightLevel
            airplaneState.SetFlightLevel(h.altitude()/100.);
        }

        if (aData.contains(Asterix062::SP_Fuel))
            airplaneState.SetFuelReserve( aData.value(Asterix062::SP_Fuel).toUInt() );

        //                                if (aData.contains(Asterix062::SP_BRF))
        //                                    airplaneState.SetTime( aData.value(Asterix062::SP_BRF).toTime() );

        if (aData.contains(Asterix062::SP_Emergency))
            airplaneState.SetVFI(airplaneState.GetVFI() | CAirplaneState::Emergency);

        if (aData.contains(Asterix062::SP_FplStatus))
            airplaneState.SetFplStatus( aData.value(Asterix062::SP_FplStatus).toUInt() );

        if (aData.contains(Asterix062::SP_HeightQNH))
            airplaneState.SetHeightQNH( aData.value(Asterix062::SP_HeightQNH).toDouble() );
    }

    addToLog("parsing completed", true);
    m_pParent->OnAsterixData(airplaneState);
}

void CSocketListener::processCat253(const AsterixData &aData)
{
    using namespace Eurocontrol;

    CBearingData bearingData;

    addToLog("SAC", aData.value(Asterix253::DataSourceIdentifier_SAC));
    bearingData.SetSAC(aData.value(Asterix253::DataSourceIdentifier_SAC).toUInt());
    addToLog("SIC", aData.value(Asterix253::DataSourceIdentifier_SIC));
    bearingData.SetSIC(aData.value(Asterix253::DataSourceIdentifier_SIC).toUInt());
    addToLog("Frequency", aData.value(Asterix253::Frequency));
    bearingData.SetID(aData.value(Asterix253::Frequency).toUInt());
    addToLog("Bearing", aData.value(Asterix253::Bearing));
    bearingData.SetBearing(aData.value(Asterix253::Bearing).toDouble());

    addToLog("parsing completed", true);
    m_pParent->OnBearingData(bearingData);
}
}
