#include "ListAC.h"
#include "front/core/GUIAirplane.h"
#include "front/core/GUIView.h"
#include "back/Master.h"
#include "back/core/View.h"

#include "main.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>

#include <QFutureWatcher>
#include <QtCore>

#include "OLDIProc.h"
#include "OLDIMessages.h"

#include "fpl.h"
#include "Plane.h"
#include "obelix/CmdChangeSFPL.h"
#include "obelix/CmdSFPLRefresh.h"
#include "SimulatorTrack.h"

#include "EditableHeader.h"
#include "XMasterWindow.h"
#include "back/utils/ProfileSettings.h"

CListACModel CListAC::m_model(0);
int CListAC::m_modelUsage(0);
CListAC::TListView CListAC::m_listView;
bool CListAC::mChanging(false);
QDateTime CListAC::mLastTimeRefresh = QDateTime::currentDateTime();

QColor  CListACModel::m_WarningColor(Qt::yellow);
QColor  CListACModel::m_DangerColor(Qt::red);
QColor  CListACModel::m_ActColor(100,100,100,120);
QColor  CListACModel::m_AssumeColor(0,150,255,120);
QColor  CListACModel::m_InColor(0,0,0,0);
QColor  CListACModel::m_TransferedColor(100,100,100,120);

QFutureWatcher<CListACData>* CListAC::m_FutureRefreshFutwatch = 0;
volatile bool CListAC::m_FutureRefreshForceStop = false;
CListACModel::TListACData* CListAC::m_pNewListData = 0;
quint32 CListAC::m_qSFPLID = 0;
quint32 CListAC::m_qSFPLIDwhenRunning = UINT_MAX;
volatile bool CListAC::m_bFutureRefreshInProcess = false;
QSet<CListAC*> CListAC::liveObjects;

CListAC::CListAC(CMaster* pMaster, QWidget* pParent) :
        QWidget(pParent), m_list(this), m_sortModel(this), m_pMaster(pMaster),spi_timer_(new QTimer(this)),m_qMinimumTableWidth(0)
{
    liveObjects.insert(this);

    if (!m_FutureRefreshFutwatch)
    {
        m_FutureRefreshFutwatch = new QFutureWatcher<CListACData>();
        connect(m_FutureRefreshFutwatch, SIGNAL(resultReadyAt(int)), this, SLOT(OnFutureRefreshFutwatchResult(int)));
        connect(m_FutureRefreshFutwatch, SIGNAL(finished()), this, SLOT(OnFutureRefreshFutwatchFinish()));
    }

    m_qMaxCellWidth = networkSettings()->value("users/SizeListAC/widthColumn").toInt();
    quint32 t_MaxCellHeight = networkSettings()->value("users/SizeListAC/heightRow").toInt();
    if (!t_MaxCellHeight)
        t_MaxCellHeight = DEFAULT_MAX_ROW_HEIGHT;
    ++m_modelUsage;

    m_sortModel.setSourceModel(&m_model);
    m_list.setModel(&m_sortModel);
    if (t_MaxCellHeight < m_list.horizontalHeader()->sizeHint().height())
        m_list.verticalHeader()->setDefaultSectionSize(m_list.horizontalHeader()->sizeHint().height()) ;

    m_model.SetMasterPtr(pMaster);

    /*if (!m_model.GetModelData().size())
    {
        CListACData::GetDeltaIPS_Big(networkSettings()->value("oldi/transfer_show_time", "60").toInt());
        Refresh(0, false);
    }*/

    m_ControlsLayout = new QHBoxLayout();
    m_ControlsLayout->setContentsMargins(0, 0, 0, 0);
    m_ControlsLayout_add = new QHBoxLayout();
    m_ControlsLayout_add->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *layout = new QVBoxLayout(this);
    //layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(3);
    layout->addWidget(&m_list);
    layout->addLayout(m_ControlsLayout);
    layout->addLayout(m_ControlsLayout_add);

    setLayout(layout);
    adjustSize();

    connect(m_list.selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this,
            SLOT(OnSelectionChanged(const QItemSelection&, const QItemSelection&)));
    connect(&m_list, SIGNAL(entered(const QModelIndex&)), this, SLOT(OnEnteredItem(const QModelIndex&)));
    connect(&m_list, SIGNAL(enter_leaveEvent(bool)), this, SLOT(OnEnterLeaveList(bool)));
    connect(&m_list, SIGNAL(keyPressedEvent(const QModelIndex&, QKeyEvent*)), this, SLOT(OnKeyPressed(const QModelIndex&, QKeyEvent*)));
    connect(&m_model, SIGNAL(timerModel()), this, SLOT(OnTimerSlot()));

    m_pActionShowRoute = new QAction(this);
    connect(m_pActionShowRoute, SIGNAL(triggered()), this, SLOT(OnShowRoute()));
    m_pActionTransfer = new QAction(this);
    connect(m_pActionTransfer, SIGNAL(triggered()), this, SLOT(OnTransfer()));
    m_pActionCancelACT = new QAction(this);
    connect(m_pActionCancelACT, SIGNAL(triggered()), this, SLOT(OnCancelAct()));
    m_list.setContextMenuPolicy(Qt::CustomContextMenu);
    connect(&m_list, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnContextMenu(const QPoint&)));

    //spi_timer_->setInterval(10 * 1000);
    //connect(spi_timer_, SIGNAL(timeout()), this, SLOT(onSpiTimeout()));

#ifndef _MSC_VER
    m_listView.append(this);
#endif

    //setStyleSheet("QPushButton {padding: 0px; padding-left: 20px; padding-right: 20px;}");
    setStyleSheet("QHeaderView::section {padding-top: 2px; padding-bottom: 2px; padding-left: 8px; padding-right: 8px;}");
}

CListAC::~CListAC()
{
    TListView::iterator itList = m_listView.begin();
    for (; itList != m_listView.end(); ++itList)
    {
        if (*itList == this)
        {
            m_listView.erase(itList);
            break;
        }
    }
    if (--m_modelUsage == 0)
        m_model.Clear();

    liveObjects.remove(this);
}

void CListAC::AddPlaneRecord(const QSharedPointer<CAirplane>& pAirplane)
{
    if (pAirplane && !m_bFutureRefreshInProcess)
    {
        int iRow = FindRow(pAirplane);
        if (iRow == -1)
            iRow = _InsertRow(pAirplane);
        if (iRow >= 0)
            _ModifyRow(iRow, pAirplane);
    }
}

void CListAC::ModPlaneRecord(const QSharedPointer<CAirplane>& pAirplane)
{
    if (pAirplane && !m_bFutureRefreshInProcess)
    {
        int iRow = FindRow(pAirplane);
        if (iRow >= 0)
            _ModifyRow(iRow, pAirplane);
        else
            AddPlaneRecord(pAirplane);
    }
}

void CListAC::DelPlaneRecord(const QSharedPointer<CAirplane>& pAirplane)
{
    if(!m_bFutureRefreshInProcess)
    {
        int iRow = FindRow(pAirplane);
        if (iRow >= 0)
        {
            if (_IsCanDeleteRow(iRow, pAirplane))
                m_model.DelRecord(m_model.GetItemData(iRow));
        }
    }
}

void CListAC::SelPlaneRecord(const QSharedPointer<CAirplane>& pAirplane)
{
    int iRow = FindRow(pAirplane);
    if (iRow >= 0)
    {
        QModelIndex modelIndex = m_model.index(iRow, 0); //Pasha
        SelectCurrentIndex(modelIndex);
    }
}

int CListAC::FindRow(const QSharedPointer<CAirplane>& pAirplane)
{
    if (pAirplane)
    {
        CListACModel::TListACData& pData = m_model.GetModelData();
        CListACModel::TListACData::const_iterator itData = pData.begin();
        for (int i = 0; itData != pData.end(); ++i, ++itData)
        {
            if (itData->GetAirplane() == pAirplane)
            {
                return i;
            }
        }
    }
    return -1;
}

int CListAC::FindRow(const QSharedPointer<CSFPL>& pSFPL)
{
    if (pSFPL)
    {
        CListACModel::TListACData& pData = m_model.GetModelData();
        CListACModel::TListACData::const_iterator itData = pData.begin();
        for (int i = 0; itData != pData.end(); ++i, ++itData)
        {
            if (itData->GetSFPL() == pSFPL)
            {
                return i;
            }
        }
    }
    return -1;
}

const QSize CListAC::GetVisibleSize() const
{
    int width  = m_list.horizontalHeader()->length();
    int height = m_list.verticalHeader()->length() + m_list.horizontalHeader()->height();

    int deltaWidth  = 2 * (m_list.frameWidth() + geometry().x());
    int deltaHeight = 2 * m_list.frameWidth() + geometry().x() + geometry().y();

    int controlsHeight = m_ControlsLayout->minimumSize().height();
    int controlsaddHeight = m_ControlsLayout_add->minimumSize().height();
    if (controlsHeight > 0)
        controlsHeight += layout()->spacing();
    if (controlsaddHeight > 0)
        controlsaddHeight += layout()->spacing();

    return QSize(width + deltaWidth, height + deltaHeight + controlsHeight+controlsaddHeight);
}

void CListAC::OnSelectionChanged(const QItemSelection& current, const QItemSelection& previous)
{
    if (mChanging)
        return;
    if (!current.isEmpty())
        SelectCurrentIndex(m_sortModel.mapToSource(m_list.currentIndex()));
}

void CListAC::OnEnteredItem(const QModelIndex& index)
{
    if (index.isValid())
    {
        QModelIndex modelIndex = m_sortModel.mapToSource(index);
        SelectCurrentIndex(modelIndex);
    }
}

void CListAC::WriteLog(const QSharedPointer<CAirplane>& pAirplane, QString sType, QString sValue)
{
    if (pAirplane)
    {
        QString sCode;
        if (pAirplane->GetMode3AReply() == 0)
            sCode = "none";
        else
            sCode = pAirplane->GetMode3AReplyToString();
        LOG_OPER_STR(sCode, sType, sValue);
    }
}

void CListAC::OnTimerSlot()
{
    TimerSlot();
    SelectRow(CListACData());
    SizeToContents();
}

