#include "CmdReminderDel.h"
#include "Common.h"
#include "Master.h"

namespace obelix
{
    const char CCmdReminderDel::XML_CMDNAME[]    = "reminder_del";
    const char CCmdReminderDel::XML_REMINDER[]    = "reminder";
    const char CCmdReminderDel::XML_AIRID[]        = "airplane_id";
    const char CCmdReminderDel::XML_REMID[]        = "id";

    const char CCmdReminderDel::XML_REMTYPE[] = "reminder_type";
    const char CCmdReminderDel::XML_REMVALUE[] = "reminder_value";

    CCmdReminderDel::CCmdReminderDel(quint32 uiAirId, qint32 iRemId)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DEReminder = GetDoc().createElement(QL1S(XML_REMINDER));
        m_DARemID = GetDoc().createAttribute(QL1S(XML_REMID));
        m_DARemID.setValue(QString::number(iRemId));
        m_DAAirID = GetDoc().createAttribute(QL1S(XML_AIRID));
        m_DAAirID.setValue(QString::number(uiAirId));
        m_DARemValue = GetDoc().createAttribute(QL1S(XML_REMVALUE));
        m_DARemValue.setValue("");

        m_DEReminder.setAttributeNode(m_DARemID);
        m_DEReminder.setAttributeNode(m_DAAirID);
        m_DEReminder.setAttributeNode(m_DARemValue);

        GetMainNode().appendChild(m_DEReminder);
    }

    CCmdReminderDel::CCmdReminderDel(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DEReminder = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DEReminder.isNull() && m_DEReminder.nodeName() == QL1S(XML_REMINDER),
            QString(QL1S(XML_REMINDER) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DARemID = m_DEReminder.attributeNode(QL1S(XML_REMID));
        TVA_CHECK(
            !m_DARemID.isNull(),
            QString(QL1S(XML_REMID) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAAirID = m_DEReminder.attributeNode(QL1S(XML_AIRID));
        TVA_CHECK(
            !m_DAAirID.isNull(),
            QString(QL1S(XML_AIRID) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DARemValue = m_DEReminder.attributeNode(QL1S(XML_REMVALUE));
        TVA_CHECK(
            !m_DARemValue.isNull(),
            QString(QL1S(XML_REMVALUE) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
    }

    qint32 CCmdReminderDel::GetRemID() const
    {
        qint32 uiID(0);
        bool bOk(false);
        uiID = m_DARemID.value().toInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_REMID) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return uiID;
    }

    CReminder::ReminderType CCmdReminderDel::GetRemType() const
    {
        bool bOk(false);
        qint32 nType = m_DEReminder.attribute(XML_REMTYPE, QL1S("0")).toInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_REMTYPE) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return (CReminder::ReminderType)nType;
    }

    QString CCmdReminderDel::GetRemValue() const
    {
        return m_DARemValue.value();
    }

    void CCmdReminderDel::SetRemType(qint32 val)
    {
        m_DEReminder.setAttribute(QL1S(XML_REMTYPE), val);
    }

    quint32 CCmdReminderDel::GetAirID() const
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

    CCmdReminderDelExe::CCmdReminderDelExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdReminderDel(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdReminderDelExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            CMaster::TAirplanesMap::iterator itAirFind =
                                m_refMaster.GetPlanes()->find(GetAirID());
            if (itAirFind != m_refMaster.GetPlanes()->end())
            {
                if(GetRemID() == 0) //-1
                {
                    QSharedPointer<CReminder> pRem(new CReminder(GetRemType(), GetRemValue()));
                    (*itAirFind)->RemoveReminder(pRem);
                }
                else
                {
                    (*itAirFind)->RemoveReminderByType((CReminder::ReminderType)GetRemType());
                }
            }
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }

        return CXMLResp::TXMLResp();
    }
}
