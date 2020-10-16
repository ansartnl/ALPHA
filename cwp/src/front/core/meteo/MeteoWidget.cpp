#include "MeteoWidget.h"
#include "ui_MeteoWidget.h"

#include "main.h"
#include "Master.h"
#include "View.h"
#include "Scene.h"
#include "back/utils/ProfileSettings.h"

#include "DBQuery.h"
#include "dbconnectionsmanager.h"
#include "databasesubscriber.h"
#include "SectoringModelImpl.h"
#include "User.h"

#include <QtCore/QProcess>

#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlError>
#include <QLabel>
#include <QScrollBar>

MeteoWidget::MeteoWidget(QString _privileges, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeteoWidget),idAirport(-1), idAirportStore(0)

{
    ui->setupUi(this);
    privileges = _privileges;
    //layoutGrid = new QGridLayout(this);
    color_notoperativ = UserProfile::CSettings::Instance().GetColor(UserProfile::CtMeteo);
    lifetime = networkSettings()->value("meteo/metar_lifetime").toString();
    const QString fir_names = networkSettings()->value("FPL_handler/fir", "").toString();
    const QStringList fir_names_list = fir_names.split(",", QString::SkipEmptyParts);

    firs = "code_fir = '" + fir_names_list.at(0) + "'";
    for ( int i = 1; i < fir_names_list.count(); i++ )
    {
        firs += " OR code_fir = '" + fir_names_list.at(i) + "'";
    }

    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOTAF_UNIQUE)){
        tabTaf = new QWidget();
        ui->tabWidget->addTab(tabTaf, "TAF");
        model_taf = new QSqlQueryModel();
        view_taf = new QTableView();
        view_taf->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        view_taf->verticalHeader()->hide();
        view_taf->horizontalHeader()->hide();

        UpdateModelTaf();

        QVBoxLayout *layoutTaf = new QVBoxLayout(this);
        layoutTaf->setContentsMargins(0, 0, 0, 0);
        layoutTaf->addWidget(view_taf);
        tabTaf->setLayout(layoutTaf);
    }
    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOMETAR_UNIQUE)){
        model_airports = new QSqlQueryModel();
        airportsName = networkSettings()->value("meteo/metar_airport").toString();
        airportsList = airportsName.split(",", QString::SkipEmptyParts);
        airportOur = networkSettings()->value("meteo/airport").toString();

        UpdateModelAirports();
        view_airports.setSelectionMode(QAbstractItemView::SingleSelection);
        view_airports.verticalHeader()->hide();
        view_airports.horizontalHeader()->hide();
        view_airports.setFixedWidth(75);
        view_airports.verticalScrollBar()->hide();
        view_airports.setStyleSheet("gridline-color: white;");

        tabMetar = new QWidget();
        ui->tabWidget->addTab(tabMetar, "METAR");
        w_direct = new QLineEdit();
        w_speed = new QLineEdit();
        qnh = new QLineEdit();
        w_max = new QLineEdit();
        w_direct_lateral = new QLineEdit();
        w_direct_axial = new QLineEdit();
        text = new QTextEdit();

        model_metar = new QSqlQueryModel();
        UpdateModelMetar();

        view_metar.verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        view_metar.verticalHeader()->hide();
        view_metar.hide();

        layoutMetar = new QHBoxLayout(this);
        layoutMetar->setContentsMargins(0, 0, 0, 0);
        layoutVert = new QVBoxLayout(this);
        layoutVert->setContentsMargins(0, 0, 0, 0);

        layoutMetar->addWidget(&view_airports);
        layoutMetar->addWidget(&view_metar);

        layoutGrid = new QGridLayout(this);
        layoutGrid->setContentsMargins(0, 0, 0, 0);

        layoutGrid->addWidget(new QLabel("W.direct:"),0,0,0);
        layoutGrid->addWidget(new QLabel("W.speed:"),0,1,0);
        layoutGrid->addWidget(new QLabel("W.max:"),0,2,0);
        layoutGrid->addWidget(new QLabel("CrossW:"),2,0,0);
        layoutGrid->addWidget(new QLabel("TailW:"),2,1,0);
        layoutGrid->addWidget(new QLabel("QNH:"),2,2,0);
        //layoutGrid->addWidget(new QLabel("  QFE:"),1,4,0);

        w_direct->setReadOnly(true);
        w_speed->setReadOnly(true);
        qnh->setReadOnly(true);
        w_max->setReadOnly(true);
        w_direct_lateral->setReadOnly(true);
        w_direct_axial->setReadOnly(true);
        text->setReadOnly(true);

        layoutGrid->addWidget(w_direct,1,0,0);
        layoutGrid->addWidget(w_speed,1,1,0);
        layoutGrid->addWidget(w_max,1,2,0);  // w.max
        layoutGrid->addWidget(w_direct_lateral,3,0,0);
        layoutGrid->addWidget(w_direct_axial,3,1,0);
        layoutGrid->addWidget(qnh,3,2,0);

        layoutVert->addLayout(layoutGrid);
        layoutVert->addWidget(text);
        layoutMetar->addLayout(layoutVert);
        tabMetar->setLayout(layoutMetar);

        DefaultSelectOurAirport();
    }
    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOSIGMET_UNIQUE)){
        tabSigmet = new QWidget();
        ui->tabWidget->addTab(tabSigmet, "SIGMET");
        model_sigmet = new QSqlQueryModel();
        view_sigmet = new QTableView();
        view_sigmet->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        view_sigmet->verticalHeader()->hide();
        view_sigmet->horizontalHeader()->hide();

        UpdateModelSigmet();

        QVBoxLayout *layoutSigmet = new QVBoxLayout(this);
        layoutSigmet->setContentsMargins(0, 0, 0, 0);
        layoutSigmet->addWidget(view_sigmet);
        tabSigmet->setLayout(layoutSigmet);
    }
    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOGAMET_UNIQUE)){
        tabGamet = new QWidget();
        ui->tabWidget->addTab(tabGamet, "GAMET");
        model_gamet = new QSqlQueryModel();
        view_gamet = new QTableView();
        //view_gamet->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        view_gamet->verticalHeader()->hide();
        view_gamet->horizontalHeader()->hide();
        connect(view_gamet, SIGNAL(clicked(QModelIndex)), this, SLOT(OnClicked(QModelIndex)));

        UpdateModelGamet();

        QVBoxLayout *layoutGamet = new QVBoxLayout(this);
        layoutGamet->setContentsMargins(0, 0, 0, 0);
        layoutGamet->addWidget(view_gamet);
        tabGamet->setLayout(layoutGamet);
    }
    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOMETREPORT_UNIQUE)){
        model_airports = new QSqlQueryModel();
        airportsName = networkSettings()->value("meteo/metar_airport").toString();
        airportsList = airportsName.split(",", QString::SkipEmptyParts);
        airportOur = networkSettings()->value("meteo/airport").toString();
        UpdateModelAirports();
        view_airports2.setSelectionMode(QAbstractItemView::SingleSelection);
        view_airports2.verticalHeader()->hide();
        view_airports2.horizontalHeader()->hide();
        view_airports2.setFixedWidth(75);
        view_airports2.verticalScrollBar()->hide();
        view_airports2.setStyleSheet("gridline-color: white;");

        tabMetreport = new QWidget();
        ui->tabWidget->addTab(tabMetreport, "METREPORT");
        model_metreport = new QSqlQueryModel();
        view_metreport.verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        view_metreport.verticalHeader()->hide();
        view_metreport.hide();

        w_direct2 = new QLineEdit();
        w_speed2 = new QLineEdit();
        qnh2 = new QLineEdit();
        w_max2 = new QLineEdit();
        w_direct_lateral2 = new QLineEdit();
        w_direct_axial2 = new QLineEdit();
        text2 = new QTextEdit();

        UpdateModelMetreport();

        layoutMetreport = new QHBoxLayout(this);
        layoutMetreport->setContentsMargins(0, 0, 0, 0);
        layoutVert2 = new QVBoxLayout(this);
        layoutVert2->setContentsMargins(0, 0, 0, 0);

        layoutMetreport->addWidget(&view_airports2);
        layoutMetreport->addWidget(&view_metreport);

        layoutGrid2 = new QGridLayout(this);
        layoutGrid2->setContentsMargins(0, 0, 0, 0);

        layoutGrid2->addWidget(new QLabel("W.direct:"),0,0,0);
        layoutGrid2->addWidget(new QLabel("W.speed:"),0,1,0);
        layoutGrid2->addWidget(new QLabel("W.max:"),0,2,0);
        layoutGrid2->addWidget(new QLabel("CrossW:"),2,0,0);
        layoutGrid2->addWidget(new QLabel("TailW:"),2,1,0);
        layoutGrid2->addWidget(new QLabel("QNH:"),2,2,0);
        //layoutGrid->addWidget(new QLabel("  QFE:"),1,4,0);

        w_direct2->setReadOnly(true);
        w_speed2->setReadOnly(true);
        qnh2->setReadOnly(true);
        w_max2->setReadOnly(true);
        w_direct_lateral2->setReadOnly(true);
        w_direct_axial2->setReadOnly(true);
        text2->setReadOnly(true);

        layoutGrid2->addWidget(w_direct2,1,0,0);
        layoutGrid2->addWidget(w_speed2,1,1,0);
        layoutGrid2->addWidget(w_max2,1,2,0);  // w.max
        layoutGrid2->addWidget(w_direct_lateral2,3,0,0);
        layoutGrid2->addWidget(w_direct_axial2,3,1,0);
        layoutGrid2->addWidget(qnh2,3,2,0);

        layoutVert2->addLayout(layoutGrid2);
        layoutVert2->addWidget(text2);
        layoutMetreport->addLayout(layoutVert2);
        tabMetreport->setLayout(layoutMetreport);

        DefaultSelectOurAirport();
    }

    timer = new QTimer(this);

    connect(ui->btnActual, SIGNAL(toggled(bool)), this, SLOT(onActualToggle(bool)));
    connect(ui->btnAll, SIGNAL(toggled(bool)), this, SLOT(onAllToggle(bool)));
    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOMETAR_UNIQUE))
    connect(view_airports.selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,
            SLOT(OnChangeSelect(const QItemSelection &, const QItemSelection &)));
    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOMETREPORT_UNIQUE))
    connect(view_airports2.selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,
            SLOT(OnChangeSelect2(const QItemSelection &, const QItemSelection &)));

    //connect(timer, SIGNAL(timeout()), this, SLOT(_OnTimer()));
    //timer->start(60000); // 1 min

    connect(DatabaseSubscriber::instance().subscribe("meteo_metar"), SIGNAL(notification()), SLOT(UpdateModelMetar()));
    connect(DatabaseSubscriber::instance().subscribe("meteo_taf"), SIGNAL(notification()), SLOT(UpdateModelTaf()));
    connect(DatabaseSubscriber::instance().subscribe("meteo_sigmet"), SIGNAL(notification()), SLOT(UpdateModelSigmet()));
    connect(DatabaseSubscriber::instance().subscribe("meteo_gamet"), SIGNAL(notification()), SLOT(UpdateModelGamet()));
    connect(DatabaseSubscriber::instance().subscribe("meteo_metreport"), SIGNAL(notification()), SLOT(UpdateModelMetreport()));

    UpdateXFont();
    SI();
    Translate();
}

