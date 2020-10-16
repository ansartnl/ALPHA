#include "CmdAirplanaCmdRespond.h"

#include "Master.h"
#include "common/Calculation.h"
#include <QMessageBox>
#include "XMasterWindow.h"
#include <math.h>

namespace obelix
{
    const char CCmdAirplaneCmdRespond::XML_CMDNAME[]    = "airplane_cmd_respond";
    const char CCmdAirplaneCmdRespond::XML_AIR[]        = "airplane";

    const char CCmdAirplaneCmdRespond::XML_AIRID[] = "id";
    const char CCmdAirplaneCmdRespond::XML_CMDTYPE[] = "type";
    const char CCmdAirplaneCmdRespond::XML_VALUE[] = "value";
    const char CCmdAirplaneCmdRespond::XML_ALTVALUE[] = "alt_value";


    CCmdAirplaneCmdRespond::CCmdAirplaneCmdRespond(
            quint32 uiId,
            CmdType type,
            QString value)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DEAir        = GetDoc().createElement(QL1S(XML_AIR));
        m_DAAirID = GetDoc().createAttribute(QL1S(XML_AIRID));
        m_DAAirID.setValue(QString::number(uiId));
        m_DACmdType = GetDoc().createAttribute(QL1S(XML_CMDTYPE));
        m_DACmdType.setValue(QString::number((int)type));
        m_DAVal = GetDoc().createAttribute(QL1S(XML_VALUE));
        m_DAVal.setValue(value);

        m_DEAir.setAttributeNode(m_DAAirID);
        m_DEAir.setAttributeNode(m_DACmdType);
        m_DEAir.setAttributeNode(m_DAVal);
        GetMainNode().appendChild(m_DEAir);
    }

    quint32 CCmdAirplaneCmdRespond::GetAirID() const
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

    CCmdAirplaneCmdRespond::CmdType CCmdAirplaneCmdRespond::GetType() const
    {
        int iType(-1);
        bool bOk(false);
        iType = m_DACmdType.value().toInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_CMDTYPE) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return (CCmdAirplaneCmdRespond::CmdType)iType;
    }

    CCmdAirplaneCmdRespond::CCmdAirplaneCmdRespond(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DEAir = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DEAir.isNull() && m_DEAir.nodeName() == QL1S(XML_AIR),
            QString(QL1S(XML_AIR) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAAirID = m_DEAir.attributeNode(QL1S(XML_AIRID));
        TVA_CHECK(
            !m_DAAirID.isNull(),
            QString(QL1S(XML_AIRID) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DACmdType = m_DEAir.attributeNode(QL1S(XML_CMDTYPE));
        TVA_CHECK(
            !m_DACmdType.isNull(),
            QString(QL1S(XML_CMDTYPE) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAVal = m_DEAir.attributeNode(QL1S(XML_VALUE));
        TVA_CHECK(
            !m_DAVal.isNull(),
            QString(QL1S(XML_VALUE) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
    }

    CCmdAirplaneCmdRespondExe::CCmdAirplaneCmdRespondExe(
            const CXMLData::TXMLData& pData,
            CMaster& refMaster)
        : CCmdAirplaneCmdRespond(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdAirplaneCmdRespondExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            quint32 uiAirID = GetAirID();

            CMaster::TAirplanesMap::iterator itFindPlane =
                    m_refMaster.GetPlanes()->find(uiAirID);
            QString sVal = GetValue();
            if (itFindPlane != m_refMaster.GetPlanes()->end())
            {
                switch(GetType()){
                case CODE:
                {//
                    (*itFindPlane)->SetMode3AReply(sVal.toInt());
                    break;
                }
                case Vz:
                {//?
                    (*itFindPlane)->ChangeVz(sVal.toDouble());
                    break;
                }
                case Roll:
                {//!
                    (*itFindPlane)->SetBank(sVal.toDouble());
                    break;
                }
                case Cfl:
                {//
                    (*itFindPlane)->SetCFL(sVal);
                    break;
                }
                case Tfl:
                {//
                    (*itFindPlane)->SetTFL(FLVal(sVal));
                    break;
                }
                case HDG:
                {//?
                    (*itFindPlane)->ChangeAzimuthLocal(sVal.toInt());
                    break;
                }
                case spd1:
                {//
                    (*itFindPlane)->SetIAS(sVal.toDouble());
                    break;
                }
                case spd2:
                {//
                    (*itFindPlane)->SetIAS(sVal.toDouble());
                    break;
                }
                case NextPoint:
                {//
                    if(GetAltValue() == "Point")
                    {
                        if((*itFindPlane)->GetSFPL() && (*itFindPlane)->GetSFPL()->GetFPLFrag())
                        {
                            (*itFindPlane)->GetSFPL()->GetFPLFrag()->SetNextPoint(GetValue().toUInt());
                        }
                    }
                    else if(GetAltValue() == "Zone")
                    {
                        (*itFindPlane)->ChangeHolding(GetValue().toUInt());
                    }
                    break;
                }
                default:
                    break;
                }
            }
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }

        return CXMLResp::TXMLResp();
    }
}
