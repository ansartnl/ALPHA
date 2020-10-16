#include "XMasterWindow.h"

#include "LoadFromDB.h"
#include "LayoutDlg.h"
#include "ActiveSectors.h"
#include "core/reminder/XReminder.h"
#include "core/GUIScene.h"
#include "core/airplane/XLabelConstructor.h"
#include "Password.h"
#include "UserAdmin.h"
#include "Sectoring.h"
#include "core/RouteFragmentation.h"
#include "utils/ProfileSettings.h"
#include "DocWidgetMoveHandler.h"
#include "Airplane.h"
#include "core/airplane/AClist/ListAC.h"
#include "MouseLocker.h"

#include <algorithm>

#include "controlwindow.h"
#include "pdfwindow.h"
#include "restrictionwidget.h"

#include "XAbout.h"

#include "main.h"
#include "CmdLineArguments.h"

#include "qtlanguagemenu.h"

#include <QFontDialog>
#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QActionGroup>
#include <QToolTip>
#include <QWidgetAction>
#include <QSizeGrip>

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QCloseEvent>
#include <QShortcut>
#include <QClipboard>
#include <QGraphicsScene>
#include "qxtspanslider.h"

#include "OLDIMessages.h"
#include "NodesViewer.h"

#include "CustomSector.h"

#include "restrictions/RestrictionsWidget.h"
#include "import/MeteoRunway.h"

#include "PulsReceiver.h"
#include "pulsar_fdp_settings.h"

#include "StdinReader.h"
#include "front/dialog/RcGridCoordDlg.h"
#include "front/dialog/BearingDlg.h"
#include "front/dialog/BearingEditor.h"
#include "front/dialog/ORCAMDlg.h"
#include "front/dialog/UserPicsWidget.h"

#include "obelix/CmdSectorChanged.h"
#include "obelix/CmdSectoringAction.h"
#include "obelix/CmdRequestAllFPL.h"
#include "obelix/CmdGUIRefresh.h"

#include "ListConstructor.h"
#include "EditHeaderDialog.h"
#include "GUIListConstructor.h"

#include "ListACC.h"
#include "ListAPP.h"
#include "ListSILACC.h"
#include "ListSILAPPARR.h"
#include "ListSILAPPDEP.h"

#include "StandWidget.h"

#include "dialog/MultiLineInputDialog.h"
#include "front/dialog/FMpassDlg.h"

#include "GUIRestriction.h"

#include "AntioverlapThread.h"
#include "StepLeftRightStrategy.h"
#include "StepLengthAngleStrategy.h"
#include "front/core/meteo/RunwayWidget.h"
#include "XDocWidget.h"

QString currentUserName;

XMasterWindow::XMasterWindow(bool registered) :
    m_AdminMenu(NULL), m_UserAction(NULL), m_ImportAction(NULL), m_SectoringAction(NULL), m_BearingAction(NULL), //m_FileMenu(NULL),
    m_LoadAction(NULL), m_AddAction(NULL), m_ChUserAction(NULL), m_ActRefreshSFPL(0), m_ExitAction(NULL),
    m_pRunwayWidgetToggle(NULL), mOptionMenu(NULL),
    m_Font(NULL), /*m_ToolsMenu(NULL),*/ /*m_FormularEditor(NULL),*/ m_RestrictionsEditor(NULL), m_BearingEditor(NULL),
    m_FPLsEditor(NULL), m_StandEditor(NULL), m_UserPicsEditor(0), m_PickCoords(NULL), m_PlanListsEditor(0), m_FMvalues(0),
    m_HelpMenu(NULL), m_HelpAbout(NULL), m_pSectoringCombo(0), m_SICombo_M(0), m_SICombo_K(0), m_pHistoryPointMenu(0),
    m_pEchelonBut(0), m_EchelonCombo(0), m_PressSpin(0), m_FLSlider(0),
    docHandler(new DocWidgetMoveHandler(this)), m_SavedZoom(0), m_bFullScreenMode(true), m_bShutdown(false), m_bStandTape(false),
    control_window_(0),
    restrictions_widget_(new RestrictionWidget(this)), pref_set_menu_(new QMenu(this)),
    m_pLicenseLabel(0), active_db_(0), active_redis_(0), help_action_(0), connected_to_db_(false), scene_fir(0), dlgLayout(0), mRegistered(registered),
    connectPostgresTrue(false), connectRedisTrue(false)
{
    qRegisterMetaType<QUuid>("QUuid");

    master(this);
    sCurrentSector = tr("");
    qApp->setStyleSheet("QSplitter::handle { background-color: gray }");

    QSettings &settings = *globalSettings();

    QVariant var = settings.value("XMasterWindow/Settings/BlockAltTab", 1);
    settings.setValue("XMasterWindow/Settings/BlockAltTab", var);
    Qt::WindowFlags flags = windowFlags();
    if (var.toInt())
        flags |= Qt::WindowStaysOnTopHint;
    setWindowFlags(flags);
    setAttribute(Qt::WA_DeleteOnClose);

    var = settings.value("XMasterWindow/Settings/FullScreenMode", 1);
    settings.setValue("XMasterWindow/Settings/FullScreenMode", var);
    m_bFullScreenMode = 0 != var.toInt();

    var = settings.value("XMasterWindow/Settings/Common");
    if (!var.isNull() && var.isValid())
    {
        QDataStream ds(var.toByteArray());
        ds >> static_cast<AppSettings&> (*this);
    }

    var = settings.value("XMasterWindow/Settings/PressureValues");
    if (!var.isNull() && var.isValid())
    {
        QDataStream ds(var.toByteArray());
        ds >> m_mapPressure;
    }
    var = settings.value("XMasterWindow/Settings/EchelonValues");
    if (!var.isNull() && var.isValid())
    {
        QDataStream ds(var.toByteArray());
        ds >> m_mapEchelon;
    }

    m_SavedZoom = settings.value("XMasterWindow/Settings/FocusSavedZoom").toInt();
    var = settings.value("XMasterWindow/Settings/FocusSavedPoint");
    if (!var.isNull() && var.isValid())
    {
        QDataStream ds(var.toByteArray());
        ds >> m_SavedPoint;
    }

    var = settings.value("XMasterWindow/Settings/UserPicsVisability");
    if (!var.isNull() && var.isValid())
    {
        QDataStream ds(var.toByteArray());
        UserPicsWidget::Load(ds);
    }

    m_pReminderDoc = 0;
    //    m_pReminderDoc = new QDockWidget(tr("Reminder"), this);
    //    m_pReminderDoc->setAllowedAreas(Qt::BottomDockWidgetArea);
    //    m_pReminderDoc->setFeatures(QDockWidget::DockWidgetClosable);
    //    XReminder* pXReminder = new XReminder(m_pReminderDoc);
    //    m_pReminderDoc->setWidget(pXReminder);
    //    addDockWidget(Qt::BottomDockWidgetArea, m_pReminderDoc);
    //    pXReminder->SetPlane(QSharedPointer<CAirplane> ());
    //    m_pReminderDoc->close();

    //Pasha temp
    var = settings.value("XMasterWindow/Settings/ShowFormularBorder", 3);
    settings.setValue("XMasterWindow/Settings/ShowFormularBorder", var);

    var = settings.value("XMasterWindow/Settings/RouteDashDotLine", 0);
    settings.setValue("XMasterWindow/Settings/RouteDashDotLine", var);

    CAirplane::aproximationInterval = networkSettings()->value("airplane/aproximat_interval").toInt();
    CAirplane::aproximationFatalInterval = networkSettings()->value("airplane/aproximat_finterval").toInt();
    CAirplane::aproximationReminder = networkSettings()->value("airplane/aproximat_start", "4").toInt();

    networkSettings()->setValue("users/current_sector_type", "");

    //mORCAMDialog = new ORCAMDlg(networkSettings()->value("users/ORCAMMinCode", "3000").toString(),
    //                            networkSettings()->value("users/ORCAMMaxCode", "3500").toString(),
    //                            this);

    //mORCAMDialog->setExpireTimeout(networkSettings()->value("users/ORCAMTimeout", 10).toInt());

    var = networkSettings()->value("hotkeys/ORCAMDialog", "Ctrl+S");
    new QShortcut(QKeySequence(var.toString()), this, SLOT(onORCAMDialog()));
    ////////////////////////

    CTextSize::TSizeMap textsize;
    textsize.insert(CTextSize::Small, networkSettings()->value("users/TextSize/Small", "8").toInt());
    textsize.insert(CTextSize::Medium, networkSettings()->value("users/TextSize/Medium", "10").toInt());
    textsize.insert(CTextSize::Large, networkSettings()->value("users/TextSize/Large", "12").toInt());
    GetTextSize().SetSizes(textsize);
    GetTextSize().SetCurrentSize(CTextSize::XFontSize(networkSettings()->value("users/TextSize/CurTextSize", 0).toInt()));
    QFont font = qApp->font();
    font.setPointSize(GetTextSize().GetCurrentTextSize());
    qApp->setFont(font);

    //Pulsar
    puls_receiver_ = new PulsReceiver(AFTNPulsarDefaultPort, this);
    connect(puls_receiver_, SIGNAL(pulsarChanged(QHostAddress, QString, QHostAddress, QVariantHash))
            , SLOT(OnPulsarChanged(QHostAddress,QString,QHostAddress,QVariantHash)));
//    puls_receiver_->start();  NOTE: not using fdp

//    StdinReader* stdIn = new StdinReader(this);
//    connect(stdIn, SIGNAL(received(QString)), SLOT(OnSuperStop(QString)));
//    stdIn->start();

    connect(qApp, SIGNAL(commitDataRequest(QSessionManager&)), SLOT(OnShutdown(QSessionManager&)), Qt::DirectConnection);
    resize(700, 500);

    connect(restrictions_widget_, SIGNAL(createRestriction(const QSharedPointer<CRestrictionUser> &)), SLOT(onCreateRestriction(const QSharedPointer<CRestrictionUser> &)));
    connect(restrictions_widget_, SIGNAL(updateRestriction(const QSharedPointer<CRestrictionUser> &)), SLOT(onUpdateRestriction(const QSharedPointer<CRestrictionUser> &)));
    connect(restrictions_widget_, SIGNAL(deleteRestriction(const QSharedPointer<CRestrictionUser> &)), SLOT(onDeleteRestriction(const QSharedPointer<CRestrictionUser> &)));

    double dpmm = ((logicalDpiX() + logicalDpiY()) / 2.0) / 25.4;
    mFormularLinkMinLength = networkSettings()->value("airplane/formular_link/minimum_length", 20).toInt() * dpmm;
    mFormularLinkMaxLength = networkSettings()->value("airplane/formular_link/maximum_length", 80).toInt() * dpmm;
    mFormularLinkPreferredLength = networkSettings()->value("airplane/formular_link/preferred_length", 30).toInt() * dpmm;
    mFormularLinkStep = networkSettings()->value("airplane/formular_link/step", 10).toInt() * dpmm;
    initAntiOverlapThread();

    restoreGeometry(globalSettings()->value("XMasterWindow/Settings/MainWindowGeometry").toByteArray());

    networkSettings()->setValue("users/hide_plains", false);
    QString airportsName = networkSettings()->value("meteo/metar_airport").toString();
    if ( airportsName.isNull() )
        networkSettings()->setValue("meteo/metar_airport","UMMG");
    QString lifetime = networkSettings()->value("meteo/metar_lifetime").toString();
    if ( lifetime.isNull() )
        networkSettings()->setValue("meteo/metar_lifetime","30 min");
}

XMasterWindow::~XMasterWindow()
{
    labels_anti_overlap_thread_->stop();
    if (!labels_anti_overlap_thread_->wait(2000))
        labels_anti_overlap_thread_->terminate();

    puls_receiver_->stop();

    m_ZonesView.clear();
    {
        QByteArray arr;
        QDataStream ds(&arr, QIODevice::WriteOnly);
        ds << static_cast<AppSettings&> (*this);
        globalSettings()->setValue("XMasterWindow/Settings/Common", arr);
    }
    {
        // Pressure values
        QByteArray arr;
        QDataStream ds(&arr, QIODevice::WriteOnly);
        ds << m_mapPressure;
        globalSettings()->setValue("XMasterWindow/Settings/PressureValues", arr);
    }
    {
        // Echelon values
        QByteArray arr;
        QDataStream ds(&arr, QIODevice::WriteOnly);
        ds << m_mapEchelon;
        globalSettings()->setValue("XMasterWindow/Settings/EchelonValues", arr);
    }
    {
        // focus
        QByteArray arr;
        QDataStream ds(&arr, QIODevice::WriteOnly);
        ds << m_SavedPoint;
        globalSettings()->setValue("XMasterWindow/Settings/FocusSavedPoint", arr);
        globalSettings()->setValue("XMasterWindow/Settings/FocusSavedZoom", m_SavedZoom);
    }
    {
        // User pics values
        QByteArray arr;
        QDataStream ds(&arr, QIODevice::WriteOnly);
        UserPicsWidget::Save(ds);
        globalSettings()->setValue("XMasterWindow/Settings/UserPicsVisability", arr);
    }
    LogUserLogon(false, currentUserName);

    networkSettings()->setValue("users/TextSize/CurTextSize", int(GetTextSize().GetCurrentSize()));
    networkSettings()->sync();

    if (!isFullScreen())
        globalSettings()->setValue("XMasterWindow/Settings/MainWindowGeometry", saveGeometry());
    QMap<XDocWidget::DocType, QSharedPointer<QDockWidget> >::const_iterator it = m_mapDoc.find(XDocWidget::CtrlWindow);
        if (it != m_mapDoc.end())
    if (it != m_mapDoc.end())
        globalSettings()->setValue("XMasterWindow/Settings/ControlWindowGeometry", (*it)->saveGeometry());
}

void XMasterWindow::changeEvent(QEvent *event)
{
    //LanguageChange events should not be confused with LocaleChange events.
    if (event->type() == QEvent::LocaleChange)
    {
    }
    else if (event->type() == QEvent::LanguageChange)
    {
        AppSettings::UpdateTranslate();
    }

    QMainWindow::changeEvent(event);
}

void XMasterWindow::Translate()
{
    setWindowTitle(tr("XMaster"));
    // Toolbar
    m_MainBar->setWindowTitle(tr("Cartography"));
    m_pPressureBar->setWindowTitle(tr("Pressure/Level"));
    m_SectoringBar->setWindowTitle(tr("Main"));
    airplaneLengthBar->setWindowTitle(tr("Airplane length"));
    m_pCustomSectorBar->setWindowTitle(tr("Custom sector"));
    m_pAltitudeFilterBar->setWindowTitle(tr("Altitude filter"));
    m_pRestrictionBar->setWindowTitle(tr("Restriction editor"));
    m_pRestrictionAction->setText(tr("Edit user restriction zones"));
    m_pLogOutAction->setText(tr("Logout"));
    m_pViewCWP->setText(tr("CWP"));
    m_pCustomSector->setText(tr("Sector"));
    m_CheckGauge->setText(tr("Gauge"));
    //m_AutoCollapse->setText(tr("Auto collapse"));
    m_CorridorRoute->setText(tr("Show route corridor"));
    m_PointZoom->setText(tr("Zoom from center"));
    m_LimitLength->setText(tr("Limit Leader line"));
    m_FlightLevelMonitoring->setText(tr("Flight level monitoring"));
    //m_ShowSectorBorder->setText(tr("Show sector border"));
    //m_ShowRouteOnAssume->setText(tr("Show route on assume"));
    //m_LayoutsAction->setText(tr("Layers"));
    m_LayoutsMainFIRAction->setText(tr("Layers"));
    //m_RcGridCoord->setText(tr("RcGrid center"));
    //m_DF->setText(tr("DF", "direction finders"));
    m_pFontAAMenu->setTitle(tr("Label Font"));
    m_pFontAAsmall->setText(tr("small"));
    m_pFontAAmedium->setText(tr("medium"));
    m_pFontAAlarge->setText(tr("big"));
    if ( m_pHistoryPointMenu )
        m_pHistoryPointMenu->setTitle(tr("Trek history (points)"));
    m_SICombo_M->setItemText(0, tr("meters"));
    m_SICombo_M->setItemText(1, tr("feet"));
    m_SICombo_K->setItemText(0, tr("km/h"));
    m_SICombo_K->setItemText(1, tr("knots"));
    m_pEchelonLabel->setText(tr("TL"));
    //m_pPressLabel->setText(tr("QFE"));
    m_pPressLabel->setText(tr("QNH(hPa)"));
    m_pSliderLabel->setText(tr("Filter"));
    m_SettingMenu->setText(tr("Settings"));
    m_pFocus->setText(tr("Focus"));
    m_pFocusSave->setText(tr("Save"));
    m_pFocusRestore->setText(tr("Restore"));
    //m_pFormularRestore->setText(tr("Restore formular"));
    airplaneLengthBar->setWindowTitle(tr("Airplane length"));
    airplaneLengthSec->setText(tr("Minutes"));
    airplaneLengthKilometers->setText(tr("Kilometers"));
    m_pGroundBar->setWindowTitle(tr("Ground bar"));
    m_pGroundStandAction->setText(tr("Stands"));
    m_pGroundRestrictionAction->setText(tr("ILS critical zone"));
    _SelectSectorModel();

    // Menu file
    //if (m_FileMenu)
        //m_FileMenu->setTitle(tr("File"));
    if (m_LoadAction)
    {
        m_LoadAction->setText(tr("Load"));
        m_LoadAction->setStatusTip(tr("Load map from DB"));
    }
    if (m_AddAction)
    {
        m_AddAction->setText(tr("Add"));
        m_AddAction->setStatusTip(tr("Add map from DB"));
    }
    if (m_ChUserAction)
    {
        m_ChUserAction->setText(tr("Logout"));
        m_ChUserAction->setStatusTip(tr("Current user logout"));
    }
    if (m_ActRefreshSFPL)
    {
        m_ActRefreshSFPL->setText(tr("Refresh"));
        m_ActRefreshSFPL->setStatusTip(tr("Refresh SFPLs"));
    }
    if (m_ExitAction) {
        m_ExitAction->setText(tr("E&xit"));
        m_ExitAction->setStatusTip(tr("Exit the application"));
    }
    // Menu operations
    if (m_AdminMenu)
        m_AdminMenu->setTitle(tr("Operations"));
    if (m_UserAction)
    {
        m_UserAction->setText(CRoleModel::tr(CRoleModel::MENUUSER_NAME));
        m_UserAction->setToolTip(CRoleModel::tr(CRoleModel::MENUUSER_DESCR));
    }
    if (m_SectoringAction)
    {
        m_SectoringAction->setText(tr("Sectoring"));
        m_SectoringAction->setToolTip(tr("Build custom sector groups"));

        m_MapNoteMenuAction->setText(tr("Map Notes"));
    }
    if (m_BearingAction)
    {
        m_BearingAction->setText(tr("Bearing"));
    }
    // Menu option
    if (mOptionMenu)
        mOptionMenu->setTitle(tr("Option"));

    if (m_Font)
        m_Font->setText(tr("Font"));
    // Menu tools
    //if (m_ToolsMenu)
        //m_ToolsMenu->setTitle(tr("Tools"));
    //if (m_FormularEditor)
        //m_FormularEditor->setText(tr("Formularies Editor"));
    if (m_PickCoords)
        m_PickCoords->setText(tr("Pick Coordinates"));
    if (m_RestrictionsEditor)
        m_RestrictionsEditor->setText(tr("Restrictions Editor"));
    if (m_BearingEditor)
        m_BearingEditor->setText(tr("Bearing Editor"));
    if (m_FPLsEditor)
        m_FPLsEditor->setText(tr("FPLs Editor"));
    if (m_StandEditor)
        m_StandEditor->setText(tr("Stand Editor"));
    if (m_UserPicsEditor)
        m_UserPicsEditor->setText(tr("User pics"));
    if (m_PlanListsEditor)
        m_PlanListsEditor->setText(tr("Plan Lists Editor"));
    if (m_FMvalues)
        m_FMvalues->setText(tr("FM values"));

    if (mColorMenu)
    {
        mColorMenu->setTitle(tr("Color"));
        m_BackgroundColorAction->setText(tr("Background ..."));
        if(mFormularMenu)
        {
            mFormularMenu->setTitle(tr("Formular ..."));
            m_FormularActiveColorAction->setText(tr("Formular selected..."));
            m_FormularPassiveColorAction->setText(tr("Formular ..."));
            m_ControlledAction->setText(tr("Controlled Foreground ..."));
            m_ControlledBackAction->setText(tr("Controlled Background ..."));
            m_InAction->setText(tr("In Foreground ..."));
            m_InBackAction->setText(tr("In Background ..."));
            m_RelatedAction->setText(tr("Related Foreground ..."));
            m_RelatedBackAction->setText(tr("Related Background ..."));
            m_UnrelatedAction->setText(tr("Unrelated Foreground ..."));
            m_UnrelatedBackAction->setText(tr("Unrelated Background ..."));
            m_ExtendedAction->setText(tr("Extended Foreground ..."));
        }
        m_MapColorAction->setText(tr("Map ..."));
        m_FIRColorAction->setText(tr("FIR ..."));
        m_ActiveSectorColorAction->setText(tr("Sector ..."));
        m_BorderColorAction->setText(tr("Border ..."));
        m_LegendColorAction->setText(tr("Legends ..."));
        m_ApproachColorAction->setText(tr("STAR ..."));
        m_BlastoffColorAction->setText(tr("SID ..."));
        m_RouteColorAction->setText(tr("Route ..."));
        m_HoldingColorAction->setText(tr("Holding ..."));
        m_CorridorAction->setText(tr("Corridor ..."));
        m_GridColorAction->setText(tr("Grid ..."));
        m_GaugeColorAction->setText(tr("Gauge..."));
        m_RestrictionColorMenu->setTitle(tr("Restrictions"));
        m_RestrictionColorAction->setText(tr("Restriction ..."));
        m_RestrictionUserColorAction->setText(tr("User restriction ..."));
        m_RestrictionUserObstacleColorAction->setText(tr("User restriction Obstacle ..."));
        m_RestrictionUserNFZColorAction->setText(tr("User restriction NFZ ..."));
        m_RestrictionMapColorAction->setText(tr("User map area..."));
        m_MapNoteColorAction->setText(tr("Map Note color..."));
        m_MeteoColorAction->setText(tr("Meteo color..."));
        m_ListColorMenu->setTitle(tr("Plan Lists"));
        m_ListActColorAction->setText(tr("Act..."));
        m_ListAssumeColorAction->setText(tr("Assume..."));
        m_ListInColorAction->setText(tr("In..."));
        m_ListSelectedTextColorAction->setText(tr("Selected Text..."));
        m_ListTransferedColorAction->setText(tr("Transferred..."));
        m_BearingColorMenu->setTitle(tr("Bearing"));
        m_ColorSchemeMenu->setTitle(tr("Color scheme"));
        m_DarkCSAction->setText(tr("Dark"));
        m_KhakiCSAction->setText(tr("Khaki"));
        m_EuroControlCSAction->setText(tr("Euro Control"));
        m_LightCSAction->setText(tr("Light"));
    }

    if (help_action_)
        help_action_->setText(tr("Help"));
    // Menu help
    m_HelpAbout->setText(tr("About"));

    // Status bar
    m_pDataSource->setText(tr("Data source"));
}

bool XMasterWindow::InitWin(bool lockMouse)
{
    // Load user profile settings.
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    //stg.SetUser(networkSettings()->value("database/user").toString());
    stg.SetUser(QSqlDatabase::database().userName());
    if (!stg.Load())
        LOG_STR(Log4Qt::Level::WARN_INT, tr("User profile settings were not loaded ..."));

    setWindowTitle(tr("XMaster"));
    CMaster::Init();

    QString sSectors = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
    QStringList sSectorsList = sSectors.split(",", QString::SkipEmptyParts);
    TVA_CATCHALL_TRY
    {
        dao()->InitOLDI(sSectorsList, false, false);
    }TVA_CATCHALL(err){
    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}

    if (networkSettings()->value("simulator/server_type").toString() == "server")
    {
        connect(m_pOLDIProc.data(), SIGNAL(recieveMessage(QSharedPointer<OLDIMessage>)), this, SLOT(RecieveOLDIMessage(QSharedPointer<OLDIMessage>)));
    }

    // Check password
    {
        // Initialize command line signleton if was not initialized yet.
        CmdLineArguments &cmdLine = CmdLineArguments::instance(QCoreApplication::arguments());
        // Check if command line has user name and password set.
        QString user = cmdLine.username(),
                password = cmdLine.password();

        bool isNeedDialog = true;
        Password Pass(true, lockMouse, 0);
        // Try to load user using specified user name and password from command line.
        if ( !user.isEmpty() )
        {
            if (Pass.loadUser(user, password))
                isNeedDialog = false;
        }
        // Load Authorization dialog if needed.
        if ( isNeedDialog )
        {
            if (!Pass.exec())
                return false;
        }
        mUser = Pass.user();
        currentUserName = mUser->GetName();
    }

    LogUserLogon(true, currentUserName);

    loadShortcuts();

    if (m_bFullScreenMode) {
        showFullScreen();
        statusBar()->addPermanentWidget(new QSizeGrip(this));
    }
    else {
        show();
    }

    XDocWidget* dw =new XDocWidget(XDocWidget::CtrlWindow,this);

    QSharedPointer<XDocWidget> control_window_dock(dw);

    control_window_ = new ControlWindow;
    connect(control_window_, SIGNAL(bearingIdChanged()), SLOT(onBearingIdChanged()));

    control_window_dock->setWidget(control_window_);
    control_window_dock->setWindowTitle(tr("Control window"));
    m_mapDoc.insert(XDocWidget::CtrlWindow, control_window_dock);
    addDockWidget(Qt::TopDockWidgetArea, control_window_dock.data());
    control_window_dock->setFloating(true);
    control_window_dock->setAllowedAreas(Qt::NoDockWidgetArea);

    control_window_dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    control_window_dock->restoreGeometry(globalSettings()->value("XMasterWindow/Settings/ControlWindowGeometry").toByteArray());
    control_window_dock->installEventFilter(docHandler);
    control_window_dock->setStyleSheet("QDockWidget::title { background: rgb(149, 208, 252); text-align: left; padding-left: 5px; }");

    _CreateMenu();
    _CreateToolBar();
    _CreateStatusBar();

    Translate();

    connect(mRedisDB, SIGNAL(changedSFPL(QSharedPointer<CSFPL>,QString)), this, SLOT(onChangedSFPL(QSharedPointer<CSFPL>,QString)), Qt::QueuedConnection);
    connect(mRedisDB, SIGNAL(changedRWFVPP(QString,RunwayWidgetDataVPP,QString)), this, SLOT(onChangedRWF(QString,RunwayWidgetDataVPP,QString)), Qt::QueuedConnection);
    connect(mRedisDB, SIGNAL(changedSectors(SectorRedis,QString)), this, SLOT(onChangedSectors(SectorRedis,QString)), Qt::QueuedConnection);
    connect(mRedisDB, SIGNAL(changedRestriction(QUuid, QString)), this, SLOT(onChangedRestrictionRedis(QUuid, QString)), Qt::QueuedConnection);
    connect(mRedisDB, SIGNAL(changedConnection(const QString &, bool)), SLOT(onChangedConnectionRedis(const QString &, bool)), Qt::QueuedConnection);
    connect(mRedisDB, SIGNAL(changedConnectionState(bool)), SLOT(onChangedConnectionStateRedis(bool)), Qt::QueuedConnection);

    //connect(mRedisDB, SIGNAL(changedOrcam(QMultiMap<QString, QString>, QString), this, SLOT(onChangedOrcam(QMultiMap<QString, QString>,QString)), Qt::QueuedConnection);
    connect(mRedisDB, SIGNAL(changedOrcam(QString, QString, QString)), this, SLOT(onChangedOrcam(QString, QString, QString)), Qt::QueuedConnection);
    //connect(mRedisDB2, SIGNAL(changedOrcam(QString, QString, QString)), this, SLOT(onChangedOrcam(QString, QString, QString)), Qt::QueuedConnection);

    onChangedConnectionRedis(mRedisDB->currentConnection(), true);
    onChangedConnectionStateRedis(true);

    if (networkSettings()->value("simulator/server_type").toString() == "server")
    {
//#if defined(REDIS)
//        mRedisDB = new RedisDB(this);
//        connect(mRedisDB, SIGNAL(changedSFPL(QSharedPointer<CSFPL>,QString)), this, SLOT(onChangedSFPL(QSharedPointer<CSFPL>,QString)));
//        mRedisDB->Init();
//#endif
        RequestAllFPL();
    }

    _LoadUserEnvironment();

    loadCurrentUserSettings();


    /*const QString fir_names = networkSettings()->value("FPL_handler/fir", "").toString();
    const QStringList fir_names_list = fir_names.split(",", QString::SkipEmptyParts);
    const QString fir_name = fir_names_list.first();

    dao()->GetOrcamList(fir_name, lst_orcamrange);*/

    const QString sector_name = control_window_->sectorCurrentText();

    dao()->GetOrcamList(sector_name, lst_orcamrange);

    mORCAMDialog = new ORCAMDlg(lst_orcamrange, networkSettings()->value("users/ORCAMLifeTime", 1).toInt() ,this);
    
    // Load fonts data from byte array ...
    QDataStream streamFonts(stg.GetFonts());
    TFontMap map;
    streamFonts >> map;
    SetMapXFont(map);
    // To avoid of extra memory allocation reset storage for fonts in UserProfile.
    stg.SetFonts(QByteArray());

    SetSIcombo();

    setRegistered(mRegistered);

    CListAC::GetSourceModel().UpdateCustomColors();

    if (!RestoreSector())
    {
        if (!SetSector(networkSettings()->value("users/default_sector").toString()))
        {
            if (!SetSector(tr("Empty")))
            {
                // Make all sectors inactive
                QSharedPointer<obelix::CCmdSectorChanged> pCmd(new obelix::CCmdSectorChanged(QString("null")));
                GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
            }
        }
    }

    QTimer::singleShot(500, this, SLOT(_OnRestoreFocus()));

    return true;
}

