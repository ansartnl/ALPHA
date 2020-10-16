#include "CmdAirplanaCmd.h"
#include "Master.h"
#include "common/Calculation.h"
#include <QMessageBox>
#include "CmdAirplanaCmdRespond.h"

namespace obelix
{
    const char CCmdAirplaneCmd::XML_CMDNAME[]    = "airplane_cmd";
    const char CCmdAirplaneCmd::XML_AIR[]        = "airplane";

    const char CCmdAirplaneCmd::XML_FPLID[] = "fpl_id";
    const char CCmdAirplaneCmd::XML_AIRID[] = "id";
    const char CCmdAirplaneCmd::XML_CMDTYPE[] = "type";
    const char CCmdAirplaneCmd::XML_VALUE[] = "value";
    const char CCmdAirplaneCmd::XML_ALTVALUE[] = "alt_value";


    CCmdAirplaneCmd::CCmdAirplaneCmd(
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

    quint32 CCmdAirplaneCmd::GetAirID() const
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

    CCmdAirplaneCmd::CmdType CCmdAirplaneCmd::GetType() const
    {
        int iType(-1);
        bool bOk(false);
        iType = m_DACmdType.value().toInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_CMDTYPE) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return (CCmdAirplaneCmd::CmdType)iType;
    }

    CCmdAirplaneCmd::CCmdAirplaneCmd(const CXMLData::TXMLData& pData)
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