bool CListAC::CalcTableColumnsWidth(QMap<quint32,quint32> &mas_colum)
{
// return true - need resize columns
// return false - didnt need resize

    QMap<quint32,quint32>::iterator it;
    bool flag_need_resize = false;
    qint32 t_min_table_width = m_qMinimumTableWidth;
    qint32 t_curr_width = 0;

    //qDebug()<<"_______))))))))))))))))CalcTableColumnsWidth(((((((((((((((((((((________________";
    // calc table width
    for (it = mas_colum.begin(); it != mas_colum.end();it++)
    {
        //qDebug()<<"column_"<<it.key()<<"      orig_width="<<it.value();
        if (m_qMaxCellWidth)
        {
            if (it.value() > m_qMaxCellWidth)
            {
                flag_need_resize = true;
                it.value() = m_qMaxCellWidth;
               // qDebug()<<"          correction width to "<<m_qMaxCellWidth;
            }
        }
        t_curr_width += it.value();
    }
    //qDebug()<<"t_cur_width="<<t_curr_width <<"    min_tab_width="<<t_min_table_width;
    // if current table width more than needed - ok
    if (t_curr_width >= t_min_table_width)
    {
        return flag_need_resize;
    }
    else
    {
        //qDebug()<<"m_qMaxCellWidth="<<m_qMaxCellWidth<<"     column count="<<mas_colum.size();
        // if max cell width = 0 we need just resize all column to minimum table width
        if (!m_qMaxCellWidth)
        {
            double koef = (double)t_min_table_width/(double)t_curr_width;
            for (it = mas_colum.begin(); it != mas_colum.end();it++)
            {
                it.value() = it.value() * koef + 1;
            }
            return true;
        }
        else
        {
            // if column count * m_qMaxCellWidth less than t_min_table_width we need make column bigger than m_qMaxCellWidth
            if (m_qMaxCellWidth * mas_colum.size() < t_min_table_width)
            {
                //qDebug()<<"SUDA";
                quint32 column_width = t_min_table_width/mas_colum.size() + 1;
                for (it = mas_colum.begin(); it != mas_colum.end();it++)
                {
                    it.value() = column_width;
                }
                return true;
            }
            else
            {
                //qDebug()<<"TUDA";
                t_curr_width = 0;
                t_min_table_width = m_qMinimumTableWidth;
                for (it = mas_colum.begin(); it != mas_colum.end();it++)
                {
                    if (it.value() >= m_qMaxCellWidth)
                        t_min_table_width -= it.value();
                    else
                        t_curr_width += it.value();
                }
                //qDebug()<<"SSSSSTART  t_curr_width="<<t_curr_width<<"     t_min_table_width= "<<t_min_table_width;
                while (t_curr_width < t_min_table_width)
                {
                    //qDebug()<<"Iteration++++++++++++++++++++++";
                    double koef = (double)t_min_table_width / (double)t_curr_width;
                    //qDebug()<<"koef="<<koef;
                    t_curr_width = 0;
                    for (it = mas_colum.begin(); it != mas_colum.end();it++)
                    {
                        if (it.value() < m_qMaxCellWidth)
                        {
                            //qDebug()<<"columns_"<<it.key();
                            if ( (it.value() * koef + 1) >= m_qMaxCellWidth)
                            {
                                it.value() = m_qMaxCellWidth;
                                t_min_table_width -= m_qMaxCellWidth;
                                //qDebug()<<"_____________size column after resize more then maxColumnWidth,  resize columns to max size";
                            }
                            else
                            {
                                it.value() = it.value() * koef + 1;
                                t_curr_width += it.value();
                                //qDebug()<<"_____________sresize columns to size="<<it.value();
                            }
                        }
                    }
                    //qDebug()<<"enditeration+++++++++++++++++++    TableCurrentWidth="<<t_curr_width<<"    MinTableWidth="<<t_min_table_width;
                }
                return true;
            }
        }
    }
}

void CListAC::SizeToContents(bool bSend /* = true */)
{
    QMap<quint32,quint32> column_mas;
    for(int i = 0; i < m_list.horizontalHeader()->count(); ++i)
    {
        if(m_list.columnWidth(i))
        {
            m_list.resizeColumnToContents(i);

            column_mas.insert(i,m_list.columnWidth(i));
        }
    }
    //qDebug()<<"_______))))))))))))))))(((((((((((((((((((((________________";
    if (CalcTableColumnsWidth(column_mas))
    {
        //qDebug()<<"RESIZEEEEE";
        QMap<quint32,quint32>::iterator it;
        for(int i = 0; i < m_list.horizontalHeader()->count(); ++i)
        {
            if(m_list.columnWidth(i))
            {
                if ( (it = column_mas.find(i)) != column_mas.end())
                {
                    m_list.horizontalHeader()->setSectionResizeMode(i,QHeaderView::Fixed);
                    m_list.setColumnWidth(i,it.value());
                    //qDebug()<<"for column_"<<i<<" set size="<<it.value();
                }
            }
        }
    }

    if (bSend)
        emit sizeChanged();
}

void CListAC::SelectRow(const CListACData& pItemData)
{
    QModelIndex prevIndex = m_model.GetCurrentIndex();
    int iCol = prevIndex.isValid() ? prevIndex.column() : 0;
    CListACData itemData;
    if (pItemData.GetAirplane() || pItemData.GetSFPL())
        itemData = pItemData;
    else
        itemData = m_model.GetItemData(prevIndex);

    mChanging = true;
    m_model.sort(1, Qt::AscendingOrder);

    if (itemData.GetAirplane() || itemData.GetSFPL())
    {
        int iRow(-1);
        CListACModel::TListACData& pData = m_model.GetModelData();
        CListACModel::TListACData::const_iterator itData = pData.begin();
        for (int i = 0; itData != pData.end(); ++i, ++itData)
        {
            if ((itData->GetAirplane() == itemData.GetAirplane()) && (itData->GetSFPL() == itemData.GetSFPL()))
            {
                iRow = i;
                break;
            }
        }
        if (iRow != -1)
        {
            QModelIndex modelIndex = m_model.index(iRow, iCol);
            SelectCurrentIndex(modelIndex);
        }
    }
    mChanging = false;
}

void CListAC::RefreshAndSelect(quint32 idSFPL)
{
    if (m_listView.size())
        m_listView.first()->Refresh(idSFPL);
}

void CListAC::Refresh(quint32 qSFPLID/* = 0*/, bool bProcessEvents/* = true*/)
{
    mLastTimeRefresh = QDateTime::currentDateTime();
    OnFutureRefresh(qSFPLID, bProcessEvents);
}

int CListAC::_InsertRow(const QSharedPointer<CAirplane>& pAirplane)
{
    int iRow(-1);

    if (pAirplane)
    {
        if (pAirplane->GetSFPL())
        {
            CListACModel::TListACData::iterator itData = m_model.GetModelData().begin();
            for (int i = 0; itData != m_model.GetModelData().end(); ++i, ++itData)
            {
                const QSharedPointer<CSFPL> &sfpl = itData->GetSFPL();
                if (pAirplane->GetMode3AReply() == sfpl->GetCODE())
                {
                    if ((!itData->GetAirplane() /*|| itData->GetAirplane()->GetID() == pAirplane->GetID()*/))
                    {
                        pAirplane->SetSFPL(sfpl);
                        const CAirplaneData::PlaneType plane_type = pAirplane->GetPlaneType();
                        if ((CAirplaneData::ASSUME == plane_type) || (CAirplaneData::Transfer == plane_type)) {
                            if (sfpl->GetCurSector() != networkSettings()->value("users/current_sector"))
                                pAirplane->SetPlaneType(CAirplaneData::Transfered);
                        } else if (sfpl->GetNextSector() == networkSettings()->value("users/current_sector")) {
                            pAirplane->SetPlaneType(CAirplaneData::ACT);
                        }
                        itData->SetAirplane(pAirplane);
                        pAirplane->SetCFL(sfpl->GetCFL());
                        pAirplane->SetTFL(sfpl->GetTFL());
                        iRow = i;
                        break;
                    }
                }
            }
        }
        else
        {
            CListACModel::TListACData::iterator itData = m_model.GetModelData().begin();
            for (int i = 0; itData != m_model.GetModelData().end(); ++i, ++itData)
            {
                if (pAirplane->GetMode3AReply() == itData->GetSFPL()->GetCODE())
                {
                    if ((!itData->GetAirplane() || itData->GetAirplane()->GetID() == pAirplane->GetID()))
                    {
                        iRow = i;
                        pAirplane->SetSFPL(itData->GetSFPL());
                        // may be applied history here?
                        if (pAirplane->GetSFPL() && pAirplane->GetSFPL()->GetFPLFrag())
                            pAirplane->GetSFPL()->GetFPLFrag()->ApplyHistory(pAirplane->GetHistoryPoints());
                        pAirplane->ChangeFL(itData->GetSFPL()->GetLevel() / (0.3048 * 100));
                        if (itData->GetTracer() && itData->GetTracer()->isVisible())
                        {
                            if (CGUIAirplane *pGUIPlane = dynamic_cast<CGUIAirplane*> (pAirplane.data()))
                                pGUIPlane->SetTracer(itData->GetTracer());
                            itData->SetTracer(QSharedPointer<CTracerFPL>());
                        }
                        itData->SetAirplane(pAirplane);
                        pAirplane->SetCFL(itData->GetSFPL()->GetCFL());
                        pAirplane->SetTFL(itData->GetSFPL()->GetTFL());
                        WriteLog(pAirplane, "ACID", itData->GetSFPL()->GetACID());

                        if (CAirplaneData::None == pAirplane->GetPlaneType())
                            pAirplane->SetPlaneType(CAirplaneData::ACT);

                        if (CAirplaneData::ACT == pAirplane->GetPlaneType())
                        {
                            const QString current_sector = networkSettings()->value("users/current_sector").toString();
                            if (!current_sector.isEmpty()) {
                                if (itData->GetSFPL()->GetCurSector() == current_sector) {
                                    if (!networkSettings()->value("users/force_sector").toBool())
                                        pAirplane->DirectAssume();
                                }
                            }
                        }

                        break;
                    }
                }
            }
        }
    }

    TListView::iterator itList = m_listView.begin();
    for (; itList != m_listView.end(); ++itList)
        (*itList)->InsertRow(pAirplane);

    return iRow;
}

void CListAC::_ModifyRow(int row, const QSharedPointer<CAirplane>& pAirplane)
{
    CListACData& itemData = m_model.GetItemData(row);
    if (pAirplane->GetSFPL())
    {
        if (pAirplane->GetSFPL() != itemData.GetSFPL())
        {
            LOG_STR(Log4Qt::Level::DEBUG_INT, QString("Different SFPL in ListAC. %1 %2").arg(pAirplane->GetMode3AReplyToString()).arg(pAirplane->GetSFPL()->GetACID()));
        }
    }
    if (itemData.GetAirplane())
    {
        itemData.SetIPS_Small(0);
        if (CAirplaneData::Transfered != itemData.GetAirplane()->GetPlaneType())
            itemData.SetIPS_Big(0);
    }
    SetREM(itemData);

    TListView::iterator itList = m_listView.begin();
    for (; itList != m_listView.end(); ++itList)
        (*itList)->ModifyRow(row, pAirplane);
}

bool CListAC::_IsCanDeleteRow(int row, const QSharedPointer<CAirplane>& pAirplane)
{
    CListACData& itemData = m_model.GetItemData(row);
    itemData.SetAirplane(QSharedPointer<CAirplane> ());
    return true;
}

void CListAC::OnContextMenu(const QPoint &pos)
{
    QModelIndex index = m_list.indexAt(pos);
    m_pActionSFPL.clear();
    if (index.isValid())
    {
        QSharedPointer<CSFPL> pSFPL = index.data(Qt::UserRole).value<CListACData>().GetSFPL();
        if (pSFPL)
        {
            m_pActionSFPL = pSFPL.toWeakRef();
            QMenu* pMenu = new QMenu(this);
            m_pActionShowRoute->setText(tr("Route"));
            pMenu->addAction(m_pActionShowRoute);
            QSharedPointer<CAirplane> pPlane = index.data(Qt::UserRole).value<CListACData>().GetAirplane();
            if(pPlane)
            {
                if ((CAirplaneData::ACT == pPlane->GetPlaneType()) || (CAirplaneData::ASSUME == pPlane->GetPlaneType()))
                {
                    if (CAirplaneData::ACT == pPlane->GetPlaneType())
                        m_pActionTransfer->setText(tr("Assume"));
                    else
                        m_pActionTransfer->setText(tr("Transfer"));
                    pMenu->addAction(m_pActionTransfer);
                }
            }
            m_pActionCancelACT->setText(tr("Cancel ACT"));
            pMenu->addAction(m_pActionCancelACT);
            pMenu->exec(m_list.viewport()->mapToGlobal(pos));
        }
    }
}

void CListAC::OnTransfer()
{
    QSharedPointer<CSFPL> pSFPL = m_pActionSFPL.toStrongRef();
    if(pSFPL)
    {
        int iRow = FindRow(pSFPL);
        if(iRow != -1)
        {
            CListACData& pData = m_model.GetItemData(iRow);
            QSharedPointer<CAirplane> pPlane = pData.GetAirplane();
            if (pPlane)
            {
                if(CAirplaneData::ACT == pPlane->GetPlaneType())
                {
                    pPlane->AssumeOperator();
                }
                if(CAirplaneData::ASSUME == pPlane->GetPlaneType())
                {
                    //if(pPlane->GetSFPL() && !pPlane->GetSFPL()->GetNextSector().isEmpty())
                        //pPlane->ChangeOperator(???, pPlane->GetSFPL()->GetNextSector());
                }
            }
        }
    }
}

