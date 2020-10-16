#include "ListACModel.h"
#include "front/core/GUIAirplane.h"

#include "main.h"

#include <QTimer>
#include <QBrush>
#include <QBitmap>
#include <QMessageBox>
#include <QFont>

#include "back/Master.h"
#include "front/core/GUIView.h"
#include "OLDIProc.h"
#include "OLDIMessages.h"

#include "proto/obelix/CmdChangeSFPL.h"

#include "XMasterWindow.h"
#include "utils/ProfileSettings.h"

#include "back/ground/GroundStand.h"

bool LessThan(CListACData& p1, CListACData& p2)
{
	/*QSharedPointer<CAirplane> pPlane1 = p1.GetAirplane();
	QSharedPointer<CAirplane> pPlane2 = p2.GetAirplane();
	if(pPlane1)
	{
		if(pPlane2)
		{
            if(pPlane1->GetPlaneType() != pPlane2->GetPlaneType())
			{
                return (pPlane1->GetPlaneType() < pPlane2->GetPlaneType());
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(pPlane2)
		{
			return true;
		}
	}*/
	const QSharedPointer<CSFPL> &pSFPL1 = p1.GetSFPL();
	const QSharedPointer<CSFPL> &pSFPL2 = p2.GetSFPL();
	if (pSFPL1 && pSFPL2)
    {
        /*bool bIn = true;
        if(pPlane1 && pPlane1->GetPlaneType() != CAirplaneData::ACT)
            bIn = false;
        if(bIn)
        {
            QStringList sCurSectorsList = networkSettings()->value("users/current_sectors_short_name", "").toString().split("-", QString::SkipEmptyParts);
            bool bIn1 = sCurSectorsList.contains(pSFPL1->GetInSector());
            bool bIn2 = sCurSectorsList.contains(pSFPL2->GetInSector());
            if(bIn1 != bIn2)
                return bIn2;
        }*/

        if (pSFPL1->GetCODE() == pSFPL2->GetCODE()) {
            if (pSFPL1->GetACID() != pSFPL2->GetACID())
                return pSFPL1->GetACID() < pSFPL2->GetACID();

            return pSFPL1->GetUid() < pSFPL2->GetUid();
        }

        return pSFPL1->GetCODE() < pSFPL2->GetCODE();
    }

    qDebug() << "LessThan error";
	return false;
}

CListACSortModel::CListACSortModel(QObject *parent) :
	QSortFilterProxyModel(parent),
	m_RowFilterType(TYPE_None),
	show_all_(true)
{
	setDynamicSortFilter(true);
}

void CListACSortModel::SetColumns(const TListColType& pListCol)
{
	m_listCol = pListCol;
    invalidate();
}

void CListACSortModel::SetColEdit(int index, bool bEdit)
{
    if(index >= 0 && index < m_listCol.size())
        m_listCol[index].SetIsEdit(bEdit);
}

Qt::ItemFlags CListACSortModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags fl(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	if(index.isValid() && index.column() < m_listCol.size())
		if(m_listCol[index.column()].GetIsEdit()) fl |= Qt::ItemIsEditable;
    return fl;
}

QVariant CListACSortModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	CListACModel *source_model = dynamic_cast<CListACModel*>(sourceModel());

    switch (role) {
    case Qt::BackgroundRole:
        if (m_RowFilterType == TYPE_ACT_IN) {
            const CListACData &data = source_model->GetItemData(mapToSource(index).row());
            const QSharedPointer<CSFPL> &sfpl = data.GetSFPL();

            const QStringList controlled_sectors = networkSettings()->value("users/current_sectors_short_name").toString().split("-", QString::SkipEmptyParts);
            for (const QString &cs : controlled_sectors) {
                if (cs == sfpl->GetNextSector()) {
                    return QColor::fromRgb(19, 149, 247);
                }
            }

            return QVariant::fromValue(Qt::lightGray);
        }
        break;
    default:
        break;
    }

    return QSortFilterProxyModel::data(index, role);
}

void CListACSortModel::setShowAll(bool show_all)
{
	show_all_ = show_all;
    invalidateFilter();
}

void CListACSortModel::SetRowFilterType(const RowFilterType filter_type)
{
	m_RowFilterType = filter_type;
}

bool CListACSortModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    return true;
}

