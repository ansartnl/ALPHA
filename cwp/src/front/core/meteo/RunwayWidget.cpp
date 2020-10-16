#include "RunwayWidget.h"
#include "ui_RunwayWidget.h"

#include "main.h"
#include "Master.h"
#include "GUIView.h"
#include "Scene.h"
#include "back/utils/ProfileSettings.h"
#include <QtSql>
#include <QDebug>
#include "dbconnectionsmanager.h"

QString RunwayWidget::mCurrentRunway1;
QString RunwayWidget::mCurrentRunway2;

RunwayWidget::RunwayWidget(RunwayType type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunwayWidget), mInvokeSave(false), mInvokeSave2(false), player(0), _type(type)
{
    ui->setupUi(this);
    ui->comboBoxTL->setStyleSheet("QComboBox {color:black;}");
    //ui->comboBoxTL->addItem("090", 30);
    ui->comboBoxTL->addItem("90", 30);
    ui->comboBoxTL->addItem("120", 40);
    ui->comboBoxTL->addItem("150", 50);
    ui->comboBoxTL->addItem("180", 60);
    ui->comboBoxTL->addItem("210", 70);
    ui->comboBoxTL->addItem("240", 80);
    ui->comboBoxTL->addItem("270", 90);
    ui->comboBoxTL->addItem("300", 100);
    ui->comboBoxTL->addItem("330", 110);
    ui->comboBoxTL->addItem("360", 120);
    ui->comboBoxTL->setEditable(true);
    QIntValidator* pIntValidator = new QIntValidator(this);
    pIntValidator->setRange(0, 999);
    ui->comboBoxTL->setValidator(pIntValidator);
    ui->comboBoxTL->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    if (networkSettings()->value("runway_alarm", true).toBool())
    {
        player = new AudioPlayer(QDir("share:").absoluteFilePath("alarm.wav"),
                                 networkSettings()->value("runway_alarm_interval", 0).toInt(),
                                 this);
    }

    bool bFromRedis = false;

    if(master() && master()->getRedisDB() && master()->GetView())
    {
        QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (pView)
        {
            if(pView->GetMainFIR())
            {
                RunwayWidgetDataVPP rwfv(pView->GetMainFIR()->GetIDName());
                if(master()->getRedisDB()->loadRWFVPP(pView->GetMainFIR()->GetIDName(), rwfv))
                    mRWFVPP = rwfv;

                RunwayWidgetDataFIR rwf(pView->GetMainFIR()->GetIDName());

                switch(_type) {
                case RW1:
                {
                    if(master()->getRedisDB()->loadRWF1(pView->GetMainFIR()->GetIDName(), rwf))
                    {
                        if(!rwf.currentRunway().isEmpty())
                            bFromRedis = true;
                        mRWF = rwf;
                        if(!mRWF.runwaysList().size())
                        {
                            loadRunwaysFromConfig();
                        }

                    }
                }
                break;
                case RW2:
                {
                    if(master()->getRedisDB()->loadRWF2(pView->GetMainFIR()->GetIDName(), rwf))
                    {
                        if(!rwf.currentRunway().isEmpty())
                            bFromRedis = true;
                        mRWF = rwf;
                        if(!mRWF.runwaysList().size())
                        {
                            loadRunwaysFromConfig();
                        }
                    }
                }
                break;
                default:
                    break;
              }
            }
        }
    }

    ui->comboBoxTL->setCurrentIndex(5);
    connect(ui->comboBoxTL, SIGNAL(activated(int)), this, SLOT(OnChangeEchelon(int)));
    ui->comboBoxTL->setEnabled(false);

    ui->lineEditQNH->setValidator(new QRegExpValidator(QRegExp("\\d{0,4}"), this));
    ui->lineEditQNH->setText(QString::number(1013));
    connect(ui->lineEditQNH, SIGNAL(editingFinished()), this, SLOT(OnQNHEditingFinished()));

    //SetRunway(true);

    if(master() && master()->getRedisDB())
    {
        connect(master()->getRedisDB(), SIGNAL(changedRWF2(QString,RunwayWidgetDataFIR,QString)), this, SLOT(onChangedRWF2(QString,RunwayWidgetDataFIR,QString)), Qt::QueuedConnection);
        connect(master()->getRedisDB(), SIGNAL(changedRWF1(QString,RunwayWidgetDataFIR,QString)), this, SLOT(onChangedRWF1(QString,RunwayWidgetDataFIR,QString)), Qt::QueuedConnection);
        connect(master()->getRedisDB(), SIGNAL(changedRWFVPP(QString,RunwayWidgetDataVPP,QString)), this, SLOT(onChangedRWFVPP(QString,RunwayWidgetDataVPP,QString)), Qt::QueuedConnection);
    }

    connect(ui->comboBoxRunway, SIGNAL(activated(int)), this, SLOT(OnChangeRunway(int)));
    connect(ui->buttonRunwayState, SIGNAL(clicked()), this, SLOT(OnChangeRunwayState()));
    connect(ui->buttonILS, SIGNAL(clicked()), this, SLOT(OnChangeILS()));
    connect(ui->buttonNDB, SIGNAL(clicked()), this, SLOT(OnChangeNDB()));
    connect(ui->lineEditATIS, SIGNAL(editingFinished()), this, SLOT(OnChangeATIS()));
    connect(ui->lineEditATIS, SIGNAL(textEdited(const QString&)), this, SLOT(toUpper(const QString&)));
    connect(ui->chUseMeteo, SIGNAL(clicked(bool)),this,  SLOT(OnClickedUseMeteo(bool)));
    connect(ui->chUseMeteo, SIGNAL(toggled(bool)),this,  SLOT(OnToggledUseMeteo(bool)));
    connect(ui->but1, SIGNAL(clicked(bool)), this, SLOT(OnClickedBut1(bool)));
    connect(ui->but2, SIGNAL(clicked(bool)), this, SLOT(OnClickedBut2(bool)));
    connect(ui->but3, SIGNAL(clicked(bool)), this, SLOT(OnClickedBut3(bool)));
    connect(ui->but4, SIGNAL(clicked(bool)), this, SLOT(OnClickedBut4(bool)));
    connect(ui->but1, SIGNAL(toggled(bool)), this, SLOT(OnToggledBut1(bool)));
    connect(ui->but2, SIGNAL(toggled(bool)), this, SLOT(OnToggledBut2(bool)));
    connect(ui->but3, SIGNAL(toggled(bool)), this, SLOT(OnToggledBut3(bool)));
    connect(ui->but4, SIGNAL(toggled(bool)), this, SLOT(OnToggledBut4(bool)));

    timer = new QTimer(this);

    color_notoperativ = UserProfile::CSettings::Instance().GetColor(UserProfile::CtMeteo);

    lifetime = networkSettings()->value("meteo/metar_lifetime").toString();
    ourAirport = networkSettings()->value("meteo/airport").toString();

    Translate();
    SetRunway(bFromRedis);

    ui->buttonRunwayState->setVisible(false);
    ui->labelFree->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    UpdateXFont();
    SI();
}