MeteoWidget::~MeteoWidget()
{
    delete ui;
}

void MeteoWidget::Translate()
{
    ui->retranslateUi(this);
}

void MeteoWidget::UpdateXFont()
{
    QFont font1 = font();
    font1.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
    setFont(font1);

}

void MeteoWidget::SI()
{

}

const QSize MeteoWidget::GetVisibleSize() const
{
    QSize size = rect().size();
#ifdef Q_OS_LINUX
    size.setWidth(size.width()+8);
    size.setHeight(size.height()+42);
#endif

    return size;
}

void MeteoWidget::SizeToContents(bool bSend/* = true*/)
{
    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOMETAR_UNIQUE))
    for(int i = 0; i < view_metar.horizontalHeader()->count(); ++i)
    {
        if(view_metar.columnWidth(i))
            view_metar.resizeColumnToContents(i);
    }
    if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOMETREPORT_UNIQUE))
    for(int i = 0; i < view_metreport.horizontalHeader()->count(); ++i)
    {
        if(view_metreport.columnWidth(i))
            view_metreport.resizeColumnToContents(i);
    }
    if (bSend)
        emit sizeChanged();

}

void MeteoWidget::onAllToggle(bool toggle)
{
    if(toggle)
    {
        if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOMETAR_UNIQUE)){
            text->hide();
            int layoutItems = layoutGrid->count();
            for( int i = 0; i < layoutItems; ++i )
            {
                QLayoutItem* item = layoutGrid->itemAt(i);

                if( item )
                    item->widget()->hide();
            }
            view_metar.setVisible(true);
        }
        if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOMETREPORT_UNIQUE)){
            text2->hide();
            int layoutItems2 = layoutGrid2->count();
            for( int i = 0; i < layoutItems2; ++i )
            {
                QLayoutItem* item = layoutGrid2->itemAt(i);

                if( item )
                    item->widget()->hide();
            }
            view_metreport.setVisible(true);
        }

            UpdateModelTaf();
            UpdateModelSigmet();
            UpdateModelGamet();
            UpdateModelMetreport();
            UpdateModelMetar();
    }
}

