#include "XDocWidget.h"
#include "main.h"
#include <QtGui/QCloseEvent>
#include "front/core/airplane/AClist/ListACC.h"
#include "front/core/airplane/AClist/ListAPP.h"
#include "front/core/airplane/AClist/ListSILACC.h"
#include "front/core/airplane/AClist/ListSILAPPARR.h"
#include "front/core/airplane/AClist/ListSILAPPDEP.h"
#include "front/core/airplane/AClist/ListACTIN.h"
#include "front/core/airplane/AClist/ListACTOUT.h"
#include "front/core/airplane/AClist/ListACTDEP.h"
#include "front/core/airplane/AClist/ListDAILYPLAN.h"
#include "front/core/airplane/AClist/ListCOST.h"
#include "front/core/meteo/MeteoRunwayWidget.h"
#include "front/core/meteo/RunwayWidget.h"
#include "front/core/meteo/MeteoWidget.h"
#include "front/core/restrictions/RestrictionsWidget.h"
#include "back/Master.h"
#include "QtCore/QSettings"
#include "XMasterWindow.h"
#include "front/dialog/vpp.h"
#include "fplbrowser.h"
#include "StandWidget.h"
#include "UserPicsWidget.h"

#include "RunwaySyncManager.h"
#include "controlwindow.h"

Titlebar ::Titlebar( QWidget *parent ) : parent( parent )
{
    QLabel *label = new QLabel( parent->windowTitle() );
    const QString sheet = "padding: 0px; padding-left: 5px;"
                          "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                          "stop:0 rgb(156, 194, 229), stop: 0.45 rgb(150, 190, 230),"
                          "stop: 0.46 rgb(138, 180, 220), stop:1 rgb(140, 182, 221))";

    label->setStyleSheet(sheet);
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addWidget( label );
    layout->setMargin(0);
}
void Titlebar :: mouseDoubleClickEvent(QMouseEvent *event)
{
    return;
}


extern QString currentUserName;

XDocWidget::XDocWidget(DocType type, IDocHandler* pParent, QWidget* parent, Qt::WindowFlags f, const QString &customData) :
    QDockWidget(parent, f),
        m_pParent(pParent), m_Type(type), m_bHover(true), m_bMinimized(false), isFirstShow(true),
        workerThread(this)
{
    Init(pParent);
}

XDocWidget::XDocWidget(DocType type, QString privileges, IDocHandler* pParent, QWidget* parent, Qt::WindowFlags f, const QString &customData) :
    QDockWidget(parent, f),
        m_pParent(pParent), m_Type(type), m_bHover(true), m_bMinimized(false), isFirstShow(true),
        workerThread(this)
{
    m_privileges = privileges;
    Init(pParent);
}

XDocWidget::XDocWidget(DocType type, QStringList _lst, IDocHandler* pParent, QWidget* parent, Qt::WindowFlags f, const QString &customData) :
    QDockWidget(parent, f),
        m_pParent(pParent), m_Type(type), m_bHover(true), m_bMinimized(false), isFirstShow(true),
        workerThread(this)
{
    lst = _lst;
    Init(pParent);
}