RunwayWidget::~RunwayWidget()
{
    delete ui;
}

void RunwayWidget::Translate()
{
    ui->retranslateUi(this);
    OnChangeRunwayState(true);
}

void RunwayWidget::UpdateXFont()
{
    OnChangeRunwayState(true);

    QFontMetrics metrics(ui->labelFree->font());
    int w = qMax(metrics.width(tr("RUNWAY FREE")), metrics.width(tr("RUNWAY OCCUPIED"))) + metrics.width("  ") + 16;
    ui->labelFree->setMinimumWidth(w);
}

void RunwayWidget::SI()
{
    if (ui->comboBoxTL && ui->comboBoxTL->count())
    {
        bool bFeet = IsSISet(AppSettings::SIFeet);
        //ui->comboBoxTL->setItemText(0, index ? "30" : "090");
        ui->comboBoxTL->setItemText(0, bFeet ? "30" : "90");
        ui->comboBoxTL->setItemText(1, bFeet ? "40" : "120");
        ui->comboBoxTL->setItemText(2, bFeet ? "50" : "150");
        ui->comboBoxTL->setItemText(3, bFeet ? "60" : "180");
        ui->comboBoxTL->setItemText(4, bFeet ? "70" : "210");
        ui->comboBoxTL->setItemText(5, bFeet ? "80" : "240");
        ui->comboBoxTL->setItemText(6, bFeet ? "90" : "270");
        ui->comboBoxTL->setItemText(7, bFeet ? "100" : "300");
        ui->comboBoxTL->setItemText(8, bFeet ? "110" : "330");
        ui->comboBoxTL->setItemText(9, bFeet ? "120" : "360");
        for(int i = 10; i < ui->comboBoxTL->count(); ++i)
        {
            int ech = ui->comboBoxTL->itemText(i).toInt();
            QString sText = bFeet ? QString("%1").arg(qRound(ech/3.048)/*, 2, 10, QLatin1Char('0')*/)
                                  : QString("%1").arg(qRound(ech*3.048)/*, 3, 10, QLatin1Char('0')*/);
            ui->comboBoxTL->setItemText(i, sText);
        }
        SetEchelonString();
    }
}