bool CListACSortModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(TYPE_COST == m_RowFilterType)
    {
        CListACModel *source_model = dynamic_cast<CListACModel*>(sourceModel());
        const QModelIndex source_index = source_model->index(source_row, 0, source_parent);
        const CListACData &data = source_model->GetItemData(source_index.row());

        const QSharedPointer<CAirplane> &plane = data.GetAirplane();
        if (plane && (plane->trackStatus() == CAirplane::Nohope || plane->trackStatus() == CAirplane::Lost))
            return true;
        else
            return false;
    }
    else if (TYPE_ACT_IN == m_RowFilterType) {
        CListACModel *source_model = dynamic_cast<CListACModel*>(sourceModel());
        const QModelIndex source_index = source_model->index(source_row, 0, source_parent);
        const CListACData &data = source_model->GetItemData(source_index.row());

        const QSharedPointer<CAirplane> &plane = data.GetAirplane();
        if (plane && plane->GetPlaneType() == CAirplaneData::ASSUME) {
            return false;
        }

        const QSharedPointer<CSFPL> &sfpl = data.GetSFPL();

        const QStringList sectors = sfpl->GetAllSectors().split("-", QString::SkipEmptyParts);
        const QStringList controlled_sectors = networkSettings()->value("users/current_sectors_short_name").toString().split("-", QString::SkipEmptyParts);
        for (const QString &cs : controlled_sectors) {
            if (sectors.contains(cs) && cs != sfpl->GetCurSector() && cs != sfpl->GetPassedSector()) {
                if (show_all_ || cs == sfpl->GetNextSector()) {
                    return true;
                }
            }
        }

        return false;
    }
    else if (TYPE_ACT_OUT == m_RowFilterType)
    {
        CListACModel *source_model = dynamic_cast<CListACModel*>(sourceModel());
        const QModelIndex source_index = source_model->index(source_row, 0, source_parent);
        const CListACData &data = source_model->GetItemData(source_index.row());

        const QSharedPointer<CAirplane> &plane = data.GetAirplane();
        if (plane) {
            const CAirplaneData::PlaneType plane_type = plane->GetPlaneType();
            if ((CAirplaneData::ActOutInFir != plane_type) && (CAirplaneData::ActOut != plane_type)) {
                return false;
            }
        } else {
            return false;
        }
    }
    else if (TYPE_ACT_DEP == m_RowFilterType)
    {
        CListACModel *source_model = dynamic_cast<CListACModel*>(sourceModel());
        const QModelIndex source_index = source_model->index(source_row, 0, source_parent);
        const CListACData &data = source_model->GetItemData(source_index.row());
        const QSharedPointer<CAirplane> &plane = data.GetAirplane();
        if(plane)
            return false;

        const QSharedPointer<CSFPL> &sfpl = data.GetSFPL();
        if(sfpl)
        {
//            QStringList sAirportList = networkSettings()->value("users/current_sector_airports", "").toString().split(",", QString::SkipEmptyParts);
//            if(sAirportList.contains(pData.GetSFPL()->GetDepAerodrome()))
//                return true;
            if(!sfpl->GetCOPin().isEmpty() && (sfpl->GetCOPin() == sfpl->GetDepAerodrome()))
                return true;
            else
                return false;
        }
    }

		/*if (pPlane)
		{
			if (TYPE_ACT_IN == m_RowFilterType) {
				if (CAirplaneData::ACT != pPlane->GetPlaneType()) {
					return false;
				}
			} else {
				if (m_RowFilterType == TYPE_SIL || m_RowFilterType == TYPE_SIL_DEP || m_RowFilterType == TYPE_SIL_ARR)
					if (CAirplaneData::ACT != pPlane->GetPlaneType()) return false;
				if (CAirplaneData::Transfered == pPlane->GetPlaneType()) {
					if (pData.GetIPS_Big() > CListACData::GetDeltaIPS_Big()) {
						CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*>(pPlane.data());
						if (pGUIPlane) pGUIPlane->ActivatePlane(false);
						return false;
					}
				}
			}
		}
		if ((TYPE_ACT_IN == m_RowFilterType) && show_in_to_sector_) {
			if (pData.GetSFPL()) {
				QStringList cur_sectors = networkSettings()->value("users/current_sectors_short_name", "").toString().split("-", QString::SkipEmptyParts);
				if (!cur_sectors.contains(pData.GetSFPL()->GetInSector()))
					return false;
			}
		}
		if (TYPE_SIL == m_RowFilterType)
        {
            if(pData.GetSFPL())
            {
                QStringList sCurSectorsList = networkSettings()->value("users/current_sectors_short_name", "").toString().split("-", QString::SkipEmptyParts);
                if(!sCurSectorsList.contains(pData.GetSFPL()->GetInSector()))
                    return false;
            }
        }
        if(m_RowFilterType == TYPE_SIL_DEP)
        {
            if(pData.GetSFPL())
            {
                QStringList sAirportList = networkSettings()->value("users/current_sector_airports", "").toString().split(",", QString::SkipEmptyParts);
                if(sAirportList.contains(pData.GetSFPL()->GetDepAerodrome()))
                {
//                    if(pData.GetSFPL()->GetIsCOPin())
//                        return false;
//                    else
                        return true;
                }
                else
                    return false;
            }
        }
        if(m_RowFilterType == TYPE_SIL_ARR)
        {
            if(pData.GetSFPL())
            {
                QStringList sAirportList = networkSettings()->value("users/current_sector_airports", "").toString().split(",", QString::SkipEmptyParts);
                if(sAirportList.contains(pData.GetSFPL()->GetDestAerodrome()))
                    return true;
                else
                    return false;
            }
        }

        if(pData.GetSFPL())
        {
            if(!pData.GetAirplane() ||
               (pData.GetAirplane() && (CAirplaneData::ACT != pData.GetAirplane()->GetPlaneType()) && (CAirplaneData::ASSUME != pData.GetAirplane()->GetPlaneType())))
            {
                bool bContains = false;
                QStringList sCurSectorsList = networkSettings()->value("users/current_sectors_short_name", "").toString().split("-", QString::SkipEmptyParts);
                QStringList sSectorsList = pData.GetSFPL()->GetAllSectors().split("-", QString::SkipEmptyParts);
                for(int i = 0; !bContains && (i < sCurSectorsList.size()); ++i)
                {
                    bContains = sSectorsList.contains(sCurSectorsList.at(i));
                }
                if(!bContains)
                    return false;
            }
        }*/

	return true;
}

/*bool CListACSortModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	return false;
}*/

qint16 CListACModel::m_qEchelon = 50;
qint16 CListACModel::m_qPress = 1013;//760;

CListACModel::CListACModel(QObject *parent) :
    QAbstractTableModel(parent),
	AppSettings(),
	m_bScroll(false),
	m_pTimer(0),
	m_pMaster(0)
{
#ifndef _MSC_VER
	m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnTimerSlot()));
#endif
}

CListACModel::~CListACModel()
{
	Clear();
}

void CListACModel::WriteLog(const QSharedPointer<CAirplane>& pPlane, QString sType, QString sValue) const
{
	if (pPlane)
	{
		QString sCode;
		if (pPlane->GetMode3AReply() == 0)
			sCode = "none";
		else
            sCode = pPlane->GetMode3AReplyToString();
		LOG_OPER_STR(sCode, sType, sValue);
    }
}

