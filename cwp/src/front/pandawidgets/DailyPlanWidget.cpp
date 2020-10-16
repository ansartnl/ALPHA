#include <QGridLayout>
#include <QToolButton>
#include <QScrollBar>
#include <QCloseEvent>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>

#include "DailyPlanWidget.h"
#include "main.h"
#include <QSharedPointer>
#include "ui_DailyPlanWidget.h"

#include "core/airplane/AClist/SFPL.h"

#include "back/core/CFPL.h"
#include "Master.h"
#include "dbconnectionsmanager.h"

class CSFPL;
const QString DailyPlanWidget::kWhiteBackgroundStyle = "background-color: rgb(255, 255, 255);";
const QString DailyPlanWidget::kOrangeBackgroundStyle = "background-color: rgb(255, 127, 0);";
const QString DailyPlanWidget::kGrayBackgroundStyle = "background-color: rgb(147, 147, 147);";
const QString DailyPlanWidget::kBorderStyle = "border: 1px solid #8f8f91";


DailyPlanWidget::DailyPlanWidget(const QString &acid, const QString &code,
                                     QWidget *parent, Qt::WindowFlags flags)
    : QFrame(parent, flags)
    , ui(new Ui::DailyPlanWidget)
    , first_show_(true)
    , visible_buttons_count_(0)
    , grid_visible_(true)
{
    ui->setupUi(this);
    //ui->labelACID->setText("ACID");

    QFont  font = qApp->font();
    double pointSize = font.pointSize();
    double small = networkSettings()->value("users/TextSize/Small", "8").toInt();
    koef = pointSize/small;
    double w = ui->scrollArea->widget()->width();
    ui->scrollArea->widget()->setFixedWidth(w*koef);
    this->setFixedWidth(width()*koef);
    ui->labelACID->setText(acid);
    code_ = code;
    setValues(loadShortFpls());

    //setFrameShape(QFrame::Box);

    if (parent) // fix Qt 5.10 crash
        setAttribute(Qt::WA_WindowPropagation);

    connect(ui->butNew, SIGNAL(clicked()), this, SLOT(onButNew()));
}

DailyPlanWidget::~DailyPlanWidget()
{

}

void DailyPlanWidget::setACID(const QString &acid)
{
    ui->labelACID->setText(acid);
}


void DailyPlanWidget::setValues(const QList<ShortFpl> &values)
{
    first_show_ = true;
    short_fpls_ = values;

    init();
}

bool DailyPlanWidget::eventFilter(QObject *obj, QEvent *event)
{
    QToolButton *button = qobject_cast<QToolButton *>(obj);
    if (!button || !buttons_.contains(button))
        return false;

    switch (event->type()) {
    case QEvent::Enter:
        if (grid_visible_)
            button->setStyleSheet(QString(kOrangeBackgroundStyle) + kBorderStyle);
        else
            button->setStyleSheet(kOrangeBackgroundStyle);
        break;
    case QEvent::Leave:
        if (grid_visible_)
            button->setStyleSheet(button->property("forebackcolor").toString() + kBorderStyle);
        else
            button->setStyleSheet(button->property("forebackcolor").toString());
        break;
    default:
        return false;
    }

    return true;
}

void DailyPlanWidget::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);
}

void DailyPlanWidget::closeEvent(QCloseEvent *event)
{

}

