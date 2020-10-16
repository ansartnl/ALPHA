#include "CmdAirplanaStat.h"

#include "Common.h"
#include "Master.h"
//#include "core/Scene.h"
#include "common/Calculation.h"
#include "common/XML.h"
#include "AirplaneData.h"
#include "Airplane.h"
//#include "core/Plane.h"
//#include "core/airplane/ActivationPlane.h"

namespace obelix
{
    const char CCmdAirplaneStat::XML_CMDNAME[]    = "airplane_state";
    const char CCmdAirplaneStat::XML_AIR[]        = "airplane";
    const char CCmdAirplaneStat::XML_AIRID[]    = "id";
    const char CCmdAirplaneStat::XML_AIROPER[]    = "operator";
    const char CCmdAirplaneStat::XML_AIREXEID[]    = "exe_id";
    const char CCmdAirplaneStat::XML_AIREXEFPLID[]="chain_id";
    const char CCmdAirplaneStat::XML_AIRSQUAWK[]= "squawk";
    const char CCmdAirplaneStat::XML_AIRIAS[]    = "IAS";
    const char CCmdAirplaneStat::XML_AIRFL[]    = "FL";
    const char CCmdAirplaneStat::XML_AIRVZ[]    = "Vz";
    const char CCmdAirplaneStat::XML_AIRAZIMUTH[]= "azimuth";
    const char CCmdAirplaneStat::XML_AIRBANK[]    = "bank";
    const char CCmdAirplaneStat::XML_AIRPNTPLAN[]= "fpl_num";
    const char CCmdAirplaneStat::XML_AIRPNTID[]    = "fplpnt_num";
    const char CCmdAirplaneStat::XML_AIRPNTLONG[]= "point_longitude";
    const char CCmdAirplaneStat::XML_AIRPNTLAT[]= "point_latitude";
    const char CCmdAirplaneStat::XML_AIRHLDID[]    = "holding_id";
    const char CCmdAirplaneStat::XML_AIRHLDSN[]    = "holding_sn";
    const char CCmdAirplaneStat::XML_AIRSSAID[]    = "ssa_id";
    const char CCmdAirplaneStat::XML_AIRSSASN[]    = "ssa_sn";
    const char CCmdAirplaneStat::XML_REM[]        = "reminders";
    const char CCmdAirplaneStat::XML_REMID[]    = "id";
    const char CCmdAirplaneStat::XML_REMTYPE[]    = "type";
    const char CCmdAirplaneStat::XML_REMVALUE[]    = "value";
    const char CCmdAirplaneStat::XML_REMCOMPLETE[]="complete";
    const char CCmdAirplaneStat::XML_REMTIME[]    = "time";
    const char CCmdAirplaneStat::XML_AIRSPI[]    = "SPI";
    const char CCmdAirplaneStat::XML_AIREQUIPMENT[] = "equipment";
    const char CCmdAirplaneStat::XML_CUSTOMSTUNT[] = "custom_stunt";

    const char CCmdAirplaneStat::XML_TYPE[] = "message_type";
    const char CCmdAirplaneStat::XML_DATAPART[] = "data";
    const char CCmdAirplaneStat::XML_OLDIMES[] = "OLDI_message";
    const char CCmdAirplaneStat::XML_TO[] = "sTo";
    const char CCmdAirplaneStat::XML_FROM[] = "sFrom";
    const char CCmdAirplaneStat::XML_ACID[] = "sACID";
    const char CCmdAirplaneStat::XML_SFPLID[] = "SFPL_ID";

    CCmdAirplaneStat::CCmdAirplaneStat(
            quint32 uiId,
            const QString& sOperName,
            const QSharedPointer<CAirplane>& pPlane)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DEAir        = GetDoc().createElement(QL1S(XML_AIR));
        GetMainNode().appendChild(m_DEAir);

