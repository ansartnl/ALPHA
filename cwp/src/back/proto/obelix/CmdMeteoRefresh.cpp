#include "CmdMeteoRefresh.h"
#include "Common.h"
#include "common/XML.h"
#include "Simulator.h"

#include <QString>


namespace obelix
{
    const char CCmdMeteoRefresh::XML_CMDNAME[]      = "meteo_refresh";
    const char CCmdMeteoRefresh::XML_METEO[]        = "meteo";
    const char CCmdMeteoRefresh::XML_METEO_FIR[]    = "idfir";

    CCmdMeteoRefresh::CCmdMeteoRefresh(quint32 idFIR)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DNMeteo = GetDoc().createElement(QL1S(XML_METEO));
        m_DAFirID = GetDoc().createAttribute(QL1S(XML_METEO_FIR));
        m_DAFirID.setValue(QString::number(idFIR));
        m_DNMeteo.setAttributeNode(m_DAFirID);
        GetMainNode().appendChild(m_DNMeteo);
    }

    CCmdMeteoRefresh::CCmdMeteoRefresh(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DNMeteo = GetMainNode().firstChildElement();
        TVA_CHECK(
                    !m_DNMeteo.isNull() && m_DNMeteo.nodeName() == QL1S(XML_METEO),
                    QString(QL1S(XML_METEO) + QObject::tr(" node in XML command is absent or has wrong name")),
                    tva::E_FAIL);

        m_DAFirID = m_DNMeteo.attributeNode(QL1S(XML_METEO_FIR));
        TVA_CHECK(
                    !m_DAFirID.isNull(),
                    QString(QL1S(XML_METEO_FIR) + QObject::tr(" attribute in XML command is absent or has wrong name")),
                    tva::E_FAIL);

    }

    quint32 CCmdMeteoRefresh::GetFIR() const
    {
        quint32 uiID(0);
        bool bOk(false);
        uiID = m_DAFirID.value().toUInt(&bOk);
        TVA_CHECK(
                    bOk,
                    QString(QL1S(XML_METEO_FIR) + QObject::tr(" attribute in XML command has wrong structure")),
                    tva::E_FAIL);
        return uiID;
    }

    CCmdMeteoRefreshExe::CCmdMeteoRefreshExe(
            const CXMLData::TXMLData& pData,
            CSimulator& refSimulator)
        : CCmdMeteoRefresh(pData), m_refSimulator(refSimulator)
    {

    }

    CXMLResp::TXMLResp CCmdMeteoRefreshExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            LOG_STR(CLog::enDebug, (QObject::tr("Execution of command") + QL1S(": %1")).arg(QL1S(XML_CMDNAME)));
            m_refSimulator.MeteoRefresh(GetFIR());
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(CLog::enError, err)
        }

        return CXMLResp::TXMLResp();
    }
}
