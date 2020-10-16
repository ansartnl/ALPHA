#include "CmdSetSector.h"
#include "Common.h"
#include "Master.h"
#include "XMasterWindow.h"

namespace obelix
{
    const char CCmdSetSector::XML_CMDNAME[] = "set_sector";
    const char CCmdSetSector::XML_SECTOR[] = "sector";
    const char CCmdSetSector::XML_INDEX[] = "index";
    const char CCmdSetSector::XML_SECTORNAME[] = "sector_name";

    CCmdSetSector::CCmdSetSector(const QString& sIndex)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DNSector = GetDoc().createElement(QL1S(XML_SECTOR));
        m_DAIndex = GetDoc().createAttribute(QL1S(XML_INDEX));
        m_DAIndex.setValue(sIndex);

        m_DNSector.setAttributeNode(m_DAIndex);        
        GetMainNode().appendChild(m_DNSector);
    }

    CCmdSetSector::CCmdSetSector(const CXMLData::TXMLData& pData)
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

    QString CCmdSetSector::GetIndex() const
    {
        QString res = m_DAIndex.value();
        return res;
    }

    void CCmdSetSector::SetSectorName(const QString &sName)
    {
        m_DNSector.setAttribute(QL1S(XML_SECTORNAME), sName);
    }

    QString CCmdSetSector::GetSectorName()
    {
        return m_DNSector.attribute(QL1S(XML_SECTORNAME), QString(""));
    }

    CCmdSetSectorExe::CCmdSetSectorExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdSetSector(pData), m_refMaster(refMaster)
    {

    }


    CXMLResp::TXMLResp CCmdSetSectorExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            if (GetSectorName().isEmpty()) {
                static_cast<XMasterWindow *>(getMainWindow())->LoadEnvironment(GetIndex());
            } else {
                QSettings &settings = *globalSettings();
                settings.beginGroup("XMaster");
                QString name = GetSectorName();
                settings.setValue("sectoring", name);
                settings.endGroup();
            }
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }

        return CXMLResp::TXMLResp();
    }
}
