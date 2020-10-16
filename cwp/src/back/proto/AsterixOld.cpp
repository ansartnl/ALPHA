#include "AsterixOld.h"

#include "Coordinate.h"
#include "main.h"

namespace asterix
{
    static const int EXTENSION_MASK			= 0x01;
    // 1st byte
    static const int DATASRCID_MASK			= 0x80;	// I062/010 Data Source Identifier
    static const int SERVICEID_MASK			= 0x20;	// I062/015 Service Identification
    static const int TIME_MASK				= 0x10;	// I062/070 Time Of Track Information
    static const int WGSPOSITION_MASK		= 0x08;	// I062/105 Calculated Track Position (WGS-84)
    static const int CARTPOSITION_MASK		= 0x04;	// I062/100 Calculated Track Position (Cartesian)
    static const int CARTVELOCITY_MASK		= 0x02;	// I062/185 Calculated Track Velocity (Cartesian)
    // 2nd byte
    static const int CARTACCELER_MASK		= 0x80;	// I062/210 Calculated Acceleration (Cartesian)
    static const int TRACKMODE3A_MASK		= 0x40;	// I062/060 Track Mode 3/A Code
    static const int TARGETID_MASK			= 0x20;	// I062/245 Target Identification
    static const int AIRCRAFTDATA_MASK		= 0x10;	// I062/380 !!! Aircraft Derived Data
    static const int TRACKNUMBER_MASK		= 0x08;	// I062/040 Track Number
    static const int TRACKSTATUS_MASK		= 0x04;	// I062/080 !!! Track Status
    static const int SYSTRACKAGES_MASK		= 0x02;	// I062/290 !!! System Track Update Ages
    // 3nd byte
    static const int MOVEMENTMODE_MASK		= 0x80;	// I062/200 Mode of Movement
    static const int TRACKDATAAGES_MASK		= 0x40;	// I062/295 !!! Track Data Ages
    static const int FLIGHTLEVEL_MASK		= 0x20;	// I062/136 Measured Flight Level
    static const int GEOMALTITUDE_MASK		= 0x10;	// I062/130 Calculated Track Geometric Altitude
    static const int BAROALTITUDE_MASK		= 0x08;	// I062/135 Calculated Track Barometric Altitude
    static const int CLIMBRATE_MASK			= 0x04;	// I062/220 Calculated Rate Of Climb/Descent
    static const int PLANDATA_MASK			= 0x02;	// I062/390 !!!	Flight Plan Related Data

    //I062/380 Aircraft Derived Data masks
    // 1st
    static const int AIRCRAFTDATA_ADR_MASK	= 0x80;
    static const int AIRCRAFTDATA_ID_MASK	= 0x40;
    static const int AIRCRAFTDATA_MHG_MASK	= 0x20;
    static const int AIRCRAFTDATA_IAS_MASK	= 0x10;
    static const int AIRCRAFTDATA_TAS_MASK	= 0x08;
    static const int AIRCRAFTDATA_SAL_MASK	= 0x04;
    static const int AIRCRAFTDATA_FSS_MASK	= 0x02;
    // 2nd
    static const int AIRCRAFTDATA_TIS_MASK	= 0x80;
    static const int AIRCRAFTDATA_TID_MASK	= 0x40;
    static const int AIRCRAFTDATA_COM_MASK	= 0x20;
    static const int AIRCRAFTDATA_SAB_MASK	= 0x10;
    static const int AIRCRAFTDATA_ACS_MASK	= 0x08;
    static const int AIRCRAFTDATA_BVR_MASK	= 0x04;
    static const int AIRCRAFTDATA_GVR_MASK	= 0x02;
    // 3d
    static const int AIRCRAFTDATA_RAN_MASK	= 0x80;
    static const int AIRCRAFTDATA_TAR_MASK	= 0x40;
    static const int AIRCRAFTDATA_TAN_MASK	= 0x20;
    static const int AIRCRAFTDATA_GSP_MASK	= 0x10;
    static const int AIRCRAFTDATA_VUN_MASK	= 0x08;
    static const int AIRCRAFTDATA_MET_MASK	= 0x04;
    static const int AIRCRAFTDATA_EMC_MASK	= 0x02;
    // 4th
    static const int AIRCRAFTDATA_POS_MASK	= 0x80;
    static const int AIRCRAFTDATA_GAL_MASK	= 0x40;
    static const int AIRCRAFTDATA_PUN_MASK	= 0x20;
    static const int AIRCRAFTDATA_MB_MASK	= 0x10;
    static const int AIRCRAFTDATA_IAR_MASK	= 0x08;
    static const int AIRCRAFTDATA_MAC_MASK	= 0x04;
    static const int AIRCRAFTDATA_BPS_MASK	= 0x02;