void MeteoWidget::onActualToggle(bool toggle)
{
    if(toggle)
    {
        if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOMETAR_UNIQUE)){
            int layoutItems = layoutGrid->count();
            for( int i = 0; i < layoutItems; ++i )
            {
                QLayoutItem* item = layoutGrid->itemAt(i);
                if( item )
                    item->widget()->setVisible(true);
            }
            text->setVisible(true);
            view_metar.hide();
        }
        if (CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOMETREPORT_UNIQUE)){
            int layoutItems2 = layoutGrid2->count();
            for( int i = 0; i < layoutItems2; ++i )
            {
                QLayoutItem* item = layoutGrid2->itemAt(i);
                if( item )
                    item->widget()->setVisible(true);
            }
            text2->setVisible(true);
            view_metreport.hide();
        }
            UpdateModelTaf();
            UpdateModelSigmet();
            UpdateModelGamet();
            UpdateModelMetreport();
            UpdateModelMetar();
    }
}

void MeteoWidget::OnChangeSelect(const QItemSelection &itsnew, const QItemSelection &its)
{
    view_airports.setSelectionMode(QAbstractItemView::NoSelection);
    QModelIndexList selection = itsnew.indexes();
    QModelIndex index= selection.at(0);
    idAirport = model_airports->record(index.row()).value("id").toInt();
    UpdateModelMetar();
    view_airports.setSelectionMode(QAbstractItemView::SingleSelection);
}