void RunwayWidget::OnChangePress(int iValue, bool bFromRedis/* = false*/)
{
    //if(mRWF.isValidRunway())
    //{
        if(!bFromRedis && mRWFVPP.QNH() != iValue) {
            mRWFVPP.setQNH(iValue);
            logOperation(tr("QNH changed to '%1'").arg(iValue));
        }

        ui->lineEditQNH->blockSignals(true);
        ui->lineEditQNH->setText(QString::number(iValue));
        ui->lineEditQNH->blockSignals(false);
    //}
    if(ui->comboBoxTL)
    {
        int iTL = 80;
        if(iValue < 977)
            iTL = 90;
        else
            iTL = 80;

        if(mRWF.isValidRunway())
        {
            if(mRWFVPP.TL() != iTL)
                SaveRWFVPP();
            mRWFVPP.setTL(iTL);
        }
        int indexEchelon = GetEchelonComboIndex(iTL);
        if(indexEchelon != -1)
        {
            if(ui->comboBoxTL->currentIndex() != indexEchelon)
            {
                ui->comboBoxTL->blockSignals(true);
                ui->comboBoxTL->setCurrentIndex(indexEchelon);
                ui->comboBoxTL->blockSignals(false);
                OnChangeEchelon(indexEchelon, true);
            }
        }
    }
    SetEchelonString();

    if(!bFromRedis && mRWF.isValidRunway())
        SaveRWFVPP();
}

void RunwayWidget::OnChangeEchelon(int index, bool bFromRedis/* = false*/)
{
    Q_UNUSED(index);
    if(!bFromRedis && mRWF.isValidRunway())
    {
        mRWFVPP.setTL(GetEchelonValue(IsSISet(AppSettings::SIFeet)));
    }
    SetEchelonString();

    if(!bFromRedis && mRWF.isValidRunway())
        SaveRWFVPP();
}

int RunwayWidget::GetEchelonValue(bool bFeet) const
{
    int iVal = 0;
    if(ui->comboBoxTL)
    {
        int ind = ui->comboBoxTL->currentIndex();
        if(ind != -1)
        {
            iVal = ui->comboBoxTL->itemData(ind).toInt();
            if(!iVal)
            {
                iVal = ui->comboBoxTL->currentText().toInt();
                if(!bFeet)
                    iVal = qRound(iVal/3.048);
            }
        }
    }
    return iVal;
}

void RunwayWidget::SetEchelonString()
{

}

int RunwayWidget::GetEchelonComboIndex(int FL) const
{
    int iRes = -1;
    if (ui->comboBoxTL)
    {
        for (int i = 0; i < ui->comboBoxTL->count(); ++i)
        {
            if (FL == ui->comboBoxTL->itemData(i).toInt())
            {
                iRes = i;
                break;
            }
        }
    }
    return iRes;
}


void RunwayWidget::SetButtonState(QToolButton* but, int state)
{
    if(state)
    {
        but->setStyleSheet(QString("background-color: red; "
                                                     "color: yellow; "));
//                                                     "border: 1px; "
//                                                     "border-color: black; "
//                                                     "border-style: outset;"));
    }
    else
    {
        //but->setStyleSheet("");
        but->setStyleSheet(QString("background-color: lightgrey; "
                                                     "color: black; "));
    }
}

void RunwayWidget::SaveRWF()
{
    if(!mInvokeSave)
        QMetaObject::invokeMethod(this, "onSaveRWF", Qt::QueuedConnection);
    mInvokeSave = true;
}

