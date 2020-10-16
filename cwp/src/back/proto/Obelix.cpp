#include "Obelix.h"

#include <QStringList>
#include "Master.h"
#include "obelix/CmdHello.h"
#include "obelix/CmdAirplanaCmd.h"
#include "obelix/CmdAirplanaStat.h"
#include "obelix/CmdPut.h"
#include "obelix/CmdTake.h"
#include "obelix/CmdTaken.h"
#include "obelix/CmdAirplanaDel.h"
#include "obelix/CmdReminderAdd.h"
#include "obelix/CmdReminderDel.h"
#include "obelix/CmdMeteoRefresh.h"

#include "obelix/CmdGUIRefresh.h"
#include "obelix/CmdSFPLRefresh.h"
#include "obelix/CmdChangeSFPL.h"
#include "obelix/CmdReminderAction.h"
#include "obelix/CmdSectorChanged.h"
#include "obelix/CmdSetSector.h"
#include "obelix/CmdReminderList.h"
#include "obelix/CmdSectoringAction.h"
#include "obelix/CmdRequestAllFPL.h"
#include "obelix/CmdSendUserZone.h"
#include "obelix/CmdMeteoUpdate.h"
#include "obelix/CmdAirplanaCmdRespond.h"
#include "obelix/CmdStandChanged.h"
#include "obelix/CmdVehicleState.h"

namespace obelix
{
    CConnect::CConnect(CMaster& refMaster)
        : tcp::CXMLConnect(), m_refMaster(refMaster)
    {

    }

    CConnect::CConnect(CMaster& refMaster, QTcpSocket* pSocket, IServer* pServer)
        : tcp::CXMLConnect(pSocket, pServer), m_refMaster(refMaster)
    {

    }