void CListACModel::UpdateCustomColors()
{

    m_WarningColor = Qt::yellow;
    m_DangerColor = Qt::red;

    m_ActColor = QColor(100,100,100,120);
    m_AssumeColor = QColor(0,150,255,120);
    m_InColor = QColor(0,0,0,0);
    m_TransferedColor = QColor(100,100,100,120);

    globalSettings()->beginGroup(currentUserName);
    if(globalSettings()->allKeys().contains(QString("WarningColor"))){
        QDataStream stream(
                    QByteArray::fromHex(
                        globalSettings()->value(QString("WarningColor")).toByteArray()));
        stream >> m_WarningColor;
    }

    if(globalSettings()->allKeys().contains(QString("DangerColor"))){
        QDataStream stream(
                    QByteArray::fromHex(
                        globalSettings()->value(QString("DangerColor")).toByteArray()));
        stream >> m_DangerColor;
    }
    globalSettings()->endGroup();

    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    m_ActColor = stg.GetColor(UserProfile::CtListAct);
    m_AssumeColor = stg.GetColor(UserProfile::CtListAssume);
    m_InColor = stg.GetColor(UserProfile::CtListIn);
    m_TransferedColor = stg.GetColor(UserProfile::CtListTransfered);
}

void CListACModel::Clear()
{
#ifdef _MSC_VER
	if (!m_pTimer) {
		m_pTimer = new QTimer(this);
        connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnTimerSlot()));
	}
#endif

	beginResetModel();
	m_pTimer->stop();
	m_listData.clear();
    m_CurrentIndex = QModelIndex();
	endResetModel();
}

void CListACModel::SetModelData(const TListACData& list_data)
{
#ifdef _MSC_VER
	if (!m_pTimer) {
		m_pTimer = new QTimer(this);
		connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnTimerSlot()));
	}
#endif

	beginResetModel();
	m_pTimer->stop();

    // podpora, somewhere hanging link, memory leak
    foreach (const CListACData& data, m_listData) {
        const QSharedPointer<CSFPL> &sfpl = data.GetSFPL();
        if (sfpl)
            sfpl->SetSimTrack(QSharedPointer<SimulatorTrack>());
    }
    /////////////////////

    m_listData.clear();
    m_CurrentIndex = QModelIndex();
    m_listData = list_data;
    qSort(m_listData.begin(), m_listData.end(), LessThan);
    m_pTimer->start(6000);
	endResetModel();
}

CListACModel::TListACData& CListACModel::GetModelData()
{
	return m_listData;
}

CListACData& CListACModel::GetItemData(const QModelIndex& index)
{
	if (index.isValid() && (index.row() < m_listData.size()))
		return m_listData[index.row()];
	return m_emptyTempData;
}

CListACData& CListACModel::GetItemData(int row)
{
	return GetItemData(index(row, 0));
}

int CListACModel::rowCount(const QModelIndex &parent) const
{
	return m_listData.size();
}

int CListACModel::columnCount(const QModelIndex &parent) const
{
	return COL_COUNT;
}

void CListACModel::AddRecord(const CListACData &pData)
{
	beginInsertRows(QModelIndex(), m_listData.size(), m_listData.size()+1);
	m_listData.append(pData);
	endInsertRows();
}

void CListACModel::DelRecord(const CListACData &pData)
{
	TListACData::iterator itData = m_listData.begin();
	for (int i = 0; itData != m_listData.end(); ++i, ++itData)
	{
		if ((itData->GetAirplane() == pData.GetAirplane()) && (itData->GetSFPL() == pData.GetSFPL()))
        {
            QMetaObject::invokeMethod(this, "OnDelRecord", Qt::QueuedConnection, Q_ARG(int, i));
			return;
		}
	}
}

void CListACModel::OnDelRecord(int index)
{
    if (index >= 0 && index < m_listData.size())
    {
        beginRemoveRows(QModelIndex(), index, index);
        //beginResetModel();
        m_listData.remove(index);
        endRemoveRows();
        //endResetModel();
    }
}

void CListACModel::sort(int column, Qt::SortOrder order)
{
	emit layoutAboutToBeChanged();
	qSort(m_listData.begin(), m_listData.end(), LessThan);
    emit layoutChanged();
}

QVariant CListACModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	switch (role)
	{
	case Qt::TextAlignmentRole:
		return int(Qt::AlignLeft|Qt::AlignVCenter);
    case Qt::DisplayRole:
        return GetItemText(index);
    case Qt::EditRole:
        return GetItemEdit(index);
    case Qt::ForegroundRole:
		return GetItemForeground(index);
	case Qt::BackgroundRole:
		return GetItemBackground(index);
	case Qt::UserRole:
		{
			QVariant var;
			var.setValue(m_listData[index.row()]);
			return var;
		}
	default:
		return QVariant();
	}
	return QVariant();
}

bool CListACModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;

	if (role == Qt::EditRole)
    {
		switch (index.column())
		{
		case ACID:
			{
				return true;
			}
		case CODE:
			{
                QString sText = value.toString();
                sText = sText.rightJustified(4, '0');
                /*if (CAirplane::sListSpecCodes.contains(sText)) {
					// special codes
				}*/
				CListACData& itemData = m_listData[index.row()];
				if (itemData.GetAirplane())
				{
					QSharedPointer<CAirplane> pPlane = itemData.GetAirplane();
					WriteLog(pPlane, "CODE", sText);
					if (sText != tr("none"))
                        pPlane->ChangeMode3AReply(sText.toInt(0, 8));
					else
						pPlane->ChangeMode3AReply(0);
				}
                else
                {
                    QSharedPointer<CSFPL> pSFPL = itemData.GetSFPL();
                    if (pSFPL)
                    {
                        pSFPL->SetCODE(sText.toInt(0, 8));
                        TVA_CATCHALL_TRY
                        {
                            QSharedPointer<obelix::CCmdChangeSFPL> pCmd(new obelix::CCmdChangeSFPL(obelix::CCmdChangeSFPL::SetData));
                            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_CODE, sText);
                            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPIN, pSFPL->GetCOPin());
                            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_ETOIN, pSFPL->GetETOin().toString());
                            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPINFL, pSFPL->GetCOPinFL().toString());
                            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_FPLID, QString::number(pSFPL->GetID()));
                            m_pMaster->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());

