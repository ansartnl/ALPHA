#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "private/mainwindow_p.h"

// translations
#include "qtlanguagemenu.h"

#include "manualeditor.h"
#include "aftnservicelog.h"
#include "logindialog.h"

// telegrams
#include "CreateTelegramWizard.h"
#include "InternalClient.h"
#include "addressbook.h"

// actions managers
#include "editactionsmanager.h"
#include "printactionsmanager.h"
#include "navigateactionsmanager.h"
#include "customactionsmanager.h"

// 3rd party
#include "qtwindowmenu.h"
#include "qttoolbardialog.h"
#include "qtmenuview.h"

// application GUI objects
#include "objectsmanager.h"

// global functions, variables
#include "main.h"
#include "pulsar_fdp_settings.h"
#include "DBSettings.h"

#include <QtDebug>

#include <QtCore/QTimer>
#include <QtCore/QSettings>
#include <QtCore/QProcess>
#include <QtCore/QDir>
#include <QtCore/QSignalMapper>
#include <QtCore/QPropertyAnimation>

#include <QtGui/QMdiSubWindow>
#include <QtGui/QLineEdit>
#include <QtGui/QAction>
#include <QtGui/QDesktopWidget>
#include <QtGui/QCloseEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QToolButton>
#include <QtGui/QSessionManager>
#include <QtGui/QFontDialog>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

void MainWindow::init()
{
    Q_D(MainWindow);
    qApp->setStyleSheet("QSplitter::handle { background-color: gray }");
    ui->setupUi(this);

    { // app settings
        globalSettings()->beginGroup(metaObject()->className());
        QVariant var = globalSettings()->value("AppFont");
        if (!var.isNull() && var.isValid())
        {
            QFont font;
            QDataStream ds(var.toByteArray());
            ds >> font;
            qApp->setFont(font);
        }
        globalSettings()->endGroup();
    }

    ui->actionCreate_telegram->setIcon(QIcon("images:32x32/actions/CreateTelegram.png"));

#warning (Fullscreen mode)
    {
        globalSettings()->beginGroup(metaObject()->className());
        QVariant var = globalSettings()->value("BlockAltTab", 1);
        globalSettings()->setValue("BlockAltTab", var);
        if(var.toInt())
            setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        var = globalSettings()->value("FullScreenMode", 1);
        globalSettings()->setValue("FullScreenMode", var);
        if(var.toInt())
        {
            setWindowState(windowState() | Qt::WindowFullScreen);
            d->m_bFullScreen = true;
        }
        globalSettings()->endGroup();
    }

    d->firstShow = true;
    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this,
            SLOT(onSubWindowActivated(QMdiSubWindow*)));

    foreach(QTabBar * tab, ui->mdiArea->findChildren<QTabBar*>())
    {
        tab->setTabsClosable(true);
        tab->setExpanding(false);
        tab->setElideMode(Qt::ElideRight);
        connect(tab, SIGNAL(tabCloseRequested(int)), this, SLOT(closeMdiTab(int)));
    }

    // ----------------------------------------------
    //! QtToolbarManager
    // ----------------------------------------------
    d->toolBarManager = new QtToolBarManager(this);
    d->toolBarManager->setObjectName("toolBarManager");
    d->toolBarManager->setMainWindow(this);

#ifdef MICROKOD_BUILD
    ui->mainToolBar->addAction(ui->actionCreate_telegram);
#endif

#ifndef MICROKOD_BUILD
    foreach(QAction* a, ObjectsManager::instance()->actions())
        d->toolBarManager->addAction(a, tr("Database objects"));
    ui->configureToolBarsAction->setIcon(QIcon("images:32x32/brick/bricks.png"));
    connect(ui->configureToolBarsAction, SIGNAL(triggered()), this,
            SLOT(configureToolBars()));
#endif

    ui->actionUpdate->setIcon(QIcon("images:32x32/table/table_refresh.png"));
    d->toolBarManager->addAction(ui->configureToolBarsAction, tr("General"));

    // -------------------------------
    //! Edit actions undo-redo-cut-copy-paste framework
#ifndef MICROKOD_BUILD
    d->editActionsManager = new EditActionsManager(this);
    ui->editMenuSeparator1->setSeparator(true);
    ui->editToolBarSeparator1->setSeparator(true);
    d->editActionsManager->addWidget(ui->editMenu, ui->editMenuSeparator1);
    d->editActionsManager->addWidget(ui->editToolBar, ui->editToolBarSeparator1);