void MeteoWidget::OnChangeSelect2(const QItemSelection &itsnew, const QItemSelection &its)
{
    view_airports2.setSelectionMode(QAbstractItemView::NoSelection);
    QModelIndexList selection = itsnew.indexes();
    QModelIndex index= selection.at(0);
    idAirport = model_airports->record(index.row()).value("id").toInt();
    UpdateModelMetreport();
    view_airports2.setSelectionMode(QAbstractItemView::SingleSelection);
}

void MeteoWidget::_OnTimer()
{
        UpdateModelTaf();
        UpdateModelMetar();
        UpdateModelSigmet();
        UpdateModelGamet();
        UpdateModelMetreport();
}

void MeteoWidget::UpdateModelMetar()
{
    if (!CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOMETAR_UNIQUE))
        return;
    if(!CheckDataBase())
         return;

    if(idAirport == 0)
    {
        UpdateModelAirports();
        idAirport = idAirportStore;
    }

    if(ui->btnAll->isChecked())
    {
        model_metar->setQuery("SELECT * FROM meteo_metar WHERE airport_id = " + QString::number(idAirport) +
                                 " ORDER BY datetime_metar DESC");
        if(model_metar->rowCount() != 0)
            SetColor("color: black;");
    }

    if(ui->btnActual->isChecked())
    {
        QString str = "SELECT * FROM meteo_metar WHERE airport_id = " + QString::number(idAirport) +
                                          " AND datetime_metar <= NOW() AND datetime_metar+ interval '" + lifetime + "' >= NOW()  ORDER BY datetime_metar DESC";
        model_metar->setQuery(str);
        if(model_metar->rowCount() != 0)
            SetColor("color: black;");

        if(model_metar->rowCount() == 0)
        {
            model_metar->setQuery("SELECT * FROM meteo_metar WHERE airport_id = " + QString::number(idAirport) +
                                    "ORDER BY datetime_metar DESC");
            if(model_metar->rowCount() != 0)
                SetColor("color:" + color_notoperativ.name());
        }
    }

    w_direct->setText(model_metar->record(0).value("w_direct").toString());
    w_speed->setText(model_metar->record(0).value("w_speed").toString());
    qnh->setText(model_metar->record(0).value("qnh").toString());
    //qfe->setText(model_metar->record(0).value("qfe").toString());
    w_max->setText(model_metar->record(0).value("w_max").toString());

    if(master() && master()->getRedisDB() && master()->GetView())
    {
        QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (pView)
        {
            if(pView->GetMainFIR())
            {
                RunwayWidgetDataFIR rwf(pView->GetMainFIR()->GetIDName());
                bool ok_runway1, ok_runway2;
                QString runway1 = networkSettings()->value("RW1/name").toString();
                if(!runway1.isEmpty())
                    ok_runway1 = true;
                QString runway2 = networkSettings()->value("RW2/name").toString();
                if(!runway2.isEmpty())
                    ok_runway2 = true;
                if(!((ok_runway1 && ok_runway2) || (!ok_runway1 && !ok_runway2)))
                {
                    //bool ok;
                    if(ok_runway1)
                        master()->getRedisDB()->loadRWF1(pView->GetMainFIR()->GetIDName(), rwf);
                    else if(ok_runway2)
                        master()->getRedisDB()->loadRWF2(pView->GetMainFIR()->GetIDName(), rwf);

                   if(!rwf.currentRunway().isNull() && !rwf.runwaysList().isEmpty())
                   {
                    if(rwf.currentRunway() == rwf.runwaysList().values().first().name())
                    {
                        w_direct_lateral->setText(model_metar->record(0).value("w_direct_lateral_course1").toString());
                        w_direct_axial->setText(model_metar->record(0).value("w_direct_axial_course1").toString());

                    }

                    if(rwf.currentRunway() == rwf.runwaysList().values().last().name())
                    {
                        w_direct_lateral->setText(model_metar->record(0).value("w_direct_lateral_course2").toString());
                        w_direct_axial->setText(model_metar->record(0).value("w_direct_axial_course2").toString());
                    }
                   }
                }

            }
        }
    }

    text->setText(model_metar->record(0).value("text_metar").toString());
    view_metar.horizontalHeader()->hide();
    view_metar.setModel(model_metar);

    for(int i = 0; i < model_metar->columnCount(); i++){
        view_metar.setColumnHidden(i,true);
    }

    int column = model_metar->record().indexOf("text_metar");
    if (column != -1)
    {
        view_metar.setColumnHidden(column, false);
    }
   view_metar.horizontalHeader()->setResizeMode( QHeaderView::Stretch );

}