//                            dao()->UpdateSFPL(pSFPL);

//                            if(m_pMaster)
//                            {
//                                OLDIMesDataCOD mesData;
//                                mesData.iMesNum = m_pMaster->GetOLDIProc()->GetNextMesNum();
//                                mesData.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
//                                mesData.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
//                                mesData.sACID = pSFPL->GetACID();
//                                mesData.sADEP = pSFPL->GetDepAerodrome();
//                                mesData.sADES = pSFPL->GetDestAerodrome();
//                                mesData.sCODE = sText;
//                                QSharedPointer<OLDIMessage> pMes(new OLDIMesCOD(mesData));
//                                //connect(pMes.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), this, SLOT(OLDIMesAnswer(const OLDIMessage*,const QStringList&)));
//                                //connect(pMes.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), this, SLOT(OLDIMesTimeout(const OLDIMessage*)));
//                                m_pMaster->GetOLDIProc()->SendMessage(pMes);

//                                QString sSectors = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
//                                QStringList sSectorsList = sSectors.split(",", QString::SkipEmptyParts);
//                                for(int i = 0; i < sSectorsList.size(); ++i)
//                                {
//                                    mesData.sTo = sSectorsList[i];
//                                    pMes = QSharedPointer<OLDIMessage>(new OLDIMesCOD(mesData));
//                                    //connect(pMes.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), this, SLOT(OLDIMesAnswer(const OLDIMessage*,const QStringList&)));
//                                    //connect(pMes.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), this, SLOT(OLDIMesTimeout(const OLDIMessage*)));
//                                    m_pMaster->GetOLDIProc()->SendMessage(pMes);
//                                }
//                            }
                        } TVA_CATCHALL(err)
                        {
                            QMessageBox::critical(getMainWindow(), tr("SFPL"), err.reason());
                        }
                    }
                }
				return true;
			}
		case PEL:
        case PEL_CFL:
            {
                CListACData& itemData = m_listData[index.row()];
                QSharedPointer<CAirplane> pPlane = itemData.GetAirplane();
                //if(pPlane && (pPlane->GetPlaneType() == CAirplaneData::ASSUME))
                if(pPlane && pPlane->IsOur())
                {
                    FLVal val = PressOnAirdromeToSee(value.value<FLVal>());
                    if(val != pPlane->GetCFL())
                    {
                        QString sText;
                        sText.sprintf("%03.0f", val.GetF());
                        WriteLog(pPlane, "CFL", sText);
                        pPlane->ChangeFL(val);
                    }
                }
                return true;
            }
        case RFL_TFL:
            {
                CListACData& itemData = m_listData[index.row()];
                QSharedPointer<CAirplane> pPlane = itemData.GetAirplane();
                //if(pPlane && (pPlane->GetPlaneType() == CAirplaneData::ASSUME))
                if(pPlane && pPlane->IsOur())
                {
                    FLVal val = PressOnAirdromeToSee(value.value<FLVal>());
                    if(val != pPlane->GetTFL())
                    {
                        QString sText;
                        sText.sprintf("%03.0f", val.GetF());
                        WriteLog(pPlane, "TFL", sText);
                        pPlane->ChangeTFL(val);
                    }
                }
                return true;
            }
        case ETD_ATD:
            {
                CListACData& itemData = m_listData[index.row()];
                QSharedPointer<CSFPL> pSFPL = itemData.GetSFPL();
                if(pSFPL)
                {
                    WriteLog(itemData.GetAirplane(), "ETD_ATD", value.toTime().toString("hh:mm"));
                    pSFPL->SetIsCOPin(true);
                    pSFPL->SetETOin(value.toTime());
                }
                return true;
            }
        case ETO_ATO_ATA:
            {
                CListACData& itemData = m_listData[index.row()];
                QSharedPointer<CSFPL> pSFPL = itemData.GetSFPL();
                if(pSFPL && !value.toString().isEmpty())
                    pSFPL->SetATA(value.toTime());
                /*CListACData& itemData = m_listData[index.row()];
                QSharedPointer<CSFPL> pSFPL = itemData.GetSFPL();
                if(pSFPL)
                {
                    WriteLog(itemData.GetAirplane(), "ATA", value.toTime().toString("hh:mm"));
                    DelRecord(itemData);
                }*/
                return true;
            }

        case STAR:
            {
                CListACData& itemData = m_listData[index.row()];
                QSharedPointer<CSFPL> pSFPL = itemData.GetSFPL();
                if(pSFPL && !value.toString().isEmpty())
                    pSFPL->SetSTAR(value.toString());
                /*CListACData& itemData = m_listData[index.row()];
                QSharedPointer<CSFPL> pSFPL = itemData.GetSFPL();
                if(pSFPL && pSFPL->GetFPLFrag() && !value.toString().isEmpty())
                {
                    if(pSFPL->GetFPLFrag()->GetProctFragList().size() && pSFPL->GetFPLFrag()->GetProctFragList().last()->GetType() == CProcFrag::enSTAR)
                    {
                        WriteLog(itemData.GetAirplane(), "STAR/RW", value.toString());
                        QString oldSTAR = pSFPL->GetFPLFrag()->GetProctFragList().last()->GetName();
                        QString newSTAR = value.toString();
                        QString oldRoute = pSFPL->GetRoute();
                        QStringList WordParts = pSFPL->GetRoute().split(QRegExp("\\s+"), QString::SkipEmptyParts);
                        if(WordParts.size() && WordParts.last() == oldSTAR)
                        {
                            WordParts.last() = newSTAR;
                            QString newRoute = WordParts.join(" ");
                            pSFPL->SetRoute(newRoute);
                            bool bRes = true;
                            if(aftnMessageProcessor)
                            {
                                QVariantMap map;
                                AFTNMessageProcessor::AftnMessageErrors errorCodes;
                                map.insert(AFTN_MESSAGE_ROUTE, newRoute);
                                bool bEmptyFIR = aftnMessageProcessor->FIRName().isEmpty();
                                if(bEmptyFIR)
                                    aftnMessageProcessor->setFIRName(pSFPL->GetFirName());
                                aftnMessageProcessor->validateType15(map, errorCodes);
                                if(!errorCodes.isEmpty())
                                {
                                    bRes = false;
                                    QStringList errors = aftnMessageProcessor->errorStrings(errorCodes);
                                    LOG_STR(WARNING, tr("Route error. %1").arg(errors.join("\n")));
                                }
                                else
                                {
                                    pSFPL->SetRoute(map.value(AFTN_MESSAGE_ROUTE).toString());
                                }
                                if(bEmptyFIR)
                                    aftnMessageProcessor->setFIRName("");
                            }
                            if(bRes)
                            {
                                TVA_CATCHALL_TRY
                                {
                                    dao()->UpdateSFPL(pSFPL);
                                    if(m_pMaster)
                                    {
                                        OLDIMesDataABI dataABI = OLDIMessage::GetABIfromSFPL(pSFPL);
                                        dataABI.iMesNum = m_pMaster->GetOLDIProc()->GetNextMesNum();
                                        dataABI.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                                        dataABI.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
                                        QSharedPointer<OLDIMessage> pMes(new OLDIMesABI(dataABI));
                                        m_pMaster->GetOLDIProc()->SendMessage(pMes);

                                        QString sSectors = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
                                        QStringList sSectorsList = sSectors.split(",", QString::SkipEmptyParts);
                                        for(int i = 0; i < sSectorsList.size(); ++i)
                                        {
                                            dataABI.sTo = sSectorsList[i];
                                            pMes = QSharedPointer<OLDIMessage>(new OLDIMesABI(dataABI));
                                            m_pMaster->GetOLDIProc()->SendMessage(pMes);
                                        }
                                        if(m_pMaster->GetView())
                                        {
                                            if(itemData.GetAirplane())
                                            {
                                                CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*>(itemData.GetAirplane().data());
                                                if (pGUIPlane)
                                                    pGUIPlane->ShowPathPoints(false);
                                            }
                                            CGUIView* pView = dynamic_cast<CGUIView*>(m_pMaster->GetView().data());
                                            if(pView)
                                                pView->OnShangedSFPL(pSFPL);
                                        }
                                    }
                                } TVA_CATCHALL(err)
                                {
                                    bRes = false;
                                    LOG_TVAERROR(WARNING, err);
                                }
                            }
                            if(!bRes)
                            {
                                pSFPL->SetRoute(oldRoute);
                            }
                        }
                    }
                }*/
                return true;
            }
        default: return false;
		}
    }
	return QAbstractTableModel::setData(index, value, role);
}

