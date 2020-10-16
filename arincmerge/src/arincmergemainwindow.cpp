#include "arincmergemainwindow.h"

#include "acdialog.h"
#include "ArincDatabaseImporterDialog.h"
#include "ArincParserDialog.h"
#include "editors.h"
#include "FirBoundaryReplacer.h"
#include "importarincdialog.h"
#include "selectfirdialog.h"
#include "synchronizationdialog.h"

#include "qtlanguagemenu.h"
#include "version.h"
#include "waitcursor.h"

#include <QtCore/QModelIndexList>
#include <QtCore/QSettings>

#include <QFileDialog>
#include <QInputDialog>
#include <QMdiSubWindow>
#include <QMessageBox>

ArincMergeMainWindow::ArincMergeMainWindow(QWidget *parent) :
    QMainWindow(parent)
  , mCurrentFirId(-1)
{
    qApp->setStyleSheet("QSplitter::handle { background-color: gray }");

    setupUi(this);

    menuFile->insertMenu(actionArinc_changes, new QtLanguageMenu(this));
    menuFile->insertSeparator(actionArinc_changes);

    actionArinc_changes->setDisabled(true);

    QSqlDatabase db = QSqlDatabase::database();
    setWindowTitle(windowTitle() + " (" + db.databaseName() + " - " + db.hostName() + ":" + QString::number(db.port()) + " - " + db.userName() + ")");

    foreach(QTabBar *tab, mdiArea->findChildren<QTabBar *>())
    {
        tab->setTabsClosable(true);
        tab->setExpanding(false);
        tab->setElideMode(Qt::ElideRight);
        connect(tab, SIGNAL(tabCloseRequested(int)), this, SLOT(closeMdiTab(int)));
    }

    connect(actionImport_FIR, SIGNAL(triggered()), SLOT(importFir()));
    connect(actionRelease_ARINC, SIGNAL(triggered()), SLOT(releseArinc()));
    connect(actionArinc_changes, SIGNAL(triggered()), SLOT(arincChanges()));
    connect(actionSynchronize, SIGNAL(triggered()), SLOT(onSynchronize()));
    connect(actionChange_FIR, SIGNAL(triggered()), SLOT(onChangeCurrentFir()));

    labelCurrentArincFile = new QLabel(this);
    labelCurrentArincFile->setAlignment(Qt::AlignLeft);
    labelCurrentArincFile->setMinimumSize(labelCurrentArincFile->sizeHint());
    mainStatusBar->addWidget(labelCurrentArincFile);
    statusBar()->setStyleSheet("QStatusBar::item {border-style:none;}");

    loadSettings();

    editRestrictUser->setVisible(false);
}

ArincMergeMainWindow::~ArincMergeMainWindow()
{
    saveSettings();
}

void ArincMergeMainWindow::closeMdiTab(int index)
{
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
    Q_UNUSED(index)
#else
    mdiArea->subWindowList().at(index)->close();
#endif
}

void ArincMergeMainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch(e->type())
    {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void ArincMergeMainWindow::closeEvent(QCloseEvent* pEvent)
{
    qApp->setStyleSheet("");
    QMainWindow::closeEvent(pEvent);
}

void ArincMergeMainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("LastFirId", mCurrentFirId);
    settings.setValue("LastFirName", mCurrentFirName);
}

void ArincMergeMainWindow::loadSettings()
{
    QSettings settings;
    mCurrentFirId = settings.value("LastFirId").toInt();
    mCurrentFirName = settings.value("LastFirName").toString();
}

template<class T> void ArincMergeMainWindow::showEditor()
{
    QList<QMdiSubWindow*> subwindowList = mdiArea->subWindowList();
    foreach (QMdiSubWindow *subwindow, subwindowList)
    {
        if (T::staticMetaObject.className() == subwindow->widget()->metaObject()->className())
        {
            mdiArea->setActiveSubWindow(subwindow);
            return;
        }
    }
    T *widget = new T(this);
    widget->onFirChanged(mCurrentFirId, mCurrentFirName);
    connect(this, SIGNAL(firChanged(int,QString)), widget, SLOT(onFirChanged(int,QString)));
    mdiArea->addSubWindow(widget);
    widget->showMaximized();
}

void ArincMergeMainWindow::on_editFir_triggered()
{
    showEditor<FirEditor> ();
}

void ArincMergeMainWindow::on_editSector_triggered()
{
    showEditor<SectorEditor> ();
}

void ArincMergeMainWindow::on_editAirport_triggered()
{
    showEditor<AirportEditor> ();
}

void ArincMergeMainWindow::on_editHolding_triggered()
{
    showEditor<HoldingEditor> ();
}

void ArincMergeMainWindow::on_editPoint_triggered()
{
    showEditor<PointEditor> ();
}

void ArincMergeMainWindow::on_editRestrict_triggered()
{
    showEditor<RestrictEditor> ();
}

void ArincMergeMainWindow::on_editRestrictUser_triggered()
{
    showEditor<RestrictUserEditor> ();
}

void ArincMergeMainWindow::on_editRoute_triggered()
{
    showEditor<RouteEditor> ();
}

void ArincMergeMainWindow::on_editSsa_triggered()
{
    showEditor<SsaEditor> ();
}