#endif
    // -------------------------------

    // -------------------------------
    //! Print actions
#ifndef MICROKOD_BUILD
    d->printActionsManager = new PrintActionsManager(this);
    ui->printActionsSeparator->setSeparator(true);
    d->printActionsManager->addWidget(ui->fileMenu, ui->printActionsSeparator);
    d->printActionsManager->addWidget(ui->printToolBar);
#else
    QAction *printAction = new QAction(tr("Print"), this);
    connect(printAction, SIGNAL(triggered()), SLOT(onPrintAction()));
    ui->printToolBar->addAction(printAction);
#endif
    // -------------------------------

    //ui->actionCreate_telegram->setEnabled(false);

    // --------------------------------
    //! Window menu
    d->windowMenu = new QtWindowMenu(this);
    d->windowMenu->setObjectName("windowMenu");
    d->windowMenu->setMdi(ui->mdiArea);
    ui->menuBar->addMenu(d->windowMenu);
    // --------------------------------

    // --------------------------------------------
    //! Navigate actions
#ifndef MICROKOD_BUILD
    d->navigateActionsManager = new NavigateActionsManager(this);

    d->navigateActionsManager->addWidget(ui->navigateMenu);
    d->navigateActionsManager->addWidget(ui->navigateToolBar);
#endif
    // ----------------------------------------------

    // --------------------------------------------
    //! Custom actions for focus widget
#ifndef MICROKOD_BUILD
    d->customActionsManager = new CustomActionsManager(this);

    d->customActionsManager->addWidget(ui->tableMenu);
#endif
    // ----------------------------------------------

    // ----------------------------------------------
    //! Manual messages button
    QAction* action = ObjectsManager::instance()->action(
            ManualEditor::staticMetaObject.className());
    d->manualMessageButton = new RedToolButton();
    d->manualMessageButton->setDefaultAction(action);
    ui->statusBar->addPermanentWidget(d->manualMessageButton);
    // ----------------------------------------------

    // ----------------------------------------------
    //! AFTN Service network log connection
#ifndef MICROKOD_BUILD
    action = ObjectsManager::instance()->action(
            AFTNServiceLog::staticMetaObject.className());
    d->aftnServiceButton = new RedToolButton();
    d->aftnServiceButton->setVisible(false);
    d->aftnServiceButton->setDefaultAction(action);
    ui->statusBar->addPermanentWidget(d->aftnServiceButton);
#endif

    // ----------------------------------------------

    //! user and time in status bar
    if(d->m_bFullScreen)
    {
        d->m_pUserStatusLabel = new QLabel("99.99.99 99:99:99", this);
        d->m_pUserStatusLabel->setAlignment(Qt::AlignRight);
        //d->m_pUserStatusLabel->setMinimumSize(d->m_pUserStatusLabel->sizeHint());
        ui->statusBar->addPermanentWidget(d->m_pUserStatusLabel);
        d->m_pUserStatusLabel->setText("");
    }

    //! Setu database front-end objects
    ui->menuDatabaseObjects->setModel(ObjectsManager::instance()->model(), true);

    ui->databaseObjectsView->setModel(ObjectsManager::instance()->model());

    connect(ui->menuDatabaseObjects, SIGNAL(triggeredIndex(QModelIndex)), this,
            SLOT(databaseObjectTriggered(QModelIndex)));
    connect(ui->databaseObjectsView, SIGNAL(doubleClicked(QModelIndex)), this,
            SLOT(databaseObjectTriggered(QModelIndex)));
    connect(ObjectsManager::instance(), SIGNAL(objectTriggered(QString)), this,
            SLOT(databaseObjectTriggered(QString)));

    // ---------------------------------------
    //! Language initialisation
    ui->menuBar->insertMenu(ui->menuDatabaseObjects->menuAction(), new QtLanguageMenu(this));

    restoreSettings();

#ifndef MICROKOD_BUILD
    ui->mainToolBar->hide();
    ui->mainToolBar->toggleViewAction()->setVisible(false);
    ui->editToolBar->show();
#else
    ui->mainToolBar->show();
    ui->editToolBar->hide();
    ui->editToolBar->toggleViewAction()->setVisible(false);
#endif

    // ---------------------------------------
    //! Initialisation of stdin reader
