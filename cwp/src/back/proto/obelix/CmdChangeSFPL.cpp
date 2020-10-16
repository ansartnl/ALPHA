#include "CmdChangeSFPL.h"

#include "Common.h"
#include "Master.h"
#include "OLDIMessages.h"
#include "CmdSFPLRefresh.h"

namespace obelix
{
    const char CCmdChangeSFPL::XML_CMDNAME[]    = "change_sfpl";
    const char CCmdChangeSFPL::XML_CHANGE[]    = "change";
    const char CCmdChangeSFPL::XML_TYPE[]       = "type";
    const char CCmdChangeSFPL::XML_AIRID[]    = "airplane_id";

    const char CCmdChangeSFPL::XML_FPLID[] = "fpl_id";  //quint32
    const char CCmdChangeSFPL::XML_CODE[] = "CODE";   //toInt(0, 8)
    const char CCmdChangeSFPL::XML_COPIN[] = "COPin";  //QString
    const char CCmdChangeSFPL::XML_ETOIN[] = "ETOin";  //time
    const char CCmdChangeSFPL::XML_COPINFL[] = "COPinFL";//double

    CCmdChangeSFPL::CCmdChangeSFPL(Type type)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DNChange = GetDoc().createElement(QL1S(XML_CHANGE));
        m_DAType = GetDoc().createAttribute(QL1S(XML_TYPE));
        m_DAType.setValue(QString::number((quint32)type));
        m_DNChange.setAttributeNode(m_DAType);