    CCmdAirplaneCmdExe::CCmdAirplaneCmdExe(
            const CXMLData::TXMLData& pData,
            CMaster& refMaster)
        : CCmdAirplaneCmd(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdAirplaneCmdExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            quint32 uiAirID = GetAirID();

            CMaster::TAirplanesMap::iterator itFindPlane =
                    m_refMaster.GetPlanes()->find(uiAirID);
            if (itFindPlane != m_refMaster.GetPlanes()->end())
            {
                switch(GetType()){
                case CODE:
                {
                    TVA_CATCHALL_TRY
                    {
                        bool bOk(false);
                        quint16 uiMode3A = GetValue().toUInt(&bOk);
                        TVA_CHECK(
                            bOk,
                            QString(QL1S(XML_VALUE) + QObject::tr(" attribute in XML command has wrong structure")),
                            E_FAIL);
                    ///////////////////////////////////
                        QString sCode = QString("%1").arg(uiMode3A, 4, 8, QLatin1Char('0'));
                        QSharedPointer<CSFPL> SFPL = (*itFindPlane)->GetSFPL();

                        SFPL->SetCODE(uiMode3A);
                        dao()->UpdateSFPL(SFPL);

                        OLDIMesDataCOD mesData;
                        mesData.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                        mesData.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                        mesData.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
                        mesData.sACID = SFPL->GetACID();
                        mesData.sADEP = SFPL->GetDepAerodrome();
                        mesData.sADES = SFPL->GetDestAerodrome();
                        mesData.sCODE = sCode;
                        QSharedPointer<OLDIMessage> pMes(new OLDIMesCOD(mesData));
                        m_refMaster.GetOLDIProc()->SendMessage(pMes);

                        QString sSectors = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
                        QStringList sSectorsList = sSectors.split(",", QString::SkipEmptyParts);
                        for(int i = 0; i < sSectorsList.size(); ++i)
                        {
                            mesData.sTo = sSectorsList[i];
                            pMes = QSharedPointer<OLDIMessage>(new OLDIMesCOD(mesData));
                            m_refMaster.GetOLDIProc()->SendMessage(pMes);
                        }

    //                    if(IsValue(QL1S(XML_FPLID)))
    //                    {
    //                        quint32 id = GetValue(QL1S(XML_FPLID)).toUInt(&bOk);
    //                        TVA_CHECK(
    //                            bOk,
    //                            QString(QL1S(XML_FPLID) + QObject::tr(" attribute in XML command has wrong structure")),
    //                            E_FAIL);

    //                        TVA_CATCHALL_TRY
    //                        {
    //                            QList<QSharedPointer<CSFPL> > SFPLList;
    //                            TVA_CATCHALL_TRY
    //                            {
    //                                dao()->LoadSFPLs(SFPLList);
    //                            }TVA_CATCHALL(err){
    //                            QMessageBox::critical(NULL, QObject::tr("SFPL"), err.reason());
    //                            }

    //                            QList<QSharedPointer<CSFPL> >::iterator itList = SFPLList.begin();
    //                            for(; itList != SFPLList.end(); ++itList)
    //                            {
    //                                if( (*itList)->GetID() == id)
    //                                    break;
    //                            }

    //                            TVA_CHECK(
    //                                itList != SFPLList.end(),
    //                                QString("SFPL with id %1 not found!").arg(id),
    //                                E_FAIL);

    //                            (*itList)->SetCODE(uiMode3A);
    //                            dao()->UpdateSFPL((*itList));

    //                            //if(m_refMaster)
    //                            {
    //                                OLDIMesDataCOD mesData;
    //                                mesData.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
    //                                mesData.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
    //                                mesData.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
    //                                mesData.sACID = (*itList)->GetACID();
    //                                mesData.sADEP = (*itList)->GetDepAerodrome();
    //                                mesData.sADES = (*itList)->GetDestAerodrome();
    //                                mesData.sCODE = sCode;
    //                                QSharedPointer<OLDIMessage> pMes(new OLDIMesCOD(mesData));
    //                                m_refMaster.GetOLDIProc()->SendMessage(pMes);

    //                                QString sSectors = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
    //                                QStringList sSectorsList = sSectors.split(",", QString::SkipEmptyParts);
    //                                for(int i = 0; i < sSectorsList.size(); ++i)
    //                                {
    //                                    mesData.sTo = sSectorsList[i];
    //                                    pMes = QSharedPointer<OLDIMessage>(new OLDIMesCOD(mesData));
    //                                    m_refMaster.GetOLDIProc()->SendMessage(pMes);
    //                                }
    //                            }
                    }TVA_CATCHALL(err){
                    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
                    //////////////////////////////////////////////
                    }
                    break;
                }
                case Vz:
                {
                    break;
                }
                case Roll:
                {
                    break;
                }
                case Cfl:
                {
                    (*itFindPlane)->ChangeFLLocal(FLVal(GetValue()));
                    QSharedPointer<obelix::CCmdAirplaneCmdRespond> pCmd(new obelix::CCmdAirplaneCmdRespond(GetAirID(), (CCmdAirplaneCmdRespond::CmdType)GetType(), GetValue()));
                    m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                    break;
                }
                case Tfl:
                {
                    (*itFindPlane)->ChangeTFLLocal(FLVal(GetValue()));
                    QSharedPointer<obelix::CCmdAirplaneCmdRespond> pCmd(new obelix::CCmdAirplaneCmdRespond(GetAirID(), (CCmdAirplaneCmdRespond::CmdType)GetType(), GetValue()));
                    m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                    break;
                }
                case HDG:
                {
                    QSharedPointer<obelix::CCmdAirplaneCmdRespond> pCmd(new obelix::CCmdAirplaneCmdRespond(GetAirID(), (CCmdAirplaneCmdRespond::CmdType)GetType(), GetValue()));
                    m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                    break;
                }
                case spd1:
                {
                    QSharedPointer<obelix::CCmdAirplaneCmdRespond> pCmd(new obelix::CCmdAirplaneCmdRespond(GetAirID(), (CCmdAirplaneCmdRespond::CmdType)GetType(), GetValue()));
                    m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                    break;
                }
                case spd2:
                {
                    QSharedPointer<obelix::CCmdAirplaneCmdRespond> pCmd(new obelix::CCmdAirplaneCmdRespond(GetAirID(), (CCmdAirplaneCmdRespond::CmdType)GetType(), GetValue()));
                    m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                    break;
                }
                case NextPoint:
                {
                    QSharedPointer<obelix::CCmdAirplaneCmdRespond> pCmd(new obelix::CCmdAirplaneCmdRespond(GetAirID(), (CCmdAirplaneCmdRespond::CmdType)GetType(), GetValue()));
                    pCmd->SetAltValue(GetAltValue());
                    m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
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
