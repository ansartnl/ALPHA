#ifndef SERVER_H
#define SERVER_H

#include <QtCore/QSharedPointer>

#include <QtNetwork/QTcpServer>

#include "Connect.h"

namespace tcp
{
	//! Implementation of tcp server
	class CServer
		:  public QObject,
		public CConnect::IServer
	{
		Q_OBJECT
	public:
		//! Smart pointer to connection
		typedef QSharedPointer<CConnect> TConnect;
		//! Map with connections
		typedef QMap<QTcpSocket*, TConnect> TConnectList;
		//! Destructor
		virtual ~CServer() {};
		//! Start TCP server
		/*!
			\param qsIP IP adrress for listening
			\param ushPort port for listening
		*/
		virtual void Start(
			const QString& qsIP,
			quint16 ushPort);
		//! Stop TCP server
		virtual void Stop();
		//! Get connection map
		/*!
			\return connection map
		*/
		TConnectList& GetConnections() { return m_Connections; }
		//! Connection factory
		/*!
			\param pSocket low-level socket
		*/
		virtual TConnect CreateConnect(QTcpSocket* pSocket) = 0;
		/*! \name Implementation of CConnect::IServer interface */
		//@{
		/*!
			\param pConnect low-level socket
		*/
		virtual void OnDelete(QTcpSocket* pConnect);
		//@}

	protected slots:
		/*! Slot to QTcpServer signal */
		virtual void SlotConnect();

	protected:
		/*! Low level tcp server */
		QTcpServer		m_Server;
		/*! Connection map */
		TConnectList	m_Connections;
	};
}

#endif // SERVER_H