void MeteoWidget::UpdateModelTaf()
{
    if(!CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOTAF_UNIQUE))
        return;
    if(!CheckDataBase())
        return;

    if(ui->btnAll->isChecked())
    {
        QString queryTaf;
        queryTaf = "SELECT * FROM meteo_taf t LEFT JOIN airport a ON a.id = t.airport_id WHERE a.name IN (''";
        foreach(QString airport, airportsList)
        {
            queryTaf += ",'" + airport + "'";
        }
        queryTaf += ") ORDER BY datetimeend DESC";
        model_taf->setQuery(queryTaf);

        tabTaf->setStyleSheet("color: black;");
    }

    if(ui->btnActual->isChecked())
    {

        QString queryTaf;
        queryTaf = "SELECT * FROM meteo_taf t LEFT JOIN airport a ON a.id = t.airport_id WHERE a.name IN (''";
        foreach(QString airport, airportsList)
        {
            queryTaf += ",'" + airport + "'";
        }
        queryTaf += ") AND datetimebegin <= NOW() AND datetimeend >= NOW() ORDER BY datetimeend DESC";
        model_taf->setQuery(queryTaf);

        tabTaf->setStyleSheet("color: black;");
        if(model_taf->rowCount() == 0)
        {
            model_taf->setQuery("SELECT ' ',' ',' ',' ',' ';");
            tabTaf->setStyleSheet("color:" + color_notoperativ.name());
        }
    }

    model_taf->setHeaderData(4, Qt::Horizontal, "TEXT");

    view_taf->setModel(model_taf);

    for(int i = 0; i < model_taf->columnCount(); i++){
        view_taf -> setColumnHidden(i,true);
    }

    int column = model_taf->record().indexOf("text_taf");
    if (column != -1)
    {
        view_taf -> setColumnHidden(column, false);
    }

    view_taf-> horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    // view_taf-> setStyleSheet("gridline-color: red;");
    // view_taf->horizontalHeader()->setDefaultSectionSize(460);

}

