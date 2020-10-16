#include "ListACC.h"

#include "GUIAirplane.h"
#include "ControlDlg.h"
#include "ListACItemDelegate.h"
#include "ACTDlg.h"
#include "SFPL.h"
#include "Master.h"
#include "View.h"

#include "main.h"

#include <QHeaderView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>

#include "OLDIMessages.h"

#include "mesviewer.h"

#include "proto/obelix/CmdChangeSFPL.h"

CListACC::CListACC( CMaster* pMaster, QWidget* pParent) :
	CListAC(pMaster, pParent)
{
#ifdef _MSC_VER
	m_listView.append(this);
#endif

    QMap<int,int> mapCol;
    mapCol.insert(CListACModel::ACID, CControlDlg::FPLType);
    mapCol.insert(CListACModel::CODE, CControlDlg::CODEType);
    mapCol.insert(CListACModel::PEL_CFL, CControlDlg::CFLType);
    mapCol.insert(CListACModel::RFL_TFL, CControlDlg::CFLType);
    CListACItemDelegate* pDelegate = new CListACItemDelegate(mapCol);
    m_list.setItemDelegate(pDelegate);
    connect(pDelegate, SIGNAL(itemEdit(const QModelIndex, bool&)), this, SLOT(OnItemEdit(const QModelIndex, bool&)));

    setListType(QString("ACC"));
//    CListACSortModel::TListColType listCol(COL_COUNT + 1);
//    for(int i = 0; i < COL_COUNT + 1; ++i)
//    {
//        switch(i)
//        {
//        case ACID: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ACID, true); break;
//        case CODE: listCol[i] = CListACSortModel::CTypeCol(CListACModel::CODE, true); break;
//        case COPin: listCol[i] = CListACSortModel::CTypeCol(CListACModel::COPin, false); break;
//        case ETOin: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETO_ATOin, false); break;
//        case PEL: listCol[i] = CListACSortModel::CTypeCol(CListACModel::PEL_CFL, true); break;
//        case COPout: listCol[i] = CListACSortModel::CTypeCol(CListACModel::COPout, false); break;
//        case ETOout: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETO_ATOout, false); break;
//        case RFL: listCol[i] = CListACSortModel::CTypeCol(CListACModel::RFL_TFL, true); break;
//        case REM: listCol[i] = CListACSortModel::CTypeCol(CListACModel::REM, false); break;
//        case RVSM: listCol[i] = CListACSortModel::CTypeCol(CListACModel::RVSM, false); break;
//        case Type: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Type_AC, false); break;
//        case ADEP: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ADEP, false); break;
//        case DEST: listCol[i] = CListACSortModel::CTypeCol(CListACModel::DEST, false); break;
//        case ALTN: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ALTN, false); break;
//        case Scroll: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Scroll, false); break;
//        }
//    }
//    m_sortModel.SetColumns(listCol);
    fillSortModel();
    m_sortModel.SetColEdit(CListACModel::ACID, true);
    m_sortModel.SetColEdit(CListACModel::CODE, true);
    m_sortModel.SetColEdit(CListACModel::PEL_CFL, true);
    m_sortModel.SetColEdit(CListACModel::RFL_TFL, true);

    m_ControlsLayout->addStretch(1);
    m_butMes = new QPushButton(this);
    m_ControlsLayout->addWidget(m_butMes);
    m_butACT = new QPushButton(this);
    m_ControlsLayout->addWidget(m_butACT);

//    connect(m_list.horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(OnScrollItem(int)));
//    m_sortModel.setHeaderData(m_list.horizontalHeader()->visualIndex(CListACModel::Scroll), Qt::Horizontal, 0, Qt::UserRole);
    //OnScrollItem(Scroll);

    Translate();

    connect(m_butACT, SIGNAL(clicked()), this, SLOT(OnButtonACT()));
    connect(m_butMes, SIGNAL(clicked()), this, SLOT(OnButtonMes()));

    connect(&m_list, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(OnDbClickItemEdit(const QModelIndex&))); //rem this for single click editing

    UpdateViewData();
}

CListACC::~CListACC()
{
}

void CListACC::Translate()
{
    m_butACT->setText(tr("ACT"));
    m_butMes->setText(tr("Messages"));
    SizeToContents();
}

int CListACC::InsertRow(const QSharedPointer<CAirplane>& pAirplane)
{
    return -1;
}