void XMasterWindow::InitRedis()
{
    mRedisDB = new RedisDBHelper(networkSettings()->value("oldi/name", "Unknown").toString(), this);
    connect(mRedisDB, SIGNAL(changedConnection(QString,bool)), SLOT(onRedisConnected(QString,bool)));

    mRedisDB->setInteraval(networkSettings()->value("redis/*timeout", 5).toInt() * 1000);
    mRedisDB->setTableChannel("cwp_channel");
    mRedisDB->setChannel(networkSettings()->value("oldi/name", "Unknown").toString());

    /*mRedisDB2 = new RedisDBHelper(networkSettings()->value("oldi/name", "Unknown").toString(), this);
    connect(mRedisDB2, SIGNAL(changedConnection(QString,bool)), SLOT(onRedisConnected(QString,bool)));
    mRedisDB2->setInteraval(networkSettings()->value("redis/*timeout", 5).toInt() * 1000);
    mRedisDB2->setTableChannel("cwp_channel");
    mRedisDB2->setChannel(networkSettings()->value("oldi/name", "Unknown").toString());
    mRedisDB2->setNotifExpire();*/

    networkSettings()->beginGroup("redis");
    foreach(const QString& group, networkSettings()->childGroups())
    {
        networkSettings()->beginGroup(group);

        RedisParams params;
        params.ip = networkSettings()->value("ip").toString();
        params.port = networkSettings()->value("port").toInt();

        mRedisDB->addRedis(params);
        mRedisDB->addDbParams(params);
        //mRedisDB2->addRedis(params);
        networkSettings()->endGroup();
    }
    networkSettings()->endGroup();

    //mRedisDB->Init();
    mRedisDB->start();
    //mRedisDB2->Init();
}

void XMasterWindow::closeEvent(QCloseEvent* pEvent)
{
    if (!m_bShutdown)
    {
        if (QMessageBox::question(this, windowTitle(), tr("Do you really want to exit?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        {
            pEvent->ignore();
            return;
        }
    }

    if (m_ZonesView)
    {
        CGUIView *guiView = dynamic_cast<CGUIView *>(m_ZonesView.data());
        if (guiView)
            saveToDB(*guiView->GetMainFIR());
    }

    CListAC::StopFutureRefresh();

    _SendSectorsList(QList<uint>());

    saveCurrentUserSettings();

    qApp->setStyleSheet("");

    if(mRedisDB)
        mRedisDB->Close();

    QMainWindow::closeEvent(pEvent);
}

void XMasterWindow::keyPressEvent(QKeyEvent *event)
{
    if (Qt::Key_F10 == event->key()) {
        _OnSaveFocus();
    } else if (Qt::Key_F11 == event->key()) {
        _OnRestoreFocus();
    } else if (Qt::Key_F1 == event->key()) {
        onTriggeredHelpAction();
    } else if (Qt::Key_F2 == event->key()) {
        onTriggeredReferenceAction();
    } else if (Qt::CTRL == event->modifiers()) {
        if (Qt::Key_R == event->key())
            m_CorridorRoute->trigger();
        else if (Qt::Key_S == event->key())
            m_pFontAAsmall->trigger();
        else if (Qt::Key_M == event->key())
            m_pFontAAmedium->trigger();
        else if (Qt::Key_B == event->key())
            m_pFontAAlarge->trigger();
        else if (Qt::Key_X == event->key())
            close();
    } else if ((Qt::CTRL|Qt::ALT) == event->modifiers()) {
        if (Qt::Key_A == event->key()) {
            mOptionMenu->menuAction()->setVisible(!mOptionMenu->menuAction()->isVisible());
        }
    }

    QWidget::keyPressEvent(event);
}

void XMasterWindow::_CreateMenu()
{
    // File
    m_MainMenu = new QMenuBar(0);
    m_MainMenu->setContextMenuPolicy(Qt::PreventContextMenu);
    /*m_FileMenu = m_MainMenu->addMenu(tr("File"));
    m_FileMenu->addSeparator();
    m_LoadAction = new QAction(tr("&Load..."), this);
    m_LoadAction->setShortcut(QKeySequence::Open);
    connect(m_LoadAction, SIGNAL(triggered()), this, SLOT(_LoadSlot()));
    m_FileMenu->addAction(m_LoadAction);
    m_AddAction = new QAction(tr("Add..."), this);
    m_AddAction->setEnabled(false);
    connect(m_AddAction, SIGNAL(triggered()), this, SLOT(_AddSlot()));
    m_FileMenu->addAction(m_AddAction);
    m_FileMenu->addSeparator();
    m_ChUserAction = new QAction(this);
    connect(m_ChUserAction, SIGNAL(triggered()), this, SLOT(_OnLogout()));
    m_FileMenu->addAction(m_ChUserAction);*/
//    m_FileMenu->addSeparator();
//    m_ActRefreshSFPL = new QAction(this);
//    connect(m_ActRefreshSFPL, SIGNAL(triggered()), this, SLOT(_OnRefreshSFPL()));
//    m_FileMenu->addAction(m_ActRefreshSFPL);
//    m_FileMenu->addSeparator();
    /*m_ExitAction = new QAction(tr("E&xit"), this);
    m_ExitAction->setShortcut(tr("Ctrl+X"));
    connect(m_ExitAction, SIGNAL(triggered()), this, SLOT(close()));
    m_FileMenu->addAction(m_ExitAction);*/

    // Operation
    m_AdminMenu = m_MainMenu->addMenu(tr("Operations"));
    m_UserAction = new QAction(this);
    connect(m_UserAction, SIGNAL(triggered()), this, SLOT(_OnUserAction()));
    m_AdminMenu->addAction(m_UserAction);

    m_SectoringAction = new QAction(this);
    connect(m_SectoringAction, SIGNAL(triggered()), this, SLOT(_SectoringSlot()));
    m_AdminMenu->addAction(m_SectoringAction);

    m_BearingAction = new QAction(this);
    connect(m_BearingAction, SIGNAL(triggered()), this, SLOT(_BearingEditor()));
    m_AdminMenu->addAction(m_BearingAction);

    m_MapNoteMenuAction = new QAction(this);
    connect(m_MapNoteMenuAction, SIGNAL(triggered()), this, SLOT(_OnBtnAddMapNote()));
    connect(control_window_, SIGNAL(remindersClicked(bool)), this, SLOT(onRemindersClicked(bool)));
    connect(control_window_, SIGNAL(meteoClicked(bool)), this, SLOT(onMeteoClicked(bool)));
    //m_AdminMenu->addAction(m_MapNoteMenuAction);

    // Option
    mOptionMenu = m_MainMenu->addMenu(tr("Option"));

    mOptionMenu->addMenu(new QtLanguageMenu(this));
    mOptionMenu->menuAction()->setVisible(false);

//    m_Font = new QAction(tr("Font"), this);
//    connect(m_Font, SIGNAL(triggered()), this, SLOT(_Font()));
//    mOptionMenu->addAction(m_Font);

    // Color menu
    mColorMenu = mOptionMenu->addMenu("");
    m_BackgroundColorAction = new QAction("", this);
    connect(m_BackgroundColorAction, SIGNAL(triggered()), this, SLOT(_BackgroundColorSlot()));
    mColorMenu->addAction(m_BackgroundColorAction);

    mFormularMenu = mColorMenu->addMenu("");

    m_FormularPassiveColorAction = new QAction("", this);
    connect(m_FormularPassiveColorAction, SIGNAL(triggered()), this, SLOT(_FormularPassiveColorSlot()));
    mFormularMenu->addAction(m_FormularPassiveColorAction);

    m_FormularActiveColorAction = new QAction("", this);
    connect(m_FormularActiveColorAction, SIGNAL(triggered()), this, SLOT(_FormularActiveColorSlot()));
    mFormularMenu->addAction(m_FormularActiveColorAction);
    mFormularMenu->addSeparator();

    m_ControlledAction = new QAction("", this);
    connect(m_ControlledAction, SIGNAL(triggered()), this, SLOT(_ControlledColorSlot()));
    mFormularMenu->addAction(m_ControlledAction);
    m_ControlledBackAction = new QAction("", this);
    connect(m_ControlledBackAction, SIGNAL(triggered()), this, SLOT(_ControlledBackColorSlot()));
    mFormularMenu->addAction(m_ControlledBackAction);
    m_InAction = new QAction("", this);
    connect(m_InAction, SIGNAL(triggered()), this, SLOT(_InColorSlot()));
    mFormularMenu->addAction(m_InAction);
    m_InBackAction = new QAction("", this);
    connect(m_InBackAction, SIGNAL(triggered()), this, SLOT(_InBackColorSlot()));
    mFormularMenu->addAction(m_InBackAction);
    m_RelatedAction = new QAction("", this);
    connect(m_RelatedAction, SIGNAL(triggered()), this, SLOT(_RelatedColorSlot()));
    mFormularMenu->addAction(m_RelatedAction);
    m_RelatedBackAction = new QAction("", this);
    connect(m_RelatedBackAction, SIGNAL(triggered()), this, SLOT(_RelatedBackColorSlot()));
    mFormularMenu->addAction(m_RelatedBackAction);
    m_UnrelatedAction = new QAction("", this);
    connect(m_UnrelatedAction, SIGNAL(triggered()), this, SLOT(_UnrelatedColorSlot()));
    mFormularMenu->addAction(m_UnrelatedAction);
    m_UnrelatedBackAction = new QAction("", this);
    connect(m_UnrelatedBackAction, SIGNAL(triggered()), this, SLOT(_UnrelatedBackColorSlot()));
    mFormularMenu->addAction(m_UnrelatedBackAction);
    m_ExtendedAction = new QAction("", this);
    connect(m_ExtendedAction, SIGNAL(triggered()), this, SLOT(_ExtendedColorSlot()));
    mFormularMenu->addAction(m_ExtendedAction);

    m_MapColorAction = new QAction("", this);
    connect(m_MapColorAction, SIGNAL(triggered()), this, SLOT(_MapColorSlot()));
    mColorMenu->addAction(m_MapColorAction);
    m_FIRColorAction = new QAction("", this);
    connect(m_FIRColorAction, SIGNAL(triggered()), this, SLOT(_FIRColorSlot()));
    mColorMenu->addAction(m_FIRColorAction);
    m_ActiveSectorColorAction = new QAction("", this);
    connect(m_ActiveSectorColorAction, SIGNAL(triggered()), this, SLOT(_SectorColorSlot()));
    mColorMenu->addAction(m_ActiveSectorColorAction);
    m_BorderColorAction = new QAction("", this);
    connect(m_BorderColorAction, SIGNAL(triggered()), this, SLOT(_BorderColorSlot()));
    mColorMenu->addAction(m_BorderColorAction);
    m_LegendColorAction = new QAction("", this);
    connect(m_LegendColorAction, SIGNAL(triggered()), this, SLOT(_LegendColorSlot()));
    //    mColorMenu->addAction(m_LegendColorAction);
    m_ApproachColorAction = new QAction("", this);
    connect(m_ApproachColorAction, SIGNAL(triggered()), this, SLOT(_ApproachColorSlot()));
    mColorMenu->addAction(m_ApproachColorAction);
    m_BlastoffColorAction = new QAction("", this);
    connect(m_BlastoffColorAction, SIGNAL(triggered()), this, SLOT(_BlastoffColorSlot()));
    mColorMenu->addAction(m_BlastoffColorAction);
    m_RouteColorAction = new QAction("", this);
    connect(m_RouteColorAction, SIGNAL(triggered()), this, SLOT(_RouteColorSlot()));
    mColorMenu->addAction(m_RouteColorAction);
    m_HoldingColorAction = new QAction("", this);
    connect(m_HoldingColorAction, SIGNAL(triggered()), this, SLOT(_HoldingColorSlot()));
    mColorMenu->addAction(m_HoldingColorAction);
    m_CorridorAction = new QAction("", this);
    connect(m_CorridorAction, SIGNAL(triggered()), this, SLOT(_CorridorColorSlot()));
    mColorMenu->addAction(m_CorridorAction);
    m_GridColorAction = new QAction("", this);
    connect(m_GridColorAction, SIGNAL(triggered()), this, SLOT(_GridColorSlot()));
    mColorMenu->addAction(m_GridColorAction);
    m_GaugeColorAction = new QAction("", this);
    connect(m_GaugeColorAction, SIGNAL(triggered()), this, SLOT(_GaugeColorSlot()));
    mColorMenu->addAction(m_GaugeColorAction);
    // Restrictions
    m_RestrictionColorMenu = mColorMenu->addMenu("");
    m_RestrictionColorAction = new QAction("", this);
    connect(m_RestrictionColorAction, SIGNAL(triggered()), this, SLOT(_RestrictionColorSlot()));
    m_RestrictionColorMenu->addAction(m_RestrictionColorAction);
    m_RestrictionUserColorAction = new QAction("", this);
    connect(m_RestrictionUserColorAction, SIGNAL(triggered()), this, SLOT(_RestrictionUserColorSlot()));
    m_RestrictionColorMenu->addAction(m_RestrictionUserColorAction);
    m_RestrictionUserObstacleColorAction = new QAction("", this);
    connect(m_RestrictionUserObstacleColorAction, SIGNAL(triggered()), this, SLOT(_RestrictionUserObstacleColorSlot()));
    m_RestrictionColorMenu->addAction(m_RestrictionUserObstacleColorAction);
    m_RestrictionUserNFZColorAction = new QAction("", this);
    connect(m_RestrictionUserNFZColorAction, SIGNAL(triggered()), this, SLOT(_RestrictionUserNFZColorSlot()));
    m_RestrictionColorMenu->addAction(m_RestrictionUserNFZColorAction);
    m_RestrictionMapColorAction = new QAction("", this);
    connect(m_RestrictionMapColorAction, SIGNAL(triggered()), this, SLOT(_RestrictionUserMapColorSlot()));
    m_RestrictionColorMenu->addAction(m_RestrictionMapColorAction);

    m_MapNoteColorAction = new QAction("", this);
    connect(m_MapNoteColorAction, SIGNAL(triggered()), this, SLOT(_MapNoteColorSlot()));
    mColorMenu->addAction(m_MapNoteColorAction);

    m_MeteoColorAction = new QAction("", this);
    connect(m_MeteoColorAction, SIGNAL(triggered()), this, SLOT(_MeteoColorSlot()));
    mColorMenu->addAction(m_MeteoColorAction);


    m_ListColorMenu = mColorMenu->addMenu("");
    m_ListActColorAction = new QAction("", this);
    connect(m_ListActColorAction, SIGNAL(triggered()), this, SLOT(_ListActColorSlot()));
    m_ListColorMenu->addAction(m_ListActColorAction);
    m_ListAssumeColorAction = new QAction("", this);
    connect(m_ListAssumeColorAction, SIGNAL(triggered()), this, SLOT(_ListAssumeColorSlot()));
    m_ListColorMenu->addAction(m_ListAssumeColorAction);
    m_ListInColorAction = new QAction("", this);
    connect(m_ListInColorAction, SIGNAL(triggered()), this, SLOT(_ListInColorSlot()));
    m_ListColorMenu->addAction(m_ListInColorAction);
    m_ListSelectedTextColorAction = new QAction("", this);
    connect(m_ListSelectedTextColorAction, SIGNAL(triggered()), this, SLOT(_ListSelectedTextColorSlot()));
    m_ListColorMenu->addAction(m_ListSelectedTextColorAction);
    m_ListTransferedColorAction = new QAction("", this);
    connect(m_ListTransferedColorAction, SIGNAL(triggered()), this, SLOT(_ListTransferedColorSlot()));
    m_ListColorMenu->addAction(m_ListTransferedColorAction);

    m_BearingColorMenu = mColorMenu->addMenu("");
    lst_bearing.clear();
    dao()->GetBearingList(lst_bearing);
    for(int i = 0; i < lst_bearing.count(); i++){
        QMenu* m_BearingAirportColorAction;
        m_BearingAirportColorAction = m_BearingColorMenu->addMenu("");
        m_BearingAirportColorAction->setTitle(lst_bearing[i]);
        lst_frequency.clear();
        dao()->GetFrequencyList(lst_bearing[i], lst_frequency);
        for(int j = 0; j < lst_frequency.count(); j++){
            QAction* m_BearingAirportFrequencyColorAction;
            m_BearingAirportFrequencyColorAction = new QAction("", this);
            m_BearingAirportFrequencyColorAction->setText(lst_frequency[j]);
            m_BearingAirportFrequencyColorAction->setProperty("airport", lst_bearing[i]);
            connect(m_BearingAirportFrequencyColorAction, SIGNAL(triggered()), this, SLOT(_BearingAirportFrequencyColorSlot()));
            m_BearingAirportColorAction->addAction(m_BearingAirportFrequencyColorAction);
        }
    }

    mColorMenu->addSeparator();

    // Color scheme menu
    m_ColorSchemeMenu = mColorMenu->addMenu("");

    m_DarkCSAction = new QAction("", this);
    connect(m_DarkCSAction, SIGNAL(triggered()), this, SLOT(_DarkCSSlot()));
    m_ColorSchemeMenu->addAction(m_DarkCSAction);
    m_KhakiCSAction = new QAction("", this);
    connect(m_KhakiCSAction, SIGNAL(triggered()), this, SLOT(_KhakiCSSlot()));
    m_ColorSchemeMenu->addAction(m_KhakiCSAction);
    m_EuroControlCSAction = new QAction("", this);
    connect(m_EuroControlCSAction, SIGNAL(triggered()), this, SLOT(_EuroControlCSSlot()));
    m_ColorSchemeMenu->addAction(m_EuroControlCSAction);
    m_LightCSAction = new QAction("", this);
    connect(m_LightCSAction, SIGNAL(triggered()), this, SLOT(_LightCSSlot()));
    m_ColorSchemeMenu->addAction(m_LightCSAction);

    m_PlanListsEditor = new QAction("Plan Lists Editor", this);
    connect(m_PlanListsEditor, SIGNAL(triggered()), this, SLOT(_OnPlanListsEditor()));
    mOptionMenu->addAction(m_PlanListsEditor);

    m_ScrollLists = new QShortcut(QKeySequence(m_Shortcuts["ExpandList"]), this, SLOT(_ToggleScrollTrigerred()));
    m_pRunwayWidgetToggle = new QShortcut(QKeySequence(m_Shortcuts["RunwayWidgetMinimize"]), this);

    m_HelpMenu = m_MainMenu->addMenu("?");
    help_action_ = m_HelpMenu->addAction(tr("Help"));
    help_action_->setShortcut(Qt::Key_F1);
    connect(help_action_, SIGNAL(triggered()), this, SLOT(onTriggeredHelpAction()));

    m_HelpAbout = new QAction(tr("About"), this);
    m_HelpMenu->addAction(m_HelpAbout);
    connect(m_HelpAbout, SIGNAL(triggered()), this, SLOT(_OnAbout()));
    setMenuBar(m_MainMenu);
}

void XMasterWindow::_CreateToolBar()
{
    _InitSectoringCombo();
    _InitMainCombo();
    _InitPressureCombo();
    _InitPlaneCombo();
    _InitCustomSector();
    _InitAltitudeFilter();
    _InitRestriction();
    _InitGroundBar();

}

void XMasterWindow::_CreateStatusBar()
{
    m_pStatusLabel = new QLabel("N99 99 99 S099 99 99", this);
    m_pStatusLabel->setAlignment(Qt::AlignLeft);
    m_pStatusLabel->setMinimumSize(m_pStatusLabel->sizeHint());
    statusBar()->addWidget(m_pStatusLabel);
    m_pStatusLabel->setText("");
    statusBar()->setStyleSheet("QStatusBar::item {border-style:none;}");

    // Data source
    m_pDataSource = new QToolButton(this);
    m_pDataSource->setText(tr("Data source"));
    m_pDataSource->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_pDataSource->setPopupMode(QToolButton::InstantPopup);
    m_pDataSource->setStyleSheet(
        " QToolButton { "
        "padding-right: 20px; } "
        " QToolButton::menu-indicator { "
        "subcontrol-origin: padding; "
        "subcontrol-position: center right; }"
        );
    QMenu* pSetMenu = new QMenu(this);
    m_pDataSource->setMenu(pSetMenu);
    m_pDataSource->addAction(pSetMenu->menuAction());
    m_pDataSource->setDefaultAction(pSetMenu->menuAction());

    OnDataSourceMenu();
    connect(pSetMenu, SIGNAL(aboutToShow()), this, SLOT(OnDataSourceMenu()));
    m_pDataSource->hide();
    //statusBar()->addPermanentWidget(m_pDataSource);

    m_pLicenseLabel = new QLabel(this);
  //  m_pLicenseLabel->setStyleSheet("QLabel { color : red; }");
    m_pLicenseLabel->setAlignment(Qt::AlignCenter);
    statusBar()->addPermanentWidget(m_pLicenseLabel);

    active_db_ = new QLabel("DB:", this);
    active_db_->setStyleSheet("QLabel { color : red; }");
    {
        QSqlDatabase database = QSqlDatabase::database();
        QString info = "DB:";
        if (database.isValid()) {
            info += database.hostName();
            active_db_->setText(info);
            if (database.isOpen())
                active_db_->setStyleSheet("QLabel { color : green; }");
        }
    }
    active_db_->setAlignment(Qt::AlignRight);
    statusBar()->addPermanentWidget(active_db_);

    active_redis_ = new QLabel("REDIS:", this);
    active_redis_->setStyleSheet("QLabel { color : red; }");
    active_redis_->setAlignment(Qt::AlignRight);
    statusBar()->addPermanentWidget(active_redis_);

    m_pStatusTempText = new QLabel(this);
    m_pStatusTempText->setAlignment(Qt::AlignRight);
    statusBar()->addPermanentWidget(m_pStatusTempText);

    m_pTimeLabel = new QLabel(this);
    m_pTimeTimer = new QTimer(this);
//    QFont ClockFont = m_pTimeLabel->font();
//    ClockFont.setPointSize(ClockFont.pointSize() * 1.2);
//    ClockFont.setBold(true);
//    m_pTimeLabel->setFont(ClockFont);
    connect(m_pTimeTimer, SIGNAL(timeout()), this, SLOT(_OnTimeTimer()));
    _OnTimeTimer();
    m_pTimeTimer->start(1000);
    m_pTimeLabel->setAlignment(Qt::AlignRight);
    m_pTimeLabel->setMinimumSize(m_pTimeLabel->sizeHint());
    statusBar()->addPermanentWidget(m_pTimeLabel);
}

void XMasterWindow::ZoomEvent(int iCurScale)
{
    m_iSlider = iCurScale;
    m_ZoomSlider->setValue(m_iSlider);
    control_window_->setZoom(m_iSlider);
}

void XMasterWindow::CoordinateEvent(const CCoord& Coordinate, const QString& sRoute)
{
    QStringList result;
    result.append(Coordinate.GetAsString());

    if (mMainAirport)
    {
        double declination = 0;
        if (networkSettings()->value("users/declination_enable", "no") == "yes")
        {
            declination = networkSettings()->value("users/declination_value", 0).toDouble();
            if (declination > 180 || declination < -180)
                declination = 0;
        }
        double angle = (GetAzimuth(Coordinate, mMainAirport->GetCoord()) * 180/M_PI) - declination;
        double distance = GetDistance(mMainAirport->GetCoord(), Coordinate) / 1000;

        result.append(QString("A - %1\u00B0 D - %2km").arg(qRound(180 + angle) % 360).arg(distance, 0, 'f', 1));
    }

    if (!sRoute.isEmpty())
    {
        result.append(tr("Route[%1]").arg(sRoute));
    }

    m_pStatusLabel->setText(result.join("   "));
}

void XMasterWindow::_LoadSlot()
{
    TVA_CATCHALL_TRY
    {
        LoadFromDB LoadFromDBForm(tr("Load from database..."), false, this);
        if (LoadFromDBForm.exec())
        {
            _SetCustomSector();
            _SendSectorsList(QList<uint>());
            _LoadFIR(LoadFromDBForm.results()[0].toUInt());
        }
    }TVA_CATCHALL(err){
    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
    QMessageBox::critical(this, tr("Error"), err.reason());
    }
}

void XMasterWindow::_AddSlot()
{
    if(m_ZonesView)
    {
        TVA_CATCHALL_TRY
        {
            LoadFromDB LoadFromDBForm(tr("Load from database..."), false, this);
            if (LoadFromDBForm.exec())
            {
                _AddFIR(LoadFromDBForm.results()[0].toUInt());
            }
        }TVA_CATCHALL(err){
        LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
        QMessageBox::critical(this, tr("Error"), err.reason());}
    }
}

void XMasterWindow::_OnLogout()
{
    if (QMessageBox::question(this, tr("Logout"), tr("Are you sure want to logout?"), QMessageBox::Ok
                              | QMessageBox::Cancel) == QMessageBox::Ok)
    {
        CloseDoc(XDocWidget::ListActIn);
        CloseDoc(XDocWidget::ListActOut);
        CloseDoc(XDocWidget::ListActDep);
        CloseDoc(XDocWidget::MeteoRunwayInfo);
        CloseDoc(XDocWidget::MeteoRunwayInfo2);
        CloseDoc(XDocWidget::Runway);
        control_window_->MeteoVisible(false);
        CloseDoc(XDocWidget::UserPicsList);
        CloseDoc(XDocWidget::ListDailyPlan);
        CloseDoc(XDocWidget::ListCost);
        _SetCustomSector();
        CleanActiveSector();
        _OnRefreshSFPL();
        LogUserLogon(false, currentUserName);
        // Check password
        Password Pass(false, false, this);
        if (Pass.exec())
        {
            if (Pass.user()->GetID() != mUser->GetID())
            {
                mUser = Pass.user();
                currentUserName = mUser->GetName();
                LogUserLogon(true, currentUserName);
                _LoadUserEnvironment();
                //Pasha
                //if(-1 == m_pSectoringCombo->currentIndex())
                if (control_window_->sectorCurrentIndex())
                {
                    TVA_CATCHALL_TRY
                    {
                        _LoadEnvironment(0);
                    }TVA_CATCHALL(err){
                    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
                }
                else
                {
                    _SetCustomSector();
                    CleanActiveSector();
                }
            }
            else
            {
                LogUserLogon(true, currentUserName);
                //int iIndex = m_pSectoringCombo->currentIndex();
                int iIndex = control_window_->sectorCurrentIndex();
                if(m_pSectoringCombo->count() > 0 && iIndex >= 0 && iIndex < m_pSectoringCombo->count())
                {
                    if(m_pSectoringModel->record(iIndex).value("id").toInt())
                    {
                        _UndoSectoring();
                    }
                }
            }
            QSharedPointer<obelix::CCmdRequestAllFPL> pCmd(new obelix::CCmdRequestAllFPL());
            GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
            //RequestAllFPL();
            QTimer::singleShot(500, this, SLOT(_OnRestoreFocus()));
        }
    }
}

void XMasterWindow::_SectoringSlot()
{
    CGUIView *view = dynamic_cast<CGUIView*>(m_ZonesView.data());
    Sectoring dlg(this);
    if (view)
    {
        dlg.setBearingPoints(view->GetListDF());
        if (dlg.exec() == QDialog::Accepted)
        {
            //int iInd = m_pSectoringCombo->currentIndex();
            int iInd = control_window_->sectorCurrentIndex();
            m_pSectoringModel->select();
            _SelectSectorModel();
            m_pSectoringCombo->setCurrentIndex(iInd);
            control_window_->setSectorCurrentIndex(iInd);
        }
    }
    else dlg.exec();
}

void XMasterWindow::_OnUserAction()
{
    UserAdmin userAdmin(mUser, this);
    if(userAdmin.exec() == QDialog::Accepted)
    {
        m_pSectoringModel->select();
        _SelectSectorModel();
    }
}

void XMasterWindow::_OnFontAAsmall()
{
    GetTextSize().SetCurrentSize(CTextSize::Small);
    networkSettings()->setValue("users/TextSize/CurTextSize", int(GetTextSize().GetCurrentSize()));
    QFont font = qApp->font();
    font.setPointSize(GetTextSize().GetCurrentTextSize());
    _SetChangedFont(font);
}

void XMasterWindow::_OnFontAAmedium()
{
    GetTextSize().SetCurrentSize(CTextSize::Medium);
    networkSettings()->setValue("users/TextSize/CurTextSize", int(GetTextSize().GetCurrentSize()));
    QFont font = qApp->font();
    font.setPointSize(GetTextSize().GetCurrentTextSize());
    _SetChangedFont(font);
}

void XMasterWindow::_OnFontAAlarge()
{
    GetTextSize().SetCurrentSize(CTextSize::Large);
    networkSettings()->setValue("users/TextSize/CurTextSize", int(GetTextSize().GetCurrentSize()));
    QFont font = qApp->font();
    font.setPointSize(GetTextSize().GetCurrentTextSize());
    _SetChangedFont(font);
}

void XMasterWindow::_OnActiveSector()
{
    if (m_ZonesView)
    {
        if (QSharedPointer<CView> pscene = qSharedPointerDynamicCast<CView>(m_ZonesView))
        {
            if (!pscene->GetMainFIR().isNull())
            {
                ActiveSectors ActiveSectors(pscene->GetMainFIR()->GetSectorList());
                ActiveSectors.exec();
            }
        }
    }
}

void XMasterWindow::_ZoomSlider(int iValue)
{
    if (m_ZonesView && m_iSlider != iValue)
    {
        m_iSlider = iValue;
        if (CGUIView* pscene = dynamic_cast<CGUIView*> (m_ZonesView.data()))
            pscene->SetZoom(iValue);
        else
        {
            if (GUIGroundScene* groundScene = dynamic_cast<GUIGroundScene*> (m_ZonesView.data()))
                groundScene->SetZoom(iValue);
        }
    }
}

void XMasterWindow::_OnChangeSI_K(int index)
{
    LOG_OPER_STR("ALL", "SI", index ? "knots" : "km/h");
    quint8 tempSI = 0;
    if(index)
        tempSI = GetSI() | SIKnot;
    else
        tempSI = GetSI() & ~SIKnot;
    SetSI(tempSI);
    UpdateSI();
}

void XMasterWindow::_OnChangeSI_M(int index)
{
    LOG_OPER_STR("ALL", "SI", index ? "feet" : "meters");
    if (m_EchelonCombo)
    {
        //m_EchelonCombo->setItemText(0, index ? "30" : "090");
        m_EchelonCombo->setItemText(0, index ? "30" : "90");
        m_EchelonCombo->setItemText(1, index ? "40" : "120");
        m_EchelonCombo->setItemText(2, index ? "50" : "150");
        m_EchelonCombo->setItemText(3, index ? "60" : "180");
        m_EchelonCombo->setItemText(4, index ? "70" : "210");
        m_EchelonCombo->setItemText(5, index ? "80" : "240");
        m_EchelonCombo->setItemText(6, index ? "90" : "270");
        m_EchelonCombo->setItemText(7, index ? "100" : "300");
        m_EchelonCombo->setItemText(8, index ? "110" : "330");
        m_EchelonCombo->setItemText(9, index ? "120" : "360");
        for(int i = 10; i < m_EchelonCombo->count(); ++i)
        {
            int ech = m_EchelonCombo->itemText(i).toInt();
            QString sText = index ? QString("%1").arg(qRound(ech/3.048)/*, 2, 10, QLatin1Char('0')*/)
                                  : QString("%1").arg(qRound(ech*3.048)/*, 3, 10, QLatin1Char('0')*/);
            m_EchelonCombo->setItemText(i, sText);
        }
    }
    if(m_FLSlider)
    {
        QString sText;
        if(index)
            sText = QString("%1 - %2").arg(m_FLSlider->lowerValue(), 3, 10, QLatin1Char('0')).arg(m_FLSlider->upperValue(), 3, 10, QLatin1Char('0'));
        else
            sText = QString("%1 - %2").arg(qRound(m_FLSlider->lowerValue()*30.48), 3, 10, QLatin1Char('0')).arg(qRound(m_FLSlider->upperValue()*30.48), 3, 10, QLatin1Char('0'));
        m_pSliderBut->setText(sText);
        m_pSliderBut->adjustSize();
    }
    quint8 tempSI = 0;
    if(index)
        tempSI = GetSI() | SIFeet;
    else
        tempSI = GetSI() & ~SIFeet;
    SetSI(tempSI);
    UpdateSI();

    SetEchelonString();
}

void XMasterWindow::_OnCheckGauge(bool checked)
{
    if(checked)
        QApplication::setOverrideCursor(Qt::UpArrowCursor);
    else QApplication::setOverrideCursor(Qt::ArrowCursor);
    if (m_ZonesView)
    {
        CGUIView* pscene = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pscene)
            pscene->SetGauge(checked);
        GUIGroundScene* groundScene = dynamic_cast<GUIGroundScene*> (m_ZonesView.data());
        if (groundScene)
            groundScene->SetGauge(checked);
    }

}

void XMasterWindow::_OnChangeEchelon(int index, bool bFromRedis/* = false*/)
{
    if (index != -1)
    {
        LOG_OPER_STR("ALL", "Echelon", GetEchelonValue(IsSISet(AppSettings::SIFeet)));
    }
    if (m_ZonesView && (index != -1))
    {
        CGUIView* pscene = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pscene)
        {
            pscene->SetEchelon(GetEchelonValue(IsSISet(AppSettings::SIFeet)));
            if(pscene->GetMainFIR())
            {
                m_mapEchelon.insert(pscene->GetMainFIR()->GetIDName(), pscene->GetEchelon());
                {
                    // Echelon values
                    QByteArray arr;
                    QDataStream ds(&arr, QIODevice::WriteOnly);
                    ds << m_mapEchelon;
                    globalSettings()->setValue("XMasterWindow/Settings/EchelonValues", arr);
                }
                if(getRedisDB() && !bFromRedis)
                {
                    RunwayWidgetDataVPP rwfv(pscene->GetMainFIR()->GetIDName());
                    if(getRedisDB()->loadRWFVPP(pscene->GetMainFIR()->GetIDName(), rwfv))
                    {
                        rwfv.setTL(pscene->GetEchelon());
                        getRedisDB()->saveRWFVPP(pscene->GetMainFIR()->GetIDName(), rwfv);
                    }
                }
            }
        }
    }
    CListAC::GetSourceModel().SetEchelon(GetEchelonValue(IsSISet(AppSettings::SIFeet)));

    SetEchelonString();
}

void XMasterWindow::_OnChangePress(int iValue, bool bFromRedis/* = false*/)
{
    if (iValue > 0)
    {
        LOG_OPER_STR("ALL", "Pressure", iValue);
        m_pPressBut->setText(QString("%1")
                               .arg(iValue, 3, 10, QLatin1Char('0')));
        //control_window_->setPressure(QString("%1").arg(iValue, 3, 10, QLatin1Char('0')));
        m_pPressBut->adjustSize();
    }
    if (m_ZonesView && (iValue > 0))
    {
        CGUIView* pscene = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pscene)
        {
            pscene->SetPress(iValue);
            if(pscene->GetMainFIR())
            {
                m_mapPressure.insert(pscene->GetMainFIR()->GetIDName(), iValue);
                {
                    // Pressure values
                    QByteArray arr;
                    QDataStream ds(&arr, QIODevice::WriteOnly);
                    ds << m_mapPressure;
                    globalSettings()->setValue("XMasterWindow/Settings/PressureValues", arr);
                }
                if(getRedisDB() && !bFromRedis)
                {
                    RunwayWidgetDataVPP rwfv(pscene->GetMainFIR()->GetIDName());
                    if(getRedisDB()->loadRWFVPP(pscene->GetMainFIR()->GetIDName(), rwfv))
                    {
                        rwfv.setQNH(iValue);
                        getRedisDB()->saveRWFVPP(pscene->GetMainFIR()->GetIDName(), rwfv);
                    }

                    if(!networkSettings()->value("RW1/name").toString().isEmpty())
                    {
                        RunwayWidgetDataFIR rwf1(pscene->GetMainFIR()->GetIDName());

                        if(getRedisDB()->loadRWF1(pscene->GetMainFIR()->GetIDName(), rwf1))
                            getRedisDB()->saveRWF1(pscene->GetMainFIR()->GetIDName(), rwf1);
                    }
                    if(!networkSettings()->value("RW2/name").toString().isEmpty())
                    {
                        RunwayWidgetDataFIR rwf2(pscene->GetMainFIR()->GetIDName());
                        if(getRedisDB()->loadRWF2(pscene->GetMainFIR()->GetIDName(), rwf2))
                            getRedisDB()->saveRWF2(pscene->GetMainFIR()->GetIDName(), rwf2);
                    }
                }
            }
        }
    }
    CListAC::GetSourceModel().SetPress(iValue);

    SetEchelonString();
}

