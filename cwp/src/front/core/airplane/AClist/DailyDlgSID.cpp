#include "DailyDlgSID.h"
#include "ui_DailyDlgSID.h"

#include "front/core/airplane/AClist/SFPL.h"
#include "front/core/airplane/AClist/ListAC.h"
#include "back/core/View.h"
#include "dbconnectionsmanager.h"

CDailyDlgSID::CDailyDlgSID(QString acid, QString type, QWidget *parent) :
    QDialog(parent),ui(new Ui::DailyDialogSID)//, m_pMaster(pMaster)

{
    ui->setupUi(this);
    this->setWindowTitle(acid);
    ui->lblType->setText(type);

    QString airpt = networkSettings()->value("meteo/airport").toString();

    if(!airpt.isEmpty()){
        if(DBConnectionsManager::instance().getConnectionStatus() == 1)
        //if (DBConnectionsManager::instance().isConnected())
        {
            query.exec(QString("select * from airport where name = '%1'").arg(airpt));
            query.next();
            int idAirport = query.value(0).toInt();
            query.exec(QString("SELECT id, name FROM runway WHERE airport_id = %1").arg(idAirport));
            query.next();
            int idRunway = query.value(0).toInt();
            ui->lblRW1->setText(query.value(1).toString());
            model_SID = new QSqlQueryModel();
            if(type == "SID")
                model_SID->setQuery("SELECT name FROM ssa WHERE runway_id = " + QString::number(idRunway) + " and type = 1 ORDER BY name");
            if(type == "STAR")
                model_SID->setQuery("SELECT name FROM ssa WHERE runway_id = " + QString::number(idRunway) + " and type = 2 ORDER BY name");

            ui->lv1->setModel(model_SID);

            query.next();
            int idRunway2 = query.value(0).toInt();
            ui->lblRW2->setText(query.value(1).toString());
            model_SID2 = new QSqlQueryModel();
            if(type == "SID")
                model_SID2->setQuery("SELECT name FROM ssa WHERE runway_id = " + QString::number(idRunway2) + " and type = 1 ORDER BY name");
            if(type == "STAR")
                model_SID2->setQuery("SELECT name FROM ssa WHERE runway_id = " + QString::number(idRunway2) + " and type = 2 ORDER BY name");
            ui->lv2->setModel(model_SID2);
        }
    }
    connect(ui->lv1->selectionModel(),
          SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
          this, SLOT(OnChangeSelect(const QItemSelection &, const QItemSelection &)));
    connect(ui->lv2->selectionModel(),
          SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
          this, SLOT(OnChangeSelect2(const QItemSelection &, const QItemSelection &)));
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(onClickCancel()));
    connect(ui->btnOK,SIGNAL(clicked()),this,SLOT(onClickOk()));
}

QString CDailyDlgSID::getValue()
{
    return value;
}

void CDailyDlgSID::OnChangeSelect(const QItemSelection &itsnew, const QItemSelection &its)
{
    QModelIndexList selection = itsnew.indexes();
    if(selection.isEmpty())
        return;
    QModelIndex index= selection.at(0);
    value = index.data(Qt::DisplayRole).toString();
    ui->lv2->clearSelection();
}

void CDailyDlgSID::OnChangeSelect2(const QItemSelection &itsnew, const QItemSelection &its)
{
    QModelIndexList selection = itsnew.indexes();
    if(selection.isEmpty())
        return;
    QModelIndex index= selection.at(0);
    value = index.data(Qt::DisplayRole).toString();
    ui->lv1->clearSelection();
}

void CDailyDlgSID::onClickCancel()
{
   done(QDialog::Rejected);
}

void CDailyDlgSID::onClickOk()
{
    done(QDialog::Accepted);
}