void XDocWidget::Init(IDocHandler* pParent)
{
    switch (m_Type)
    {
    case ListACC:
    case RestrictionsAdminEditor:
    case RestrictionsUserEditor:
    case FPLsList:
    case StandList:
    case UserPicsList:
        setFeatures(DockWidgetMovable | DockWidgetFloatable | DockWidgetClosable);
        setAllowedAreas(Qt::NoDockWidgetArea);
        break;
    case Runway:
    default:
        setFeatures(DockWidgetMovable | DockWidgetFloatable);
        setAllowedAreas(Qt::NoDockWidgetArea);
        break;
    }

    switch(m_Type)
    {
    case ListACC:
        setWidget(new CListACC( dynamic_cast<CMaster*>(pParent), this));
        break;
    case ListSIL_ACC:
        setWidget(new CListSIL_ACC( dynamic_cast<CMaster*>(pParent), this));
        break;
    case ListAPP:
        setWidget(new CListAPP( dynamic_cast<CMaster*>(pParent), this));
        break;
    case ListSIL_APP_ARR:
        setWidget(new CListSIL_APP_ARR( dynamic_cast<CMaster*>(pParent), this));
        break;
    case ListSIL_APP_DEP:
        setWidget(new CListSIL_APP_DEP( dynamic_cast<CMaster*>(pParent), this));
        break;
    case MeteoRunwayInfo:
        {
            //setWidget(new MeteoRunwayWidget(this));
            setWidget(new RunwayWidget(RunwayWidget::RW1, this));
            //setWidget(new MeteoWidget(this));
        }
        break;
    case MeteoRunwayInfo2:
        {
            //setWidget(new MeteoRunwayWidget(this));
            setWidget(new RunwayWidget(RunwayWidget::RW2, this));
            //setWidget(new MeteoWidget(this));
        }
        break;
    case RestrictionsAdminEditor:
        {
            RestrictionsWidget *restrictionWidget = new RestrictionsWidget(this);
            restrictionWidget->setUserRole(RestrictionsWidget::Admin);
            setWidget(restrictionWidget);
        }
        break;
    case RestrictionsUserEditor:
        {
            RestrictionsWidget *restrictionWidget = new RestrictionsWidget(this);
            restrictionWidget->setUserRole(RestrictionsWidget::User);
            setWidget(restrictionWidget);
        }
        break;
    case Runway:
    {
        setWidget(new MeteoWidget(m_privileges, this));

        break;

    }
    case FPLsList:
    {
        FplBrowser* fplBrowser = new FplBrowser(this);
        fplBrowser->setCanOtherInfoEmpty(networkSettings()->value("FPL_handler/field_18", false).toBool());
        setWidget(fplBrowser);
        OnChildSizeChanged();
        break;
    }
    case StandList:
        setWidget(new StandWidget(this));
        break;
    case UserPicsList:
        setWidget(new UserPicsWidget(this));
        break;
    case ListActIn:
        setWidget(new CListActIn(dynamic_cast<CMaster*>(pParent), this));
        break;
    case ListActOut:
        setWidget(new CListActOut(dynamic_cast<CMaster*>(pParent), this));
        break;
    case ListActDep:
        setWidget(new CListActDep(lst, dynamic_cast<CMaster*>(pParent), this));
        break;
    case ListDailyPlan:
        setWidget(new CListDailyPlan(lst, dynamic_cast<CMaster*>(pParent), this));
        break;
    case ListCost:
        setWidget(new CListCost(dynamic_cast<CMaster*>(pParent), this));
        break;
    case CtrlWindow:
        setWidget(new ControlWindow());
        break;
    default:
        break;
    }

    CalcVisibleRect();
    setAutoFillBackground(false);
    widget()->setAutoFillBackground(false);

    Translate();

    if(dynamic_cast<CListAC*>(widget()))
    {
        OnChildSizeChanged();
        connect(dynamic_cast<CListAC*>(widget()), SIGNAL(sizeChanged()), this, SLOT(OnChildSizeChanged()));
    }
    if(qobject_cast<MeteoRunwayWidget*>(widget()))
    {
        OnChildSizeChanged();
        connect(qobject_cast<MeteoRunwayWidget*>(widget()), SIGNAL(sizeChanged()), this, SLOT(OnChildSizeChanged()));
    }
    if(qobject_cast<RunwayWidget*>(widget()))
    {
        OnChildSizeChanged();
        connect(qobject_cast<RunwayWidget*>(widget()), SIGNAL(sizeChanged()), this, SLOT(OnChildSizeChanged()));
        connect(qobject_cast<RunwayWidget*>(widget()), SIGNAL(UpdateMeteoToMaster()), this, SLOT(OnUpdateMeteoToMaster()));
    }
    if(qobject_cast<RestrictionsWidget*>(widget()))
    {
        OnChildSizeChanged();
        connect(qobject_cast<RestrictionsWidget*>(widget()), SIGNAL(sizeChanged()), this, SLOT(OnChildSizeChanged()));
    }
    if(qobject_cast<StandWidget*>(widget()))
    {
        OnChildSizeChanged();
        connect(qobject_cast<StandWidget*>(widget()), SIGNAL(sizeChanged()), this, SLOT(OnChildSizeChanged()));
    }
    if(qobject_cast<UserPicsWidget*>(widget()))
    {
        OnChildSizeChanged();
        connect(qobject_cast<UserPicsWidget*>(widget()), SIGNAL(sizeChanged()), this, SLOT(OnChildSizeChanged()), Qt::QueuedConnection);
        connect(qobject_cast<UserPicsWidget*>(widget()), SIGNAL(getPickCoord(bool)), SLOT(OnGetPickCoord(bool)));
    }

    connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(OnTopLevelChanged(bool)));
    if(qobject_cast<MeteoWidget*>(widget()))
    {
        OnChildSizeChanged();
        connect(qobject_cast<UserPicsWidget*>(widget()), SIGNAL(sizeChanged()), this, SLOT(OnChildSizeChanged()), Qt::QueuedConnection);
        connect(this, SIGNAL(UpdateMeteo()), qobject_cast<MeteoWidget*>(widget()), SLOT(onUpdateMeteo()));
    }
}