    CXMLCmd::TXMLCmd CConnect::ResolveCmd(const CXMLData::TXMLData& pData)
    {
        TVA_CHECK(
            pData->GetProtoName() == QL1S(XML_PROTO),
            tr("Unknow XML protocol") + QL1S(": ") + pData->GetProtoName(),
            E_FAIL);

        if (pData->GetCmdName() == QL1S(CCmdHello::XML_CMDNAME) && m_pServer != NULL)
            return CXMLCmd::TXMLCmd(new CCmdHelloExe(pData, m_refMaster, *this));
        else if (pData->GetCmdName() == QL1S(CCmdAirplaneCmdExe::XML_CMDNAME) && m_pServer != NULL)
            return CXMLCmd::TXMLCmd(new CCmdAirplaneCmdExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdAirplaneCmdRespondExe::XML_CMDNAME))
            return CXMLCmd::TXMLCmd(new CCmdAirplaneCmdRespondExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdAirplaneStatExe::XML_CMDNAME) )// && m_pServer == NULL)
            return CXMLCmd::TXMLCmd(new CCmdAirplaneStatExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdPutExe::XML_CMDNAME) && m_pServer != NULL)
            return CXMLCmd::TXMLCmd(new CCmdPutExe(pData, m_refMaster, *this));
        else if (pData->GetCmdName() == QL1S(CCmdTakeExe::XML_CMDNAME) )//&& m_pServer == NULL)
            return CXMLCmd::TXMLCmd(new CCmdTakeExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdTakenExe::XML_CMDNAME) && m_pServer != NULL)
            return CXMLCmd::TXMLCmd(new CCmdTakenExe(pData, m_refMaster, *this));
        else if (pData->GetCmdName() == QL1S(CCmdAirplaneDelExe::XML_CMDNAME) && m_pServer == NULL)
            return CXMLCmd::TXMLCmd(new CCmdAirplaneDelExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdReminderAddExe::XML_CMDNAME) )// && m_pServer != NULL)
            return CXMLCmd::TXMLCmd(new CCmdReminderAddExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdReminderDelExe::XML_CMDNAME))
            return CXMLCmd::TXMLCmd(new CCmdReminderDelExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdTransferExe::XML_CMDNAME) )//&& m_pServer != NULL)
            return CXMLCmd::TXMLCmd(new CCmdTransferExe(pData, m_refMaster));

        else if (pData->GetCmdName() == QL1S(CCmdGUIRefreshExe::XML_CMDNAME) )//&& m_pServer != NULL)
            return CXMLCmd::TXMLCmd(new CCmdGUIRefreshExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdSFPLRefreshExe::XML_CMDNAME) )//&& m_pServer != NULL)
            return CXMLCmd::TXMLCmd(new CCmdSFPLRefreshExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdChangeSFPLExe::XML_CMDNAME) && m_pServer != NULL)
            return CXMLCmd::TXMLCmd(new CCmdChangeSFPLExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdReminderActionExe::XML_CMDNAME) && m_pServer != NULL)
            return CXMLCmd::TXMLCmd(new CCmdReminderActionExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdSectorChangedExe::XML_CMDNAME))
            return CXMLCmd::TXMLCmd(new CCmdSectorChangedExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdSetSectorExe::XML_CMDNAME))
            return CXMLCmd::TXMLCmd(new CCmdSetSectorExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdReminderListExe::XML_CMDNAME))
            return CXMLCmd::TXMLCmd(new CCmdReminderListExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdSectoringActionExe::XML_CMDNAME))
            return CXMLCmd::TXMLCmd(new CCmdSectoringActionExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdRequestAllFPLExe::XML_CMDNAME) && m_pServer != NULL)
            return CXMLCmd::TXMLCmd(new CCmdRequestAllFPLExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdMeteoUpdateExe::XML_CMDNAME) && m_pServer != NULL)
            return CXMLCmd::TXMLCmd(new CCmdMeteoUpdateExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdSendUserZoneExe::XML_CMDNAME) && m_pServer != NULL)
            return CXMLCmd::TXMLCmd(new CCmdSendUserZoneExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdStandChanged::XML_CMDNAME)/* && m_pServer != NULL*/)
            return CXMLCmd::TXMLCmd(new CCmdStandChangedExe(pData, m_refMaster));
        else if (pData->GetCmdName() == QL1S(CCmdVehicleState::XML_CMDNAME)/* && m_pServer != NULL*/)
            return CXMLCmd::TXMLCmd(new CCmdVehicleStateExe(pData, m_refMaster));
        else
            TVA_THROW(
                tr("Unknow XML cmd") + QL1S(": ") + pData->GetCmdName(),
                E_FAIL);
    }

    CXMLResp::TXMLResp CConnect::ResolveResp(
            const QString& sCmdName,
            const CXMLData::TXMLData& pData)
    {
        TVA_CHECK(
            pData->GetProtoName() == QL1S(XML_PROTO),
            tr("Unknow XML protocol") + QL1S(": ") + pData->GetProtoName(),
            E_FAIL);

        if (sCmdName == QL1S(CCmdHello::XML_CMDNAME))
            return CXMLResp::TXMLResp(new CRespHello(pData));
        else
            TVA_THROW(
                tr("Unknow XML cmd") + QL1S(": ") + sCmdName,
                E_FAIL);
    }

    void CConnect::OnXMLConnect()
    {
        // client. Send hello
        CXMLCmd::TXMLCmd pCmd(new CCmdHello(GetOperName()));
        XMLSend(pCmd);
    }

    void CConnect::OnXMLDisconnect()
    {
        //if (m_pServer == NULL)
            //m_refMaster.SetOperNames(QStringList());
            //NULL;
        // stop attempts to connect
        XMLDisconnect();
    }

    CServer::CServer(CMaster& refMaster)
        : m_refMaster(refMaster)
    {

    }

    CServer::TConnect CServer::CreateConnect(QTcpSocket* pSocket)
    {
        return TConnect(new CConnect(m_refMaster, pSocket, this));
    }

    void CServer::OnDelete(QTcpSocket* pConnect)
    {
        TConnect pTcpConnect;
        TConnectList::iterator itFindCon = m_Connections.find(pConnect);
        if (itFindCon != m_Connections.end())
            pTcpConnect = *itFindCon;

        tcp::CServer::OnDelete(pConnect);
    }

    void CServer::SendToAllClients(const QSharedPointer<CXMLCmd> &pXMLCmd)
    {
        TOperConnectList::iterator itScan = m_OperConnectList.begin();
        for (; itScan != m_OperConnectList.end(); ++itScan)
            (*itScan)->XMLSend(pXMLCmd);
    }

}