void MeteoWidget::UpdateModelSigmet()
{
    if(!CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOSIGMET_UNIQUE))
        return;
    if(!CheckDataBase())
        return;

    if(ui->btnAll->isChecked())
    {
        model_sigmet->setQuery("SELECT * FROM meteo_sigmet WHERE " + firs + " ORDER BY datetimeend DESC");
        tabSigmet->setStyleSheet("color: black;");
    }

    if(ui->btnActual->isChecked())
    {
        model_sigmet->setQuery("SELECT * FROM meteo_sigmet WHERE datetimebegin <= NOW() AND datetimeend >= NOW() AND (" + firs + ") ORDER BY datetimeend DESC");

        tabSigmet->setStyleSheet("color: black;");
        if(model_sigmet->rowCount() == 0)
        {
            model_sigmet->setQuery("SELECT ' ',' ',' ',' ',' ',' ',' ';");
            tabSigmet->setStyleSheet("color:" + color_notoperativ.name());
        }
    }

    model_sigmet->setHeaderData(6, Qt::Horizontal, "TEXT");

    view_sigmet->setModel(model_sigmet);

    for(int i = 0; i < model_sigmet->columnCount(); i++){
        view_sigmet -> setColumnHidden(i,true);
    }

    int column = model_sigmet->record().indexOf("text_sigmet");
    if (column != -1)
    {
        view_sigmet -> setColumnHidden(column, false);
    }

    view_sigmet-> horizontalHeader()->setResizeMode( QHeaderView::Stretch );

}

