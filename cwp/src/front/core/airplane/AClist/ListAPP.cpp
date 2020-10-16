#include "ListAPP.h"

#include "front/core/GUIAirplane.h"
#include "ControlDlg.h"
#include "ListACItemDelegate.h"

#include "main.h"

#include <QMessageBox>

#include "proto/obelix/CmdChangeSFPL.h"

CListAPP::CListAPP( CMaster* pMaster, QWidget* pParent) :
		CListAC(pMaster, pParent)
{
    QMap<int,int> mapCol;
    mapCol.insert(CListACModel::ACID, CControlDlg::FPLType);
    mapCol.insert(CListACModel::CODE, CControlDlg::CODEType);
    mapCol.insert(CListACModel::ETO_ATO_ATA, CControlDlg::ETDType);
    CListACItemDelegate* pDelegate = new CListACItemDelegate(mapCol);
    m_list.setItemDelegate(pDelegate);
    connect(pDelegate, SIGNAL(itemEdit(const QModelIndex, bool&)), this, SLOT(OnItemEdit(const QModelIndex, bool&)));

    setListType(QString("APP"));
//    CListACSortModel::TListColType listCol(COL_COUNT);
//    for(int i = 0; i < COL_COUNT; ++i)
//    {
//        switch(i)
//        {
//        case ACID: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ACID, true); break;
//        case CODE: listCol[i] = CListACSortModel::CTypeCol(CListACModel::CODE, true); break;
//        case COPin: listCol[i] = CListACSortModel::CTypeCol(CListACModel::COPin_SID, false); break;
//        case ETOin: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETO_ATO_ETD, false); break;
//        case PEL: listCol[i] = CListACSortModel::CTypeCol(CListACModel::PEL_CFL, false); break;
//        case COPout: listCol[i] = CListACSortModel::CTypeCol(CListACModel::COPout_STAR, false); break;
//        case ETOout: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETO_ATO_ATA, true); break;
//        case RFL: listCol[i] = CListACSortModel::CTypeCol(CListACModel::RFL_TFL, false); break;
//        case Type: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Type, false); break;
//        case Wct: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Wct, false); break;
//        case Stand: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Stand, false); break;
//        case REM: listCol[i] = CListACSortModel::CTypeCol(CListACModel::REM, false); break;
//        }
//    }
//    m_sortModel.SetColumns(listCol);

    fillSortModel();
    m_sortModel.SetColEdit(CListACModel::ACID, true);
    m_sortModel.SetColEdit(CListACModel::CODE, true);
    m_sortModel.SetColEdit(CListACModel::ETO_ATO_ATA, true);

    connect(&m_list, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(OnDbClickItemEdit(const QModelIndex&))); //rem this for single click editing

    Translate();
    UpdateViewData();
}

CListAPP::~CListAPP()
{
}

void CListAPP::Translate()
{
	SizeToContents();
}

int CListAPP::InsertRow(const QSharedPointer<CAirplane>& pAirplane)
{
	return -1;
}

void CListAPP::ModifyRow(int row, const QSharedPointer<CAirplane>& pAirplane)
{
}

bool CListAPP::IsCanDeleteRow(int row, const QSharedPointer<CAirplane> &pAirplane)
{
	return false;
}

void CListAPP::OnItemEdit(const QModelIndex index, bool& bEdit)
{
    if(index.column() == CListACModel::ACID)
    {
        QSharedPointer<CSFPL> pSFPL = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
        ItemEditACID(pSFPL);
    }
    if(index.column() == CListACModel::ETO_ATO_ATA)
    {
        CListACData itemData = index.data(Qt::UserRole).value<CListACData>();
        if(itemData.GetAirplane())
        {
            if(CAirplaneData::ASSUME == itemData.GetAirplane()->GetPlaneType())
            {
                QStringList sAirportList = networkSettings()->value("users/current_sector_airports", "").toString().split(",", QString::SkipEmptyParts);
                if(itemData.GetAirplane()->GetSFPL() && sAirportList.contains(itemData.GetAirplane()->GetSFPL()->GetDestAerodrome()))
                {
                    bEdit = true;
                }
            }
        }
    }
}

void CListAPP::OnDbClickItemEdit(const QModelIndex& index)
{
    if(index.isValid())
    {
        if(index.column() == CListACModel::ACID)
        {
            QSharedPointer<CSFPL> pSFPL = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
            ItemEditACID(pSFPL);
        }
    }
}
