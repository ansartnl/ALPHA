#include "Server.h"

#include "main.h"
#include "TVAError.h"

namespace tcp {
    void CServer::Start(const QString& sIP, quint16 ushPort)
    {
        Stop();
        m_Server.setMaxPendingConnections(5);
        TVA_CHECK(
                m_Server.listen(QHostAddress(sIP), ushPort),
                tr("Unable to listen adress: %1 : %2. Description: %3").arg(sIP).arg(ushPort).arg(m_Server.errorString()),
                E_FAIL);
        connect(&m_Server, SIGNAL(newConnection()), this, SLOT(SlotConnect()));
    }

    void CServer::Stop()
    {
        disconnect(&m_Server, 0, 0, 0);
        m_Server.close();
    }

    void CServer::OnDelete(QTcpSocket* pConnect)
    {
        TConnectList::iterator itFind = m_Connections.find(pConnect);
        if (itFind != m_Connections.end())
            m_Connections.erase(itFind);
        else
            LOG_STR(Log4Qt::Level::ERROR_INT, tr(
                "Unable to delete tcp connection"));
    }

    void CServer::SlotConnect()
    {
        int iAttempt(5);
        while (m_Server.hasPendingConnections() && iAttempt-- > 0)
        {
            QTcpSocket* pSocket = m_Server.nextPendingConnection();
            if (pSocket)
            {
                TConnect pConnect(CreateConnect(pSocket));
                m_Connections[pSocket] = pConnect;
            }
        }
    }
}