void RunwayWidget::onSaveRWF()
{
    if(master() && master()->GetView())
    {
        QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (pView)
        {
            if(pView->GetMainFIR() && (pView->GetMainFIR()->GetIDName() == mRWF.FIR()))
            {
                if(master()->getRedisDB())   
                    _type == RW1 ? master()->getRedisDB()->saveRWF1(mRWF.FIR(), mRWF) : master()->getRedisDB()->saveRWF2(mRWF.FIR(), mRWF);
            }
        }
    }
    mInvokeSave = false;
}

void RunwayWidget::SaveRWFVPP()
{
    if(!mInvokeSave2)
        QMetaObject::invokeMethod(this, "onSaveRWFVPP", Qt::QueuedConnection);
    mInvokeSave2 = true;

}

void RunwayWidget::onSaveRWFVPP()
{
    if(master() && master()->GetView())
    {
        QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (pView)
        {
            if(pView->GetMainFIR() && (pView->GetMainFIR()->GetIDName() == mRWFVPP.FIR()))
            {
                if(master()->getRedisDB())
                {
                    master()->getRedisDB()->saveRWFVPP(mRWFVPP.FIR(), mRWFVPP);
                }
            }
        }
    }
    mInvokeSave2 = false;
}

void RunwayWidget::OnChangeRunway(int index, bool bFromRedis/* = false*/)
{
    if(!bFromRedis)
    {
        if(index != -1 && index < ui->comboBoxRunway->count())
        {
            QString sRunway = ui->comboBoxRunway->itemText(index);
            mRWF.setCurrentRunway(sRunway);
        }
    }

    OnChangeRunwayState(true);
    OnChangeILS(true);
    OnChangeNDB(true);

    bool ch = mRWFVPP.UseMeteo();

    if(ui->chUseMeteo->isChecked() != ch)
    {
        ui->chUseMeteo->setChecked(ch);

    }

    ch = mRWF.Serv1();
    ui->but1->setChecked(ch);
    ch = mRWF.Vehicle();
    ui->but2->setChecked(ch);
    ch = mRWF.Other();
    ui->but3->setChecked(ch);
    ch = mRWF.Serv2();
    ui->but4->setChecked(ch);

//    if(mRWF.isValidRunway())
//    {
//        int indexEchelon = GetEchelonComboIndex(mRWF.TL());
//        if(indexEchelon != -1)
//        {
//            if(ui->comboBoxTL->currentIndex() != indexEchelon)
//            {
//                ui->comboBoxTL->blockSignals(true);
//                ui->comboBoxTL->setCurrentIndex(indexEchelon);
//                ui->comboBoxTL->blockSignals(false);
//                OnChangeEchelon(indexEchelon, true);
//            }
//        }
//    }

    if(mRWF.isValidRunway())
    {
        if(ui->lineEditQNH->text().toInt() != mRWFVPP.QNH())
        {
            ui->lineEditQNH->blockSignals(true);
            ui->lineEditQNH->setText(QString::number(mRWFVPP.QNH()));
            ui->lineEditQNH->blockSignals(false);
            OnChangePress(mRWFVPP.QNH(), true);
        }
    }

    if(mRWF.isValidRunway())
    {
        ui->lineEditATIS->blockSignals(true);
        ui->lineEditATIS->setText(mRWFVPP.ATIS());
        ui->lineEditATIS->blockSignals(false);
    }

    if(!bFromRedis)
        SaveRWF();

    emit UpdateMeteoToMaster();

}

void RunwayWidget::OnChangeRunwayState(bool bFromRedis)
{
  /*  if(mRWF.isValidRunway())
    {
        int state(mRWF.runway(mRWF.currentRunway()).state());
        if(!bFromRedis)
        {
            if(state)
                state = 0;
            else
                state = 1;
            mRWF.runway(mRWF.currentRunway()).setState(state);
            // for UUMV podpora, must find reverse course
            RunwayWidgetDataFIR::TRunways& rws = mRWF.runwaysList();
            for(RunwayWidgetDataFIR::TRunways::iterator rw = rws.begin(); rw != rws.end(); ++rw)
            {
                rw->setState(state);
            }
            /////////////////////////////////////////////
            logOperation(tr("Runway '%1' state changed to '%2'")
                         .arg(mRWF.currentRunway())
                         .arg(GetButtonStateText(state)));
        }
        ui->buttonRunwayState->setText(GetButtonStateText(state));
        SetButtonState(ui->buttonRunwayState, state);

        if(!bFromRedis)
            SaveRWF();

        if (player)
        {
            if (state)
                player->play();
            else
                player->stop();
        }
    }
    else
    {
        ui->buttonRunwayState->setText(GetButtonStateText(0));
        SetButtonState(ui->buttonRunwayState, 0);
    }*/
}