QVariant CListACModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
	{
		if ((section >= 0) && (section < COL_COUNT))
		{
			switch (section)
			{
			case ACID: return tr("ACID");
			case CODE: return tr("CODE");
			case COPin: return tr("COPin");
            case SID: return tr("SID");
            case COPin_SID: return tr("COPin nSID nRW");
			case ETOin: return tr("ETO");
            case ETD_ATD: return tr("ETD ATD");
            case ETO_ATOin: return tr("ETO ATO");
            case ETO_ATO_ETD: return tr("ETO ATO nETD ATD");
			case PEL: return tr("PEL");
            case PEL_CFL: return tr("PEL CFL");
			case COPout: return tr("COPout");
            case STAR: return tr("STAR");
            case COPout_STAR: return tr("COPout nSTAR nRW");
			case ETOout: return tr("ETO");
            case ETO_ATOout: return tr("ETO ATO");
            //case ETO_ATO_ATA: return tr("ETO ATO nETA ATA");
            case ETO_ATO_ATA: return tr("ATA");
            case RFL_TFL: return tr("RFL TFL");
            case Type:
            case Type_AC: return tr("TYPE");
			case Wct: return tr("Wct");
			case Stand: return tr("Stand");
			case ADEP: return tr("ADEP");
			case DEST: return tr("DEST");
			case ALTN: return tr("ALTN");
            case REM: return tr("WARN");
			case RVSM: return tr("RVSM");
            case Azimuth: return tr("Course Angle");
            case CurrentSpeed: return tr("Speed");
			case Eq: return tr("Eq");
            case STATUS: return tr("STATUS");
            case LIT: return tr("LIT");
            case FPLEqf1: return tr("FPLEqf1");
            case FPLEqf2: return tr("FPLEqf2");
            case FPLEqf3: return tr("FPLEqf3");
            case ETD: return tr("ETD");
            case ETA: return tr("ETA");
            case FType: return tr("FType");
            case MesType: return tr("MesType");
            case DATE: return tr("DATE");
            case Scroll:
            case None: return QVariant();
			}
		}
	}
	if((orientation == Qt::Horizontal) && (role == Qt::DecorationRole))
	{
		if((section >= 0) && (section < COL_COUNT))
		{
			if(section == Scroll)
			{
                //int iVal = headerData(section, orientation, Qt::UserRole).toInt();
                //QPixmap pixmap(iVal ? ":/Arrows/RightArrow" : ":/Arrows/LeftArrow");
                QPixmap pixmap(":/Arrows/TwoWayArrow");

                //pixmap.setMask(pixmap.createHeuristicMask());
				QVariant var;
				var.setValue(pixmap);
				return var;
			}
		}
	}
	if((orientation == Qt::Horizontal) && (role == Qt::UserRole))
    {
		if((section >= 0) && (section < COL_COUNT))
		{
			if(section == Scroll)
			{
                return m_bScroll;
			}
		}
    }
    if((orientation == Qt::Horizontal) && (role == Qt::UserRole+1))
        return section;

    if((orientation == Qt::Horizontal) && (role == Qt::FontRole))
    {
        QFont font1 = QFont();
        font1.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
        return font1;
    }

	return QAbstractTableModel::headerData(section, orientation, role);
}

