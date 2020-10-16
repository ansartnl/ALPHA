#include "CmdSendUserZone.h"

#include "CmdPut.h"
#include "Common.h"
#include "Master.h"
#include "proto/obelix/CmdTake.h"
#include "CmdReminderList.h"

namespace obelix
{
    const char CCmdSendUserZone::XML_CMDNAME[]    = "send_user_zone";
    const char CCmdSendUserZone::XML_USERZONE[]     = "user_zone";
    const char CCmdSendUserZone::XML_DBTYPE[]       = "DBType";
    const char CCmdSendUserZone::XML_FIRIDNAME[]    = "FIRIDName";
    const char CCmdSendUserZone::XML_HEXDATA[]      = "HexData";

    CCmdSendUserZone::CCmdSendUserZone(const QString& sDBType,const QString& sFIRIDName, const QString& sHexData)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DEUserZone = GetDoc().createElement(QL1S(XML_USERZONE));
        m_DADBType = GetDoc().createAttribute(QL1S(XML_DBTYPE));
        m_DAFIRIDName = GetDoc().createAttribute(QL1S(XML_FIRIDNAME));
        m_DAHexData = GetDoc().createAttribute(QL1S(XML_HEXDATA));

        m_DADBType.setValue(sDBType);
        m_DAFIRIDName.setValue(sFIRIDName);
        m_DAHexData.setValue(sHexData);

        m_DEUserZone.setAttributeNode(m_DADBType);
        m_DEUserZone.setAttributeNode(m_DAFIRIDName);
        m_DEUserZone.setAttributeNode(m_DAHexData);
        GetMainNode().appendChild(m_DEUserZone);
    }

    CCmdSendUserZone::CCmdSendUserZone(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DEUserZone = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DEUserZone.isNull() && m_DEUserZone.nodeName() == QL1S(XML_USERZONE),
            QString(QL1S(XML_USERZONE) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DADBType = m_DEUserZone.attributeNode(QL1S(XML_DBTYPE));
        TVA_CHECK(
            !m_DADBType.isNull(),
            QString(QL1S(XML_DBTYPE) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAFIRIDName = m_DEUserZone.attributeNode(QL1S(XML_FIRIDNAME));
        TVA_CHECK(
            !m_DAFIRIDName.isNull(),
            QString(QL1S(XML_FIRIDNAME) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAHexData = m_DEUserZone.attributeNode(QL1S(XML_HEXDATA));
        TVA_CHECK(
            !m_DAHexData.isNull(),
            QString(QL1S(XML_HEXDATA) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
    }

    QString CCmdSendUserZone::GetDBType() const
    {
        return m_DADBType.value();
    }

    QString CCmdSendUserZone::GetFIRIDName() const
    {
        return m_DAFIRIDName.value();
    }
    QString CCmdSendUserZone::GetHexData() const
    {
        return m_DAHexData.value();
    }

    CCmdSendUserZoneExe::CCmdSendUserZoneExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdSendUserZone(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdSendUserZoneExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            OLDIMesDataURZ mesData;
            mesData.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
            mesData.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
            mesData.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
            mesData.sDBType = GetDBType();
            mesData.sFirIdName = GetFIRIDName();
            mesData.sHexData = GetHexData();
            QSharedPointer<OLDIMessage> pMes(new OLDIMesURZ(mesData));
            m_refMaster.GetOLDIProc()->SendMessage(pMes, false);
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }
        return CXMLResp::TXMLResp();
    }
}