XDocWidget::~XDocWidget()
{
    workerThread.exit();

    QSettings &settings = *globalSettings();
    settings.beginGroup(currentUserName);
    settings.setValue(objectName(), saveGeometry().toHex());
    settings.endGroup();

    workerThread.wait();
}

void XDocWidget::Translate()
{
    switch(m_Type)
    {
    case ListACC:
        setWindowTitle(tr("List ACC"));
        break;
    case ListSIL_ACC:
        setWindowTitle(tr("List SIL ACC"));
        break;
    case ListAPP:
        setWindowTitle(tr("List APP"));
        break;
    case ListSIL_APP_ARR:
        setWindowTitle(tr("List SIL APP ARR"));
        break;
    case ListSIL_APP_DEP:
        setWindowTitle(tr("List SIL APP DEP"));
        break;
    case MeteoRunwayInfo:
        //setWindowTitle(tr("Meteo information"));
        //setWindowTitle(tr("Runway 1"));
        setWindowTitle(networkSettings()->value("RW1/name").toString());
        setTitleBarWidget(new Titlebar(this));
        break;
    case MeteoRunwayInfo2:
        setWindowTitle(networkSettings()->value("RW2/name").toString());
        setTitleBarWidget(new Titlebar(this));
        break;
    case RestrictionsAdminEditor:
    case RestrictionsUserEditor:
        setWindowTitle(tr("User restriction zones Editor"));
        break;
    case Runway:
        setWindowTitle(tr("Meteo"));
        setTitleBarWidget(new Titlebar(this));
        break;
    case FPLsList:
        setWindowTitle(tr("FPLs list"));
        break;
    case StandList:
        setWindowTitle(tr("Stands list"));
        break;
    case UserPicsList:
        setWindowTitle(tr("User pics list"));
        break;
    case ListActIn:
        setWindowTitle(tr("ACT IN"));
        setTitleBarWidget(new Titlebar(this));
        break;
    case ListActOut:
        setWindowTitle(tr("ACT OUT"));
        setTitleBarWidget(new Titlebar(this));
        break;
    case ListActDep:
        setWindowTitle(tr("ACT DEPARTURE"));
        setTitleBarWidget(new Titlebar(this));
        break;
    case ListDailyPlan:
        setWindowTitle(tr("DAILY PLAN"));
        setTitleBarWidget(new Titlebar(this));
        break;
    case ListCost:
        setWindowTitle(tr("COST"));
        setTitleBarWidget(new Titlebar(this));
        break;
    case CtrlWindow:
        setWindowTitle(tr("Control Window"));
        setTitleBarWidget(new Titlebar(this));
        break;
    default:
        break;
    }
}

void XDocWidget::enterEvent(QEvent *event)
{
    QDockWidget::enterEvent(event);
    m_bHover = true;
    if(isFloating()) CalcVisibleRect();
}

void XDocWidget::leaveEvent(QEvent *event)
{
    QDockWidget::leaveEvent(event);
    m_bHover = false;
    if(isFloating()) CalcVisibleRect();
}

void XDocWidget::showEvent(QShowEvent* event)
{
    QDockWidget::showEvent(event);
    if (isFirstShow)
    {
        QSettings &settings = *globalSettings();
        settings.beginGroup(currentUserName);
        restoreGeometry(QByteArray::fromHex(settings.value(objectName()).toByteArray()));
        isFirstShow = false;
        settings.endGroup();
    }
}