void RunwayWidget::OnChangeILS(bool bFromRedis)
{
    if(mRWF.isValidRunway())
    {
        int state(mRWF.runway(mRWF.currentRunway()).ILS());
        if(!bFromRedis)
        {
            if(state)
                state = 0;
            else
                state = 1;
            mRWF.runway(mRWF.currentRunway()).setILS(state);

            logOperation(tr("Runway '%1' ILS changed to '%2'")
                         .arg(mRWF.currentRunway())
                         .arg(state));
        }
        SetButtonState(ui->buttonILS, state);

        if(!bFromRedis)
            SaveRWF();
    }
    else
        SetButtonState(ui->buttonILS, 0);
}

void RunwayWidget::OnChangeNDB(bool bFromRedis)
{
    if(mRWF.isValidRunway())
    {
        int state(mRWF.runway(mRWF.currentRunway()).NDB());
        if(!bFromRedis)
        {
            if(state)
                state = 0;
            else
                state = 1;
            mRWF.runway(mRWF.currentRunway()).setNDB(state);

            logOperation(tr("Runway '%1' NDB changed to '%2'")
                         .arg(mRWF.currentRunway())
                         .arg(state));
        }
        SetButtonState(ui->buttonNDB, state);

        if(!bFromRedis)
            SaveRWF();
    }
    else
        SetButtonState(ui->buttonNDB, 0);
}

void RunwayWidget::OnChangeATIS(bool bFromRedis)
{
    if(mRWF.isValidRunway())
    {
        if(!bFromRedis)
            mRWFVPP.setATIS(ui->lineEditATIS->text());

        ui->lineEditATIS->blockSignals(true);
        ui->lineEditATIS->setText(mRWFVPP.ATIS());
        ui->lineEditATIS->blockSignals(false);

        if(!bFromRedis)
            SaveRWFVPP();
   }
}

void RunwayWidget::OnQNHEditingFinished()
{
    OnChangePress(ui->lineEditQNH->text().toInt());
}

void RunwayWidget::onChangedRWF1(QString idFIR, RunwayWidgetDataFIR rwf, QString operType)
{
    if(_type == RW1)
        onChangedRWF(idFIR, rwf, operType);
}

void RunwayWidget::onChangedRWF2(QString idFIR, RunwayWidgetDataFIR rwf, QString operType)
{
    if(_type == RW2)
        onChangedRWF(idFIR, rwf, operType);
}

void RunwayWidget::onChangedRWF(QString idFIR, RunwayWidgetDataFIR rwf, QString operType)
{
    Q_UNUSED(operType);
    if(master() && master()->GetView())
    {
        QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (pView)
        {
            if(pView->GetMainFIR())
            {
                if (pView->GetMainFIR()->GetIDName() == idFIR)
                {
                    mRWF = rwf;
                    if(!mRWF.runwaysList().size())
                    {
                        loadRunwaysFromConfig();
                    }

                    SetRunway(true);
                }
            }
        }
    }
}

void RunwayWidget::onChangedRWFVPP(QString idFIR, RunwayWidgetDataVPP rwf, QString operType)
{
    Q_UNUSED(operType);
    if(master() && master()->GetView())
    {
        QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (pView)
        {
            if(pView->GetMainFIR())
            {
                if (pView->GetMainFIR()->GetIDName() == idFIR)
                {
                    mRWFVPP = rwf;
                    SetRunway(true);
                }
            }
        }
    }
}

void RunwayWidget::SetRunway(bool bFromRedis/* = false*/)
{
    ui->comboBoxRunway->blockSignals(true);
    ui->comboBoxRunway->clear();
    if(mRWF.isValidRunway())
    {
        ui->comboBoxRunway->addItems(mRWF.runwaysList().keys());
        int ind = ui->comboBoxRunway->findText(mRWF.currentRunway());
        ui->comboBoxRunway->setCurrentIndex(ind);
         OnChangeRunway(ind, bFromRedis);
    }
    ui->comboBoxRunway->blockSignals(false);

    if(_type == RW1)
        mCurrentRunway1 = ui->comboBoxRunway->currentText();
    if(_type == RW2)
        mCurrentRunway2 = ui->comboBoxRunway->currentText();

    CGUIView *pView = dynamic_cast<CGUIView*>(master()->GetView().data());
    if (pView)
        pView->SetLayouts();
}

