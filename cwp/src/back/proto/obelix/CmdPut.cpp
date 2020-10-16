#include "CmdPut.h"
#include "Common.h"
#include "Master.h"
#include "proto/obelix/CmdTake.h"
#include "CmdReminderList.h"

namespace obelix
{
    const char CCmdPut::XML_CMDNAME[]    = "put";
    const char CCmdPut::XML_TRANSFER[]    = "transfer";
    const char CCmdPut::XML_AIRID[]        = "airplane_id";
    const char CCmdPut::XML_OPERNAME[]    = "operator";

    const char CCmdPut::XML_OLDITYPE[]      = "OLDI_message_type";
    const char CCmdPut::XML_FPLID[] = "fpl_id";

    CCmdPut::CCmdPut(quint32 uiAirID, const QString &sOperName)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DNTransfer = GetDoc().createElement(QL1S(XML_TRANSFER));
        m_DAAirID = GetDoc().createAttribute(QL1S(XML_AIRID));
        m_DAOperName = GetDoc().createAttribute(QL1S(XML_OPERNAME));
        m_DAAirID.setValue(QString::number(uiAirID));
        m_DAOperName.setValue(sOperName);
        m_DNTransfer.setAttributeNode(m_DAAirID);
        m_DNTransfer.setAttributeNode(m_DAOperName);
        GetMainNode().appendChild(m_DNTransfer);
    }    

    CCmdPut::CCmdPut(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DNTransfer = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DNTransfer.isNull() && m_DNTransfer.nodeName() == QL1S(XML_TRANSFER),
            QString(QL1S(XML_TRANSFER) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAAirID = m_DNTransfer.attributeNode(QL1S(XML_AIRID));
        TVA_CHECK(
            !m_DAAirID.isNull(),
            QString(QL1S(XML_AIRID) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
        m_DAOperName = m_DNTransfer.attributeNode(QL1S(XML_OPERNAME));
        TVA_CHECK(
            !m_DAOperName.isNull(),
            QString(QL1S(XML_OPERNAME) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
    }

    QString CCmdPut::GetOperator() const
    {
        return m_DAOperName.value();
    }

    quint32 CCmdPut::GetAirID() const
    {
        quint32 uiID(0);
        bool bOk(false);
        uiID = m_DAAirID.value().toUInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_AIRID) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return uiID;
    }

    CCmdPutExe::CCmdPutExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster,
        obelix::CConnect& connect)
        : CCmdPut(pData), m_refMaster(refMaster),
          m_refConnect(connect)
    {

    }

    CXMLResp::TXMLResp CCmdPutExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            QString sOperName = GetOperator();
            CMaster::TAirplanesMap::iterator itAirFind =
                                            m_refMaster.GetPlanes()->find(GetAirID());
            if(!sOperName.isEmpty())
            {
                bool bSend = false;
                if(CAirplaneData::Transfer == (*itAirFind)->GetPlaneType())
                {
                    if(sOperName != (*itAirFind)->GetTransition())
                    {
                        (*itAirFind)->RemoveReminderByType(CReminder::Transfer);
                        QSharedPointer<obelix::CCmdReminderList> pCmd(new obelix::CCmdReminderList(GetAirID()));
                        QMap<CReminderKey, QSharedPointer<CReminder> >::iterator itRem = (*itAirFind)->GetTempReminders().begin();
                        for(;itRem != (*itAirFind)->GetTempReminders().end();itRem++)
                            pCmd->InsertReminder((*itRem));
                        m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

                        // send MAC to previos
                        QString sClient;
                        TVA_CATCHALL_TRY
                        {
                            sClient = dao()->GetCWPbySector((*itAirFind)->GetTransition());
                        }TVA_CATCHALL(err){
                        LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
                        if(!sClient.isEmpty() && sClient != networkSettings()->value("oldi/name", "Unknown").toString())
                        {
                            OLDIMesDataMAC dataMAC;
                            dataMAC.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                            dataMAC.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                            dataMAC.sTo = sClient;
                            dataMAC.sACID = (*itAirFind)->GetSFPL()->GetACID();

                            QSharedPointer<OLDIMessage> pMesMAC(new OLDIMesMAC(dataMAC));
                            m_refMaster.GetOLDIProc()->SendMessage(pMesMAC);
                        }

                        bSend = true;
                    }
                }

                if(bSend || (CAirplaneData::ASSUME == (*itAirFind)->GetPlaneType()))
                {
                    QString sClient;
                    TVA_CATCHALL_TRY
                    {
                        sClient = dao()->GetCWPbySector(sOperName);
                    }TVA_CATCHALL(err){
                    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
                    if(!sClient.isEmpty() && sClient != networkSettings()->value("oldi/name", "Unknown").toString())
                    {
                        // send ACT to sOperName
                        QSharedPointer<CSFPL> pSFPLtoSend(new CSFPL(*(*itAirFind)->GetSFPL().data()));
                        (*itAirFind)->COPoutToCOPin((*itAirFind)->GetSFPL(), pSFPLtoSend);
                        OLDIMesDataABI dataACT = OLDIMessage::GetABIfromSFPL(pSFPLtoSend);
                        dataACT.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                        dataACT.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                        dataACT.sTo = sClient;

                        QSharedPointer<OLDIMessage> pMesACT(new OLDIMesACT(dataACT));
                        (*itAirFind)->m_listOutMes.append(pMesACT);
                        ObRespond* respond = new ObRespond();

                        respond->SetMaster(&m_refMaster);
                        respond->setParameters(CAirplaneData::Transfer, pMesACT->GetMesData(), (*itAirFind)->GetSFPL()->GetACID(), GetAirID());
                        respond->setOperator(GetOperator());
                        QObject::connect(pMesACT.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), respond, SLOT(MesAnswer(const OLDIMessage*,const QStringList&)));
                        QObject::connect(pMesACT.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), respond, SLOT(MesTimeout(const OLDIMessage*)));
                        m_refMaster.GetOLDIProc()->SendMessage(pMesACT);

                        QSharedPointer<obelix::CCmdTransfer> pCmdTransfer(new obelix::CCmdTransfer(GetAirID()));//, obelix::CCmdTransfer::Transfer_begin));
                        pCmdTransfer->SetPlaneType(CAirplaneData::Transfer);
                        pCmdTransfer->SetTransition(sOperName);
                        pCmdTransfer->SetTransferTime();
                        m_refMaster.GetObServer().SendToAllClients(pCmdTransfer.staticCast<CXMLCmd>());
                    }
                    else
                    {//!!!!!!
                        QSharedPointer<CReminder> pRem(new CReminder(CReminder::Transfer_Com, sOperName));
                        pRem->SetComplete(true);
                        pRem->SetTime(QDateTime::currentDateTime());

                        QSharedPointer<obelix::CCmdReminderList> pCmd(new obelix::CCmdReminderList(GetAirID()));
                        pCmd->InsertReminder(pRem);
                        QMap<CReminderKey, QSharedPointer<CReminder> >::iterator itRem = (*itAirFind)->GetTempReminders().begin();
                        for(;itRem != (*itAirFind)->GetTempReminders().end();itRem++)
                            pCmd->InsertReminder((*itRem));
                        m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                    }
                }
            }
            else
            {
                // TRANSFER OUT
                QSharedPointer<obelix::CCmdTransfer> pCmdTransfer(new obelix::CCmdTransfer(GetAirID()));//, obelix::CCmdTransfer::Transfer_out));
                pCmdTransfer->SetCancelTransfer(false);
                pCmdTransfer->SetOur(false);
                pCmdTransfer->SetPlaneType(CAirplaneData::Transfered);
                pCmdTransfer->SetCurSector("");
                m_refMaster.GetObServer().SendToAllClients(pCmdTransfer.staticCast<CXMLCmd>());


                TVA_CATCHALL_TRY
                {
                    dao()->UpdateSFPL((*itAirFind)->GetSFPL());
                    // may be move in RPL, Pasha?
                }TVA_CATCHALL(err){
                LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
                // send INF
                OLDIMesDataINF dataINF;
                dataINF.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                dataINF.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                dataINF.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
                dataINF.sTypeINF = oldi::INF_ASSUME;
                dataINF.sACID = (*itAirFind)->GetSFPL()->GetACID();
                dataINF.sADEP = (*itAirFind)->GetSFPL()->GetDepAerodrome();
                dataINF.sADES = (*itAirFind)->GetSFPL()->GetDestAerodrome();
                dataINF.sCODE = (*itAirFind)->GetSFPL()->GetCODEtoString();
                dataINF.sSector = " ";
                QSharedPointer<OLDIMessage> pMesINF(new OLDIMesINF(dataINF));
                pMesINF->GetMesSourceData().bAnswer = false;
                m_refMaster.GetOLDIProc()->SendMessage(pMesINF);

                QString sSectors = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
                QStringList sSectorsList = sSectors.split(",", QString::SkipEmptyParts);
                for(int i = 0; i < sSectorsList.size(); ++i)
                {
                    dataINF.sTo = sSectorsList[i];
                    pMesINF = QSharedPointer<OLDIMessage>(new OLDIMesINF(dataINF));
                    pMesINF->GetMesSourceData().bAnswer = false;
                    m_refMaster.GetOLDIProc()->SendMessage(pMesINF);
                }
            }
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }
        return CXMLResp::TXMLResp();
    }
}