void CListAC::OnShowRoute()
{
    QSharedPointer<CSFPL> pSFPL = m_pActionSFPL.toStrongRef();
    if(pSFPL)
    {
        int iRow = FindRow(pSFPL);
        if(iRow != -1)
        {
            CListACData& pData = m_model.GetItemData(iRow);
            QSharedPointer<CAirplane> pPlane = pData.GetAirplane();
            if (pPlane)
            {
                CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*> (pPlane.data());
                if (pGUIPlane)
                    pGUIPlane->ShowPathPoints(!pGUIPlane->isShowPathPoints());
                pData.SetTracer(QSharedPointer<CTracerFPL>());
            }
            else
            {
                QSharedPointer<CTracerFPL> pTracer = pData.GetTracer();
                if(pTracer)
                {
                    pTracer.clear();
                }
                else
                {
                    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_pMaster->GetView());
                    if(m_pMaster && view && view->GetMainFIR())
                    {
                        if (CGUIView* pView = dynamic_cast<CGUIView*>(m_pMaster->GetView().data()))
                        {
                            pTracer = QSharedPointer<CTracerFPL>(new CTracerFPL(pData.GetSFPL()->GetFPLFrag()
                                                                                , m_pMaster->GetView()));
                            pView->scene()->addItem(pTracer.data());
                            pTracer->UpdateTracerPlane(0);
                            pView->Draw();
                        }
                    }
                }
                pData.SetTracer(pTracer);
            }
        }
    }
}

void CListAC::OnShowRouteCost(QSharedPointer<CSFPL> pSFPL)
{
    if(pSFPL)
    {
        int iRow = FindRow(pSFPL);
        if(iRow != -1)
        {
            CListACData& pData = m_model.GetItemData(iRow);
            QSharedPointer<CAirplane> pPlane = pData.GetAirplane();
            if (pPlane)
            {
                CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*> (pPlane.data());
                if(pGUIPlane)
                    if(!(pGUIPlane->getSignShowRoute()))
                    {
                        if (pGUIPlane->scene())
                            pGUIPlane->ShowPathPoints(!pGUIPlane->isShowPathPoints());
                        else
                        {
                            QSharedPointer<CTracerFPL> pTracer = pGUIPlane->Get_pTracer();
                            if(!pTracer)
                            {
                                if(pGUIPlane)
                                    if(pGUIPlane->GetSFPL())
                                    {
                                        QSharedPointer<CFPLFrag> FPLFrag;
                                        FPLFrag = pGUIPlane->GetSFPL()->GetFPLFrag();
                                        QColor color = UserProfile::CSettings::Instance().GetColor(pGUIPlane->IsLastActive() ? UserProfile::CtFormularActive : UserProfile::CtFormularPassive);
                                        pTracer = QSharedPointer<CTracerFPL>(new CTracerFPL(FPLFrag, m_pMaster->GetView(), color));
                                        pTracer->SetReadOnly(true);
                                        CGUIView* pView = dynamic_cast<CGUIView*>(m_pMaster->GetView().data());
                                        pView->scene()->addItem(pTracer.data());
                                        pGUIPlane->SetTracer(pTracer);
                                        pGUIPlane->Get_pTracer()->SetPosition(pGUIPlane->Get_qllXMin(), pGUIPlane->Get_qllYMax(), pGUIPlane->Get_dScale(), pGUIPlane->Get_CentralCoord());
                                        pGUIPlane->Get_pTracer()->SetCGUIAirplane(pGUIPlane);
                                        pGUIPlane->Get_pTracer()->RecalculateTracer();
                                        pGUIPlane->setSignShowRoute(true);
                                    }
                            }
                        }
                    }
                    else
                    {
                        pGUIPlane->SetTracer(0);
                        pGUIPlane->setSignShowRoute(false);
                    }
            }
            else
            {
                QSharedPointer<CTracerFPL> pTracer = pData.GetTracer();
                if(pTracer)
                {
                    pTracer.clear();
                }
                else
                {
                    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_pMaster->GetView());
                    if(m_pMaster && view && view->GetMainFIR())
                    {
                        if (CGUIView* pView = dynamic_cast<CGUIView*>(m_pMaster->GetView().data()))
                        {
                            QSharedPointer<CAirplane> pAirplane;
                            pAirplane = QSharedPointer<CAirplane>(view->CreateAirplane());
                            pAirplane->SetID(5555);
                            pAirplane->SetOperName(QObject::tr("   "));
                            pAirplane->setIsFlying(false);
                            CCoord CoordTemp;
                            CoordTemp.Center(view->GetCentralPoint()->GetCoord());
                            pAirplane->SetCoord(CoordTemp);

                            CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*> (pAirplane.data());

                            pData.GetSFPL()->CreateFPLFrag(pData.GetSFPL());
                            int iBeforehandFly = CPlane::GetStepSeconds();
                            QSharedPointer<CPlane> simPlane;
                            simPlane = QSharedPointer<CPlane>(new CPlane(pData.GetSFPL(), pView->GetCentralPoint()->GetCoord(),
                                                                         iBeforehandFly, true));
                            /*bool bLife = simPlane->CalculatePosition(1);
                            // Speed
                            double dSeconds = simPlane->GetStepSeconds();
                            double dIASAcceler(0);
                            if (simPlane->GetIAS() < simPlane->GetAimIAS())
                                dIASAcceler = 0.5;
                            else if (simPlane->GetIAS() > simPlane->GetAimIAS())
                                dIASAcceler = -0.5;
                            double dTempIAS = simPlane->GetIAS() + dIASAcceler * dSeconds;


                            if ((dTempIAS > simPlane->GetAimIAS() && dIASAcceler > 0) ||
                                (dTempIAS < simPlane->GetAimIAS() && dIASAcceler < 0))
                                dTempIAS = simPlane->GetAimIAS();

                            if (simPlane->GetIAS() != dTempIAS)
                                simPlane->SetIsChanged(true);
                            simPlane->SetIAS(dTempIAS);
                            pGUIPlane->SetIAS(dTempIAS);*/

                            double dTAS = simPlane->GetWindTAS();
                            simPlane->SetVelocity(CVelocity(
                                sin(simPlane->GetWindAzimuth()) * dTAS,
                                cos(simPlane->GetWindAzimuth()) * dTAS));

                            pGUIPlane->SetVelocity(CVelocity(
                                                       sin(simPlane->GetWindAzimuth()) * dTAS,
                                                       cos(simPlane->GetWindAzimuth()) * dTAS));

                            QColor color = UserProfile::CSettings::Instance().GetColor( UserProfile::CtFormularPassive );
                            pTracer = QSharedPointer<CTracerFPL>(new CTracerFPL(pData.GetSFPL()->GetFPLFrag()
                                                                                , m_pMaster->GetView(), color));
                            pView->scene()->addItem(pTracer.data());
                            //pTracer->UpdateTracerPlane(pGUIPlane);
                            pTracer->SetCGUIAirplane(pGUIPlane);
                            pTracer->RecalculateTracer();
                            pView->Draw();
                        }
                    }
                }
                pData.SetTracer(pTracer);
            }
        }
    }
}

void CListAC::SelectIndex(const QModelIndex &modelIndex)
{
    QModelIndex index = m_sortModel.mapFromSource(modelIndex);
    if (!index.isValid())
    {
        QModelIndex indexPrev = m_list.currentIndex();
        QModelIndex modelIndexPrev = m_sortModel.mapToSource(indexPrev);
        index = m_sortModel.mapFromSource(m_model.index(modelIndex.row(),
                                                        modelIndexPrev.isValid() ? modelIndexPrev.column() : 0));
    }
    if (index.isValid() && !m_list.isRowHidden(index.row()))
    {
        if(!m_list.isColumnHidden(index.column()))
        {
            m_list.scrollTo(index);
            m_list.setCurrentIndex(index);
        }
    }
    else
        m_list.clearSelection();
}

void CListAC::SelectCurrentIndex(const QModelIndex &modelIndex)
{
    QModelIndex modelIndexPrev = m_model.GetCurrentIndex();
    if (modelIndexPrev == modelIndex)
        return;
    mChanging = true;
    m_model.SetCurrentIndex(modelIndex);
    TListView::iterator itList = m_listView.begin();
    for (; itList != m_listView.end(); ++itList)
        (*itList)->SelectIndex(modelIndex);
    if (modelIndex.row() != modelIndexPrev.row())
    {
        if (modelIndexPrev.isValid() && modelIndexPrev.row() < m_model.rowCount(QModelIndex()))
        {
            QSharedPointer<CAirplane> pPlane = modelIndexPrev.data(Qt::UserRole).value<CListACData> ().GetAirplane();
            if (pPlane)
            {
                CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*> (pPlane.data());
                if (pGUIPlane)
                    pGUIPlane->ActivatePlane(false);
            }
        }
        if (modelIndex.isValid())
        {
            QSharedPointer<CAirplane> pPlane = modelIndex.data(Qt::UserRole).value<CListACData> ().GetAirplane();
            if (pPlane)
            {
                CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*> (pPlane.data());
                if (pGUIPlane)
                    pGUIPlane->ActivatePlane(true);
            }
        }
    }
    mChanging = false;
}

void CListAC::SetREM(CListACData& itemData)
{
    CListACWarning itemWRN;
    QSharedPointer<CAirplane> pPlane = itemData.GetAirplane();

    if(pPlane && pPlane->GetSFPL())
    {
        if(itemWRN.GetLevel() < 31)
        {
            //if (pPlane->GetMode3AReply() != pPlane->GetSFPL()->GetCODE())
            //alarm CODE 7500,7600,7700,2000
            if (pPlane->GetMode3AReply() == 3904 || pPlane->GetMode3AReply() == 3968 || pPlane->GetMode3AReply() == 4032 || pPlane->GetMode3AReply() == 1024)
            {
                QString sText;
                sText = pPlane->GetMode3AReplyToString();
                itemWRN.SetText(sText);
                itemWRN.SetLevel(31);
            }
        }
    }

    if(pPlane)
    {
        if(itemWRN.GetLevel() < 20)
        {
            if(pPlane->isSTCA())
            {
                itemWRN.SetText(tr("STCA"));
                itemWRN.SetLevel(20);
            }
        }
        if(itemWRN.GetLevel() < 17)
        {
            if(pPlane->GetSPI())
            {
                itemWRN.SetText(tr("SPI"));
                itemWRN.SetLevel(17);
            }
        }
        QString restriction_warning = pPlane->restrictionWarning();
        if (!restriction_warning.isEmpty())
        {
            if(itemWRN.GetLevel() < 15)
            {
                itemWRN.SetText(restriction_warning);
                itemWRN.SetLevel(15);
            }
        }
        if(itemWRN.GetLevel() < 10)
        {
            if(pPlane->trackStatus() != CAirplane::Active)
            {
                itemWRN.SetText(tr("COST"));
                itemWRN.SetLevel(10);
            }
        }
    }
    if(itemWRN.GetLevel() < 30)
    {
        if(isDoubleCode(itemData))
        {
            itemWRN.SetText(tr("Double Code"));
            itemWRN.SetLevel(30);
        }
    }

    itemData.SetWRN(itemWRN);
}

bool CListAC::isDoubleCode(CListACData& itemData)
{
    return m_model.isDoubleCode(itemData);
}

void CListAC::MigrationFrag(QSharedPointer<CFPLFrag> &pFragSFPL, const QSharedPointer<CFPLFrag> &pFragPlane)
{
    // algorithm is not accurate even if routeFPL == routePlane
    if (pFragSFPL->GetRoutePart() && pFragPlane->GetRoutePart()
        && (pFragSFPL->GetRoutePart()->GetPointFragList().size()
            == pFragPlane->GetRoutePart()->GetPointFragList().size()))
    {
        CProcFrag::TPointFragList FPLPoints = pFragSFPL->GetAllPoint();
        CProcFrag::TPointFragList PlanePoints = pFragPlane->GetAllPoint();
        for(int i = 0; i < FPLPoints.size(); ++i)
        {
            if(i < PlanePoints.size())
            {
                if(PlanePoints[i]->IsPassed())
                {
                    if(FPLPoints[i]->GetPoint() && PlanePoints[i]->GetPoint()
                        && (FPLPoints[i]->GetPoint()->GetID() == PlanePoints[i]->GetPoint()->GetID()))
                    {
                        *FPLPoints[i] = *PlanePoints[i];
                    }
                    else break;
                }
                else break;
            }
            else break;
        }
        pFragSFPL->SetNextPoint(pFragPlane->GetNextPoint());
    }
}