void RunwayWidget::toUpper(const QString &text)
{
    ui->lineEditATIS->setText(text.toUpper());
}

QString RunwayWidget::GetButtonStateText(int state) const
{
    QString s0 = tr("RUNWAY FREE");
    QString s1 = tr("RUNWAY OCCUPIED");
    QFont font = ui->buttonRunwayState->font();
    QFontMetrics metrics(font);
    int w0 = metrics.width(s0);
    int w1 = metrics.width(s1);
    int w = w0;
    if(w1 > w0)
        w = w1;
    w += metrics.width("  ") + 16;
  //  int minW = ui->buttonRunwayState->minimumWidth();
    int minW = ui->labelFree->minimumWidth();

    if(w != minW)
        ui->buttonRunwayState->setMinimumWidth(w);
        ui->labelFree->setMinimumWidth(w);
    if(state)
        return s1;
    return s0;
}

void RunwayWidget::OnClickedUseMeteo(bool checked)
{
    mRWFVPP.setUseMeteo(checked);
    SaveRWFVPP();
}

void RunwayWidget::OnToggledUseMeteo(bool checked)
{
    if(checked)
    {
        UpdateQNH();
        ui->lineEditQNH->setEnabled(false);
        connect(timer, SIGNAL(timeout()), this, SLOT(_OnTimer()));
        timer->start(60000); // 1 min
    }
    else
    {
        ui->lineEditQNH->setEnabled(true);
        ui->lineEditQNH->setStyleSheet("color: black;");
        disconnect(timer, SIGNAL(timeout()), this, SLOT(_OnTimer()));
    }

}

void RunwayWidget::_OnTimer()
{
    UpdateQNH();
}
void RunwayWidget::UpdateQNH()
{
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
    //if(!CheckDataBase())
        return;

    QSqlQuery queryAirport;
    queryAirport.exec("SELECT id, name FROM airport WHERE name = '" + ourAirport +"'");
    queryAirport.next();
    int idAirport = queryAirport.value(0).toInt();

    QSqlQuery query;
    bool error = query.exec("SELECT qnh FROM meteo_metar WHERE airport_id = " + QString::number(idAirport) +
                              " AND datetime_metar <= NOW() AND datetime_metar+ interval '" + lifetime + "' >= NOW()  ORDER BY datetime_metar DESC");
    if(!error)
        return;
    ui->lineEditQNH->setStyleSheet("color: black;");
    if(query.size() <= 0)
    {
        query.exec("SELECT qnh FROM meteo_metar WHERE airport_id = " + QString::number(idAirport) +
                                "ORDER BY datetime_metar DESC");
        ui->lineEditQNH->setStyleSheet("color:" + color_notoperativ.name());
    }
    query.next();
    int qnh = query.value(0).toInt();
    ui->lineEditQNH->blockSignals(true);
    ui->lineEditQNH->setText(QString::number(qnh));
    ui->lineEditQNH->blockSignals(false);
    OnChangePress(ui->lineEditQNH->text().toInt());
    return;

}

bool RunwayWidget::CheckDataBase()
{
    QSqlDatabase database = QSqlDatabase::database();

    int exitCode = QProcess::execute("ping", QStringList() << "-c" << "1" << "-W" << "1" << database.hostName());
    if (exitCode!=0 || !database.isOpen())
    {
        ui->lineEditQNH->clear();
        ui->chUseMeteo->setEnabled(false);
        return false;
    }
    ui->chUseMeteo->setEnabled(true);
    return true;
}

void RunwayWidget::loadRunwaysFromConfig()
{
    RunwayWidgetData rdata;

    if(_type == RW1) {
        rdata.setName(networkSettings()->value("RW1/runway1", "13L").toString());
        mRWF.addRunway(rdata);
        rdata.setName(networkSettings()->value("RW1/runway2", "31L").toString());
        mRWF.addRunway(rdata);
    }

    if(_type == RW2) {
        rdata.setName(networkSettings()->value("RW2/runway1", "13R").toString());
        mRWF.addRunway(rdata);
        rdata.setName(networkSettings()->value("RW2/runway2", "31R").toString());
        mRWF.addRunway(rdata);
    }

    mRWF.setCurrentRunway(rdata.name());
}