void XDocWidget::closeEvent(QCloseEvent *event)
{
    XMasterWindow* pMain = dynamic_cast<XMasterWindow*>(getMainWindow());
    if(pMain && !pMain->IsShutdown())
        event->ignore();
    else
        event->accept();

    // Special bahavior for restriction widget - see RestrictionsWidget::closeEvent.
    // Restriction widget - is closable widget.
    if ( qobject_cast<RestrictionsWidget*>(widget()) )
    {
        event->accept();
        qobject_cast<RestrictionsWidget*>(widget())->close();
        return;
    }

    if ( m_Type == FPLsList || m_Type == StandList || m_Type == UserPicsList )
    {
        event->accept();
        m_pParent->CloseDoc(m_Type);
        return;
    }
    if(m_Type == MeteoRunwayInfo)
    {
        event->ignore();
        return;
    }
    if(m_Type == MeteoRunwayInfo2)
    {
        event->ignore();
        return;
    }
    m_bMinimized = !m_bMinimized;
    if(m_bMinimized)
    {
        setFloating(true);
        QSize size(70,1);
#ifdef Q_OS_LINUX
        size = QSize(140, 29);
#endif
        setMinimumSize(size);
        setMaximumSize(size);
        QPoint point(10, 100);
        QWidget* pParent(parentWidget());
        if (pParent)
        {
            QRect rect(pParent->frameGeometry());
            point = QPoint(rect.left()+20, rect.bottom()-90);
#ifdef Q_OS_LINUX
            point = QPoint(rect.left()+20, rect.bottom()-120);
#endif
        }
        move(point);
        m_bMinimized = true;
    }
    else
    {
        setFloating(true);
        QPoint point(300, 300);
        QWidget* pParent(parentWidget());
        if (pParent)
        {
            QRect rect(pParent->frameGeometry());
            QSize size(pParent->frameSize());
            point = QPoint(rect.left()+size.width()/4, rect.top()+size.height()/4);
        }
        move(point);
        if(dynamic_cast<CListAC*>(widget()))
        {
            dynamic_cast<CListAC*>(widget())->SizeToContents(true);
            OnChildSizeChanged();
            dynamic_cast<CListAC*>(widget())->SizeToContents(true);
        }
        /*if(m_Type == Runway)
        {
            setMinimumSize(mRunwaySize);
            setMaximumSize(mRunwaySize);
        }*/
    }
}

void XDocWidget::resizeEvent(QResizeEvent *event)
{
    if(m_bMinimized) m_bMinimized = false;
    QDockWidget::resizeEvent(event);
}

void XDocWidget::CalcVisibleRect()
{
#ifndef _MSC_VER
    setWindowOpacity(0.65);
#else
    setWindowOpacity(1.0);
#endif // _MSC_VER
}