bool CListAC::RecieveOLDIMessage(QSharedPointer<OLDIMessage>& pMes)
{
    if(m_listView.size())
        return m_listView.first()->_RecieveOLDIMessage(pMes);
    else
        return false;
}

void CListAC::toggleScroll()
{
    if(m_listView.size())
    {
        TListView::iterator itList = m_listView.begin();
        for (; itList != m_listView.end(); ++itList)
            (*itList)->_toggleScroll();
    }
}

bool CListAC::_RecieveOLDIMessage(QSharedPointer<OLDIMessage>& pMes)
{
    QString sMessageType = pMes->GetMesData().sType;
    if(oldi::MES_ABI == sMessageType)
    {// update SFPL or insert SFPL from FPL?    //server?
        OLDIMesDataABI dataABI = pMes.staticCast<OLDIMesABI>()->GetDataABI();
        CListACData& pItemData = GetDataBySFPLkey(dataABI.sACID, dataABI.sADEP, dataABI.sADES);
        if(pItemData.GetSFPL())
        {//server
            QSharedPointer<CSFPL> pSFPL = pItemData.GetSFPL();
            OLDIMessage::SetABItoSFPL(dataABI, pSFPL);
            TVA_CATCHALL_TRY
            {
                dao()->UpdateSFPL(pSFPL);
                pMes->SetExecuted();

                QSharedPointer<obelix::CCmdSFPLRefresh> pCmd(new obelix::CCmdSFPLRefresh());
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_TYPE, QL1S("COMMON"));
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ACID, QString("%1").arg(pSFPL->GetID()));
                m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

                //Refresh(pSFPL->GetID());    //just client
            }TVA_CATCHALL(err){
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
        }
        else
        {//server
            quint32 idFPL = 0;
            TVA_CATCHALL_TRY
            {
                idFPL = dao()->GetFPLIdByKey(dataABI.sACID, dataABI.sADEP, dataABI.sADES, 2);
            }TVA_CATCHALL(err){
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

            if(idFPL)
            {
                // SFPL exist, but not in lists, why?
                TVA_CATCHALL_TRY
                {
                    QList<QSharedPointer<CSFPL> > listSFPL;
                    dao()->LoadSFPLs(listSFPL, idFPL);
                    if(listSFPL.size())
                    {
                        OLDIMessage::SetABItoSFPL(dataABI, listSFPL[0]);
                        dao()->UpdateSFPL(listSFPL[0]);

                        pMes->SetExecuted();

                        QSharedPointer<obelix::CCmdSFPLRefresh> pCmd(new obelix::CCmdSFPLRefresh());
                        pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_TYPE, QL1S("COMMON"));
                        pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ACID, QString("%1").arg(idFPL));
                        m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

                        //Refresh(idFPL); //just client
                    }
                    else
                    {
                        // nothing understand :(
                        qWarning() << tr("SFPL doesn't exist");
                    }
                }TVA_CATCHALL(err){
                LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
            }
            else
            {//server
                // SFPL doesn't exist, may be exist in FPL?
                TVA_CATCHALL_TRY
                {
                    idFPL = dao()->GetFPLIdByKey(dataABI.sACID, dataABI.sADEP, dataABI.sADES, 1);
                }TVA_CATCHALL(err){
                LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

                if(idFPL)
                {
                    QSharedPointer<CSFPL> pSFPL(new CSFPL);
                    pSFPL->SetID(idFPL);
                    OLDIMessage::SetABItoSFPL(dataABI, pSFPL);
                    TVA_CATCHALL_TRY
                    {
                        dao()->InsertSFPL(pSFPL);
                    }TVA_CATCHALL(err){
                    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

                    pMes->SetExecuted();

                    QSharedPointer<obelix::CCmdSFPLRefresh> pCmd(new obelix::CCmdSFPLRefresh());
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_TYPE, QL1S("COMMON"));
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ACID, QString("%1").arg(idFPL));
                    m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

                    //Refresh(idFPL); //just client
                }
                else
                {
                    qWarning() << tr("FPL doesn't exist");
                }
            }
        }

        if(pMes->GetMesData().bAnswer)
        {//server
            OLDIMesData dataLAM = pMes->GetLAM(m_pMaster->GetOLDIProc()->GetNextMesNum());
            QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
            m_pMaster->GetOLDIProc()->SendMessage(pMesLAM);
        }
        return true;
    }
    else if(oldi::MES_ACT == sMessageType)
    {// almost the same? o_O    //server?
        OLDIMesDataABI dataACT = pMes.staticCast<OLDIMesACT>()->GetDataACT();
        CListACData& pItemData = GetDataBySFPLkey(dataACT.sACID, dataACT.sADEP, dataACT.sADES);

        QString sInSector;
        if(pItemData.GetSFPL())
        {//server
            QSharedPointer<CSFPL> pSFPL = pItemData.GetSFPL();
            OLDIMessage::SetABItoSFPL(dataACT, pSFPL);
            CalcInSector(pSFPL);
            sInSector = pSFPL->GetInSector();
            TVA_CATCHALL_TRY
            {
                dao()->UpdateSFPL(pSFPL);
                pMes->SetExecuted();

                QSharedPointer<obelix::CCmdSFPLRefresh> pCmd(new obelix::CCmdSFPLRefresh());
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_TYPE, QL1S("COMMON"));
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ACID, QString("%1").arg(pSFPL->GetID()));
                m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

                //Refresh(pSFPL->GetID());    //just client
            }TVA_CATCHALL(err){
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
        }
        else
        {//server
            quint32 idFPL = 0;
            TVA_CATCHALL_TRY
            {
                idFPL = dao()->GetFPLIdByKey(dataACT.sACID, dataACT.sADEP, dataACT.sADES, 2);
            }TVA_CATCHALL(err){
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

            if(idFPL)
            {
                // SFPL exist, but not in lists, why?
                TVA_CATCHALL_TRY
                {
                    QList<QSharedPointer<CSFPL> > listSFPL;
                    dao()->LoadSFPLs(listSFPL, idFPL);
                    if(listSFPL.size())
                    {
                        OLDIMessage::SetABItoSFPL(dataACT, listSFPL[0]);
                        CalcInSector(listSFPL[0]);
                        sInSector = listSFPL[0]->GetInSector();
                        dao()->UpdateSFPL(listSFPL[0]);

                        pMes->SetExecuted();

                        QSharedPointer<obelix::CCmdSFPLRefresh> pCmd(new obelix::CCmdSFPLRefresh());
                        pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_TYPE, QL1S("COMMON"));
                        pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ACID, QString("%1").arg(idFPL));
                        m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

                        //Refresh(idFPL);    //just client
                    }
                    else
                    {
                        // nothing understand :(
                        qWarning() << tr("SFPL doesn't exist");
                    }
                }TVA_CATCHALL(err){
                LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
            }
            else
            {
                // SFPL doesn't exist, may be exist in FPL?
                TVA_CATCHALL_TRY
                {
                    idFPL = dao()->GetFPLIdByKey(dataACT.sACID, dataACT.sADEP, dataACT.sADES, 1);
                }TVA_CATCHALL(err){
                LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

                if(idFPL)
                {
                    QSharedPointer<CSFPL> pSFPL(new CSFPL);
                    pSFPL->SetID(idFPL);
                    OLDIMessage::SetABItoSFPL(dataACT, pSFPL);
                    CalcInSector(pSFPL);
                    sInSector = pSFPL->GetInSector();
                    TVA_CATCHALL_TRY
                    {
                        dao()->InsertSFPL(pSFPL);
                    }TVA_CATCHALL(err){
                    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

                    pMes->SetExecuted();

                    QSharedPointer<obelix::CCmdSFPLRefresh> pCmd(new obelix::CCmdSFPLRefresh());
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_TYPE, QL1S("COMMON"));
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ACID, QString("%1").arg(idFPL));
                    m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

                    //Refresh(idFPL);    //just client
                }
                else
                {
                    qWarning() << tr("FPL doesn't exist");
                }
            }
        }

        if(pMes->GetMesData().bAnswer)
        {//server
            OLDIMesData dataLAM = pMes->GetLAM(m_pMaster->GetOLDIProc()->GetNextMesNum());
            QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
            m_pMaster->GetOLDIProc()->SendMessage(pMesLAM);
        }

        // send INF
        OLDIMesDataINF dataINF;
        dataINF.iMesNum = m_pMaster->GetOLDIProc()->GetNextMesNum();
        dataINF.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
        dataINF.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
        dataINF.sTypeINF = oldi::INF_ACT;
        dataINF.sACID = dataACT.sACID;
        dataINF.sADEP = dataACT.sADEP;
        dataINF.sADES = dataACT.sADES;
        dataINF.sCODE = dataACT.sCODE;
        dataINF.sSector = sInSector;
        QSharedPointer<OLDIMessage> pMesINF(new OLDIMesINF(dataINF));
        //connect(pMes.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), this, SLOT(OLDIMesAnswer(const OLDIMessage*,const QStringList&)));
        //connect(pMes.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), this, SLOT(OLDIMesTimeout(const OLDIMessage*)));
        m_pMaster->GetOLDIProc()->SendMessage(pMesINF);

        QString sSectors = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
        QStringList sSectorsList = sSectors.split(",", QString::SkipEmptyParts);
        for(int i = 0; i < sSectorsList.size(); ++i)
        {
            dataINF.sTo = sSectorsList[i];
            pMesINF = QSharedPointer<OLDIMessage>(new OLDIMesINF(dataINF));
            //connect(pMes.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), this, SLOT(OLDIMesAnswer(const OLDIMessage*,const QStringList&)));
            //connect(pMes.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), this, SLOT(OLDIMesTimeout(const OLDIMessage*)));
            m_pMaster->GetOLDIProc()->SendMessage(pMesINF);
        }

        CListACData& pItemDataTemp = GetDataBySFPLkey(dataACT.sACID, dataACT.sADEP, dataACT.sADES);
        if(pItemDataTemp.GetAirplane())
            pItemDataTemp.GetAirplane()->RecieveOLDIMessage(pMes);

        return true;
    }
    else if(oldi::MES_COD == sMessageType)
    {// update SFPL CODE    //server
        OLDIMesDataCOD dataCOD = pMes.staticCast<OLDIMesCOD>()->GetDataCOD();
        CListACData& pItemData = GetDataBySFPLkey(dataCOD.sACID, dataCOD.sADEP, dataCOD.sADES);
        if(pItemData.GetSFPL())
        {
            pItemData.GetSFPL()->SetCODE(dataCOD.sCODE.toInt(0, 8));

            TVA_CATCHALL_TRY
            {
                dao()->UpdateSFPL(pItemData.GetSFPL());
                pMes->SetExecuted();
            }TVA_CATCHALL(err){
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

            if(pMes->GetMesData().bAnswer)
            {
                OLDIMesData dataLAM = pMes->GetLAM(m_pMaster->GetOLDIProc()->GetNextMesNum());
                QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
                m_pMaster->GetOLDIProc()->SendMessage(pMesLAM);
            }
//            QSharedPointer<obelix::CCmdSFPLRefresh> pCmd(new obelix::CCmdSFPLRefresh());
//            pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_TYPE, QL1S(oldi::MES_COD111));
//            pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ACID, dataCOD.sACID);
//            pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ADEP, dataCOD.sADEP);
//            pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ADES, dataCOD.sADES);
//            m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
            return true;
        }
        else
        {
            qWarning() << tr("SFPL doesn't exist");
        }
    }
    else if(oldi::MES_INF == sMessageType)
    {   //server
        OLDIMesDataINF dataINF = pMes.staticCast<OLDIMesINF>()->GetDataINF();

        if(dataINF.sTypeINF == oldi::INF_ACT || dataINF.sTypeINF == oldi::INF_ASSUME)
        {
            CListACData& pItemData = GetDataBySFPLkey(dataINF.sACID, dataINF.sADEP, dataINF.sADES);
            if(pItemData.GetSFPL())
            {
                pItemData.GetSFPL()->SetCODE(dataINF.sCODE.toInt(0, 8));
                if(dataINF.sTypeINF == oldi::INF_ACT)
                    pItemData.GetSFPL()->SetInSector(dataINF.sSector);
                if(dataINF.sTypeINF == oldi::INF_ASSUME)
                    pItemData.GetSFPL()->SetCurSector(dataINF.sSector);

                TVA_CATCHALL_TRY
                {
                    dao()->UpdateSFPL(pItemData.GetSFPL());
                    pMes->SetExecuted();
                }TVA_CATCHALL(err){
                LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

                //TEMP
                QSharedPointer<obelix::CCmdSFPLRefresh> pCmd(new obelix::CCmdSFPLRefresh());
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_TYPE, dataINF.sTypeINF);
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ACID, dataINF.sACID);
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ADEP, dataINF.sADEP);
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ADES, dataINF.sADES);
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_CODE, dataINF.sCODE);
                pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_SECTOR, dataINF.sSector);
                m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

            }
            else
            {
                quint32 idFPL = 0;
                TVA_CATCHALL_TRY
                {
                    idFPL = dao()->GetFPLIdByKey(dataINF.sACID, dataINF.sADEP, dataINF.sADES, 1);
                }TVA_CATCHALL(err){
                LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

                if(idFPL)
                {
                    QSharedPointer<CSFPL> pSFPL(new CSFPL);
                    pSFPL->SetID(idFPL);
                    pSFPL->SetCODE(dataINF.sCODE.toInt(0, 8));
                    if(dataINF.sTypeINF == oldi::INF_ACT)
                        pSFPL->SetInSector(dataINF.sSector);
                    if(dataINF.sTypeINF == oldi::INF_ASSUME)
                        pSFPL->SetCurSector(dataINF.sSector);
                    TVA_CATCHALL_TRY
                    {
                        dao()->InsertSFPL(pSFPL);
                    }TVA_CATCHALL(err){
                    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

                    pMes->SetExecuted();

                    QSharedPointer<obelix::CCmdSFPLRefresh> pCmd(new obelix::CCmdSFPLRefresh());
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_TYPE, QL1S("COMMON"));
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ACID, QString("%1").arg(idFPL));
                    m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

                    //Refresh(idFPL); //just client
                }
                else
                {
                    qWarning() << tr("FPL doesn't exist");
                }
            }
        }

        if(dataINF.sTypeINF == oldi::INF_DEL_SFPL)
        {//client
            //CListACData& pItemData = GetDataBySFPLkey(dataINF.sACID, dataINF.sADEP, dataINF.sADES);
            //if(pItemData.GetSFPL())
            {
                //if(pItemData.GetAirplane())
                {
                    QSharedPointer<obelix::CCmdSFPLRefresh> pCmd(new obelix::CCmdSFPLRefresh());
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_TYPE, QL1S(oldi::INF_DEL_SFPL));
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ACID, dataINF.sACID);
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ADEP, dataINF.sADEP);
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ADES, dataINF.sADES);
                    m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());

