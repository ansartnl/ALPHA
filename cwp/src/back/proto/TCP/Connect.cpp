#include "Connect.h"

#include "TVAError.h"
#include "main.h"

#include <QtNetwork/QHostAddress>

namespace tcp
{
    CConnect::CConnect()
        : m_pServer(NULL), m_pSocket(NULL)
    {
        m_pSocket = new QTcpSocket(this);
        _ConnectSlots();
    }

    CConnect::CConnect(QTcpSocket* pSocket, IServer* pServer)
        : m_pServer(pServer), m_pSocket(pSocket)
    {
        LOG_STR(Log4Qt::Level::INFO_INT, tr("Connection is established with: %1:%2").
                arg(m_pSocket->peerAddress().toString()).arg(m_pSocket->peerPort()));
        _ConnectSlots();
    }

    void CConnect::Connect(const QString& sIP, quint16 ushPort)
    {
        Disconnect();
        LOG_STR(Log4Qt::Level::INFO_INT, tr("Attempt to connect to: %1:%2").arg(sIP).arg(ushPort));
        m_pSocket->connectToHost(sIP, ushPort);
    }

    void CConnect::Disconnect()
    {
        if (m_pSocket->state() != QAbstractSocket::UnconnectedState)
        {
            if (m_pServer)
            {
                LOG_STR(Log4Qt::Level::INFO_INT, tr("Disconnection from client"));
            } else
            {
                LOG_STR(Log4Qt::Level::INFO_INT, tr("Disconnection from server"));
            }
            m_pSocket->abort();
        }
    }

    void CConnect::Send(const QByteArray& Buffer)
    {
        if (m_BufferToSend.isEmpty())
        {
            TVA_CHECK(
                m_pSocket->write(Buffer) > -1,
                QString(tr("Unable to send data over TCP. Reason") + ": %1").arg(m_pSocket->errorString()),
                E_FAIL);
        }
        m_BufferToSend.append(Buffer);
    }

    void CConnect::SlotConnect()
    {
        LOG_STR(Log4Qt::Level::INFO_INT, tr("Connection is successfully established"));
        TVA_CATCHALL_TRY
        {
            OnConnect();
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
            ShutDown();
        }
    }

    void CConnect::SlotDisconnect()
    {
        LOG_STR(Log4Qt::Level::WARN_INT, tr("Connection is closed"));
        TVA_CATCHALL_TRY
        {
            OnDisconnect();
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
        }
        ShutDown();
    }

    void CConnect::SlotError(QAbstractSocket::SocketError socketError)
    {
        LOG_STR(Log4Qt::Level::ERROR_INT, QString(tr("Connection error. Description") + ": %1").arg(m_pSocket->errorString()));
        SlotDisconnect();
    }

    void CConnect::SlotWriteData(qint64 llSize)
    {
        TVA_CATCHALL_TRY
        {
            if (m_BufferToSend.size() < llSize)
                m_BufferToSend = m_BufferToSend.mid(llSize);
            else
                m_BufferToSend.clear();

            if (!m_BufferToSend.isEmpty())
                m_pSocket->write(m_BufferToSend);
            else
                OnSend();
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
            ShutDown();
        }
    }

    void CConnect::SlotReadData()
    {
        QByteArray Buffer = m_pSocket->readAll();
        TVA_CATCHALL_TRY
        {
            if (!Buffer.isEmpty())
                OnRead(Buffer);
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
            TVA_CATCHALL_TRY
            {
                ShutDown();
            } TVA_TRAPALL{}
        }
    }

    void CConnect::_ConnectSlots()
    {
        connect(m_pSocket, SIGNAL(connected()), this, SLOT(SlotConnect()));
        connect(m_pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotError(QAbstractSocket::SocketError)));
        connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(SlotReadData()));
        connect(m_pSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(SlotWriteData(qint64)));
    }

    void CConnect::ShutDown()
    {
        TVA_CATCHALL_TRY
        {
            Disconnect();
            if (m_pServer)
                m_pServer->OnDelete(m_pSocket);
        } TVA_TRAPALL{}
    }
}