bool CListACModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if ((orientation == Qt::Horizontal) && (role == Qt::UserRole))
    {
		if ((section >= 0) && (section < COL_COUNT))
		{
			if (section == Scroll)
			{
                m_bScroll = value.toBool();
                return true;
			}
		}
	}
	return QAbstractTableModel::setHeaderData(section, orientation, value, role);
}

QVariant CListACModel::GetItemText(const QModelIndex &index) const
{
	CListACData itemData = index.data(Qt::UserRole).value<CListACData>();
	QSharedPointer<CSFPL> pSFPL;
	if (itemData.GetAirplane())
        pSFPL = itemData.GetAirplane()->GetSFPL();
	else if (itemData.GetSFPL())
        pSFPL = itemData.GetSFPL();
	if (pSFPL)
	{
		switch (index.column())
		{
		case Eq:
		{
			QString eq = pSFPL->GetEquipment();
            return /*eq.isEmpty() ? "???" : */eq;
		}
		break;
		case ACID:
			{
				QString sText = pSFPL->GetACID();
				return sText;
			}
            break;
		case CODE:
			{
				QString sText = QString::number(pSFPL->GetCODE(), 8);
				sText = sText.rightJustified(4, '0');
				return sText;
			}
            break;
		case COPin:

		case COPin_SID:
			{
				QString sText = pSFPL->GetCOPin();
				return sText;
			}
            break;
		case ETOin:
        case ETD_ATD:
        case ETO_ATOin:
		case ETO_ATO_ETD:
			{
                if(pSFPL->GetETOin().isValid())
                {
                    QString sText = pSFPL->GetETOin().toString("hh:mm");
                    return sText;
                }
			}
            break;
        case PEL:
		case PEL_CFL:
			{
                FLVal flval;
                const QSharedPointer<CAirplane> &airplane = itemData.GetAirplane();
                if (airplane && airplane->GetCFL())
                {
                    flval = airplane->GetCFL();
                }
                else if (pSFPL->GetCOPinFL())
                {
                    flval = pSFPL->GetCOPinFL();
                }
                else
                {
                    flval = FLVal(pSFPL->GetLevelS());
                }
                QString sText = GetFLText(flval);
                return sText;
			}
			break;
		case COPout:
            {
                QString sText = pSFPL->GetCOPout();
                return sText;
            }
            break;
        case SID:
            {
                QString sText;
                sText = pSFPL->GetSID();
                return sText;
            }
            break;
        case STAR:
            {
                QString sText;
                sText = pSFPL->GetSTAR();
                return sText;
            }
            break;
            /*{
                QString sText;
                if (pSFPL->GetFPLFrag() && pSFPL->GetFPLFrag()->GetProctFragList().size() && pSFPL->GetFPLFrag()->GetProctFragList().last()->GetType() == CProcFrag::enSTAR)
                    sText = pSFPL->GetFPLFrag()->GetProctFragList().last()->GetName();
                return sText;
            }
            break;*/
        case COPout_STAR:
			{
				QString sText;
                if (pSFPL->GetFPLFrag() && pSFPL->GetFPLFrag()->GetProctFragList().size() && pSFPL->GetFPLFrag()->GetProctFragList().last()->GetType() == CProcFrag::enSTAR)
                    sText = pSFPL->GetFPLFrag()->GetProctFragList().last()->GetName();
                if (sText.isEmpty())
                    sText = pSFPL->GetCOPout();
				return sText;
			}
            break;
		case ETOout:
		case ETO_ATOout:
		case ETO_ATO_ATA:
            {
                if (pSFPL->GetATA().isValid())
                {
                    QString sText = pSFPL->GetATA().toString("hh:mm");
                    return sText;
                }
                /*if (pSFPL->GetETOout().isValid())
                {
                    QString sText = pSFPL->GetETOout().toString("hh:mm");
                    return sText;
                }*/
            }
            break;
		case RFL_TFL:
            {
                QString sText;
                if (itemData.GetAirplane() && itemData.GetAirplane()->GetTFL())
                {
                    sText = GetFLText(itemData.GetAirplane()->GetTFL());
                }
                return sText;
            }
			break;
		case Type:
            {
            QString sText = pSFPL->GetFlightType();
            return sText;
            }
		case Type_AC:
			{
				QString sText;
                //if(pSFPL->GetAirplanePtr())
                //    sText = pSFPL->GetAirplanePtr()->GetName();
                sText = pSFPL->GetTypeFlight();
				return sText;
			}
            break;
		case Wct:
			{
				QString sText = pSFPL->GetTurbulence();
				return sText;
			}
            break;
		case Stand:
            {
                QString sText = m_mapACID_Stand.value(pSFPL->GetACID());
                return sText;
            }
			break;
		case ADEP:
			{
				QString adep = pSFPL->GetDepAerodrome();
				return adep.isEmpty() ? "???" : adep;
			}
            break;
		case DEST:
			{
				QString dest = pSFPL->GetDestAerodrome();
				return dest.isEmpty() ? "???" : dest;
			}
            break;
		case ALTN:
			{
				QString sText = pSFPL->GetAlt1Aerodrome() + " " + pSFPL->GetAlt2Aerodrome();
				return sText;
			}
            break;
		case REM:
            {
                QString sText = itemData.GetWRN().GetText();
                return sText;
            }
            break;
        case RVSM:
			{
				QString sText = itemData.GetRVSM();
				return sText;
			}
            break;
        case Azimuth:
            {
                QString sText;
                if(itemData.GetAirplane())
                    sText = QString::number(itemData.GetAirplane()->GetAzimuth());
                return sText;
            }
            break;
        case CurrentSpeed:
            {
                QString sText;
                if(itemData.GetAirplane())
                {
                    double dVal = itemData.GetAirplane()->GetGS() * TransformM_SEC();
                    sText.sprintf("%03.0f", dVal);
                }
                return sText;
            }
            break;
        case STATUS:
        {
            QString status = pSFPL->GetStatus();
            return status.isEmpty() ? "???" : status;
        }
        break;
        case LIT:
        {
            QString Lit = pSFPL->GetLit();
            return Lit.isEmpty() ? "???" : Lit;
        }
            break;
        case FPLEqf1:
        {
            QString FPLEqf1 = pSFPL->GetFPLEqf1();
            return FPLEqf1.isEmpty() ? "???" : FPLEqf1;
        }
            break;
        case FPLEqf2:
        {
            QString FPLEqf2 = pSFPL->GetFPLEqf2();
            return FPLEqf2.isEmpty() ? "???" : FPLEqf2;
        }
            break;
        case ETD:

        case ETA:

        case FType:
        {
            QString FType = pSFPL->GetFType();
            return FType.isEmpty() ? "???" : FType;
        }
            break;
        case MesType:
        {
            QString MesType = pSFPL->GetMesType();
            return MesType.isEmpty() ? "???" : MesType;
        }
            break;
        case DATE:
		case Scroll:
		case None:
			break;
		}
	}
	return QVariant();
}

