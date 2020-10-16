#ifndef OBELIX_H
#define OBELIX_H

#include "TCP/Server.h"
#include "TCP/XMLConnect.h"
#include "obelix/Common.h"

#include "TVAMacro.h"
#include "TVAError.h"
#include "TVAUtils_global.h"

class CMaster;
namespace obelix
{
    //! Implementation of obelix connection for client & server
    class CConnect
        : public tcp::CXMLConnect
    {
    Q_OBJECT
    public:
        //! Map of airplane count(how many times airplane was subscribed). Key - airplane identifier
        typedef QMap<quint32, int>    TAirplaneList;
        //! Connection constructor for client
        explicit CConnect(CMaster& refMaster);
        //! Connection constructor for server
        /*!
            \param refMaster reference to CMaster
            \param pSocket system socket
            \param pServer pointer to server, which created this connection
        */
        CConnect(CMaster& refMaster, QTcpSocket* pSocket, IServer* pServer);

        /*! \name Implementation of tcp::CXMLConnect interface */
        //@{
        virtual CXMLCmd::TXMLCmd ResolveCmd(const CXMLData::TXMLData& pData);
        virtual CXMLResp::TXMLResp ResolveResp(
                const QString& sCmdName,
                const CXMLData::TXMLData& pData);

        virtual void OnXMLConnect();
        virtual void OnXMLDisconnect();
        //@}

        //! Get airplane count map
        /*!
            \return  airplane count map
        */
        TAirplaneList& GetAirplaneList() { return m_AirplaneList; }

        //! Operator name, which uses this connection
        TVA_PROPERTIE(QString, OperName)

    private:
        CMaster&        m_refMaster;
        TAirplaneList    m_AirplaneList;
    };

    //! Implementation of obelix server
    class CServer
        : public tcp::CServer
    {
    public:
        //! Smart pointer to obelix connection
        typedef QSharedPointer<obelix::CConnect> TOperConnect;
        //! Map of obelix connections. Key - operator name of a connection
        typedef QMap<QString, TOperConnect> TOperConnectList;

        //! Constructor
        /*!
            \param refMaster reference to CMaster
        */
        explicit CServer(CMaster& refMaster);
        //! Implementation of connection factory
        virtual TConnect CreateConnect(QTcpSocket* pSocket);
        //! Get connection map
        /*!
            \return    connection map
        */
        TOperConnectList& GetOperConnections() { return m_OperConnectList; }

        /*! \name Implementation of CConnect::IServer interface */
        //@{
        /*!
            \param pConnect low-level socket
        */
        virtual void OnDelete(QTcpSocket* pConnect);
        //@}

        //! Sends message to all connected clients
        void SendToAllClients(const QSharedPointer<CXMLCmd>& pXMLCmd);

    private:
        CMaster&            m_refMaster;
        TOperConnectList    m_OperConnectList;
    };
}

#endif // OBELIX_H
