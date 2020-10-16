#ifndef CONNECT_H
#define CONNECT_H

#include <QtNetwork/QTcpSocket>

//! Namespace contains classes to work with tcp socket
namespace tcp
{
	//! Implementation of tcp connection for client & server
	class CConnect
		: public QObject
	{
		Q_OBJECT
	public:
		//! Interface to tcp server
		class IServer
		{
		public:
			//! Destructor
			virtual ~IServer() {};
			//! Notification about socket destruction
			/*!
				\param pConnect low-level socket
			*/
			virtual void OnDelete(QTcpSocket* pConnect) = 0;
		};

		//! Connection constructor for tcp client
		CConnect();
		//! Connection constructor for tcp server
		/*!
			\param pSocket system socket
			\param pServer pointer to tcp server, which created this connection
		*/
		CConnect(QTcpSocket* pSocket, IServer* pServer);
		//! Destructor
		virtual ~CConnect() {};
		//! Connect to remote server
		/*!
			\param sIP IP address
			\param ushPort port
		*/
		void Connect(const QString& sIP, quint16 ushPort);
		//! Disconnect from remote server
		void Disconnect();
		//! Send data array over tcp connection
		/*!
			\param Buffer buffer to send
		*/
		void Send(const QByteArray& Buffer);
		//! Get low-level socket
		/*!
			\return Buffer buffer to send
		*/
		QTcpSocket* GetSocket() { return m_pSocket; }

		/*! \name Events which should be implemented in descendant class */
		//@{
		//! Event of successful connection
		virtual void OnConnect() = 0;
		//! Event of disconnection
		virtual void OnDisconnect() = 0;
		//! Event of data coming
		/*!
			\param Buffer received data from socket
		*/
		virtual void OnRead(QByteArray& Buffer) = 0;
		//! Event of data sending
		virtual void OnSend() = 0;
		//@}

	protected slots:
		/*! \name Slots to QTcpSocket signals*/
		//@{
		void SlotConnect();
		void SlotDisconnect();
		void SlotError(QAbstractSocket::SocketError socketError);
		void SlotWriteData(qint64 llSize);
		void SlotReadData();
		//@}

	protected:
		//! Connect signals to slots
		void _ConnectSlots();
		//! Shutdown the socket
		void ShutDown();
		//! Pointer to tcp server. If this connection belong to tcp client, than null
		IServer*	m_pServer;
		//! Low level socket
		QTcpSocket* m_pSocket;
		//! Output data buffer
		QByteArray	m_BufferToSend;
	};
}

#endif // CONNECT_H