void CListACC::ModifyRow(int row, const QSharedPointer<CAirplane>& pAirplane)
{
    if(pAirplane->GetSFPL())
    {
        if(pAirplane->GetSFPL()->GetFPLFrag())
        {
            CProcFrag::TPointFragList pPoint = pAirplane->GetSFPL()->GetFPLFrag()->GetSectorPoints(m_pMaster->GetView());
            if(pPoint.size() && pAirplane->GetSFPL()->GetCOPin() != pPoint.last()->GetPoint()->GetName())
            {
                pAirplane->GetSFPL()->SetCOPoutPoint(pPoint.last());
                pAirplane->GetSFPL()->SetCOPout(pPoint.last()->GetPoint()->GetName());
                QTime dt(QTime::currentTime());
                dt = dt.addSecs(pPoint.last()->GetDSec());
                pAirplane->GetSFPL()->SetETOout(dt);
            }
            else
            {
                pAirplane->GetSFPL()->SetCOPoutPoint(QSharedPointer<CPointFrag>());
                pAirplane->GetSFPL()->SetCOPout("");
                pAirplane->GetSFPL()->SetETOout(QTime());
                pAirplane->GetSFPL()->SetIsCOPout(false);
            }
        }
    }
}

void CListACC::OnButtonACT()
{
    CACTDlg dlg(m_pMaster, 0,0, this);
    if(dlg.exec() == QDialog::Accepted)
    {
        QSharedPointer<CSFPL> pSFPL = dlg.GetSFPL();
        if(pSFPL)
        {
            QSharedPointer<obelix::CCmdChangeSFPL> pCmd(new obelix::CCmdChangeSFPL(obelix::CCmdChangeSFPL::ACC));
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_CODE, QString::number(pSFPL->GetCODE()));
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPIN, pSFPL->GetCOPin());
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_ETOIN, pSFPL->GetETOin().toString());
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPINFL, pSFPL->GetCOPinFL().toString());
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_FPLID, QString::number(pSFPL->GetID()));
            m_pMaster->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
        }
    }
}

//void CListACC::OnScrollItem(int index)
//{
//    if(index == Scroll)
//    {
//        if(m_sortModel.headerData(Scroll, Qt::Horizontal, Qt::UserRole).toBool())
//        {
//            m_sortModel.setHeaderData(Scroll, Qt::Horizontal, 0, Qt::UserRole);
//            m_list.showColumn(Type);
//            m_list.showColumn(ADEP);
//            m_list.showColumn(DEST);
//            m_list.showColumn(ALTN);
//        }
//        else
//        {
//            m_sortModel.setHeaderData(Scroll, Qt::Horizontal, 1, Qt::UserRole);
//            m_list.hideColumn(Type);
//            m_list.hideColumn(ADEP);
//            m_list.hideColumn(DEST);
//            m_list.hideColumn(ALTN);
//        }
//        SizeToContents();
//    }
//}

bool CListACC::IsCanDeleteRow(int row, const QSharedPointer<CAirplane> &pAirplane)
{
    return false;
}

void CListACC::TimerSlot()
{
}

void CListACC::OnItemEdit(const QModelIndex index, bool& bEdit)
{
    if(index.column() == CListACModel::ACID)
    {
        QSharedPointer<CSFPL> pSFPL = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
        ItemEditACID(pSFPL);
    }
    else if(index.column() == CListACModel::PEL_CFL)
    {
        QSharedPointer<CAirplane> pPlane = m_model.GetItemData(m_sortModel.mapToSource(index)).GetAirplane();
        if(pPlane && pPlane->IsOur())
            bEdit = true;
    }
    else if(index.column() == CListACModel::RFL_TFL)
    {
        QSharedPointer<CAirplane> pPlane = m_model.GetItemData(m_sortModel.mapToSource(index)).GetAirplane();
        if(pPlane && pPlane->IsOur())
            bEdit = true;
    }
}

void CListACC::OnButtonMes()
{
    QDialog dlg(this);
    MesViewer view(&dlg);
    QModelIndex index = m_list.currentIndex();
    if(index.isValid())
    {
        QSharedPointer<CSFPL> pSFPL = index.data(Qt::UserRole).value<CListACData>().GetSFPL();
        if(pSFPL)
        {
            view.SetFilter(1,
                           "",
                           networkSettings()->value("oldi/name", "Unknown").toString(),
                           "",
                           pSFPL->GetACID(),
                           QDateTime::currentDateTime().addSecs(-3600));
        }
    }
    QVBoxLayout layout(&dlg);
    layout.setContentsMargins(0, 0, 0, 0);
    layout.addWidget(&view);
    dlg.setLayout(&layout);
    dlg.adjustSize();
    dlg.setWindowTitle(view.windowTitle());
    dlg.exec();
}

void CListACC::OnDbClickItemEdit(const QModelIndex& index)
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
