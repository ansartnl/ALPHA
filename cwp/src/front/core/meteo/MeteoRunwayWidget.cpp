#include "MeteoRunwayWidget.h"
#include "ui_MeteoRunwayWidget.h"

#include "main.h"
#include "Dao.h"
#include "Master.h"
#include "MeteoRunwayModel.h"
#include "MeteoRunwayDelegate.h"
#include "import/Airport.h"
#include "MeteoFilterDlg.h"
#include "MeteoRunwayDialog.h"

#include "OLDIMessages.h"

#include <QtCore/QSet>
#include <QtCore/QSettings>
#include <QtCore/QTimer>

#include <QMessageBox>
#include <QBoxLayout>

#include "obelix/CmdMeteoUpdate.h"

extern QString currentUserName;
//static const QString gMeteoHeaderStateName = "MeteoHeaderState";


MeteoRunwayWidget::MeteoRunwayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeteoRunwayWidget)
{
    ui->setupUi(this);
    setupUi();
    init();
    UpdateXFont();
}

MeteoRunwayWidget::~MeteoRunwayWidget()
{
//    QSettings settings;
//    settings.beginGroup(currentUserName);
//    settings.setValue(objectName(), saveGeometry());
//    settings.setValue(gMeteoHeaderStateName, ui->tableView->horizontalHeader()->saveState());

    delete ui;
}

void MeteoRunwayWidget::Translate()
{
    ui->retranslateUi(this);
    SizeToContents();
}

void MeteoRunwayWidget::UpdateXFont()
{
    if(ui->tableView)
    {
        QFont font1 = ui->tableView->font();
        font1.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
        ui->tableView->setFont(font1);
        SizeToContents();
    }
}

void MeteoRunwayWidget::setupUi()
{
    ui->toolButtonApply->hide();
    ui->toolButtonReset->hide();

    MeteoRunwayModel *model = new MeteoRunwayModel(QString(), MeteoRunwayModel::TMeteoList(), this);
    connect(model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            SLOT(setModeldataChanged(const QModelIndex &, const QModelIndex &)),
            Qt::QueuedConnection);
    model->setEditEnabled(false);
    ui->tableView->setModel( model );

    // Set item delegate to edit table content.
    if ( networkSettings()->contains("meteo/editable") )
    {
        if ( networkSettings()->value("meteo/editable").toBool() )
        {
            model->setEditEnabled(true);

            MeteoRunwayDelegate *delegate = new MeteoRunwayDelegate(this);
            ui->tableView->setItemDelegate( delegate );
        }
    }

    // Скрытие верхнего и нижнего виджетов для рабочего места диспетчера РЦ.
    if ( networkSettings()->contains("meteo/table_only") )
    {
        if ( networkSettings()->value("meteo/table_only").toBool() )
        {
            ui->widgetTop->hide();
            ui->widgetBottom->hide();
            ui->tableView->horizontalHeader()->hide();
        }
    }
    ui->tableView->horizontalHeader()->hide();
}

void MeteoRunwayWidget::init()
{
    MeteoRunwayModel *model = dynamic_cast<MeteoRunwayModel*>( ui->tableView->model() );
    if ( model )
    {
        ui->toolButtonApply->hide();
        ui->toolButtonReset->hide();

        // Config file should have airport tag defined.
        if ( !networkSettings()->contains("meteo/airport") )
        {
            LOG_STR(Log4Qt::Level::WARN_INT, tr("Settings for 'meteo/airport' were not defined in the config file. ")
                    + tr("Loading of meteo information is skipped"));
            //return;
        }
        QString sAirportName = GetAirportName();

        QDateTime lastEditTime;
        QString currentRunway;
        MeteoRunwayModel::TMeteoList meteo;

        TVA_CATCHALL_TRY
        {
            dao()->LoadMeteoAirportSettings(sAirportName, lastEditTime, currentRunway);
            MeteoFilterDlg dlgFilter(this);
            if(dlgFilter.isFilter())
            {
                MeteoRunwayModel::TMeteoList tmpMeteo;
                dao()->LoadMeteoRunway(sAirportName, tmpMeteo);
                foreach(const MeteoRunway &m, tmpMeteo)
                {
                    if(dlgFilter.isRunwayEnable(sAirportName, m.GetRunwayName()))
                        meteo.append(m);
                }
            }
            else
            {
                dao()->LoadMeteoRunway(sAirportName, meteo);
            }

            mMeteoOld = meteo;
            mCurrentRunwayOld = currentRunway;

            model->setMeteo( meteo );
            model->setCurrentRunway( currentRunway );

            ui->tableView->resizeColumnsToContents();
            ui->tableView->resizeRowsToContents();

            // Define runway names for combo box.
            QSet<QString> runwayNames;
            foreach ( const MeteoRunway &m, meteo )
                runwayNames << m.GetRunwayName();

            ui->comboBoxRunways->blockSignals(true);
            ui->comboBoxRunways->clear();
            QStringList names = runwayNames.toList();
            qSort(names);
            ui->comboBoxRunways->addItems( names );
            if ( currentRunway.isEmpty() )
            {
                if ( ui->comboBoxRunways->count() > 0 )
                {
                    ui->comboBoxRunways->setCurrentIndex( 0 );
                    mCurrentRunwayOld = ui->comboBoxRunways->currentText();
                }
            }
            else
                ui->comboBoxRunways->setCurrentIndex( ui->comboBoxRunways->findText(currentRunway) );
            ui->comboBoxRunways->blockSignals(false);

            // Last edit time setting.
            ui->labelEditTime->setText( lastEditTime.toString("HH:mm:ss") );
        }
        TVA_CATCHALL(err)
        {
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
        }
        QTimer::singleShot(0, this, SLOT(OnTimerSize()));
    }
}

