#include "CmdTake.h"
#include "Common.h"
#include "Master.h"
#include "CmdReminderList.h"
//#include "core/Scene.h"
#include "OLDIMessages.h"

#include "CmdTransfer.h"
#include "ObRespond.h"

namespace obelix
{
    const char CCmdTake::XML_CMDNAME[]    = "take";
    const char CCmdTake::XML_TRANSFER[]    = "transfer";
    const char CCmdTake::XML_AIRID[]    = "airplane_id";
    const char CCmdTake::XML_RESET[]    = "reset";

    const char CCmdTake::XML_OLDIMES[] = "OLDI_message_type";

    CCmdTake::CCmdTake(quint32 uiAirID, bool bReset)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DNTransfer = GetDoc().createElement(QL1S(XML_TRANSFER));
        m_DAAirID = GetDoc().createAttribute(QL1S(XML_AIRID));
        m_DAReset = GetDoc().createAttribute(QL1S(XML_RESET));
        m_DAAirID.setValue(QString::number(uiAirID));
        if (bReset)
            m_DAReset.setValue(QL1S("true"));
        else
            m_DAReset.setValue(QL1S("false"));
        m_DNTransfer.setAttributeNode(m_DAAirID);
        m_DNTransfer.setAttributeNode(m_DAReset);
        GetMainNode().appendChild(m_DNTransfer);
    }

    CCmdTake::CCmdTake(quint32 uiAirID)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DNTransfer = GetDoc().createElement(QL1S(XML_TRANSFER));
        m_DAAirID = GetDoc().createAttribute(QL1S(XML_AIRID));
        m_DAAirID.setValue(QString::number(uiAirID));
        m_DNTransfer.setAttributeNode(m_DAAirID);

        GetMainNode().appendChild(m_DNTransfer);
    }


    CCmdTake::CCmdTake(const CXMLData::TXMLData& pData)
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

