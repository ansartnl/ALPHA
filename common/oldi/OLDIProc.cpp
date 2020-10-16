#include "OLDIProc.h"
#include "OLDIMessages.h"
#include <QTimer>
#include <QSqlError>
#include <QDebug>
#include "qtautotransaction.h"
#include "TVAError.h"
#include "Database.h"
#include "../sleep/sleep.h"
#include "OLDIClients.h"

OLDIProc::OLDIProc(QSqlDatabase base, QString sOwnerSenderName, quint32 iSecTimeout, QObject* parent)
    : QObject(parent), m_pAckTimer(0)
    , m_mesnum(0), /*m_DB(base),*/ m_sOwnerSenderName(sOwnerSenderName), m_iTimeOut(iSecTimeout)
    , m_bIsFDP(false)
{
    // maybe not need yet
    m_DB = QSqlDatabase::cloneDatabase(base, QString("OLDIProc%1").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")));
    if (!m_DB.open())
    {
        qWarning() << tr("OLDIProc could not connect to database\nError: %1").arg(m_DB.lastError().text());
    }
    /////////////////////////////

#if defined(REDIS)
#else
    m_pAckTimer = new QTimer(this);
    connect(m_pAckTimer, SIGNAL(timeout()), this, SLOT(OnAckTimer()));
    m_pAckTimer->start(1100);
#endif
}

OLDIProc::~OLDIProc()
{
    if(m_pAckTimer)
        m_pAckTimer->stop();
}

quint16 OLDIProc::GetNextMesNum()
{
    ++m_mesnum %= 1000;
    return m_mesnum;
}

void OLDIProc::SendMessage(QSharedPointer<OLDIMessage> pMes, bool bWait/* = true*/)
{
#if defined(REDIS)
#else
    if (bWait) Sleep::msleep(11);

    TVA_CATCHALL_TRY
    {
        QtAutoTransaction transaction(m_DB);
        QSharedPointer<db::CDBQuery> query = db::CreateQuery(m_DB);
        query->Prepare(
                    "INSERT INTO OUTPUTMESSAGES (msgtime, msgfrom, msgto, msgtext, msgtype, msgstatus) "
                    "VALUES(" + query->GetSystimestampStr() + ", :2, :3, :4, :5, 0)",
                    "msgid");
        //query->BindValue(":1", );
        query->BindValue(":2", pMes->GetMesData().sFrom);
        query->BindValue(":3", pMes->GetMesData().sTo);
        query->BindValue(":4", pMes->GetMesData().sOLDIMes);
        query->BindValue(":5", pMes->GetMesData().sType);
        //query->BindValue(":6", );
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
        //int lastid = query->GetLastInsertID().toInt();
        transaction.commit();

        if(pMes->GetMesData().bAnswer)
            m_mapSendMes.insert(MesMapKey(pMes->GetMesData().iMesNum, pMes->GetMesData().sFrom, pMes->GetMesData().sTo), pMes);

        qWarning() << tr("Send OLDI message %1").arg(pMes->GetMesData().sOLDIMes);
    }
    TVA_CATCHALL(err)
    {
        qWarning() << err.reason();
    }
#endif
}

void OLDIProc::OnAckTimer()
{
    QList<OLDIMesData> listMes;
    TVA_CATCHALL_TRY
    {
        QtAutoTransaction transaction(m_DB);
        QSharedPointer<db::CDBQuery> query = db::CreateQuery(m_DB);
        query->Prepare(
                "SELECT msgid, msgtime, msgfrom, msgto, msgtext, msgtype "
                "FROM inputmessages "
                "WHERE msgto = :1"
                "ORDER BY msgid");
        // problem with time zone
        //        query->Prepare(
        //                "SELECT msgid, msgtime, msgfrom, msgto, msgtext, msgtype "
        //                "FROM inputmessages "
        //                "WHERE msgto = :1 AND " + query->GetSystimestampStr() + " - msgtime < to_dsinterval(:2) "
        //                "ORDER BY msgtime");
        query->BindValue(":1", m_sOwnerSenderName);
        query->BindValue(":2", QTime().addSecs(m_iTimeOut).toString("0 hh:mm:ss"));
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
        while(query->next())
        {
            OLDIMesData mesData;
            mesData.sFrom = query->value(2).toString();
            mesData.sTo = query->value(3).toString();
            mesData.sOLDIMes = QString(query->value(4).toByteArray());
            mesData.sType = query->value(5).toString();
            listMes.append(mesData);
        }
        query->Prepare(
                "DELETE FROM inputmessages "
                "WHERE msgto = :1");
        query->BindValue(":1", m_sOwnerSenderName);
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
        transaction.commit();
    }
    TVA_CATCHALL(err)
    {
        qWarning() << err.reason();
    }

    QList<OLDIMesData>::const_iterator itMesData = listMes.constBegin();
    for(; itMesData != listMes.constEnd(); ++itMesData)
    {
        qWarning() << tr("Recieve OLDI message %1").arg(itMesData->sOLDIMes);
        QSharedPointer<OLDIMessage> pMes = CreateMessage(itMesData->sType, itMesData->sOLDIMes);
        if(pMes)
        {
            if(oldi::MES_ERR == pMes->GetMesData().sType || oldi::MES_LAM == pMes->GetMesData().sType)
            {
                TMesMap::iterator itMes = m_mapSendMes.begin();
                for(; itMes != m_mapSendMes.end(); ++itMes)
                {
                    if((*itMes)->isAnswer(pMes.data()))
                    {
                        m_mapSendMes.erase(itMes);
                        break;
                    }
                }
            }
            else
            {
                if(oldi::MES_STS == pMes->GetMesData().sType)
                {
                    RecieveSTSMes(pMes);
                }
                emit recieveMessage(pMes);
                if(!pMes->isExecuted())
                    m_mapReceiveMes.insert(MesMapKey(pMes->GetMesData().iMesNum, pMes->GetMesData().sFrom, pMes->GetMesData().sTo), pMes);
            }
        }
    }

    QDateTime timeOut = QDateTime::currentDateTime().addSecs(-m_iTimeOut);
    TMesMap::iterator itMapMes = m_mapSendMes.begin();
    while(itMapMes != m_mapSendMes.end())
    {
        if((*itMapMes)->GetMesData().dtMes < timeOut)
        {
            (*itMapMes)->TimeOut();
            itMapMes = m_mapSendMes.erase(itMapMes);
        }
        else ++itMapMes;
    }
    itMapMes = m_mapReceiveMes.begin();
    while(itMapMes != m_mapReceiveMes.end())
    {
        if((*itMapMes)->isExecuted())
            itMapMes = m_mapReceiveMes.erase(itMapMes);
        else
            ++itMapMes;
    }
}

void OLDIProc::RecieveSTSMes(QSharedPointer<OLDIMessage> pMes)
{
    pMes->SetExecuted();
    TMesMap mapMes;
    QList<QString> listCWPforUpdate;
    if(CheckSTSMes(pMes, mapMes, listCWPforUpdate))
    {
        OLDIMesData dataLAM = pMes->GetLAM(GetNextMesNum());
        QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
        mapMes.insert(MesMapKey(dataLAM.iMesNum, dataLAM.sFrom, dataLAM.sTo), pMesLAM);

        if(m_bIsFDP)
        {
            OLDIMesDataSTS dataSTS = pMes.staticCast<OLDIMesSTS>()->GetDataSTS();
            QStringList sSectorList = dataSTS.sText.split("-", QString::SkipEmptyParts);
            QSharedPointer<db::CDBQuery> query = db::CreateQuery(m_DB);
            query->Prepare(
                    "SELECT client, status, sector "
                    "FROM oldi_clients "
                    "ORDER BY status");
            TVA_CATCHALL_TRY
            {
                TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
            }
            TVA_CATCHALL(err)
            {
                qWarning() << err.reason();
            }
            QMap<QString, QString> mapSectors;
            while(query->next())
            {
                mapSectors[query->value(0).toString()] += "-" + query->value(2).toString();
            }

            if(sSectorList.size() == 1)
            {
                // send only delete
                OLDIMesDataSTS mesDataSTS;
                mesDataSTS.iMesNum = GetNextMesNum();
                mesDataSTS.sFrom = m_sOwnerSenderName;
                QMap<QString, QString>::const_iterator itClient = mapSectors.constBegin();
                for(; itClient != mapSectors.constEnd(); ++itClient)
                {
                    mesDataSTS.sTo = itClient.key();
                    mesDataSTS.sText = dataSTS.sText;
                    QSharedPointer<OLDIMessage> pMesSTS(new OLDIMesSTS(mesDataSTS));
                    mapMes.insert(MesMapKey(mesDataSTS.iMesNum, mesDataSTS.sFrom, mesDataSTS.sTo), pMesSTS);
                }
                mesDataSTS.sTo = dataSTS.sFrom;
                mesDataSTS.sText = dataSTS.sText;
                QSharedPointer<OLDIMessage> pMesSTS(new OLDIMesSTS(mesDataSTS));
                mapMes.insert(MesMapKey(mesDataSTS.iMesNum, mesDataSTS.sFrom, mesDataSTS.sTo), pMesSTS);
            }
            else
            {
                // send all contents of sectors table
                OLDIMesDataSTS mesDataSTS;
                mesDataSTS.sFrom = m_sOwnerSenderName;
                QMap<QString, QString>::const_iterator itSector = mapSectors.constBegin();
                for(; itSector != mapSectors.constEnd(); ++itSector)
                {
                    mesDataSTS.iMesNum = GetNextMesNum();
                    mesDataSTS.sText = "-" + itSector.key() + itSector.value();
                    QMap<QString, QString>::const_iterator itClient = mapSectors.constBegin();
                    for(; itClient != mapSectors.constEnd(); ++itClient)
                    {
                        if(itSector.key() == itClient.key() && listCWPforUpdate.contains(itSector.key()))
                            mesDataSTS.iPriority = 1;
                        else
                            mesDataSTS.iPriority = 0;
                        mesDataSTS.sTo = itClient.key();
                        QSharedPointer<OLDIMessage> pMesSTS(new OLDIMesSTS(mesDataSTS));
                        mapMes.insert(MesMapKey(mesDataSTS.iMesNum, mesDataSTS.sFrom, mesDataSTS.sTo), pMesSTS);
                    }
                }
            }
        }
    }

    TMesMap::const_iterator itMapMes = mapMes.constBegin();
    for(; itMapMes != mapMes.constEnd(); ++itMapMes)
        SendMessage(*itMapMes);
}

QSharedPointer<OLDIMessage> OLDIProc::CreateMessage(const QString& sType, const QString& sOLDIMes) const
{
    QSharedPointer<OLDIMessage> pMes;
    if(oldi::MES_ABI == sType)
        pMes = QSharedPointer<OLDIMessage>(new OLDIMesABI(sOLDIMes));
    else
        if(oldi::MES_ACT == sType)
            pMes = QSharedPointer<OLDIMessage>(new OLDIMesACT(sOLDIMes));
    else
        if(oldi::MES_COD == sType)
            pMes = QSharedPointer<OLDIMessage>(new OLDIMesCOD(sOLDIMes));
    else
        if(oldi::MES_COF == sType)
            pMes = QSharedPointer<OLDIMessage>(new OLDIMesCOF(sOLDIMes));
    else
        if(oldi::MES_ERR == sType)
            pMes = QSharedPointer<OLDIMessage>(new OLDIMesERR(sOLDIMes));
    else
        if(oldi::MES_FPL == sType)
            pMes = QSharedPointer<OLDIMessage>(new OLDIMesFPL(sOLDIMes));
    else
        if(oldi::MES_HOP == sType)
            pMes = QSharedPointer<OLDIMessage>(new OLDIMesHOP(sOLDIMes));
    else
        if(oldi::MES_INF == sType)
            pMes = QSharedPointer<OLDIMessage>(new OLDIMesINF(sOLDIMes));
    else
        if(oldi::MES_LAM == sType)
            pMes = QSharedPointer<OLDIMessage>(new OLDIMesLAM(sOLDIMes));
    else
        if(oldi::MES_MAC == sType)
            pMes = QSharedPointer<OLDIMessage>(new OLDIMesMAC(sOLDIMes));
    else
        if(oldi::MES_MAS == sType)
            pMes = QSharedPointer<OLDIMessage>(new OLDIMesMAS(sOLDIMes));
    else
        if(oldi::MES_ROF == sType)
            pMes = QSharedPointer<OLDIMessage>(new OLDIMesROF(sOLDIMes));
    else
        if(oldi::MES_STS == sType)
            pMes = QSharedPointer<OLDIMessage>(new OLDIMesSTS(sOLDIMes));
    else
        if(oldi::MES_URZ == sType)
            pMes = QSharedPointer<OLDIMessage>(new OLDIMesURZ(sOLDIMes));
    else
        qWarning() << tr("Not processed type of OLDI message '%1'").arg(sType);

    return pMes;
}

bool OLDIProc::CheckSTSMes(QSharedPointer<OLDIMessage> pMes, TMesMap& mapMes, QList<QString>& listCWPforUpdate)
{
    if(oldi::MES_STS != pMes->GetMesData().sType) return false;
    OLDIMesDataSTS dataSTS = pMes.staticCast<OLDIMesSTS>()->GetDataSTS();
    QStringList sSectorList = dataSTS.sText.split("-", QString::SkipEmptyParts);
    if(sSectorList.size() < 1) return false;

    bool bOK = false;
    QtAutoTransaction transaction(m_DB);
    QSharedPointer<db::CDBQuery> query = db::CreateQuery(m_DB);
    query->Prepare(
            "DELETE FROM oldi_clients "
            "WHERE client = :1");
    query->BindValue(":1", sSectorList[0]);
    TVA_CATCHALL_TRY
    {
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
        bOK = true;
    }
    TVA_CATCHALL(err)
    {
        bOK = false;
        qWarning() << err.reason();
    }
    if(!bOK) return false;

    if(!m_bIsFDP)
    {
        bOK = true;
        query->Prepare(
                "DELETE FROM oldi_clients "
                "WHERE sector = :1");
        TVA_CATCHALL_TRY
        {
            for(int i = 1; i < sSectorList.size(); ++i)
            {
                query->BindValue(":1", sSectorList[i]);
                TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
            }
        }
        TVA_CATCHALL(err)
        {
            bOK = false;
            qWarning() << err.reason();
        }
        if(!bOK) return false;
    }

    bOK = true;
    query->Prepare(
            "INSERT INTO oldi_clients(client, status, sector) "
            "VALUES(:1, :2, :3)");
    TVA_CATCHALL_TRY
    {
        for(int i = 1; i < sSectorList.size(); ++i)
        {
            query->BindValue(":1", sSectorList[0]);
            if(i == 1)
                query->BindValue(":2", 1);
            else
                query->BindValue(":2", 2);
            query->BindValue(":3", sSectorList[i]);
            TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
        }
    }
    TVA_CATCHALL(err)
    {
        bOK = false;
        qWarning() << err.reason();
    }
    if(!bOK)
    {
        if(m_bIsFDP)
        {
            if(dataSTS.iPriority)
            {
                if(1 == dataSTS.iPriority)
                {
                    query->Prepare(
                            "DELETE FROM oldi_clients "
                            "WHERE client = :1");
                    query->BindValue(":1", sSectorList[0]);
                    TVA_CATCHALL_TRY
                    {
                        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
                        bOK = true;
                    }
                    TVA_CATCHALL(err)
                    {
                        bOK = false;
                        qWarning() << err.reason();
                    }
                    if(!bOK) return false;

                    // load all data for analysis
                    OLDIClients::TClientMap mapCWP;
                    query->Prepare("SELECT client, status, sector FROM oldi_clients");
                    TVA_CATCHALL_TRY
                    {
                        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
                    }
                    TVA_CATCHALL(err)
                    {
                        qWarning() << err.reason();
                    }
                    while(query->next())
                    {
                        OLDIClients clData;
                        clData.SetClient(query->value(0).toString());
                        clData.SetStatus(query->value(1).toInt());
                        clData.SetSector(query->value(2).toString());

                        if(mapCWP[clData.GetClient()].size())
                        {
                            if(mapCWP[clData.GetClient()].first().GetStatus() > clData.GetStatus())
                                mapCWP[clData.GetClient()].prepend(clData);
                            else
                                mapCWP[clData.GetClient()].append(clData);
                        }
                        else
                            mapCWP[clData.GetClient()].append(clData);
                    }
                    OLDIClients::TClientMap mapCWPforDel;
                    OLDIClients::TClientMap mapSectorforAdd;
                    OLDIClients::TClientMap::iterator itCWP = mapCWP.begin();
                    for(; itCWP != mapCWP.end(); ++itCWP)
                    {
                        QList<OLDIClients>::iterator itSector = itCWP->begin();
                        while(itSector != itCWP->end())
                        {
                            bool bFind = false;
                            for(int i = 1; i < sSectorList.size(); ++i)
                            {
                                if(itSector->GetSector() == sSectorList[i])
                                {
                                    bFind = true;
                                    break;
                                }
                            }
                            if(bFind)
                            {
                                if(1 == itSector->GetStatus())
                                {
                                    QList<OLDIClients>::iterator itAssign = itCWP->begin();
                                    for(; itAssign != itCWP->end(); ++itAssign)
                                    {
                                        mapCWPforDel[itAssign->GetClient()].append(*itAssign);
                                        OLDIClients clData;
                                        clData.SetClient(sSectorList[0]);
                                        clData.SetSector(itAssign->GetSector());
                                        clData.SetStatus(2);
                                        mapSectorforAdd[clData.GetClient()].append(clData);
                                    }
                                    itCWP->clear();
                                    itSector = itCWP->end();
                                }
                                else
                                {
                                    mapCWPforDel[itSector->GetClient()].append(*itSector);
                                    OLDIClients clData;
                                    clData.SetClient(sSectorList[0]);
                                    clData.SetSector(itSector->GetSector());
                                    clData.SetStatus(2);
                                    mapSectorforAdd[clData.GetClient()].append(clData);
                                    itSector = itCWP->erase(itSector);
                                }
                            }
                            else
                                ++itSector;
                        }
                    }
                    TVA_CATCHALL_TRY
                    {
                        query->Prepare("DELETE FROM oldi_clients WHERE client = :1 AND sector = :2 AND status = :3");
                        itCWP = mapCWPforDel.begin();
                        for(; itCWP != mapCWPforDel.end(); ++itCWP)
                        {
                            listCWPforUpdate.append(itCWP.key());
                            QList<OLDIClients>::iterator itSector = itCWP->begin();
                            for(; itSector != itCWP->end(); ++itSector)
                            {
                                query->BindValue(":1", itSector->GetClient());
                                query->BindValue(":2", itSector->GetSector());
                                query->BindValue(":3", itSector->GetStatus());
                                TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
                                if(1 == itSector->GetStatus())
                                {
                                    // send close
                                    OLDIMesDataSTS mesDataSTS;
                                    mesDataSTS.iMesNum = GetNextMesNum();
                                    mesDataSTS.sFrom = m_sOwnerSenderName;
                                    mesDataSTS.sTo = itSector->GetClient();
                                    mesDataSTS.sText = "-" + itSector->GetClient();
                                    mesDataSTS.iPriority = 1;
                                    QSharedPointer<OLDIMessage> pMesSTS(new OLDIMesSTS(mesDataSTS));
                                    mapMes.insert(MesMapKey(mesDataSTS.iMesNum, mesDataSTS.sFrom, mesDataSTS.sTo), pMesSTS);
                                }
                            }
                        }
                        listCWPforUpdate.append(sSectorList[0]);
                        QMap<QString, QPair<int, QString> > mapSectorToInsert;
                        itCWP = mapSectorforAdd.begin();
                        for(; itCWP != mapSectorforAdd.end(); ++itCWP)
                        {
                            QList<OLDIClients>::iterator itSector = itCWP->begin();
                            for(; itSector != itCWP->end(); ++itSector)
                            {
                                mapSectorToInsert.insert(itSector->GetSector(),
                                                         QPair<int, QString>(2, itSector->GetSector()));
                            }
                        }
                        for(int i = 2; i < sSectorList.size(); ++i)
                        {
                            mapSectorToInsert.insert(sSectorList[i],
                                                     QPair<int, QString>(2, sSectorList[i]));
                        }
                        mapSectorToInsert.insert(sSectorList[1],
                                                 QPair<int, QString>(1, sSectorList[1]));
                        query->Prepare(
                                "INSERT INTO oldi_clients(client, status, sector) "
                                "VALUES(:1, :2, :3)");
                        QMap<QString, QPair<int, QString> >::const_iterator itInsSector = mapSectorToInsert.constBegin();
                        for(; itInsSector != mapSectorToInsert.constEnd(); ++itInsSector)
                        {
                            query->BindValue(":1", sSectorList[0]);
                            query->BindValue(":2", itInsSector->first);
                            query->BindValue(":3", itInsSector->second);
                            TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
                        }

                        transaction.commit();
                        return true;
                    }
                    TVA_CATCHALL(err)
                    {
                        qWarning() << err.reason();
                        mapMes.clear();
                        //send error
                        OLDIMesDataERR dataERR;
                        dataERR.iMesNum = GetNextMesNum();
                        dataERR.sFrom = dataSTS.sTo;
                        dataERR.sTo = dataSTS.sFrom;
                        dataERR.iAnwMesNum = dataSTS.iMesNum;
                        dataERR.sAnwFrom = dataSTS.sFrom;
                        dataERR.sAnwTo = dataSTS.sTo;
                        dataERR.sListError.append("001");
                        QSharedPointer<OLDIMessage> pMesERR(new OLDIMesERR(dataERR));
                        mapMes.insert(MesMapKey(dataERR.iMesNum, dataERR.sFrom, dataERR.sTo), pMesERR);
                        return false;
                    }
                }
            }
            else
            {
                //send error
                OLDIMesDataERR dataERR;
                dataERR.iMesNum = GetNextMesNum();
                dataERR.sFrom = dataSTS.sTo;
                dataERR.sTo = dataSTS.sFrom;
                dataERR.iAnwMesNum = dataSTS.iMesNum;
                dataERR.sAnwFrom = dataSTS.sFrom;
                dataERR.sAnwTo = dataSTS.sTo;
                dataERR.sListError.append("001");
                QSharedPointer<OLDIMessage> pMesERR(new OLDIMesERR(dataERR));
                mapMes.insert(MesMapKey(dataERR.iMesNum, dataERR.sFrom, dataERR.sTo), pMesERR);
                return false;
            }
        }
    }
    else
        transaction.commit();
    return true;
}