double CListACModel::TransformM_SEC() const
{
    if(AppSettings::IsSISet(AppSettings::SIKnot))
        // m/s -> knots
        return 1 / 0.5144;
    else
        // m/s -> km/h
        return 3.6;
}

QVariant CListACModel::GetItemEdit(const QModelIndex &index) const
{
    CListACData itemData = index.data(Qt::UserRole).value<CListACData>();
    switch(index.column())
    {
    case PEL:
    case PEL_CFL:
        {
            QVariant var;
            if(itemData.GetAirplane() && itemData.GetAirplane()->GetCFL())
            {
                var.setValue(PressOnAirdromeFromSee(itemData.GetAirplane()->GetCFL()));
            }
            else
            {
                if(itemData.GetSFPL()->GetCOPinFL())
                {
                    var.setValue(PressOnAirdromeFromSee(itemData.GetSFPL()->GetCOPinFL()));
                }
            }
            return var;
        }
        break;
    case RFL_TFL:
        {
            QVariant var;
            if(itemData.GetAirplane() && itemData.GetAirplane()->GetTFL())
            {
                var.setValue(PressOnAirdromeFromSee(itemData.GetAirplane()->GetTFL()));
            }
            return var;
        }
        break;
    default: break;
    }
    return QVariant();
}

QVariant CListACModel::GetItemForeground(const QModelIndex &index) const
{
	QColor color(Qt::black);
    if (QSharedPointer<CAirplane> pPlane = m_listData[index.row()].GetAirplane()) {
        switch(pPlane->GetPlaneType())
		{
        case CAirplaneData::None: color = QColor(Qt::black); break;
		default: color = QColor(Qt::black); break;
		}
	}
	return QBrush(color);
}

QVariant CListACModel::GetItemBackground(const QModelIndex &index) const
{
	QBrush backBrush(Qt::NoBrush);
    /*QSharedPointer<CAirplane> pPlane = m_listData[index.row()].GetAirplane();
	if(pPlane)
	{
        switch(pPlane->GetPlaneType())
		{
        case CAirplaneData::ASSUME:
        case CAirplaneData::Transfer:
        case CAirplaneData::ActOutInFir:
        case CAirplaneData::ActOut:
            {
                backBrush = QBrush(m_AssumeColor);
            }
            break;
        case CAirplaneData::Transfered:
            {
                backBrush = QBrush(m_TransferedColor);
            }
            break;
        case CAirplaneData::ACT:
            {
                QStringList sCurSectorsList = networkSettings()->value("users/current_sectors_short_name", "").toString().split("-", QString::SkipEmptyParts);
                if(pPlane && pPlane->GetSFPL() && !sCurSectorsList.contains(pPlane->GetSFPL()->GetInSector()))
                {
                    backBrush = QBrush(m_ActColor);
                }
                else
                {
                    backBrush = QBrush(m_InColor);
                }
            }
            break;
        case CAirplaneData::None: break;
        }
	}

	switch(index.column())
	{
	case ACID:
		{

		}
		break;
	case CODE:
		{
			const CListACData& itemData = m_listData[index.row()];
            if(itemData.GetIPS_Small() > CListACData::GetDeltaIPS_Small())
			{
                //QColor color(Qt::yellow);
                QColor color(m_WarningColor);
				color.setAlpha(120);
				backBrush = QBrush(color);
			}
			if(pPlane && pPlane->GetSFPL())
			{
				if(pPlane->GetMode3AReply() != pPlane->GetSFPL()->GetCODE())
				{
                    //QColor color(Qt::red);
                    QColor color(m_DangerColor);
					color.setAlpha(120);
					backBrush = QBrush(color);
				}
			}
		}
		break;
	case ETOin:
	case ETD:
	case ETO_ATOin:
	case ETO_ATO_ETD:
		{
			const CListACData& itemData = m_listData[index.row()];
            if(itemData.GetSFPL() && !itemData.GetSFPL()->GetIsCOPin())
			{
                QTime timeETO(itemData.GetSFPL()->GetETOin());
                if(!timeETO.isNull() && timeETO.addSecs(CListACData::GetDeltaIPS_Big()) < QTime::currentTime())
                {
                    //QColor color(Qt::yellow);
                    QColor color(m_WarningColor);
                    color.setAlpha(120);
                    backBrush = QBrush(color);
                }
			}
//			if(pPlane && (pPlane->GetTempType() == 0  ))// || no COPin
//			{
//				if(pPlane->GetSFPL())
//				{
//					QTime timeETO(pPlane->GetSFPL()->GetETOin());
//					if(timeETO.addSecs(CListACData::GetDeltaIPS_Big()) < QTime::currentTime())
//					{
//						QColor color(Qt::yellow);
//						color.setAlpha(120);
//						backBrush = QBrush(color);
//					}
//				}
//			}
		}
		break;
	case PEL:
	case PEL_CFL:
		{
			if(pPlane && pPlane->GetCFL())
			{
                if(qAbs(pPlane->GetFL() - pPlane->GetCFL()) > networkSettings()->value("reminders/FL_dev", 1).toInt())
				{
                    //QColor color(Qt::yellow);
                    QColor color(m_WarningColor);
					color.setAlpha(120);
					backBrush = QBrush(color);
				}
			}
		}
		break;
    case REM:
        {
            const CListACData& itemData = m_listData[index.row()];
            if(!itemData.GetWRN().GetText().isEmpty())
            {
                //QColor color(Qt::red);
                QColor color(m_DangerColor);
                color.setAlpha(120);
                backBrush = QBrush(color);
            }
        }
        break;
	default: break;
	}
    if(backBrush == QBrush(QColor(0, 0, 0, 0)))
        backBrush = QBrush(Qt::NoBrush);*/
	return backBrush;
}

