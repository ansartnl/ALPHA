#include "CmdReminderAction.h"

#include "CmdReminderAdd.h"
#include "CmdReminderList.h"
#include "AirplaneData.h"
#include "Common.h"
#include "Master.h"
#include "OLDIMessages.h"

namespace obelix
{
    const char CCmdReminderAction::XML_CMDNAME[]    = "reminder_action";
    const char CCmdReminderAction::XML_REMINDER[]    = "reminder";
    const char CCmdReminderAction::XML_AIRID[]        = "airplane_id";
    const char CCmdReminderAction::XML_REMTYPE[]    = "type";
    const char CCmdReminderAction::XML_REMVALUE[]    = "value";

    const char CCmdReminderAction::XML_REMLEVEL[] = "level";
    const char CCmdReminderAction::XML_REMCOMPLETE[] = "is_complete";
    const char CCmdReminderAction::XML_REMACTION[] = "action";

    const char CCmdReminderAction::XML_REMTIME[]        = "rem_time";
    const char CCmdReminderAction::XML_REMCLOSABLE[]    = "is_closable";

    CCmdReminderAction::CCmdReminderAction(
            quint32 uiAirId,
            const QSharedPointer<CReminder>& pReminder, RemAction action)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DEReminder = GetDoc().createElement(QL1S(XML_REMINDER));
        m_DAAirID = GetDoc().createAttribute(QL1S(XML_AIRID));
        m_DAAirID.setValue(QString::number(uiAirId));
        m_DARemType = GetDoc().createAttribute(QL1S(XML_REMTYPE));
        m_DARemType.setValue(QString::number((int)pReminder->GetType()));
        m_DARemValue = GetDoc().createAttribute(QL1S(XML_REMVALUE));
        m_DARemValue.setValue(pReminder->GetValue());
        m_DARemAction = GetDoc().createAttribute(QL1S(XML_REMACTION));
        m_DARemAction.setValue(QString::number((quint32)action));
        m_DARemLevel = GetDoc().createAttribute(QL1S(XML_REMLEVEL));
        m_DARemLevel.setValue(QString::number((quint32)pReminder->GetLevel()));
        m_DARemComplete = GetDoc().createAttribute(QL1S(XML_REMCOMPLETE));
        m_DARemComplete.setValue((pReminder->GetComplete()?QL1S("true"):QL1S("false")));
        m_DARemTime = GetDoc().createAttribute(QL1S(XML_REMTIME));
        m_DARemTime.setValue(pReminder->GetTime().toString("dd.MM.yyyy hh:mm:ss.zzz"));
        m_DARemClosable = GetDoc().createAttribute(QL1S(XML_REMCLOSABLE));
        m_DARemClosable.setValue((pReminder->IsClosable()?QL1S("true"):QL1S("false")));

        m_DEReminder.setAttributeNode(m_DAAirID);
        m_DEReminder.setAttributeNode(m_DARemType);
        m_DEReminder.setAttributeNode(m_DARemValue);
        m_DEReminder.setAttributeNode(m_DARemAction);
        m_DEReminder.setAttributeNode(m_DARemLevel);
        m_DEReminder.setAttributeNode(m_DARemComplete);
        m_DEReminder.setAttributeNode(m_DARemTime);
        m_DEReminder.setAttributeNode(m_DARemClosable);