void RunwayWidget::OnClickedBut1(bool checked)
{
    mRWF.setServ1(checked);
    SaveRWF();
    OnToggledBut1(checked);
}
void RunwayWidget::OnClickedBut2(bool checked)
{
    mRWF.setVehicle(checked);
    SaveRWF();
    OnToggledBut2(checked);
}
void RunwayWidget::OnClickedBut3(bool checked)
{
   mRWF.setOther(checked);
   SaveRWF();
   OnToggledBut3(checked);
}
void RunwayWidget::OnClickedBut4(bool checked)
{
    mRWF.setServ2(checked);
    SaveRWF();
    OnToggledBut4(checked);
}
void RunwayWidget::OnToggledBut1(bool checked)
{
    if(checked){
        setBorder(false);
  //      ui->but1->setStyleSheet("border: 1px solid gray; border-radius: 4px; padding: 2px; color:" + color_notoperativ.name());
        ui->but1->setStyleSheet(QString("background-color: red; " "color: yellow"));

    }
    else {
      //  ui->but1->setStyleSheet("border: 1px solid gray; border-radius: 4px; padding: 2px; color: black");
         ui->but1->setStyleSheet( QString("background-color: lightgrey;  border-color: gray; "
                                          "color: black; "));

        (ui->but2->isChecked() || ui->but3->isChecked() || ui->but4->isChecked()) ? setBorder(false) : setBorder(true);
    }
}
void RunwayWidget::OnToggledBut2(bool checked)
{
    if(checked){
        setBorder(false);
        ui->but2->setStyleSheet(QString("background-color: red; " "color: yellow"));
    }
    else {
        ui->but2->setStyleSheet( QString("background-color: lightgrey; border-color: gray; "
                                         "color: black; "));
        (ui->but1->isChecked() || ui->but3->isChecked() || ui->but4->isChecked()) ? setBorder(false) : setBorder(true);
    }
}

void RunwayWidget::OnToggledBut3(bool checked)
{
    if(checked){
        setBorder(false);
        ui->but3->setStyleSheet(QString("background-color: red; " "color: yellow"));

    }
    else {
        ui->but3->setStyleSheet( QString("background-color: lightgrey; border-color: gray; "
                                         "color: black; "));
        (ui->but1->isChecked() || ui->but2->isChecked() || ui->but4->isChecked()) ? setBorder(false) : setBorder(true);
    }
}

void RunwayWidget::OnToggledBut4(bool checked)
{
    if(checked){
        setBorder(false);
        ui->but4->setStyleSheet(QString("background-color: red; " "color: yellow"));
    }
    else {
        ui->but4->setStyleSheet( QString("background-color: lightgrey; border-color: gray; "
                                         "color: black; "));

        (ui->but1->isChecked() || ui->but2->isChecked() || ui->but3->isChecked()) ? setBorder(false) : setBorder(true);
    }
}
void RunwayWidget::setBorder(bool free)
{
    if(free){
        ui->labelFree->setText(tr("RUNWAY FREE"));
        player->stop();
        ui->frameFree->setStyleSheet(" border: 1px solid black; border-radius: 3px; padding: 2px");
        ui->labelFree->setStyleSheet("border: 0px; color: black");
    }
    else {
        ui->labelFree->setText(tr("RUNWAY OCCUPIED"));
        player->play();
        ui->frameFree->setStyleSheet(" border: 1px solid red; border-radius: 3px; padding: 2px");
        ui->labelFree->setStyleSheet("border: 0px; color:" + color_notoperativ.name());

        ui->but1->setStyleSheet(ui->but1->isChecked() ? "background-color: red; border-color: gray; color:yellow" :
                                                        "background-color: lightgrey; border-color: gray; color: black");
        ui->but2->setStyleSheet(ui->but2->isChecked() ? "background-color: red; border-color: gray; color:yellow":
                                                        "background-color: lightgrey; border-color: gray; color: black");
        ui->but3->setStyleSheet(ui->but3->isChecked() ? "background-color: red; border-color: gray; color:yellow":
                                                        "background-color: lightgrey; border-color: gray; color: black");
        ui->but4->setStyleSheet(ui->but4->isChecked() ? "background-color: red; border-color: gray; color:yellow":
                                                        "background-color: lightgrey; border-color: gray; color: black");

    }
}