//        m_DAReset = m_DNTransfer.attributeNode(QL1S(XML_RESET));
//        TVA_CHECK(
//            !m_DAReset.isNull(),
//            QString(QL1S(XML_RESET) + QObject::tr(" attribute in XML command is absent or has wrong name")),
//            E_FAIL);
    }

    quint32 CCmdTake::GetAirID() const
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

    bool CCmdTake::IsReset() const
    {
        QString sValue = m_DAReset.value().toLower() ;
        TVA_CHECK(
            sValue == QL1S("true") ||
            sValue == QL1S("false"),
            QString(QL1S(XML_RESET) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);

        return sValue == QL1S("true");
    }

    QString CCmdTake::GetValue(const QString& sAttrName)
    {
        return m_DNTransfer.attribute(sAttrName);
    }

    CCmdTakeExe::CCmdTakeExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdTake(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdTakeExe::Execute()
    {
        CMaster::TAirplanesMap::iterator itAirFind =
                            m_refMaster.GetPlanes()->find(GetAirID());

        TVA_CATCHALL_TRY
        {
            if(((CAirplaneData::ACT == (*itAirFind)->GetPlaneType()) ||
                (CAirplaneData::Transfered == (*itAirFind)->GetPlaneType())) &&
                    (*itAirFind)->GetTransition().isEmpty())
            {
                // ASSUME
                if((*itAirFind)->GetSFPL()->GetCurSector().isEmpty())
                {
                    if(!networkSettings()->value("users/current_sector", "NS").toString().isEmpty())
                    {
                        OLDIMesDataINF dataINF;
                        dataINF.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                        dataINF.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                        dataINF.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
                        dataINF.sTypeINF = oldi::INF_ASSUME;
                        dataINF.sACID = (*itAirFind)->GetSFPL()->GetACID();
                        dataINF.sADEP = (*itAirFind)->GetSFPL()->GetDepAerodrome();
                        dataINF.sADES = (*itAirFind)->GetSFPL()->GetDestAerodrome();
                        dataINF.sCODE = (*itAirFind)->GetSFPL()->GetCODEtoString();
                        dataINF.sSector = networkSettings()->value("users/current_sector", "NS").toString();
                        QSharedPointer<OLDIMessage> pMesINF(new OLDIMesINF(dataINF));
                        m_refMaster.GetOLDIProc()->SendMessage(pMesINF);

                        QString sSectors = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
                        QStringList sSectorsList = sSectors.split(",", QString::SkipEmptyParts);
                        for(int i = 0; i < sSectorsList.size(); ++i)
                        {
                            dataINF.sTo = sSectorsList[i];
                            pMesINF = QSharedPointer<OLDIMessage>(new OLDIMesINF(dataINF));
                            m_refMaster.GetOLDIProc()->SendMessage(pMesINF);
                        }

                        QSharedPointer<obelix::CCmdTransfer> pCmd(new obelix::CCmdTransfer(GetAirID()));//, obelix::CCmdTransfer::DirectAssume));
                        pCmd->SetDirectAssume();
                        m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                    }
                }
            }

            // MAS
            if(((CAirplaneData::ACT == (*itAirFind)->GetPlaneType()) ||
                (CAirplaneData::Transfered == (*itAirFind)->GetPlaneType())) &&
                    !(*itAirFind)->GetTransition().isEmpty())
            {   //transfer <- "TSW"
                if((*itAirFind)->m_listInMes.size() /*&& !isTransferTimeOut()*/)
                {
                    OLDIMesData lastMesData = (*itAirFind)->m_listInMes.last()->GetMesData();
                    if((oldi::MES_COF == lastMesData.sType) ||
                       (oldi::MES_HOP == lastMesData.sType))
                    {
                        OLDIMesDataMAS dataMAS;
                        dataMAS.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                        dataMAS.sFrom = lastMesData.sTo;
                        dataMAS.sTo = lastMesData.sFrom;
                        dataMAS.sACID = (*itAirFind)->GetSFPL()->GetACID();

                        QSharedPointer<OLDIMessage> pMesMAS(new OLDIMesMAS(dataMAS));
                        (*itAirFind)->m_listOutMes.append(pMesMAS);
                        ObRespond* respond = new ObRespond();
                        respond->SetMaster(&m_refMaster);
                        respond->setParameters((*itAirFind)->GetPlaneType(), pMesMAS->GetMesData(),
                                               (*itAirFind)->GetSFPL()->GetACID(), GetAirID());
                        QObject::connect(pMesMAS.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), respond, SLOT(MesAnswer(const OLDIMessage*,const QStringList&)));
                        QObject::connect(pMesMAS.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), respond, SLOT(MesTimeout(const OLDIMessage*)));

                        m_refMaster.GetOLDIProc()->SendMessage(pMesMAS);
                    }
                }
            }

            // ROF
            if((*itAirFind)->GetTransition().isEmpty() &&
                    !(*itAirFind)->GetSFPL()->GetCurSector().isEmpty())
            {
                if(networkSettings()->value("oldi/disable_ROF", "false").toString() != "true")
                {
                    QString sClient;
                    TVA_CATCHALL_TRY
                    {
                        sClient = dao()->GetCWPbySector((*itAirFind)->GetSFPL()->GetCurSector());
                    }TVA_CATCHALL(err){
                    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
                    if(!sClient.isEmpty() && sClient != networkSettings()->value("oldi/name", "Unknown").toString())
                    {
                        OLDIMesDataROF dataROF;
                        dataROF.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                        dataROF.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                        dataROF.sTo = sClient;
                        dataROF.sACID = (*itAirFind)->GetSFPL()->GetACID();

                        QSharedPointer<OLDIMessage> pMesROF(new OLDIMesROF(dataROF));
                        (*itAirFind)->m_listOutMes.append(pMesROF);

                        ObRespond* respond = new ObRespond();
                        respond->SetMaster(&m_refMaster);
                        respond->setParameters((*itAirFind)->GetPlaneType(), pMesROF->GetMesData(),
                                               (*itAirFind)->GetSFPL()->GetACID(), GetAirID());
                        QObject::connect(pMesROF.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), respond, SLOT(MesAnswer(const OLDIMessage*,const QStringList&)));
                        QObject::connect(pMesROF.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), respond, SLOT(MesTimeout(const OLDIMessage*)));

                        m_refMaster.GetOLDIProc()->SendMessage(pMesROF);

                        //client

                        QSharedPointer<obelix::CCmdTransfer> pCmd(new obelix::CCmdTransfer(GetAirID()));
                        pCmd->SetTransition((*itAirFind)->GetSFPL()->GetCurSector());
                        pCmd->SetTransferTime();
                        m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

                        (*itAirFind)->RemoveReminderByType(CReminder::TrHOP);
                        QSharedPointer<obelix::CCmdReminderList> pCmdList(new obelix::CCmdReminderList(GetAirID()));
                        QMap<CReminderKey, QSharedPointer<CReminder> >::iterator itRem = (*itAirFind)->GetTempReminders().begin();
                        for(;itRem != (*itAirFind)->GetTempReminders().end();itRem++)
                            pCmdList->InsertReminder((*itRem));
                        m_refMaster.GetObServer().SendToAllClients(pCmdList.staticCast<CXMLCmd>());
                    }
                    else
                    {
                        //client
                        QSharedPointer<CReminder> pRem(new CReminder(CReminder::TimeOutROF, (*itAirFind)->GetSFPL()->GetCurSector()));
                        pRem->SetLevel(CReminder::Warning);
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
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }

        return CXMLResp::TXMLResp();
    }
}

