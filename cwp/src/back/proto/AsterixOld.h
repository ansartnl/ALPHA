#ifndef ASTERIX_OLD_H
#define ASTERIX_OLD_H

#include <QUdpSocket>
#include <QList>
#include <QTime>
#include <QVariant>
#include "common/AirplaneState.h"

//! Namespace contains classes to work with asterix protocol cat.62
namespace asterix
{
	//! Asterix category
	static const quint8 CATEGORY = 62;

	//! Asterix listener
	class CReader :
			public QObject,
			public IAsterixDataSrc
	{
	Q_OBJECT
	public:
		//! Constructor
		/*!
			\param pParent pointer to data handler
			\param qsIP address for listening
			\param quiPort port for listening
		*/
		CReader(IParent* pParent, const QString& qsIP, quint16 quiPort);
		//! Destructor
		virtual ~CReader();
		//! Start listening
		virtual void Start();
		//! Stop listening
		virtual void Stop();
        //! Block messages.
        virtual void BlockMessages(bool on) { m_isMessagesBlocked = on; }
        //! Parse data from buffer to a logical structure
		/*!
			\param Buffer data buffer
			\param Result generated airplane data
		*/
		static bool ParseData(QByteArray& Buffer, CAirplaneState& Result);

	protected slots:
		//! Slot to QUdpSocket::readyRead() signal
		virtual void OnUDPData();

	protected:
		/*! \name Auxiliary functions to parse asterix datagrams */
		//@{
		static void GetAircraftData(QDataStream& qStream, quint16& quiBufferSize);
		static void GetTrackStatus(QDataStream& qStream, quint16& quiBufferSize);
		static void GetSysTrackUpdateAges(QDataStream& qStream, quint16& quiBufferSize);
		static void GetTrackDataAges(QDataStream& qStream, quint16& quiBufferSize);
		//@}
		//! Pointer to data handler
		IParent* m_pParent;
		//! Host name for listen
		QHostAddress m_Host;
		//! Port for listening
		quint16 m_ushPort;
		//! Low-level UDP socket
		QUdpSocket m_Socket;
        //! True if messages should be blocked - not parsed.
        volatile bool m_isMessagesBlocked;
	};

	//! Asterix generator
	class CGenerator : public QObject
	{
	Q_OBJECT
	public:
		//! Constructor
		CGenerator();
		//! Destructor
		virtual ~CGenerator();
		//! Start generator
		/*!
			\param qsIP destination address
			\param quiPort destination port
		*/
		virtual void Start(const QString& qsIP,
				   quint16 quiPort);
		//! Stop generator
		virtual void Stop();
		//! Send asterix datagram
		/*!
			\param AirplaneState airplane data for sending
		*/
		virtual void Send(const CAirplaneState& AirplaneState);
		//! Convert airplane data to raw buffer
		/*!
			\param AirplaneState airplane data
			\param Data row buffer
		*/
		static void CreateData(const CAirplaneState& AirplaneState, QByteArray& Data);

	protected slots:
		//! Slot to QUdpSocket::bytesWritten(qint64) signal
		virtual void OnUDPData(qint64 bytes);

	protected:
		//! Destination host name
		QHostAddress m_Host;
		//! Destination host port
		quint16 m_ushPort;
		//! Low-level UDP socket
		QUdpSocket m_Socket;
		//! Output queue
		QList<QByteArray> m_Queue;
	};

	//! Bearing reader
	class CBearReader : public CReader
	{
	Q_OBJECT
	public:
		//! Constructor
		/*!
			\param pParent pointer to data handler
		*/
		CBearReader(IParent* pParent, const QString& qsIP, quint16 quiPort);
		//! Parse data from buffer to a logical structure
		/*!
			\param Buffer data buffer
			\param Result generated airplane data
		*/
		static bool ParseData(QByteArray& Buffer, CAirplaneState& Result);
	protected slots:
		//! Slot to QUdpSocket::readyRead() signal
		virtual void OnUDPData();
	private:
		IParent* m_pParent;
	};

	//! Bearing generator
	class CBearGenerator : public CGenerator
	{
	Q_OBJECT
	public:
		//! Constructor
		CBearGenerator();
		//! Send bearing datagram
		/*!
			\param AirplaneState airplane data for sending
		*/
		virtual void Send(const CAirplaneState& AirplaneState);
		//! Convert airplane data to raw buffer
		/*!
			\param AirplaneState airplane data
			\param Data row buffer
		*/
		static void CreateData(const CAirplaneState& AirplaneState, QByteArray& Data);
	};

    //! STCA reader
    class CSTCAReader : public CReader
    {
    Q_OBJECT
    public:
        //! Constructor
        /*!
            \param pParent pointer to data handler
        */
        CSTCAReader(IParent* pParent, const QString& qsIP, quint16 quiPort);
        //! Parse data from buffer to a logical structure
        /*!
            \param Buffer data buffer
            \param Result generated airplane data
        */
        bool ParseData(QByteArray& Buffer, QVariant& v);
    protected slots:
        //! Slot to QUdpSocket::readyRead() signal
        virtual void OnUDPData();
    private:
        IParent*    m_pParent;
    };
}

#endif // ASTERIX_OLD_H
