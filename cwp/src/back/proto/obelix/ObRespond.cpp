#include "ObRespond.h"
#include "Master.h"
#include "CmdReminderList.h"

ObRespond::ObRespond(QObject *parent) :
    QObject(parent), m_pMaster(NULL)
{
}

void ObRespond::setParameters(CAirplaneData::PlaneType type, OLDIMesData lastMes, QString sACID, quint16 id)
{
    planeType = type;
    lastMesData = lastMes;
    ACID = sACID;
    m_ID = id;
}

void ObRespond::MesAnswer(const OLDIMessage *pMes, const QStringList &err)
{    
    TVA_CATCHALL_TRY
    {
        if(CAirplaneData::Transfer == planeType)
        {   //transfer -> "TSW"
            {
                if(pMes->GetMesData() == lastMesData)
                {
                    CMaster::TAirplanesMap::iterator itAirFind =
                                        m_pMaster->GetPlanes()->find(m_ID);
                    if (itAirFind != m_pMaster->GetPlanes()->end())
                    {
                        if(oldi::MES_ACT == lastMesData.sType)
                        {
                            OLDIMesDataHOP dataHOP;
                            dataHOP.iMesNum = m_pMaster->GetOLDIProc()->GetNextMesNum();
                            dataHOP.sFrom = lastMesData.sFrom;
                            dataHOP.sTo = lastMesData.sTo;
                            dataHOP.sACID = ACID;

                            QSharedPointer<OLDIMessage> pMesHOP(new OLDIMesHOP(dataHOP));
                            ObRespond* respond = new ObRespond();
                            respond->SetMaster(m_pMaster);
                            respond->setParameters(planeType, pMesHOP->GetMesData(), ACID, m_ID);
                            respond->setOperator(sOperator);
                            connect(pMesHOP.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), respond, SLOT(MesAnswer(const OLDIMessage*,const QStringList&)));
                            connect(pMesHOP.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), respond, SLOT(MesTimeout(const OLDIMessage*)));
                            m_pMaster->GetOLDIProc()->SendMessage(pMesHOP);

                            QSharedPointer<obelix::CCmdTransfer> pCmd(new obelix::CCmdTransfer(m_ID));//, obelix::CCmdTransfer::Transfer));
                            pCmd->SetTransferTime();
                            m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                        }
                        if(oldi::MES_COF == lastMesData.sType)
                        {
                            (*itAirFind)->RemoveReminderByType(CReminder::Transfer);
                            (*itAirFind)->RemoveReminderByType(CReminder::TrROF);
                            QSharedPointer<obelix::CCmdReminderList> pCmd(new obelix::CCmdReminderList(m_ID));
                            QMap<CReminderKey, QSharedPointer<CReminder> >::iterator itRem = (*itAirFind)->GetTempReminders().begin();
                            for(;itRem != (*itAirFind)->GetTempReminders().end();itRem++)
                                pCmd->InsertReminder((*itRem));
                            m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

                            QSharedPointer<obelix::CCmdTransfer> pCmdTransfer(new obelix::CCmdTransfer(m_ID));//, obelix::CCmdTransfer::Transfer));
                            pCmdTransfer->SetTransferTime();
                            m_pMaster->GetObServer().SendToAllClients(pCmdTransfer.staticCast<CXMLCmd>());
                        }
                        if(oldi::MES_HOP == lastMesData.sType)
                        {
                            QSharedPointer<obelix::CCmdTransfer> pCmdTransfer(new obelix::CCmdTransfer(m_ID));//, obelix::CCmdTransfer::Transfer));
                            pCmdTransfer->SetTransferTime();
                            m_pMaster->GetObServer().SendToAllClients(pCmdTransfer.staticCast<CXMLCmd>());

                            QSharedPointer<CReminder> pRem(new CReminder(CReminder::Transfer, sOperator));
                            pRem->SetLevel(CReminder::Warning);
                            pRem->SetComplete(true);
                            pRem->SetTime(QDateTime::currentDateTime());

                            QSharedPointer<obelix::CCmdReminderList> pCmd(new obelix::CCmdReminderList(m_ID));
                            pCmd->InsertReminder(pRem);
                            QMap<CReminderKey, QSharedPointer<CReminder> >::iterator itRem = (*itAirFind)->GetTempReminders().begin();
                            for(;itRem != (*itAirFind)->GetTempReminders().end();itRem++)
                                pCmd->InsertReminder((*itRem));
                            m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

                        }
                    }
                    return;
                }
            }
        }

        if(CAirplaneData::ACT == planeType || CAirplaneData::Transfered == planeType)
        {   //transfer <- "TSW"
            if(pMes->GetMesData() == lastMesData)
            {
                if(oldi::MES_MAS == lastMesData.sType)
                {
                    QSharedPointer<obelix::CCmdTransfer> pCmd(new obelix::CCmdTransfer(m_ID));//, obelix::CCmdTransfer::Assume));
                    pCmd->SetCancelTransfer(true);
                    pCmd->SetCurSector("");
                    pCmd->SetPlaneType(CAirplaneData::ACT);
                    pCmd->SetAssumeOperator();
                    m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                }
                if(oldi::MES_ROF == lastMesData.sType)
                {
                    QSharedPointer<obelix::CCmdTransfer> pCmd(new obelix::CCmdTransfer(m_ID));//, obelix::CCmdTransfer::Transfer));
                    pCmd->SetTransferTime();
                    m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                }
                return;
            }
        }
        if(CAirplaneData::ASSUME == planeType)
        {
            if(pMes->GetMesData() == lastMesData)
            {
                if(oldi::MES_MAS == lastMesData.sType)
                {
                    QSharedPointer<obelix::CCmdTransfer> pCmd(new obelix::CCmdTransfer(m_ID));//, obelix::CCmdTransfer::Transition));
                    pCmd->SetTransition("");
                    m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                }
                return;
            }

        }
    } TVA_CATCHALL(err)
    {
        LOG_TVAERROR(ERROR, err)
    }
    delete this;
}

