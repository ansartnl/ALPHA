#include "CmdSectorChanged.h"
#include "Master.h"
#include "Common.h"
#include "proto/Obelix.h"
#include "CmdSetSector.h"

namespace obelix
{
    const char CCmdSectorChanged::XML_CMDNAME[] = "sector_changed";
    const char CCmdSectorChanged::XML_SECTOR[] = "sector";
    const char CCmdSectorChanged::XML_INDEX[] = "index";

    CCmdSectorChanged::CCmdSectorChanged(const QString &sIndex)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));

        m_DNSector = GetDoc().createElement(QL1S(XML_SECTOR));
        m_DAIndex = GetDoc().createAttribute(QL1S(XML_INDEX));
        m_DAIndex.setValue(sIndex);
        m_DNSector.setAttributeNode(m_DAIndex);
        GetMainNode().appendChild(m_DNSector);
        MarkACK(false);
    }

    CCmdSectorChanged::CCmdSectorChanged(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DNSector = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DNSector.isNull() && m_DNSector.nodeName() == QL1S(XML_SECTOR),
            QString(QL1S(XML_SECTOR) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAIndex = m_DNSector.attributeNode(QL1S(XML_INDEX));
        TVA_CHECK(
            !m_DAIndex.isNull(),
            QString(QL1S(XML_INDEX) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
        MarkACK(false);
    }

    QString CCmdSectorChanged::GetIndex() const
    {
        QString res = m_DAIndex.value();
        return res;
    }

    CCmdSectorChangedExe::CCmdSectorChangedExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdSectorChanged(pData), m_refMaster(refMaster)
    {

    }


    CXMLResp::TXMLResp CCmdSectorChangedExe::Execute()
    {
        CXMLResp::TXMLResp pResp;
        TVA_CATCHALL_TRY
        {
            QSharedPointer<CCmdSetSector> pCmd(new obelix::CCmdSetSector(GetIndex()));
            m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }

        return pResp;
    }
}
