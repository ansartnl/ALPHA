#include "XMLConnect.h"

#include "TVAError.h"
#include "main.h"

#include <QtCore/QTimer>
namespace tcp {
    CXMLConnect::CXMLConnect() :
        CConnect(), m_State(enDisconnected), connectToTimeout(0), m_uiID(1), connectTimer(new QTimer(this))
    {
        connectTimer->setSingleShot(true);
        connect(connectTimer, SIGNAL(timeout()), this, SLOT(onConnectTo()));
    }

    CXMLConnect::CXMLConnect(QTcpSocket* pSocket, IServer* pServer) :
        CConnect(pSocket, pServer), m_State(enConnected), connectToTimeout(0), m_uiID(1),
                        connectTimer(new QTimer(this))
    {
        connectTimer->setSingleShot(true);
        connect(connectTimer, SIGNAL(timeout()), this, SLOT(onConnectTo()));
    }

    void CXMLConnect::XMLConnect(const QString& sIP, quint16 ushPort, int iMSec)
    {
        _XMLShutdown();
        CConnect::Connect(sIP, ushPort);
        m_State = enConnecting;

        m_sIP = sIP;
        m_ushPort = ushPort;
        connectToTimeout = iMSec;
        m_uiID = 1;
    }

    void CXMLConnect::XMLDisconnect()
    {
        _XMLShutdown();
        //Pasha, hz connectToTimeout = -1;
    }

    void CXMLConnect::XMLSend(const TXMLCmd& pXMLCmd)
    {
        TVA_CATCHALL_TRY
            {
                pXMLCmd->SetID(m_uiID++);
                _XMLSend(pXMLCmd.staticCast<CXMLBase> ());
                if (pXMLCmd->IsACK())
                    m_CmdOutQueue[pXMLCmd->GetID()] = pXMLCmd;
            }TVA_CATCHALL(err){
        LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
    }

}

void CXMLConnect::XMLSend(const TXMLResp& pXMLResp)
{
    TVA_CATCHALL_TRY
    {
        _XMLSend(pXMLResp.staticCast<CXMLBase>());
    }TVA_CATCHALL(err)
    {
        LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
    }
}

void CXMLConnect::OnConnect()
{
    m_State = enConnected;
    OnXMLConnect();
}

void CXMLConnect::OnDisconnect()
{
    if (m_State == enConnected ||
                    (m_State == enConnecting && connectToTimeout == 0))
    OnXMLDisconnect();

    _XMLShutdown();

    if (connectToTimeout > 0)
    connectTimer->start(connectToTimeout);
}

void CXMLConnect::OnRead(QByteArray& Buffer)
{
    // Parse
    m_InQueue.append(Buffer);
    while(true)
    {
        int iLengthEnd = m_InQueue.indexOf('!');
        TVA_CHECK(
                        (iLengthEnd != -1 && iLengthEnd <= 5) || m_InQueue.size() <= 5,
                        tr("Incoming packet has wrong size"),
                        E_FAIL);
        if (iLengthEnd != -1)
        {
            bool bOk(false);
            int iPacketSize = m_InQueue.left(iLengthEnd).toUInt(&bOk);
            TVA_CHECK(
                            bOk,
                            tr("Incoming packet has wrong size"),
                            E_FAIL);
            if (iPacketSize <= m_InQueue.size() - iLengthEnd - 1)
            {
                // Preparse
                CXMLData::TXMLData pData(new CXMLData(m_InQueue.mid(iLengthEnd+1, iPacketSize)));
                m_InQueue = m_InQueue.mid(iLengthEnd + 1 + iPacketSize);
                LOG_STR(Log4Qt::Level::TRACE_INT, QString(tr("Incoming packet") + ":\n%1").arg(pData->ToString()));
                if (pData->IsCmd())
                {
                    TXMLCmd pCmd = ResolveCmd(pData);

                    TXMLResp pResp = pCmd->Execute();
                    if (pResp)
                    XMLSend(pResp);
                }
                else
                {
                    TCmdOutQueue::iterator itFind = m_CmdOutQueue.find(pData->GetID());
                    if (itFind != m_CmdOutQueue.end())
                    {
                        TXMLCmd pCmd = *itFind;
                        m_CmdOutQueue.erase(itFind);
                        TXMLResp pResp = ResolveResp(pCmd->GetCmdName(), pData);
                        pCmd->Answer(pResp);
                    } else {
                        qWarning() << tr("Could not find request for id = %1").arg(pData->GetID());
                    }
                }
            }
            else
            break;
        }
        else
        break;
    }
}

void CXMLConnect::OnSend()
{
    if (!m_OutQueue.isEmpty())
    m_OutQueue.pop_front();
    if (!m_OutQueue.isEmpty())
    _PushMsg();
}

void CXMLConnect::onConnectTo()
{
    XMLConnect(m_sIP, m_ushPort, connectToTimeout);
}

void CXMLConnect::_XMLSend(const TXMLBase& pXMLBase)
{
    TVA_CHECK(m_State == enConnected, tr("Connection is not established"), E_FAIL);
    LOG_STR(Log4Qt::Level::TRACE_INT, tr("Outgoing packet:\n%1").arg(pXMLBase->ToString()));
    m_OutQueue.append(pXMLBase);
    if (m_OutQueue.size() == 1)
    _PushMsg();
}

void CXMLConnect::_XMLShutdown()
{
    CConnect::Disconnect();
    m_OutQueue.clear();
    m_CmdOutQueue.clear();
    m_InQueue.clear();

    m_State = enDisconnected;
    connectTimer->stop();
}

void CXMLConnect::_PushMsg()
{
    if (!m_OutQueue.isEmpty())
    {
        QByteArray baBuffer = m_OutQueue.front()->ToByteArray();
        QByteArray baTemp = QByteArray::number(baBuffer.size());
        baTemp += '!';
        CConnect::Send(baTemp + baBuffer);
    }
}
}
