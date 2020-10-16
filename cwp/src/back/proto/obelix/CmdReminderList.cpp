#include "CmdReminderList.h"
#include "Common.h"
#include "Master.h"

namespace obelix
{
    const char CCmdReminderList::XML_CMDNAME[]    = "reminder_list";
    const char CCmdReminderList::XML_REMINDER[]    = "reminder";
    const char CCmdReminderList::XML_AIRID[]        = "airplane_id";
    const char CCmdReminderList::XML_REMTYPE[]    = "type";
    const char CCmdReminderList::XML_REMVALUE[]    = "value";
    const char CCmdReminderList::XML_REMLEVEL[] = "level";
    const char CCmdReminderList::XML_REMCOMPLETE[] = "is_complete";
    const char CCmdReminderList::XML_LIST[] = "list";

    const char CCmdReminderList::XML_REMTIME[]        = "rem_time";
    const char CCmdReminderList::XML_REMCLOSABLE[]    = "is_closable";

    CCmdReminderList::CCmdReminderList(
            quint32 uiAirId)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DEList = GetDoc().createElement(QL1S(XML_LIST));
        m_DAAirID = GetDoc().createAttribute(QL1S(XML_AIRID));
        m_DAAirID.setValue(QString::number(uiAirId));

        m_DEList.setAttributeNode(m_DAAirID);
        GetMainNode().appendChild(m_DEList);
    }

    CCmdReminderList::CCmdReminderList(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DEList = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DEList.isNull() && m_DEList.nodeName() == QL1S(XML_LIST),
            QString(QL1S(XML_LIST) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAAirID = m_DEList.attributeNode(QL1S(XML_AIRID));
        TVA_CHECK(
            !m_DAAirID.isNull(),
            QString(QL1S(XML_AIRID) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
    }

    quint32 CCmdReminderList::GetAirID()
    {
        quint32 uiID(0);
        bool bOk(false);
        uiID = m_DEList.attribute(XML_AIRID).toUInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_AIRID) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return uiID;
    }

    QDomNodeList CCmdReminderList::GetReminders()
    {
        return m_DEList.childNodes();
    }

    QSharedPointer<CReminder> CCmdReminderList::FormReminder(QDomNode rem)
    {
        QDomNamedNodeMap attributes = rem.attributes();
        QSharedPointer<CReminder> pReminder(
                    new CReminder(
                        GetRemType(attributes.namedItem(QL1S(XML_REMTYPE))),
                        GetRemValue(attributes.namedItem(QL1S(XML_REMVALUE)))));
        pReminder->SetComplete(GetComplete(attributes.namedItem(QL1S(XML_REMCOMPLETE))));
        pReminder->SetLevel((CReminder::ReminderLevel) GetLevel(attributes.namedItem(QL1S(XML_REMLEVEL))));
        pReminder->SetTime(GetRemTime(attributes.namedItem(QL1S(XML_REMTIME))));
        pReminder->SetClosable(GetRemClosable(attributes.namedItem(QL1S(XML_REMCLOSABLE))));
        return pReminder;
    }

    void CCmdReminderList::InsertReminder(QSharedPointer<CReminder> pReminder)
    {
        QDomElement DEReminder = GetDoc().createElement(QL1S(XML_REMINDER));
        QDomAttr DARemType = GetDoc().createAttribute(QL1S(XML_REMTYPE));
        DARemType.setValue(QString::number((int)pReminder->GetType()));
        QDomAttr DARemValue = GetDoc().createAttribute(QL1S(XML_REMVALUE));
        DARemValue.setValue(pReminder->GetValue());
        QDomAttr DARemLevel = GetDoc().createAttribute(QL1S(XML_REMLEVEL));
        DARemLevel.setValue(QString::number((quint32)pReminder->GetLevel()));
        QDomAttr DARemComplete = GetDoc().createAttribute(QL1S(XML_REMCOMPLETE));
        DARemComplete.setValue((pReminder->GetComplete()?QL1S("true"):QL1S("false")));
        QDomAttr DARemTime = GetDoc().createAttribute(QL1S(XML_REMTIME));
        DARemTime.setValue(pReminder->GetTime().toString("dd.MM.yyyy hh:mm:ss.zzz"));
        QDomAttr DARemClosable = GetDoc().createAttribute(QL1S(XML_REMCLOSABLE));
        DARemClosable.setValue((pReminder->IsClosable()?QL1S("true"):QL1S("false")));

        DEReminder.setAttributeNode(DARemType);
        DEReminder.setAttributeNode(DARemValue);
        DEReminder.setAttributeNode(DARemLevel);
        DEReminder.setAttributeNode(DARemComplete);
        DEReminder.setAttributeNode(DARemTime);
        DEReminder.setAttributeNode(DARemClosable);

        m_DEList.appendChild(DEReminder);
    }

    CReminder::ReminderType CCmdReminderList::GetRemType(QDomNode node) const
    {
        int iType(-1);
        bool bOk(false);
        iType = node.toAttr().value().toInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_REMTYPE) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return (CReminder::ReminderType)iType;
    }

    QString CCmdReminderList::GetRemValue(QDomNode node) const
    {
        return node.toAttr().value();
    }

    qint32 CCmdReminderList::GetLevel(QDomNode node) const
    {
        bool bOk(false);
        qint32 nType = node.toAttr().value().toInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_REMLEVEL) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return (CReminder::ReminderType)nType;
    }

    bool CCmdReminderList::GetComplete(QDomNode node) const
    {
        return (node.toAttr().value() == "true"? true: false);
    }

    QDateTime CCmdReminderList::GetRemTime(QDomNode node) const
    {
        return QDateTime::fromString(node.toAttr().value(), "dd.MM.yyyy hh:mm:ss.zzz");
    }

    bool CCmdReminderList::GetRemClosable(QDomNode node) const
    {
        return (node.toAttr().value() == "true"? true: false);
    }

    CCmdReminderListExe::CCmdReminderListExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdReminderList(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdReminderListExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            if(m_refMaster.GetPlanes())
            {
                CMaster::TAirplanesMap::iterator itAirFind =
                                    m_refMaster.GetPlanes()->find(GetAirID());
                if (itAirFind != m_refMaster.GetPlanes()->end())
                {
                    (*itAirFind)->GetTempReminders().clear();
                    QDomNodeList nodes = GetReminders();
                    for(int i = 0; i < nodes.count(); i++)
                        (*itAirFind)->AddReminderLocal(FormReminder(nodes.at(i)));
                } else
                {
                    //LOG
                }
            }
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
        }
        return CXMLResp::TXMLResp();
    }
}