//    stdinReader = new StdinReader(this);
//    connect(stdinReader, SIGNAL(received(QString)), SLOT(onStdin(QString)));
//    stdinReader->start();

    connect(qApp, SIGNAL(commitDataRequest(QSessionManager&)), SLOT(OnShutdown(QSessionManager&)), Qt::DirectConnection);

    connect(ui->fileMenu, SIGNAL(aboutToShow()), SLOT(onSetCheck_actionAuto_answer_on_error()));
}

void MainWindow::OnShutdown(QSessionManager& manager)
{
    Q_D(MainWindow);
    d->m_bShutdown = true;
}

void MainWindow::onPrintAction()
{
}

MainWindow::MainWindow(MainWindowPrivate& dd, QWidget* parent) :
        QMainWindow(parent), d_ptr(&dd)
{
    init();
}

MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent), d_ptr(new MainWindowPrivate), ui(new Ui::MainWindow)
{
    init();
}

MainWindow::~MainWindow()
{
}

void MainWindow::timerEvent(QTimerEvent* event)
{
    Q_D(MainWindow);

    if (event->timerId() == d->redrawTitleTimerId)
    {
        setWindowTitle(
                tr("AFTN Planning - %1 - %2") .arg(currentUserFullName,
                                                   QDateTime::currentDateTime().toUTC().toString(
                                                           QLocale().dateTimeFormat())));
        if(d->m_bFullScreen && d->m_pUserStatusLabel)
        {
            QString sText = QString("%1 (%2)").arg(QDateTime::currentDateTime().toUTC().toString("dd.MM.yyyy hh:mm:ss"))
                    .arg(currentUserFullName.isEmpty() ? currentUser : currentUserFullName);
            d->m_pUserStatusLabel->setText(sText);
        }
    }
    QWidget::timerEvent(event);
}

void MainWindow::closeMdiTab(int index)
{
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
    Q_UNUSED(index)
#else
    ui->mdiArea->subWindowList().at(index)->close();
#endif
}

void MainWindow::onSubWindowActivated(QMdiSubWindow* activeMdi)
{
    disconnect(ui->actionUpdate, SIGNAL(triggered()), 0, 0);

    if (activeMdi) {
        ui->statusBar->showMessage(
                activeMdi->widget()->property("browserStatus").toString());

        if (activeMdi->widget()->metaObject()->indexOfSlot("updateModel()") != -1) {
            ui->actionUpdate->setEnabled(true);
            activeMdi->widget()->metaObject()->invokeMethod(activeMdi->widget(),
                                                            "updateModel");
            connect(ui->actionUpdate, SIGNAL(triggered()), activeMdi->widget(),
                    SLOT(updateModel()));
        } else {
            ui->actionUpdate->setEnabled(false);
        }
    }
}

void MainWindow::browserStatusChanged(const QString& newStatus)
{
    QWidget* browser = qobject_cast<QWidget*> (sender());

    if (browser && browser->isActiveWindow())
        ui->statusBar->showMessage(browser->property("browserStatus").toString());
}

void MainWindow::on_actionLogin_triggered()
{
    // -----------------------------
    /// Authorization
    // -----------------------------
    LoginDialog* loginDialog = getLoginDialog(this);

    saveSettings();
    loginDialog->exec();

    ObjectsManager::instance()->refresh();
    restoreSettings();
    //loginDialog->deleteLater();
}

void MainWindow::on_actionCreate_telegram_triggered()
{
//    CreateTelegramWizard dlg;
//    dlg.exec();
    CreateTelegramWizard* dlg = new CreateTelegramWizard(this);
    InternalClient* internalClient = new InternalClient(dlg);
    dlg->setInternalClient(internalClient);

//    ReplyDictionary dict;
//    dict.version = ParseVersion::n_4444;
//    dict.taircraft = standardDictionary("taircraft", version);
//    dict.frules = standardDictionary("frules", version);
//    dict.tflight = standardDictionary("tflight", version);
//    dict.wtc = standardDictionary("wtc", version);
//    dict.equipment = standardDictionary("equipment", version);
//    dlg->setDictionary(dict);

    dlg->show();
}

void MainWindow::on_actionAddress_book_triggered()
{
    AddressBook dlg(this);
    dlg.exec();
}

void MainWindow::configureToolBars()
{
    Q_D(MainWindow);
    QtToolBarDialog dlg(this);
    dlg.setToolBarManager(d->toolBarManager);
    dlg.exec();
}

