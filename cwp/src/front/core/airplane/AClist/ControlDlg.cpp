#include "ControlDlg.h"

#include "fpldlg.h"
#include "back/core/Airplane.h"
#include "main.h"
#include "back/Master.h"
#include "Scene.h"

#include <QGridLayout>
#include <QListView>
#include <QTreeWidget>
#include <QLineEdit>

CControlDlg::CControlDlg(const Type type, QWidget* parent, Qt::WindowFlags wFlags/* = Qt::Popup*/) :
        QFrame(parent, wFlags),
        m_type(type)
{
	setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
	setWindowOpacity(1);
	setAutoFillBackground(true);
	setFocusPolicy(Qt::NoFocus);
	setAttribute(Qt::WA_TranslucentBackground, false);
	setAttribute(Qt::WA_DeleteOnClose);
}

CControlDlg::~CControlDlg()
{
}

void CControlDlg::Init(QString sVal)
{
	m_sValues.clear();
	QGridLayout* layout = new QGridLayout(this);
	layout->setContentsMargins(0,0,0,0);
	layout->setSpacing(1);
	switch(m_type)
	{
	case FPLType:
        {
            CAirplane* pPlane = (CAirplane*)(m_sValues1.first().toULong());
            CFPLDlg* pWnd = new CFPLDlg(pPlane->GetSFPL(), this);
            pWnd->SetAirplane(pPlane);
            pWnd->buttonActivate->hide();
            pWnd->setFocusPolicy(Qt::StrongFocus);
            QPalette pal(pWnd->palette());
            pal.setBrush(QPalette::Background, QBrush(Qt::lightGray));
            pWnd->setPalette(pal);
            layout->addWidget(pWnd);
            setFocusProxy(pWnd);
        }
        break;
	case CODEType:
        {
            m_pEdit = new QLineEdit(this);
            m_pEdit->setFocusPolicy(Qt::StrongFocus);
            QRegExp regExp("[0-7]{1,4}");
            m_pEdit->setValidator(new QRegExpValidator(regExp, this));
            m_pEdit->setText(sVal);
            m_pEdit->selectAll();
            layout->addWidget(m_pEdit, 0, 0);

            m_pListView = new QListView(this);
            m_pListView->setFocusPolicy(Qt::StrongFocus);
            //QFont listFont = GetXFont(FONT_CTRL_DLG_LIST);
            //if(listFont != QFont()) m_pListView->setFont(listFont);
            m_pListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            m_sModel.setStringList(m_sValues1);
            int index  = m_sValues1.indexOf(sVal);
            QModelIndex curIndex = m_sModel.index(index, 0);
            m_pListView->setModel(&m_sModel);
            m_pListView->setCurrentIndex(curIndex);
            layout->addWidget(m_pListView, 1, 0);

            setTabOrder(m_pEdit, m_pListView);
            setFocusProxy(m_pEdit);

            connect(m_pEdit, SIGNAL( returnPressed() ),
                    this, SLOT( SelectEditItem() ));
            connect(m_pListView, SIGNAL( activated(const QModelIndex&) ),
                    this, SLOT( SelectListItem(const QModelIndex&) ));
            /*Pasha*/connect(m_pListView, SIGNAL( clicked(const QModelIndex&) ),
                             /*OneClickative*/				this, SLOT( SelectListItem(const QModelIndex&) ));
        }
        break;
    case STARType:
        {
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
            if(m_sValues1.size() && master() && view && view->GetMainFIR())
            {
                Scene::TAirportsMap::const_iterator itAirport = view->GetMainFIR()->GetAirports().constFind(m_sValues1.first());
                if(itAirport != view->GetMainFIR()->GetAirports().constEnd() && *itAirport)
                {
                    m_pTreeWidget = new QTreeWidget(this);
                    m_pTreeWidget->setFocusPolicy(Qt::StrongFocus);
                    QPalette pal(m_pTreeWidget->palette());
                    pal.setBrush(QPalette::Base, QBrush(Qt::lightGray));
                    m_pTreeWidget->setPalette(pal);
                    QTreeWidgetItem* pItem = new QTreeWidgetItem();
                    pItem->setText(0, QString::fromUtf8("1"));
                    m_pTreeWidget->setHeaderItem(pItem);
                    m_pTreeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
                    m_pTreeWidget->setProperty("showDropIndicator", QVariant(false));
                    m_pTreeWidget->setDragDropMode(QAbstractItemView::NoDragDrop);
                    m_pTreeWidget->setAllColumnsShowFocus(true);
                    m_pTreeWidget->header()->setVisible(false);

                    QTreeWidgetItem* pSelItem = 0;
                    QTreeWidgetItem* pAirportItem = new QTreeWidgetItem(m_pTreeWidget);
                    pAirportItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
                    pAirportItem->setText(0, (*itAirport)->GetName());
                    pAirportItem->setData(0, Qt::UserRole+1, (*itAirport)->GetID());
                    CAirport::TRunways::const_iterator itRunway = (*itAirport)->GetRunways().constBegin();
                    for(; itRunway != (*itAirport)->GetRunways().constEnd(); ++itRunway)
                    {
                        if((*itRunway)->GetSSA().size())
                        {
                            QTreeWidgetItem* pRunwayItem = new QTreeWidgetItem(pAirportItem);
                            pRunwayItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
                            pRunwayItem->setText(0, (*itRunway)->GetName());
                            pRunwayItem->setData(0, Qt::UserRole, 0);
                            pRunwayItem->setData(0, Qt::UserRole+1, (*itRunway)->GetID());
                            if((*itRunway)->GetName() == sVal)
                                pSelItem = pRunwayItem;

                            CRunway::TSSA::const_iterator itSSA = (*itRunway)->GetSSA().constBegin();
                            for(; itSSA != (*itRunway)->GetSSA().constEnd(); ++itSSA)
                            {
                                if((*itSSA)->GetEnable() && (*itSSA)->GetType() == CSSA::enSTAR)
                                {
                                    QTreeWidgetItem* pSSAItem = new QTreeWidgetItem(pRunwayItem);
                                    pSSAItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
                                    pSSAItem->setText(0, (*itSSA)->GetName());
                                    pSSAItem->setData(0, Qt::UserRole, 1);
                                    pSSAItem->setData(0, Qt::UserRole+1, (*itSSA)->GetID());
                                    if((*itSSA)->GetName() == sVal)
                                        pSelItem = pSSAItem;
                                }
                            }
                        }
                    }
                    m_pTreeWidget->expandItem(pAirportItem);
                    if(pSelItem)
                    {
                        m_pTreeWidget->scrollToItem(pSelItem);
                        m_pTreeWidget->setCurrentItem(pSelItem);
                    }
                    layout->addWidget(m_pTreeWidget, 0, 0);
                    setFocusProxy(m_pTreeWidget);
                    connect(m_pTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(SelectTreeItem(QTreeWidgetItem*,int)));
                    /*Pasha*/connect(m_pTreeWidget, SIGNAL( itemClicked(QTreeWidgetItem*,int) ),
                    /*OneClickative*/this, SLOT( SelectTreeItem(QTreeWidgetItem*,int) ));
                    connect(m_pTreeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(OnResizeColumns(QTreeWidgetItem*)));
                    connect(m_pTreeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(OnResizeColumns(QTreeWidgetItem*)));
                }
            }
        }
        break;
	default: break;
	}

	QPalette pal = palette();
	pal.setBrush(QPalette::Background, QBrush(Qt::lightGray));
	setPalette(pal);

	setLayout(layout);
}

void CControlDlg::SelectListItem(const QModelIndex& item)
{
	QString sValue = item.data().toString();
	m_sValues.clear();
	m_sValues << sValue;
	emit endEdit();
	close();
}

void CControlDlg::SelectTreeItem(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if(item && item->data(0, Qt::UserRole).toInt())
    {
        QString sValue = item->text(0);
        m_sValues.clear();
        m_sValues << sValue;
        emit endEdit();
        close();
    }
}

void CControlDlg::SelectEditItem()
{
	QString sValue = m_pEdit->text();
	if(sValue.toInt() == 0) return;
	m_sValues.clear();
	m_sValues << sValue;
	emit endEdit();
	close();
}

void CControlDlg::OnResizeColumns(QTreeWidgetItem* pItem)
{
    if(pItem && pItem->childCount())
        m_pTreeWidget->resizeColumnToContents(0);
}

void CControlDlg::UpdateXFont()
{

}
