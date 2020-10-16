#include "ListSILAPPDEP.h"

#include "front/core/GUIAirplane.h"
#include "ControlDlg.h"
#include "ACTDlgDEP.h"
#include "ListACItemDelegate.h"
#include "back/Master.h"
#include "main.h"
#include "OLDIMessages.h"

#include "proto/obelix/CmdChangeSFPL.h"

#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>

CListSIL_APP_DEP::CListSIL_APP_DEP( CMaster* pMaster, QWidget* pParent) :
		CListAC(pMaster, pParent)
{
    QMap<int,int> mapCol;
    mapCol.insert(CListACModel::ACID, CControlDlg::FPLType);
    mapCol.insert(CListACModel::CODE, CControlDlg::CODEType);
    mapCol.insert(CListACModel::ETD, CControlDlg::ETDType);
    CListACItemDelegate* pDelegate = new CListACItemDelegate(mapCol);
    m_list.setItemDelegate(pDelegate);
    connect(pDelegate, SIGNAL(itemEdit(const QModelIndex, bool&)), this, SLOT(OnItemEdit(const QModelIndex, bool&)));

    setListType(QString("SIL_APP_DEP"));
//    CListACSortModel::TListColType listCol(COL_COUNT);
//    for(int i = 0; i < COL_COUNT; ++i)
//    {
//        switch(i)
//        {
//        case ACID: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ACID, true); break;
//        case CODE: listCol[i] = CListACSortModel::CTypeCol(CListACModel::CODE, true); break;
//        case SID: listCol[i] = CListACSortModel::CTypeCol(CListACModel::SID, false); break;
//        case ETD: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETD, true); break;
//        case PEL: listCol[i] = CListACSortModel::CTypeCol(CListACModel::PEL, false); break;
//        case COPout: listCol[i] = CListACSortModel::CTypeCol(CListACModel::COPout, false); break;
//        case Type: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Type, false); break;
//        case Wct: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Wct, false); break;
//        }
//    }
//    m_sortModel.SetColumns(listCol);

    fillSortModel();
    m_sortModel.SetColEdit(CListACModel::ACID, true);
    m_sortModel.SetColEdit(CListACModel::CODE, true);
    m_sortModel.SetColEdit(CListACModel::ETD, true);

    m_sortModel.SetRowFilterType(CListACSortModel::TYPE_SIL_DEP);

    m_ControlsLayout->addStretch(1);
    m_butACT = new QPushButton(this);
    m_ControlsLayout->addWidget(m_butACT);

    connect(m_butACT, SIGNAL(clicked()), this, SLOT(OnButtonACT()));

    connect(&m_list, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(OnDbClickItemEdit(const QModelIndex&))); //rem this for single click editing

    Translate();
    UpdateViewData();
}

CListSIL_APP_DEP::~CListSIL_APP_DEP()
{
}

void CListSIL_APP_DEP::Translate()
{
    m_butACT->setText(tr("ACT"));
    SizeToContents();
}

int CListSIL_APP_DEP::InsertRow(const QSharedPointer<CAirplane>& pAirplane)
{
	return -1;
}

void CListSIL_APP_DEP::ModifyRow(int row, const QSharedPointer<CAirplane>& pAirplane)
{
}

bool CListSIL_APP_DEP::IsCanDeleteRow(int row, const QSharedPointer<CAirplane> &pAirplane)
{
	return false;
}

void CListSIL_APP_DEP::OnItemEdit(const QModelIndex index, bool& bEdit)
{
    if(index.column() == CListACModel::ACID)
    {
        QSharedPointer<CSFPL> pSFPL = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
        ItemEditACID(pSFPL);
    }
    if(index.column() == CListACModel::ETD)
    {
        bEdit = true;
    }
}

void CListSIL_APP_DEP::OnButtonACT()
{
    CACTDlgDEP dlg(m_pMaster, 0,0, this);
    if(dlg.exec() == QDialog::Accepted)
    {
        QSharedPointer<CSFPL> pSFPL = dlg.GetSFPL();
        if(pSFPL)
        {
            QSharedPointer<obelix::CCmdChangeSFPL> pCmd(new obelix::CCmdChangeSFPL(obelix::CCmdChangeSFPL::DEP));
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_CODE, QString::number(pSFPL->GetCODE()));
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPIN, pSFPL->GetCOPin());
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_ETOIN, pSFPL->GetETOin().toString());
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPINFL, pSFPL->GetCOPinFL().toString());
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_FPLID, QString::number(pSFPL->GetID()));
            m_pMaster->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
        }
    }
}

void CListSIL_APP_DEP::OnDbClickItemEdit(const QModelIndex& index)
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