void CListACModel::OnTimerSlot()
{
	TListACData::iterator itData = m_listData.begin();
	for (; itData != m_listData.end(); ++itData) {
		++itData->GetIPS_Small();
		++itData->GetIPS_Big();
	}
	emit timerModel();
}

void CListACModel::Translate()
{
	emit headerDataChanged(Qt::Horizontal, 0, COL_COUNT-1);
}

QString CListACModel::GetFLText(const FLVal& val, bool bHundredMeters/* = false*/) const
{
    QString sRes;// = " - ";
    if (val)
    {
        FLVal flval(PressOnAirdromeFromSee(val));
        if (flval.isFeet())
        {
            // hundred feet
            sRes = QString("F%1").arg(flval.GetF() > 0 ? flval.GetF() : 0, 3, 'f', 0, QLatin1Char('0'));
        }
        else
        {
            if (bHundredMeters)
                //hundred meters
                sRes = QString("%1").arg(flval.GetM() > 0 ? flval.GetM()/AppSettings::CFL_METER_DIVIDER : 0, AppSettings::CFL_METER_CAPACITY, 'f', 0, QLatin1Char('0'));
            else
                //tens of meters
                sRes = QString("%1").arg(flval.GetM() > 0 ? flval.GetM()/AppSettings::METER_DIVIDER : 0, AppSettings::METER_CAPACITY, 'f', 0, QLatin1Char('0'));
        }
    }
    return sRes;
}

double CListACModel::PressOnAirdromeFromSee(double FL) const
{
    double height = FL;
    if (GetEchelon() > FL)
    {
        FL *= 100 * 0.3048; // hundred feet -> meters
        //QFE
        //height = FL - (760-GetPress())*11;
        //QNH
        height = FL + 8.25*(GetPress()-1013);
        height /= 100 * 0.3048;
    }
    return height;
}

FLVal CListACModel::PressOnAirdromeFromSee(const FLVal& val) const
{
    FLVal ret(val);
    if (ret && (GetEchelon() > ret.GetF()))
    {
        FLVal tmp(PressOnAirdromeFromSee(ret.GetF()));
        ret = FLVal(tmp.GetF(), tmp.GetM(), ret.isFeet());
    }
    return ret;
}

double CListACModel::PressOnAirdromeToSee(double FL) const
{
    double height = FL;
    if (GetEchelon() > FL)
    {
        FL *= 100 * 0.3048; // hundred feet -> meters
        //QFE
        //height = FL + (760-GetPress())*11;
        //QNH
        height = FL - 8.25*(GetPress()-1013);
        height /= 100 * 0.3048;
    }
    return height;
}

FLVal CListACModel::PressOnAirdromeToSee(const FLVal& val) const
{
    FLVal ret(val);
    if (ret && (GetEchelon() > ret.GetF()))
    {
        FLVal tmp(PressOnAirdromeToSee(ret.GetF()));
        ret = FLVal(tmp.GetF(), tmp.GetM(), ret.isFeet());
    }
    return ret;
}

void CListACModel::DeleteRowPlane(int row)
{
    beginResetModel();
    GetItemData(row).SetAirplane(QSharedPointer<CAirplane>());
    endResetModel();
}

void CListACModel::SetStand(const QMap<quint32, QSharedPointer<GroundStand> >& pListData)
{
    emit layoutAboutToBeChanged();
    m_mapACID_Stand.clear();
    QMap<quint32, QSharedPointer<GroundStand> >::const_iterator itMap = pListData.constBegin();
    for (; itMap != pListData.constEnd(); ++itMap)
        m_mapACID_Stand.insert((*itMap)->mFplName, (*itMap)->mStandName);
    emit layoutChanged();
}

void CListACModel::SetStand(const QSharedPointer<GroundStand>& pData)
{
    emit layoutAboutToBeChanged();
    m_mapACID_Stand[pData->mFplName] = pData->mStandName;
    emit layoutChanged();
}

bool CListACModel::isDoubleCode(CListACData& itemData)
{
    bool bRes = false;
    quint16 code1 = USHRT_MAX;
    if(itemData.GetAirplane())
        code1 = itemData.GetAirplane()->GetMode3AReply();
    else if(itemData.GetSFPL())
        code1 = itemData.GetSFPL()->GetCODE();
    if (code1 != USHRT_MAX)
    {
        QString sCode = QString::number(code1, 8);
        sCode = sCode.rightJustified(4, '0');
        if (!CAirplane::sListSpecCodes.contains(sCode))
        {
            TListACData::const_iterator itData = m_listData.constBegin();
            for (; itData != m_listData.constEnd(); ++itData)
            {
                if (itData->GetSFPL() != itemData.GetSFPL())
                {
                    quint16 code2 = USHRT_MAX;
                    if(itData->GetAirplane())
                        code2 = itData->GetAirplane()->GetMode3AReply();
                    else if(itData->GetSFPL())
                        code2 = itData->GetSFPL()->GetCODE();
                    if(code2 != USHRT_MAX)
                    {
                        if(code1 == code2)
                        {
                            bRes = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    return bRes;
}