        GetMainNode().appendChild(m_DEReminder);
    }

    CCmdReminderAction::CCmdReminderAction(
            quint32 uiAirId,
            CReminder::ReminderType type, CCmdReminderAction::RemAction action)
    : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DEReminder = GetDoc().createElement(QL1S(XML_REMINDER));
        m_DAAirID = GetDoc().createAttribute(QL1S(XML_AIRID));
        m_DAAirID.setValue(QString::number(uiAirId));
        m_DARemType = GetDoc().createAttribute(QL1S(XML_REMTYPE));
        m_DARemType.setValue(QString::number((int)type));
        m_DARemValue = GetDoc().createAttribute(QL1S(XML_REMVALUE));
        m_DARemValue.setValue("null");
        m_DARemAction = GetDoc().createAttribute(QL1S(XML_REMACTION));
        m_DARemAction.setValue(QString::number((quint32)action));
        m_DARemLevel = GetDoc().createAttribute(QL1S(XML_REMLEVEL));
        m_DARemLevel.setValue(QString::number(0));
        m_DARemComplete = GetDoc().createAttribute(QL1S(XML_REMCOMPLETE));
        m_DARemComplete.setValue(QL1S("false"));
        m_DARemTime = GetDoc().createAttribute(QL1S(XML_REMTIME));
        m_DARemTime.setValue(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz"));
        m_DARemClosable = GetDoc().createAttribute(QL1S(XML_REMCLOSABLE));
        m_DARemClosable.setValue(QL1S("false"));

        m_DEReminder.setAttributeNode(m_DAAirID);
        m_DEReminder.setAttributeNode(m_DARemType);
        m_DEReminder.setAttributeNode(m_DARemValue);
        m_DEReminder.setAttributeNode(m_DARemAction);
        m_DEReminder.setAttributeNode(m_DARemLevel);
        m_DEReminder.setAttributeNode(m_DARemComplete);
        m_DEReminder.setAttributeNode(m_DARemTime);
        m_DEReminder.setAttributeNode(m_DARemClosable);

        GetMainNode().appendChild(m_DEReminder);
    }

    CCmdReminderAction::CCmdReminderAction(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DEReminder = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DEReminder.isNull() && m_DEReminder.nodeName() == QL1S(XML_REMINDER),
            QString(QL1S(XML_REMINDER) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAAirID = m_DEReminder.attributeNode(QL1S(XML_AIRID));
        TVA_CHECK(
            !m_DAAirID.isNull(),
            QString(QL1S(XML_AIRID) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DARemType = m_DEReminder.attributeNode(QL1S(XML_REMTYPE));
        TVA_CHECK(
            !m_DARemType.isNull(),
            QString(QL1S(XML_REMTYPE) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DARemValue = m_DEReminder.attributeNode(QL1S(XML_REMVALUE));
        TVA_CHECK(
            !m_DARemValue.isNull(),
            QString(QL1S(XML_REMVALUE) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DARemAction = m_DEReminder.attributeNode(QL1S(XML_REMACTION));
        TVA_CHECK(
            !m_DARemAction.isNull(),
            QString(QL1S(XML_REMACTION) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DARemComplete = m_DEReminder.attributeNode(QL1S(XML_REMCOMPLETE));
        TVA_CHECK(
            !m_DARemComplete.isNull(),
            QString(QL1S(XML_REMCOMPLETE) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DARemTime = m_DEReminder.attributeNode(QL1S(XML_REMTIME));
        TVA_CHECK(
            !m_DARemTime.isNull(),
            QString(QL1S(XML_REMTIME) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DARemClosable = m_DEReminder.attributeNode(QL1S(XML_REMCLOSABLE));
        TVA_CHECK(
            !m_DARemClosable.isNull(),
            QString(QL1S(XML_REMCLOSABLE) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
    }

    quint32 CCmdReminderAction::GetAirID() const
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

    CReminder::ReminderType CCmdReminderAction::GetRemType() const
    {
        int iType(-1);
        bool bOk(false);
        iType = m_DARemType.value().toInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_REMTYPE) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return (CReminder::ReminderType)iType;
    }

    QString CCmdReminderAction::GetRemValue() const
    {
        return m_DARemValue.value();
    }

    CReminder::ReminderLevel CCmdReminderAction::GetLevel() const
    {
        bool bOk(false);
        qint32 iType = m_DEReminder.attribute(QL1S(XML_REMLEVEL), "-1").toInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_REMLEVEL) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return (CReminder::ReminderLevel)iType;
    }

    void CCmdReminderAction::SetLevel(qint32 level)
    {
        m_DEReminder.setAttribute(QL1S(XML_REMLEVEL), level);
    }

    bool CCmdReminderAction::GetComplete() const
    {
        return (m_DARemComplete.value() == "true"? true: false);
    }

    void CCmdReminderAction::SetComplete(bool isComplete)
    {
        if(isComplete)
            m_DARemComplete.setValue(QL1S("true"));
        else
            m_DARemComplete.setValue(QL1S("false"));
    }

    QDateTime CCmdReminderAction::GetRemTime() const
    {
        return QDateTime::fromString(m_DARemTime.value(), "dd.MM.yyyy hh:mm:ss.zzz");
    }

    bool CCmdReminderAction::GetRemClosable() const
    {
        return (m_DARemClosable.value() == "true"? true: false);
    }

    CCmdReminderActionExe::CCmdReminderActionExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdReminderAction(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdReminderActionExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            CMaster::TAirplanesMap::iterator itAirFind =
                                m_refMaster.GetPlanes()->find(GetAirID());
            RemAction action = GetAction();
            if (itAirFind != m_refMaster.GetPlanes()->end())
            {
                switch(action)
                {
                case Add:
                {
                    QSharedPointer<CReminder> pRem(new CReminder(GetRemType(), GetRemValue()));
                    pRem->SetLevel(GetLevel());
                    pRem->SetComplete(GetComplete());
                    pRem->SetTime(GetRemTime());
                    pRem->SetClosable(GetRemClosable());

                    if(GetRemValue().isEmpty())
                        pRem->SetValue(CReminder::GetReminderName(pRem->GetType()));

                    QSharedPointer<obelix::CCmdReminderList> pCmd(new obelix::CCmdReminderList(GetAirID()));
                    pCmd->InsertReminder(pRem);
                    QMap<CReminderKey, QSharedPointer<CReminder> >::iterator itRem = (*itAirFind)->GetTempReminders().begin();
                    for(;itRem != (*itAirFind)->GetTempReminders().end();itRem++)
                        pCmd->InsertReminder((*itRem));
                    m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

                    break;
                }
                case Del:
                {
                    QSharedPointer<CReminder> pRem(new CReminder(GetRemType(), GetRemValue()));
                    pRem->SetLevel(GetLevel());
                    pRem->SetComplete(GetComplete());
                    pRem->SetTime(GetRemTime());
                    pRem->SetClosable(GetRemClosable());
                    (*itAirFind)->RemoveReminder(pRem);

                    QSharedPointer<obelix::CCmdReminderList> pCmd(new obelix::CCmdReminderList(GetAirID()));
                    QMap<CReminderKey, QSharedPointer<CReminder> >::iterator itRem = (*itAirFind)->GetTempReminders().begin();
                    for(;itRem != (*itAirFind)->GetTempReminders().end();itRem++)
                        pCmd->InsertReminder((*itRem));
                    m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                    break;
                }
                case DelByType:
                {
                    (*itAirFind)->RemoveReminderByType(GetRemType());
                    QSharedPointer<obelix::CCmdReminderList> pCmd(new obelix::CCmdReminderList(GetAirID()));
                    QMap<CReminderKey, QSharedPointer<CReminder> >::iterator itRem = (*itAirFind)->GetTempReminders().begin();
                    for(;itRem != (*itAirFind)->GetTempReminders().end();itRem++)
                        pCmd->InsertReminder((*itRem));
                    m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                    break;
                }
                case CanRemove:
                {
                    switch(GetRemType())
                    {
                    case CReminder::TrROF:
                    case CReminder::Transfer:
                        {
                        if((*itAirFind)->m_listOutMes.size() && (oldi::MES_COF == (*itAirFind)->m_listOutMes.last()->GetMesData().sType))
                            {
                            }
                            else
                            {
                                if((CReminder::Normal == GetLevel()) || (CReminder::TrROF == GetRemType()))
                                {
                                    QString sClient;
                                    TVA_CATCHALL_TRY
                                    {
                                        sClient = dao()->GetCWPbySector(GetRemValue());
                                    }TVA_CATCHALL(err){
                                    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
                                    if(!sClient.isEmpty() && sClient != networkSettings()->value("oldi/name", "Unknown").toString())
                                    {
                                        // send COF to NS
                                        OLDIMesDataCOF dataCOF;
                                        dataCOF.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                                        dataCOF.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                                        dataCOF.sTo = sClient;
                                        dataCOF.sACID = (*itAirFind)->GetSFPL()->GetACID();

                                        QSharedPointer<OLDIMessage> pMesCOF(new OLDIMesCOF(dataCOF));
                                        (*itAirFind)->m_listOutMes.append(pMesCOF);
                                        ObRespond* respond = new ObRespond();
                                        respond->SetMaster(&m_refMaster);
                                        respond->setParameters((*itAirFind)->GetPlaneType(), pMesCOF->GetMesData(),
                                                               (*itAirFind)->GetSFPL()->GetACID(), GetAirID());
                                        QObject::connect(pMesCOF.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), respond, SLOT(MesAnswer(const OLDIMessage*,const QStringList&)));
                                        QObject::connect(pMesCOF.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), respond, SLOT(MesTimeout(const OLDIMessage*)));

                                        m_refMaster.GetOLDIProc()->SendMessage(pMesCOF);

                                        QSharedPointer<obelix::CCmdTransfer> pCmd(new obelix::CCmdTransfer(GetAirID()));
                                        pCmd->SetPlaneType(CAirplaneData::Transfer);
                                        pCmd->SetTransition(GetRemValue());
                                        pCmd->SetTransferTime();
                                        m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                                    }
                                    else
                                    {
                                        QSharedPointer<obelix::CCmdTransfer> pCmd(new obelix::CCmdTransfer(GetAirID()));
                                        pCmd->SetChangeOperator(0, "");
                                        m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                                    }
                                }
                                return CXMLResp::TXMLResp();// false;
                            }
                        }
                        break;
                    case CReminder::Assume:
                        if(CReminder::Normal == GetLevel())
                        {
                            QSharedPointer<obelix::CCmdTransfer> pCmd(new obelix::CCmdTransfer(GetAirID()));
                            pCmd->SetAssumeOperator();
                            m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                        }
                        else
                        {
                            QSharedPointer<obelix::CCmdTransfer> pCmd(new obelix::CCmdTransfer(GetAirID()));
                            pCmd->SetCurSector("");
                            pCmd->SetPlaneType(CAirplaneData::ACT);
                            pCmd->SetAssumeOperator();
                            m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                        }
                        break;
                    case CReminder::TrHOP:
                        {
                            QString sClient;
                            TVA_CATCHALL_TRY
                            {
                                sClient = dao()->GetCWPbySector(GetRemValue());
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

                                QSharedPointer<obelix::CCmdTransfer> pCmd(new obelix::CCmdTransfer(GetAirID()));
                                pCmd->SetTransition(GetRemValue());
                                pCmd->SetTransferTime();
                                m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                            }
                        }
                        break;
                    case CReminder::TimeOutROF:
                        if(!networkSettings()->value("users/current_sector", "NS").toString().isEmpty())
                        {
                            QSharedPointer<CReminder> pRem(new CReminder(CReminder::Assume, QL1S("IS")));
                            pRem->SetLevel(CReminder::Warning);
                            pRem->SetComplete(true);
                            pRem->SetTime(QDateTime::currentDateTime());

                            QSharedPointer<obelix::CCmdReminderList> pCmd(new obelix::CCmdReminderList(GetAirID()));
                            pCmd->InsertReminder(pRem); //Pasha ?%o, hz, all OLDI put down
                            (*itAirFind)->AddReminderLocal(pRem);
                            QMap<CReminderKey, QSharedPointer<CReminder> >::iterator itRem = (*itAirFind)->GetTempReminders().begin();
                            for(;itRem != (*itAirFind)->GetTempReminders().end();itRem++)
                                pCmd->InsertReminder((*itRem));
                            m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                        }
                        break;
                    default: break;
                    }
                    QSharedPointer<CReminder> pRem(new CReminder(GetRemType(), GetRemValue()));
                    pRem->SetLevel(GetLevel());
                    pRem->SetComplete(GetComplete());
                    pRem->SetTime(GetRemTime());
                    pRem->SetClosable(GetRemClosable());
                    (*itAirFind)->RemoveReminder(pRem);

                    QSharedPointer<obelix::CCmdReminderList> pCmd(new obelix::CCmdReminderList(GetAirID()));
                    QMap<CReminderKey, QSharedPointer<CReminder> >::iterator itRem = (*itAirFind)->GetTempReminders().begin();
                    for(;itRem != (*itAirFind)->GetTempReminders().end();itRem++)
                        pCmd->InsertReminder((*itRem));
                    m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                    break;
                }
                default:
                    break;
                }
            } else
            {
                //LOG
            }
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }

        return CXMLResp::TXMLResp();
    }

    obelix::CCmdReminderAction::RemAction obelix::CCmdReminderAction::GetAction() const
    {
        bool bOk(false);
        RemAction type = (RemAction)m_DARemAction.value().toUInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_REMACTION) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);

        return type;
    }
}