void MeteoRunwayWidget::setModeldataChanged(const QModelIndex & topLeft,
                                            const QModelIndex & bottomRight)
{
    MeteoRunwayModel *model = dynamic_cast<MeteoRunwayModel*>( ui->tableView->model() );
    if ( model )
    {
        // Reported that model has been changed -> need to show button for
        // saving changes to the database.
        if ( isDataChanged() )
        {
            ui->toolButtonApply->show();
            ui->toolButtonReset->show();
        }
        else
        {
            ui->toolButtonApply->hide();
            ui->toolButtonReset->hide();
        }

        SizeToContents(true);
    }
}

void MeteoRunwayWidget::on_comboBoxRunways_currentIndexChanged(int index)
{
    QString currentRunway = ui->comboBoxRunways->currentText();
    if ( !currentRunway.isEmpty() )
    {
        MeteoRunwayModel *model = dynamic_cast<MeteoRunwayModel*>( ui->tableView->model() );
        if ( model )
        {
            model->setCurrentRunway( currentRunway );

            // Current runway is changed -> info need to be saved to DB.
            if ( mCurrentRunwayOld != currentRunway )
            {
                ui->toolButtonApply->show();
                ui->toolButtonReset->show();
            }
        }
    }
}

void MeteoRunwayWidget::on_toolButtonApply_clicked()
{
    MeteoRunwayModel *model = dynamic_cast<MeteoRunwayModel*>( ui->tableView->model() );
    if ( model )
    {
        QString currentRunway = ui->comboBoxRunways->currentText();

        TVA_CATCHALL_TRY
        {
            dao()->SaveMeteoRunway(model->meteoList());
            QString sAirportName = GetAirportName();
            if ( !sAirportName.isEmpty() && !currentRunway.isEmpty())
                dao()->SaveMeteoAirportSettings(sAirportName, QDateTime::currentDateTime(), currentRunway);

            MeteoRunwayModel::TMeteoList meteoCur = model->meteoList();
            if ( meteoCur.size() == mMeteoOld.size() )
            {
                for ( int i = 0; i < meteoCur.size(); i++ )
                {
                    const MeteoRunway &meteo = meteoCur.at(i);
                    const MeteoRunway &meteoOld = mMeteoOld.at(i);

                    if ( (meteo != meteoOld) && master())
                    {
                        QByteArray arr;
                        QDataStream strm(&arr, QIODevice::WriteOnly);
                        strm << meteo;

                        QSharedPointer<obelix::CCmdMeteoUpdate> pCmd(new obelix::CCmdMeteoUpdate(arr.toHex()));
                        master()->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());

//                        OLDIMesDataURZ mesData;
//                        mesData.iMesNum = master()->GetOLDIProc()->GetNextMesNum();
//                        mesData.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
//                        mesData.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
//                        mesData.sDBType = QString(OLDIMesURZ::enMeteo);
//                        mesData.sFirIdName = " ";
//                        QByteArray arr;
//                        QDataStream strm(&arr, QIODevice::WriteOnly);
//                        strm << meteo;
//                        mesData.sHexData = arr.toHex();
//                        QSharedPointer<OLDIMessage> pMes(new OLDIMesURZ(mesData));
//                        master()->GetOLDIProc()->SendMessage(pMes, false);
                    }
                }
            }

            mMeteoOld = model->meteoList();
            mCurrentRunwayOld = ui->comboBoxRunways->currentText();

            // Do not need this button visible as last changes has been saved.
            ui->toolButtonApply->hide();
            ui->toolButtonReset->hide();

            // Last edit time setting.
            ui->labelEditTime->setText( QDateTime::currentDateTime().toString("HH:mm:ss") );
        }
        TVA_CATCHALL(err)
        {
            LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
            QMessageBox::warning(this, tr("Warning"), err.reason());
        }
    }
}

