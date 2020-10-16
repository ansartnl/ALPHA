#include "XtListeners.h"

#include "XtGlobal.h"
#include "XtProtocol.h"
#include "main.h"

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QUdpSocket>


namespace xtProto
{
    void BaseServer::parseProtocol(const QByteArray &ba)
    {
        QByteArray buffer = ba;

        while ( buffer.size() > 0 )
        {
            quint32 bytesRead;
            Xt::TMessage message = Xt::CProtocol::Deserialize(buffer, &bytesRead);
            if ( message && message->GetType() == Xt::CDataMessage::GetClassType() )
            {
                QSharedPointer<Xt::CDataMessage> msgData = message.staticCast<Xt::CDataMessage>();
                const Xt::TObject& obj = msgData->GetObject();

                // Empty object received.
                if ( !obj )
                {
                    LOG_STR(Log4Qt::Level::WARN_INT, QObject::tr("Xt Listener: Empty object was transmitted"));
                    return;
                }

                if ( obj->GetVersion() != Xt::CObject::GetClassVersion() )
                {
                    LOG_STR(Log4Qt::Level::ERROR_INT,
                            QObject::tr("Xt Listener: Version of transmitted and supported objects differs.") +
                                        QObject::tr(" Received object version: '%1'. Supported version: '%2'")
                                        .arg(obj->GetVersion()).arg(Xt::CObject::GetClassVersion()));
                    return;
                }

                try
                {
                    try
                    {
                        if ( mpParent )
                                mpParent->OnObjectData(obj);
                    }
                    catch (const Error&)
                    {
                        throw;
                    }
                    catch (...)
                    {
                        throw Error(__FILE__, __LINE__, "catch(...)", E_FAIL);
                    }
                }
                catch (Error& err)
                {
                    LOG_STR(Log4Qt::Level::ERROR_INT, err.reason());
                }
            }

            // Should not happen.
            if ( bytesRead < 1 )
            {
                qDebug() << "BaseServer::parseProtocol. Buffer parsing error.";
                break;
            }
            buffer.remove(0, bytesRead);
        }
    }




    ////////////////////////////////////////////////////////////////////////////
    // TCPServer implementation

    TCPServer::TCPServer(IParent* pParent, QObject *parent)
            : BaseServer(pParent, parent)

    {
        mServer = new QTcpServer(this);
        connect(mServer, SIGNAL(newConnection()), this, SLOT(on_newConnection()));
    }

    bool TCPServer::listen(const QHostAddress & address, quint16 port)
    {
        disconnectAll();

        if ( !mServer->listen(address, port) )
        {
            LOG_STR(Log4Qt::Level::WARN_INT, QObject::tr("TCP Listener: Failed to listen '%1:%2' address"));
            return false;
        }

        LOG_STR(Log4Qt::Level::INFO_INT, QObject::tr("TCP Listener: Listening started for '%1:%2' address")
                .arg(address.toString()).arg(port));

        return true;
    }

    void TCPServer::disconnectAll()
    {
        if ( mServer->isListening() )
            mServer->close();
    }

    QString TCPServer::errorString() const
    {
        return mServer->errorString();
    }

    void TCPServer::on_newConnection()
    {
        if ( !mServer->hasPendingConnections() )
            return;

        QTcpSocket *tcpSocket = mServer->nextPendingConnection();
        connect(tcpSocket, SIGNAL(disconnected()), tcpSocket, SLOT(deleteLater()));
        connect(tcpSocket, SIGNAL(disconnected()), SLOT(on_socketDisconected()));
        connect(tcpSocket, SIGNAL(readyRead()), SLOT(on_readyRead()));

        LOG_STR(Log4Qt::Level::INFO_INT, QObject::tr("TCP Listener: New connection received from '%1:%2' address")
                .arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort()));
    }

    void TCPServer::on_socketDisconected()
    {
        QTcpSocket *tcpSocket = qobject_cast<QTcpSocket *>( sender() );
        Q_ASSERT(tcpSocket);

        LOG_STR(Log4Qt::Level::INFO_INT, QObject::tr("TCP Listener: Sockect '%1:%2' has been disconnected")
                .arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort()));
    }

    void TCPServer::on_readyRead()
    {
        // Only when message is not blocked it will be parsed.
        if ( mIsMessagesBlocked )
            return;

        QTcpSocket *tcpSocket = qobject_cast<QTcpSocket *>( sender() );
        Q_ASSERT(tcpSocket);

        if ( tcpSocket->bytesAvailable() > 0 )
            parseProtocol( tcpSocket->readAll() );
    }

    bool TCPServer::Start(const QHostAddress & address, quint16 port)
    {
        return listen(address, port);
    }

    void TCPServer::Stop()
    {
        disconnectAll();
    }



    ////////////////////////////////////////////////////////////////////////////
    // UDPSocket implementation

    UDPSocket::UDPSocket(IParent* pParent, QObject *parent)
            : BaseServer(pParent, parent)

    {
        mSocket = new QUdpSocket(this);
        connect(mSocket, SIGNAL(readyRead()), SLOT(on_readyRead()));
    }

    bool UDPSocket::bind(const QHostAddress & address, quint16 port)
    {
        if ( mSocket->state() != QAbstractSocket::UnconnectedState )
            mSocket->close();

        if ( !mSocket->bind(address, port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint) )
        {
            LOG_STR(Log4Qt::Level::WARN_INT, QObject::tr("UDP Listener: Failed to bind to '%1:%2' address"));
            return false;
        }

        LOG_STR(Log4Qt::Level::INFO_INT, QObject::tr("UDP Listener: Socket '%1:%2' bound successfully")
                .arg(address.toString()).arg(port));

        return true;
    }

    void UDPSocket::disconnect()
    {
        mSocket->close();
    }

    QString UDPSocket::errorString() const
    {
        return mSocket->errorString();
    }

    void UDPSocket::on_readyRead()
    {
        // Only when message is not blocked it will be parsed.
        if ( mIsMessagesBlocked )
            return;

        while ( mSocket->hasPendingDatagrams() )
        {
             QByteArray datagram;
             datagram.resize(mSocket->pendingDatagramSize());
             QHostAddress sender;
             quint16 senderPort;

             mSocket->readDatagram(datagram.data(), datagram.size(),
                                   &sender, &senderPort);

            parseProtocol(datagram);
        }
    }

    bool UDPSocket::Start(const QHostAddress & address, quint16 port)
    {
        return bind(address, port);
    }

    void UDPSocket::Stop()
    {
        disconnect();
    }

}