void DailyPlanWidget::toolClicked()
{
    foreach (const FPL &mFpl, Fpls) {
        qDebug()<<"++++++++++++++++++++planid_mfpl="<<mFpl.id()<<"      planid_sender="<<sender()->property("planId").toInt();
    if(mFpl.id() == sender()->property("planId").toInt()){
    QSharedPointer<CSFPL> sfpl_(new CSFPL);
    sfpl_->SetACID(mFpl.aircarftId());
    sfpl_->SetCODE(code_.toInt(0, 8));
    //sfpl_->SetCOPin(act_in.cop);
    //sfpl_->SetCOPinFL(FLVal(act_in.pel, true));
    sfpl_->SetCFL(sfpl_->GetCOPinFL());
    sfpl_->SetDepAerodrome(mFpl.departureAerodrome());
    sfpl_->SetDestAerodrome(mFpl.destinationAerodrome());
    //sfpl_->SetRoute(newRoute);
    sfpl_->SetRoute(mFpl.route());
    sfpl_->SetFRules(mFpl.flightRules());
    sfpl_->SetFType(mFpl.flightType());
    sfpl_->SetTurbulence(mFpl.turbulenceCategory());
    sfpl_->SetLit(mFpl.lit());
    sfpl_->SetSpeed(mFpl.speed());
    sfpl_->SetLevel(mFpl.level());
    sfpl_->SetTypeFlight(mFpl.aircraftType());
    sfpl_->SetAlt1Aerodrome(mFpl.altDestinationAerodrome());
    sfpl_->SetStatus(mFpl.exFPL());
    sfpl_->SetETA(mFpl.ETA());
    sfpl_->SetFPLEqf1(mFpl.equipment().split("/").front());
    sfpl_->SetFPLEqf2(mFpl.equipment().split("/").last());
    QString eq="";
    if(mFpl.equipment().split("/").front().contains("W"))
        eq += "w";
    if(mFpl.equipment().split("/").front().contains("Y"))
        eq += "y";
    sfpl_->SetEquipment(eq);

    /*if (act_in_model_->type() == ActTableModel::act_dep)
    {
        sfpl_->SetETOin(mFpl.departureTime());
        sfpl_->SetETD(act_in.eto);
    }
    else
    {
        sfpl_->SetETOin(act_in.eto);
        sfpl_->SetETD(mFpl.ETD());
    }*/

    bool ok = master()->getRedisDB()->saveSFPL(sfpl_);

    /*int min = networkSettings()->value("users/ORCAMLifeTime", 1).toInt();

    QMultiMap<QString, QString> map;
    master()->getRedisDB()->loadOrcams(map);
    QMapIterator <QString, QString> iter(map);
    bool isCodeOrcam = false;
    while (iter.hasNext()){
        iter.next();
        if(act_in.code == iter.key()){
            isCodeOrcam = true;
            break;
        }
    }
    bool okk = true;
    if (!isCodeOrcam)
        bool okk = master()->getRedisDB()->addOrcam(act_in.code, QDateTime::currentDateTime(), min);*/

    if (!ok)
        QMessageBox::critical(this, tr("Error"), tr("Error saving FPL"));
    else
        logOperation(tr("Assume + DailyPlan: ( %1 )")
                     .arg((QStringList()
                           << tr("equipment = '%1'").arg(sfpl_->GetEquipment())
                           << tr("acid = '%1'").arg(sfpl_->GetACID())
                           << tr("code = '%1'").arg(sfpl_->GetCODEtoString())
                           << tr("cop in = '%1'").arg(sfpl_->GetCOPin())
                           << tr("eto in = '%1'").arg(sfpl_->GetETOin().toString())
                           << tr("cop in fl = '%1'").arg(sfpl_->GetCOPinFL().toString())
                           << tr("cfl = '%1'").arg(sfpl_->GetCFL().toString())
                           << tr("dep = '%1'").arg(sfpl_->GetDepAerodrome())
                           << tr("dest = '%1'").arg(sfpl_->GetDestAerodrome())).join(" | ")));
    }
    }
    foreach (QToolButton * button, buttons_) {
        disconnect(button, SIGNAL(clicked()), this, SLOT(toolClicked()));
    }

    emit accepted();
    close();
}

