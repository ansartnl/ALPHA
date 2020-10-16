#include "CmdAirplanaDel.h"
#include "Common.h"
#include "Master.h"
//#include "core/Scene.h"

namespace obelix
{
    const char CCmdAirplaneDel::XML_CMDNAME[]    = "air_delete";
    const char CCmdAirplaneDel::XML_AIR[]        = "airplane";
    const char CCmdAirplaneDel::XML_AIRID[]        = "id";

    CCmdAirplaneDel::CCmdAirplaneDel(quint32 uiAirID)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DNAirplane = GetDoc().createElement(QL1S(XML_AIR));
        m_DAAirID = GetDoc().createAttribute(QL1S(XML_AIRID));
        m_DAAirID.setValue(QString::number(uiAirID));
        m_DNAirplane.setAttributeNode(m_DAAirID);
        GetMainNode().appendChild(m_DNAirplane);
    }

    CCmdAirplaneDel::CCmdAirplaneDel(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DNAirplane = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DNAirplane.isNull() && m_DNAirplane.nodeName() == QL1S(XML_AIR),
            QString(QL1S(XML_AIR) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAAirID = m_DNAirplane.attributeNode(QL1S(XML_AIRID));
        TVA_CHECK(
            !m_DAAirID.isNull(),
            QString(QL1S(XML_AIRID) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
    }

    quint32 CCmdAirplaneDel::GetAirID() const
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


    CCmdAirplaneDelExe::CCmdAirplaneDelExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdAirplaneDel(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdAirplaneDelExe::Execute()
    {
//        TVA_CATCHALL_TRY
//        {
//            LOG_STR(CLog::enDebug, (QObject::tr("Execution of command") + QL1S(": %1")).arg(QL1S(XML_CMDNAME)));
//            db::CDB::TFIRMap::iterator itScan = m_refMaster.GetScenes().begin();
//            for(; itScan != m_refMaster.GetScenes().end(); ++itScan)
//            {
//                CScene::TAirplanesMap::iterator itAir = (*itScan)->GetAirplanes().find(GetAirID());
//                if (itAir != (*itScan)->GetAirplanes().end())
//                    (*itScan)->DelAirplane(*itAir, false);
//            }
//        } TVA_CATCHALL(err)
//        {
//            LOG_TVAERROR(CLog::enError, err)
//        }

        return CXMLResp::TXMLResp();
    }
}