bool MainWindow::maybeSave()
{
    Q_D(MainWindow);
    if(!d->m_bShutdown)
    {
        QMessageBox::StandardButton ok = QMessageBox::question(this, tr(
                "AFTN Planning Server"), tr("Exit program?"), QMessageBox::Ok
                                                               | QMessageBox::Cancel);
        if (ok != QMessageBox::Ok)
            return false;
    }

    saveSettings();
    return true;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (maybeSave()) {
        event->accept();
        qApp->setStyleSheet("");
    } else {
        event->ignore();
    }
}

void MainWindow::showEvent(QShowEvent* event)
{
    Q_D(MainWindow);

    if (d->firstShow) {
        d->firstShow = false;
        d->redrawTitleTimerId = startTimer(1000);
    }
}

void MainWindow::databaseObjectTriggered(const QModelIndex& index)
{
    QString id = ObjectsManager::instance()->objectIdFromIndex(index);
    QWidget* widget = ObjectsManager::instance()->widget(index, this);

    if (!widget)
        return;

    QDialog* dialog = qobject_cast<QDialog*> (widget);

    if (dialog) {
        dialog->exec();
        delete dialog;
    } else {
        QList<QMdiSubWindow*> mdiChildren = ui->mdiArea->subWindowList();

        for (int i = 0; i < mdiChildren.count(); ++i) {
            if (mdiChildren.at(i)->widget()->objectName() == id) {
                ui->mdiArea->setActiveSubWindow(mdiChildren[i]);
                return;
            }
        }

        widget->setObjectName(id);

        if (widget->metaObject()->indexOfSignal("statusChanged(QString)") != -1)
            connect(widget, SIGNAL(statusChanged(QString)), this,
                    SLOT(browserStatusChanged(QString)));

        ui->mdiArea->addSubWindow(widget);
        widget->showMaximized();
    }
}

void MainWindow::databaseObjectTriggered(const QString& id)
{
    databaseObjectTriggered(ObjectsManager::instance()->indexFromId(id));
}

void MainWindow::changeEvent(QEvent* event)
{
    Q_D(MainWindow);
    //LanguageChange events should not be confused with LocaleChange events.
    if (event->type() == QEvent::LocaleChange) {

        // HERE we can un|install qtranslator
        //    Locale-Change events are generated by the system and tell the application,
        //    "Maybe you should load a new translation."
    } else if (event->type() == QEvent::LanguageChange) {
        //    LanguageChange events are generated by Qt and tell the application's widgets,
        //    "Maybe you should retranslate all your strings."
        ui->retranslateUi(this);
        d->windowMenu->retranslateUi();
#ifndef MICROKOD_BUILD
        d->editActionsManager->retranslateUi();
        d->navigateActionsManager->retranslateUi();
        d->customActionsManager->retranslateUi();
#endif
        d->printActionsManager->retranslateUi();

        ObjectsManager::instance()->retranslateUi();
    }

    QMainWindow::changeEvent(event);
}

QStringList MainWindow::saveObjectsState() const
{
    Q_D(const MainWindow);
    d->manualMessageButton->hide();
#ifndef MICROKOD_BUILD
    d->aftnServiceButton->hide();
#endif

    QStringList result;

    QList<QMdiSubWindow*> mdiChildren = ui->mdiArea->subWindowList();

    for (int i = 0; i < mdiChildren.count(); ++i)
        result << mdiChildren.at(i)->widget()->objectName();

    return result;
}

void MainWindow::saveSettings()
{
    Q_D(MainWindow);
    globalSettings()->beginGroup(metaObject()->className());
    globalSettings()->setValue("state", saveState().toHex());
    globalSettings()->setValue("geometry", saveGeometry().toHex());
    globalSettings()->setValue("toolbarmanager",
                               d->toolBarManager->saveState().toHex());
    globalSettings()->setValue("objects", saveObjectsState());
    globalSettings()->endGroup();

    // send close event that it save it's settings
    ui->mdiArea->closeAllSubWindows();
}

void MainWindow::restoreObjectsState(const QStringList& objects)
{
    Q_D(MainWindow);
    if (!ObjectsManager::instance()->allow(
            ManualEditor::staticMetaObject.className()) ||
        !ObjectsManager::instance()->contains(
            ManualEditor::staticMetaObject.className())) {
        d->manualMessageButton->hide();
    } else {
        d->manualMessageButton->show();
    }
#ifndef MICROKOD_BUILD
    if (!ObjectsManager::instance()->allow(
            AFTNServiceLog::staticMetaObject.className())) {
        d->aftnServiceButton->hide();
    } else {
        d->aftnServiceButton->show();
    }
#endif
    foreach (const QString& object, objects)
        databaseObjectTriggered(ObjectsManager::instance()->indexFromId(object));
}