void MeteoWidget::UpdateModelMetreport()
{
    if(!CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOMETREPORT_UNIQUE))
        return;
    if(!CheckDataBase())
        return;

    if(idAirport == 0)
    {
        UpdateModelAirports();
        idAirport = idAirportStore;
    }

    /*if(ui->btnAll->isChecked())
    {
        QString queryMetreport;
        queryMetreport = "SELECT * FROM meteo_metreport mr LEFT JOIN airport a ON a.id = mr.airport_id WHERE a.name IN (''";
        foreach(QString airport, airportsList)
        {
            queryMetreport += ",'" + airport + "'";
        }

        queryMetreport += ") ORDER BY datetime_metreport DESC";

        model_metreport->setQuery(queryMetreport);
        SetColorMetreport("color: black;");
    }

    if(ui->btnActual->isChecked())
    {
        QString queryMetreport;
        queryMetreport = "SELECT * FROM meteo_metreport mr LEFT JOIN airport a ON a.id = mr.airport_id WHERE a.name IN (''";
        foreach(QString airport, airportsList)
        {
            queryMetreport += ",'" + airport + "'";
        }

        queryMetreport += ") AND datetime_metreport <= NOW() AND datetime_metreport + interval '" + lifetime +
                                          "' >= NOW() ORDER BY datetime_metreport DESC";

        model_metreport->setQuery(queryMetreport);
        SetColorMetreport("color: black;");

        if(model_metreport->rowCount() == 0)
        {
            model_metreport->setQuery("SELECT ' ',' ',' ',' ',' ';");
            SetColorMetreport("color:" + color_notoperativ.name());
        }
    }*/


    if(ui->btnAll->isChecked())
    {
        model_metreport->setQuery("SELECT * FROM meteo_metreport WHERE airport_id = " + QString::number(idAirport) +
                                 " ORDER BY datetime_metreport DESC");
        if(model_metreport->rowCount() != 0)
            SetColorMetreport("color: black;");
    }

    if(ui->btnActual->isChecked())
    {
        QString str = "SELECT * FROM meteo_metreport WHERE airport_id = " + QString::number(idAirport) +
                      " AND datetime_metreport <= NOW() AND datetime_metreport+ interval '" + lifetime + "' >= NOW()  ORDER BY datetime_metreport DESC";

        model_metreport->setQuery(str);

        if(model_metreport->rowCount() != 0)
            SetColor("color: black;");

        if(model_metreport->rowCount() == 0)
        {
            model_metreport->setQuery("SELECT * FROM meteo_metreport WHERE airport_id = " + QString::number(idAirport) +
                                    "ORDER BY meteo_metreport DESC");
            if(model_metreport->rowCount() != 0)
                SetColorMetreport("color:" + color_notoperativ.name());
        }

    }


    w_direct2->setText(model_metreport->record(0).value("w_direct").toString());

    w_speed2->setText(model_metreport->record(0).value("w_speed").toString());
    qnh2->setText(model_metreport->record(0).value("qnh").toString());
    w_max2->setText(model_metreport->record(0).value("w_max").toString());


    if(master() && master()->getRedisDB() && master()->GetView())
    {
        QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (pView)
        {
            if(pView->GetMainFIR())
            {
                RunwayWidgetDataFIR rwf(pView->GetMainFIR()->GetIDName());
                bool ok_runway1, ok_runway2;
                QString runway1 = networkSettings()->value("RW1/name").toString();
                if(!runway1.isEmpty())
                    ok_runway1 = true;
                QString runway2 = networkSettings()->value("RW2/name").toString();
                if(!runway2.isEmpty())
                    ok_runway2 = true;
                if(!((ok_runway1 && ok_runway2) || (!ok_runway1 && !ok_runway2)))
                {
                    //bool ok;
                    if(ok_runway1)
                        master()->getRedisDB()->loadRWF1(pView->GetMainFIR()->GetIDName(), rwf);
                    else if(ok_runway2)
                        master()->getRedisDB()->loadRWF2(pView->GetMainFIR()->GetIDName(), rwf);

                   if(!rwf.currentRunway().isNull() && !rwf.runwaysList().isEmpty())
                   {
                    if(rwf.currentRunway() == rwf.runwaysList().values().first().name())
                    {
                        w_direct_lateral2->setText(model_metreport->record(0).value("w_direct_lateral_course1").toString());
                        w_direct_axial2->setText(model_metreport->record(0).value("w_direct_axial_course1").toString());

                    }

                    if(rwf.currentRunway() == rwf.runwaysList().values().last().name())
                    {
                        w_direct_lateral2->setText(model_metreport->record(0).value("w_direct_lateral_course2").toString());
                        w_direct_axial2->setText(model_metreport->record(0).value("w_direct_axial_course2").toString());
                    }
                   }
                }

            }
        }
    }

    text2->setText(model_metreport->record(0).value("text_metreport").toString());

    view_metreport.horizontalHeader()->hide();
    view_metreport.setModel(model_metreport);

    for(int i = 0; i < model_metreport->columnCount(); i++){
        view_metreport.setColumnHidden(i,true);
    }

    int column = model_metreport->record().indexOf("text_metreport");
    if (column != -1)
    {
        view_metreport.setColumnHidden(column, false);
    }
   view_metreport.horizontalHeader()->setResizeMode( QHeaderView::Stretch );

}

void MeteoWidget::UpdateModelGamet()
{
    if(!CUserRole::IsPermission(privileges, CSectoringModel::LISTMETEOGAMET_UNIQUE))
        return;
    if(!CheckDataBase())
        return;

    if(ui->btnAll->isChecked())
    {
        model_gamet->setQuery("SELECT * FROM meteo_gamet WHERE " + firs + " ORDER BY datetimeend DESC");
        tabGamet->setStyleSheet("color: black;");
    }

    if(ui->btnActual->isChecked())
    {
        model_gamet->setQuery("SELECT * FROM meteo_gamet WHERE datetimebegin <= NOW() AND datetimeend >= NOW() AND (" + firs + ") ORDER BY datetimeend DESC");
        tabGamet->setStyleSheet("color: black;");
        if(model_gamet->rowCount() == 0)
        {
            model_gamet->setQuery("SELECT ' ',' ',' ',' ',' ';");
            tabGamet->setStyleSheet("color:" + color_notoperativ.name());
        }
    }

    model_gamet->setHeaderData(4, Qt::Horizontal, "TEXT");

    view_gamet->setModel(model_gamet);

    for(int i = 0; i < model_gamet->columnCount(); i++){
        view_gamet->setColumnHidden(i,true);
    }

    int column = model_gamet->record().indexOf("text_gamet");
    if (column != -1)
    {
        view_gamet->setColumnHidden(column, false);
    }

    view_gamet->horizontalHeader()->setResizeMode( QHeaderView::Stretch );

}