        GetMainNode().appendChild(m_DNChange);
    }


    CCmdChangeSFPL::CCmdChangeSFPL(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DNChange = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DNChange.isNull() && m_DNChange.nodeName() == QL1S(XML_CHANGE),
            QString(QL1S(XML_CHANGE) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAType = m_DNChange.attributeNode(QL1S(XML_TYPE));
        TVA_CHECK(
            !m_DAType.isNull(),
            QString(QL1S(XML_TYPE) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
    }

    CCmdChangeSFPL::Type CCmdChangeSFPL::GetType()
    {
        quint32 uiID(0);
        bool bOk(false);
        uiID = m_DAType.value().toUInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_AIRID) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return (Type)uiID;
    }

    QString CCmdChangeSFPL::GetValue(const QString& sAttrName)
    {
        return m_DNChange.attribute(sAttrName);
    }

    CCmdChangeSFPLExe::CCmdChangeSFPLExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdChangeSFPL(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdChangeSFPLExe::Execute()
    {
        QList<CFPL> list;
        TVA_CATCHALL_TRY
        {
            dao()->LoadFPLs(list, "1 = 1");//ACHTUNG!! visibility = 1
        } TVA_CATCHALL(err)
        {
			Q_UNUSED(err)
        }        

        TVA_CATCHALL_TRY
        {
            QList<CFPL>::iterator itList = list.begin();
            for(; itList != list.end(); ++itList)
            {
                if( (*itList).GetID() == GetValue(QL1S(XML_FPLID)).toUInt())
                    break;
            }

            if(itList == list.end())
                return CXMLResp::TXMLResp();

            QSharedPointer<CSFPL> pSFPL = QSharedPointer<CSFPL>(new CSFPL);
            *(static_cast<CFPL*>(pSFPL.data())) = *(itList);
            pSFPL->SetCODE(GetValue(QL1S(XML_CODE)).toUInt());
            pSFPL->SetCOPin(GetValue(QL1S(XML_COPIN)));
            pSFPL->SetETOin(QTime().fromString(GetValue(QL1S(XML_ETOIN))));
            pSFPL->SetCOPinFL(FLVal(GetValue(QL1S(XML_COPINFL))));

            Type type = GetType();

            if(type == STAR)
            {
                dao()->UpdateSFPL(pSFPL);

                OLDIMesDataABI dataABI = OLDIMessage::GetABIfromSFPL(pSFPL);
                dataABI.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                dataABI.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                dataABI.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
                QSharedPointer<OLDIMessage> pMes(new OLDIMesABI(dataABI));
                m_refMaster.GetOLDIProc()->SendMessage(pMes);

                QString sSectors = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
                QStringList sSectorsList = sSectors.split(",", QString::SkipEmptyParts);
                for(int i = 0; i < sSectorsList.size(); ++i)
                {
                    dataABI.sTo = sSectorsList[i];
                    pMes = QSharedPointer<OLDIMessage>(new OLDIMesABI(dataABI));
                    m_refMaster.GetOLDIProc()->SendMessage(pMes);
                }

                QSharedPointer<obelix::CCmdSFPLRefresh> pCmd(new obelix::CCmdSFPLRefresh());
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_TYPE, oldi::MES_ABI);
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_CODE, QString::number(pSFPL->GetCODE()));
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_COPIN, pSFPL->GetCOPin());
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ETOIN, pSFPL->GetETOin().toString());
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_COPINFL, pSFPL->GetCOPinFL().toString());
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_FPLID, QString::number(pSFPL->GetID()));
                m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
            }
            else if(type == SetData)
            {
                dao()->UpdateSFPL(pSFPL);

                OLDIMesDataCOD mesData;
                mesData.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                mesData.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                mesData.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
                mesData.sACID = pSFPL->GetACID();
                mesData.sADEP = pSFPL->GetDepAerodrome();
                mesData.sADES = pSFPL->GetDestAerodrome();
                mesData.sCODE = GetValue(QL1S(XML_CODE));   //!!
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
            }
            else if(type == Edit)
            {
                // update FPL
                //LOG_OPER_SIMPLE_STR("Change ACID " + pSFPL->GetACID());
//                LOG_OPER_STR("ALL", "ACID", pSFPL->GetACID());
                //Refresh(pSFPL->GetID());
                QSharedPointer<obelix::CCmdSFPLRefresh> pCmd(new obelix::CCmdSFPLRefresh());
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_TYPE, "COMMON");
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ACID, QString("%1").arg(pSFPL->GetID()));
                m_refMaster.GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

                // send ABI
                QSharedPointer<CSFPL> pSFPLtoSend;
                CListACModel::TListACData& pData = CListAC::GetSourceModel().GetModelData();
                CListACModel::TListACData::const_iterator itData = pData.begin();
                for (; itData != pData.end(); ++itData)
                {
                    if (itData->GetSFPL() && itData->GetSFPL()->GetID() == pSFPL->GetID())
                    {
                        pSFPLtoSend = itData->GetSFPL();
                        break;
                    }
                }
                if(pSFPLtoSend)
                {
                    OLDIMesDataABI dataABI = OLDIMessage::GetABIfromSFPL(pSFPLtoSend);
                    dataABI.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                    dataABI.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                    dataABI.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
                    QSharedPointer<OLDIMessage> pMesABI(new OLDIMesABI(dataABI));
                    m_refMaster.GetOLDIProc()->SendMessage(pMesABI);

                    QString sSectors = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
                    QStringList sSectorsList = sSectors.split(",", QString::SkipEmptyParts);
                    for(int i = 0; i < sSectorsList.size(); ++i)
                    {
                        dataABI.sTo = sSectorsList[i];
                        pMesABI = QSharedPointer<OLDIMessage>(new OLDIMesABI(dataABI));
                        m_refMaster.GetOLDIProc()->SendMessage(pMesABI);
                    }
                }
            }
            else if(type == Cancel)
            {
                OLDIMesDataINF dataINF;
                dataINF.iMesNum = 0;
                dataINF.sTypeINF = oldi::INF_UNDO_SFPL;
                dataINF.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                dataINF.sTo = dataINF.sFrom;
                dataINF.sACID = pSFPL->GetACID();
                dataINF.sADEP = pSFPL->GetDepAerodrome();
                dataINF.sADES = pSFPL->GetDestAerodrome();
                dataINF.sCODE = pSFPL->GetCODEtoString();
                dataINF.sSector = " ";

                QSharedPointer<OLDIMessage> pMesINF(new OLDIMesINF(dataINF));
                pMesINF->GetMesSourceData().bAnswer = false;
                CListAC::RecieveOLDIMessage(pMesINF);

                dataINF.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                dataINF.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();

                pMesINF = QSharedPointer<OLDIMessage>(new OLDIMesINF(dataINF));
                m_refMaster.GetOLDIProc()->SendMessage(pMesINF);

                QString sCWP = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
                QStringList sCWPList = sCWP.split(",", QString::SkipEmptyParts);
                for(int i = 0; i < sCWPList.size(); ++i)
                {
                    dataINF.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                    dataINF.sTo = sCWPList.at(i);

                    pMesINF = QSharedPointer<OLDIMessage>(new OLDIMesINF(dataINF));
                    m_refMaster.GetOLDIProc()->SendMessage(pMesINF);
                }
            }
            else
            if(type != DEP)
            {
                QStringList sCurSectorsList = networkSettings()->value("users/current_sectors_short_name", "").toString().split("-", QString::SkipEmptyParts);

                QMap<QString, QString> mapCWP;
                QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_refMaster.GetView());
                if(view)
                {
                    Scene::TSceneList listSectors = view->FindSectorsByPoint(pSFPL->GetCOPin(), pSFPL->GetCOPinFL().GetM());
                    if(pSFPL->GetFPLFrag())
                        listSectors = pSFPL->GetFPLFrag()->GetWhichRouteSectors(pSFPL->GetCOPin(), pSFPL->GetCOPinFL().GetM(), listSectors);
                    mapCWP.insert(networkSettings()->value("users/current_fdp_name", "FDP").toString(), networkSettings()->value("users/current_fdp_name", "FDP").toString());
                    Scene::TSceneList::const_iterator itSector = listSectors.constBegin();
                    for(; itSector != listSectors.constEnd(); ++itSector)
                    {
                        TVA_CATCHALL_TRY
                        {
                            if(sCurSectorsList.contains((*itSector)->GetShortName()))
                            {
                                mapCWP.insert(networkSettings()->value("oldi/name", "Unknown").toString(), networkSettings()->value("oldi/name", "Unknown").toString());
                            }
                            else
                            {
                                QString sCWP = dao()->GetCWPbySector((*itSector)->GetShortName());
                                if(!sCWP.isEmpty())
                                    mapCWP.insert(sCWP, sCWP);
                            }
                        }TVA_CATCHALL(err){
                            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
                    }
                }
                if(mapCWP.size() > 1)
                {
                    if(type == Change)
                    {
                        CMaster::TAirplanesMap::iterator itAirFind =
                                m_refMaster.GetPlanes()->find(GetValue(QL1S(XML_AIRID)).toUInt());
                        if((*itAirFind)->GetSFPL())
                        {
                            //undo SFPL
                            OLDIMesDataINF dataINF;
                            dataINF.iMesNum = 0;
                            dataINF.sTypeINF = oldi::INF_UNDO_SFPL;
                            dataINF.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                            dataINF.sTo = dataINF.sFrom;
                            dataINF.sACID = (*itAirFind)->GetSFPL()->GetACID();
                            dataINF.sADEP = (*itAirFind)->GetSFPL()->GetDepAerodrome();
                            dataINF.sADES = (*itAirFind)->GetSFPL()->GetDestAerodrome();
                            dataINF.sCODE = (*itAirFind)->GetSFPL()->GetCODEtoString();
                            dataINF.sSector = " ";

                            QSharedPointer<OLDIMessage> pMesINF(new OLDIMesINF(dataINF));
                            pMesINF->GetMesSourceData().bAnswer = false;
                            CListAC::RecieveOLDIMessage(pMesINF);

                            dataINF.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                            dataINF.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();

                            pMesINF = QSharedPointer<OLDIMessage>(new OLDIMesINF(dataINF));
                            m_refMaster.GetOLDIProc()->SendMessage(pMesINF);

                            QString sCWP = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
                            QStringList sCWPList = sCWP.split(",", QString::SkipEmptyParts);
                            for(int i = 0; i < sCWPList.size(); ++i)
                            {
                                dataINF.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                                dataINF.sTo = sCWPList.at(i);

                                pMesINF = QSharedPointer<OLDIMessage>(new OLDIMesINF(dataINF));
                                m_refMaster.GetOLDIProc()->SendMessage(pMesINF);
                            }
                        }
                    }
                    bool bSendItself = false;
                    QMap<QString, QString>::const_iterator itClient = mapCWP.constBegin();
                    for(; itClient != mapCWP.constEnd(); ++itClient)
                    {
                        if(networkSettings()->value("oldi/name", "Unknown").toString() == itClient.key())
                        {
                            bSendItself = true;
                        }
                        else
                        {
                            if(!itClient->isEmpty())
                            {
                                //Pasha temp--------------------
                                {
                                    OLDIMesDataABI dataFPL = OLDIMessage::GetABIfromSFPL(pSFPL);
                                    dataFPL.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                                    dataFPL.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                                    dataFPL.sTo = *itClient;
                                    QSharedPointer<OLDIMessage> pMesFPL(new OLDIMesFPL(dataFPL));
                                    m_refMaster.GetOLDIProc()->SendMessage(pMesFPL);
                                }
                                //------------------------------
                                OLDIMesDataABI dataACT = OLDIMessage::GetABIfromSFPL(pSFPL);
                                dataACT.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                                dataACT.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                                dataACT.sTo = *itClient;
                                QSharedPointer<OLDIMessage> pMesACT(new OLDIMesACT(dataACT));
                                m_refMaster.GetOLDIProc()->SendMessage(pMesACT);
                            }
                        }
                    }
                    if(bSendItself)
                    {
                        // send ACT to itself direct
                        OLDIMesDataABI dataACT = OLDIMessage::GetABIfromSFPL(pSFPL);
                        dataACT.iMesNum = 0;
                        dataACT.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                        dataACT.sTo = dataACT.sFrom;

                        QSharedPointer<OLDIMessage> pMesACT(new OLDIMesACT(dataACT));
                        pMesACT->GetMesSourceData().bAnswer = false;
                        CListAC::RecieveOLDIMessage(pMesACT);
                    }
                }
            }
            else    //type == DEP
            {
                OLDIMesDataABI dataACT = OLDIMessage::GetABIfromSFPL(pSFPL);
                dataACT.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                dataACT.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                dataACT.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
                QSharedPointer<OLDIMessage> pMesACT(new OLDIMesACT(dataACT));
                m_refMaster.GetOLDIProc()->SendMessage(pMesACT);

                // send ACT to itself direct
                dataACT.iMesNum = 0;
                dataACT.sTo = dataACT.sFrom;
                pMesACT = QSharedPointer<OLDIMessage>(new OLDIMesACT(dataACT));
                pMesACT->GetMesSourceData().bAnswer = false;
                CListAC::RecieveOLDIMessage(pMesACT);
            }
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }
        return CXMLResp::TXMLResp();
    }
}