void ObRespond::MesTimeout(const OLDIMessage *pMes)
{
    TVA_CATCHALL_TRY
    {
        if(CAirplaneData::Transfer == planeType)
        {
            if(pMes->GetMesData() == lastMesData)
            {
                if((oldi::MES_ACT == lastMesData.sType)
                    || (oldi::MES_COF == lastMesData.sType)
                    || (oldi::MES_HOP == lastMesData.sType))
                    {
                    QSharedPointer<obelix::CCmdTransfer> pCmd(new obelix::CCmdTransfer(m_ID));//, obelix::CCmdTransfer::Cancel));
                    pCmd->SetCancelTransfer(true);
                    m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                }
                return;
            }
        }
        if(CAirplaneData::ACT == planeType || CAirplaneData::Transfered == planeType)
        {
            if(pMes->GetMesData() == lastMesData)
            {
                if(oldi::MES_MAS == lastMesData.sType)
                {
                    QSharedPointer<obelix::CCmdTransfer> pCmd(new obelix::CCmdTransfer(m_ID));//, obelix::CCmdTransfer::Cancel));
                    pCmd->SetCancelTransfer(true);
                    m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                }
                if(oldi::MES_ROF == lastMesData.sType)
                {
                    CMaster::TAirplanesMap::iterator itAirFind =
                                        m_pMaster->GetPlanes()->find(m_ID);
                    if (itAirFind != m_pMaster->GetPlanes()->end())
                    {
                        QSharedPointer<CReminder> pRem(new CReminder(CReminder::Transfer, sOperator));
                        pRem->SetLevel(CReminder::Warning);
                        pRem->SetComplete(true);

                        QSharedPointer<obelix::CCmdReminderList> pCmd(new obelix::CCmdReminderList(m_ID));
                        pCmd->InsertReminder(pRem);
                        QMap<CReminderKey, QSharedPointer<CReminder> >::iterator itRem = (*itAirFind)->GetTempReminders().begin();
                        for(;itRem != (*itAirFind)->GetTempReminders().end();itRem++)
                            pCmd->InsertReminder((*itRem));
                        m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                    }
                }
                return;
            }
        }
    } TVA_CATCHALL(err)
    {
        LOG_TVAERROR(ERROR, err)
    }
}