void MeteoWidget::SetColor(QString color)
{
    w_direct->setStyleSheet(color);
    w_speed->setStyleSheet(color);
    qnh->setStyleSheet(color);
    w_max->setStyleSheet(color);
    w_direct_lateral->setStyleSheet(color);
    w_direct_axial->setStyleSheet(color);
    text->setStyleSheet(color);
}

void MeteoWidget::SetColorMetreport(QString color)
{
    w_direct2->setStyleSheet(color);
    w_speed2->setStyleSheet(color);
    qnh2->setStyleSheet(color);
    w_max2->setStyleSheet(color);
    w_direct_lateral2->setStyleSheet(color);
    w_direct_axial2->setStyleSheet(color);
    text2->setStyleSheet(color);
}

void MeteoWidget::UpdateModelAirports()
{
    if(CheckDataBase())
    {
        queryStr = "SELECT id,name FROM airport WHERE name IN (''";
        foreach(QString airport, airportsList)
        {
            queryStr += ",'" + airport + "'";
        }
        queryStr += ") ORDER BY name";
        model_airports->setQuery(queryStr);
        if(view_airports.isEnabled()){
        view_airports.setModel(model_airports);
        view_airports.setColumnHidden(0,true);
        connect(view_airports.selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,
                SLOT(OnChangeSelect(const QItemSelection &, const QItemSelection &)));
        }
        if(view_airports2.isEnabled()){
        view_airports2.setModel(model_airports);
        view_airports2.setColumnHidden(0,true);
        connect(view_airports2.selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,
                SLOT(OnChangeSelect2(const QItemSelection &, const QItemSelection &)));
        }

    }
}

void MeteoWidget::ClearMetar()
{
    w_direct->clear();
    w_speed->clear();
    qnh->clear();
    w_max->clear();
    w_direct_lateral->clear();
    w_direct_axial->clear();
    text->clear();
}

void MeteoWidget::ClearMetreport()
{
    w_direct2->clear();
    w_speed2->clear();
    qnh2->clear();
    w_max2->clear();
    w_direct_lateral2->clear();
    w_direct_axial2->clear();
    text2->clear();
}
bool MeteoWidget::CheckDataBase()
{
    QSqlDatabase database = QSqlDatabase::database();

    ui->btnActual->setEnabled(false);
    ui->btnAll->setEnabled(false);
    ui->tabWidget->setEnabled(false);

    int exitCode = QProcess::execute("ping", QStringList() << "-c" << "1" << "-W" << "1" << database.hostName());
    if (exitCode!=0 || !database.isOpen())
    //if (DBConnectionsManager::instance().getConnectionStatus() == 1 )
    {
        view_airports.setModel(0);
        view_metar.setModel(0);
        view_taf->setModel(0);
        model_airports = new QSqlQueryModel();
        model_taf = new QSqlQueryModel();
        model_metar = new QSqlQueryModel();

        if(idAirport != -1)
            idAirportStore = idAirport;
        idAirport = 0;

        ClearMetar();
        ClearMetreport();

        connect(ui->btnActual, SIGNAL(toggled(bool)), this, SLOT(onActualToggle(bool)));
        connect(ui->btnAll, SIGNAL(toggled(bool)), this, SLOT(onAllToggle(bool)));
        return false;
    }

    ui->btnActual->setEnabled(true);
    ui->btnAll->setEnabled(true);
    ui->tabWidget->setEnabled(true);

    return true;
}

void MeteoWidget::onUpdateMeteo()
{
        UpdateModelTaf();
        UpdateModelMetar();
        UpdateModelSigmet();
        UpdateModelGamet();
        UpdateModelMetreport();
}

void MeteoWidget::DefaultSelectOurAirport()
{
    QModelIndex current_index ;
    for(int i = 0; i < view_airports.model()->rowCount(); i++)
    {
        if(model_airports->record(i).value("name").toString() == airportOur)
            current_index = view_airports.model()->index(i, 1);
    }
    if(current_index.isValid())
      view_airports.setCurrentIndex(current_index);
}

void MeteoWidget::OnClicked(QModelIndex index)
{
    QTextEdit *edit = new QTextEdit();
    edit->setText(index.data(Qt::DisplayRole).toString());
    view_gamet->setIndexWidget(index, edit);
}