void MainWindow::restoreSettings()
{
    Q_D(MainWindow);
    // ---------------------
    //! Load settings
    globalSettings()->beginGroup(metaObject()->className());
    restoreState(QByteArray::fromHex(globalSettings()->value("state").toByteArray()));

    if (globalSettings()->value("FullScreenMode", 1).toInt())
    {
        if (globalSettings()->value("FSA", false).toBool())
        {
            restoreGeometry(QByteArray::fromHex(globalSettings()->value("geometry").toByteArray()));
        } else
        {
            globalSettings()->setValue("FSA", true);
            globalSettings()->setValue("WDA", false);
        }
    } else
    {
        if (globalSettings()->value("WDA", false).toBool())
        {
            restoreGeometry(QByteArray::fromHex(globalSettings()->value("geometry").toByteArray()));
        } else
        {
            globalSettings()->setValue("WDA", true);
            globalSettings()->setValue("FSA", false);
        }
    }
    d->toolBarManager->restoreState(QByteArray::fromHex(globalSettings()->value(
            "toolbarmanager").toByteArray()));
    QStringList objects = globalSettings()->value("objects").toStringList();
    globalSettings()->endGroup();

    // First QSettings::endGroup, than load objects, because objects use QSettings::beginGroup in their initializations
    restoreObjectsState(objects);
}

void MainWindow::onStdin(const QString &str)
{
    if (str == "stop")
        qApp->quit();
}

void MainWindow::statusAFTNChanged(bool bConnected)
{
    //ui->actionCreate_telegram->setEnabled(bConnected);
}

void MainWindow::on_actionFont_triggered()
{
    bool ok(false);
    QFont font = QFontDialog::getFont(&ok, qApp->font(), this);
    if(ok)
    {
        QByteArray arr;
        QDataStream ds(&arr, QIODevice::WriteOnly);
        ds << font;
        globalSettings()->beginGroup(metaObject()->className());
        globalSettings()->setValue("AppFont", arr);
        globalSettings()->endGroup();

        qApp->setFont(font);
        foreach (QWidget *widget, QApplication::allWidgets())
            widget->setFont(font);
    }
}

void MainWindow::on_actionAuto_answer_on_error_triggered()
{
    bool bCheck = ui->actionAuto_answer_on_error->isChecked();
    DBSettings dbsettings(QSqlDatabase::database(), this);
    dbsettings.setValue(PulsarSettings::autoAnswerOnError, bCheck ? 1 : 0);
}

void MainWindow::onSetCheck_actionAuto_answer_on_error()
{
    DBSettings dbsettings(QSqlDatabase::database(), this);
    QVariant val = dbsettings.value(PulsarSettings::autoAnswerOnError);
    if(!val.isNull())
    {
        ui->actionAuto_answer_on_error->setEnabled(true);
        ui->actionAuto_answer_on_error->setChecked(val.toInt() == 1);
    }
    else
    {
        ui->actionAuto_answer_on_error->setEnabled(false);
        ui->actionAuto_answer_on_error->setChecked(false);
    }
}

MainWindowPrivate::MainWindowPrivate()
    : m_pUserStatusLabel(0), m_bShutdown(false), m_bFullScreen(false)
{
}
MainWindowPrivate::~MainWindowPrivate()
{
}

// ---------------------------------------------------
#include <QtGui/QStylePainter>
RedToolButton::RedToolButton()
{
}

void RedToolButton::paintEvent(QPaintEvent* event)
{
    QStylePainter painter(this);
    QStyleOptionToolButton options;
    initStyleOption(&options);

    if (isEnabled()) {
        QColor color(Qt::black);
        if (defaultAction()) {
            color = QColor(defaultAction()->property("color").toInt());
        }
        // draw red text
        QString text = options.text;
        options.text.clear();
        painter.drawComplexControl(QStyle::CC_ToolButton, options);
        painter.setPen(QPen(color));
        painter.drawText(options.rect, text, QTextOption(Qt::AlignCenter));
        return;
    }

    painter.drawComplexControl(QStyle::CC_ToolButton, options);
}