    // I062/290 System Track Update Ages masks
    // 1st
    static const int SYSTRACKAGES_TRK_MASK	= 0x80;
    static const int SYSTRACKAGES_PSR_MASK	= 0x40;
    static const int SYSTRACKAGES_SSR_MASK	= 0x20;
    static const int SYSTRACKAGES_MDS_MASK	= 0x10;
    static const int SYSTRACKAGES_ADS_MASK	= 0x08;
    static const int SYSTRACKAGES_ES_MASK	= 0x04;
    static const int SYSTRACKAGES_VDL_MASK	= 0x02;
    // 2nd
    static const int SYSTRACKAGES_UAT_MASK	= 0x80;
    static const int SYSTRACKAGES_LOP_MASK	= 0x40;
    static const int SYSTRACKAGES_MLT_MASK	= 0x20;

    // I062/295 Track Data Ages masks
    // 1st
    static const int TRACKDATAAGES_MFL_MASK	= 0x80;
    static const int TRACKDATAAGES_MD1_MASK	= 0x40;
    static const int TRACKDATAAGES_MD2_MASK	= 0x20;
    static const int TRACKDATAAGES_MDA_MASK	= 0x10;
    static const int TRACKDATAAGES_MD4_MASK	= 0x08;
    static const int TRACKDATAAGES_MD5_MASK	= 0x04;
    static const int TRACKDATAAGES_MHG_MASK	= 0x02;
    // 2nd
    static const int TRACKDATAAGES_IAS_MASK	= 0x80;
    static const int TRACKDATAAGES_TAS_MASK	= 0x40;
    static const int TRACKDATAAGES_SAL_MASK	= 0x20;
    static const int TRACKDATAAGES_FSS_MASK	= 0x10;
    static const int TRACKDATAAGES_TID_MASK	= 0x08;
    static const int TRACKDATAAGES_COM_MASK	= 0x04;
    static const int TRACKDATAAGES_SAB_MASK	= 0x02;
    // 3d
    static const int TRACKDATAAGES_ACS_MASK	= 0x80;
    static const int TRACKDATAAGES_BVR_MASK	= 0x40;
    static const int TRACKDATAAGES_GVR_MASK	= 0x20;
    static const int TRACKDATAAGES_RAN_MASK	= 0x10;
    static const int TRACKDATAAGES_TAR_MASK	= 0x08;
    static const int TRACKDATAAGES_TAN_MASK	= 0x04;
    static const int TRACKDATAAGES_GSP_MASK	= 0x02;
    // 4th
    static const int TRACKDATAAGES_VUN_MASK	= 0x80;
    static const int TRACKDATAAGES_MET_MASK	= 0x40;
    static const int TRACKDATAAGES_EMC_MASK	= 0x20;
    static const int TRACKDATAAGES_POS_MASK	= 0x10;
    static const int TRACKDATAAGES_GAL_MASK	= 0x08;
    static const int TRACKDATAAGES_PUN_MASK	= 0x04;
    static const int TRACKDATAAGES_MB_MASK	= 0x02;
    // 5th
    static const int TRACKDATAAGES_IAR_MASK	= 0x80;
    static const int TRACKDATAAGES_MAC_MASK	= 0x40;
    static const int TRACKDATAAGES_BPS_MASK	= 0x20;

    CReader::CReader(IParent* pParent, const QString& qsIP, quint16 quiPort)
        : m_pParent(pParent), m_Host(qsIP), m_ushPort(quiPort), m_isMessagesBlocked(true)

    {

    }

    CReader::~CReader()
    {
        Stop();
    }

    void CReader::Start()
    {
        Stop();
        //m_FromHost.setAddress(qsFromIP);
        //m_ushFromPort = quiFromPort;
        TVA_CHECK(
            m_Socket.bind(m_Host, m_ushPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint),
            (tr("Unable to listen adress") + ": %1 : %2").arg(m_Host.toString()).arg(m_ushPort),
            E_FAIL);
        connect(&m_Socket, SIGNAL(readyRead()), this, SLOT(OnUDPData()));
    }