void MeteoRunwayWidget::on_toolButtonReset_clicked()
{
    MeteoRunwayModel *model = dynamic_cast<MeteoRunwayModel*>( ui->tableView->model() );
    if ( model )
    {
        model->setMeteo( mMeteoOld );
        model->setCurrentRunway( mCurrentRunwayOld );

        ui->comboBoxRunways->blockSignals(true);
        ui->comboBoxRunways->setCurrentIndex( ui->comboBoxRunways->findText(mCurrentRunwayOld) );
        ui->comboBoxRunways->blockSignals(false);

        ui->toolButtonApply->hide();
        ui->toolButtonReset->hide();

        SizeToContents(true);
    }
}

bool MeteoRunwayWidget::isDataChanged() const
{
    MeteoRunwayModel *model = dynamic_cast<MeteoRunwayModel*>( ui->tableView->model() );
    if ( !model )
        return false;

    MeteoRunwayModel::TMeteoList meteoCur = model->meteoList();
    if ( meteoCur.size() != mMeteoOld.size() )
        return true;

    for ( int i = 0; i < meteoCur.size(); i++ )
    {
        const MeteoRunway &meteo = meteoCur.at(i);
        const MeteoRunway &meteoOld = mMeteoOld.at(i);

        if ( meteo != meteoOld )
            return true;
    }

    return false;
}

bool MeteoRunwayWidget::isAirportOnFIR(const QString& sAirport) const
{
    if(!sAirport.isEmpty())
    {
        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
        if(master() && view && view->GetMainFIR())
        {
            QSharedPointer<Scene> pFIR = view->GetMainFIR();
            Scene::TAirportsMap::const_iterator itAirport = pFIR->GetAirports().constFind(CAirportKey(sAirport));
            if(itAirport != pFIR->GetAirports().end())
            {
                return true;
            }
        }
    }
    return false;
}

QString MeteoRunwayWidget::GetAirportName()
{
    QString sAirportName = networkSettings()->value("meteo/airport").toString();
    if(!isAirportOnFIR(sAirportName))
        sAirportName = "";
    MeteoFilterDlg dlgFilter(this);
    if(dlgFilter.isFilter())
        sAirportName = dlgFilter.GetAirport();
    return sAirportName;
}

void MeteoRunwayWidget::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

//    QSettings settings;
//    settings.beginGroup(currentUserName);
//    restoreGeometry( settings.value(objectName()).toByteArray() );
//    ui->tableView->horizontalHeader()->restoreState(
//                settings.value(gMeteoHeaderStateName).toByteArray() );
}

const QSize MeteoRunwayWidget::GetVisibleSize() const
{
    int layoutSpacing = qobject_cast<QBoxLayout*>( layout() ) ?
                qobject_cast<QBoxLayout*>( layout() )->spacing() : 0;
    int iWidth = ui->tableView->horizontalHeader()->length(),
        iHeight = ui->tableView->verticalHeader()->length();
    iHeight += ui->tableView->horizontalHeader()->isVisible() ? ui->tableView->horizontalHeader()->height() : 0;
    iHeight += ui->widgetTop->isVisible() ? qMax(ui->widgetTop->height(), 20) + layoutSpacing : 0;
    iHeight += ui->widgetBottom->isVisible() ? qMax(ui->widgetBottom->height(), 22) + layoutSpacing : 0;
#ifdef Q_OS_LINUX
    iWidth += 8;
    iHeight += 42;
#endif
    return QSize(iWidth + 4, iHeight + 4);
}

void MeteoRunwayWidget::SizeToContents(bool bSend/* = true*/)
{
    ui->tableView->resizeColumnsToContents();
    if (bSend)
        emit sizeChanged();
}

void MeteoRunwayWidget::on_toolButtonFilter_clicked()
{
    QString sAirportName = GetAirportName();
    MeteoFilterDlg dlgFilter(this);
    dlgFilter.init(sAirportName);
    if(dlgFilter.exec() == QDialog::Accepted)
    {
        init();
    }
}

void MeteoRunwayWidget::on_editRow_clicked()
{
    if(ui->tableView->currentIndex().isValid())
    {
        MeteoRunwayDialog dialog(this);
        MeteoRunwayModel *model = dynamic_cast<MeteoRunwayModel*>( ui->tableView->model() );
        dialog.setMeteo(model->meteoList().at(ui->tableView->currentIndex().row()));

        if (dialog.exec() == QDialog::Accepted)
        {
            QList<MeteoRunway> list = model->meteoList();
            list[ui->tableView->currentIndex().row()] = dialog.meteo();
            model->setMeteo(list);

            setModeldataChanged(QModelIndex(), QModelIndex());
        }
    }
}

void MeteoRunwayWidget::OnTimerSize()
{
    SizeToContents(true);
}

void MeteoRunwayWidget::Refresh()
{
    if(!isDataChanged())
        init();
}