void XDocWidget::OnChildSizeChanged()
{
    if(isFloating() && !m_bMinimized)
    {
         if(dynamic_cast<CListAC*>(widget()))
         {
             int Height = networkSettings()->value("users/SizeListAC/Height").toInt();
             if (!Height)
                 Height = DEFAULT_MAX_TABLE_HEIGHT;
             QSize size(dynamic_cast<CListAC*>(widget())->GetVisibleSize());

             if(size.width() < 100) size.setWidth(100);
             if(size.height() < 35) size.setHeight(35);
             if(size.height() > Height)
             {
                 int scrollWidth = qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
                 if (qApp->style()->styleHint(QStyle::SH_ScrollView_FrameOnlyAroundContents))
                 {
                     scrollWidth += qApp->style()->pixelMetric(QStyle::PM_ScrollView_ScrollBarSpacing);
                 }

                 size.rwidth() += scrollWidth;
                 size.setHeight(Height);
                 setMinimumSize(size);
                 setMaximumSize(size);
                 adjustSize();
                 updateGeometry();
                 dynamic_cast<CListAC*>(widget())->SizeToContents(false);
                 size = dynamic_cast<CListAC*>(widget())->GetVisibleSize();
                 if(size.width() < 100) size.setWidth(100);
                 if(size.height() < 35) size.setHeight(35);
                 if(size.height() > Height)
                 {
                     size.rwidth() += scrollWidth;
                     size.setHeight(Height);
                 }
             }

             setMinimumSize(size);
             setMaximumSize(size);
         }

        if(qobject_cast<MeteoRunwayWidget*>(widget()))
        {
            MeteoRunwayWidget *widgetMR = qobject_cast<MeteoRunwayWidget*>(widget());

            widgetMR->SizeToContents(false);
            QSize size = widgetMR->GetVisibleSize();
            setMinimumSize(size);
            setMaximumSize(size);
        }

        if(qobject_cast<MeteoWidget*>(widget()))
        {
            MeteoWidget *widgetMR = qobject_cast<MeteoWidget*>(widget());

            widgetMR->SizeToContents(true);
            QSize size = widgetMR->GetVisibleSize();
            setMinimumSize(size);
            setMaximumSize(size);
        }



        if(qobject_cast<Vpp*>(widget()) || m_Type == Runway)
        {
//            setMinimumSize(size());
//            setMaximumSize(size());

//            Vpp *widgetVpp = qobject_cast<Vpp*>(widget());
//            widgetVpp->adjustSize();
//            QSize size = widgetVpp->rect().size();
//#ifdef Q_OS_LINUX
//            size.setWidth(size.width()+8);
//            size.setHeight(size.height()+42);
//#endif
//            setMinimumSize(size);
//            setMaximumSize(size);
        }

        if(qobject_cast<FplBrowser*>(widget()))
        {
            FplBrowser *widgetFPL = qobject_cast<FplBrowser*>(widget());
            widgetFPL->adjustSize();
            QSize size = widgetFPL->rect().size();
#ifdef Q_OS_LINUX
            size.setWidth(size.width()+8);
            size.setHeight(size.height()+42);
#endif
            setMinimumSize(size);
            setMaximumSize(size);
        }

        if(qobject_cast<StandWidget*>(widget()))
        {
            StandWidget *widgetStand = qobject_cast<StandWidget*>(widget());
            widgetStand->SizeToContents(false);
            QSize size = widgetStand->GetVisibleSize();
            if(size.height() > 500)
            {
                size.rwidth() += 17;
                size.setHeight(500);
                setMinimumSize(size);
                setMaximumSize(size);
                adjustSize();
                updateGeometry();
                widgetStand->SizeToContents(false);
                size = widgetStand->GetVisibleSize();
                if(size.height() > 500)
                {
                    size.rwidth() += 17;
                    size.setHeight(500);
                }
            }
            setMinimumSize(size);
            setMaximumSize(size);
        }

        if(qobject_cast<UserPicsWidget*>(widget()))
        {
            UserPicsWidget *widgetUserPics = qobject_cast<UserPicsWidget*>(widget());
            widgetUserPics->SizeToContents(false);
            widgetUserPics->SizeToContents(false);
            QSize size = widgetUserPics->GetVisibleSize();
            if(size.height() > 500)
            {
                size.rwidth() += 17;
                size.setHeight(500);
                setMinimumSize(size);
                setMaximumSize(size);
                adjustSize();
                updateGeometry();
                widgetUserPics->SizeToContents(false);
                size = widgetUserPics->GetVisibleSize();
                if(size.height() > 500)
                {
                    size.rwidth() += 17;
                    size.setHeight(500);
                }
            }
            setMinimumSize(size);
            setMaximumSize(size);
        }

        if(qobject_cast<RestrictionsWidget*>(widget()))
        {
            RestrictionsWidget *widgetRW = qobject_cast<RestrictionsWidget*>(widget());

            widgetRW->SizeToContents(false);
            QSize size = widgetRW->GetVisibleSize();
            setMinimumSize(size);
            setMaximumSize(size);
        }
    }
    adjustSize();
    updateGeometry();
}

void XDocWidget::OnTopLevelChanged(bool topLevel)
{
    if(topLevel)
    {
        OnChildSizeChanged();
    }
    else
    {
        setMinimumSize(QSize(0,0));
        setMaximumSize(QSize(16777215,16777215));
    }
}

void XDocWidget::OnRunwayMinimizeToggle()
{
    QCloseEvent event;
    closeEvent(&event);
}

bool XDocWidget::event(QEvent *event)
{
    if(event->type() == QEvent::NonClientAreaMouseButtonDblClick)
        return false;
    return QDockWidget::event(event);
}

void XDocWidget::Refresh()
{
    if ( qobject_cast<RestrictionsWidget*>(widget()) )
        qobject_cast<RestrictionsWidget*>(widget())->Refresh();
    else if ( qobject_cast<MeteoRunwayWidget*>(widget()) )
        qobject_cast<MeteoRunwayWidget*>(widget())->Refresh();
}

void XDocWidget::OnGetPickCoord(bool bMulti)
{
    mPickCoordID = QUuid::createUuid();
    emit getPickCoord(bMulti, mPickCoordID);
}

void XDocWidget::OnPickCoord(QUuid id)
{
    if ( id == mPickCoordID )
    {
        if ( qobject_cast<UserPicsWidget*>(widget()) )
            qobject_cast<UserPicsWidget*>(widget())->onPickCoord();
    }
}

void XDocWidget::OnUpdateMeteoToMaster()
{
    emit UpdateMeteoToMaster();
}