    void CReader::Stop()
    {
        disconnect(&m_Socket, SIGNAL(readyRead()), 0, 0);
    }

    void CReader::OnUDPData()
    {
        int iAttemp(5);
        while (m_Socket.hasPendingDatagrams() && iAttemp-- > 0) {
            QByteArray qabBuffer;
            qabBuffer.resize(m_Socket.pendingDatagramSize());
            QHostAddress SenderIP;
            quint16 quiSenderPort(0);
            m_Socket.readDatagram(qabBuffer.data(), qabBuffer.size(),
                                    &SenderIP, &quiSenderPort);

            if ( m_isMessagesBlocked )
                continue;

            CAirplaneState Data;
            if(ParseData(qabBuffer, Data))
            {
                TVA_CATCHALL_TRY
                {
                    if(m_pParent)
                        m_pParent->OnAsterixData(Data);
                } TVA_TRAPALL{}
            }
        }
    }

    #define CHECK_COUNTER(a, b) if(a < b) {TVA_THROW(QObject::tr("Error"), E_FAIL);} else a-=b;
    bool CReader::ParseData(QByteArray& Buffer, CAirplaneState& Data)
    {
        TVA_CATCHALL_TRY
        {
            quint16 quiBufferSize = Buffer.size();
            TVA_CHECK(quiBufferSize > 3, tr("Invalid data"), S_FALSE);
            //if (SenderIP && quiSenderPort)
            // Parse
            QDataStream qStream(Buffer);
            // Category & Size
            quint8 qbCategory(0);
            quint16 quiSize(0);
            qStream >> qbCategory;
            qStream >> quiSize;
            quiBufferSize = qMin(quiSize, quiBufferSize);

            TVA_CHECK(qbCategory == CATEGORY && quiBufferSize > 3, tr("Invalid data"), S_FALSE);
            quiBufferSize -= 3;

            // Mask
            quint8 qbMask;
            QByteArray qaMask;
            do
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream >> qbMask;
                qaMask.append(qbMask);
            } while(qbMask & EXTENSION_MASK);
            // Fields
            // 1st
            //-----------------------------------
            if(qaMask.size() > 0)
            {
                // 062/010 Data Source Identifier
                if (qaMask[0] & DATASRCID_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 2);
                    quint8 ubTemp(0);
                    qStream >> ubTemp;
                    Data.SetSAC(ubTemp);
                    qStream >> ubTemp;
                    Data.SetSIC(ubTemp);
                }
                // I062/015 Service Identification
                if (qaMask[0] & SERVICEID_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 1);
                    qStream.skipRawData(1);
                }
                // I062/070 Time Of Track Information
                if (qaMask[0] & TIME_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 3);
                    quint8 qubT1, qubT2, qubT3;
                    qStream >> qubT1;
                    qStream >> qubT2;
                    qStream >> qubT3;
                    quint64 quMiliseconds = ((quint64)qubT1<<16) + ((quint64)qubT2<<8) + qubT3;
                    quMiliseconds *= 1000;
                    quMiliseconds /= 128;
                    int iMS = quMiliseconds % 1000;
                    quMiliseconds /= 1000;
                    int iS = quMiliseconds % 60;
                    quMiliseconds /= 60;
                    int iM = quMiliseconds % 60;
                    int iH = quMiliseconds / 60;
                    Data.SetTime(QTime(iH, iM, iS, iMS));
                }
                // I062/105 Calculated Track Position (WGS-84)
                if (qaMask[0] & WGSPOSITION_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 8);
                    qint32 iTemp(0);
                    qStream >> iTemp;
                    qint64 qillLatitude = iTemp;
                    qStream >> iTemp;
                    qint64 qillLongitude = iTemp;
                    Data.SetCoordinate(CCoord(
                            (double)qillLongitude * 180 / 33554432,
                            (double)qillLatitude * 180 / 33554432));
                }
                // I062/100 Calculated Track Position (Cartesian)
                if (qaMask[0] & CARTPOSITION_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 6);
                    qStream.skipRawData(6);
                }
                // I062/185 Calculated Track Velocity (Cartesian)
                if (qaMask[0] & CARTVELOCITY_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 4);
                    qint16 qiXVel, qiYVel;
                    qStream >> qiXVel;
                    qStream >> qiYVel;
                    CVelocity Vel;
                    Vel.SetVx((double)qiXVel / 4);
                    Vel.SetVy((double)qiYVel / 4);
                    Data.SetVelocity(Vel);
                }
            }
            // 2nd
            //-----------------------------------
            if (qaMask.size() > 1)
            {
                // I062/210 Calculated Acceleration (Cartesian)
                if (qaMask[1] & CARTACCELER_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 2);
                    qStream.skipRawData(2);
                }
                // I062/060 Track Mode 3/A Code
                if (qaMask[1] & TRACKMODE3A_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 2);
                    quint16 ushTemp(0);
                    qStream >> ushTemp;
                    ushTemp &= 0x0FFF;;
                    Data.SetMode3AReply(ushTemp);
                }
                // I062/245 Target Identification
                if (qaMask[1] & TARGETID_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 7);
                    qStream.skipRawData(7);
                }
                // I062/380 Aircraft Derived Data
                if (qaMask[1] & AIRCRAFTDATA_MASK)
                {
                    GetAircraftData(qStream, quiBufferSize);
                }
                // I062/040 Track Number
                if (qaMask[1] & TRACKNUMBER_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 2);
                    quint16 ushTemp(0);
                    qStream >> ushTemp;
                    Data.SetTrackNumber(ushTemp);
                }
                // I062/080 Track Status
                if (qaMask[1] & TRACKSTATUS_MASK)
                {
                    quint8 ubTemp(0);
                    CHECK_COUNTER(quiBufferSize, 1);
                    qStream >> ubTemp;
                    Data.SetSPI(ubTemp);
                    while(ubTemp & EXTENSION_MASK)
                    {
                        CHECK_COUNTER(quiBufferSize, 1);
                        qStream >> ubTemp;
                    }
                    //GetTrackStatus(qStream, quiBufferSize);
                }
                // I062/290 System Track Update Ages
                if (qaMask[1] & SYSTRACKAGES_MASK)
                {
                    GetSysTrackUpdateAges(qStream, quiBufferSize);
                }
            }
            // 3d
            //-----------------------------------
            if (qaMask.size() > 2)
            {
                // I062/210 Calculated Acceleration (Cartesian)
                if (qaMask[2] & MOVEMENTMODE_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 1);
                    qStream.skipRawData(1);
                }
                // I062/295 Track Data Ages
                if (qaMask[2] & TRACKDATAAGES_MASK)
                {
                    GetTrackDataAges(qStream, quiBufferSize);
                }
                // I062/136 Measured Flight Level
                if (qaMask[2] & FLIGHTLEVEL_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 2);
                    qint16 quTemp(0);
                    qStream >> quTemp;
                    Data.SetFlightLevel((double)quTemp / 4);
                }
                // I062/130 Calculated Track Geometric Altitude
                if (qaMask[2] & GEOMALTITUDE_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 2);
                    qStream.skipRawData(2);
                }
                // I062/135 Calculated Track Barometric Altitude
                if (qaMask[2] & BAROALTITUDE_MASK)
                {
                    CHECK_COUNTER(quiBufferSize, 2);
                    qStream.skipRawData(2);
//					quint16 ushTemp(0);
//					qStream >> ushTemp;
//					Data.SetTrackNumber(ushTemp);
                }
                // I062/220 Calculated Rate Of Climb/Descent
                if (qaMask[2] & CLIMBRATE_MASK)
                {
                    qint16 qiTemp(0);
                    CHECK_COUNTER(quiBufferSize, 2);
                    qStream >> qiTemp;
                    Data.SetZVelocity((double)qiTemp * 25 / 4);
                }
            }
        }TVA_TRAPALL
        {
            return false;
        }
        return true;
    }

    void CReader::GetAircraftData(QDataStream& qStream, quint16& quiBufferSize)
    {
        // Mask
        quint8 qbMask;
        QByteArray qaMask;
        do
        {
            CHECK_COUNTER(quiBufferSize, 1);
            qStream >> qbMask;
            qaMask.append(qbMask);
            TVA_CHECK(
                qaMask.size() <= 4,
                tr("Wrong size of item mask I062/380, Aircraft Derived Data"),
                E_FAIL);
        } while(qbMask & EXTENSION_MASK);
        // 1st
        //-----------------------------------
        if(qaMask.size() > 0)
        {
            if (qaMask[0] & AIRCRAFTDATA_ADR_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 3);
                qStream.skipRawData(3);
            }
            if (qaMask[0] & AIRCRAFTDATA_ID_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 6);
                qStream.skipRawData(6);
            }
            if (qaMask[0] & AIRCRAFTDATA_MHG_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[0] & AIRCRAFTDATA_IAS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[0] & AIRCRAFTDATA_TAS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[0] & AIRCRAFTDATA_SAL_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[0] & AIRCRAFTDATA_FSS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }

        }
        // 2nd
        //-----------------------------------
        if (qaMask.size() > 1)
        {
            if (qaMask[1] & AIRCRAFTDATA_TIS_MASK)
            {
                quint8 quiTemp;
                do
                {
                    CHECK_COUNTER(quiBufferSize, 1);
                    qStream >> quiTemp;
                } while (quiTemp & EXTENSION_MASK);
            }
            if (qaMask[1] & AIRCRAFTDATA_TID_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 16);
                qStream.skipRawData(16);
            }
            if (qaMask[1] & AIRCRAFTDATA_COM_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[1] & AIRCRAFTDATA_SAB_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[1] & AIRCRAFTDATA_ACS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 7);
                qStream.skipRawData(7);
            }
            if (qaMask[1] & AIRCRAFTDATA_BVR_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[1] & AIRCRAFTDATA_GVR_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
        }
        // 3d
        //-----------------------------------
        if (qaMask.size() > 2)
        {
            if (qaMask[2] & AIRCRAFTDATA_RAN_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[2] & AIRCRAFTDATA_TAR_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[2] & AIRCRAFTDATA_TAN_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[2] & AIRCRAFTDATA_GSP_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[2] & AIRCRAFTDATA_VUN_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[2] & AIRCRAFTDATA_MET_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 8);
                qStream.skipRawData(8);
            }
            if (qaMask[2] & AIRCRAFTDATA_EMC_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
        }
        // 4th
        //-----------------------------------
        if (qaMask.size() > 3)
        {
            if (qaMask[3] & AIRCRAFTDATA_POS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 6);
                qStream.skipRawData(6);
            }
            if (qaMask[3] & AIRCRAFTDATA_GAL_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[3] & AIRCRAFTDATA_PUN_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[3] & AIRCRAFTDATA_MB_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 9);
                qStream.skipRawData(9);
            }
            if (qaMask[3] & AIRCRAFTDATA_IAR_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[3] & AIRCRAFTDATA_MAC_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[3] & AIRCRAFTDATA_BPS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
        }
    }

    void CReader::GetTrackStatus(QDataStream& qStream, quint16& quiBufferSize)
    {
        quint8 ubTemp(0);
        do
        {
            CHECK_COUNTER(quiBufferSize, 1);
            qStream >> ubTemp;
        } while(ubTemp & EXTENSION_MASK);
    }

    void CReader::GetSysTrackUpdateAges(QDataStream& qStream, quint16& quiBufferSize)
    {
        // Mask
        quint8 qbMask;
        QByteArray qaMask;
        do
        {
            CHECK_COUNTER(quiBufferSize, 1);
            qStream >> qbMask;
            qaMask.append(qbMask);
            TVA_CHECK(
                qaMask.size() <= 3,
                tr("Wrong size of item mask I062/290 System Track Update Ages"),
                E_FAIL);
        } while(qbMask & EXTENSION_MASK);
        // 1st
        //-----------------------------------
        if(qaMask.size() > 0)
        {
            if (qaMask[0] & SYSTRACKAGES_TRK_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[0] & SYSTRACKAGES_PSR_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[0] & SYSTRACKAGES_SSR_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[0] & SYSTRACKAGES_MDS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[0] & SYSTRACKAGES_ADS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 2);
                qStream.skipRawData(2);
            }
            if (qaMask[0] & SYSTRACKAGES_ES_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[0] & SYSTRACKAGES_VDL_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
        }
        // 2nd
        //-----------------------------------
        if (qaMask.size() > 1)
        {
            if (qaMask[1] & SYSTRACKAGES_UAT_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[1] & SYSTRACKAGES_LOP_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[1] & SYSTRACKAGES_MLT_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
        }
    }

    void CReader::GetTrackDataAges(QDataStream& qStream, quint16& quiBufferSize)
    {
        // Mask
        quint8 qbMask;
        QByteArray qaMask;
        do
        {
            CHECK_COUNTER(quiBufferSize, 1);
            qStream >> qbMask;
            qaMask.append(qbMask);
            TVA_CHECK(
                qaMask.size() <= 5,
                tr("Wrong size of item mask I062/295 Track Data Ages"),
                E_FAIL);
        } while(qbMask & EXTENSION_MASK);
        // 1st
        //-----------------------------------
        if(qaMask.size() > 0)
        {
            if (qaMask[0] & TRACKDATAAGES_MFL_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[0] & TRACKDATAAGES_MD1_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[0] & TRACKDATAAGES_MD2_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[0] & TRACKDATAAGES_MDA_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[0] & TRACKDATAAGES_MD4_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[0] & TRACKDATAAGES_MD5_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[0] & TRACKDATAAGES_MHG_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
        }
        // 2nd
        //-----------------------------------
        if(qaMask.size() > 1)
        {
            if (qaMask[1] & TRACKDATAAGES_IAS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[1] & TRACKDATAAGES_TAS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[1] & TRACKDATAAGES_SAL_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[1] & TRACKDATAAGES_FSS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[1] & TRACKDATAAGES_TID_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[1] & TRACKDATAAGES_COM_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[1] & TRACKDATAAGES_SAB_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
        }
        // 3d
        //-----------------------------------
        if(qaMask.size() > 2)
        {
            if (qaMask[2] & TRACKDATAAGES_ACS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[2] & TRACKDATAAGES_BVR_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[2] & TRACKDATAAGES_GVR_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[2] & TRACKDATAAGES_RAN_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[2] & TRACKDATAAGES_TAR_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[2] & TRACKDATAAGES_TAN_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[2] & TRACKDATAAGES_GSP_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
        }
        // 4th
        //-----------------------------------
        if(qaMask.size() > 3)
        {
            if (qaMask[3] & TRACKDATAAGES_VUN_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[3] & TRACKDATAAGES_MET_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[3] & TRACKDATAAGES_EMC_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[3] & TRACKDATAAGES_POS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[3] & TRACKDATAAGES_GAL_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[3] & TRACKDATAAGES_PUN_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[3] & TRACKDATAAGES_MB_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
        }
        // 5th
        //-----------------------------------
        if(qaMask.size() > 4)
        {
            if (qaMask[4] & TRACKDATAAGES_IAR_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[4] & TRACKDATAAGES_MAC_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
            if (qaMask[4] & TRACKDATAAGES_BPS_MASK)
            {
                CHECK_COUNTER(quiBufferSize, 1);
                qStream.skipRawData(1);
            }
        }
    }

    CGenerator::CGenerator()
        : m_ushPort(0)
    {

    }
    CGenerator::~CGenerator()
    {
        Stop();
    }

    void CGenerator::Start(const QString &qsIP, quint16 quiPort)
    {
        Stop();
        m_Host.setAddress(qsIP);
        m_ushPort = quiPort;
        //m_FromHost.setAddress(qsFromIP);
        //m_ushFromPort = quiFromPort;
        /*TVA_CHECK_STRM(
            m_Socket.bind(m_FromHost, m_ushFromPort),
            "Unable to bind to adress: " << qsFromIP.toStdString() << ":" << quiFromPort,
            E_FAIL);*/
        connect(&m_Socket, SIGNAL(bytesWritten(qint64)), this, SLOT(OnUDPData(qint64)));
    }

    void CGenerator::Stop()
    {
        disconnect(&m_Socket, SIGNAL(bytesWritten(qint64)), 0, 0);
    }

    void CGenerator::Send(const CAirplaneState& AirplaneState)
    {
        TVA_CHECK(m_Queue.size() < 10000, tr("UDP Queue is overflowed"), E_FAIL);
        QByteArray baData;
        CreateData(AirplaneState, baData);
        if (m_Socket.writeDatagram(baData, m_Host, m_ushPort) < 0)
        {
            QAbstractSocket::SocketError SockError = m_Socket.error();
            if (SockError == 0)
                m_Queue.append(baData);
        }
    }

    void CGenerator::CreateData(const CAirplaneState& AirplaneState, QByteArray& Data)
    {
        QByteArray baMask, baDataReal;
        baDataReal.reserve(256);
        QDataStream DataStream(&baDataReal, QIODevice::WriteOnly);

        // 1st
        //-----------------------------------
        baMask += (char)EXTENSION_MASK;
        // I062/010 Data Source Identifier
        baMask[0] = baMask[0] | DATASRCID_MASK;
        DataStream << AirplaneState.GetSAC();
        DataStream << AirplaneState.GetSIC();
        // I062/070 Time Of Track Information
        baMask[0] = baMask[0] | TIME_MASK;
        qint64 qiTime =
            AirplaneState.GetTime().msec() +
            1000 *
            (AirplaneState.GetTime().second() + 60 *
            (AirplaneState.GetTime().minute() +
            60 * AirplaneState.GetTime().hour()));
        qiTime *= 128;
        qiTime /= 1000;
        DataStream << (quint8)(qiTime >> 16);
        DataStream << (quint8)(qiTime >> 8);
        DataStream << (quint8)qiTime;
        // I062/105 Calculated Track Position (WGS-84)
        if (AirplaneState.IsCoordinate())
        {
            baMask[0] = baMask[0] | WGSPOSITION_MASK;
            DataStream << (qint32)(AirplaneState.GetCoordinate().GetLatitude().GetAngle() * 33554432 / 180);
            DataStream << (qint32)(AirplaneState.GetCoordinate().GetLongitude().GetAngle() * 33554432 / 180);
        }
        // I062/185 Calculated Track Velocity (Cartesian)
        if (AirplaneState.IsVelocity())
        {
            baMask[0] = baMask[0] | CARTVELOCITY_MASK;
            qint16 Vx = (qint16)(PreRound(AirplaneState.GetVelocity().GetVx() * 4));
            qint16 Vy = (qint16)(PreRound(AirplaneState.GetVelocity().GetVy() * 4));
            DataStream << Vx;
            DataStream << Vy;
        }
        // 2nd
        //-----------------------------------
        baMask += (char)EXTENSION_MASK;
        // I062/060 Track Mode 3/A Code
        if (AirplaneState.IsMode3AReply())
        {
            baMask[1] = baMask[1] | TRACKMODE3A_MASK;
            DataStream << quint16(AirplaneState.GetMode3AReply());
        }
        // I062/040 Track Number
        baMask[1] = baMask[1] | TRACKNUMBER_MASK;
        DataStream << AirplaneState.GetTrackNumber();
        // I062/080 Track Status
        baMask[1] = baMask[1] | TRACKSTATUS_MASK;
        DataStream << (quint8)0;
        // 3d
        //-----------------------------------
        baMask += (char)EXTENSION_MASK;
        // I062/136 Measured Flight Level
        if (AirplaneState.IsFlightLevel())
        {
            baMask[2] = baMask[2] | FLIGHTLEVEL_MASK;
            DataStream << (qint16)(AirplaneState.GetFlightLevel()*4);
        }
        // I062/220 Calculated Rate Of Climb/Descent
        if (AirplaneState.IsZVelocity())
        {
            baMask[2] = baMask[2] | CLIMBRATE_MASK;
            DataStream << (qint16)(PreRound(AirplaneState.GetZVelocity() * 4 / 25));
        }

        // Truncate extra bytes
        int iSize = baMask.size();
        for(; iSize > 0; --iSize)
            if(baMask[iSize-1] != (char)EXTENSION_MASK)
                break;
        baMask.resize(iSize);
        baMask[iSize - 1] = baMask[iSize - 1] & (0xFF ^ EXTENSION_MASK);
        Data.clear();
        QDataStream StreamHeader(&Data, QIODevice::WriteOnly);
        StreamHeader << (quint8)CATEGORY;
        StreamHeader << (quint16)(3 + baMask.size() + baDataReal.size());
        Data += baMask;
        Data += baDataReal;
    }

    void CGenerator::OnUDPData(qint64 bytes)
    {
        if (!m_Queue.empty())
        {
            if (m_Socket.writeDatagram(m_Queue.front(), m_Host, m_ushPort)<0)
            {
                // Write error;
                int i=1;
                i++;
            }
            m_Queue.pop_front();
        }
    }


    CBearReader::CBearReader(IParent* pParent, const QString& qsIP, quint16 quiPort)
        : CReader(0, qsIP, quiPort), m_pParent(pParent)
    {

    }

    bool CBearReader::ParseData(QByteArray& Buffer, CAirplaneState& Result)
    {
        if(Buffer.size() == 9)
        {
            QDataStream StreamData(&Buffer, QIODevice::ReadOnly);
            quint8 i8(0), i8_1(0), i8_2(0);
            quint16 i16(0);
            StreamData >> i8_1;
            StreamData >> i16;
            StreamData >> i8_2;
            if(i8_1 == 253 && i16 == 9 && i8_2 == 0xc0)
            {
                StreamData >> i8;
                Result.SetSAC(i8);
                StreamData >> i8;
                Result.SetSIC(i8);
                StreamData >> i8;
                Result.SetTrackNumber(i8);
                StreamData >> i16;
                Result.SetMode3AReply(i16);
            }

            return true;
        }
        return false;
    }

    void CBearReader::OnUDPData()
    {
        int iAttemp(5);
        while (m_Socket.hasPendingDatagrams() && iAttemp-- > 0) {
            QByteArray qabBuffer;
            qabBuffer.resize(m_Socket.pendingDatagramSize());
            QHostAddress SenderIP;
            quint16 quiSenderPort(0);
            m_Socket.readDatagram(qabBuffer.data(), qabBuffer.size(),
                                    &SenderIP, &quiSenderPort);

            if ( m_isMessagesBlocked )
                continue;

            LOG_STR(DEBUG, tr("Recieve bearing data:%1").arg(QString(qabBuffer.toHex())));
            CAirplaneState Data;
            if(ParseData(qabBuffer, Data))
            {
                TVA_CATCHALL_TRY
                {
//                    if(m_pParent)
//                        m_pParent->OnBearingData(Data);
                } TVA_TRAPALL{}
            }
        }
    }

    CBearGenerator::CBearGenerator()
        : CGenerator()
    {

    }

    void CBearGenerator::Send(const CAirplaneState& AirplaneState)
    {
        TVA_CHECK(m_Queue.size() < 10000, tr("UDP Queue is overflowed"), E_FAIL);
        QByteArray baData;
        CreateData(AirplaneState, baData);
        if (m_Socket.writeDatagram(baData, m_Host, m_ushPort) < 0)
        {
            QAbstractSocket::SocketError SockError = m_Socket.error();
            if (SockError == 0)
                m_Queue.append(baData);
        }
    }

    void CBearGenerator::CreateData(const CAirplaneState& AirplaneState, QByteArray& Data)
    {
        QDataStream StreamData(&Data, QIODevice::WriteOnly);
        StreamData << quint8(253);
        StreamData << quint16(9);
        StreamData << quint8(0xc0);
        StreamData << quint8(AirplaneState.GetSAC());
        StreamData << quint8(AirplaneState.GetSIC());
        //Channel
        StreamData << quint8(AirplaneState.GetTrackNumber());
        //Direction
        StreamData << quint16(AirplaneState.GetMode3AReply());
    }

    CSTCAReader::CSTCAReader(IParent* pParent, const QString& qsIP, quint16 quiPort)
        : CReader(0, qsIP, quiPort), m_pParent(pParent)
    {

    }

    bool CSTCAReader::ParseData(QByteArray& Buffer, QVariant& v)
    {
        QDataStream ds(Buffer);
        ds >> v;
        if (QDataStream::Ok == ds.status())
        {
            Buffer.remove(0, ds.device()->pos());
            return true;
        }
        return false;
    }

    void CSTCAReader::OnUDPData()
    {
        while (m_Socket.hasPendingDatagrams()) {
            QByteArray qabBuffer;
            qabBuffer.resize(m_Socket.pendingDatagramSize());
            QHostAddress SenderIP;
            quint16 quiSenderPort(0);
            m_Socket.readDatagram(qabBuffer.data(), qabBuffer.size(),
                                    &SenderIP, &quiSenderPort);

            if ( m_isMessagesBlocked )
                continue;

            QVariant v;
            while (ParseData(qabBuffer, v) && !v.isNull())
            {
                TVA_CATCHALL_TRY
                {
                    if(m_pParent)
                        m_pParent->OnSTCAData(v);
                } TVA_TRAPALL{}
                if (qabBuffer.isEmpty())
                    break;
            }
        }
    }

}
