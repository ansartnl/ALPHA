#include "AsterixDB.h"

#include "main.h"
#include "QOciQuery.h"


#include <QtCore/QTimer>

#include <QtSql/QSqlError>
#include "dbconnectionsmanager.h"

namespace asterix {
    CDBReader::CDBReader(IParent* pParent) :
        checkToTimer(new QTimer(this)), m_pParent(pParent)
    {
        connect(checkToTimer, SIGNAL(timeout()), this, SLOT(checkProcess()));
    }

    CDBReader::~CDBReader()
    {
        Stop();
    }

    void CDBReader::Start()
    {
        Stop();
        checkToTimer->start(2000);
    }

    void CDBReader::Stop()
    {
        checkToTimer->stop();
    }

    void CDBReader::checkProcess()
    {
        TVA_CATCHALL_TRY
            {
                if(DBConnectionsManager::instance().getConnectionStatus() == 1)
                    return;
                QOciQuery query(QSqlDatabase::database());
                query.prepare("SELECT sac, sic, track_number, mode_3a_reply, track_time, longitude, latitude, "
                    "flight_level, v_x, v_y, v_z FROM asterix");
                TVA_CHECK(query.exec(), QObject::tr("Execution error. %1").arg(query.lastError().text()), E_FAIL);
                while (query.next())
                {
                    CAirplaneState AirState;
                    AirState.SetSAC(query.value(0).toUInt());
                    AirState.SetSIC(query.value(1).toUInt());
                    AirState.SetTrackNumber(query.value(2).toUInt());
                    if (!query.value(3).isNull())
                        AirState.SetMode3AReply(query.value(3).toUInt());
                    if (!query.value(4).isNull())
                        AirState.SetTime(query.value(4).toTime());
                    if (!query.value(5).isNull() && !query.value(6).isNull())
                    {
                        AirState.SetCoordinate(CCoord(query.value(5).toDouble(), query.value(6).toDouble()));
                    }
                    if (!query.value(7).isNull())
                        AirState.SetFlightLevel(query.value(7).toDouble());
                    if (!query.value(8).isNull() && !query.value(9).isNull())
                        AirState.SetVelocity(CVelocity(query.value(8).toDouble(), query.value(9).toDouble()));
                    if (!query.value(10).isNull())
                        AirState.SetZVelocity(query.value(10).toDouble());

                    m_pParent->OnAsterixData(AirState);
                }
            }TVA_CATCHALL(err){
        LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
    }

}
}