//                    CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*> (pItemData.GetAirplane().data());
//                    if (pGUIPlane)
//                        pGUIPlane->ShowPathPoints(false);
//                    pItemData.GetAirplane()->ClearSFPL();
                }
//                pItemData.SetAirplane(QSharedPointer<CAirplane>());
            }
            TVA_CATCHALL_TRY
            {   //just server
                dao()->ClearFPLByKey(dataINF.sACID, dataINF.sADEP, dataINF.sADES);
            }TVA_CATCHALL(err){
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

            pMes->SetExecuted();

            //Refresh();
        }

        if(dataINF.sTypeINF == oldi::INF_UNDO_SFPL)
        {//server
            CListACData& pItemData = GetDataBySFPLkey(dataINF.sACID, dataINF.sADEP, dataINF.sADES);
            if (pItemData.GetSFPL())
            {
                bool bFlag = true;
                if (pItemData.GetAirplane())
                {
                    if (pItemData.GetSFPL()->GetCODEtoString() != dataINF.sCODE)
                    {
                        bFlag = false;
                        qDebug() << QString("different code %1 != %2").arg(pItemData.GetSFPL()->GetCODEtoString()).arg(dataINF.sCODE);
                    }
                }
                if (bFlag)
                {
                    if (pItemData.GetAirplane())
                        pItemData.GetAirplane()->ClearSFPL();
                    pItemData.SetAirplane(QSharedPointer<CAirplane>());
                    TVA_CATCHALL_TRY
                    {
                        dao()->UndoSFPL(pItemData.GetSFPL()->GetID());
                    }TVA_CATCHALL(err){
                    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

                    QSharedPointer<obelix::CCmdSFPLRefresh> pCmd(new obelix::CCmdSFPLRefresh());
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_TYPE, QL1S(oldi::INF_UNDO_SFPL));
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ACID, dataINF.sACID);
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ADEP, dataINF.sADEP);
                    pCmd->SetValue(obelix::CCmdSFPLRefresh::XML_ADES, dataINF.sADES);
                    m_pMaster->GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                }
            }

            pMes->SetExecuted();

            //Refresh();  //just client
        }

        if(pMes->GetMesData().bAnswer)
        {//server
            OLDIMesData dataLAM = pMes->GetLAM(m_pMaster->GetOLDIProc()->GetNextMesNum());
            QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
            m_pMaster->GetOLDIProc()->SendMessage(pMesLAM);
        }
        return true;
    }
    else if(oldi::MES_FPL == sMessageType)
    {// FPL update message  //server (check if FPL is SFPL already)
        OLDIMesDataABI dataFPL = pMes.staticCast<OLDIMesFPL>()->GetDataFPL();
        CListACData& pItemData = GetDataBySFPLkey(dataFPL.sACID, dataFPL.sADEP, dataFPL.sADES);
        if(pItemData.GetSFPL())
        {
            qWarning() << tr("discard the message FPL because it is already SFPL");
        }
        else
        {
            TVA_CATCHALL_TRY
            {
                QSharedPointer<CSFPL> pSFPL(new CSFPL);
                OLDIMessage::SetABItoSFPL(dataFPL, pSFPL);
                dao()->UpdateFPLTable(pSFPL);
                pMes->SetExecuted();
            }TVA_CATCHALL(err){
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
        }

        if(pMes->GetMesData().bAnswer)
        {
            OLDIMesData dataLAM = pMes->GetLAM(m_pMaster->GetOLDIProc()->GetNextMesNum());
            QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
            m_pMaster->GetOLDIProc()->SendMessage(pMesLAM);
        }
        return true;
    }

    return false;
}

void CListAC::UpdateViewData()
{
    if (!sListType.isEmpty()){
        //qDebug()<<"__________________________________UpdateViewData";
        //globalSettings()->beginGroup(currentUserName + "/" + sListType);
        //for all users, nah
        globalSettings()->beginGroup("ListConstructor/" + sListType);
        EditHeaderItems     Items;
        Items.clear();
        foreach (const QString &item, globalSettings()->childKeys()){
            if (item.left(4) == QString("data"))
            {
                QDataStream stream(
                            QByteArray::fromHex(
                                globalSettings()->value(item).toByteArray()));
                EditHeaderItem it;
                stream >> it;
                Items.append(it);
            }
        }
        globalSettings()->endGroup();

        if (!Items.isEmpty()){
            QStringList         headers;
            headers.reserve(Items.count());
            for (int i = 0; i < Items.count(); i++)
                headers.append(QString(""));

            int ScrollVisualIdx = 10000;
            foreach(EditHeaderItem item, Items){

                int idx = GetColumnIndex(item.identifier);
                if(idx == CListACModel::Scroll)
                    ScrollVisualIdx = item.number;
                if((idx >= 0) && (idx < headers.size())){
                    headers.replace(idx, item.texts[0]);
                    int visual_idx = m_list.horizontalHeader()->visualIndex(idx);

                    if(visual_idx != item.number)
                        m_list.horizontalHeader()->swapSections(visual_idx, item.number);

                    if(item.visible)
                        m_list.resizeColumnToContents(idx);
                    else
                        m_list.horizontalHeader()->resizeSection(idx, 0);
                }
            }
            //m_list.setPlanHeaders(headers);

            m_ScrolledItems.clear();
            foreach (EditHeaderItem item, Items) {
                if (item.number > ScrollVisualIdx && item.visible)
                {
                    int idx = GetColumnIndex(item.identifier);
                    if (idx != -1)
                        m_ScrolledItems.append(idx);
                }
            }

            if (m_list.isScrolled())
            {
                foreach (int idx, m_ScrolledItems)
                    m_list.horizontalHeader()->resizeSection(idx, 0);
            }

            m_sortModel.setHeaderData(CListACModel::Scroll, Qt::Horizontal, 0, Qt::UserRole);
        }
    }
}

void CListAC::OnScrollItem(int index)
{
    if(CListACModel::Scroll == index && !m_ScrolledItems.isEmpty())
    {
        _toggleScroll();
    }
}

void CListAC::fillSortModel()
{
    CListACSortModel::TListColType listCol(CListACModel::COL_COUNT);
    for (int i = 0; i < CListACModel::COL_COUNT; ++i)
    {
        switch (i)
        {
        case CListACModel::ACID: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ACID, false); break;
        case CListACModel::CODE: listCol[i] = CListACSortModel::CTypeCol(CListACModel::CODE, false); break;
        case CListACModel::COPin: listCol[i] = CListACSortModel::CTypeCol(CListACModel::COPin, false); break;
        case CListACModel::ETOin: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETOin, false); break;
        case CListACModel::ETO_ATOin: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETO_ATOin, false); break;
        case CListACModel::PEL: listCol[i] = CListACSortModel::CTypeCol(CListACModel::PEL, false); break;
        case CListACModel::PEL_CFL: listCol[i] = CListACSortModel::CTypeCol(CListACModel::PEL_CFL, false); break;
        case CListACModel::COPout: listCol[i] = CListACSortModel::CTypeCol(CListACModel::COPout, false); break;
        case CListACModel::ETOout: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETOout, false); break;
        case CListACModel::ETO_ATOout: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETO_ATOout, false); break;
        case CListACModel::RFL_TFL: listCol[i] = CListACSortModel::CTypeCol(CListACModel::RFL_TFL, false); break;
        case CListACModel::REM: listCol[i] = CListACSortModel::CTypeCol(CListACModel::REM, false); break;
        case CListACModel::RVSM: listCol[i] = CListACSortModel::CTypeCol(CListACModel::RVSM, false); break;
        case CListACModel::Type: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Type, false); break;
        case CListACModel::Type_AC: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Type_AC, false); break;
        case CListACModel::ADEP: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ADEP, false); break;
        case CListACModel::DEST: listCol[i] = CListACSortModel::CTypeCol(CListACModel::DEST, false); break;
        case CListACModel::ALTN: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ALTN, false); break;
        case CListACModel::Scroll: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Scroll, false); break;
        case CListACModel::Wct: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Wct, false); break;
        case CListACModel::Stand: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Stand, false); break;
        case CListACModel::SID: listCol[i] = CListACSortModel::CTypeCol(CListACModel::SID, false); break;
        case CListACModel::ETD_ATD: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETD, false); break;
        case CListACModel::COPin_SID: listCol[i] = CListACSortModel::CTypeCol(CListACModel::COPin_SID, false); break;
        case CListACModel::ETO_ATO_ETD: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETO_ATO_ETD, false); break;
        case CListACModel::STAR: listCol[i] = CListACSortModel::CTypeCol(CListACModel::STAR, false); break;
        case CListACModel::COPout_STAR: listCol[i] = CListACSortModel::CTypeCol(CListACModel::COPout_STAR, false); break;
        case CListACModel::ETO_ATO_ATA: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETO_ATO_ATA, false); break;
        case CListACModel::Azimuth: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Azimuth, false); break;
        case CListACModel::CurrentSpeed: listCol[i] = CListACSortModel::CTypeCol(CListACModel::CurrentSpeed, false); break;
        case CListACModel::Eq: listCol[i] = CListACSortModel::CTypeCol(CListACModel::Eq, false); break;
        case CListACModel::STATUS: listCol[i] = CListACSortModel::CTypeCol(CListACModel::STATUS, false); break;
        case CListACModel::LIT: listCol[i] = CListACSortModel::CTypeCol(CListACModel::LIT, false); break;
        case CListACModel::FPLEqf1: listCol[i] = CListACSortModel::CTypeCol(CListACModel::FPLEqf1, false); break;
        case CListACModel::FPLEqf2: listCol[i] = CListACSortModel::CTypeCol(CListACModel::FPLEqf2, false); break;
        case CListACModel::FPLEqf3: listCol[i] = CListACSortModel::CTypeCol(CListACModel::FPLEqf3, false); break;
        case CListACModel::ETD: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETD, false); break;
        case CListACModel::ETA: listCol[i] = CListACSortModel::CTypeCol(CListACModel::ETA, false); break;
        case CListACModel::FType: listCol[i] = CListACSortModel::CTypeCol(CListACModel::FType, false); break;
        case CListACModel::MesType: listCol[i] = CListACSortModel::CTypeCol(CListACModel::MesType, false); break;
        case CListACModel::DATE: listCol[i] = CListACSortModel::CTypeCol(CListACModel::DATE, false); break;
        }
    }
    m_sortModel.SetColumns(listCol);

    connect(m_list.horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(OnScrollItem(int)));
    m_sortModel.setHeaderData(m_list.horizontalHeader()->visualIndex(CListACModel::Scroll), Qt::Horizontal, 0, Qt::UserRole);
}

