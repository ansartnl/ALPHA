#ifndef XMLCONNECT_H
#define XMLCONNECT_H

#include "Connect.h"
#include "XMLCmd.h"

class QTimer;

namespace tcp {
    //! Implementation of xml connection for client & server
    class CXMLConnect: public CConnect
    {
        Q_OBJECT
        public:
            //! State of the xml connection
            enum State
            {

                enDisconnected = 0, /*!< disconnected state */
                enConnecting, /*!< attempt to establish a connection */
                enConnected
            /*!< connected state */
            };
            //! Smart pointer to CXMLBase
            typedef QSharedPointer<CXMLBase> TXMLBase;
            //! Smart pointer to CXMLCmd
            typedef QSharedPointer<CXMLCmd> TXMLCmd;
            //! Smart pointer to CXMLResp
            typedef QSharedPointer<CXMLResp> TXMLResp;
            //! List of TXMLBase
            typedef QList<TXMLBase> TOutQueue;
            //! Map of TXMLCmd where key contains identificator of the XML packet
            typedef QMap<unsigned int, TXMLCmd> TCmdOutQueue;

            //! Connection constructor for client
            CXMLConnect();
            //! Connection constructor for server
            /*!
             \param pSocket system socket
             \param pServer pointer to tcp server, which created this connection
             */
            CXMLConnect(QTcpSocket* pSocket, IServer* pServer);
            //! Connect to remote server
            /*!
             \param sIP IP address
             \param ushPort port
             \param iMSec timeout between attempts to establish a connection
             */
            void XMLConnect(const QString& sIP, quint16 ushPort, int iMSec = 0);
            //! Disconnect from remote server
            void XMLDisconnect();
            //! Send CXMLCmd data(command)
            /*!
             \param pXMLCmd smart pointer to CXMLCmd
             */
            void XMLSend(const TXMLCmd& pXMLCmd);
            //! Send CXMLResp data(response)
            /*!
             \param pXMLResp smart pointer to CXMLResp
             */
            void XMLSend(const TXMLResp& pXMLResp);
            //! Get current connection state
            /*!
             \return  connection state
             */
            State GetState() const
            {
                return m_State;
            }
            /*! \name Functions for defining a type of a command/response. Should be implemented in descendant class */
            //@{
            //! Resolve a command
            /*!
             \param pData smart poiner to CXMLData
             */
            virtual CXMLCmd::TXMLCmd ResolveCmd(const CXMLData::TXMLData& pData) = 0;
            //! Resolve a response
            /*!
             \param sCmdName response name
             \param pData smart poiner to CXMLData
             */
            virtual CXMLResp::TXMLResp ResolveResp(const QString& sCmdName, const CXMLData::TXMLData& pData) = 0;
            //@}

            /*! \name Events which should be implemented in descendant class */
            //@{
            //! Event of successful connection
            virtual void OnXMLConnect() = 0;
            //! Event of disconnection
            virtual void OnXMLDisconnect() = 0;
            //@}

        private:
            virtual void OnConnect();
            virtual void OnDisconnect();
            virtual void OnRead(QByteArray& Buffer);
            virtual void OnSend();

            Q_SLOT void onConnectTo();
            void _XMLSend(const TXMLBase& pXMLBase);
            void _XMLShutdown();
            void _PushMsg();
            TOutQueue m_OutQueue;
            TCmdOutQueue m_CmdOutQueue;
            QByteArray m_InQueue;
            State m_State;

            QString m_sIP;
            quint16 m_ushPort;
            int connectToTimeout;
            quint32 m_uiID;
            QTimer *connectTimer;
    };
}
#endif // XMLCONNECT_H
