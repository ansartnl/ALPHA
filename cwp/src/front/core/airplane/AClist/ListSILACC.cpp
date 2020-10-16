#include "ListSILACC.h"

#include "GUIAirplane.h"
#include "ControlDlg.h"
#include "ListACItemDelegate.h"

#include "main.h"

#include <QMessageBox>

CListSIL_ACC::CListSIL_ACC( CMaster* pMaster, QWidget* pParent) :
		CListAC(pMaster, pParent)
{
	QMap<int,int> mapCol;
    mapCol.insert(CListACModel::ACID, CControlDlg::FPLType);
    mapCol.insert(CListACModel::CODE, CControlDlg::CODEType);
	CListACItemDelegate* pDelegate = new CListACItemDelegate(mapCol);
	m_list.setItemDelegate(pDelegate);
    connect(pDelegate, SIGNAL(itemEdit(const QModelIndex, bool&)), this, SLOT(OnItemEdit(const QModelIndex, bool&)));

    setListType(QString("SIL_ACC"));
//	CListACSortModel::TListColType listCol(COL_COUNT);
//	for(int i = 0; i < COL_COUNT; ++i)
//	{
//		switch(i)
//		{
//		case ACID: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ACID, true); break;
//		case CODE: listCol[i] = CListACSortModel::CTypeCol(CListACModel::CODE, true); break;
//		case COPin: listCol[i] = CListACSortModel::CTypeCol(CListACModel::COPin, false); break;
//		case ETO: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETOin, false); break;
//		case PEL: listCol[i] = CListACSortModel::CTypeCol(CListACModel::PEL, false); break;
//		case COPout: listCol[i] = CListACSortModel::CTypeCol(CListACModel::COPout, false); break;
//		case RVSM: listCol[i] = CListACSortModel::CTypeCol(CListACModel::RVSM, false); break;
//		}
//	}
//	m_sortModel.SetColumns(listCol);

    fillSortModel();
    m_sortModel.SetColEdit(CListACModel::ACID, true);
    m_sortModel.SetColEdit(CListACModel::CODE, true);

	m_sortModel.SetRowFilterType(CListACSortModel::TYPE_SIL);

    connect(&m_list, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(OnDbClickItemEdit(const QModelIndex&))); //rem this for single click editing

    Translate();
    UpdateViewData();
}

CListSIL_ACC::~CListSIL_ACC()
{
}

void CListSIL_ACC::Translate()
{
	SizeToContents();
}

int CListSIL_ACC::InsertRow(const QSharedPointer<CAirplane>& pAirplane)
{
	return -1;
}

void CListSIL_ACC::ModifyRow(int row, const QSharedPointer<CAirplane>& pAirplane)
{
}

bool CListSIL_ACC::IsCanDeleteRow(int row, const QSharedPointer<CAirplane> &pAirplane)
{
	return false;
}

void CListSIL_ACC::TimerSlot()
{
}

void CListSIL_ACC::OnItemEdit(const QModelIndex index, bool& bEdit)
{
    if(index.column() == CListACModel::ACID)
	{
		QSharedPointer<CSFPL> pSFPL = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
        ItemEditACID(pSFPL);
    }
}

void CListSIL_ACC::OnDbClickItemEdit(const QModelIndex& index)
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
