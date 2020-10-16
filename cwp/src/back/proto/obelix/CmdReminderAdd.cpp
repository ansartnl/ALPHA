#include "CmdReminderAdd.h"
#include "Common.h"
#include "Master.h"

namespace obelix
{
    const char CCmdReminderAdd::XML_CMDNAME[]    = "reminder_add";
    const char CCmdReminderAdd::XML_REMINDER[]    = "reminder";
    const char CCmdReminderAdd::XML_AIRID[]        = "airplane_id";
    const char CCmdReminderAdd::XML_REMTYPE[]    = "type";
    const char CCmdReminderAdd::XML_REMVALUE[]    = "value";

    const char CCmdReminderAdd::XML_REMLEVEL[] = "level";
    const char CCmdReminderAdd::XML_REMCOMPLETE[] = "is_complete";

    const char CCmdReminderAdd::XML_REMTIME[]        = "rem_time";
    const char CCmdReminderAdd::XML_REMCLOSABLE[]    = "is_closable";

    CCmdReminderAdd::CCmdReminderAdd(
            quint32 uiAirId,
            const QSharedPointer<CReminder>& pReminder)
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
        m_DEReminder.setAttributeNode(m_DARemLevel);
        m_DEReminder.setAttributeNode(m_DARemComplete);
        m_DEReminder.setAttributeNode(m_DARemTime);
        m_DEReminder.setAttributeNode(m_DARemClosable);

        GetMainNode().appendChild(m_DEReminder);
    }

    CCmdReminderAdd::CCmdReminderAdd(const CXMLData::TXMLData& pData)
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

        m_DARemLevel = m_DEReminder.attributeNode(QL1S(XML_REMLEVEL));
        TVA_CHECK(
            !m_DARemLevel.isNull(),
            QString(QL1S(XML_REMLEVEL) + QObject::tr(" attribute in XML command is absent or has wrong name")),
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

    quint32 CCmdReminderAdd::GetAirID() const
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

    CReminder::ReminderType CCmdReminderAdd::GetRemType() const
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

    QString CCmdReminderAdd::GetRemValue() const
    {
        return m_DARemValue.value();
    }

    qint32 CCmdReminderAdd::GetLevel() const
    {
        bool bOk(false);
        qint32 nType = m_DARemLevel.value().toInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_REMLEVEL) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return (CReminder::ReminderType)nType;
    }

    void CCmdReminderAdd::SetLevel(qint32 level)
    {
        m_DARemLevel.setValue(QString::number(level));
    }

    bool CCmdReminderAdd::GetComplete() const
    {
        return (m_DARemComplete.value() == "true"? true: false);
    }

    void CCmdReminderAdd::SetComplete(bool isComplete)
    {
        if(isComplete)
            m_DARemComplete.setValue(QL1S("true"));
        else
            m_DARemComplete.setValue(QL1S("false"));
    }

    QDateTime CCmdReminderAdd::GetRemTime() const
    {
        return QDateTime::fromString(m_DARemTime.value(), "dd.MM.yyyy hh:mm:ss.zzz");
    }

    bool CCmdReminderAdd::GetRemClosable() const
    {
        return (m_DARemClosable.value() == "true"? true: false);
    }


    CCmdReminderAddExe::CCmdReminderAddExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdReminderAdd(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdReminderAddExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            CMaster::TAirplanesMap::iterator itAirFind =
                                m_refMaster.GetPlanes()->find(GetAirID());
            if (itAirFind != m_refMaster.GetPlanes()->end())
            {
                QSharedPointer<CReminder> pReminder(new CReminder(GetRemType(), GetRemValue()));
                pReminder->SetComplete(GetComplete());
                pReminder->SetLevel((CReminder::ReminderLevel) GetLevel());
                pReminder->SetTime(GetRemTime());
                pReminder->SetClosable(GetRemClosable());

                (*itAirFind)->AddReminderLocal(pReminder);
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
}