void XMasterWindow::_OnAbout()
{
    //QSharedPointer<OLDIMesURZ> pMes(new OLDIMesURZ("(URZFDP/CWPB138-I-UTDD-0000077c000004d400000052000000060034003400344008000000000000403300000000000000257a16019d5c50ff00257fcb019d5c50ff0bc29da69fa24a5c9afe018950cb001f000000010000000a000019f90000000a000000430000000000000000000000000000000040438615c813549c40516408329eb8bd40c5879667c4388c0000077c)"));
    //RecieveOLDIMessage(pMes);

//    QSharedPointer<CSFPL> sfpl(new CSFPL);
//    sfpl->SetID(555);
//    sfpl->SetFirName("UTTR");
//    sfpl->SetACID("AFL1234");
//    mRedisDB->saveSFPL(sfpl);

//    XAbout(this).exec();

//    QList<QSharedPointer<CSFPL> > list;
//    mRedisDB->loadSFPLs(list);
//    if(list.size())
//    {
//        QSharedPointer<CSFPL> sfpl = list.last();
//        sfpl->SetID(777);
//        mRedisDB->deleteSFPL(sfpl);
//    }

    XAbout dialog(mRegistered, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        setRegistered(true);
    }
}

void XMasterWindow::_OnSectoringCombo(int iIndex)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    TVA_CATCHALL_TRY
    {
        if (m_pSectoringCombo->count() > 0 && iIndex >= 0 && iIndex < m_pSectoringCombo->count())
        {
            qDebug() << "selected sector " + control_window_->sectorCurrentText();
            //QSharedPointer<obelix::CCmdSectorChanged> pCmd(new obelix::CCmdSectorChanged(m_pSectoringCombo->currentText()));
            QSharedPointer<obelix::CCmdSectorChanged> pCmd(new obelix::CCmdSectorChanged(control_window_->sectorCurrentText()));
            GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
            SetSectoringInitiator(true);
            //_LoadEnvironment(m_pSectoringModel->record(iIndex).value("id").toInt());
            //networkSettings()->setValue("users/current_sector", m_pSectoringCombo->itemText(iIndex));
            //control_window_->setBearingId(m_pSectoringModel->record(iIndex).value("bearing_point_id"));
            control_window_->setBearingId(m_pSectoringModel->record(iIndex).value("id").toUInt());

            saveApp(m_pSectoringModel->record(iIndex).value("id").toInt());

            QString sector_name;
            sector_name = control_window_->sectorCurrentText();
            lst_orcamrange.clear();
            dao()->GetOrcamList(sector_name, lst_orcamrange);

            bool OrcamVisible = false;
            if (mORCAMDialog->isVisible())
                OrcamVisible = true;
            delete mORCAMDialog;
            mORCAMDialog = new ORCAMDlg(lst_orcamrange, networkSettings()->value("users/ORCAMLifeTime", 1).toInt() ,this);
            if (OrcamVisible)
                onORCAMDialog();

        }
        else
        {
            //Pasha may be not in this place
            _SendSectorsList(QList<uint>());
        }
    }TVA_CATCHALL(err){
    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
        QMessageBox::critical(this, tr("Error"), err.reason());
    }
    QApplication::restoreOverrideCursor();
}

void XMasterWindow::_OnTimeTimer()
{
    QDateTime date_time = QDateTime::currentDateTime();
    m_pTimeLabel->setText(date_time.toString("dd.MM.yy hh:mm:ss"));

    control_window_->setCurrentDate(date_time.date().toString("dd.MMM.yy"));
    control_window_->setCurrentTime(date_time.time().toString("hh:mm:ss"));
}

void XMasterWindow::InitAFTNMessageProcessor()
{
    if (aftnMessageProcessor)
        delete aftnMessageProcessor;
    aftnMessageProcessor = new AFTNMessageProcessor(QSqlDatabase::database(), qApp);

    networkSettings()->beginGroup("FPL_handler");
    aftnMessageProcessor->setFIRName(networkSettings()->value("fir", "UTTR,UTT2").toString());
    aftnMessageProcessor->setSaveOnlyFIR(networkSettings()->value("fir_route", true).toBool());
    aftnMessageProcessor->setValidateRoute(networkSettings()->value("chk_route", true).toBool());
    aftnMessageProcessor->setStandard(networkSettings()->value("FPL_2012", false).toBool()
                                      ? AFTNMessageProcessor::FPL_2012
                                      : AFTNMessageProcessor::FPL_2007);
    networkSettings()->endGroup();

    disconnect(this, SLOT(InitAFTNMessageProcessor()));
}

void XMasterWindow::_OnDBConnected(const DbParams &database)
{
    connected_to_db_ = true;

    if (active_db_) {
        active_db_->setStyleSheet("QLabel { color : green; }");
        active_db_->setText("DB:" + database.host);
    }

    reloadRestrictions();

    restrictions_widget_->onDbConnectionChanged(true);
}

void XMasterWindow::_OnDBDisconnected()
{
    connected_to_db_ = false;

    if (active_db_)
        active_db_->setStyleSheet("QLabel { color : red; }");

    deleteRestrictions();

    restrictions_widget_->onDbConnectionChanged(false);
}

void XMasterWindow::_OnDBConnectAttemptFailed(const DbParams &params)
{
    if (active_db_) {
        active_db_->setStyleSheet("QLabel { color : red; }");
        active_db_->setText("DB:" + params.host);
    }
}

void XMasterWindow::_OnSetConnectRedisTrue()
{
    if(connectRedisTrue)
       return;
    connectRedisTrue = true;
    LOG_OPER_SIMPLE_STR(tr("Connection database Redis successful!"));
    if(getConnect())
        emit closeDialog();
}

void XMasterWindow::_OnSetConnectPostgresTrue()
{
    connectPostgresTrue = true;
    LOG_OPER_SIMPLE_STR(tr("Connection database Postgres successful!"));
    if(getConnect())
        emit closeDialog();
}

void XMasterWindow::_OnBtnLayouts()
{
    if ( dlgLayout )
    {
        dlgLayout->close();
        dlgLayout->deleteLater();
        dlgLayout = 0;
    }
    //else
    {
        dlgLayout = new LayoutDlg(false, m_ZonesView, this);
        dlgLayout->setModal(false);
        dlgLayout->show();
        connect(dlgLayout, SIGNAL(applyLayouts()), SLOT(onApplyLayouts()));
    }
}

void XMasterWindow::_OnBtnLayoutsMainFIR()
{
    if ( dlgLayout )
    {
        dlgLayout->close();
        dlgLayout->deleteLater();
        dlgLayout = 0;
    }
    //else
    {
        dlgLayout = new LayoutDlg(true, m_ZonesView, this);
        dlgLayout->setModal(false);
        dlgLayout->show();
        connect(dlgLayout, SIGNAL(applyLayouts()), SLOT(onApplyLayouts()));
    }
}

void XMasterWindow::onApplyLayouts()
{
    QSettings &settings = *globalSettings();
    settings.beginGroup(currentUserName);
    QByteArray qArray;
    QDataStream qStream(&qArray, QIODevice::ReadWrite);
    LayoutDlg::Save(qStream);
    settings.setValue("Layers", qArray);
    settings.endGroup();
}

void XMasterWindow::_OnBtnAddMapNote()
{
    if (m_ZonesView) {
        if (CGUIView *view = dynamic_cast<CGUIView *>(m_ZonesView.data()))
            view->startCoordsPicking(false);
    }
}

void XMasterWindow::onRemindersClicked(bool checked)
{
    if(checked)
        QApplication::setOverrideCursor(Qt::CrossCursor);
    else QApplication::setOverrideCursor(Qt::ArrowCursor);
    if (m_ZonesView) {
        if (CGUIView *view = dynamic_cast<CGUIView *>(m_ZonesView.data()))
            view->onSetMapNotesMode(checked);
    }
}

void XMasterWindow::onMeteoClicked(bool checked)
{
    if(checked)
        OpenDoc(XDocWidget::Runway);
    else
        CloseDoc(XDocWidget::Runway);
}

void XMasterWindow::_SetupMapNote()
{
    QPointF coord;

    QClipboard *clipboard = QApplication::clipboard();
    if (!clipboard->mimeData() || !clipboard->mimeData()->hasText())
    {
        QMessageBox::information(this, tr("Add Map Note"), tr("Unknown data in the clipboard"));
    }

    QString textClp = clipboard->text();
    QTextStream stream(&textClp);
    while ( !stream.atEnd() )
    {
        QString lat, lon;
        stream >> lat >> lon;
        if ( QTextStream::Ok != stream.status() )
            continue;

        coord = QPointF(lat.toDouble(), lon.toDouble());
        break;
    }

    MultiLineInputDialog dialog(this);
    dialog.setWindowTitle(tr("Description"));
    if(dialog.exec())
    {
        const QString text = dialog.getText();

        if(m_ZonesView)
        {
            CGUIView *guiView = dynamic_cast<CGUIView *>(m_ZonesView.data());
            if ( guiView )
            {
                QSharedPointer<MapNote> pNote = guiView->GetMainFIR()->CreateNote();
                pNote->setText(text);
                CCoord coordinate(coord.y(), coord.x());
                pNote->setCoord(coordinate);
                guiView->GetMainFIR()->AddNote(pNote);

                guiView->Draw();
            }
        }
    }
}

void XMasterWindow::EditMapNote(quint32 id)
{
    if(m_ZonesView)
    {
        CGUIView *guiView = dynamic_cast<CGUIView *>(m_ZonesView.data());
        if ( guiView )
        {
            CView::TSceneList::iterator itScene = guiView->GetScenes().begin();
            for (; itScene != guiView->GetScenes().end(); ++itScene)
            {
                CGUIScene* pscene = dynamic_cast<CGUIScene*> (itScene->data());
                if (pscene)
                {
                    QSharedPointer<MapNote> pNote = pscene->GetNotes().take(id);

                    if(pNote)
                    {
                        pNote->setID(pNote->id());
                        pNote->setCoord(pNote->coord());
                        MultiLineInputDialog dialog(this);
                        dialog.setWindowTitle(tr("Description"));
                        dialog.setText(pNote->text());
                        if(dialog.exec())
                            pNote->setText(dialog.getText());
                        else
                            pNote->setText(pNote->text());
                        pscene->GetNotes().insert(id,pNote);
                    }
                }
            }
            guiView->Draw();
        }
    }
}

bool XMasterWindow::getConnect()
{
    if(connectPostgresTrue && connectRedisTrue)
        return true;
    return false;
}


void XMasterWindow::SceneClean()
{
    CListAC::StopFutureRefresh();

    if ( dlgLayout )
    {
        dlgLayout->close();
        dlgLayout->deleteLater();
        dlgLayout = 0;
    }

    //CloseDoc(XDocWidget::UserPicsList);
    for(int i = XDocWidget::ListACC; i <= XDocWidget::ListCost; i++)
    {
        XDocWidget::DocType type = static_cast<XDocWidget::DocType>(i);

        if(type != XDocWidget::CtrlWindow)
        {
            CloseDoc(type);
        }
    }
    while (1 != m_mapDoc.size())
        CloseDoc(m_mapDoc.begin().key());

    removeToolBar(m_MainBar);
    removeToolBar(m_pPressureBar);
    removeToolBar(airplaneLengthBar);
    removeToolBar(m_pCustomSectorBar);
    removeToolBar(m_pAltitudeFilterBar);
    removeToolBar(m_pRestrictionBar);
    removeToolBar(m_pGroundBar);

    if(m_ZonesView)
    {
        CGUIView *guiView = dynamic_cast<CGUIView *>(m_ZonesView.data());
        if ( guiView )
            saveToDB(*guiView->GetMainFIR());
    }

    m_ZonesView.clear();
    SetPlanes(0);

    // Clear fir list in the restriction editor.
    if (gWidgetRestriction)
        gWidgetRestriction->removeFirs();

    m_pStatusLabel->clear();
    m_pStatusTempText->clear();
    setWindowTitle(tr("XMaster"));
}

void XMasterWindow::_Font()
{
    // need dialog with CXFont types
    bool ok;
    QFont font = QFontDialog::getFont(&ok, AppSettings::GetXFont(AppSettings::FONT_CTRL_DLG_LIST), this);
    if (ok)
    {
        _SetChangedFont(font);
    }
}

void XMasterWindow::_FormularEditor()
{
    CXLabelConstructor dlg(this);
    dlg.exec();
    if(GetView())
    {
        CGUIView *guiView = dynamic_cast<CGUIView *>(GetView().data());
        if ( guiView )
            guiView->UpdateFormularType();
    }
}

void XMasterWindow::_BearingEditor()
{
    CBearingEditor dlg(this);
    dlg.exec();
}

void XMasterWindow::_FPLsEditor()
{
    if (!m_mapDoc.contains(XDocWidget::FPLsList))
        OpenDoc(XDocWidget::FPLsList);
    else
        CloseDoc(XDocWidget::FPLsList);
}

void XMasterWindow::_StandEditor()
{
    if (!m_mapDoc.contains(XDocWidget::StandList)) {
        OpenDoc(XDocWidget::StandList);
        QMap<XDocWidget::DocType, QSharedPointer<QDockWidget> >::iterator itDoc = m_mapDoc.find(XDocWidget::StandList);
        if (itDoc != m_mapDoc.end()) {
            if (StandWidget *pStandWidget = qobject_cast<StandWidget*>((*itDoc)->widget()))
                pStandWidget->SetModelData(m_mapGUIStand);
        }
    } else {
        CloseDoc(XDocWidget::StandList);
    }
}

void XMasterWindow::_UserPicsEditor()
{
    if (!m_mapDoc.contains(XDocWidget::UserPicsList)) {
        OpenDoc(XDocWidget::UserPicsList);
        QMap<XDocWidget::DocType, QSharedPointer<QDockWidget> >::iterator itDoc = m_mapDoc.find(XDocWidget::UserPicsList);
        if (itDoc != m_mapDoc.end())
        {
            if (UserPicsWidget *pUserPicsWidget = qobject_cast<UserPicsWidget*>((*itDoc)->widget()))
            {
                QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(GetView());
                if(view && view->GetMainFIR())
                {
                    CGUIView *guiView = dynamic_cast<CGUIView *>(view.data());
                    if(guiView)
                    {
                        connect((*itDoc).data(), SIGNAL(getPickCoord(bool, QUuid)), guiView, SLOT(onGetPickCoord(bool, QUuid)));
                        connect(guiView, SIGNAL(pickCoord(QUuid)), (*itDoc).data(), SLOT(OnPickCoord(QUuid)));

                        connect(pUserPicsWidget, SIGNAL(userPicsChanged()), guiView, SLOT(onUserPicsChanged()), Qt::QueuedConnection);
                        connect(pUserPicsWidget, SIGNAL(userPicsSelectChanged(int, int, bool)), guiView, SLOT(userPicsSelectChanged(int, int, bool)), Qt::QueuedConnection);
                        connect(pUserPicsWidget, SIGNAL(userPicsVisabilityChanged()), guiView, SLOT(onUserPicsVisabilityChanged()), Qt::QueuedConnection);
                        connect(pUserPicsWidget, SIGNAL(userPicsSelectionDrop()), guiView, SLOT(userPicsSelectionDrop()), Qt::QueuedConnection);
                    }
                    pUserPicsWidget->SetModelData(currentUserName, view->GetMainFIR()->GetID());
                }
            }
        }
    } else {
        CloseDoc(XDocWidget::UserPicsList);
    }
}

void XMasterWindow::_GroundRadioRestriction(bool show)
{
    if(m_ZonesView)
    {
        GUIGroundScene *guiGroundView = dynamic_cast<GUIGroundScene *>(m_ZonesView.data());
        if ( guiGroundView )
            guiGroundView->DisplayRadioRestrictions(show);
    }
}

void XMasterWindow::_PickCoordinates()
{
    if(m_ZonesView)
    {
        CGUIView *guiView = dynamic_cast<CGUIView *>(m_ZonesView.data());
        if ( guiView )
            guiView->startCoordsPicking();
        GUIGroundScene *guiGroundView = dynamic_cast<GUIGroundScene *>(m_ZonesView.data());
        if ( guiGroundView )
            guiGroundView->StartCoordsPicking();
    }
}

void XMasterWindow::_OnPlanListsEditor()
{
    CGUIListConstructor dlg(this);
    dlg.setUsername(currentUserName);
    dlg.exec();
    QString sResult = dlg.sResult;
    if(sResult != QString("none")){
        CListAC::GetSourceModel().UpdateCustomColors();
        updatePlanList(sResult);
    }
}

void XMasterWindow::CloseDoc(XDocWidget::DocType type)
{
    if (XDocWidget::CtrlWindow == type)
        return;
    if (m_mapDoc.contains(type)) {
        removeDockWidget(m_mapDoc.value(type).data());
        m_mapDoc.remove(type);
    }
}

void XMasterWindow::OpenDoc(XDocWidget::DocType type)
{
    QMap<XDocWidget::DocType, QSharedPointer<QDockWidget> >::iterator itDoc = m_mapDoc.find(type);
    if (itDoc == m_mapDoc.end())
    {
        switch (type)
        {
        case XDocWidget::MeteoRunwayInfo:
        {
            QStringList airports;
            GetSectorAirports(&airports);

            itDoc = m_mapDoc.insert(type, QSharedPointer<QDockWidget> (new XDocWidget(type, this)));
            (*itDoc)->setObjectName("CXDocWidget" + QString::number(type));
            if (m_pRunwayWidgetToggle)
                connect(m_pRunwayWidgetToggle, SIGNAL(activated()), (*itDoc).data(), SLOT(OnRunwayMinimizeToggle()));
            addDockWidget(Qt::TopDockWidgetArea, itDoc->data());
            (*itDoc)->setFloating(true);
            (*itDoc)->setAllowedAreas(Qt::NoDockWidgetArea);
            (*itDoc)->setVisible(true);
            (*itDoc)->installEventFilter(docHandler);//Pasha
            connect((*itDoc).data(), SIGNAL(UpdateMeteoToMaster()), this, SLOT(onUpdateMeteoToMaster()));
            break;
        }
        case XDocWidget::MeteoRunwayInfo2:
        {
            QStringList airports;
            GetSectorAirports(&airports);

            itDoc = m_mapDoc.insert(type, QSharedPointer<QDockWidget> (new XDocWidget(type, this)));
            (*itDoc)->setObjectName("CXDocWidget" + QString::number(type));
            if (m_pRunwayWidgetToggle)
                connect(m_pRunwayWidgetToggle, SIGNAL(activated()), (*itDoc).data(), SLOT(OnRunwayMinimizeToggle()));
            addDockWidget(Qt::TopDockWidgetArea, itDoc->data());
            (*itDoc)->setFloating(true);
            (*itDoc)->setAllowedAreas(Qt::NoDockWidgetArea);
            (*itDoc)->setVisible(true);
            (*itDoc)->installEventFilter(docHandler);//Pasha
            connect((*itDoc).data(), SIGNAL(UpdateMeteoToMaster()), this, SLOT(onUpdateMeteoToMaster()));
            break;
        }
        /*case XDocWidget::Runway:
        {
            QStringList airports;
            GetSectorAirports(&airports);
            //itDoc = m_mapDoc.insertMulti(type, QSharedPointer<QDockWidget> (new XDocWidget(type, this, 0, 0, airport)));
            itDoc = m_mapDoc.insert(type, QSharedPointer<QDockWidget> (new XDocWidget(type, this)));
                (*itDoc)->setObjectName("CXDocWidget" + QString::number(type));
                if (m_pRunwayWidgetToggle)
                    connect(m_pRunwayWidgetToggle, SIGNAL(activated()), (*itDoc).data(), SLOT(OnRunwayMinimizeToggle()));

                addDockWidget(Qt::TopDockWidgetArea, itDoc->data());
                (*itDoc)->setFloating(true);
                (*itDoc)->setAllowedAreas(Qt::NoDockWidgetArea);
                (*itDoc)->setVisible(true);

                (*itDoc)->installEventFilter(docHandler);//Pasha
            //}
            break;
        }*/
        case XDocWidget::Runway:
        {
            XDocWidget *rw = new XDocWidget(type, privileges, this);
            itDoc = m_mapDoc.insert(type, QSharedPointer<QDockWidget> (rw));
            (*itDoc)->setObjectName("CXDocWidget" + QString::number(type));
            addDockWidget(Qt::TopDockWidgetArea, itDoc->data());
            (*itDoc)->setFloating(true);
            (*itDoc)->setAllowedAreas(Qt::NoDockWidgetArea);
            (*itDoc)->setVisible(true);
            (*itDoc)->installEventFilter(docHandler);//Pasha

            connect(this, SIGNAL(UpdateMeteo()), rw , SIGNAL(UpdateMeteo()));
            break;
        }
        case (XDocWidget::ListActDep):
        {
            XDocWidget *rw = new XDocWidget(type, lst_orcamrange, this);
            itDoc = m_mapDoc.insert(type, QSharedPointer<QDockWidget> (rw));
            (*itDoc)->setObjectName("CXDocWidget" + QString::number(type));

            addDockWidget(Qt::TopDockWidgetArea, itDoc->data());
            (*itDoc)->setFloating(true);
            (*itDoc)->setAllowedAreas(Qt::NoDockWidgetArea);
            (*itDoc)->setVisible(true);

            (*itDoc)->installEventFilter(docHandler);//Pasha
            break;
        }
        case (XDocWidget::ListDailyPlan):
        {
            XDocWidget *rw = new XDocWidget(type, lst_orcamrange, this);
            itDoc = m_mapDoc.insert(type, QSharedPointer<QDockWidget> (rw));
            (*itDoc)->setObjectName("CXDocWidget" + QString::number(type));

            addDockWidget(Qt::TopDockWidgetArea, itDoc->data());
            (*itDoc)->setFloating(true);
            (*itDoc)->setAllowedAreas(Qt::NoDockWidgetArea);
            (*itDoc)->setVisible(true);

            (*itDoc)->installEventFilter(docHandler);//Pasha
            break;
        }
        default:
            itDoc = m_mapDoc.insert(type, QSharedPointer<QDockWidget> (new XDocWidget(type, this)));
            //(*itDoc)->installEventFilter(docHandler);
            (*itDoc)->setObjectName("CXDocWidget" + QString::number(type));

            addDockWidget(Qt::TopDockWidgetArea, itDoc->data());
            (*itDoc)->setFloating(true);
            (*itDoc)->setAllowedAreas(Qt::NoDockWidgetArea);
            (*itDoc)->setVisible(true);

            (*itDoc)->installEventFilter(docHandler);//Pasha
            break;
        }
        QEvent eve_m(QEvent::Move),eve_mr(QEvent::MouseButtonRelease) ;
        QApplication::sendEvent(itDoc->data(),&eve_m);
        QApplication::sendEvent(itDoc->data(),&eve_mr);
        qApp->processEvents();
    }
}

