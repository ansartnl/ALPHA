#include "CmdHello.h"
#include "Common.h"
#include "proto/Obelix.h"
#include "Master.h"
#include "XMasterWindow.h"
#include "CmdSetSector.h"

namespace obelix
{
    const char CCmdHello::XML_CMDNAME[] = "hello";
    const char CCmdHello::XML_OPER[] = "operator";
    const char CCmdHello::XML_OPERNAME[] = "name";

    CCmdHello::CCmdHello(const QString& sOperName)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));

        m_DNOper = GetDoc().createElement(QL1S(XML_OPER));
        m_DAOperName = GetDoc().createAttribute(QL1S(XML_OPERNAME));
        m_DAOperName.setValue(sOperName);
        m_DNOper.setAttributeNode(m_DAOperName);
        GetMainNode().appendChild(m_DNOper);
    }

    CCmdHello::CCmdHello(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DNOper = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DNOper.isNull() && m_DNOper.nodeName() == QL1S(XML_OPER),
            QString(QL1S(XML_OPER) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAOperName = m_DNOper.attributeNode(QL1S(XML_OPERNAME));
        TVA_CHECK(
            !m_DAOperName.isNull(),
            QString(QL1S(XML_OPERNAME) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
    }

    QString CCmdHello::GetOperName() const
    {
        return m_DAOperName.value();
    }

    void CCmdHello::Answer(CXMLResp::TXMLResp& pResp)
    {
        //LOG_STR(CLog::enDebug, (QObject::tr("Response for command") + QL1S(": %1")).arg(QL1S(XML_CMDNAME)));
        // throw & close connection
        TVA_CHECK(
            pResp->GetCode() == XMLERR_OK,
            (QObject::tr("Negative answer") + QL1S(". %1")).arg(pResp->GetDescr()),
            E_FAIL);
    }

    CCmdHelloExe::CCmdHelloExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster,
        obelix::CConnect& refConnect)
        : CCmdHello(pData), m_refMaster(refMaster), m_refConnect(refConnect), m_bSendList(false)
    {

    }

    CCmdHelloExe::~CCmdHelloExe()
    {
//        TVA_CATCHALL_TRY
//        {
//            if (m_bSendList)
//                m_refMaster.GetObServer().SendOperatorList();
//        } TVA_TRAPALL{}
    }

    CXMLResp::TXMLResp CCmdHelloExe::Execute()
    {
        CXMLResp::TXMLResp pResp;
        TVA_CATCHALL_TRY
        {
            //LOG_STR(CLog::enDebug, (QObject::tr("Execution of command") + QL1S(": %1")).arg(QL1S(XML_CMDNAME)));
            pResp = CXMLResp::TXMLResp(new CRespHello(*this));
            QString peer = m_refConnect.GetSocket()->peerAddress().toString() + ":" +
                    QString::number(m_refConnect.GetSocket()->peerPort());    //peer identificator (address:port)
            obelix::CServer::TOperConnectList::iterator itFind =
                m_refMaster.GetObServer().GetOperConnections().find(peer);
            if (itFind == m_refMaster.GetObServer().GetOperConnections().end())
            {
                tcp::CServer::TConnectList::iterator itConFind =
                        m_refMaster.GetObServer().GetConnections().find(m_refConnect.GetSocket());
                TVA_CHECK(
                        itConFind != m_refMaster.GetObServer().GetConnections().end(),
                        QObject::tr("Inner error"),
                        E_FAIL);


                obelix::CServer::TOperConnect pConnect = itConFind->staticCast<obelix::CConnect>();
                pConnect->SetOperName(GetOperName());
                m_refMaster.GetObServer().GetOperConnections()[peer] = pConnect;

                QSharedPointer<CCmdSetSector> pCmd(new obelix::CCmdSetSector(QString("null")));
                pCmd->SetSectorName(static_cast<XMasterWindow *>(getMainWindow())->GetCurrentSector());
                m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

            } else
            {
                pResp->SetCode(XMLERR_ERROR);
                pResp->SetDescr(QObject::tr("Connection already exists with such operator name"));
            }
        } TVA_CATCHALL(err)
        {
			Q_UNUSED(err)
            //LOG_TVAERROR(CLog::enError, err)
        }
        
        return pResp;
    }

    CRespHello::CRespHello(CCmdHello& refCmd)
        : CXMLResp(QL1S(XML_PROTO), refCmd)
    {

    }

    CRespHello::CRespHello(const CXMLData::TXMLData& pData)
        : CXMLResp(pData)
    {

    }
}
