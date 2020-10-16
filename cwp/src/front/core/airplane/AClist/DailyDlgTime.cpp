#include "DailyDlgTime.h"
#include "ui_DailyDlgTime.h"

#include "front/core/airplane/AClist/SFPL.h"
#include "front/core/airplane/AClist/ListAC.h"
#include "back/core/View.h"
#include "dbconnectionsmanager.h"

CDailyDlgTime::CDailyDlgTime(QString acid, QString type, QWidget *parent) :
    QDialog(parent),ui(new Ui::DailyDialogTime)//, m_pMaster(pMaster)

{
    ui->setupUi(this);
    this->setWindowTitle(acid);
    ui->lblType->setText(type);
    QTime time = QTime::currentTime();
    ui->timeEdit->setTime(time);
    connect(ui->timeEdit,SIGNAL(clicked()),this,SLOT(onClickCancel()));
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(onClickCancel()));
    connect(ui->btnOK,SIGNAL(clicked()),this,SLOT(onClickOk()));
}

const QTime CDailyDlgTime::getValue()
{
    return value;
}

void CDailyDlgTime::onClickCancel()
{
   done(QDialog::Rejected);
}

void CDailyDlgTime::onClickOk()
{
    value = ui->timeEdit->time();
    done(QDialog::Accepted);
}