int CListAC::GetColumnIndex(const QString &column_name)
{
    int idx = -1;
    if (column_name == tr("ACID"))
        idx = CListACModel::ACID;
    else if (column_name == tr("CODE"))
        idx = CListACModel::CODE;
    else if (column_name == tr("COPin"))
        idx = CListACModel::COPin;
    else if (column_name == tr("SID"))
        idx = CListACModel::SID;
    else if(column_name == tr("COPin_SID"))
        idx = CListACModel::COPin_SID;
    else if(column_name == tr("ETOin"))
        idx = CListACModel::ETOin;
    else if(column_name == tr("ETO_ATOin"))
        idx = CListACModel::ETO_ATOin;
    else if(column_name == tr("ETD_ATD"))
        idx = CListACModel::ETD_ATD;
    else if(column_name == tr("ETO_ATO_ETD"))
        idx = CListACModel::ETO_ATO_ETD;
    else if(column_name == tr("PEL"))
        idx = CListACModel::PEL;
    else if(column_name == tr("PEL_CFL"))
        idx = CListACModel::PEL_CFL;
    else if(column_name == tr("COPout"))
        idx = CListACModel::COPout;
    else if(column_name == tr("STAR"))
        idx = CListACModel::STAR;
    else if(column_name == tr("COPout_STAR"))
        idx = CListACModel::COPout_STAR;
    else if(column_name == tr("ETOout"))
        idx = CListACModel::ETOout;
    else if(column_name == tr("ETO_ATOout"))
        idx = CListACModel::ETO_ATOout;
    else if(column_name == tr("ETO_ATO_ATA"))
        idx = CListACModel::ETO_ATO_ATA;
    else if(column_name == tr("RFL_TFL"))
        idx = CListACModel::RFL_TFL;
    else if(column_name == tr("Type"))
        idx = CListACModel::Type;
    else if (column_name == tr("Wct"))
        idx = CListACModel::Wct;
    else if (column_name == tr("Stand"))
        idx = CListACModel::Stand;
    else if (column_name == tr("REM"))
        idx = CListACModel::REM;
    else if (column_name == tr("RVSM"))
        idx = CListACModel::RVSM;
    else if (column_name == tr("Type_AC"))
        idx = CListACModel::Type_AC;
    else if (column_name == tr("ADEP"))
        idx = CListACModel::ADEP;
    else if (column_name == tr("DEST"))
        idx = CListACModel::DEST;
    else if (column_name == tr("ALTN"))
        idx = CListACModel::ALTN;
    else if(column_name == QString("Course Angle"))//tr("Course Angle"))
        idx = CListACModel::Azimuth;
    else if (column_name == tr("Speed"))
        idx = CListACModel::CurrentSpeed;
    else if (column_name == tr("Eq"))
        idx = CListACModel::Eq;
    else if (column_name == tr("STATUS"))
        idx = CListACModel::STATUS;
    else if (column_name == tr("LIT"))
        idx = CListACModel::LIT;
    else if (column_name == tr("FPLEqf1"))
        idx = CListACModel::FPLEqf1;
    else if (column_name == tr("FPLEqf2"))
        idx = CListACModel::FPLEqf2;
    else if (column_name == tr("FPLEqf3"))
        idx = CListACModel::FPLEqf3;
    else if (column_name == tr("ETD"))
        idx = CListACModel::ETD;
    else if (column_name == tr("ETA"))
        idx = CListACModel::ETA;
    else if (column_name == tr("FType"))
        idx = CListACModel::FType;
    else if (column_name == tr("MesType"))
        idx = CListACModel::MesType;
    else if (column_name == tr("DATE"))
        idx = CListACModel::DATE;
    else if (column_name == tr("Scroll"))
        idx = CListACModel::Scroll;
    return idx;
}

void CListAC::_toggleScroll()
{
    if(m_list.isScrolled())
    {
        m_list.scroll(false);
        foreach(int idx, m_ScrolledItems)
            m_list.resizeColumnToContents(idx);
    }
    else
    {
        m_list.scroll(true);
        foreach(int idx, m_ScrolledItems)
            m_list.horizontalHeader()->resizeSection(idx, 0);
    }
    SizeToContents();
}

CListACData& CListAC::GetDataBySFPLkey(QString sACID, QString sADEP, QString sADES)
{
    CListACModel::TListACData& pData = m_model.GetModelData();
    for(int i = 0; i < pData.size(); ++i)
    {
        CListACData& pItemData = pData[i];
        if(pItemData.GetSFPL())
        {
            if(pItemData.GetSFPL()->GetACID() == sACID &&
               pItemData.GetSFPL()->GetDepAerodrome() == sADEP &&
               pItemData.GetSFPL()->GetDestAerodrome() == sADES)
                return pItemData;
        }
    }
    return m_model.GetItemData(QModelIndex());
}

QSharedPointer<CSFPL> CListAC::GetSFPLByCode(quint32 code)
{
    foreach (const CListACData& item, m_model.GetModelData())
    {
        auto sfpl = item.GetSFPL();
        if (sfpl && sfpl->GetCODE() == code)
        {
            return sfpl;
        }
    }

    return QSharedPointer<CSFPL>();
}

void CListAC::OnEnterLeaveList(bool bEnter)
{
    QModelIndex index = m_list.currentIndex();
    if(index.isValid() && !bEnter)
    {
        QSharedPointer<CAirplane> pPlane = index.data(Qt::UserRole).value<CListACData>().GetAirplane();
        if(pPlane)
        {
            CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*> (pPlane.data());
            if(pGUIPlane)
                pGUIPlane->ActivatePlane(bEnter);
        }
    }
}

QString CListAC::GetAirportSector(const QString& sAirport, const QStringList& sSectors)
{
    QString sInSector;
    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_pMaster->GetView());
    if(m_pMaster && view && view->GetMainFIR())
    {
        Scene::TAirportsMap::const_iterator itAirport = view->GetMainFIR()->GetAirports().constFind(sAirport);
        if(itAirport != view->GetMainFIR()->GetAirports().constEnd())
        {
            QMultiMap<double, Scene::TScenePtr> mapSector;
            Scene::TSceneList::const_iterator itSector = view->GetMainFIR()->GetSectorList().constBegin();
            for(; itSector != view->GetMainFIR()->GetSectorList().constEnd(); ++itSector)
            {
                if((*itSector)->IsItemActive())
                {
                    if (((*itAirport)->GetAlt() >= (*itSector)->GetLowLimit() && ((*itAirport)->GetAlt() <= (*itSector)->GetUpLimit()
                        || (*itSector)->GetUpLimit() == 0)) && (*itSector)->IsIncluded(
                                (*itAirport)->GetCoord()))
                    {
                        double altKey = (*itSector)->GetUpLimit() == 0 ? 1000000 : (*itSector)->GetUpLimit();
                        mapSector.insertMulti(altKey, *itSector);
                    }
                }
            }
            QMultiMap<double, Scene::TScenePtr>::const_iterator itMap = mapSector.constBegin();
            for(; itMap != mapSector.constEnd(); ++itMap)
            {
                if(sSectors.contains((*itMap)->GetShortName()))
                {
                    sInSector = (*itMap)->GetShortName();
                    break;
                }
            }
        }
    }
    return sInSector;
}

QString CListAC::CalcInSector(QSharedPointer<CSFPL> pSFPL, bool bPodpora/* = true*/)
{
    QString sInSector;
    if(pSFPL && m_pMaster && m_pMaster->GetView())
    {
        Scene::TSceneList listInSectors;
        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_pMaster->GetView());
        if(view)
        {
            Scene::TSceneList listSectors = view->FindSectorsByPoint(pSFPL->GetCOPin(), pSFPL->GetCOPinFL().GetM());
            if(pSFPL->GetFPLFrag())
                listSectors = pSFPL->GetFPLFrag()->GetWhichRouteSectors(pSFPL->GetCOPin(), pSFPL->GetCOPinFL().GetM(), listSectors);
            QStringList sCurSectorsList = networkSettings()->value("users/current_sectors_short_name").toString().split("-", QString::SkipEmptyParts);
            Scene::TSceneList::const_iterator itSector = listSectors.constBegin();
            for(; itSector != listSectors.constEnd(); ++itSector)
            {
                if(sCurSectorsList.contains((*itSector)->GetShortName()))
                    listInSectors.append(*itSector);
            }

            if(listInSectors.size())
            {
                if(listInSectors.size() > 1)
                {
                    QString sText;
                    itSector = listInSectors.constBegin();
                    for(; itSector != listInSectors.constEnd(); ++itSector)
                    {
                        if(!sText.isEmpty()) sText += "-";
                        sText += (*itSector)->GetShortName();
                    }
                    qDebug() << "more then 1 InSector " << sText;
                }
                sInSector = listInSectors.first()->GetShortName();
            }
            else
            {
                QStringList sAirportList = networkSettings()->value("users/current_sector_airports").toString().split(",", QString::SkipEmptyParts);
                if(sAirportList.contains(pSFPL->GetDepAerodrome()))
                {
                    sInSector = GetAirportSector(pSFPL->GetDepAerodrome(), sCurSectorsList);
                }
            }
            ////podpora
            if(bPodpora)
            {
                if(sInSector.isEmpty() && sCurSectorsList.size())
                    sInSector = sCurSectorsList.first();
            }
            ////
            pSFPL->SetInSector(sInSector);
        }
    }
    return sInSector;
}

void CListAC::OnCancelAct()
{
    QSharedPointer<CSFPL> pSFPL = m_pActionSFPL.toStrongRef();
    if(pSFPL)
    {
        QMessageBox::StandardButton btn = QMessageBox::warning(this, tr("SFPL"), tr("Cancel ACT '%1'?")
                                       .arg(pSFPL->GetACID())
                                       , QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        if(btn != QMessageBox::Yes) return;

        //undo SFPL
        QSharedPointer<obelix::CCmdChangeSFPL> pCmd(new obelix::CCmdChangeSFPL(obelix::CCmdChangeSFPL::Cancel));
        pCmd->SetValue(obelix::CCmdChangeSFPL::XML_CODE, QString::number(pSFPL->GetCODE()));
        pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPIN, pSFPL->GetCOPin());
        pCmd->SetValue(obelix::CCmdChangeSFPL::XML_ETOIN, pSFPL->GetETOin().toString());
        pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPINFL, pSFPL->GetCOPinFL().toString());
        pCmd->SetValue(obelix::CCmdChangeSFPL::XML_FPLID, QString::number(pSFPL->GetID()));
        m_pMaster->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());