QList<DailyPlanWidget::ShortFpl> DailyPlanWidget::loadShortFpls()
{
    static QList<ShortFpl> shortFpls;
    shortFpls.clear();

        QSqlDatabase db = QSqlDatabase::database();
        if(DBConnectionsManager::instance().getConnectionStatus() == 1)
        //if (db.isOpen())
        {
            QSqlQuery query(db);

            QString querystr;
            if(ui->labelACID->text() == "ACID")
               querystr = "SELECT id,fir_name,aircraft_id, taircraft,frules, tflight,num,wtc,equipment,adep,time::date as d, time::time as t,speed,flevel,route,"
                            "ades,alt1,alt2,exfpl,lit,eet "
                           "FROM fpl WHERE time::date >= NOW()::date - INTERVAL '1 days' "
                           "AND time::date <= NOW()::date + INTERVAL '1 days'";

            else
                querystr = "SELECT id,fir_name,aircraft_id, taircraft,frules, tflight,num,wtc,equipment,adep,time::date as d, time::time as t,speed,flevel,route,"
                          "ades,alt1,alt2,exfpl,lit,eet "
                          "FROM fpl WHERE time::date >= NOW()::date - INTERVAL '1 days' "
                          "AND time::date <= NOW()::date + INTERVAL '1 days' AND aircraft_id = '" + ui->labelACID->text() + "'";

            if(query.exec(querystr))
            {
                while (query.next())
                {
                    ShortFpl fpl;
                    fpl.id = query.value("id").toInt();
                    fpl.acid = query.value("aircraft_id").toString();
                    fpl.date = query.value("d").toString();
                    fpl.time = query.value("t").toString().remove(5,3);
                    fpl.adep = query.value("adep").toString();
                    fpl.ades = query.value("ades").toString();

                    mFpl.setId(query.value("id").toInt());
                    mFpl.setAircraftId(query.value("aircraft_id").toString());
                    mFpl.setDepartureTime(query.value("eet").toTime());
                    mFpl.setDepartureAerodrome(query.value("adep").toString());
                    mFpl.setDestAerodrome(query.value("ades").toString());
                    mFpl.setRoute(query.value("route").toString());
                    mFpl.setFlightRules(query.value("frules").toString());
                    mFpl.setFlightType(query.value("tflight").toString());
                    mFpl.setTurbulenceCategory(query.value("wtc").toString());
                    mFpl.setLit(query.value("lit").toString());
                    mFpl.setSpeed(query.value("speed").toString());
                    mFpl.setLevel(query.value("flevel").toString());
                    mFpl.setAircraftType(query.value("taircraft").toString());
                    mFpl.setAltDestinationAerodrome(query.value("alt1").toString());
                    mFpl.setExFPL(query.value("exfpl").toString());
                    mFpl.setETD(query.value("t").toTime());
                    mFpl.setETA(query.value("eet").toTime());
                    mFpl.setEquipment(query.value("equipment").toString());

                    if (!fpl.acid.isEmpty()) {
                        shortFpls.append(fpl);
                    }

                    if (!mFpl.aircarftId().isEmpty()) {
                        Fpls.append(mFpl);
                    }
                }
            }
        }

    return shortFpls;
}

void DailyPlanWidget::init()
{
    foreach (QToolButton * button, buttons_) {
        ui->gridLayout->removeWidget(button);
        delete button;
    }
    buttons_.clear();

    int i = 0;
    foreach (const ShortFpl &fpl, short_fpls_) {
        QToolButton *button = new QToolButton(ui->scrollArea->widget());
        button->setToolButtonStyle(Qt::ToolButtonTextOnly);
        button->setProperty("forebackcolor", kWhiteBackgroundStyle);
        button->setProperty("visiblebutton", 0);

        button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
        button->setFixedWidth(width());

        button->setText(fpl.acid + "  " + fpl.date + "  " + fpl.time + "  " + fpl.adep + "  " + fpl.ades);

        button->setProperty("planId", fpl.id);
        connect(button, SIGNAL(clicked()), this, SLOT(toolClicked()));
        button->installEventFilter(this);

        buttons_.insert(button);
        ui->gridLayout->addWidget(button, i, 0, 2, 0, Qt::AlignTop|Qt::AlignLeft );
        if(i > 0){
            QSize size_item;
            size_item = QSize(width(), height() + button->height()*koef);
            setFixedSize(size_item);
            ui->gridLayout->update();
        }

        ++i;
    }
}

void DailyPlanWidget::onButNew()
{
    emit accepted();
    close();
}



