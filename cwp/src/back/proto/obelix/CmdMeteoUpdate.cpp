#include "CmdMeteoUpdate.h"


#include "CmdPut.h"
#include "Common.h"
#include "Master.h"
#include "proto/obelix/CmdTake.h"
#include "CmdReminderList.h"

namespace obelix
{
    const char CCmdMeteoUpdate::XML_CMDNAME[]    = "meteo+update";
    const char CCmdMeteoUpdate::XML_METEOUPDATE[]     = "meteo";
    const char CCmdMeteoUpdate::XML_HEXDATA[]      = "HexData";

    CCmdMeteoUpdate::CCmdMeteoUpdate(const QString& sHexData)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DEUserZone = GetDoc().createElement(QL1S(XML_METEOUPDATE));
        m_DAHexData = GetDoc().createAttribute(QL1S(XML_HEXDATA));

        m_DAHexData.setValue(sHexData);

        m_DEUserZone.setAttributeNode(m_DAHexData);
        GetMainNode().appendChild(m_DEUserZone);
    }

    CCmdMeteoUpdate::CCmdMeteoUpdate(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DEUserZone = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DEUserZone.isNull() && m_DEUserZone.nodeName() == QL1S(XML_METEOUPDATE),
            QString(QL1S(XML_METEOUPDATE) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAHexData = m_DEUserZone.attributeNode(QL1S(XML_HEXDATA));
        TVA_CHECK(
            !m_DAHexData.isNull(),
            QString(QL1S(XML_HEXDATA) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
    }

    QString CCmdMeteoUpdate::GetHexData() const
    {
        return m_DAHexData.value();
    }

    CCmdMeteoUpdateExe::CCmdMeteoUpdateExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdMeteoUpdate(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdMeteoUpdateExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            OLDIMesDataURZ mesData;
            mesData.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
            mesData.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
            mesData.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
            mesData.sDBType = QString(OLDIMesURZ::enMeteo);
            mesData.sFirIdName = " ";
            mesData.sHexData = GetHexData();
            QSharedPointer<OLDIMessage> pMes(new OLDIMesURZ(mesData));
            m_refMaster.GetOLDIProc()->SendMessage(pMes, false);
        } TVA_CATCHALL(err)
        {
			Q_UNUSED(err)
        }
        return CXMLResp::TXMLResp();
    }
}