void XMasterWindow::_InitSectoringCombo()
{
    m_SectoringBar = new QToolBar();
    //m_SectoringBar->setAllowedAreas(Qt::TopToolBarArea);

    // Sectoring
    m_pSectoringCombo = new QComboBox(this);
    m_pSectoringCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_pSectoringModel = new QSqlTableModel();
    m_pSectoringModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_pSectoringModel->setTable("vw_sectoring");
    m_pSectoringModel->setFilter(QString("id IN "
                                         "(SELECT client_sectoring.sectoring_id "
                                         "FROM client_sectoring "
                                         "WHERE client_sectoring.client_role_id = %1) "
                                         "OR id = 0 OR id = -1").arg(mUser->GetRole()));
    m_pSectoringCombo->setModel(m_pSectoringModel);
    m_pSectoringCombo->setModelColumn(m_pSectoringModel->fieldIndex("name"));
    control_window_->setSectorModel(m_pSectoringModel, m_pSectoringModel->fieldIndex("name"));
    m_pSectoringModel->select();

    m_SectorTypeModel = new QSqlQueryModel();
    m_SectorTypeModel->setQuery("SELECT s.id, t.app  FROM sectoring as s LEFT JOIN sectoring_type as t ON s.type_id = t.id");

    _SelectSectorModel();

    //connect(m_pSectoringCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(_OnSectoringCombo(int)));
    connect(control_window_, SIGNAL(sectorComboIndexChanged(int)), this, SLOT(_OnSectoringCombo(int)));

    // Login/out
    //m_pLogOutAction = new QAction(QIcon(":/Icons/Logout"), "Logout", this);
    m_pLogOutAction = new QAction(tr("Logout"), this);
    connect(m_pLogOutAction, SIGNAL(triggered()), this, SLOT(_OnLogout()));
    connect(control_window_, SIGNAL(logOffButtonClicked()), m_pLogOutAction, SIGNAL(triggered()));

    // User full name
    m_pUserFullNameLabel = new QLabel(this);
    QFont UFNFont = m_pUserFullNameLabel->font();
    UFNFont.setPointSize(UFNFont.pointSize() * 1.2);
    UFNFont.setBold(true);
    m_pUserFullNameLabel->setFont(UFNFont);
    m_pUserFullNameLabel->setText(mUser->GetFullName());
    control_window_->setCurrentUser(mUser->GetFullName());

    m_pViewCWP = new QAction(tr("CWP"), this);
    connect(m_pViewCWP, SIGNAL(triggered()), this, SLOT(_OnViewCWP()));
    //connect(control_window_, SIGNAL(cwpButtonClicked()), m_pViewCWP, SIGNAL(triggered()));

    m_pFocus = new QToolButton(this);
    m_pFocus->setText(tr("Focus"));
    m_pFocus->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_pFocus->setPopupMode(QToolButton::InstantPopup);
    m_pFocus->setStyleSheet(
        " QToolButton { "
        "padding-right: 20px; } "
        " QToolButton::menu-indicator { "
        "subcontrol-origin: padding; "
        "subcontrol-position: center right; }"
        );
    QMenu* pSetMenu = new QMenu(this);
    m_pFocusSave = new QAction(tr("Focus save"), this);
    m_pFocusSave->setShortcut(Qt::Key_F10);
    connect(m_pFocusSave, SIGNAL(triggered()), this, SLOT(_OnSaveFocus()));
    pSetMenu->addAction(m_pFocusSave);
    pref_set_menu_->addAction(m_pFocusSave);
    /*m_pFormularRestore = new QAction(tr("Restore formular"), this);
    m_pFormularRestore->setShortcut(QKeySequence(m_Shortcuts["RestoreLabels"]));
    connect(m_pFormularRestore, SIGNAL(triggered(bool)), this, SLOT(_OnRestoreFormular(bool)));
    pSetMenu->addAction(m_pFormularRestore);
    pref_set_menu_->addAction(m_pFormularRestore);*/
    m_pFocusRestore = new QAction(tr("Focus restore"), this);
    m_pFocusRestore->setShortcut(Qt::Key_F11);
    connect(m_pFocusRestore, SIGNAL(triggered()), this, SLOT(_OnRestoreFocus()));
    //connect(control_window_, SIGNAL(normalButtonClicked()), this, SLOT(_OnRestoreFocus()));
    connect(control_window_, SIGNAL(normalButtonClicked()), this, SLOT(_OnRestoreFocusCentralPoint()));
    pSetMenu->addAction(m_pFocusRestore);
    pref_set_menu_->addAction(m_pFocusRestore);
    m_pFocus->setMenu(pSetMenu);
    m_pFocus->addAction(pSetMenu->menuAction());
    m_pFocus->setDefaultAction(pSetMenu->menuAction());

    m_SectoringBar->addWidget(m_pUserFullNameLabel);
    m_SectoringBar->addAction(m_pLogOutAction);
    m_SectoringBar->addWidget(m_pSectoringCombo);
    //m_SectoringBar->addSeparator();
    m_SectoringBar->addAction(m_pViewCWP);
    m_SectoringBar->addWidget(m_pFocus);
//    addToolBar(m_SectoringBar);
    //m_SectoringBar->setVisible(true);
}

void XMasterWindow::_InitMainCombo()
{
    m_MainBar = new QToolBar();
    //m_MainBar->setAllowedAreas(Qt::TopToolBarArea);

    m_ZoomSlider = new QSlider(Qt::Horizontal, this);
    m_ZoomSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_iSlider = 0;
    m_ZoomSlider->setPageStep(5);
    m_ZoomSlider->setMinimum(0);
    m_ZoomSlider->setMaximum(CGUIView::MAX_SCALE);
    control_window_->setZoomRange(0, CGUIView::MAX_SCALE);
    connect(m_ZoomSlider, SIGNAL(valueChanged(int)), this, SLOT(_ZoomSlider(int)));
    connect(control_window_, SIGNAL(zoomChanged(int)), this, SLOT(_ZoomSlider(int)));

    m_CheckGauge = new QCheckBox(this);
    m_CheckGauge->setText(tr("Gauge"));
    m_CheckGauge->setCheckState(Qt::Unchecked);
    //control_window_->setGaugeCheckState(Qt::Unchecked);
    //connect(m_CheckGauge, SIGNAL(stateChanged(int)), this, SLOT(_OnCheckGauge(int)));
    //connect(control_window_, SIGNAL(gaugeStateChanged(int)), this, SLOT(_OnCheckGauge(int)));

    /*m_AutoCollapse = new QAction(tr("Auto collapse"), this);
    m_AutoCollapse->setCheckable(true);
    connect(m_AutoCollapse, SIGNAL(triggered(bool)), this, SLOT(_OnCheckAutoCollapse(bool)));
    if(networkSettings()->value("airplane/auto_collapse", "yes").toString() == "yes")
        m_AutoCollapse->setChecked(true);
    else
        m_AutoCollapse->setChecked(false);*/

    m_CorridorRoute = new QAction(tr("Route corridors"), this);
    m_CorridorRoute->setShortcut(tr("Ctrl+R"));
    m_CorridorRoute->setCheckable(true);
    const bool show_routes = networkSettings()->value("users/show_routes", "no").toString() == "yes";
    control_window_->onRoutesClicked(show_routes);
    const bool show_corridors = networkSettings()->value("users/corridor_route", "no").toString() == "yes";
    m_CorridorRoute->setChecked(show_corridors);
    connect(m_CorridorRoute, SIGNAL(triggered(bool)), this, SLOT(_OnCheckCorridorRoute(bool)));
    connect(control_window_, SIGNAL(routesClicked(bool)), this, SLOT(onRoutesClicked(bool)));


    m_PointZoom = new QAction(tr("pzoom"), this);
    m_PointZoom->setCheckable(true);
    const bool point_zoom = networkSettings()->value("users/point_zoom", "no").toString() == "yes";
    m_PointZoom->setChecked(point_zoom);
    connect(m_PointZoom, SIGNAL(triggered(bool)), this, SLOT(onPointZoomClicked(bool)));

//    m_UserPicsEditor = new QAction("User pics", this);
//    connect(m_UserPicsEditor, SIGNAL(triggered()), this, SLOT(_UserPicsEditor()));
    connect(control_window_, SIGNAL(freqButtonClicked(bool)), this, SLOT(_UserPicsEditor()));

    m_LimitLength = new QAction(this);
    m_LimitLength->setCheckable(true);

    m_FlightLevelMonitoring = new QAction(tr("Flight level monitoring"), this);
    m_FlightLevelMonitoring->setCheckable(true);
    const bool flight_level_monitoring = networkSettings()->value("users/flight_level_monitoring", "no").toString() == "yes";
    m_FlightLevelMonitoring->setChecked(flight_level_monitoring);
    connect(m_FlightLevelMonitoring, SIGNAL(triggered(bool)), this, SLOT(_OnCheckFlightLevelMonitoring(bool)));

    if (networkSettings()->value("users/draw_points", "yes").toString() == "yes")
        control_window_->onPointsClicked(true);
    else
        control_window_->onPointsClicked(false);
    if (networkSettings()->value("users/draw_Borderpoints", "yes").toString() == "yes")
        control_window_->onBPointsClicked(true);
    else
        control_window_->onBPointsClicked(false);

    connect(control_window_, SIGNAL(pointsClicked(bool)), this, SLOT(onCheckPoints(bool)));
    connect(control_window_, SIGNAL(BpointsClicked(bool)), this, SLOT(onCheckBPoints(bool)));

    if (networkSettings()->value("users/sector_border", "yes").toString() == "yes")
        control_window_->onSectorsClicked(true);
    else
        control_window_->onSectorsClicked(false);

    connect(control_window_, SIGNAL(sectorsClicked(bool)), this, SLOT(_OnCheckShowSectorBorder(bool)));

    /*m_ShowRouteOnAssume = new QAction(tr("Show route on assume"), this);
    m_ShowRouteOnAssume->setCheckable(true);
    if(networkSettings()->value("users/show_route_on_assume", "yes").toString() == "yes")
        m_ShowRouteOnAssume->setChecked(true);
    else
        m_ShowRouteOnAssume->setChecked(false);
    connect(m_ShowRouteOnAssume, SIGNAL(triggered(bool)), this, SLOT(_OnCheckShowRouteOnAssume(bool)));*/

    // SI
    m_SICombo_M = new QComboBox(this);
    m_SICombo_M->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_SICombo_M->addItem(tr("meters"), 0);
    m_SICombo_M->addItem(tr("feet"), 1);
    connect(m_SICombo_M, SIGNAL(currentIndexChanged(int)), this, SLOT(_OnChangeSI_M(int)));
    connect(control_window_, SIGNAL(heightCurrentIndexChanged(int)), this, SLOT(_OnChangeSI_M(int)));
    m_SICombo_K = new QComboBox(this);
    m_SICombo_K->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_SICombo_K->addItem(tr("km/h"), 0);
    m_SICombo_K->addItem(tr("knots"), 1);
    connect(m_SICombo_K, SIGNAL(currentIndexChanged(int)), this, SLOT(_OnChangeSI_K(int)));
    connect(control_window_, SIGNAL(speedCurrentIndexChanged(int)), this, SLOT(_OnChangeSI_K(int)));

    //m_LayoutsAction = new QAction(tr("Layers"), this);
    //connect(m_LayoutsAction, SIGNAL(triggered()), this, SLOT(_OnBtnLayouts()));

    m_LayoutsMainFIRAction = new QAction(tr("Layers"), this);
    connect(m_LayoutsMainFIRAction, SIGNAL(triggered()), this, SLOT(_OnBtnLayoutsMainFIR()));
    connect(control_window_, SIGNAL(layersTriggered()), this, SLOT(_OnBtnLayoutsMainFIR()));

    m_MapNoteAction = new QAction(tr("Add Map Note..."), this);
    connect(m_MapNoteAction, SIGNAL(triggered()), this, SLOT(_OnBtnAddMapNote()));
    //connect(control_window_, SIGNAL(addMapNoteTriggered()), this, SLOT(_OnBtnAddMapNote()));

    //m_RcGridCoord = new QAction(tr("RcGrid center"), this);
    //connect(m_RcGridCoord, SIGNAL(triggered()), this, SLOT(_OnRcGridCoord()));

    //m_DF = new QAction(tr("DF", "direction finders"), this);
    //connect(m_DF, SIGNAL(triggered()), this, SLOT(_OnDF()));

    //m_pFontAction = new QAction(QIcon(":/Icons/FontA"), "Change font size", this);
    m_pFontAAsmall = new QAction(tr("small"), this);
   // m_pFontAAsmall->setShortcut(tr("Ctrl+S"));
    m_pFontAAsmall->setCheckable(true);
    connect(m_pFontAAsmall, SIGNAL(triggered()), this, SLOT(_OnFontAAsmall()));
    m_pFontAAmedium = new QAction(tr("medium"), this);
   // m_pFontAAmedium->setShortcut(tr("Ctrl+M"));
    m_pFontAAmedium->setCheckable(true);
    connect(m_pFontAAmedium, SIGNAL(triggered()), this, SLOT(_OnFontAAmedium()));
    m_pFontAAlarge = new QAction(tr("big"), this);
   // m_pFontAAlarge->setShortcut(tr("Ctrl+B"));
    m_pFontAAlarge->setCheckable(true);
    connect(m_pFontAAlarge, SIGNAL(triggered()), this, SLOT(_OnFontAAlarge()));
    m_pFontAAMenu = new QMenu(tr("Change font size"), this);
    QActionGroup* pFontAAGroup = new QActionGroup(this);
    pFontAAGroup->addAction(m_pFontAAsmall);
    m_pFontAAMenu->addAction(m_pFontAAsmall);
    pFontAAGroup->addAction(m_pFontAAmedium);
    m_pFontAAMenu->addAction(m_pFontAAmedium);
    pFontAAGroup->addAction(m_pFontAAlarge);
    m_pFontAAMenu->addAction(m_pFontAAlarge);
    switch (GetTextSize().GetCurrentSize())
    {
    case CTextSize::Small:
        m_pFontAAsmall->setChecked(true);
        break;
    case CTextSize::Medium:
        m_pFontAAmedium->setChecked(true);
        break;
    case CTextSize::Large:
        m_pFontAAlarge->setChecked(true);
        break;
    }

    toolbox_vertical_ = new QAction(tr("vertical"), this);
    toolbox_vertical_->setCheckable(true);
    connect(toolbox_vertical_, SIGNAL(triggered()), this, SLOT(onToolboxVerticalOrientation()));
    toolbox_horizontal_ = new QAction(tr("horizontal"), this);
    toolbox_horizontal_->setCheckable(true);
    connect(toolbox_horizontal_, SIGNAL(triggered()), this, SLOT(onToolboxHorizontalOrientation()));
    toolbox_orientation_menu_ = new QMenu(tr("Control Window orientation"), this);
    QActionGroup *toolbox_orientation_group = new QActionGroup(this);

    toolbox_orientation_group->addAction(toolbox_vertical_);
    toolbox_orientation_menu_->addAction(toolbox_vertical_);
    toolbox_orientation_group->addAction(toolbox_horizontal_);
    toolbox_orientation_menu_->addAction(toolbox_horizontal_);
    toolbox_horizontal_->setChecked(true);

    toolbox_Old_SpeedDistance_measure = new QAction(tr("ACC"), this);
    toolbox_Old_SpeedDistance_measure->setCheckable(true);
    connect(toolbox_Old_SpeedDistance_measure, SIGNAL(changed()), this, SLOT(onToolboxOldSpeedDistanceMeasure()));
    toolbox_New_SpeedDistance_measure = new QAction(tr("TMA"), this);
    toolbox_New_SpeedDistance_measure->setCheckable(true);
    connect(toolbox_New_SpeedDistance_measure, SIGNAL(changed()), this, SLOT(onToolboxNewSpeedDistanceMeasure()));
    toolbox_speed_distance_variants_menu = new QMenu(tr("Speed, Distance vectors view"), this);
    QActionGroup *toolbox_SpeedDistance_measure_group = new QActionGroup(this);
    toolbox_SpeedDistance_measure_group->addAction(toolbox_Old_SpeedDistance_measure);
    toolbox_speed_distance_variants_menu->addAction(toolbox_Old_SpeedDistance_measure);
    toolbox_SpeedDistance_measure_group->addAction(toolbox_New_SpeedDistance_measure);
    toolbox_speed_distance_variants_menu->addAction(toolbox_New_SpeedDistance_measure);
    if(networkSettings()->value("airplane/speed_distance_menu_type", "ACC") == "ACC")
    {
        toolbox_Old_SpeedDistance_measure->setChecked(true);
    }
    else
    {
        toolbox_New_SpeedDistance_measure->setChecked(true);
    }
    connect(control_window_, SIGNAL(vectorMeasurementButtonClicked(bool)), this, SLOT(_OnCheckGauge(bool)));


    m_SettingMenu = new QToolButton(this);
    m_SettingMenu->setText(tr("Settings"));
    m_SettingMenu->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_SettingMenu->setPopupMode(QToolButton::InstantPopup);
    m_SettingMenu->setStyleSheet(
        " QToolButton { "
        "padding-right: 20px; } "
        " QToolButton::menu-indicator { "
        "subcontrol-origin: padding; "
        "subcontrol-position: center right; }"
        );
    QMenu *pSetMenu = new QMenu(this);
    m_SettingMenu->setMenu(pSetMenu);
    m_SettingMenu->addAction(pSetMenu->menuAction());
    m_SettingMenu->setDefaultAction(pSetMenu->menuAction());
    control_window_->setSettingsMenu(pref_set_menu_);

    m_pHistoryPointMenu = createHistoryPointMenu();

    //pSetMenu->addAction(m_AutoCollapse);
    pSetMenu->addAction(m_CorridorRoute);
    //pSetMenu->addAction(m_ShowSectorBorder);
    //pSetMenu->addAction(m_ShowRouteOnAssume);
    pSetMenu->addMenu(m_pFontAAMenu);
    //pSetMenu->addAction(m_LayoutsAction);
    //pSetMenu->addAction(m_RcGridCoord);
    //pSetMenu->addAction(m_DF);

    //pref_set_menu_->addSeparator();
    //pref_set_menu_->addAction(m_AutoCollapse);
    pref_set_menu_->addAction(m_CorridorRoute);
    pref_set_menu_->addAction(m_PointZoom);
    pref_set_menu_->addAction(m_LimitLength);
    pref_set_menu_->addAction(m_FlightLevelMonitoring);
    //pref_set_menu_->addAction(m_ShowSectorBorder);
    //pref_set_menu_->addAction(m_ShowRouteOnAssume);
    pref_set_menu_->addMenu(m_pFontAAMenu);
    //pref_set_menu_->addAction(m_LayoutsAction);
    //pref_set_menu_->addAction(m_RcGridCoord);
    //pref_set_menu_->addAction(m_DF);
    pref_set_menu_->addMenu(toolbox_orientation_menu_);
    pref_set_menu_->addMenu(toolbox_speed_distance_variants_menu);
    if ( m_pHistoryPointMenu )
        pref_set_menu_->addMenu(m_pHistoryPointMenu);

    m_MainBar->addWidget(m_ZoomSlider);
    m_MainBar->addWidget(m_SICombo_M);
    m_MainBar->addWidget(m_SICombo_K);
    m_MainBar->addSeparator();
    m_MainBar->addWidget(m_CheckGauge);
    m_MainBar->addAction(m_LayoutsMainFIRAction);
    m_MainBar->addAction(m_MapNoteAction);
    m_MainBar->addWidget(m_SettingMenu);

    //m_MainBar->addAction(m_UserPicsEditor);

    //connect(control_window_, SIGNAL(routesClicked(bool)), this, SLOT(onRoutesClicked(bool)));
    connect(control_window_, SIGNAL(HidePlainsClicked(bool)), this, SLOT(onHidePlainsClicked(bool)));//

}

void XMasterWindow::_InitPressureCombo()
{
    m_pPressureBar = new QToolBar();
    //m_pPressureBar->setAllowedAreas(Qt::TopToolBarArea);

    m_pEchelonLabel = new QLabel(tr("TL"), this);
    m_pEchelonBut = new QToolButton(this);
    m_pEchelonBut->setText("");
    m_pEchelonBut->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_pEchelonBut->setPopupMode(QToolButton::InstantPopup);
    m_pEchelonBut->setStyleSheet(
            " QToolButton { "
            "border: 1px solid yellow; "
            "border-radius: 2px; "
            "background-color: yellow; "
            "padding-right: 10px; } "
            " QToolButton::menu-indicator { "
            "subcontrol-origin: padding; "
            "subcontrol-position: center right; }"
            );
    QMenu* pSetMenu = 0;
    if(!networkSettings()->value("users/readonly_TL", false).toBool())
    {
        pSetMenu = new QMenu(this);
        m_pEchelonBut->setMenu(pSetMenu);
        m_pEchelonBut->addAction(pSetMenu->menuAction());
        m_pEchelonBut->setDefaultAction(pSetMenu->menuAction());
        //control_window_->setFlightLevelMenu(pSetMenu);
    }
    m_EchelonCombo = new QComboBox(this);
    //m_EchelonCombo->addItem("090", 30);
    m_EchelonCombo->addItem("90", 30);
    m_EchelonCombo->addItem("120", 40);
    m_EchelonCombo->addItem("150", 50);
    m_EchelonCombo->addItem("180", 60);
    m_EchelonCombo->addItem("210", 70);
    m_EchelonCombo->addItem("240", 80);
    m_EchelonCombo->addItem("270", 90);
    m_EchelonCombo->addItem("300", 100);
    m_EchelonCombo->addItem("330", 110);
    m_EchelonCombo->addItem("360", 120);
    m_EchelonCombo->setEditable(true);
    QIntValidator* pIntValidator = new QIntValidator(this);
    pIntValidator->setRange(0, 999);
    m_EchelonCombo->setValidator(pIntValidator);
    m_EchelonCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    connect(m_EchelonCombo, SIGNAL(activated(int)), this, SLOT(_OnChangeEchelon(int)));
    //////////connect(m_EchelonCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(_OnChangeEchelon(int)));
    m_EchelonCombo->setCurrentIndex(5);
    _OnChangeEchelon(5);
    QWidgetAction* pWidgetAct = 0;
    if(!networkSettings()->value("users/readonly_TL", false).toBool())
    {
        pWidgetAct = new QWidgetAction(this);
        pWidgetAct->setDefaultWidget(m_EchelonCombo);
        pSetMenu->addAction(pWidgetAct);
        connect(pSetMenu, SIGNAL(aboutToShow()), this, SLOT(OnShowToolButMenu()));
    }
    else
    {
        m_EchelonCombo->hide();
    }

    //m_pPressLabel = new QLabel(tr("QFE"), this);
    m_pPressLabel = new QLabel(tr("QNH(hPa)"), this);
    m_pPressBut = new QToolButton(this);
    m_pPressBut->setText("");
    m_pPressBut->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_pPressBut->setPopupMode(QToolButton::InstantPopup);
    m_pPressBut->setStyleSheet(
            " QToolButton { "
            "border: 1px solid yellow; "
            "border-radius: 2px; "
            "background-color: yellow; "
            "padding-right: 10px; } "
            " QToolButton::menu-indicator { "
            "subcontrol-origin: padding; "
            "subcontrol-position: center right; }"
            );
    if(!networkSettings()->value("users/readonly_QFE", false).toBool())
    {
        pSetMenu = new QMenu(this);
        m_pPressBut->setMenu(pSetMenu);
        m_pPressBut->addAction(pSetMenu->menuAction());
        m_pPressBut->setDefaultAction(pSetMenu->menuAction());
        //control_window_->setPressureMenu(pSetMenu);
    }
    m_PressSpin = new QSpinBox(this);
    //m_PressSpin->setRange(100, 999);
    m_PressSpin->setRange(800, 1200);
    m_PressSpin->setSingleStep(1);
    connect(m_PressSpin, SIGNAL(valueChanged(int)), this, SLOT(_OnChangePress(int)));
    //m_PressSpin->setValue(760);
    m_PressSpin->setValue(1013);
    if(!networkSettings()->value("users/readonly_QFE", false).toBool())
    {
        pWidgetAct = new QWidgetAction(this);
        pWidgetAct->setDefaultWidget(m_PressSpin);
        pSetMenu->addAction(pWidgetAct);
    }
    else
    {
        m_PressSpin->hide();
    }

    m_pPressureBar->addWidget(m_pEchelonLabel);
    m_pPressureBar->addWidget(m_pEchelonBut);
    m_pPressureBar->addWidget(m_pPressLabel);
    m_pPressureBar->addWidget(m_pPressBut);
}

void XMasterWindow::_InitPlaneCombo()
{
    airplaneLengthBar = new QToolBar(tr("Airplane length"));
    //airplaneLengthBar->setAllowedAreas(Qt::TopToolBarArea);

    airplaneLength = new QSpinBox(this);
    airplaneLength->setMaximum(100);
    airplaneLength->setValue(networkSettings()->value("airplane/deflexion").toInt());

    airplaneLengthBar->addWidget(airplaneLength);

    airplaneLengthGroup = new QActionGroup(airplaneLengthBar);
    airplaneLengthGroup->setExclusive(true);

    airplaneLengthSec = new QAction(tr("Minutes"), airplaneLengthGroup);
    airplaneLengthSec->setCheckable(true);

    airplaneLengthKilometers = new QAction(tr("Kilometers"), airplaneLengthGroup);
    airplaneLengthKilometers->setCheckable(true);

    airplaneLengthBar->addActions(airplaneLengthGroup->actions());

    if (networkSettings()->value("airplane/deflexion_type").toString() == "sec") {
            airplaneLengthSec->setChecked(true);
            control_window_->setSpeedVectorValue(networkSettings()->value("airplane/deflexion_sec").toInt());
    } else if (networkSettings()->value("airplane/deflexion_type").toString() == "kilometer") {
             airplaneLengthKilometers->setChecked(true);
             control_window_->setDistanceVectorValue(networkSettings()->value("airplane/deflexion_kilometer").toInt());
    }
    airplaneLengthTypeChanged();

    connect(airplaneLength, SIGNAL(valueChanged(int)), this, SLOT(airplaneLengthChanged(int)));
    connect(airplaneLengthSec, SIGNAL(triggered()), this, SLOT(airplaneLengthTypeChanged()));
    connect(airplaneLengthKilometers, SIGNAL(triggered()), this, SLOT(airplaneLengthTypeChanged()));
    /*connect(control_window_, SIGNAL(deflexionValueChanged(int)), this, SLOT(airplaneLengthChanged(int)));
    connect(control_window_, SIGNAL(minutesTriggered()), this, SLOT(airplaneLengthTypeChanged()));
    connect(control_window_, SIGNAL(kilometersTriggered()), this, SLOT(airplaneLengthTypeChanged()));*/
    connect(control_window_, SIGNAL(speedClicked(int)), this, SLOT(speedVectorClicked(int)));
    connect(control_window_, SIGNAL(distanceClicked(int)), this, SLOT(distanceVectorClicked(int)));

    connect(control_window_, SIGNAL(angleClicked(int)), this, SLOT(onAngleClicked(int)));
    connect(control_window_, SIGNAL(autoAntiOverlapClicked(bool)), this, SLOT(onAutoAntiOverlapClicked(bool)));
}

void XMasterWindow::_InitCustomSector()
{
    m_pCustomSectorBar = new QToolBar();
    //m_pCustomSectorBar->setAllowedAreas(Qt::TopToolBarArea);

    m_pCustomSector = new QAction(tr("Sector"), this);
    connect(m_pCustomSector, SIGNAL(triggered()), this, SLOT(_OnCustomSector()));
    connect(m_pCustomSector, SIGNAL(hovered()), this, SLOT(OnToolTipCustomSector()));
    connect(control_window_, SIGNAL(sectorButtonClicked()), this, SLOT(_OnCustomSector()));

    m_pCustomSectorBar->addAction(m_pCustomSector);
}

