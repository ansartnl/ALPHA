#include "ORCAMDlg.h"
#include "ui_ORCAMDlg.h"
#include "Master.h"

#include <QSettings>
#include <QTableView>

#include "ORCAMModel.h"

ORCAMDlg::ORCAMDlg(const QStringList &lst, int minutes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ORCAMDlg), mExpireTimeout(10)
{
    ui->setupUi(this);
    //mTimer = startTimer(30 * 1000);
    mTimerOccupied = new QTimer();
    mTimerOccupied->start(60000); // 1 min
    //mModel = new ORCAMModel(minCode, maxCode, this);
    mModel = new ORCAMModel(lst, minutes, this);

    //ui->modelView->horizontalHeader()->setStretchLastSection(true);

    setGeometry(0,0,244,413);
    setMinimumSize(244,413);
    setMaximumSize(244,413);

    ui->modelView->verticalHeader()->hide();
    ui->modelView->setColumnWidth(0,122);
    ui->modelView->setColumnWidth(1,122);

    ui->modelView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->btnClose->setMinimumWidth(55);
    ui->btnClose->setMaximumWidth(55);
    ui->btnFindNext->setMinimumWidth(70);
    ui->btnFindNext->setMaximumWidth(70);

    ui->modelView->setModel(mModel);
    QSettings settings;
    restoreGeometry(settings.value("ORCAMDlgGeometry").toByteArray());
    ui->modelView->horizontalHeader()->restoreState(settings.value("ORCAMDlgViewState").toByteArray());

    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnFindNext, SIGNAL(clicked()), this, SLOT(onFindNext()));
    connect(mTimerOccupied, SIGNAL(timeout()), this, SLOT(_OnTimerOccupied()));

}

ORCAMDlg::~ORCAMDlg()
{
    QSettings settings;
    settings.setValue("ORCAMDlgGeometry", saveGeometry());
    settings.setValue("ORCAMDlgViewState", ui->modelView->horizontalHeader()->saveState());

    delete ui;

    if(mTimer)
        killTimer(mTimer);
}

void ORCAMDlg::setExpireTimeout(int minutes)
{
    mExpireTimeout = minutes;
}

void ORCAMDlg::airplaneData(const CAirplaneState &data)
{
    mModel->airplaneData(data);
}

void ORCAMDlg::reset()
{
    mModel->clear();
}

void ORCAMDlg::timerEvent(QTimerEvent *event)
{
    //mModel->checkTimeout(mExpireTimeout);
}

void ORCAMDlg::onFindNext()
{
    int row = 0;
    if(ui->modelView->selectionModel()->hasSelection())
        row = ui->modelView->selectionModel()->currentIndex().row() + 1;

    row = mModel->findCode(ui->lineFilter->text(), row);

    if(row != -1)
        ui->modelView->selectRow(row);
    else
        ui->modelView->clearSelection();
}

void ORCAMDlg::ChangedOrcamRedis(QString code, QString assigned_time, QString otType)
{
    mModel->ChangedOrcamRedis(code, assigned_time, otType);
}

void ORCAMDlg::_OnTimerOccupied()
{
    mModel->checkTimeOccupied();
}
