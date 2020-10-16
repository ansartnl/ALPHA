#ifndef _XT_PROTO_LISTENERS_H__
#define _XT_PROTO_LISTENERS_H__

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtNetwork/QHostAddress>
class QTcpServer;
class QUdpSocket;
class QByteArray;

#include "XtMessage.h"
#include "XtObject.h"
#include "XtParent.h"


//! Namespace contains classes to work with data retrieved from network in internal data format
namespace xtProto
{
    class BaseServer : public QObject, public IListenerParent
    {
    public:
        //! Block for incoming messages.
        void blockMessages(bool on)
        { mIsMessagesBlocked = on; }

        //! Parse byte array.
        void parseProtocol(const QByteArray &ba);

    protected:
        explicit BaseServer(IParent* pParent, QObject *parent=0)
            : QObject(parent), mIsMessagesBlocked(false), mpParent(pParent)
        {}

    protected:
        /** State of blocking incoming messages */
        bool        mIsMessagesBlocked;
        /** Pointer to rls data handler */
        IParent     *mpParent;
    };


    //! Rls TCP server listener.
    class TCPServer : public BaseServer
    {
        Q_OBJECT
    public:
        //! Constructor
        /*!
            \param pRlsParent Pointer to data handler
        */
        explicit TCPServer(IParent* pParent, QObject *parent=0);

        //! Start listening the specified address and port.
        bool listen(const QHostAddress & address = QHostAddress::Any, quint16 port = 0);

        //! Disconnect all connections and stop listening the port.
        void disconnectAll();

        //! Returns a human readable description of the last error that occurred.
        QString errorString() const;

        /*! \name Implementation IListenerParent interface */
        //@{
        //! Start service
        virtual bool Start(const QHostAddress & address = QHostAddress::Any, quint16 port = 0);
        //! Stop service
        virtual void Stop();
        //@}

    protected slots:
        void on_newConnection();
        void on_socketDisconected();
        void on_readyRead();

    private:
        /** Tcp server */
        QTcpServer      *mServer;
    };


    //! Rls UDP socket listener.
    class UDPSocket : public BaseServer
    {
        Q_OBJECT
    public:
        //! Constructor
        /*!
            \param pRlsParent Pointer to data handler
        */
        explicit UDPSocket(IParent* pParent, QObject *parent=0);

        //! Start listening the specified address and port.
        bool bind(const QHostAddress & address = QHostAddress::Any, quint16 port = 0);

        //! Stop listening the port.
        void disconnect();

        //! Returns a human readable description of the last error that occurred.
        QString errorString() const;

        /*! \name Implementation IListenerParent interface */
        //@{
        //! Start service
        virtual bool Start(const QHostAddress & address = QHostAddress::Any, quint16 port = 0);
        //! Stop service
        virtual void Stop();
        //@}

    protected slots:
        void on_readyRead();

    private:
        /** Udp socket */
        QUdpSocket      *mSocket;
    };
}

#endif // _XT_PROTO_LISTENERS_H__