void XMasterWindow::_InitAltitudeFilter()
{
    m_pAltitudeFilterBar = new QToolBar();
    //m_pAltitudeFilterBar->setAllowedAreas(Qt::TopToolBarArea);

    m_pSliderLabel = new QLabel(tr("Filter"), this);
    m_pSliderBut = new QToolButton(this);
    m_pSliderBut->setText("");
    m_pSliderBut->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_pSliderBut->setPopupMode(QToolButton::InstantPopup);
    m_pSliderBut->setStyleSheet(
            " QToolButton { "
            "border: 1px solid yellow; "
            "border-radius: 2px; "
            "background-color: yellow; "
            "padding-right: 10px; } "
            " QToolButton::menu-indicator { "
            "subcontrol-origin: padding; "
            "subcontrol-position: center right; }"
            );
    QMenu* pSetMenu = new QMenu(this);
    m_pSliderBut->setMenu(pSetMenu);
    m_pSliderBut->addAction(pSetMenu->menuAction());
    m_pSliderBut->setDefaultAction(pSetMenu->menuAction());
    //networkSettings()->setValue("airplane/low_FL", 0);
    //networkSettings()->setValue("airplane/high_FL", 500);
    m_FLSlider = new QxtSpanSlider(Qt::Horizontal, this);
    m_FLSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_FLSlider->setSingleStep(10);
    m_FLSlider->setPageStep(100);
    m_FLSlider->setMinimum(0);
    m_FLSlider->setMaximum(500);
    m_FLSlider->setTickPosition(QSlider::TicksBelow);
    m_FLSlider->setHandleMovementMode(QxtSpanSlider::NoCrossing);
    //m_FLSlider->setMouseTracking(true);
    connect(m_FLSlider, SIGNAL(spanChanged(int,int)), this, SLOT(_FLSlider(int,int)));
    connect(m_FLSlider, SIGNAL(lowerPositionChanged(int)), this, SLOT(onLowerPositionChanged(int)));
    connect(m_FLSlider, SIGNAL(upperPositionChanged(int)), this, SLOT(onUpperPositionChanged(int)));
    connect(control_window_, SIGNAL(heightSpanChanged(int, int)), this, SLOT(_FLSlider(int, int)));
    connect(control_window_, SIGNAL(heightLowerPositionChanged(int)), this, SLOT(onLowerPositionChanged(int)));
    connect(control_window_, SIGNAL(heightUpperPositionChanged(int)), this, SLOT(onUpperPositionChanged(int)));
    //m_FLSlider->installEventFilter(this);
    m_FLSlider->setSpan(networkSettings()->value("airplane/low_FL", 0).toInt(), networkSettings()->value("airplane/high_FL", 680).toInt());
    control_window_->setSpanRange(networkSettings()->value("airplane/low_FL", 0).toInt(), networkSettings()->value("airplane/high_FL", 680).toInt());
    QWidgetAction* pWidgetAct = new QWidgetAction(this);
    pWidgetAct->setDefaultWidget(m_FLSlider);
    pSetMenu->addAction(pWidgetAct);

    m_pAltitudeFilterBar->addWidget(m_pSliderLabel);
    m_pAltitudeFilterBar->addWidget(m_pSliderBut);
}

void XMasterWindow::_InitRestriction()
{
    m_pRestrictionBar = new QToolBar();

    m_pRestrictionAction = new QAction(QIcon(":/Images/AttentionIcon"), "", this);
    connect(m_pRestrictionAction, SIGNAL(triggered()), restrictions_widget_, SLOT(show()));
    connect(control_window_, SIGNAL(restrictionsButtonClicked(bool)), restrictions_widget_, SLOT(show()));

    m_pRestrictionBar->addAction(m_pRestrictionAction);
}

void XMasterWindow::_InitGroundBar()
{
    m_pGroundBar = new QToolBar();

    m_pGroundStandAction = new QAction(tr("Sector"), this);
    connect(m_pGroundStandAction, SIGNAL(triggered()), this, SLOT(_StandEditor()));

    m_pGroundBar->addAction(m_pGroundStandAction);

    m_pGroundRestrictionAction = new QAction(tr("ILS critical zone"), this);
    m_pGroundRestrictionAction->setCheckable(true);
    m_pGroundRestrictionAction->setChecked(true);
    connect(m_pGroundRestrictionAction, SIGNAL(toggled(bool)), this, SLOT(_GroundRadioRestriction(bool)));

    m_pGroundBar->addAction(m_pGroundRestrictionAction);
}

void XMasterWindow::_LoadFIR(uint fir_id)
{
    BlockNetworkMessages(true);
    SceneClean();
    QSharedPointer<CGUIView> newView(new CGUIView(this));

    newView->SetMaster(this);
    newView->SetReminder(m_pReminderDoc ? static_cast<XReminder*>(m_pReminderDoc->widget()) : 0);
    newView->SetEchelon(GetEchelonValue(IsSISet(AppSettings::SIFeet)));
    newView->SetPress(m_PressSpin->value());
    newView->SetGauge(m_CheckGauge->checkState() == Qt::Checked);

    CListAC::GetSourceModel().SetEchelon(GetEchelonValue(IsSISet(AppSettings::SIFeet)));
    CListAC::GetSourceModel().SetPress(m_PressSpin->value());

    QGraphicsScene * scene_fir = new QGraphicsScene(0, 0, WINDOW_SIZE, WINDOW_SIZE, newView.data());
    newView->setScene(scene_fir);

    CView::TScenePtr fir(new CGUIScene());
    loadFromDB(fir_id, *fir, *newView);

    mMainAirport.clear();
    foreach (const QSharedPointer<CAirport> &airport, fir->GetAirports().values())
    {
        if (airport->GetName() == networkSettings()->value("meteo/airport").toString())
            mMainAirport = airport;
    }

    newView->AddScene(fir->GetIDName(), fir);

    newView->setMinimumSize(WINDOW_SIZE + 2, WINDOW_SIZE + 2);
    newView->prepare();

    m_ZonesView = newView;

    QStringList sectors;
    foreach (const CView::TScenePtr &sector, fir->GetSectorList()) {
        if (Zone::enSector == sector->GetType())
            sectors.append(sector->GetShortName());
    }
    networkSettings()->setValue("users/fir_sectors", sectors.join("-"));

    connect(newView.data(), SIGNAL(singleCoordPicked()), this, SLOT(_SetupMapNote()));

    newView->setBackgroundBrush(UserProfile::CSettings::Instance().GetColor(UserProfile::CtBackground));
    setCentralWidget(newView.data());
    //setWindowState(Qt::WindowMaximized);
    setWindowTitle(fir->GetIDName() + " - " + fir->GetName());

    //m_AddAction->setEnabled(true);

    newView->LoadDF();

    control_window_->setBearingPoints(newView->GetListDF());
    int sectorIndex = control_window_->sectorCurrentIndex();
    if (sectorIndex != -1)
        control_window_->setBearingId(m_pSectoringModel->record(sectorIndex).value("id"));

    restrictions_widget_->setCompleter(CListAC::getFirPoints());
    restrictions_widget_->itemListReset(fir->GetRestrictionUser());

    if (getRedisDB())
    {
        RunwayWidgetDataVPP rwf(fir->GetIDName());
        if(getRedisDB()->loadRWFVPP(fir->GetIDName(), rwf))
        {
            m_mapEchelon.insert(fir->GetIDName(), rwf.TL());
            m_mapPressure.insert(fir->GetIDName(), rwf.QNH());
        }
    }
    int indexEchelon = GetEchelonComboIndex(m_mapEchelon.value(fir->GetIDName(), newView->GetEchelon()));
    if(indexEchelon != -1)
    {
        m_EchelonCombo->setCurrentIndex(indexEchelon);
        _OnChangeEchelon(indexEchelon);
    }
    m_PressSpin->setValue(m_mapPressure.value(fir->GetIDName(), m_PressSpin->value()));

    newView->onUserPicsVisabilityChanged();

    BlockNetworkMessages(false);
}

void XMasterWindow::_AddFIR(uint uiIdFIR)
{
    if (QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView))
    {
        BlockNetworkMessages(true);

        CView::TScenePtr pFIR = CView::TScenePtr(new CGUIScene());
        pFIR->SetCentralPoint(view->GetCentralPoint());
        loadFromDB(uiIdFIR, *pFIR, *view);

        QSharedPointer<Scene> pSector(new Scene());
        pSector->SetCentralPoint(view->GetCentralPoint());
        TVA_CATCHALL_TRY
        {
            dao()->LoadZone(uiIdFIR, *pSector);
        }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }

        CView::TSceneList::iterator itFind = view->GetScenes().find(pFIR->GetIDName());
        if (itFind != view->GetScenes().end())
        {
            if (pSector->GetType() == Zone::enSector)
            {
                Scene::TScenePtr pExistedSector = (*itFind)->FindSector(pSector->GetID());
                if (pExistedSector)
                    pExistedSector->MarkItemActive(true);
            }
            else
                (*itFind)->MarkItemActive(true);
        }
        else
        {
            view->AddScene(pFIR->GetIDName(), pFIR);

            // Add new FIR into restrictions editor FIR list.
            if (gWidgetRestriction)
                gWidgetRestriction->addFir(pFIR->GetID(), pFIR->GetIDName(), pFIR->GetName());
        }
        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pView)
            pView->prepare();

        BlockNetworkMessages(false);
    }
}

void XMasterWindow::_LoadEnvironment(int uiSectoring)
{
    if (uiSectoring == -1)
    {
        if (getRedisDB())
        {
            SectorR sr;
            const QString sClient = networkSettings()->value("oldi/name", "Unknown").toString();
            sr.SetClient(sClient);
            getRedisDB()->removeClient(sr);
            CleanActiveSector();
        }
        return;
    }
    // Load specified environment
    QString sSectorType;
    // FIRs
    QList<uint> ListIdFIR;
    // Active sectors
    QList<uint> ListIdSector;

    TVA_CATCHALL_TRY
    {
        if (uiSectoring)
        {
            privileges = dao()->LoadSectoringTypePrivilege(uiSectoring);
            sSectorType = dao()->LoadSectoringType(uiSectoring);
            dao()->LoadIdFIREnvironment(uiSectoring, ListIdFIR);
            foreach (uint id, ListIdFIR) {
                QList<uint> sectors;
                dao()->LoadIdSectorsEnvironment(uiSectoring, id, sectors);
                ListIdSector += sectors;
            }
        }
        //networkSettings()->setValue("users/current_sector_type", sSectorType);
        //qDebug() << "sector:" << sSectorType;
        if (CUserRole::IsPermission(privileges, CSectoringModel::GROUNDTAXIING_UNIQUE))
        {
            QString airport;
            dao()->LoadSectorIdName(ListIdSector.first(), &airport);
            if (!airport.isEmpty())
            {
                _LoadGroundView(airport);
                loadToolbarPrivileges(privileges);
                loadListPrivileges(privileges);
            }
        }
        else
        {
            _LoadEnvironment(privileges, ListIdFIR, ListIdSector);
        }
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
}

void XMasterWindow::_LoadEnvironment(QString privileges, QList<uint> ListIdFIR, const QList<uint> &ListIdSector)
{
    if (!ListIdFIR.empty())
    {
        bool bReload = true;

        m_LayoutsMainFIRAction->setVisible(true);

        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(GetView());
        if(view && view->GetMainFIR())
        {
            bReload = false;
            QList<uint>::iterator itFIR = ListIdFIR.begin();
            if(itFIR != ListIdFIR.end())
            {
                if(*itFIR != view->GetMainFIR()->GetID())
                    bReload = true;
            }
            if(!bReload)
            {
                bool bDelete = false;
                CView::TSceneList::iterator itChildFir = view->GetScenes().begin();
                while(itChildFir != view->GetScenes().end())
                {
                    if(ListIdFIR.contains((*itChildFir)->GetID()))
                        ++itChildFir;
                    else
                    {
                        bDelete = true;
                        itChildFir = view->GetScenes().erase(itChildFir);
                    }
                }
                itFIR = ListIdFIR.begin();
                for(; itFIR != ListIdFIR.end(); ++itFIR)
                {
                    bool bAdd = true;
                    itChildFir = view->GetScenes().begin();
                    for(; bAdd && itChildFir != view->GetScenes().end(); ++itChildFir)
                    {
                        if((*itChildFir)->GetID() == *itFIR)
                            bAdd = false;
                    }
                    if(bAdd)
                    {
                        Scene::TScenePtr pscene = view->GetMainFIR()->FindSector(*itFIR);
                        if(pscene)
                            bAdd = false;
                    }
                    if(bAdd)
                    {
                        bDelete = false;
                        _AddFIR(*itFIR);
                    }
                }
                if(bDelete)
                {
                    CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
                    if (pView)
                        pView->prepare();
                }
                {
                    CView::TSceneList::iterator itFIR = view->GetScenes().begin();
                    for(; itFIR != view->GetScenes().end(); ++itFIR)
                    {
                        Scene::TSceneList::iterator itSector = (*itFIR)->GetSectorList().begin();
                        for(; itSector != (*itFIR)->GetSectorList().end(); ++itSector)
                            (*itSector)->MarkItemActive(false);
                    }
                }
            }
        }
        if (bReload)
        {
            QList<uint>::iterator itFIR = ListIdFIR.begin();
            for (; itFIR != ListIdFIR.end(); ++itFIR)
            {
                if (itFIR == ListIdFIR.begin())
                    _LoadFIR(*itFIR);
                else
                    _AddFIR(*itFIR);
            }
            view = qSharedPointerDynamicCast<CView>(GetView());
        }
        if (view && ListIdFIR.size())
        {
            CView::TSceneList::iterator itFIR = view->GetScenes().begin();
            for(; itFIR != view->GetScenes().end(); ++itFIR)
            {
                if((*itFIR)->GetID() == ListIdFIR.at(0))
                    (*itFIR)->SetMainFIR(true);
                else
                    (*itFIR)->SetMainFIR(false);
            }
        }
        // Active sectors
        if (view && view->GetMainFIR())
        {
            QList<uint>::const_iterator itFIRSector = ListIdSector.begin();
            for (; itFIRSector != ListIdSector.end(); ++itFIRSector)
            {
                CView::TSceneList::iterator itFIR = view->GetScenes().begin();
                for (; itFIR != view->GetScenes().end(); ++itFIR)
                {
                    if (Scene::TScenePtr pscene = (*itFIR)->FindSector(*itFIRSector))
                        pscene->MarkItemActive(true);
                }
            }
            SetCurSectorAirports();
            if (CGUIView *pView = dynamic_cast<CGUIView*> (m_ZonesView.data())) {
                pView->Draw();
                pView->Redraw();
            }
            _SendSectorsList(ListIdSector);
        }
        // Toolbars
        loadToolbarPrivileges(privileges);
    }
    else
    {
        _SendSectorsList(QList<uint>());
        SceneClean();
    }

    // Privileges
    // Lists
    /*Pasha while (!m_mapDoc.empty())
        CloseDoc(m_mapDoc.begin().key());*/
    loadListPrivileges(privileges);
}

void XMasterWindow::_LoadUserEnvironment()
{
    m_MainMenu->setVisible(mUser->GetRoleObj()->IsPermission(CRoleModel::MENU_UNIQUE));

    // Sectoring
    m_pSectoringCombo->blockSignals(true);
    control_window_->sectorBlockSignals(true);
    m_pSectoringModel->setFilter(QString("id IN "
                                         "(SELECT client_sectoring.sectoring_id "
                                         "FROM client_sectoring "
                                         "WHERE client_sectoring.client_role_id = %1) "
                                         "OR id = 0 OR id = -1").arg(mUser->GetRole()));
    m_pSectoringCombo->blockSignals(false);
    control_window_->sectorBlockSignals(false);
    _SelectSectorModel();

    // User name
    m_pUserFullNameLabel->setText(mUser->GetFullName());
    control_window_->setCurrentUser(mUser->GetFullName());
}

void XMasterWindow::_OnListACC()
{
    if (!m_mapDoc.contains(XDocWidget::ListACC))
        OpenDoc(XDocWidget::ListACC);
    else
        CloseDoc(XDocWidget::ListACC);
}

void XMasterWindow::_OnListSIL_ACC()
{
    if (!m_mapDoc.contains(XDocWidget::ListSIL_ACC))
        OpenDoc(XDocWidget::ListSIL_ACC);
    else
        CloseDoc(XDocWidget::ListSIL_ACC);
}

void XMasterWindow::_OnListAPP()
{
    if (!m_mapDoc.contains(XDocWidget::ListAPP))
        OpenDoc(XDocWidget::ListAPP);
    else
        CloseDoc(XDocWidget::ListAPP);
}

void XMasterWindow::_OnListSIL_APP_ARR()
{
    if (!m_mapDoc.contains(XDocWidget::ListSIL_APP_ARR))
        OpenDoc(XDocWidget::ListSIL_APP_ARR);
    else
        CloseDoc(XDocWidget::ListSIL_APP_ARR);
}

void XMasterWindow::_OnListSIL_APP_DEP()
{
    if (!m_mapDoc.contains(XDocWidget::ListSIL_APP_DEP))
        OpenDoc(XDocWidget::ListSIL_APP_DEP);
    else
        CloseDoc(XDocWidget::ListSIL_APP_DEP);
}

void XMasterWindow::_BackgroundColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtBackground), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid() && m_ZonesView)
    {
        stg.SetColor(UserProfile::CtBackground, colorNew);
        stg.Save();

        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView);
        if (view)
        {
            dynamic_cast<QGraphicsView*> (m_ZonesView.data())->setBackgroundBrush(colorNew);
        }
    }
}

void XMasterWindow::_FormularActiveColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtFormularActive), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid() && m_ZonesView)
    {
        stg.SetColor(UserProfile::CtFormularActive, colorNew);
        stg.Save();

        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pView)
            pView->SetFormularColor(colorNew);
    }
}
void XMasterWindow::_FormularPassiveColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    //QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtFormularPassive), this);
    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtFormularPassive), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid() && m_ZonesView)
    {
        stg.SetColor(UserProfile::CtFormularPassive, colorNew);
        stg.Save();

        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pView)
            pView->SetFormularColor(colorNew);
    }
}
void XMasterWindow::_ControlledColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew_foreground = QColorDialog::getColor(stg.GetColor(UserProfile::CtControlled), this,
                      tr("Foreground Controlled Full, Controlled Short color"), QColorDialog::ShowAlphaChannel);
    if (colorNew_foreground.isValid())
    {
        stg.SetColor(UserProfile::CtControlled, colorNew_foreground);
        stg.Save();
        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pView)
            pView->update();
    }
}
void XMasterWindow::_ControlledBackColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    QColor colorNew_background = QColorDialog::getColor(stg.GetColor(UserProfile::CtControlled_back), this,
                      tr("Background Controlled Full, Controlled Short color"), QColorDialog::ShowAlphaChannel);
    if (colorNew_background.isValid())
    {
        stg.SetColor(UserProfile::CtControlled_back, colorNew_background);
        stg.Save();
        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pView)
            pView->update();
    }
}
void XMasterWindow::_InColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew_foreground = QColorDialog::getColor(stg.GetColor(UserProfile::CtIn), this,
                      tr("Foreground In Full, In Short color"), QColorDialog::ShowAlphaChannel);
    if (colorNew_foreground.isValid())
    {
        stg.SetColor(UserProfile::CtIn, colorNew_foreground);
        stg.Save();
        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pView)
            pView->update();
    }
}
void XMasterWindow::_InBackColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    QColor colorNew_background = QColorDialog::getColor(stg.GetColor(UserProfile::CtIn_back), this,
                      tr("Background In Full, In Short color"), QColorDialog::ShowAlphaChannel);
    if (colorNew_background.isValid())
    {
        stg.SetColor(UserProfile::CtIn_back, colorNew_background);
        stg.Save();
        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pView)
            pView->update();
    }
}
void XMasterWindow::_RelatedColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew_foreground = QColorDialog::getColor(stg.GetColor(UserProfile::CtRelated), this,
                      tr("Foreground Related Full, Related Short color"), QColorDialog::ShowAlphaChannel);
    if (colorNew_foreground.isValid())
    {
        stg.SetColor(UserProfile::CtRelated, colorNew_foreground);
        stg.Save();
        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pView)
            pView->update();
    }
}
void XMasterWindow::_RelatedBackColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    QColor colorNew_background = QColorDialog::getColor(stg.GetColor(UserProfile::CtRelated_back), this,
                      tr("Background Related Full, Related Short color"), QColorDialog::ShowAlphaChannel);
    if (colorNew_background.isValid())
    {
        stg.SetColor(UserProfile::CtRelated_back, colorNew_background);
        stg.Save();
        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pView)
            pView->update();
    }
}
void XMasterWindow::_UnrelatedColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew_foreground = QColorDialog::getColor(stg.GetColor(UserProfile::CtUnrelated), this,
                      tr("Foreground Unrelated Full, Unrelated Short color"), QColorDialog::ShowAlphaChannel);
    if (colorNew_foreground.isValid())
    {
        stg.SetColor(UserProfile::CtUnrelated, colorNew_foreground);
        stg.Save();
        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pView)
            pView->update();
    }
}
void XMasterWindow::_UnrelatedBackColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    QColor colorNew_background = QColorDialog::getColor(stg.GetColor(UserProfile::CtUnrelated_back), this,
                      tr("Background Unrelated Full, Unrelated Short color"), QColorDialog::ShowAlphaChannel);
    if (colorNew_background.isValid())
    {
        stg.SetColor(UserProfile::CtUnrelated_back, colorNew_background);
        stg.Save();
        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pView)
            pView->update();
    }
}
void XMasterWindow::_ExtendedColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew_foreground = QColorDialog::getColor(stg.GetColor(UserProfile::CtExtended), this,
                      tr("Foreground Extended color"), QColorDialog::ShowAlphaChannel);
    if (colorNew_foreground.isValid())
    {
        stg.SetColor(UserProfile::CtExtended, colorNew_foreground);
        stg.Save();
        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pView)
            pView->update();
    }
}
void XMasterWindow::_MapColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtMap), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtMap, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView);
            if (view)
            {
                CView::TSceneList::iterator itScene = view->GetScenes().begin();
                for (; itScene != view->GetScenes().end(); ++itScene)
                {
                    CGUIScene* pscene = dynamic_cast<CGUIScene*> (itScene->data());
                    if (pscene)
                        pscene->SetMapColor(colorNew);
                }
            }
        }
    }
}

void XMasterWindow::_FIRColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtFIR), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtFIR, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView);
            if (view)
            {
                CView::TSceneList::iterator itScene = view->GetScenes().begin();
                for (; itScene != view->GetScenes().end(); ++itScene)
                {
                    CGUIScene* pscene = dynamic_cast<CGUIScene*> (itScene->data());
                    if (pscene)
                        pscene->SetFIRColor(colorNew);
                }
            }
        }
    }
}

void XMasterWindow::_BorderColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtBorder), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtBorder, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView);
            if (view)
            {
                CView::TSceneList::iterator itScene = view->GetScenes().begin();
                for (; itScene != view->GetScenes().end(); ++itScene)
                {
                    CGUIScene* pscene = dynamic_cast<CGUIScene*> (itScene->data());
                    if (pscene)
                        pscene->SetBorderColor(colorNew);
                }
            }
        }
    }
}

void XMasterWindow::_SectorColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtSector), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtSector, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView);
            if (view)
            {
                CView::TSceneList::iterator itScene = view->GetScenes().begin();
                for (; itScene != view->GetScenes().end(); ++itScene)
                {
                    CGUIScene* pscene = dynamic_cast<CGUIScene*> (itScene->data());
                    if (pscene)
                        pscene->SetSectorColor(colorNew);
                }
            }
        }
    }
}

void XMasterWindow::_LegendColorSlot()
{
    QColor colorNew = QColorDialog::getColor(Qt::white, this);
    if (colorNew.isValid())
    {
        UserProfile::CSettings::Instance().SetColor(UserProfile::CtLegend, colorNew);
    }
}

void XMasterWindow::_ApproachColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtApproach), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtApproach, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView);
            if (view)
            {
                CView::TSceneList::iterator itScene = view->GetScenes().begin();
                for (; itScene != view->GetScenes().end(); ++itScene)
                {
                    CGUIScene* pscene = dynamic_cast<CGUIScene*> (itScene->data());
                    if (pscene)
                        pscene->SetApproachColor(colorNew);
                }
            }
        }
    }
}

void XMasterWindow::_BlastoffColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtBlastoff), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtBlastoff, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView);
            if (view)
            {
                CView::TSceneList::iterator itScene = view->GetScenes().begin();
                for (; itScene != view->GetScenes().end(); ++itScene)
                {
                    CGUIScene* pscene = dynamic_cast<CGUIScene*> (itScene->data());
                    if (pscene)
                        pscene->SetBlastoffColor(colorNew);
                }
            }
        }
    }
}

void XMasterWindow::_RouteColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtMonitoringRoute), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtMonitoringRoute, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView);
            if (view)
            {
                CView::TSceneList::iterator itScene = view->GetScenes().begin();
                for (; itScene != view->GetScenes().end(); ++itScene)
                {
                    CGUIScene* pscene = dynamic_cast<CGUIScene*> (itScene->data());
                    if (pscene)
                        pscene->SetRouteColor(colorNew);
                }
            }
        }
    }
}

void XMasterWindow::_HoldingColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtHolding), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtHolding, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView);
            if (view)
            {
                CView::TSceneList::iterator itScene = view->GetScenes().begin();
                for (; itScene != view->GetScenes().end(); ++itScene)
                {
                    CGUIScene* pscene = dynamic_cast<CGUIScene*> (itScene->data());
                    if (pscene)
                        pscene->SetHoldingColor(colorNew);
                }
            }
        }
    }
}

void XMasterWindow::_CorridorColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtCorridor), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtCorridor, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView);
            if (view)
            {
                CView::TSceneList::iterator itScene = view->GetScenes().begin();
                for (; itScene != view->GetScenes().end(); ++itScene)
                {
                    CGUIScene* pscene = dynamic_cast<CGUIScene*> (itScene->data());
                    if (pscene)
                        pscene->SetCorridorColor(colorNew);
                }
            }
        }
    }
}
void XMasterWindow::_GridColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtGrid), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtGrid, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
            if (pView)
            {
                pView->SetGridColor(colorNew);
            }
        }
    }
}

void XMasterWindow::_RestrictionColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorOld = stg.GetColor(UserProfile::CtRestr);
    QColor colorNew = QColorDialog::getColor(colorOld.isValid() ? colorOld : Qt::red, this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtRestr, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
            if (pView)
            {
                pView->update();
            }
        }
    }
}

void XMasterWindow::_MapNoteColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorOld = stg.GetColor(UserProfile::CtMapNote);
    QColor colorNew = QColorDialog::getColor(colorOld.isValid() ? colorOld : Qt::black, this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtMapNote, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
            if (pView)
            {
                pView->Draw();
//                pView->update();
            }
        }
    }
}

void XMasterWindow::_MeteoColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorOld = stg.GetColor(UserProfile::CtMeteo);
    QColor colorNew = QColorDialog::getColor(colorOld.isValid() ? colorOld : Qt::black, this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtMeteo, colorNew);
        stg.Save();
    }
}

void XMasterWindow::_RestrictionUserColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorOld = stg.GetColor(UserProfile::CtRestrUser);
    QColor colorNew = QColorDialog::getColor(colorOld.isValid() ? colorOld : Qt::red, this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtRestrUser, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
            if (pView)
            {
                pView->update();
            }
        }
    }
}

void XMasterWindow::_RestrictionUserObstacleColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorOld = stg.GetColor(UserProfile::CtRestrUserObstacle);
    QColor colorNew = QColorDialog::getColor(colorOld.isValid() ? colorOld : Qt::blue, this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtRestrUserObstacle, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
            if (pView)
            {
                pView->update();
            }
        }
    }
}

void XMasterWindow::_RestrictionUserNFZColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorOld = stg.GetColor(UserProfile::CtRestrUserNFZ);
    QColor colorNew = QColorDialog::getColor(colorOld.isValid() ? colorOld : Qt::yellow, this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtRestrUserNFZ, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
            if (pView)
            {
                pView->update();
            }
        }
    }
}

void XMasterWindow::_RestrictionUserMapColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorOld = stg.GetColor(UserProfile::CtRestrUserMap);
    QColor colorNew = QColorDialog::getColor(colorOld.isValid() ? colorOld : Qt::yellow, this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtRestrUserMap, colorNew);
        stg.Save();

        if (m_ZonesView)
        {
            CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
            if (pView)
            {
                pView->update();
            }
        }
    }
}

void XMasterWindow::_DarkCSSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    stg.SetColor(UserProfile::CtBackground, QColor(24, 32, 64));
    stg.SetColor(UserProfile::CtGrid, QColor(70, 100, 80));
    stg.SetColor(UserProfile::CtMap, QColor(100, 100, 100));
    stg.SetColor(UserProfile::CtCorridor, QColor(1, 52, 73));
    stg.SetColor(UserProfile::CtMonitoringRoute, QColor(120, 120, 120));
    stg.SetColor(UserProfile::CtHolding, QColor(120, 120, 120));
    stg.SetColor(UserProfile::CtFIR, QColor(30, 40, 70));
    stg.SetColor(UserProfile::CtBorder, Qt::transparent);
    stg.SetColor(UserProfile::CtApproach, QColor(150, 150, 150));
    stg.SetColor(UserProfile::CtBlastoff, QColor(160, 160, 160));
    stg.SetColor(UserProfile::CtSector, Qt::gray);

    stg.SetColorFlags(UserProfile::CtFIR, 0);
    stg.SetColorFlags(UserProfile::CtBorder, UserProfile::CfContour);

    _UpdateColorScheme();
}

void XMasterWindow::_KhakiCSSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    stg.SetColor(UserProfile::CtBackground, QColor(102, 126, 64));
    stg.SetColor(UserProfile::CtGrid, QColor(140, 198, 88));
    stg.SetColor(UserProfile::CtMap, QColor(100, 186, 100));
    stg.SetColor(UserProfile::CtCorridor, QColor(36, 92, 73));
    stg.SetColor(UserProfile::CtMonitoringRoute, QColor(108, 190, 108));
    stg.SetColor(UserProfile::CtHolding, QColor(108, 190, 108));
    stg.SetColor(UserProfile::CtFIR, QColor(52, 106, 69));
    stg.SetColor(UserProfile::CtBorder, Qt::transparent);
    stg.SetColor(UserProfile::CtApproach, QColor(150, 210, 150));
    stg.SetColor(UserProfile::CtBlastoff, QColor(160, 220, 160));
    stg.SetColor(UserProfile::CtSector, Qt::gray);

    stg.SetColorFlags(UserProfile::CtFIR, 0);
    stg.SetColorFlags(UserProfile::CtBorder, UserProfile::CfContour);

    _UpdateColorScheme();
}