void ArincMergeMainWindow::on_importArinc_triggered()
{
    QSettings settings;
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open ARINC file")
                                                    , settings.value("LastImportFolder").toString()
                                                    , tr("ARINC files (*.txt);;All Files(*)"));

    if (filePath.isEmpty())
        return;

    settings.setValue("LastImportFolder", QFileInfo(filePath).absolutePath());
    filePath = QFileInfo(filePath).absoluteFilePath();

    ArincFilePosIterator it(filePath, QTextCodec::codecForName("CP1251"));
    if (it.isEnd())
        return;

    QScopedPointer<ArincStatusStorage> newArinc(new ArincStatusStorage());
    ArincParserDialog parserDialog;
    parserDialog.restoreGeometry(QByteArray::fromBase64(settings.value("ArincParserDialogGeometry").toByteArray()));
    int result = parserDialog.startParsing(&it, newArinc.data());
    settings.setValue("ArincParserDialogGeometry", parserDialog.saveGeometry().toBase64());
    if (QDialog::Accepted != result)
        return;

    arinc.swap(newArinc);
    actionImport_FIR->setEnabled(true);
    actionRelease_ARINC->setEnabled(true);
    labelCurrentArincFile->setText(filePath);
}

void ArincMergeMainWindow::importFir()
{
    if (!arinc)
        return;

    ImportArincDialog importDialog;
    for (int i = 0; i < arinc->firs.size(); ++i)
        importDialog.addFir(i, arinc->firs.at(i).id, arinc->firs.at(i).name);

    QSettings settings;
    importDialog.restoreGeometry(QByteArray::fromBase64(settings.value("ImportArincDialogGeomerty").toByteArray()));
    int result = importDialog.exec();
    settings.setValue("ImportArincDialogGeomerty", importDialog.saveGeometry().toBase64());
    if (QDialog::Accepted != result)
        return;

    QModelIndexList selectedRows = importDialog.tableWidget->selectionModel()->selectedRows(0);
    if (selectedRows.isEmpty())
        return;

    // Replace boundary points of database FIR if identical one already exists in the database.
    FirBoundaryReplacer firReplacer(selectedRows[0].data().toInt(), arinc.data());
    {
        WaitCursor wc;
        FirBoundaryReplacer::TFirMap firsDB = firReplacer.loadExisted();
        if ( !firsDB.empty() )
        {
            wc.restore();

            QString nameFir;
            int indexFir = selectedRows[0].data().toInt();
            if ( arinc && arinc->firs.size() > indexFir )
                nameFir = arinc->firs[indexFir].name;

            if ( QMessageBox::information(this, tr("Replace boundary points"),
                                          tr("'%1' FIR already exists in the database. Would you like to save boundary points of database FIR in loaded one?")
                                          .arg(nameFir),
                                          QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes )
            {
                int id = -1;
                // In the case when more than one FIR has identical 'id_name' -> let
                // user choose which boundary they want to use for replacement.
                if ( firsDB.size() > 1 )
                {
                    bool ok;
                    QString text = QInputDialog::getItem(this, tr("Choose FIR"),
                                                         tr("FIR name:"), firsDB.keys(),
                                                         0, false, &ok);
                    if (ok && !text.isEmpty())
                        id = firsDB[text];
                }

                wc.wait();
                firReplacer.replace(id);
            }
        }
    }

    ArincDatabaseImporterDialog importer;
    importer.restoreGeometry(QByteArray::fromBase64(settings.value("ArincDatabaseImporterDialogGeometry").toByteArray()));
    result = importer.startImport(arinc.data(), selectedRows[0].data().toInt());
    settings.setValue("ArincDatabaseImporterDialogGeometry", importer.saveGeometry().toBase64());

    if (QDialog::Accepted == result)
    {
        QList<QMdiSubWindow *> subs = mdiArea->subWindowList();
        foreach (QMdiSubWindow *sub, subs)
        {
            RefreshInterface *refreshableWinget = dynamic_cast<RefreshInterface *>(sub->widget());
            if (refreshableWinget)
                refreshableWinget->refresh();
        }
    }
}

void ArincMergeMainWindow::releseArinc()
{
    arinc.reset();
    actionImport_FIR->setEnabled(false);
    actionRelease_ARINC->setEnabled(false);
    labelCurrentArincFile->clear();
}

void ArincMergeMainWindow::arincChanges()
{
    ACDialog dlg(this);
    if (QDialog::Accepted == dlg.exec())
    {
        QList<QMdiSubWindow *> subs = mdiArea->subWindowList();
        foreach (QMdiSubWindow *sub, subs)
        {
            RefreshInterface *refreshableWinget = dynamic_cast<RefreshInterface *>(sub->widget());
            if (refreshableWinget)
                refreshableWinget->refresh();
        }
    }
}

void ArincMergeMainWindow::onSynchronize()
{
    SynchronizationDialog dlg(this);
    dlg.exec();
}

void ArincMergeMainWindow::onChangeCurrentFir()
{
    SelectFirDialog dlg(mCurrentFirId, this);
    if (dlg.exec() == QDialog::Accepted) {
        mCurrentFirId = dlg.selectedFirId();
        mCurrentFirName = dlg.selectedFirName();
        emit firChanged(mCurrentFirId, mCurrentFirName);
    }
}

void ArincMergeMainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, tr("About '%1' application ...").arg(QCoreApplication::applicationName()),
                             tr("%1, Version %2\n\n").arg(QCoreApplication::applicationName()).arg(VERSION_FULL) +
                             tr("Copyright © 2010 %1. All rights reserved.\n\n").arg(QCoreApplication::organizationName()) +
                             tr("This program is protected by copyright law and international treaties. "
                                "Unauthorized reproduction or distribution of this program, or any portion of it, "
                                "may result in severe civil and criminal penalties, and will be prosecuted "
                                "to the maximum extent possible under the law."));
}