//        OLDIMesDataINF dataINF;
//        dataINF.iMesNum = 0;
//        dataINF.sTypeINF = oldi::INF_UNDO_SFPL;
//        dataINF.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
//        dataINF.sTo = dataINF.sFrom;
//        dataINF.sACID = pSFPL->GetACID();
//        dataINF.sADEP = pSFPL->GetDepAerodrome();
//        dataINF.sADES = pSFPL->GetDestAerodrome();
//        dataINF.sCODE = pSFPL->GetCODEtoString();
//        dataINF.sSector = " ";

//        QSharedPointer<OLDIMessage> pMesINF(new OLDIMesINF(dataINF));
//        pMesINF->GetMesSourceData().bAnswer = false;
//        RecieveOLDIMessage(pMesINF);

//        dataINF.iMesNum = m_pMaster->GetOLDIProc()->GetNextMesNum();
//        dataINF.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();

//        pMesINF = QSharedPointer<OLDIMessage>(new OLDIMesINF(dataINF));
//        m_pMaster->GetOLDIProc()->SendMessage(pMesINF);

//        QString sCWP = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
//        QStringList sCWPList = sCWP.split(",", QString::SkipEmptyParts);
//        for(int i = 0; i < sCWPList.size(); ++i)
//        {
//            dataINF.iMesNum = m_pMaster->GetOLDIProc()->GetNextMesNum();
//            dataINF.sTo = sCWPList.at(i);

//            pMesINF = QSharedPointer<OLDIMessage>(new OLDIMesINF(dataINF));
//            m_pMaster->GetOLDIProc()->SendMessage(pMesINF);
//        }
    }
}

void CListAC::OnKeyPressed(const QModelIndex& index, QKeyEvent* event)
{
    if(index.isValid())
    {
        const CListACData& itemData = index.data(Qt::UserRole).value<CListACData>();
        switch(event->key())
        {
        case Qt::Key_Delete:
            {
                if((Qt::NoModifier == event->modifiers()) ||
                   (Qt::KeypadModifier == event->modifiers()))
                {
                    if(itemData.GetSFPL())
                    {
                        m_pActionSFPL = itemData.GetSFPL().toWeakRef();
                        OnCancelAct();
                    }
                }
                break;
            }
        }
    }
}

void CListAC::ItemEditACID(QSharedPointer<CSFPL> pSFPL)
{
    if(pSFPL)
    {
        TVA_CATCHALL_TRY
        {
            FPL Fpl(pSFPL->GetID(), Dao::MASTER_SFPL_TABLE);
            QSqlError sqlError = AFTNDao::loadFPL(Fpl);
            if(sqlError.type() != QSqlError::NoError)
            {
                qDebug() << sqlError.text();
            }

            FplEditor dlg(this);
            dlg.setCanOtherInfoEmpty(networkSettings()->value("FPL_handler/field_18", false).toBool());
            dlg.setFpl(Fpl);
            if(QDialog::Accepted == dlg.exec())
            {
                // update FPL
                //LOG_OPER_SIMPLE_STR("Change ACID " + pSFPL->GetACID());
//                LOG_OPER_STR("ALL", "ACID", pSFPL->GetACID());
                //Refresh(pSFPL->GetID());

                // send ABI
                QSharedPointer<obelix::CCmdChangeSFPL> pCmd(new obelix::CCmdChangeSFPL(obelix::CCmdChangeSFPL::Edit));
                pCmd->SetValue(obelix::CCmdChangeSFPL::XML_CODE, QString::number(pSFPL->GetCODE()));
                pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPIN, pSFPL->GetCOPin());
                pCmd->SetValue(obelix::CCmdChangeSFPL::XML_ETOIN, pSFPL->GetETOin().toString());
                pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPINFL, pSFPL->GetCOPinFL().toString());
                pCmd->SetValue(obelix::CCmdChangeSFPL::XML_FPLID, QString::number(pSFPL->GetID()));
                m_pMaster->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
//                QSharedPointer<CSFPL> pSFPLtoSend;
//                CListACModel::TListACData& pData = m_model.GetModelData();
//                CListACModel::TListACData::const_iterator itData = pData.begin();
//                for (; itData != pData.end(); ++itData)
//                {
//                    if (itData->GetSFPL() && itData->GetSFPL()->GetID() == pSFPL->GetID())
//                    {
//                        pSFPLtoSend = itData->GetSFPL();
//                        break;
//                    }
//                }
//                if(pSFPLtoSend)
//                {
//                    OLDIMesDataABI dataABI = OLDIMessage::GetABIfromSFPL(pSFPLtoSend);
//                    dataABI.iMesNum = m_pMaster->GetOLDIProc()->GetNextMesNum();
//                    dataABI.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
//                    dataABI.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
//                    QSharedPointer<OLDIMessage> pMesABI(new OLDIMesABI(dataABI));
//                    m_pMaster->GetOLDIProc()->SendMessage(pMesABI);

//                    QString sSectors = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
//                    QStringList sSectorsList = sSectors.split(",", QString::SkipEmptyParts);
//                    for(int i = 0; i < sSectorsList.size(); ++i)
//                    {
//                        dataABI.sTo = sSectorsList[i];
//                        pMesABI = QSharedPointer<OLDIMessage>(new OLDIMesABI(dataABI));
//                        m_pMaster->GetOLDIProc()->SendMessage(pMesABI);
//                    }
//                }
            }
        }
        TVA_CATCHALL(err)
        {
            QMessageBox::critical(this, tr("SFPL"), err.reason());
        }
    }
}

QDateTime CListAC::lastTimeRefresh()
{
    return mLastTimeRefresh;
}

QList<int> CListAC::GetMode3AReplyList()
{
    QList<int> list;
    CListACModel::TListACData::const_iterator itData = m_model.GetModelData().constBegin();
    for(; itData != m_model.GetModelData().constEnd(); ++itData)
    {
        if(itData->GetAirplane())
            list.append(itData->GetAirplane()->GetMode3AReply());
        /*else */if(itData->GetSFPL())
            list.append(itData->GetSFPL()->GetCODE());
    }
    return list;
}

void CListAC::StopFutureRefresh()
{
    m_FutureRefreshForceStop = true;
    if(m_FutureRefreshFutwatch)
    {
        m_FutureRefreshFutwatch->cancel();
        m_FutureRefreshFutwatch->waitForFinished();
        delete m_FutureRefreshFutwatch;
    }
    m_FutureRefreshFutwatch = 0;
    m_FutureRefreshForceStop = false;
}

bool CListAC::isFutureRefreshRunning()
{
    if(m_FutureRefreshFutwatch && m_FutureRefreshFutwatch->isRunning())
        return true;
    return m_bFutureRefreshInProcess;
}

// previos version
void CListAC::OnFutureRefresh(quint32 qSFPLID, bool bProcessEvents)
{
    if (m_bFutureRefreshInProcess) return;

    if (bProcessEvents)
        bProcessEvents = globalSettings()->value("XMasterWindow/Settings/ProcessEvents", false).toBool();
    bProcessEvents = false;
    QEventLoop::ProcessEventsFlags processEventsFlags = QEventLoop::AllEvents;//QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers;

    QList<QSharedPointer<CSFPL> > SFPLList;
    TVA_CATCHALL_TRY
    {
        dao()->LoadSFPLs(SFPLList);
    }TVA_CATCHALL(err){
        QMessageBox::critical(this, tr("SFPL"), err.reason());
    }

    m_bFutureRefreshInProcess = true;
    //master()->BlockNetworkMessages(true);

    if (bProcessEvents)
        qApp->processEvents(processEventsFlags);

    if (!liveObjects.contains(this))
        return;

    CListACData curSelectData = m_model.GetItemData(m_sortModel.mapToSource(m_list.currentIndex()));
    CListACModel::TListACData oldData = m_model.GetModelData();
    CListACModel::TListACData listData(SFPLList.size());
    for (int i = 0; i < SFPLList.size(); ++i)
    {
        CListACData itemData;
        itemData.SetSFPL(SFPLList[i]);
        if (SFPLList[i]->GetOther().contains("NON RVSM"))
            itemData.SetRVSM(tr("NON RVSM"));
        else
        {
            if (SFPLList[i]->GetEquipment().contains("W"))
                itemData.SetRVSM(tr("YES"));
            else
                itemData.SetRVSM(tr("NO"));
        }

        if (bProcessEvents) {
            qApp->processEvents(processEventsFlags);
            if (!liveObjects.contains(this))
                return;
        }

        CListACModel::TListACData::iterator itData = oldData.begin();
        for (; itData != oldData.end(); ++itData)
        {
            if (itData->GetSFPL()->GetUid() == SFPLList[i]->GetUid())
            {
                if (itData->GetSFPL()->GetID() == qSFPLID)
                {
                    const QSharedPointer<CAirplane> &airplane = itData->GetAirplane();
                    if (airplane)
                    {
                        const QSharedPointer<CSFPL> &sfpl = itemData.GetSFPL();
                        if (sfpl) {
                            const CAirplaneData::PlaneType plane_type = airplane->GetPlaneType();
                            if ((CAirplaneData::ASSUME == plane_type) || (CAirplaneData::Transfer == plane_type)) {
                                //if (sfpl->GetCurSector() != networkSettings()->value("users/current_sector"))
                                const QStringList controlled_sectors = networkSettings()->value("users/current_sectors_short_name").toString().split("-", QString::SkipEmptyParts);
                                if (!controlled_sectors.contains(sfpl->GetCurSector()))
                                    airplane->SetPlaneType(CAirplaneData::Transfered);
                            } else if (sfpl->GetNextSector() == networkSettings()->value("users/current_sector")) {
                                airplane->SetPlaneType(CAirplaneData::ACT);
                            }
                        }
                        itemData.SetIPS_Small(itData->GetIPS_Small());
                        itemData.SetIPS_Big(itData->GetIPS_Big());
                        itemData.SetWRN(itData->GetWRN());
                        //Pasha itemData.SetRVSM(itData->GetRVSM());
                        itemData.SetAirplane(airplane);
                        airplane->SetCFL(itemData.GetSFPL()->GetCFL());
                        airplane->SetTFL(itemData.GetSFPL()->GetTFL());
                        airplane->SetSFPL(itemData.GetSFPL());
                    }
                }
                else
                {
                    if (itData->GetAirplane())
                        itemData = *itData;
                }
                break;
            }
        }
        listData[i] = itemData;
    }
    m_model.SetModelData(listData);
    SelectRow(curSelectData);
    SizeToContents();
    if (!m_list.underMouse())
        OnEnterLeaveList(false);

    m_bFutureRefreshInProcess = false;
    //master()->BlockNetworkMessages(false);
}
/*
void CListAC::OnFutureRefresh(quint32 qSFPLID)
{
    m_qSFPLIDwhenRunning = UINT_MAX;
    ////////////////////////////////
//    if(!m_FutureRefreshFutwatch)
//    {
//        m_FutureRefreshFutwatch = new QFutureWatcher<CListACData>();
//        connect(m_FutureRefreshFutwatch, SIGNAL(resultReadyAt(int)), this, SLOT(OnFutureRefreshFutwatchResult(int)));
//        connect(m_FutureRefreshFutwatch, SIGNAL(finished()), this, SLOT(OnFutureRefreshFutwatchFinish()));
//    }
    ////////////////////////////////
    if(m_FutureRefreshFutwatch && !m_FutureRefreshFutwatch->isRunning())
    {
        if(m_FutureRefreshForceStop) return;
        qDebug() << "OnFutureRefresh new run " << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
        QList<QSharedPointer<CSFPL> > SFPLList;
        TVA_CATCHALL_TRY
        {
            dao()->LoadSFPLs(SFPLList);
        }TVA_CATCHALL(err){
            QMessageBox::critical(this, tr("SFPL"), err.reason());
        }
        if(m_FutureRefreshForceStop) return;

        if(m_pNewListData)
        {
            m_pNewListData->clear();
            delete m_pNewListData;
            m_pNewListData = 0;
        }
        m_pNewListData = new CListACModel::TListACData(SFPLList.size());
        for(int i = 0; i < SFPLList.size(); ++i)
        {
            CListACData itemData;
            itemData.SetSFPL(SFPLList[i]);
            if(SFPLList[i]->GetOther().contains("NON RVSM"))
                itemData.SetRVSM(tr("NON RVSM"));
            else
            {
                if(SFPLList[i]->isRVSM())
                    itemData.SetRVSM(tr("YES"));
                else
                    itemData.SetRVSM(tr("NO"));
            }
            (*m_pNewListData)[i] = itemData;
        }
        m_qSFPLID = qSFPLID;
        m_FutureRefreshFutwatch->setFuture(QtConcurrent::mapped(*m_pNewListData, &CListAC::CalcFutureRefresh));
        // for debug
//        for(int i = 0; i < m_pNewListData->size(); ++i)
//        {
//            CListACData itemData = CalcFutureRefresh((*m_pNewListData)[i]);
//            debugOnFutureRefreshFutwatchResult(i, itemData);
//        }
//        debugOnFutureRefreshFutwatchFinish();
        ///////////////////////////////////////////////////
    }
    else
    {
        if(m_FutureRefreshFutwatch)
        {
            qDebug() << "OnFutureRefresh running";
            if(!m_FutureRefreshForceStop)
                m_qSFPLIDwhenRunning = qSFPLID;
        }
    }
}
*/
void CListAC::OnFutureRefreshFutwatchResult(int index)
{
    qDebug() << QString("OnFutureRefresh result for index = %1 ").arg(index) << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    if(m_pNewListData && !m_FutureRefreshForceStop)
    {
        if(index >= 0 && index < m_pNewListData->size())
            (*m_pNewListData)[index] = m_FutureRefreshFutwatch->resultAt(index);
    }
}