void XMasterWindow::_EuroControlCSSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    stg.SetColor(UserProfile::CtBackground, QColor(123, 123, 123));
    stg.SetColor(UserProfile::CtGrid, QColor(150, 150, 150));
    stg.SetColor(UserProfile::CtMap, QColor(170, 170, 170));
    stg.SetColor(UserProfile::CtCorridor, QColor(139, 139, 139));
    stg.SetColor(UserProfile::CtMonitoringRoute, QColor(140, 140, 140));
    stg.SetColor(UserProfile::CtHolding, QColor(140, 140, 140));
    stg.SetColor(UserProfile::CtFIR, QColor(135, 135, 135));
    stg.SetColor(UserProfile::CtBorder, Qt::transparent);
    stg.SetColor(UserProfile::CtApproach, QColor(210, 210, 210));
    stg.SetColor(UserProfile::CtBlastoff, QColor(220, 220, 220));
    stg.SetColor(UserProfile::CtSector, Qt::gray);

    stg.SetColorFlags(UserProfile::CtFIR, 0);
    stg.SetColorFlags(UserProfile::CtBorder, UserProfile::CfContour);

    _UpdateColorScheme();
}

void XMasterWindow::_LightCSSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    stg.SetColor(UserProfile::CtBackground, QColor(255, 255, 255));
    stg.SetColor(UserProfile::CtGrid, QColor(140, 115, 88));
    stg.SetColor(UserProfile::CtMap, QColor(170, 170, 170));
    stg.SetColor(UserProfile::CtCorridor, QColor(232, 232, 232));
    stg.SetColor(UserProfile::CtMonitoringRoute, QColor(130, 130, 130));
    stg.SetColor(UserProfile::CtHolding, QColor(130, 130, 130));
    stg.SetColor(UserProfile::CtFIR, QColor(200, 200, 200));
    stg.SetColor(UserProfile::CtBorder, Qt::transparent);
    stg.SetColor(UserProfile::CtApproach, QColor(110, 110, 110));
    stg.SetColor(UserProfile::CtBlastoff, QColor(120, 120, 120));
    stg.SetColor(UserProfile::CtSector, Qt::gray);

    stg.SetColorFlags(UserProfile::CtFIR, 0);
    stg.SetColorFlags(UserProfile::CtBorder, UserProfile::CfContour);

    _UpdateColorScheme();
}

void XMasterWindow::_UpdateColorScheme()
{
    if (!m_ZonesView)
        return;

    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    stg.Save();

    CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
    if (pView)
    {
        pView->setBackgroundBrush(stg.GetColor(UserProfile::CtBackground));
        pView->SetFormularColor(stg.GetColor(UserProfile::CtFormularPassive));
        pView->SetGridColor(stg.GetColor(UserProfile::CtGrid));
    }

    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView);
    if (view)
    {
        CView::TSceneList::iterator itScene = view->GetScenes().begin();
        for (; itScene != view->GetScenes().end(); ++itScene)
        {
            CGUIScene* pscene = dynamic_cast<CGUIScene*> (itScene->data());
            if (pscene)
            {
                pscene->SetMapColor(stg.GetColor(UserProfile::CtMap));
                pscene->SetFIRColor(stg.GetColor(UserProfile::CtFIR));
                pscene->SetBorderColor(stg.GetColor(UserProfile::CtBorder));
                pscene->SetSectorColor(stg.GetColor(UserProfile::CtSector));
                pscene->SetApproachColor(stg.GetColor(UserProfile::CtApproach));
                pscene->SetBlastoffColor(stg.GetColor(UserProfile::CtBlastoff));
                pscene->SetRouteColor(stg.GetColor(UserProfile::CtMonitoringRoute));
                pscene->SetHoldingColor(stg.GetColor(UserProfile::CtHolding));
                pscene->SetCorridorColor(stg.GetColor(UserProfile::CtCorridor));
            }
        }
    }
}

void XMasterWindow::loadCurrentUserSettings()
{
    QSettings &settings = *globalSettings();
    settings.beginGroup(mUser->GetName());
    int zoomValue = settings.value("zoom", 0).toInt();
    QPointF qCenter(settings.value("center").toPointF());
    settings.endGroup();

    m_ZoomSlider->setValue(zoomValue);
    control_window_->setZoom(zoomValue);
    QGraphicsView* pView = dynamic_cast<QGraphicsView*> (centralWidget());
    if (pView && !qCenter.isNull())
        pView->centerOn(qCenter);
}

void XMasterWindow::saveCurrentUserSettings()
{
    QSettings &settings = *globalSettings();
    settings.beginGroup(mUser->GetName());
    settings.setValue("zoom", m_ZoomSlider->value());
    settings.setValue("zoom", control_window_->zoom());
    if (QGraphicsView* pView = dynamic_cast<QGraphicsView*> (centralWidget()))
        settings.setValue("center", pView->mapToScene(pView->viewport()->rect().center()));
    settings.endGroup();
}

void XMasterWindow::airplaneLengthChanged(int val)
{
    if (airplaneLengthKilometers->isChecked())
        networkSettings()->setValue("airplane/deflexion_kilometer", val);
    else if (airplaneLengthSec->isChecked())
        networkSettings()->setValue("airplane/deflexion_sec", val);
}

void XMasterWindow::airplaneLengthTypeChanged()
{
    if (airplaneLengthKilometers->isChecked())
    {
        networkSettings()->setValue("airplane/deflexion_type", "kilometer");
        airplaneLength->setValue(networkSettings()->value("airplane/deflexion_kilometer", 0).toInt());
        control_window_->setDistanceVectorValue(networkSettings()->value("airplane/deflexion_kilometer", 0).toInt());
    }
    else if (airplaneLengthSec->isChecked())
    {
        networkSettings()->setValue("airplane/deflexion_type", "sec");
        airplaneLength->setValue(networkSettings()->value("airplane/deflexion_sec", 0).toInt());
        control_window_->setSpeedVectorValue(networkSettings()->value("airplane/deflexion_sec", 0).toInt());
    }
}

void XMasterWindow::speedVectorClicked(int value)
{
    airplaneLengthSec->setChecked(true);
    airplaneLength->setValue(value);
    networkSettings()->setValue("airplane/deflexion_type", "sec");
    airplaneLength->setValue(networkSettings()->value("airplane/deflexion_sec", 0).toInt());
    airplaneLengthChanged(value);
    control_window_->setSpeedVectorValue(value);
}

void XMasterWindow::distanceVectorClicked(int value)
{
    airplaneLengthKilometers->setChecked(true);
    airplaneLength->setValue(value);
    networkSettings()->setValue("airplane/deflexion_type", "kilometer");
    airplaneLength->setValue(networkSettings()->value("airplane/deflexion_kilometer", 0).toInt());
    airplaneLengthChanged(value);
    control_window_->setDistanceVectorValue(value);
}

void XMasterWindow::_OnCheckAutoCollapse(bool checked)
{
    networkSettings()->setValue("airplane/auto_collapse", checked ? "yes" : "no");

    if (m_ZonesView)
    {
        if (CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data()))
            pView->SetAutoCollapse(checked);
        if (GUIGroundScene* pGroundView = dynamic_cast<GUIGroundScene*> (m_ZonesView.data()))
            pGroundView->SetAutoCollapse(checked);
    }
}

void XMasterWindow::_OnCheckCorridorRoute(bool checked)
{
    networkSettings()->setValue("users/corridor_route", checked ? "yes" : "no");
    if (m_ZonesView) {
        if (CGUIView *view = dynamic_cast<CGUIView*> (m_ZonesView.data()))
            view->Redraw();
    }
}

void XMasterWindow::_OnCheckFlightLevelMonitoring(bool checked)
{
    networkSettings()->setValue("users/flight_level_monitoring", checked ? "yes" : "no");
    if (m_ZonesView) {
        if (CGUIView *view = dynamic_cast<CGUIView*> (m_ZonesView.data()))
            view->OnDrawAirplanes();
    }
}

void XMasterWindow::onPointZoomClicked(bool checked)
{
    networkSettings()->setValue("users/point_zoom", checked ? "yes" : "no");
}

void XMasterWindow::onRoutesClicked(bool checked)
{
    networkSettings()->setValue("users/show_routes", checked ? "yes" : "no");
    if (m_ZonesView) {
        if (CGUIView *view = dynamic_cast<CGUIView*> (m_ZonesView.data()))
            view->Redraw();
    }
}

void XMasterWindow::onCheckPoints(bool checked)
{
    networkSettings()->setValue("users/draw_points", checked ? "yes" : "no");

    if (m_ZonesView) {
        if (CGUIView *pView = dynamic_cast<CGUIView*> (m_ZonesView.data()))
            pView->Redraw();
    }
}

void XMasterWindow::onCheckBPoints(bool checked)
{
    networkSettings()->setValue("users/draw_Borderpoints", checked ? "yes" : "no");

    if (m_ZonesView) {
        if (CGUIView *pView = dynamic_cast<CGUIView*> (m_ZonesView.data()))
            pView->Redraw();
    }
}
void XMasterWindow::onCheckMeteo(bool checked)
{
    networkSettings()->setValue("users/draw_meteo", checked ? "yes" : "no");

    if (m_ZonesView) {
        if (CGUIView *pView = dynamic_cast<CGUIView*> (m_ZonesView.data()))
            pView->Redraw();
    }
}

void XMasterWindow::_OnCheckShowSectorBorder(bool checked)
{
    networkSettings()->setValue("users/sector_border", checked ? "yes" : "no");

    if (m_ZonesView) {
        if (CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data()))
            pView->Redraw();
    }
}

void XMasterWindow::_OnCheckShowRouteOnAssume(bool checked)
{
    networkSettings()->setValue("users/show_route_on_assume", checked ? "yes" : "no");
}

void XMasterWindow::_FLSlider(int iLowValue, int iHighValue)
{
    networkSettings()->setValue("airplane/low_FL", iLowValue);
    networkSettings()->setValue("airplane/high_FL", iHighValue);
    QString sText;
    if (IsSISet(SIFeet))
        sText = QString("%1 - %2").arg(iLowValue, 3, 10, QLatin1Char('0')).arg(iHighValue, 3, 10, QLatin1Char('0'));
    else
        sText = QString("%1 - %2").arg(qRound(iLowValue*30.48), 3, 10, QLatin1Char('0')).arg(qRound(iHighValue*30.48), 3, 10, QLatin1Char('0'));
    m_pSliderBut->setText(sText);
    m_pSliderBut->adjustSize();
    if (m_ZonesView)
    {
        if (CGUIView *pView = dynamic_cast<CGUIView*> (m_ZonesView.data()))
            pView->CheckPlanesAlt();
    }

    control_window_->setHeightMinText(QString("%1").arg(iLowValue));
    control_window_->setHeightMaxText(QString("%1").arg(iHighValue));
}

QList<uint> XMasterWindow::_SendPrevSectorList()
{
    QList<uint> listSectors;
    if (!m_listPrevSect.isEmpty())
        m_listPrevSect.pop_back();
    if (!m_listPrevSect.isEmpty())
    {
        listSectors = m_listPrevSect.last();
        m_listPrevSect.pop_back();
    }
    _SendSectorsList(listSectors);
    return listSectors;
}

void XMasterWindow::_SendSectorsList(const QList<uint> &listIdFIR)
{
    m_listPrevSect.append(listIdFIR);

    QString sSectors;
    QList<Zone> Zones;
    TVA_CATCHALL_TRY
    {
        if (!listIdFIR.empty())
            dao()->LoadZones(Zones);
    }TVA_CATCHALL(err){
    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
    networkSettings()->setValue("users/current_sector", "");

    int lower_fl,lower_fl1,lower_fl2;
        int upper_fl;
        for (int i = 0; i < listIdFIR.size(); ++i)
        {
            foreach (const Zone &zone, Zones) {
                if (listIdFIR[i] == zone.GetID())
                {
                   /* if (i == 0) {
                        networkSettings()->setValue("users/current_sector", zone.GetShortName());
                        int lower_fl = zone.GetLowLimit() / 30;
                        if (lower_fl > 10)
                            lower_fl -= 10;
                        const int upper_fl = (zone.GetUpLimit() / 30) + 10;
                        control_window_->setRestrictedSpanRange(lower_fl, upper_fl);
                    }*/

                    if (i == 0)
                    {
                        networkSettings()->setValue("users/current_sector", zone.GetShortName());
                        lower_fl = zone.GetLowLimit() / 30;
                        upper_fl = (zone.GetUpLimit() / 30) + 10;
                    }
                    if (i == 1 && listIdFIR.size() == 2 )
                        lower_fl = zone.GetLowLimit() / 30;
                    if (i == 1 && listIdFIR.size() == 3 )
                        lower_fl1 = zone.GetLowLimit() / 30;
                    if (i == 2 && listIdFIR.size() == 2)
                        lower_fl = zone.GetLowLimit() / 30;
                    if (i == 2 && listIdFIR.size() == 3 )
                        lower_fl2 = zone.GetLowLimit() / 30;
                    if (listIdFIR.size() == 3 )
                    {
                        if (lower_fl1 < lower_fl2)
                            lower_fl = lower_fl1;
                        else
                            lower_fl = lower_fl2;

                    }



                    if (!sSectors.isEmpty())
                        sSectors += "-";
                    sSectors += zone.GetShortName();
                    break;
                }
            }
        }

        if (!listIdFIR.empty())
        {
            if (lower_fl > 10)
                lower_fl -= 10;
            control_window_->setRestrictedSpanRange(lower_fl, upper_fl);
        }


    networkSettings()->setValue("users/current_sectors_short_name", sSectors);
    networkSettings()->setValue("users/force_sector", false);

#if defined(REDIS)
    OccupySectors();
    return;
#endif
    if (networkSettings()->value("simulator/server_type").toString() == "server")
    {
        OLDIMesDataSTS mesData;
        mesData.iMesNum = GetOLDIProc()->GetNextMesNum();
        mesData.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
        mesData.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
        mesData.sText = "-" + mesData.sFrom;
        if(sSectors.size()) mesData.sText += "-" + sSectors;
        QSharedPointer<OLDIMessage> pMes(new OLDIMesSTS(mesData));
        connect(pMes.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), this, SLOT(OLDIMesAnswer(const OLDIMessage*,const QStringList&)));
        connect(pMes.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), this, SLOT(OLDIMesTimeout(const OLDIMessage*)));
        m_listMes.append(pMes.data());
        GetOLDIProc()->SendMessage(pMes);
    }
}

void XMasterWindow::OccupySectors()
{
#if defined(REDIS)
    const QString sSectors = networkSettings()->value("users/current_sectors_short_name").toString();

    if (getRedisDB())
    {
        SectorRedis sr;
        if (getRedisDB()->loadSectors2(sr))
        {
            const QString sClient = networkSettings()->value("oldi/name", "Unknown").toString();
            QStringList sectors = sSectors.split("-", QString::SkipEmptyParts);
            if (sr.isOccupiedClientSectors(sClient, sectors))
            {
                if (QMessageBox::warning(this, windowTitle(), tr("Force sectoring?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
                {
                    _UndoSectoring();
                    return;
                }
                networkSettings()->setValue("users/force_sector", true);
            }
            sr.setClientSectors(sClient, sectors);
            getRedisDB()->saveSectors2(sr);

            setTransferSectors(sectors, sr);
        }

        const QString current_sector = networkSettings()->value("users/current_sector").toString();
        if (m_ZonesView) {
            if (QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView))
                view->releaseAirplanes(current_sector);
        }
        /*if (!current_sector.isEmpty()) {
            if (!networkSettings()->value("users/force_sector").toBool()) {
                if (m_ZonesView) {
                    if (QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView))
                        view->assumeAirplanes(current_sector);
                }
            }
        }*/
	if (!current_sector.isEmpty()) {
            if (m_ZonesView) {
                    if (QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(m_ZonesView))
                        view->assumeAirplanes(current_sector);
            }
        }
    }
#endif
}

void XMasterWindow::OLDIMesAnswer(const OLDIMessage* pMes, const QStringList& sListError)
{
    if(oldi::MES_STS == pMes->GetMesData().sType)
    {
        sSectOLDIMes = QString::null;
        if(sListError.size())
        {
            QSharedPointer<obelix::CCmdSectoringAction> pCmd(new obelix::CCmdSectoringAction(obelix::CCmdSectoringAction::FDP_rejected));
            GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
            sSectOLDIMes = pMes->GetMesData().sOLDIMes;
        }
        else
        {
            if(m_ZonesView && 0 != m_pSectoringModel->record(m_pSectoringCombo->currentIndex()).value("id").toInt())
            {
                QSharedPointer<obelix::CCmdSectoringAction> pCmd(new obelix::CCmdSectoringAction(obelix::CCmdSectoringAction::FDP_agreed));
                GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
            }
        }
        m_listMes.removeOne(pMes);
    }
}

void XMasterWindow::OLDIMesTimeout(const OLDIMessage* pMes)
{
    if(oldi::MES_STS == pMes->GetMesData().sType)
    {
        QSharedPointer<obelix::CCmdSectoringAction> pCmd(new obelix::CCmdSectoringAction(obelix::CCmdSectoringAction::FDP_timeout));
        GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
        m_listMes.removeOne(pMes);
    }
}

void XMasterWindow::RecieveOLDIMessage(QSharedPointer<OLDIMessage> pMes)
{
    if(!m_ZonesView)
        return;

    QString sMessageType = pMes->GetMesData().sType;
    if(oldi::MES_STS == sMessageType)
    {//status   // looks like client side
        OLDIMesDataSTS dataSTS = pMes.staticCast<OLDIMesSTS>()->GetDataSTS();
        if(1 == dataSTS.iPriority)
        {
            QSharedPointer<obelix::CCmdGUIRefresh> pCmd(new obelix::CCmdGUIRefresh());
            pCmd->SetValue(obelix::CCmdGUIRefresh::XML_TYPE, QL1S("STS"));
            GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
        }
    }
    else if(oldi::MES_URZ == sMessageType)
    {// restrictions & meteo info   //mixed client/server
        OLDIMesDataURZ dataURZ = pMes.staticCast<OLDIMesURZ>()->GetDataURZ();
        pMes->SetExecuted();
        if(pMes->GetMesData().bAnswer)
        {
            OLDIMesData dataLAM = pMes->GetLAM(GetOLDIProc()->GetNextMesNum());
            QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
            GetOLDIProc()->SendMessage(pMesLAM, false);
        }
        OLDIMesURZ::Type urzDBType(OLDIMesURZ::Type(dataURZ.sDBType.data()->toLatin1()));
        if(OLDIMesURZ::enInsert == urzDBType || OLDIMesURZ::enUpdate == urzDBType || OLDIMesURZ::enDelete == urzDBType)
        {
            if(!dataURZ.sFirIdName.isEmpty())
            {
                QByteArray arr = QByteArray::fromHex(dataURZ.sHexData.toLatin1());
                QDataStream strm(&arr, QIODevice::ReadOnly);
                CRestrictionUser urz;
                strm >> urz;
                if(!urz.GetGUID().isNull())
                {
                    TVA_CATCHALL_TRY
                    {
                        //save in DB
                        if(OLDIMesURZ::enInsert == urzDBType || OLDIMesURZ::enUpdate == urzDBType)
                        {
                            QList<Zone> Zones;
                            QString sWhere = QString("id_name = '%1' AND type = 1").arg(dataURZ.sFirIdName);
                            dao()->LoadZones(Zones, sWhere);

                            if(Zones.size() != 1) return;

                            urz.SetZoneID(Zones[0].GetID());
                            dao()->SaveUserRestriction(urz.GetZoneID(), urz, true);
                        }
                        if(OLDIMesURZ::enDelete == urzDBType)
                        {
                            quint32 restId = dao()->GetUserRestrictionIDbyGUID(urz.GetGUID());
                            if (!restId)
                                return;

                            urz.SetID(restId);
                            dao()->DeleteUserRestriction(urz.GetGUID());
                        }

                        //update on view
                        QSharedPointer<obelix::CCmdGUIRefresh> pCmd(new obelix::CCmdGUIRefresh());
                        pCmd->SetValue(obelix::CCmdGUIRefresh::XML_TYPE, QL1S("URZ"));
                        pCmd->SetValue(obelix::CCmdGUIRefresh::XML_URZ_ID, urz.GetID());
                        pCmd->SetValue(obelix::CCmdGUIRefresh::XML_URZTYPE, dataURZ.sDBType);
                        pCmd->SetValue(obelix::CCmdGUIRefresh::XML_FIR_NAME, dataURZ.sFirIdName);
                        GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
                    }
                    TVA_CATCHALL(err)
                    {
                        LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
                        return;
                    }
                }
            }
        }
        else if(OLDIMesURZ::enMeteo == urzDBType)
        {
            QByteArray arr = QByteArray::fromHex(dataURZ.sHexData.toLatin1());
            QDataStream strm(&arr, QIODevice::ReadOnly);
            MeteoRunway meteo;
            strm >> meteo;
            TVA_CATCHALL_TRY
            {
                //save in DB
                QList<MeteoRunway> list;
                list.append(meteo);
                dao()->SaveMeteoRunway(list);

                QSharedPointer<obelix::CCmdGUIRefresh> pCmd(new obelix::CCmdGUIRefresh());
                pCmd->SetValue(obelix::CCmdGUIRefresh::XML_TYPE, QL1S("URZ"));
                pCmd->SetValue(obelix::CCmdGUIRefresh::XML_URZTYPE, dataURZ.sDBType);
                GetObServer().SendToAllClients(pCmd.staticCast<CXMLCmd>());
            }
            TVA_CATCHALL(err)
            {
                LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
                return;
            }
        }
    }

    if(!CListAC::RecieveOLDIMessage(pMes))
    {
        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if(pView)
        {
            pView->RecieveOLDIMessage(pMes);
        }
    }
}

bool XMasterWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_FLSlider)
    {
        if(event->type() == QEvent::ToolTip)
        {
            QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
            if(helpEvent)
            {
                //QToolTip::showText(QCursor::pos(), "12345", m_FLSlider);
            }
        }
        if(event->type() == QEvent::MouseMove)
        {
            QToolTip::hideText();
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void XMasterWindow::onLowerPositionChanged(int value)
{
    control_window_->setHeightMinText(FLSliderPositionChanged(value));
}

void XMasterWindow::onUpperPositionChanged(int value)
{
    control_window_->setHeightMaxText(FLSliderPositionChanged(value));
}

QString XMasterWindow::FLSliderPositionChanged(int value)
{
    QString value_text;
    QString tool_tip_text;
    if (IsSISet(SIFeet)) {
        tool_tip_text = value_text = QString("%1").arg(value, 3, 10, QLatin1Char('0'));
        tool_tip_text += QString("%1").arg(tr("FL"));
    } else {
        tool_tip_text = value_text = QString("%1").arg(qRound(value*30.48), 3, 10, QLatin1Char('0'));
        tool_tip_text += QString("%1").arg(tr("M"));
    }
    QToolTip::showText(QCursor::pos(), tool_tip_text, m_FLSlider);
    return value_text;
}

void XMasterWindow::OnShowToolButMenu()
{
    QMenu* pMenu = static_cast<QMenu*>(sender());
    if (pMenu) {
        pMenu->addAction(m_ExitAction);
        pMenu->removeAction(m_ExitAction);
    }
}

void XMasterWindow::_OnCustomSector()
{
    uint idFir = 0;
    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(GetView());
    if(view && view->GetMainFIR())
        idFir = view->GetMainFIR()->GetID();
    CustomSector dlg(idFir, this);
    if(dlg.exec() == QDialog::Accepted)
    {
        QList<uint> mainFIR;
        if(dlg.GetIdFIR())
            mainFIR.append(dlg.GetIdFIR());
        networkSettings()->setValue("users/current_sector_type", dlg.GetSectorType());
        _LoadEnvironment(dlg.GetPrivileges(), mainFIR, dlg.GetSectors());
        //_FindOrSetCustomSector();
         _SetCustomSector();
    }
}

void XMasterWindow::_OnViewCWP()
{
    QDialog dlg(this);
    NodesViewer view(&dlg);
    QVBoxLayout layout(&dlg);
    layout.setContentsMargins(0, 0, 0, 0);
    layout.addWidget(&view);
    dlg.setLayout(&layout);
    dlg.adjustSize();
    dlg.resize(QSize(400,300));
    dlg.setWindowTitle(view.windowTitle());
    dlg.exec();
}

void XMasterWindow::_SetCustomSector()
{
    if (m_pSectoringCombo)
    {
        int ind = m_pSectoringCombo->count() - 1;
        for (int i = 0; i < m_pSectoringCombo->count(); ++i)
        {
            if (-1 == m_pSectoringModel->record(i).value("id").toInt())
            {
                ind = i;
                break;
            }
        }
        if (ind != -1) {
            m_pSectoringCombo->setCurrentIndex(ind);
            control_window_->setSectorCurrentIndex(ind);
        }
    }
    CloseDoc(XDocWidget::ListActIn);
    CloseDoc(XDocWidget::ListActOut);
    CloseDoc(XDocWidget::ListActDep);
    CloseDoc(XDocWidget::MeteoRunwayInfo);
    CloseDoc(XDocWidget::MeteoRunwayInfo2);
    CloseDoc(XDocWidget::Runway);
    control_window_->MeteoVisible(false);
    CloseDoc(XDocWidget::UserPicsList);
    CloseDoc(XDocWidget::ListDailyPlan);
    CloseDoc(XDocWidget::ListCost);
}

void XMasterWindow::_SelectSectorModel()
{
    for (int i = 0; i < m_pSectoringModel->rowCount(); ++i)
    {
        if (0 == m_pSectoringModel->data(m_pSectoringModel->index(i, m_pSectoringModel->fieldIndex("id"))).toInt())
        {
            m_pSectoringModel->setData(m_pSectoringModel->index(i, m_pSectoringModel->fieldIndex("name")), tr("Empty"));
        }
        if (-1 == m_pSectoringModel->data(m_pSectoringModel->index(i, m_pSectoringModel->fieldIndex("id"))).toInt())
        {
            m_pSectoringModel->setData(m_pSectoringModel->index(i, m_pSectoringModel->fieldIndex("name")), tr("Custom"));
        }
    }
}

void XMasterWindow::OnToolTipCustomSector()
{
    QString sText = networkSettings()->value("users/current_sector").toString() + "   "
                    + networkSettings()->value("users/current_sectors_short_name").toString();
    m_pCustomSector->setToolTip(sText);
}

void XMasterWindow::_UndoSectoring()
{
    QMetaObject::invokeMethod(this, "_onUndoSectoring", Qt::QueuedConnection);
}

void XMasterWindow::_onUndoSectoring()
{
    QList<uint> listPrevSectors = _SendPrevSectorList();
    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(GetView());
    if(view && view->GetMainFIR())
    {
        CView::TSceneList::iterator itFIR = view->GetScenes().begin();
        for(; itFIR != view->GetScenes().end(); ++itFIR)
        {
            Scene::TSceneList::iterator itSector = (*itFIR)->GetSectorList().begin();
            for(; itSector != (*itFIR)->GetSectorList().end(); ++itSector)
                (*itSector)->MarkItemActive(false);
        }
        QList<uint>::iterator itSector = listPrevSectors.begin();
        for (; itSector != listPrevSectors.end(); ++itSector)
        {
            itFIR = view->GetScenes().begin();
            for(; itFIR != view->GetScenes().end(); ++itFIR)
            {
                Scene::TScenePtr pscene = (*itFIR)->FindSector(*itSector);
                if (pscene)
                    pscene->MarkItemActive(true);
            }
        }
        SetCurSectorAirports();
        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pView)
        {
            pView->Draw();
            pView->Redraw();
        }
    }
    _FindOrSetCustomSector();
}

void XMasterWindow::ForceSectoring()
{
    OLDIMesDataSTS dataSTS = OLDIMesSTS(sSectOLDIMes).GetDataSTS();
    sSectOLDIMes = QString::null;
    dataSTS.iMesNum = GetOLDIProc()->GetNextMesNum();
    dataSTS.iPriority = 1;
    QSharedPointer<OLDIMessage> pMesSTS(new OLDIMesSTS(dataSTS));
    connect(pMesSTS.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), this, SLOT(OLDIMesAnswer(const OLDIMessage*,const QStringList&)));
    connect(pMesSTS.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), this, SLOT(OLDIMesTimeout(const OLDIMessage*)));
    m_listMes.append(pMesSTS.data());
    GetOLDIProc()->SendMessage(pMesSTS);
}

