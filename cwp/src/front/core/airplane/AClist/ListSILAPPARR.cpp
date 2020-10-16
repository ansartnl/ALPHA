#include "ListSILAPPARR.h"
#include "GUIAirplane.h"
#include "ControlDlg.h"
#include "ACTDlgARR.h"
#include "ListACItemDelegate.h"
#include "back/Master.h"
#include "main.h"

#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>

#include "OLDIMessages.h"

#include "proto/obelix/CmdChangeSFPL.h"

CListSIL_APP_ARR::CListSIL_APP_ARR( CMaster* pMaster, QWidget* pParent) :
		CListAC(pMaster, pParent)
{
    QMap<int,int> mapCol;
    mapCol.insert(CListACModel::ACID, CControlDlg::FPLType);
    mapCol.insert(CListACModel::CODE, CControlDlg::CODEType);
    mapCol.insert(CListACModel::STAR, CControlDlg::STARType);
    CListACItemDelegate* pDelegate = new CListACItemDelegate(mapCol);
    m_list.setItemDelegate(pDelegate);
    connect(pDelegate, SIGNAL(itemEdit(const QModelIndex, bool&)), this, SLOT(OnItemEdit(const QModelIndex, bool&)));

    setListType(QString("SIL_APP_ARR"));
//    CListACSortModel::TListColType listCol(COL_COUNT);
//    for(int i = 0; i < COL_COUNT; ++i)
//    {
//        switch(i)
//        {
//        case ACID: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ACID, true); break;
//        case CODE: listCol[i] = CListACSortModel::CTypeCol(CListACModel::CODE, true); break;
//        case COPin: listCol[i] = CListACSortModel::CTypeCol(CListACModel::COPin, false); break;
//        case ETO: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETO_ATOin, false); break;
//        case PEL: listCol[i] = CListACSortModel::CTypeCol(CListACModel::PEL, false); break;
//        case COPout: listCol[i] = CListACSortModel::CTypeCol(CListACModel::STAR, true); break;
//        case Type: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Type, false); break;
//        case Wct: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Wct, false); break;
//        }
//    }
//    m_sortModel.SetColumns(listCol);

    fillSortModel();
    m_sortModel.SetColEdit(CListACModel::ACID, true);
    m_sortModel.SetColEdit(CListACModel::CODE, true);
    m_sortModel.SetColEdit(CListACModel::STAR, true);

    m_sortModel.SetRowFilterType(CListACSortModel::TYPE_SIL_ARR);

    m_ControlsLayout->addStretch(1);
    m_butACT = new QPushButton(this);
    m_ControlsLayout->addWidget(m_butACT);

    connect(m_butACT, SIGNAL(clicked()), this, SLOT(OnButtonACT()));

    connect(&m_list, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(OnDbClickItemEdit(const QModelIndex&))); //rem this for single click editing

    Translate();
    UpdateViewData();
}

CListSIL_APP_ARR::~CListSIL_APP_ARR()
{
}

void CListSIL_APP_ARR::Translate()
{
    m_butACT->setText(tr("ACT"));
	SizeToContents();
}

int CListSIL_APP_ARR::InsertRow(const QSharedPointer<CAirplane>& pAirplane)
{
	return -1;
}

void CListSIL_APP_ARR::ModifyRow(int row, const QSharedPointer<CAirplane>& pAirplane)
{
}

bool CListSIL_APP_ARR::IsCanDeleteRow(int row, const QSharedPointer<CAirplane> &pAirplane)
{
	return false;
}

void CListSIL_APP_ARR::OnItemEdit(const QModelIndex index, bool& bEdit)
{
    if(index.column() == CListACModel::ACID)
    {
        QSharedPointer<CSFPL> pSFPL = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
        ItemEditACID(pSFPL);
    }
}

void CListSIL_APP_ARR::OnButtonACT()
{
    CACTDlgARR dlg(m_pMaster, 0,0, this);
    if(dlg.exec() == QDialog::Accepted)
    {
        QSharedPointer<CSFPL> pSFPL = dlg.GetSFPL();
        if(pSFPL)
        {
            QSharedPointer<obelix::CCmdChangeSFPL> pCmd(new obelix::CCmdChangeSFPL(obelix::CCmdChangeSFPL::ARR));
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_CODE, QString::number(pSFPL->GetCODE()));
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPIN, pSFPL->GetCOPin());
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_ETOIN, pSFPL->GetETOin().toString());
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPINFL, pSFPL->GetCOPinFL().toString());
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_FPLID, QString::number(pSFPL->GetID()));
            m_pMaster->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
        }
    }
}

void CListSIL_APP_ARR::OnDbClickItemEdit(const QModelIndex& index)
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