void CListAC::OnFutureRefreshFutwatchFinish()
{
    qDebug() << "OnFutureRefresh finish "<< QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    if(m_pNewListData)
    {
        if(!m_FutureRefreshForceStop)
        {
            CListACData curSelectData = m_model.GetItemData(m_sortModel.mapToSource(m_list.currentIndex()));
            m_model.SetModelData(*m_pNewListData);
            SelectRow(curSelectData);
            SizeToContents();
            if(!m_list.underMouse())
                OnEnterLeaveList(false);
        }
        m_pNewListData->clear();
        delete m_pNewListData;
    }
    m_pNewListData = 0;

    if(!m_FutureRefreshForceStop && m_qSFPLIDwhenRunning != UINT_MAX)
    {
        QMetaObject::invokeMethod(this, "OnFutureRefresh", Qt::QueuedConnection, Q_ARG(quint32, m_qSFPLIDwhenRunning));
    }

}

CListACData CListAC::CalcFutureRefresh(const CListACData& acData)
{
    if(m_FutureRefreshForceStop)
        return CListACData();
    // set FPL route fragmentation
    CListACData itemData(acData);
    QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(master()->GetView());
    CListACModel::TListACData& oldData = m_model.GetModelData();
    QSharedPointer<CSFPL> pSFPL = itemData.GetSFPL();
    TVA_CATCHALL_TRY
    {
        if(pView && pSFPL)
        {
#if defined(REDIS)
             QSharedPointer<CFPLFrag> pFrag;
#else
            QSharedPointer<CFPLFrag> pFrag = CFPLFrag::FragmentateFPL(pSFPL, pView);
#endif
            if(pFrag && !m_FutureRefreshForceStop)
            {
                pFrag->SetFPL(pSFPL);
                pSFPL->SetFPLFrag(pFrag);

                // use simulation
                if(networkSettings()->value("simulator/use_route_simulation").toString().toLower() == "true")
                {
                    qDebug() << "START SIMULATION " << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
                    QSharedPointer<SimulatorTrack> simTrack(new SimulatorTrack);
                    int iBeforehandFly = CPlane::GetStepSeconds();
                    CPlane simPlane(pSFPL, pView->GetCentralPoint()->GetCoord(), iBeforehandFly, true);
                    bool bLife(true);
                    int iNonCyclic = 24*60*60 / CPlane::GetStepSeconds();
                    for(int i = 0; bLife && i < iNonCyclic; ++i)
                    {
                        bLife = simPlane.CalculatePosition(1);
                        if(bLife)
                        {
                            simTrack->AddPoint(simPlane, i*CPlane::GetStepSeconds()-iBeforehandFly);
                        }
                        if(m_FutureRefreshForceStop)
                            return CListACData();
                    }
                    simTrack->CalculateSectors(pView);
                    if(m_FutureRefreshForceStop)
                        return CListACData();
                    pSFPL->SetSimTrack(simTrack);
                    if(m_FutureRefreshForceStop)
                        return CListACData();
                    pFrag->SimExtRoute();
                    ////////////////
                    //CGUIView* pView = dynamic_cast<CGUIView*> (pView.data());
                    //if (pView)
                    //{
                    //    pView->AddTempLine(simTrack->GetAllPoints().first()->GetCoord(), simTrack->GetAllPoints().last()->GetCoord());
                    //}
                    ////////////////
                    qDebug() << "END SIMULATION   " << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
                }

                if(m_FutureRefreshForceStop)
                    return CListACData();
                QList<QSharedPointer<Scene > > listSectors = pFrag->GetRouteSectors(pView);
                QString sSectors;
                QList<QSharedPointer<Scene > >::const_iterator itSector = listSectors.constBegin();
                for(; itSector != listSectors.constEnd(); ++itSector)
                {
                    if(sSectors.size()) sSectors += "-";
                    sSectors += (*itSector)->GetShortName();
                }
                pSFPL->SetAllSectors(sSectors);

                CProcFrag::TPointFragList pPoint = pFrag->GetSectorPoints(pView);
                if(pPoint.size())
                {
                    if(pSFPL->GetCOPin() == pPoint.first()->GetPoint()->GetName())
                    {
                        pSFPL->SetCOPinPoint(pPoint.first());
                    }
                    if(pSFPL->GetCOPin() != pPoint.last()->GetPoint()->GetName())
                    {
                        pSFPL->SetCOPout(pPoint.last()->GetPoint()->GetName());
                        pSFPL->SetCOPoutPoint(pPoint.last());
                    }
                }

                //??? test
                if(pSFPL->GetInSector().isEmpty())
                    if(m_listView.size()) m_listView.first()->CalcInSector(pSFPL, false);
                //////////
            }
        }
    }
    TVA_CATCHALL(err)
    {
        LOG_STR(Log4Qt::Level::DEBUG_INT, "FragmentateFPL. " + err.reason());
    }

    if (pSFPL && !m_FutureRefreshForceStop)
    {
        CListACModel::TListACData::iterator itData = oldData.begin();
        for(; itData != oldData.end(); ++itData)
        {
            if(itData->GetSFPL()->GetID() == pSFPL->GetID())
            {
                if(itData->GetSFPL()->GetID() == m_qSFPLID)
                {
                    if (itData->GetAirplane())
                    {
                        QSharedPointer<CSFPL> pSFPL = itData->GetAirplane()->GetSFPL();
                        if (pSFPL)
                        {
                            QSharedPointer<CFPLFrag> pFragPlane = pSFPL->GetFPLFrag();
                            QSharedPointer<CFPLFrag> pFragSFPL = itemData.GetSFPL()->GetFPLFrag();
                            if (pFragSFPL && pFragPlane)
                                if(m_listView.size()) m_listView.first()->MigrationFrag(pFragSFPL, pFragPlane);
                        }
                        itemData.SetIPS_Small(itData->GetIPS_Small());
                        itemData.SetIPS_Big(itData->GetIPS_Big());
                        itemData.SetWRN(itData->GetWRN());
                        //Pasha itemData.SetRVSM(itData->GetRVSM());
                        itemData.SetAirplane(itData->GetAirplane());
                        itemData.GetAirplane()->SetSFPL(itemData.GetSFPL());
                        // may be applied history here?
                        if (itemData.GetAirplane()->GetSFPL()->GetFPLFrag())
                            itemData.GetAirplane()->GetSFPL()->GetFPLFrag()->ApplyHistory(itemData.GetAirplane()->GetHistoryPoints());
                    }
                }
                else
                {
                    if (itData->GetAirplane())
                        itemData = *itData;
                }
                break;
            }
        }
    }
    return itemData;
}

void CListAC::debugOnFutureRefreshFutwatchResult(int index, CListACData itemData)
{
    if (m_pNewListData)
    {
        if (index >= 0 && index < m_pNewListData->size())
            (*m_pNewListData)[index] = itemData;
    }
}

void CListAC::debugOnFutureRefreshFutwatchFinish()
{
    if (m_pNewListData)
    {
        CListACData curSelectData = m_model.GetItemData(m_sortModel.mapToSource(m_list.currentIndex()));
        m_model.SetModelData(*m_pNewListData);
        SelectRow(curSelectData);
        SizeToContents();
        if (!m_list.underMouse())
            OnEnterLeaveList(false);
        delete m_pNewListData;
    }
    m_pNewListData = 0;
}

QStringList CListAC::getFirPoints()
{
    QStringList points;
    if (master()) {
        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (view) {
            CView::TSceneList::const_iterator itFIR = view->GetScenes().constBegin();
            for (; itFIR != view->GetScenes().constEnd(); ++itFIR)
            {
                Scene::TPointsMap::const_iterator itPoint = (*itFIR)->GetPoints().begin();
                for (; itPoint != (*itFIR)->GetPoints().end(); ++itPoint)
                    points << (*itPoint)->GetName();
            }
        }
    }
    points.removeAll("");
    points.removeDuplicates();
    return points;
}

QStringList CListAC::getFir2Points()
{
    QStringList points;
    if (master()) {
        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (view) {
            CView::TSceneList::const_iterator itFIR = view->GetScenes().constBegin();
            for (; itFIR != view->GetScenes().constEnd(); ++itFIR)
            {
                Scene::TPointsMap::const_iterator itPoint = (*itFIR)->GetPoints().begin();
                for (; itPoint != (*itFIR)->GetPoints().end(); ++itPoint)
                {
                    if((*itPoint)->GetSectorBorder())
                    {
                        points << (*itPoint)->GetName();
                    }
                }
            }
        }
    }
    points.removeAll("");
    points.removeDuplicates();
    return points;
}

CCoord CListAC::getCoordsByPointName(const QString &name)
{
    if (master()) {
        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (view) {
            CView::TSceneList::const_iterator it_fir = view->GetScenes().constBegin();
            for (; it_fir != view->GetScenes().constEnd(); ++it_fir) {
                Scene::TPointsMap::const_iterator it_point = (*it_fir)->GetPoints().begin();
                for (; it_point != (*it_fir)->GetPoints().end(); ++it_point) {
                    if ((*it_point)->GetName() == name)
                        return (*it_point)->GetCoord();
                }
            }
        }
    }

    return CCoord();
}

QString CListAC::getPointNameByCoords(const CCoord &coord)
{
    if (master()) {
        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (view) {
            CView::TSceneList::const_iterator it_fir = view->GetScenes().constBegin();
            for (; it_fir != view->GetScenes().constEnd(); ++it_fir) {
                Scene::TPointsMap::const_iterator it_point = (*it_fir)->GetPoints().begin();
                for (; it_point != (*it_fir)->GetPoints().end(); ++it_point) {
                    if ((*it_point)->GetCoord() == coord)
                        return (*it_point)->GetName();
                }
            }
        }
    }

    return QString();
}