void XMasterWindow::OnDataSourceMenu()
{
    QAction* pDefAction = m_pDataSource->defaultAction();
    if(pDefAction)
    {
        QMenu* pMenu = pDefAction->menu();
        if(pMenu)
        {
            pMenu->clear();
            QString sFDP = networkSettings()->value("oldi/fdp_name", "FDP").toString();
            QStringList listFDP = sFDP.split(",", QString::SkipEmptyParts);
            QString curFDP = networkSettings()->value("users/current_fdp_name", "FDP").toString();
            if(listFDP.size() && !listFDP.contains(curFDP))
                    curFDP = listFDP[0];
            networkSettings()->setValue("users/current_fdp_name", curFDP);
            for(int i = 0; i < listFDP.size(); ++i)
            {
                QAction* pAction = pMenu->addAction(listFDP[i]);
                pAction->setCheckable(true);
                pAction->setChecked(listFDP[i] == curFDP);
                connect(pAction, SIGNAL(triggered()), this, SLOT(OnDataSourceMenuFDP()));
            }

            pMenu->addSeparator();

            QStringList listRDP;
            listRDP << tr("Main RDP") << tr("By pass");
            QString curRDP = networkSettings()->value("users/current_rdp_name", "Main RDP").toString();
            if(listRDP.size() && !listRDP.contains(curRDP))
                    curRDP = listRDP[0];
            networkSettings()->setValue("users/current_rdp_name", curRDP);
            for(int i = 0; i < listRDP.size(); ++i)
            {
                QAction* pAction = pMenu->addAction(listRDP[i]);
                pAction->setCheckable(true);
                pAction->setChecked(listRDP[i] == curRDP);
                connect(pAction, SIGNAL(triggered()), this, SLOT(OnDataSourceMenuRDP()));
            }
        }
    }
}

void XMasterWindow::OnDataSourceMenuFDP()
{
    QAction* pAction = static_cast<QAction*>(sender());
    if(pAction)
    {
        networkSettings()->setValue("users/current_fdp_name", pAction->text());
    }
}

void XMasterWindow::OnDataSourceMenuRDP()
{
    QAction* pAction = static_cast<QAction*>(sender());
    if(pAction)
    {
        networkSettings()->setValue("users/current_rdp_name", pAction->text());
    }
}

void XMasterWindow::SetCurSectorAirports()
{
    QStringList listAirports;
    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(GetView());
    if(view && view->GetMainFIR())
    {
        Scene::TAirportsMap::const_iterator itAirport = view->GetMainFIR()->GetAirports().begin();
        for (; itAirport != view->GetMainFIR()->GetAirports().end(); ++itAirport)
        {
            Scene::TSceneList::iterator itSector = view->GetMainFIR()->GetSectorList().begin();
            for(; itSector != view->GetMainFIR()->GetSectorList().end(); ++itSector)
            {
                if((*itSector)->IsItemActive())
                {
                    if((*itSector)->IsIncluded((*itAirport)->GetCoord()))
                    {
                        listAirports.append((*itAirport)->GetName());
                        break;
                    }
                }
            }
        }
    }
    networkSettings()->setValue("users/current_sector_airports", listAirports.join(","));
}

void XMasterWindow::_OnSaveFocus()
{
    if (QGraphicsView *view = dynamic_cast<QGraphicsView*> (centralWidget())) {
        m_SavedZoom = m_ZoomSlider->value();
        m_SavedPoint = view->mapToScene(view->viewport()->rect().center());
        QByteArray arr;
        QDataStream ds(&arr, QIODevice::WriteOnly);
        ds << m_SavedPoint;
        globalSettings()->setValue("XMasterWindow/Settings/FocusSavedPoint", arr);
        globalSettings()->setValue("XMasterWindow/Settings/FocusSavedZoom", m_SavedZoom);

        if (CGUIView *guiView = dynamic_cast<CGUIView*>(view))
            guiView->onHighlightCenter();
    }
}

void XMasterWindow::_OnRestoreFocus()
{
    if (QGraphicsView *view = dynamic_cast<QGraphicsView*> (centralWidget())) {
        m_ZoomSlider->setValue(m_SavedZoom);
        control_window_->setZoom(m_SavedZoom);
        view->centerOn(m_SavedPoint);
    }
    if (m_ZonesView)
        m_ZonesView->SetFormularDefaultPos();
}

void XMasterWindow::_OnRestoreFocusCentralPoint()
{
   control_window_->setZoom(0);
}

void XMasterWindow::_OnRestoreFormular(bool check)
{
    Q_UNUSED(check)

    control_window_->setAutoAntiOverlapChecked(false);

    qreal angle = formularLinkAngle();
    foreach (const QSharedPointer<CAirplane> &airplane, m_ZonesView->GetAirplanes().values())
    {
        airplane->setFixedFormular(false);
        airplane->setFormularPos(angle, formularLinkLength());
    }
}

void XMasterWindow::_OnRestrictionAdmin()
{
    if (!gWidgetRestriction)
        OpenDoc(XDocWidget::RestrictionsAdminEditor);
}

void XMasterWindow::CleanActiveSector()
{
    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(GetView());
    if (view && view->GetMainFIR())
    {
        CView::TSceneList::iterator itFIR = view->GetScenes().begin();
        for (; itFIR != view->GetScenes().end(); ++itFIR)
        {
            Scene::TSceneList::iterator itSector = (*itFIR)->GetSectorList().begin();
            for (; itSector != (*itFIR)->GetSectorList().end(); ++itSector)
                (*itSector)->MarkItemActive(false);
        }
        SetCurSectorAirports();
        if (CGUIView *gui_view = dynamic_cast<CGUIView*>(m_ZonesView.data())) {
            gui_view->Draw();
            gui_view->Redraw();
        }
    }
    _SendSectorsList(QList<uint>());
}

void XMasterWindow::LogUserLogon(bool bLogin, const QString &sUser)
{
    Log4Qt::LogManager::rootLogger()->log(Log4Qt::Level::FATAL_INT,
                                          tr("=======User %1 %2 %3=======")
                                          .arg(sUser)
                                          .arg(bLogin ? tr("login") : tr("logout"))
                                          .arg(QDateTime::currentDateTime().toString()));

    Log4Qt::LogManager::rootLogger()->log(Log4Qt::Level::FATAL_INT,
                                          tr("<usr>%1</usr> %2")
                                          .arg(sUser)
                                          .arg(bLogin ? tr("<login>") : tr("<logout>")));

    logOperation(tr("=======User %1 %2 %3=======")
                 .arg(sUser)
                 .arg(bLogin ? tr("login") : tr("logout"))
                 .arg(QDateTime::currentDateTime().toString()));

    // Layers
    QSettings &settings = *globalSettings();
    settings.beginGroup(sUser);
    if (bLogin)
    {
        QVariant var;
        var = settings.value("Layers");
        QDataStream qStream(var.toByteArray());
        LayoutDlg::Load(qStream);
        if (m_ZonesView) {
            if (CGUIView *pView = dynamic_cast<CGUIView*>(m_ZonesView.data()))
                pView->SetLayouts();
        }
    }
    else
    {
        QByteArray qArray;
        QDataStream qStream(&qArray, QIODevice::ReadWrite);
        LayoutDlg::Save(qStream);
        settings.setValue("Layers", qArray);
    }
    settings.endGroup();
}

void XMasterWindow::RequestAllFPL()
{
    // request all FPLs
    OLDIMesDataINF mesData;
    mesData.iMesNum = GetOLDIProc()->GetNextMesNum();
    mesData.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
    mesData.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
    mesData.sTypeINF = oldi::INF_FPL;
    mesData.sACID = " ";
    mesData.sADEP = " ";
    mesData.sADES = " ";
    mesData.sCODE = " ";
    mesData.sSector = " ";
    QSharedPointer<OLDIMessage> pMes(new OLDIMesINF(mesData));
    //connect(pMes.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), this, SLOT(OLDIMesAnswer(const OLDIMessage*,const QStringList&)));
    //connect(pMes.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), this, SLOT(OLDIMesTimeout(const OLDIMessage*)));
    GetOLDIProc()->SendMessage(pMes);
}

void XMasterWindow::_FindOrSetCustomSector()
{
    QStringList sSectorShortNames = networkSettings()->value("users/current_sectors_short_name").toString().split("-", QString::SkipEmptyParts);
    if(sSectorShortNames.size())
    {
        TVA_CATCHALL_TRY
        {
            QList<uint> listSectorIDShortName;
            QList<Zone> Zones;
            dao()->LoadZones(Zones);
            for(QStringList::const_iterator itSector = sSectorShortNames.constBegin(); itSector != sSectorShortNames.constEnd(); ++itSector)
            {
                for(QList<Zone>::const_iterator itZone = Zones.constBegin(); itZone != Zones.constEnd(); ++itZone)
                {
                    if(*itSector == itZone->GetShortName())
                    {
                        listSectorIDShortName.append(itZone->GetID());
                        break;
                    }
                }
            }
            TVA_CHECK(sSectorShortNames.size() == listSectorIDShortName.size(), "Find sector error.", E_FAIL);

            Dao::TSectorZoneList mapSectors;
            dao()->LoadSectorZoneAll(mapSectors);

            QMap<uint, QSharedPointer<CSectorZone> > mapFirstCandidates;
            QMap<uint, QSharedPointer<CSectorZone> > mapSecondCandidates;
            for(Dao::TSectorZoneList::const_iterator itSector = mapSectors.constBegin(); itSector != mapSectors.constEnd(); ++itSector)
            {
                if(listSectorIDShortName.first() == (*itSector)->GetZoneID())
                {
                    if((*itSector)->GetMainFIR())
                        mapFirstCandidates.insert(itSector.key(), *itSector);
                    else
                        mapSecondCandidates.insert(itSector.key(), *itSector);
                }
            }
            QList<uint> listFindID;
            /*if(!*/_FindSectorID(mapFirstCandidates.uniqueKeys(), listSectorIDShortName, listFindID);//)
                //;//_FindSectorID(mapSecondCandidates.uniqueKeys(), listSectorIDShortName, listFindID);

            if(listFindID.size())
            {
                if(m_pSectoringCombo)
                {
                    int ind = -1;
                    for(int i = 0; i < m_pSectoringCombo->count(); ++i)
                    {
                        if(listFindID.first() == m_pSectoringModel->record(i).value("id").toUInt())
                        {
                            ind = i;
                            break;
                        }
                    }
                    if(ind != -1)
                    {
                        m_pSectoringCombo->blockSignals(true);
                        m_pSectoringCombo->setCurrentIndex(ind);
                        m_pSectoringCombo->blockSignals(false);

                        control_window_->blockSignals(true);
                        control_window_->setSectorCurrentIndex(ind);
                        control_window_->blockSignals(false);

                        if(!sSectOLDIMes.isNull())
                        {
                            QSharedPointer<obelix::CCmdSectorChanged> pCmd(new obelix::CCmdSectorChanged(m_pSectoringCombo->currentText()));
                            GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
                            sSectOLDIMes = QString::null;
                        }
                        QString privileges;
                        privileges = dao()->LoadSectoringTypePrivilege(listFindID.first());
                        loadListPrivileges(privileges);
                        return;
                    }
                }
            }
        }TVA_CATCHALL(err){
        LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
    }
    else
        _SetCustomSector();
}

void XMasterWindow::LoadEnvironment(const QString &sIndex)
{
    // fuck redis
    CGUIView *guiView = dynamic_cast<CGUIView*>(m_ZonesView.data());
    if (guiView && guiView->IsDrawing())
    {
        QMetaObject::invokeMethod(this, "LoadEnvironment", Qt::QueuedConnection, Q_ARG(QString, sIndex));
        return;
    }

    try
    {
    if(!m_pSectoringCombo)
        return;

    if (!connected_to_db_) {
        const int index = m_pSectoringCombo->findText(sCurrentSector);
        m_pSectoringCombo->setCurrentIndex(index);
        control_window_->setSectorCurrentIndex(index);
        return;
    }

    if(sIndex == QString("null"))
    {// make all sectors inactive
        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(GetView());
        if(view && view->GetMainFIR())
        {
            CView::TSceneList::iterator itFIR = view->GetScenes().begin();
            for(; itFIR != view->GetScenes().end(); ++itFIR)
            {
                Scene::TSceneList::iterator itSector = (*itFIR)->GetSectorList().begin();
                for(; itSector != (*itFIR)->GetSectorList().end(); ++itSector)
                    (*itSector)->MarkItemActive(false);
            }
            CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
            if(pView)
            {
                pView->Draw();
                pView->Redraw();
            }
        }
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    int iIndex = m_pSectoringCombo->findText(sIndex);
    TVA_CATCHALL_TRY
    {
        if (m_pSectoringCombo->count() > 0 && iIndex >= 0 && iIndex < m_pSectoringCombo->count())
        {
            setCurrentSectorId(m_pSectoringModel->record(iIndex).value("id").toInt());
            _LoadEnvironment(m_pSectoringModel->record(iIndex).value("id").toInt());
            //networkSettings()->setValue("users/current_sector", m_pSectoringCombo->itemText(iIndex));
            m_pSectoringCombo->blockSignals(true);
            m_pSectoringCombo->setCurrentIndex(iIndex);
            m_pSectoringCombo->blockSignals(false);
            if (sCurrentSector != m_pSectoringCombo->currentText()) {
                sCurrentSector = m_pSectoringCombo->currentText();
                logOperation(tr("Current sector changed to '%1'").arg(sCurrentSector));
            }
        }
        else
        {
            //Pasha may be not in this place
            _SendSectorsList(QList<uint>());
        }
    }TVA_CATCHALL(err){
    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
        QMessageBox::critical(this, tr("Error"), err.reason());
    }
    QApplication::restoreOverrideCursor();

    }
    catch(...){}
}

QString XMasterWindow::GetCurrentSector()
{
    return sCurrentSector;
}

void XMasterWindow::SetTempStatusText(QString text)
{
    m_pStatusTempText->setText(text);
}

bool XMasterWindow::_FindSectorID(QList<uint> listUserSectors, QList<uint> listSectors, QList<uint>& listFind)
{
    TVA_CATCHALL_TRY
    {
        for(QList<uint>::const_iterator itUserSector = listUserSectors.constBegin(); itUserSector != listUserSectors.constEnd(); ++itUserSector)
        {
            QList<uint> listFIR;
            dao()->LoadIdFIREnvironment(*itUserSector, listFIR);
            QList<uint> listFIRSectors;
            QList<uint> listTemp;
            for(QList<uint>::const_iterator itFIR = listFIR.constBegin(); itFIR != listFIR.constEnd(); ++itFIR)
            {
                dao()->LoadIdSectorsEnvironment(*itUserSector, *itFIR, listTemp);
                listFIRSectors.append(listTemp);
            }

            bool bFind = true;
            for(QList<uint>::const_iterator itSector = listFIRSectors.constBegin(); bFind && itSector != listFIRSectors.constEnd(); ++itSector)
            {
                if(!listSectors.contains(*itSector))
                    bFind = false;
            }
            for(QList<uint>::const_iterator itSector = listSectors.constBegin(); bFind && itSector != listSectors.constEnd(); ++itSector)
            {
                if(!listFIRSectors.contains(*itSector))
                    bFind = false;
            }
            if(bFind)
                listFind.append(*itUserSector);
        }
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
    return listFind.size();
}

void XMasterWindow::OnPulsarChanged(const QHostAddress &newpulsar, const QString &status, const QHostAddress &oldpulsar, const QVariantHash &info)
{
    QString sFDP = info[PulsarSettings::fdpName].toString();
    if(!sFDP.isEmpty() && networkSettings()->value("oldi/fdp_name", "FDP").toString().split(",", QString::SkipEmptyParts).contains(sFDP))
    {
        networkSettings()->setValue("users/current_fdp_name", sFDP);
    }
}

void XMasterWindow::OnShutdown(QSessionManager& manager)
{
    m_bShutdown = true;
}

void XMasterWindow::OnSuperStop(const QString &s)
{
    if(s == "stop")
    {
        m_bShutdown = true;
        qApp->quit();
    }
}

void XMasterWindow::_LoadGroundView(const QString &name)
{
    BlockNetworkMessages(true);

    m_LayoutsMainFIRAction->setVisible(false);

    SceneClean();
    QSharedPointer<GUIGroundScene> newView(new GUIGroundScene(this));
    SetPlanes(&newView->GetAirplanes());

    newView->SetEchelon(GetEchelonValue(IsSISet(AppSettings::SIFeet)));
    newView->SetPress(m_PressSpin->value());
    newView->SetGauge(m_CheckGauge->checkState() == Qt::Checked);

    QGraphicsScene* newScene = new QGraphicsScene(0, 0, WINDOW_SIZE, WINDOW_SIZE, newView.data());
    newView->setScene(newScene);
    CView::TScenePtr fir(new CGUIScene());
    newView->setName(name);
    GetAirportDataFromXML(*newView);

    newView->setMinimumSize(WINDOW_SIZE + 2, WINDOW_SIZE + 2);
    newView->Prepare();
    newView->SetLayouts();

    m_ZonesView = newView;

    //newView->setBackgroundBrush(UserProfile::CSettings::Instance().GetColor(UserProfile::CtBackground));
    newView->setBackgroundBrush(QBrush(QColor(216, 216, 216)));
    setCentralWidget(newView.data());
    setWindowTitle(newView->name() + tr(" Ground airport view"));

//    m_AddAction->setEnabled(true);

    if(getRedisDB())
    {
        RunwayWidgetDataVPP rwf(fir->GetIDName());
        if(getRedisDB()->loadRWFVPP(fir->GetIDName(), rwf))
        {
            m_mapEchelon.insert(fir->GetIDName(), rwf.TL());
            m_mapPressure.insert(fir->GetIDName(), rwf.QNH());
        }
    }
    int indexEchelon = GetEchelonComboIndex(m_mapEchelon.value(fir->GetIDName(), GetEchelonValue(IsSISet(AppSettings::SIFeet))));
    if(indexEchelon != -1)
    {
        m_EchelonCombo->setCurrentIndex(indexEchelon);
        _OnChangeEchelon(indexEchelon);
    }
    m_PressSpin->setValue(m_mapPressure.value(fir->GetIDName(), m_PressSpin->value()));

    setCentralWidget(newView.data());

    BlockNetworkMessages(false);
}

void XMasterWindow::_ToggleScrollTrigerred()
{
    CListAC::toggleScroll();
}

void XMasterWindow::StopExercise(bool bSameDB)
{
    CMaster::StopExercise(bSameDB);
    if(GetView())
    {
        CListAC::RefreshAndSelect(0);
//        CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
//        if (pView)
        if(m_ZonesView)
        {
            while(GetView()->GetAirplanes().size())
                m_ZonesView->DelAirplane(*GetView()->GetAirplanes().begin());
        }
    }
}

void XMasterWindow::_OnRefreshSFPL()
{
    StopExercise(false);
    //RequestAllFPL();
}

int XMasterWindow::GetEchelonValue(bool bFeet) const
{
    int iVal = 0;
    if(m_EchelonCombo)
    {
        int ind = m_EchelonCombo->currentIndex();
        if(ind != -1)
        {
            iVal = m_EchelonCombo->itemData(ind).toInt();
            if(!iVal)
            {
                iVal = m_EchelonCombo->currentText().toInt();
                if(!bFeet)
                    iVal = qRound(iVal/3.048);
            }
        }
    }
    return iVal;
}

void XMasterWindow::updatePlanList(QString sType)
{
    XDocWidget::DocType type(XDocWidget::ListACC);
    if(sType == QString("ACC")){
        type = XDocWidget::ListACC;
    } else
    if(sType == QString("SIL_ACC")){
        type = XDocWidget::ListSIL_ACC;
    } else
    if(sType == QString("APP")){
        type = XDocWidget::ListAPP;
    } else
    if(sType == QString("SIL_APP_ARR")){
        type = XDocWidget::ListSIL_APP_ARR;
    } else
    if(sType == QString("SIL_APP_DEP")){
        type = XDocWidget::ListSIL_APP_DEP;
    } else
    {
        // unknown list type
        LOG_STR(Log4Qt::Level::WARN_INT, tr("Unknown list type to update"));
        return;
    }
    QMap<XDocWidget::DocType, QSharedPointer<QDockWidget> >::iterator itr = m_mapDoc.find(type);
    if (itr != m_mapDoc.end())
        ((CListAC*)(*itr)->widget())->UpdateViewData();
}

void XMasterWindow::EndSimMeteo(QString sIdFIR, bool bSameDB)
{
    CMaster::EndSimMeteo(sIdFIR, bSameDB);
    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(GetView());
    if(view && view->GetMainFIR() && view->GetMainFIR()->GetIDName() == sIdFIR)
    {
        CGUIView* pView = dynamic_cast<CGUIView*> (GetView().data());
        if (pView)
            pView->OnMeteoRefresh();
    }
}

void XMasterWindow::OnBearingData(CBearingData& Data)
{
    CGUIView *view = dynamic_cast<CGUIView*>(m_ZonesView.data());
    if (view)
    {
        view->BearingData(Data);
    }
}

void XMasterWindow::_OnRcGridCoord()
{
    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(GetView());
    if(view && view->GetMainFIR())
    {
        //InputCoordDlg dlg(this, tr("RcGrid center coordinate"));
        RcGridCoordDlg dlg(this);
        RcGridValues values = CGUIRcGrid::LoadRcGridValues(view->GetMainFIR()->GetIDName());
        dlg.SetRcGridValues(values);
        if(dlg.exec() == QDialog::Accepted)
        {
            CGUIView* pView = dynamic_cast<CGUIView*> (GetView().data());
            if (pView)
                pView->SetRcGridCenter(dlg.GetRcGridValues());
        }
    }
}

void XMasterWindow::_OnDF()
{
    QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(GetView());
    if(view && view->GetMainFIR())
    {
        CGUIView* pView = dynamic_cast<CGUIView*> (GetView().data());
        if (pView)
        {
            CBearingDlg dlg(view->GetMainFIR()->GetIDName(), pView->GetListDF(), this);
            dlg.exec();
        }
    }
}

void XMasterWindow::SetEchelonString()
{
    if(m_pEchelonBut && m_EchelonCombo)
    {
        int ind = m_EchelonCombo->currentIndex();
        if(ind != -1)
        {
            QString sText = m_EchelonCombo->itemText(ind);
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(GetView());
            if(view && view->GetMainFIR())
            {
                CGUIView* pView = dynamic_cast<CGUIView*> (GetView().data());
                if (pView)
                {
                    int iPress = pView->GetPress();
                    if(iPress > 0 /*&& iPress != 760*/)
                    {
                        int iEchelon = pView->GetEchelon();
                        double height = iEchelon * 100 * 0.3048;
                        //QFE
                        //height -= (760-iPress)*11;
                        //QNH
                        height += 8.25*(iPress-1013);
                        int iNewEchelon = qRound(height / (100 * 0.3048));
                        if(iNewEchelon != iEchelon)
                        {
                            sText += "(A";
                            sText += QString("%1").arg(IsSISet(AppSettings::SIFeet) ? iNewEchelon : qRound(iNewEchelon*3.048)/*, 3, 10, '0'*/);
                            sText += ")";
                        }
                    }
                }
            }
            m_pEchelonBut->setText(sText);
            //control_window_->setFlightLevel(sText);
            m_pEchelonBut->adjustSize();
        }
    }
}

void XMasterWindow::loadListPrivileges(const QString &privileges)
{
    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTACC_UNIQUE))
        OpenDoc(XDocWidget::ListACC);
    else
        CloseDoc(XDocWidget::ListACC);

    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTSILACC_UNIQUE))
        OpenDoc(XDocWidget::ListSIL_ACC);
    else
        CloseDoc(XDocWidget::ListSIL_ACC);

    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTAPP_UNIQUE))
        OpenDoc(XDocWidget::ListAPP);
    else
        CloseDoc(XDocWidget::ListAPP);

    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTSILAPPARR_UNIQUE))
        OpenDoc(XDocWidget::ListSIL_APP_ARR);
    else
        CloseDoc(XDocWidget::ListSIL_APP_ARR);

    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTSILAPPDEP_UNIQUE))
        OpenDoc(XDocWidget::ListSIL_APP_DEP);
    else
        CloseDoc(XDocWidget::ListSIL_APP_DEP);

    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTACTIN_UNIQUE))
        OpenDoc(XDocWidget::ListActIn);
    else
        CloseDoc(XDocWidget::ListActIn);

    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTACTOUT_UNIQUE))
        OpenDoc(XDocWidget::ListActOut);
    else
        CloseDoc(XDocWidget::ListActOut);

    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTACTDEP_UNIQUE))
        OpenDoc(XDocWidget::ListActDep);
    else
        CloseDoc(XDocWidget::ListActDep);

    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTCOST_UNIQUE))
        OpenDoc(XDocWidget::ListCost);
    else
        CloseDoc(XDocWidget::ListCost);

    // Meteo information
    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEO_UNIQUE)) {
       QString runway1 = networkSettings()->value("RW1/name").toString();
       if(!runway1.isEmpty())
            OpenDoc(XDocWidget::MeteoRunwayInfo);
       QString runway2 = networkSettings()->value("RW2/name").toString();
       if(!runway2.isEmpty())
            OpenDoc(XDocWidget::MeteoRunwayInfo2);
    }
    else {
        CloseDoc(XDocWidget::MeteoRunwayInfo);
        CloseDoc(XDocWidget::MeteoRunwayInfo2);
    }

    // Restriction zone editor
    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTRESTRICTION_UNIQUE))
        OpenDoc(XDocWidget::RestrictionsAdminEditor);
    else
        CloseDoc(XDocWidget::RestrictionsAdminEditor);

    if (CUserRole::IsPermission(privileges, CSectoringModel::RUNWAY_UNIQUE))
    {
        OpenDoc(XDocWidget::Runway);
        control_window_->MeteoVisible(true);
        control_window_->MeteoCheck();
    }
    else
    {
        CloseDoc(XDocWidget::Runway);
        control_window_->MeteoVisible(false);
    }

    //DAILY PLAN
    if (CUserRole::IsPermission(privileges, CSectoringModel::DAILYPLAN_UNIQUE))
        OpenDoc(XDocWidget::ListDailyPlan);
    else
        CloseDoc(XDocWidget::ListDailyPlan);
}

void XMasterWindow::loadToolbarPrivileges(const QString &privileges)
{
    // Toolbars
    addToolBar(m_MainBar);
    //m_MainBar->setVisible(true);
    control_window_->setMainFrameVisible(true);
    if (CUserRole::IsPermission(privileges, CSectoringModel::TOOLBPRESSURE_UNIQUE))
    {
        addToolBar(m_pPressureBar);
        //m_pPressureBar->setVisible(true);
        //control_window_->setPressureLevelFrameVisible(true);
    }
    else
    {
        removeToolBar(m_pPressureBar);
    }
    if (CUserRole::IsPermission(privileges, CSectoringModel::TOOLBPAIRPLANELENGTH_UNIQUE))
    {
        addToolBar(airplaneLengthBar);
        //airplaneLengthBar->setVisible(true);
        control_window_->setAirplaneLengthFrameVisible(true);
    }
    else
    {
        removeToolBar(airplaneLengthBar);
    }
    if (CUserRole::IsPermission(privileges, CSectoringModel::TOOLBCUSTOMSECTOR_UNIQUE))
    {
        addToolBar(m_pCustomSectorBar);
        //m_pCustomSectorBar->setVisible(true);
        //control_window_->setCustomSectorVisible(true);
    }
    else
    {
        removeToolBar(m_pCustomSectorBar);
    }
    if (CUserRole::IsPermission(privileges, CSectoringModel::TOOLBALTITUDEFILTER_UNIQUE))
    {
        addToolBar(m_pAltitudeFilterBar);
        //m_pAltitudeFilterBar->setVisible(true);
        control_window_->setHeightFilterFrameVisible(true);
    }
    else
    {
        removeToolBar(m_pAltitudeFilterBar);
    }
    if (CUserRole::IsPermission(privileges, CSectoringModel::TOOLBRESTRICTION_UNIQUE))
    {
        addToolBar(m_pRestrictionBar);
        //m_pRestrictionBar->setVisible(true);
        control_window_->setRestrictionsEditorVisible(true);
    }
    else
    {
        removeToolBar(m_pRestrictionBar);
    }
    if (CUserRole::IsPermission(privileges, CSectoringModel::TOOLBGROUND_UNIQUE))
    {
        addToolBar(m_pGroundBar);
        //m_pGroundBar->setVisible(true);
    }
    else
    {
        removeToolBar(m_pGroundBar);
    }
}

void XMasterWindow::StandChangedGUI(const GroundStand& stand)
{
    if (stand.mId == 0)
    {
        m_bStandTape = true;
        m_mapGUIStand.clear();
    }
    else if (stand.mId == UINT_MAX)
    {
        m_bStandTape = false;
        QMap<XDocWidget::DocType, QSharedPointer<QDockWidget> >::iterator itDoc = m_mapDoc.find(XDocWidget::StandList);
        if (itDoc != m_mapDoc.end()) {
            if (StandWidget *pStandWidget = qobject_cast<StandWidget*>((*itDoc)->widget()))
                pStandWidget->SetModelData(m_mapGUIStand);
        }

        CListAC::GetSourceModel().SetStand(m_mapGUIStand);
    }
    else
    {
        QSharedPointer<GroundStand> pStand(new GroundStand(stand));
        m_mapGUIStand[stand.mId] = pStand;
        if (!m_bStandTape)
        {
            QMap<XDocWidget::DocType, QSharedPointer<QDockWidget> >::iterator itDoc = m_mapDoc.find(XDocWidget::StandList);
            if (itDoc != m_mapDoc.end()) {
                if (StandWidget *pStandWidget = qobject_cast<StandWidget*>((*itDoc)->widget()))
                    pStandWidget->SetModelData(pStand);
            }

            CListAC::GetSourceModel().SetStand(pStand);
        }
    }
}

int XMasterWindow::GetEchelonComboIndex(int FL)
{
    int iRes = -1;
    if (m_EchelonCombo)
    {
        for (int i = 0; i < m_EchelonCombo->count(); ++i)
        {
            if (FL == m_EchelonCombo->itemData(i).toInt())
            {
                iRes = i;
                break;
            }
        }
    }
    return iRes;
}