        SetValue(QL1S(XML_AIRID), uiId);
        SetValue(QL1S(XML_AIROPER), sOperName);        
    }

    CCmdAirplaneStat::CCmdAirplaneStat(
            quint32 uiId,
            const QString& sOLDIMes)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DEAir        = GetDoc().createElement(QL1S(XML_AIR));
        GetMainNode().appendChild(m_DEAir);

        SetValue(QL1S(XML_AIRID), uiId);
        SetValue(QL1S(XML_OLDIMES), sOLDIMes);
    }

    QString CCmdAirplaneStat::GetValue(const QString& sAttrName)
    {
        return m_DEAir.attribute(sAttrName);
    }

    quint32 CCmdAirplaneStat::GetAirID()
    {
        quint32 uiID(0);
        bool bOk(false);
        uiID = m_DEAir.attribute(QL1S(XML_AIRID)).toUInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_AIRID) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return uiID;
    }

    bool CCmdAirplaneStat::IsValue(const QString& sAttrName)
    {
        return !m_DEAir.attributeNode(sAttrName).isNull();
    }

    CCmdAirplaneStat::CCmdAirplaneStat(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DEAir = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DEAir.isNull() && m_DEAir.nodeName() == QL1S(XML_AIR),
            QString(QL1S(XML_AIR) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);
        m_DERem = m_DEAir.nextSiblingElement();
    }

    CCmdAirplaneStatExe::CCmdAirplaneStatExe(
            const CXMLData::TXMLData& pData,
            CMaster& refMaster)
        : CCmdAirplaneStat(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdAirplaneStatExe::Execute()
    {
        QString sMessageType = GetValue(QL1S(XML_OLDIMES));
        QString sFrom = GetValue(QL1S(XML_FROM));
        CMaster::TAirplanesMap::iterator itAirFind =
                            m_refMaster.GetPlanes()->find(GetAirID());
        if(itAirFind != m_refMaster.GetPlanes()->end())
        {
            TVA_CATCHALL_TRY
            {
                if(oldi::MES_MAS == sMessageType)
                {
                    (*itAirFind)->CancelTransfer(false);
                    (*itAirFind)->SetOur(false);
                    (*itAirFind)->SetPlaneType(CAirplaneData::Transfered);
                }
                else if(oldi::MES_ACT == sMessageType)
                {

                }
                else if(oldi::MES_COF == sMessageType)
                {
                    (*itAirFind)->SetTransition((*itAirFind)->GetSFPL()->GetCurSector());
                    (*itAirFind)->SetTransferTime(QDateTime::currentDateTime());
                    //(*itAirFind)->m_listInMes.append(pMes);   //!!!!
                    //QSharedPointer<CReminder> pRem(new CReminder(CReminder::Transfer, tr("COF")));
                    QSharedPointer<CReminder> pRem(new CReminder(CReminder::Assume, (*itAirFind)->GetTransition()));
                    pRem->SetLevel(CReminder::Normal);
                    pRem->SetComplete(true);
                    pRem->SetTime(QDateTime::currentDateTime());
                    (*itAirFind)->AddReminder(pRem);
                }
                else if(oldi::MES_HOP == sMessageType)
                {
                    (*itAirFind)->SetTransferTime(QDateTime::currentDateTime());
                    //(*itAirFind)->m_listInMes.append(pMes);   //!!!!
                    QString sSector;
                    TVA_CATCHALL_TRY
                    {
                        sSector = dao()->GetSectorByCWP(sFrom);
                    }TVA_CATCHALL(err){
                    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
                    QSharedPointer<CReminder> pRem(new CReminder(CReminder::TrHOP, sSector));
                    //QSharedPointer<CReminder> pRem(new CReminder(CReminder::Assume, ""));
                    pRem->SetLevel(CReminder::Warning);
                    pRem->SetComplete(true);
                    pRem->SetTime(QDateTime::currentDateTime());
                    (*itAirFind)->AddReminder(pRem);
                }
                else if(oldi::MES_ROF == sMessageType)
                {
                    if(CAirplaneData::Transfer == (*itAirFind)->GetPlaneType())
                    {
                        CAirplaneData::TReminderMap::iterator itRem = (*itAirFind)->GetTempReminders().find(CReminderKey(CReminder(CReminder::Transfer, (*itAirFind)->GetTransition())));
                        if(itRem != (*itAirFind)->GetTempReminders().end())
                        {
                            (*itRem)->SetLevel(CReminder::Normal);
                        }
                    }
                    else
                    {
                        QString sSector;
                        TVA_CATCHALL_TRY
                        {
                            sSector = dao()->GetSectorByCWP(sFrom);
                        }TVA_CATCHALL(err){
                        LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
                        if(!sSector.isEmpty())
                        {
                            (*itAirFind)->SetTransition(sSector);
                            (*itAirFind)->SetTransferTime(QDateTime::currentDateTime());
                            //(*itAirFind)->m_listInMes.append(pMes);   // !!!!!!
                            QSharedPointer<CReminder> pRem(new CReminder(CReminder::TrROF, sSector));
                            pRem->SetLevel(CReminder::Warning);
                            pRem->SetComplete(true);
                            pRem->SetTime(QDateTime::currentDateTime());
                            (*itAirFind)->AddReminder(pRem);
                        }
                    }
                }
                else if(oldi::MES_MAC == sMessageType)
                {
                    (*itAirFind)->CancelTransfer(false);
                }
                else if(oldi::MES_STS == sMessageType)
                {
                    (*itAirFind)->SetNS();
                }
            } TVA_CATCHALL(err)
            {
                LOG_TVAERROR(ERROR, err)
            }
        }

        return CXMLResp::TXMLResp();
    }
}
