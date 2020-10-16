#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qtlanguagemenu.h"
#include "qttranslator.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mLang("english")
{
    ui->setupUi(this);

    connect(ui->btnRun, SIGNAL(clicked()), this, SLOT(onBtnRun()));
    connect(ui->btnCopy, SIGNAL(clicked()), this, SLOT(onBtnCopy()));
    connect(ui->btnRename, SIGNAL(clicked()), this, SLOT(onBtnRename()));
    connect(ui->btnDelete, SIGNAL(clicked()), this, SLOT(onBtnDelete()));
    connect(ui->btnRefresh, SIGNAL(clicked()), this, SLOT(onBtnRefresh()));
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(onBtnClose()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(onBtnClose()));

    connect(&mModel, SIGNAL(fileDirectory(bool)), this, SLOT(onFileDirectory(bool)));
    connect(&mModel, SIGNAL(flashDirectory(bool)), this, SLOT(onFlashDirectory(bool)));
    connect(&mModel, SIGNAL(scriptPath(bool)), this, SLOT(onScriptPath(bool)));

    CFileListFilterModel *filterModel = new CFileListFilterModel(this);
    filterModel->setSourceModel((QAbstractItemModel*)&mModel);
    ui->fileView->setModel(filterModel);

    resize(500, 400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
    Translate();

    xmlSettings().beginGroup(tr("paths"));

    mFileDir = xmlSettings().value(tr("dest_dir"), tr("/")).toString();
    mFlashDir = xmlSettings().value(tr("copy_dir"), tr("/backup")).toString();
    mScript = xmlSettings().value(tr("script"), tr("/run.bat")).toString();
    mMask = xmlSettings().value(tr("mask"), tr("*")).toString();

    xmlSettings().endGroup();

    mModel.setParams(mFileDir, mFlashDir, mMask, mScript);
    mModel.refreshModel();
}

void MainWindow::Translate()
{
    mLang = xmlSettings().value("language", "english").toString().toLower();

    xmlSettings().beginGroup(tr("names"));
    xmlSettings().beginGroup(mLang);

    setWindowTitle(xmlSettings().value(tr("window"), tr("Recovery tool")).toString());
    ui->labelTableName->setText(xmlSettings().value(tr("table"), tr("File list:")).toString());
    ui->btnRun->setText(xmlSettings().value(tr("run"), tr("Run")).toString());
    ui->btnCopy->setText(xmlSettings().value(tr("copy"), tr("Copy")).toString());
    ui->btnRename->setText(xmlSettings().value(tr("rename"), tr("Rename")).toString());
    ui->btnDelete->setText(xmlSettings().value(tr("delete"), tr("Delete")).toString());
    ui->btnRefresh->setText(xmlSettings().value(tr("refresh"), tr("Refresh")).toString());
    ui->btnClose->setText(xmlSettings().value(tr("close"), tr("Close")).toString());

    xmlSettings().endGroup();
    xmlSettings().endGroup();
}

void MainWindow::onFileDirectory(bool exists)
{
    ui->btnRun->setEnabled(exists);
    ui->btnRename->setEnabled(exists);
    ui->btnDelete->setEnabled(exists);
    ui->btnCopy->setEnabled(exists);
}

void MainWindow::onFlashDirectory(bool exists)
{
    ui->btnCopy->setEnabled(exists && ui->btnRename->isEnabled());
}

void MainWindow::onScriptPath(bool exists)
{
    ui->btnRun->setEnabled(exists && ui->btnRename->isEnabled());
}

void MainWindow::onBtnRun()
{
    mModel.run(ui->fileView->currentIndex().row());
}

void MainWindow::onBtnCopy()
{
    mModel.copy(ui->fileView->currentIndex().row());
}

void MainWindow::onBtnRename()
{
    mModel.rename(ui->fileView->currentIndex().row());
}

void MainWindow::onBtnDelete()
{
    mModel.del(ui->fileView->currentIndex().row());
}

void MainWindow::onBtnRefresh()
{
    mModel.refreshModel();
}

void MainWindow::onBtnClose()
{
    close();
}