void XMasterWindow::loadShortcuts()
{
    m_Shortcuts[QString("RestoreLabels")] = networkSettings()->value(QString("hotkeys/RestoreLabels"), QL1S("Ctrl+Z")).toString();
    m_Shortcuts[QString("ExpandList")] = networkSettings()->value(QString("hotkeys/ExpandList"), QL1S("Ctrl+L")).toString();
    m_Shortcuts[QString("RunwayWidgetMinimize")] = networkSettings()->value(QString("hotkeys/RunwayWidgetMinimize"), QL1S("Esc")).toString();
}

void XMasterWindow::_SetChangedFont(const QFont& font)
{
    qApp->setFont(font);

    foreach (QWidget *widget, QApplication::allWidgets())
    {
        if(widget->objectName() != "lTime")
            widget->setFont(font);
        if(widget->objectName() == "PlanEditor")
        {
            QWidgetList childrenWidgets  = widget->findChildren<QWidget *>();
            QList<QLineEdit *> list =  widget->findChildren<QLineEdit *>();
            foreach(QLineEdit *child, list)
            {
                QFontMetrics fm(font);
                int lenM = 0;
                QString s = child->objectName();
                QString str = child->text();
                lenM = fm.width(str);
                child->setMinimumWidth(lenM+15);
                child->setMaximumWidth(lenM+15);
            }

            QList<QComboBox *> listBox =  widget->findChildren<QComboBox *>();
            foreach(QComboBox *child, listBox)
            {
                QFontMetrics fm(font);
                int lenM = 0;
                QString str = child->currentText();
                lenM = fm.width(str);
                child->setMinimumWidth(lenM+40);
                child->setMaximumWidth(lenM+40);
                child->setMinimumHeight(font.pointSize() + 10);
                child->setMaximumHeight(font.pointSize() + 12);
            }

            QList<QSpinBox *> listSpinBox =  widget->findChildren<QSpinBox *>();
            foreach(QSpinBox *child, listSpinBox)
            {
                QFontMetrics fm(font);
                int lenM = 0;
                QString str = child->text();
                lenM = fm.width(str);
                child->setMinimumWidth(lenM+33);
                child->setMaximumWidth(lenM+33);
                child->setMinimumHeight(font.pointSize() + 12);
                child->setMaximumHeight(font.pointSize() + 12);
            }

            QList<QPushButton *> listButton =  widget->findChildren<QPushButton *>();
            foreach(QPushButton *child, listButton)
            {
                QFontMetrics fm(font);
                int lenM = 0;
                QString str = child->text();
                lenM = fm.width(str);
                child->setMinimumWidth(lenM + 10);
                child->setMaximumWidth(lenM + 10);
                child->setMinimumHeight(font.pointSize() + 12);
                child->setMaximumHeight(font.pointSize() + 12);
            }

            int line_1_w = 30;
            int line_2_w = 15;
            foreach(QWidget *childWidget, childrenWidgets)
            {
                QString s = childWidget->objectName();
                int size_W = childWidget->size().width();
                if( s == "MesTypeCombo" )
                {
                    line_1_w += size_W;
                }
                if( s == "ACIDEdit" )
                {
                    line_1_w += size_W;
                }
                if( s == "CODEEdit" )
                {
                    line_1_w += size_W;
                }
                if( s == "RulesCombo" )
                {
                    line_1_w += size_W;
                }
                if( s == "FlightTypeCombo" )
                {
                    line_1_w += size_W;
                }

                if( s == "NumberEdit" )
                {
                    line_2_w += size_W;
                }
                if( s == "AirplaneCombo" )
                {
                    line_2_w += size_W;
                }
                if( s == "TurbulenceCombo" )
                {
                    line_2_w += size_W;
                }
                if( s == "Equipment2Edit" )
                {
                    line_2_w += size_W;
                }
                if( s == "Equipment3Edit" )
                {
                    line_2_w += size_W;
                }
            }

            QSize size = widget->size();
            if(line_1_w > line_2_w)
            {
                widget->setMaximumSize(line_1_w + 160, size.height());
                widget->setMinimumSize(line_1_w + 160, size.height());
            }
            else
            {
                if(line_2_w > 300)
                    line_2_w += 80;
                else
                    line_2_w += 50;

                widget->setMaximumSize(line_2_w + 110, size.height());
                widget->setMinimumSize(line_2_w + 110, size.height());
            }

            widget->setFont(font);
        }
    }

    SetXFont(AppSettings::FONT_CTRL_DLG_LIST, font);
    UpdateXFonts();

    QFont UFNFont = font;
    UFNFont.setPointSize(UFNFont.pointSize() * 1.2);
    UFNFont.setBold(true);
    m_pUserFullNameLabel->setFont(UFNFont);

    if (m_ZonesView)
    {
        CGUIView* pscene = dynamic_cast<CGUIView*> (m_ZonesView.data());
        if (pscene)
            pscene->RedrawAll();
    }
}

void XMasterWindow::_ListActColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtListAct), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtListAct, colorNew);
        stg.Save();
        CListAC::GetSourceModel().UpdateCustomColors();
    }
}

void XMasterWindow::_ListAssumeColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtListAssume), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtListAssume, colorNew);
        stg.Save();
        CListAC::GetSourceModel().UpdateCustomColors();
    }
}

void XMasterWindow::_ListTransferedColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtListTransfered), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtListTransfered, colorNew);
        stg.Save();
        CListAC::GetSourceModel().UpdateCustomColors();
    }
}

void XMasterWindow::_ListInColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtListIn), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtListIn, colorNew);
        stg.Save();
        CListAC::GetSourceModel().UpdateCustomColors();
    }
}

void XMasterWindow::_ListSelectedTextColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtListSelectedText), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtListSelectedText, colorNew);
        stg.Save();
        CListAC::GetSourceModel().UpdateCustomColors();
    }
}

void XMasterWindow::_BearingAirportFrequencyColorSlot()
{

    QAction* pAction = static_cast<QAction*>(sender());
    if (!pAction)
        return;
    QString frequency_name;
    frequency_name.clear();
    frequency_name = pAction->property("airport").toString() + "_" + pAction->text();

    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    stg.SetFrequency(frequency_name);
    stg.Load();

    QColor colorNew= QColorDialog::getColor(stg.GetColor(UserProfile::CtFrequency), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtFrequency, colorNew);
        stg.Save();
    }
}

void XMasterWindow::_GaugeColorSlot()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();

    QColor colorNew = QColorDialog::getColor(stg.GetColor(UserProfile::CtGauge), this, QLatin1String(""),
                                             QColorDialog::ShowAlphaChannel);
    if (colorNew.isValid())
    {
        stg.SetColor(UserProfile::CtGauge, colorNew);
        stg.Save();
        if (m_ZonesView)
        {
            CGUIView* pscene = dynamic_cast<CGUIView*> (m_ZonesView.data());
            if (pscene)
                pscene->UpdateGaugeColor();
        }
    }
}

void XMasterWindow::SetSIcombo()
{
    if (m_SICombo_M) {
        m_SICombo_M->setCurrentIndex(IsSISet(SIFeet) ? 1 : 0);
        control_window_->setHeightCurrentIndex(IsSISet(SIFeet) ? 1 : 0);
        m_SICombo_M->setEnabled(mSIcomboEnable);
    }
    if (m_SICombo_K) {
        m_SICombo_K->setCurrentIndex(IsSISet(SIKnot) ? 1 : 0);
        control_window_->setSpeedCurrentIndex(IsSISet(SIKnot) ? 1 : 0);
        m_SICombo_K->setEnabled(mSIcomboEnable);
    }
}

void XMasterWindow::_FMvalues()
{
    QString oldSI = getSI_FL();
    FMpassDlg dlg(this);
    dlg.exec();
    QString newSI = getSI_FL();
    if(oldSI != newSI)
    {
        setSI_FL(newSI);
        SetSIcombo();
    }
}

void XMasterWindow::onChangedSFPL(QSharedPointer<CSFPL> sfpl, QString operType)
{
    //temp
    CListAC::RefreshAndSelect(0);
    //need refresh current sfpl
    if(operType == "delete"){
        if (m_ZonesView) {
            if (CGUIView *view = dynamic_cast<CGUIView*> (m_ZonesView.data())){
                //view->OnDrawAirplanes();
                TAirplanesMap::const_iterator itAirplans = view->GetAirplanes().begin();
                for(; itAirplans != view->GetAirplanes().end(); ++itAirplans)
                {
                    CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>(itAirplans->data());
                    if(pPlane && pPlane->GetSFPL())
                        if(pPlane && pPlane->GetSFPL()->GetUid() == sfpl->GetUid())
                            pPlane->ClearSFPL();
                }
            }
        }
    }
}

void XMasterWindow::onToolboxHorizontalOrientation()
{
    control_window_->setOrientation(ControlWindow::horizontal);
}

void XMasterWindow::onToolboxVerticalOrientation()
{
    control_window_->setOrientation(ControlWindow::vertical);
}

void XMasterWindow::onToolboxOldSpeedDistanceMeasure()
{
    if(toolbox_Old_SpeedDistance_measure->isChecked())
    {
        control_window_->CreateOldButtons();
        networkSettings()->setValue("airplane/speed_distance_menu_type", "ACC");
    }
}

void XMasterWindow::onToolboxNewSpeedDistanceMeasure()
{
    if(toolbox_New_SpeedDistance_measure->isChecked())
    {
        control_window_->CreateNewButtons();
        networkSettings()->setValue("airplane/speed_distance_menu_type", "TMA");
    }
}

void XMasterWindow::onChangedRWF(QString idFIR, RunwayWidgetDataVPP rwf, QString operType)
{
    Q_UNUSED(operType);
    if (m_ZonesView)
    {
        if (QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(m_ZonesView))
        {
            if (pView->GetMainFIR()->GetIDName() == idFIR)
            {
                if(m_PressSpin)
                {
                    if(m_PressSpin->value() != rwf.QNH())
                    {
                        m_PressSpin->blockSignals(true);
                        m_PressSpin->setValue(rwf.QNH());
                        m_PressSpin->blockSignals(false);
                        _OnChangePress(rwf.QNH(), true);
                    }
                }
                if(m_EchelonCombo)
                {
                    int indexEchelon = GetEchelonComboIndex(rwf.TL());
                    if(indexEchelon != -1)
                    {
                        if(m_EchelonCombo->currentIndex() != indexEchelon)
                        {
                            m_EchelonCombo->blockSignals(true);
                            m_EchelonCombo->setCurrentIndex(indexEchelon);
                            m_EchelonCombo->blockSignals(false);
                            _OnChangeEchelon(indexEchelon, true);
                        }
                    }
                }
            }
        }
    }
}

void XMasterWindow::onORCAMDialog()
{
    showORCAMDialog();
}

void XMasterWindow::onChangedOrcam(QString code, QString assigned_time, QString otType)
{
  mORCAMDialog->ChangedOrcamRedis(code, assigned_time, otType);
}

void XMasterWindow::onChangedSectors(SectorRedis sr, QString operType)
{
    Q_UNUSED(operType);
    if (!m_ZonesView)
        return;
    CGUIView* pView = dynamic_cast<CGUIView*> (m_ZonesView.data());
    if (!pView)
        return;
    const QString sClient = networkSettings()->value("oldi/name", "Unknown").toString();
    QStringList sectors = networkSettings()->value("users/current_sectors_short_name").toString().split("-", QString::SkipEmptyParts);
    if (!sr.isEqualClientSectors(sClient, sectors))
    {
        QStringList sSectors = sr.getClientSectors(sClient);
        CView::TSceneList::iterator itFIR = pView->GetScenes().begin();
        for (; itFIR != pView->GetScenes().end(); ++itFIR)
        {//looking for active(selected) sector
            Scene::TSceneList& pSectorList = (*itFIR)->GetSectorList();
            Scene::TSceneList::iterator itSector = pSectorList.begin();
            for (; itSector != pSectorList.end(); ++itSector)
            {
                bool bActive = false;
                for (int i = 0; i < sSectors.size(); ++i)
                {
                    if (sSectors.at(i) == (*itSector)->GetShortName())
                    {
                        bActive = true;
                        break;
                    }
                }
                (*itSector)->MarkItemActive(bActive);
            }
        }

        sectors = sSectors;
        //store current sector in config
        networkSettings()->setValue("users/current_sector", sSectors.size() ? sSectors.first() : "");
        networkSettings()->setValue("users/current_sectors_short_name", sSectors.join("-"));

        _FindOrSetCustomSector();

        if (!sSectors.size())
        {// if no active sector - make all inactive
            if (pView && pView->GetMainFIR())
            {
                CView::TSceneList::iterator itFIR = pView->GetScenes().begin();
                for (; itFIR != pView->GetScenes().end(); ++itFIR)
                {
                    Scene::TSceneList::iterator itSector = (*itFIR)->GetSectorList().begin();
                    for (; itSector != (*itFIR)->GetSectorList().end(); ++itSector)
                        (*itSector)->MarkItemActive(false);
                }
            }
        }
        SetCurSectorAirports(); //stores sector airports in XML
        pView->Draw();
        pView->Redraw();
    }

    setTransferSectors(sectors, sr);
}

void XMasterWindow::onCreateRestriction(const QSharedPointer<CRestrictionUser> &restriction)
{
    if (!connected_to_db_) {
        QMessageBox::critical(this, tr("Error"), tr("No active connection to database"));
        return;
    }

    if (!m_ZonesView)
        return;

    CGUIView *view = dynamic_cast<CGUIView *>(m_ZonesView.data());
    if (!view)
        return;

    const unsigned int zone_id = view->GetMainFIR()->GetID();

    const QUuid uid = restriction->GetGUID();
    TVA_CATCHALL_TRY
    {
        dao()->SaveUserRestriction(zone_id, *restriction.data(), true);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }

    createRestriction(uid, true);

    RedisDBHelper *redis = getRedisDB();
    if (redis)
        redis->sendRestriction(uid, RedisDBHelper::OT_INSERT);

    if (restriction->actual() != Qt::Unchecked) {
        logOperation(tr("Restriction '%1' is actual").arg(restriction->GetName()));
    } else {
        logOperation(tr("Restriction '%1' is not actual").arg(restriction->GetName()));
    }
}

void XMasterWindow::onUpdateRestriction(const QSharedPointer<CRestrictionUser> &restriction)
{
    const QUuid uid = restriction->GetGUID();
    const quint32 zone_id = restriction->GetZoneID();
    TVA_CATCHALL_TRY
    {
        dao()->SaveUserRestriction(zone_id, *restriction.data(), true);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }

    updateRestriction(uid);

    RedisDBHelper *redis = getRedisDB();
    if (redis)
        redis->sendRestriction(uid, RedisDBHelper::OT_UPDATE);

    if (restriction->actual()) {
        logOperation(tr("Restriction '%1' is actual").arg(restriction->GetName()));
    } else {
        logOperation(tr("Restriction '%1' is not actual").arg(restriction->GetName()));
    }
}

void XMasterWindow::onDeleteRestriction(const QSharedPointer<CRestrictionUser> &restriction)
{
    const QUuid uid = restriction->GetGUID();
    deleteRestriction(uid, true);

    TVA_CATCHALL_TRY
    {
        dao()->DeleteUserRestriction(uid);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }

    RedisDBHelper *redis = getRedisDB();
    if (redis)
        redis->sendRestriction(uid, RedisDBHelper::OT_DELETE);
}

void XMasterWindow::onChangedRestrictionRedis(const QUuid &uid, const QString &operation)
{
    if (operation == RedisDBHelper::OT_DELETE) {
        deleteRestriction(uid, true);
    } else if (operation == RedisDBHelper::OT_UPDATE) {
        updateRestriction(uid);
    } else if (operation == RedisDBHelper::OT_INSERT) {
        createRestriction(uid, true);
    }
}

void XMasterWindow::deleteRestrictions()
{
    if (!m_ZonesView)
        return;

    CGUIView *view = dynamic_cast<CGUIView *>(m_ZonesView.data());
    if (!view)
        return;

    const unsigned int zone_id = view->GetMainFIR()->GetID();
    QList<QUuid> uids;

    CView::TSceneList &firs = view->GetScenes();
    foreach (const CView::TScenePtr &fir, firs) {
        if (fir->GetID() == zone_id) {
            foreach (const QSharedPointer<CRestrictionUser> &restriction, fir->GetRestrictionUser()) {
                uids.append(restriction->GetGUID());
            }

            break;
        }
    }

    foreach (const QUuid &uid, uids) {
        deleteRestriction(uid, true);
    }
}

void XMasterWindow::reloadRestrictions()
{
    if (!m_ZonesView)
        return;

    CGUIView *view = dynamic_cast<CGUIView *>(m_ZonesView.data());
    if (!view)
        return;

    deleteRestrictions();

    const unsigned int zone_id = view->GetMainFIR()->GetID();
    QList<QUuid> uids;
    TVA_CATCHALL_TRY
    {
        dao()->loadUserRestrictionsUids(zone_id, uids);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }

    foreach (const QUuid &uid, uids) {
        createRestriction(uid, true);
    }
}

void XMasterWindow::createRestriction(const QUuid &uid, bool send_to_restriction_widget)
{
    if (!m_ZonesView)
        return;

    CGUIView *view = dynamic_cast<CGUIView *>(m_ZonesView.data());
    if (!view)
        return;

    const unsigned int zone_id = view->GetMainFIR()->GetID();

    CView::TSceneList &firs = view->GetScenes();
    foreach (const CView::TScenePtr &fir, firs) {
        if (fir->GetID() == zone_id) {
            TVA_CATCHALL_TRY
            {
                dao()->loadUserRestriction(*fir.data(), uid);
            }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }

            foreach (const QSharedPointer<CRestrictionUser> &restriction, fir->GetRestrictionUser()) {
                if (restriction->GetGUID() == uid) {
                    CGUIRestrictionUser *gui_restriction = dynamic_cast<CGUIRestrictionUser *>(restriction.data());
                    if (gui_restriction) {
                        view->scene()->addItem(gui_restriction);

                        qint64 qllXMin, qllYMax;
                        double scale;
                        CCoord central_coord;
                        view->GetCoordSystem(qllXMin, qllYMax, scale, central_coord);
                        gui_restriction->SetPosition(qllXMin, qllYMax, scale, central_coord);
                    }

                    if (send_to_restriction_widget) {
                        qDebug() << "restriction created " + restriction->GetName();
                        restrictions_widget_->itemAdded(restriction);
                    } else {
                        qDebug() << "restriction updated " + restriction->GetName();
                        restrictions_widget_->itemModified(restriction);
                    }

                    break;
                }
            }

            break;
        }
    }
}

void XMasterWindow::updateRestriction(const QUuid &uid)
{
    deleteRestriction(uid);

    createRestriction(uid, false);
}

void XMasterWindow::deleteRestriction(const QUuid &uid, bool send_to_restriction_widget)
{
    if (!m_ZonesView)
        return;

    CGUIView *view = dynamic_cast<CGUIView *>(m_ZonesView.data());
    if (!view)
        return;

    const unsigned int zone_id = view->GetMainFIR()->GetID();

    CView::TSceneList &firs = view->GetScenes();
    foreach (const CView::TScenePtr &fir, firs) {
        if (fir->GetID() == zone_id) {
            foreach (const QSharedPointer<CRestrictionUser> &restriction, fir->GetRestrictionUser()) {
                if (restriction->GetGUID() == uid) {
                    const quint32 restriction_id = restriction->GetID();
                    const QString name = restriction->GetName();
                    QSharedPointer<CRestrictionUser> restriction_zone = fir->DelUserRestriction(restriction_id);
                    QGraphicsItem *graphics_item = dynamic_cast<QGraphicsItem *>(restriction_zone.data());
                    if (graphics_item)
                        view->scene()->removeItem(graphics_item);

                    if (send_to_restriction_widget) {
                        qDebug() << "restriction deleted " + name;
                        restrictions_widget_->itemDeleted(uid);
                    }

                    return;
                }
            }
        }
    }
}

void XMasterWindow::onRedisConnected(const QString &ip, bool connected)
{
    Q_UNUSED(ip)
    if (connected)
        emit redisConnected();
}

void XMasterWindow::onChangedConnectionRedis(const QString &ip, bool connected)
{
    if(!active_redis_)
        return;
    active_redis_->setText("REDIS:" + ip);
    active_redis_->setStyleSheet(connected ? "QLabel { color : green; }" : "QLabel { color : red; }");

    if (connected)
    {
        if (control_window_ && control_window_->sectorCurrentIndex() != -1)
        {
            OccupySectors();
        }
    }
}

void XMasterWindow::onChangedConnectionStateRedis(bool connected)
{
    if(active_redis_)
        active_redis_->setStyleSheet(connected ? "QLabel { color : green; }" : "QLabel { color : red; }");
}

bool XMasterWindow::RestoreSector()
{
    if (m_pSectoringCombo && control_window_)
    {
        SectorRedis sr;
        if (getRedisDB()->loadSectors2(sr))
        {
            const QString client = networkSettings()->value("oldi/name", "Unknown").toString();
            QStringList redisSectors = sr.getClientSectors(client);
            if (!redisSectors.isEmpty())
            {
                for (int index = 0; index < m_pSectoringCombo->count(); ++index)
                {
                    QStringList sectors = LoadZoneNames(m_pSectoringModel->record(index).value("id").toInt());
                    if (sectors == redisSectors)
                    {
                        m_pSectoringCombo->setCurrentIndex(index);
                        control_window_->setSectorCurrentIndex(index);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool XMasterWindow::SetSector(const QString &name)
{
    if (m_pSectoringCombo && control_window_)
    {
        if (!name.isEmpty())
        {
            int sectorIndex = m_pSectoringCombo->findText(name);
            if (sectorIndex != -1)
            {
                m_pSectoringCombo->setCurrentIndex(sectorIndex);
                control_window_->setSectorCurrentIndex(sectorIndex);
                return true;
            }
        }
    }

    return false;
}

QStringList XMasterWindow::LoadZoneNames(int uiSectoring)
{
    QStringList result;

    TVA_CATCHALL_TRY
    {
        if (uiSectoring > 0)
        {
            QList<uint> ListIdFIR;
            dao()->LoadIdFIREnvironment(uiSectoring, ListIdFIR);

            QList<uint> ListIdSector;
            foreach (uint id, ListIdFIR)
            {
                QList<uint> sectors;
                dao()->LoadIdSectorsEnvironment(uiSectoring, id, sectors);
                ListIdSector += sectors;
            }

            QList<Zone> Zones;
            dao()->LoadZones(Zones);

            for (int i = 0; i < ListIdSector.size(); ++i)
            {
                foreach (const Zone &zone, Zones)
                {
                    if (ListIdSector[i] == zone.GetID())
                    {
                        result.append(zone.GetShortName());
                        break;
                    }
                }
            }
        }
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }

    return result;
}

void XMasterWindow::initAntiOverlapThread()
{   
    AntioverlapStrategy *strategy = new StepLengthAngleStrategy(mFormularLinkMinLength,
                                                                mFormularLinkMaxLength,
                                                                mFormularLinkPreferredLength,
                                                                mFormularLinkStep);
    labels_anti_overlap_thread_.reset(new AntioverlapThread(QSharedPointer<AntioverlapStrategy>(strategy), this));
    connect(labels_anti_overlap_thread_.data(), SIGNAL(repositionRequired(quint16,qreal,int)), SLOT(onLabelRepositionRequired(quint16,qreal,int)));

}

void XMasterWindow::onAutoAntiOverlapClicked(bool checked)
{
    CGUIView *view = dynamic_cast<CGUIView *>(m_ZonesView.data());
    if (view)
    {
        if (checked)
        {
            labels_anti_overlap_thread_->start();
        }
        else
        {
            labels_anti_overlap_thread_->stop();
            m_ZonesView->setFormularsPos(formularLinkAngle(), formularLinkLength());
        }
    }
}

void XMasterWindow::onLabelRepositionRequired(quint16 id, qreal angle, int length)
{
    TAirplanesMap::iterator it = m_ZonesView->GetAirplanes().find(id);
    if (it == m_ZonesView->GetAirplanes().end())
        return;

    if (!(*it)->fixedFormular())
    {
        (*it)->setFormularPos(angle - (*it)->angle(), length);
    }
}

void XMasterWindow::onAngleClicked(int value)
{
    if (!labels_anti_overlap_thread_->isRunning()) {
        m_ZonesView->setFormularsPos(value, formularLinkLength());
    }
}

void XMasterWindow::addFormular(quint16 id)
{
    TAirplanesMap::const_iterator it = m_ZonesView->GetAirplanes().constFind(id);
    if (it != m_ZonesView->GetAirplanes().constEnd())
    {
        FormularInfo formular_info;
        (*it)->formularBoundingRect(formular_info.pos, formular_info.rect);
        formular_info.fixed = (*it)->fixedFormular();

        labels_anti_overlap_thread_->add(id, formular_info);
    }
}

void XMasterWindow::removeFormular(quint16 id)
{
    labels_anti_overlap_thread_->remove(id);
}

void XMasterWindow::setFixedFormular(quint16 id, bool fixed)
{
    labels_anti_overlap_thread_->setFixed(id, fixed);
}

qreal XMasterWindow::formularLinkAngle() const
{
    return control_window_->angle();
}

int XMasterWindow::formularLinkLength() const
{
    return mFormularLinkPreferredLength;
}

int XMasterWindow::formularLinkMinLength() const
{
    return mFormularLinkMinLength;
}

int XMasterWindow::formularLinkMaxLength() const
{
    return mFormularLinkMaxLength;
}

bool XMasterWindow::formularLinkLengthLimited() const
{
    return m_LimitLength->isChecked();
}

void XMasterWindow::onTriggeredHelpAction()
{
    if (!help_window_)
        help_window_.reset(new PdfWindow(this));
    help_window_->openFile(qApp->applicationDirPath() + "/../share/help.pdf");
    help_window_->show();
}

void XMasterWindow::onTriggeredReferenceAction()
{
    if (!reference_window_)
        reference_window_.reset(new PdfWindow(this));
    reference_window_->openFile(qApp->applicationDirPath() + "/../share/reference.pdf");
    reference_window_->show();
}

void XMasterWindow::setTransferSectors(QStringList &sectors, SectorRedis &sr)
{
    //QStringList working_sectors = sr.clients();
    QStringList working_sectors = sr.getMasterSectors();
    QStringList diff;
    sectors.sort();
    set_difference(working_sectors.begin(), working_sectors.end(), sectors.begin(), sectors.end(), std::inserter(diff, diff.begin()));
    const QString transfer_sectors = diff.join("-");
    networkSettings()->setValue("users/transfer_sectors", transfer_sectors);
}

void XMasterWindow::onHidePlainsClicked(bool checked)
{
    networkSettings()->setValue("users/hide_plains", checked ? "true" : "false");

    if (m_ZonesView) {
        if (CGUIView *view = dynamic_cast<CGUIView*> (m_ZonesView.data()))
            view->Redraw();
    }
}

QMenu* XMasterWindow::createHistoryPointMenu()
{
    //return 0;
    QMenu* pMenu = new QMenu(tr("Trek history (points)"), this);
    QActionGroup* pGroup = new QActionGroup(this);
    QList<QAction*> listActions;
    for ( int i = 0; i < 9; ++i )
    {
        int val = 1 * i;
        QAction* pAction = new QAction(QString::number(val), this);
        pAction->setData(val);
        pAction->setCheckable(true);
        connect(pAction, SIGNAL(toggled(bool)), SLOT(_OnHistoryPointChange(bool)));
        pMenu->addAction(pAction);
        pGroup->addAction(pAction);
        listActions.append(pAction);
    }

    QSettings &settings = *globalSettings();
    settings.beginGroup(currentUserName);
    int curVal = settings.value("HistoryTrekPoints", 0).toInt();
    settings.endGroup();

    if ( listActions.size() )
    {
        listActions[0]->setChecked(true);
        foreach ( QAction* act, listActions )
        {
            if ( act->data().toInt() == curVal )
            {
                act->setChecked(true);
                break;
            }
        }
    }
    return pMenu;
}

void XMasterWindow::_OnHistoryPointChange(bool checked)
{
    if ( checked )
    {
        QAction* pAction = qobject_cast<QAction*>(sender());
        if ( pAction )
        {
            int val = pAction->data().toInt();
            QSettings &settings = *globalSettings();
            settings.beginGroup(currentUserName);
            settings.setValue("HistoryTrekPoints", val);
            settings.endGroup();

            networkSettings()->setValue("airplane/history_number_of_points", val);
        }
    }
}

void XMasterWindow::onUpdateMeteoToMaster()
{
    emit UpdateMeteo();
}

void XMasterWindow::onBearingIdChanged()
{
    CGUIView *view = dynamic_cast<CGUIView*>(m_ZonesView.data());
    if (view)
    {
        view->setBearingId(control_window_->getBearingId());
        connect(view, SIGNAL(lastBearing(const QString&)), control_window_, SLOT(onLastBearing(const QString&)));
    }
}

void XMasterWindow::saveApp(int ind)
{
    for(int i = 0; i < m_SectorTypeModel->rowCount(); ++i)
    {
        if(m_SectorTypeModel->record(i).value(0) == ind)
            networkSettings()->setValue("users/sectortype_app", m_SectorTypeModel->record(i).value(1).toBool());
    }
}

void XMasterWindow::setRegistered(bool registered)
{
    mRegistered = registered;

     if (mRegistered)

        {
        m_pLicenseLabel->clear();
        m_pLicenseLabel->setStyleSheet("font: bold 2px; background-color: gray");
        }
    else
        {
        m_pLicenseLabel->setText(tr("       U N L I C E N S E D  S O F T W A R E       "));
        m_pLicenseLabel->setStyleSheet("font: bold 20px; background-color: red; color:yellow"  );
        }
}


