#include "planeditor.h"
#include "ui_planeditor.h"
//#include "fpl.h"
#include "main.h"

#include "uppercasevalidator.h"
#include "Master.h"

#include "plvalidator.h"

#include <QPushButton>
#include <QMessageBox>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include <QStringListModel>
#include <QKeyEvent>

#include "actwidget.h"
#include "acttablemodel.h"
#include "lineeditdelegate.h"
#include "timedelegate.h"
#include "flightleveldelegate.h"
#include "comboboxdelegate.h"
#include "aftnmessageprocessor.h"


PlanEditor::PlanEditor(int cur_index_row, QUuid _uid, QWidget* pParent) :
    AbstractDialog(pParent), m_act_widget(0), act_in_model_(0),
    canOtherInfoEmpty(false)

{
    act_in_model_ = new ActTableModel(ActTableModel::act_in, 0, this,getFir2Points());
    QSharedPointer<QValidator> cop_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,5}"), 0));
    cop_delegate = new LineEditDelegate(cop_validator, this);
    setupUi(this);
    //init();
    setWindowTitle(tr("FPL"));
    validValue = "";
    invalidValue = "background-color: #FF637F;";
    current_row = cur_index_row;
    uppercaseValidator = new UppercaseValidator(this);
    ACIDEdit->setValidator(uppercaseValidator);
    Equipment2Edit->setValidator(uppercaseValidator);
    Equipment3Edit->setValidator(uppercaseValidator);
    AerodromeDepEdit->setValidator(uppercaseValidator);
    SpeedEdit->setValidator(uppercaseValidator);
    RFLEdit->setValidator(uppercaseValidator);
    AerodromeDestEdit->setValidator(uppercaseValidator);
    ALTNEdit->setValidator(uppercaseValidator);
    ALTN2Edit->setValidator(uppercaseValidator);

    //FlightTypeCombo_text = RulesCombo_text =
    //        AirplaneCombo_text = TurbulenceCombo_text = Equipment1Combo_text = "";
    //MesTypeCombo_text = "FPL";
    //MesTypeCombo->setCurrentIndex(0);

    MesTypeCombo->addItem("FPL");
    MesTypeCombo->setCurrentIndex(0);
    MesTypeCombo->lineEdit()->setReadOnly(true);

    FlightTypeCombo->lineEdit()->setText("");
    RulesCombo->lineEdit()->setText("");
    AirplaneCombo->lineEdit()->setText("");
    TurbulenceCombo->lineEdit()->setText("");
    //Equipment1Combo->lineEdit()->setText("");
    MesTypeCombo->lineEdit()->setReadOnly(true);
    FlightTypeCombo->lineEdit()->setReadOnly(true);
    RulesCombo->lineEdit()->setReadOnly(true);
    AirplaneCombo->lineEdit()->setReadOnly(true);
    TurbulenceCombo->lineEdit()->setReadOnly(true);
    //Equipment1Combo->lineEdit()->setReadOnly(true);
    AerodromeDepEdit->setReadOnly(true);
    connect(MesTypeCombo, SIGNAL(editTextChanged(QString)), SLOT(onComboBoxActivated(QString)));
    connect(FlightTypeCombo, SIGNAL(editTextChanged(QString)), SLOT(onComboBoxActivated(QString)));
    connect(RulesCombo, SIGNAL(editTextChanged(QString)), SLOT(onComboBoxActivated(QString)));
    connect(AirplaneCombo, SIGNAL(editTextChanged(QString)), SLOT(onComboBoxActivated(QString)));
    connect(TurbulenceCombo, SIGNAL(editTextChanged(QString)), SLOT(onComboBoxActivated(QString)));
    //connect(Equipment1Combo, SIGNAL(editTextChanged(QString)), SLOT(onComboBoxActivated(QString)));
    /*label_11->hide();
    label_10->hide();
    label_12->hide();
    lineEdit_6->hide();
    lineEdit_5->hide();
    lineEdit_7->hide();*/
    //Equipment2Edit->hide();
    //label_14->hide();
    //Equipment3Edit->hide();

    OtherEdit->setReadOnly(true);
    NumberEdit->setReadOnly(true);
    TimeDepEdit->setReadOnly(true);
    CopouttimeEdit->setReadOnly(true);
    SpeedEdit->setReadOnly(true);
    CODEEdit->setMaxLength(4);
    AirplaneCombo->lineEdit()->setMaxLength(4);
    SpeedEdit->setMaxLength(5);
    RFLEdit->setMaxLength(5);

    uid = _uid;
    //QString str;
    //restoreState(staticMetaObject.className());

    //QMetaObject::invokeMethod(this, "OnFixedSize", Qt::QueuedConnection);
    pushButton_6->hide();
    pushButton_7->hide();
    pushButton_11->hide();
    pushButton_10->hide();
    enterButton->hide();
    firstButton->hide();
    lastButton->hide();
    prevButton->hide();
    nextButton->hide();


    connect(enterButton, SIGNAL(clicked()), this, SLOT(doSavePlan()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(onClose()));
    connect(firstButton, SIGNAL(clicked()), this, SLOT(onPushButtonFirstClicked()));
    connect(lastButton, SIGNAL(clicked()), this, SLOT(onPushButtonLastClicked()));
    connect(prevButton, SIGNAL(clicked()), this, SLOT(onPushButtonPrevClicked()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(onPushButtonNextClicked()));
    //connect(pushButton_11, SIGNAL(clicked()), this, SLOT(onPushButtonActClicked()));
    connect(pushButton_10, SIGNAL(clicked()), this, SLOT(onPushButtonDeleteClicked()));
    connect(ACIDEdit, SIGNAL(editingFinished()), this, SLOT(on_ACIDEdit_editingFinished()));
    connect(CODEEdit, SIGNAL(editingFinished()), this, SLOT(on_CODEEdit_editingFinished()));
    connect(Equipment2Edit, SIGNAL(editingFinished()), this, SLOT(on_Equipment2Edit_editingFinished()));
    connect(Equipment3Edit, SIGNAL(editingFinished()), this, SLOT(on_Equipment3Edit_editingFinished()));
    connect(AerodromeDepEdit, SIGNAL(editingFinished()), this, SLOT(on_AerodromeDepEdit_editingFinished()));
    connect(SpeedEdit, SIGNAL(editingFinished()), this, SLOT(on_SpeedEdit_editingFinished()));
    connect(RFLEdit, SIGNAL(editingFinished()), this, SLOT(on_RFLEdit_editingFinished()));
    connect(AerodromeDestEdit, SIGNAL(editingFinished()), this, SLOT(on_AerodromeDestEdit_editingFinished()));
    connect(ALTNEdit, SIGNAL(editingFinished()), this, SLOT(on_ALTNEdit_editingFinished()));
    connect(ALTN2Edit, SIGNAL(editingFinished()), this, SLOT(on_ALTN2Edit_editingFinished()));
    connect(OtherEdit, SIGNAL(editingFinished()), this, SLOT(on_OtherEdit_editingFinished()));

    InitSize();
    setVis(false);
}

PlanEditor::PlanEditor(int id, QStringList _lst, QWidget* pParent) :
    AbstractDialog(pParent),m_act_widget(0),act_in_model_(0),
    canOtherInfoEmpty(false)

{
    lst_orcamrange = _lst;
    act_in_model_ = new ActTableModel(ActTableModel::act_in, 0, this,getFir2Points());
    setupUi(this);
    setWindowTitle(tr("FPL"));
    //SetId(id);

    MesTypeCombo->addItem("FPL");
    MesTypeCombo->setCurrentIndex(0);
    MesTypeCombo->lineEdit()->setReadOnly(true);

    //MesTypeCombo->lineEdit()->setReadOnly(true);
    ACIDEdit->setReadOnly(true);
    RulesCombo->lineEdit()->setReadOnly(true);

    FlightTypeCombo->lineEdit()->setReadOnly(true);

    NumberEdit->setReadOnly(true);
    AirplaneCombo->lineEdit()->setReadOnly(true);
    TurbulenceCombo->lineEdit()->setReadOnly(true);
    //Equipment1Combo->lineEdit()->setReadOnly(true);
    Equipment2Edit->setReadOnly(true);
    Equipment3Edit->setReadOnly(true);

    AerodromeDepEdit->setReadOnly(true);
    TimeDepEdit->setReadOnly(true);

    SpeedEdit->setReadOnly(true);
    RFLEdit->setReadOnly(true);

    RouteEdit->setReadOnly(true);

    AerodromeDestEdit->setReadOnly(true);
    CopouttimeEdit->setReadOnly(true);
    ALTNEdit->setReadOnly(true);
    ALTN2Edit->setReadOnly(true);

    OtherEdit->setReadOnly(true);

    lineEdit_2->setReadOnly(true);
    lineEdit_4->setReadOnly(true);
    lineEdit_6->setReadOnly(true);

    lineEdit_3->setReadOnly(true);
    lineEdit_5->setReadOnly(true);
    lineEdit_7->setReadOnly(true);

    pushButton_10->hide();
    enterButton->hide();
    pushButton_6->hide();
    pushButton_7->hide();
    firstButton->hide();
    lastButton->hide();
    prevButton->hide();
    nextButton->hide();

    connect(closeButton, SIGNAL(clicked()), this, SLOT(onClose()));
    connect(firstButton, SIGNAL(clicked()), this, SLOT(onPushButtonFirstClicked()));
    connect(lastButton, SIGNAL(clicked()), this, SLOT(onPushButtonLastClicked()));
    connect(prevButton, SIGNAL(clicked()), this, SLOT(onPushButtonPrevClicked()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(onPushButtonNextClicked()));
    connect(pushButton_11, SIGNAL(clicked()), this, SLOT(onPushButtonActClicked()));

    InitSize();
    setVis(false);

    //init();
    //FillFields();
    SetNewData(id);
}

PlanEditor::~PlanEditor()
{
    saveState(staticMetaObject.className());
    if(m_act_widget)
        m_act_widget->deleteLater();
}

void PlanEditor::onPushButtonDeleteClicked()
{
    bool ok;
    ok = master()->getRedisDB()->deleteSFPL(sfpl_);
    if(ok)
        close();
}

void PlanEditor::onPushButtonActClicked()
{
    /*if(!m_act_widget)
    {
        m_act_widget = new ActWidget(tr("ACT IN"), act_in_model_, this);
        m_act_widget->setWindowModality(Qt::ApplicationModal);
    }*/
    //setActInWidget();

    AdaptateRoute(mFpl.route());
    setActInWidget2();
    m_act_widget->show();
}

void PlanEditor::setActInWidget()
{
    bool ok;
    ok = master()->getRedisDB()->loadSFPL(sfpl_,uid);
    if (!ok)
       logOperation(tr("Fail load from Redis '%1':").arg(uid.toString()));
    else
    {
        QList<Act> act_ins;
        FLVal flval;
        if (sfpl_->GetCFL())
        {
            flval = sfpl_->GetCFL();
        }
        else if (sfpl_->GetCOPinFL())
        {
            flval = sfpl_->GetCOPinFL();
        }
        else
        {
            flval = FLVal(sfpl_->GetLevelS());
        }
        Act act = {sfpl_->GetEquipment(), sfpl_->GetACID(), sfpl_->GetCODEtoString(), sfpl_->GetCOPin(), sfpl_->GetETOin(),
            flval, sfpl_->GetDepAerodrome(), sfpl_->GetDestAerodrome(),
            sfpl_->GetCurSector(), sfpl_->GetNextSector()};
        act_ins.push_back(act);
        act_in_model_->setActs(act_ins);
        QSet<int> non_editable_fields;
        non_editable_fields.insert(1);
        act_in_model_->setNonEditableFields(non_editable_fields);
        act_in_model_->clearErrors();
        //QStringList eqList;
        // eqList << "w" << "y" << "wy" << "";
        //QStringListModel* eqModel = new QStringListModel(eqList, this);
        //ComboBoxDelegate* eq_delegate = new ComboBoxDelegate(this);
        //eq_delegate->setModel(eqModel);
        //eq_delegate->setHideKeyColumn(false);
        QSharedPointer<QValidator> equip_validator(new QRegExpValidator(QRegExp("\\w{0,5}"), 0));
        LineEditDelegate *equip_delegate = new LineEditDelegate(equip_validator, m_act_widget);
        m_act_widget->setItemDelegateForColumn(0, equip_delegate);

        QSharedPointer<QValidator> acid_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,7}"), 0));
        QAbstractItemDelegate *acid_delegate = new LineEditDelegate(acid_validator, m_act_widget);
        m_act_widget->setItemDelegateForColumn(1, acid_delegate);

        QSharedPointer<QValidator> code_validator(new QRegExpValidator(QRegExp("[0-7]{4}"), 0));
        QAbstractItemDelegate *code_delegate = new LineEditDelegate(code_validator, m_act_widget);
        m_act_widget->setItemDelegateForColumn(2, code_delegate);

        QSharedPointer<QValidator> cop_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,5}"), 0));
        LineEditDelegate *cop_delegate = new LineEditDelegate(cop_validator, m_act_widget);
        cop_delegate->setCompleter(getFir2Points());
        m_act_widget->setItemDelegateForColumn(3, cop_delegate);

        QAbstractItemDelegate *eto_delegate = new TimeDelegate(m_act_widget);
        m_act_widget->setItemDelegateForColumn(4, eto_delegate);

        QAbstractItemDelegate *pel_delegate = new FlightLevelDelegate(m_act_widget, this);
        m_act_widget->setItemDelegateForColumn(5, pel_delegate);

        QSharedPointer<QValidator> adep_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,4}"), 0));
        LineEditDelegate *adep_delegate = new LineEditDelegate(adep_validator, m_act_widget);
        m_act_widget->setItemDelegateForColumn(6, adep_delegate);

        QSharedPointer<QValidator> ades_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,4}"), 0));
        LineEditDelegate *ades_delegate = new LineEditDelegate(ades_validator, m_act_widget);
        m_act_widget->setItemDelegateForColumn(7, ades_delegate);

        m_act_widget->setWindowModality(Qt::ApplicationModal);
        connect(m_act_widget, SIGNAL(act()), this, SLOT(onAct()));
    }
}

void PlanEditor::setActInWidget2()
{
        QList<Act> act_ins;

        QString eq;
        if(mFpl.equipment().split("/").front().contains("W"))
            eq += "w";
        if(mFpl.equipment().split("/").front().contains("Y"))
            eq += "y";

        ActWidget *widget = m_act_widget;

        QString airpt = networkSettings()->value("meteo/airport").toString();
        if(!airpt.isEmpty() && mFpl.departureAerodrome() == airpt) {
            if (act_in_model_)
                act_in_model_->deleteLater();

            act_in_model_ = new ActTableModel(ActTableModel::act_dep, 0, this);
            m_act_widget = new ActWidget(tr("ACT DEP"), act_in_model_, this);
            QString code = FreeCodeOrcam();
            Act act = {eq, mFpl.aircarftId(), code ,mFpl.departureAerodrome(), mFpl.departureTime(), 0,mFpl.departureAerodrome(), mFpl.destinationAerodrome()};
            act_ins.push_back(act);
        }
        else {
            QString code;
            if (master()) {
                CView *view = dynamic_cast<CView*>(master()->GetView().data());
                if (view) {
                    QStringList codes;
                    foreach (const QSharedPointer<CAirplane> &airplane, view->GetAirplanes().values()) {
                        if (airplane->GetACID() == mFpl.aircarftId())
                            codes.append(airplane->GetMode3AReplyToString());
                    }

                    if (codes.size() == 1)
                        code = codes.first();
                }
            }

            //Разбиваем маршрут на точки,
            //а потом каждую точку проверяем на принадлежность к границе сектора
            AdaptateRoute(mFpl.route());
            CView *view = dynamic_cast<CView*>(master()->GetView().data());
            QSharedPointer<CFPLFrag> pResult(new CFPLFrag());
            QSharedPointer<CPoint> pPoint;
            QSharedPointer<CProcFrag> pRouteFrag(new CProcFrag());
            pRouteFrag->SetType(CProcFrag::enRoute);
            pRouteFrag->SetName(newRoute);

            QList<QSharedPointer<CRoute> > listRoute;
            QStringList routeParts = newRoute.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            QStringList::const_iterator itScan = routeParts.begin();
            for (; itScan != routeParts.end(); ++itScan)
            {
                QStringList tokens = itScan->split("/");
                QString pointName = tokens.front();
                QString speedLevel;
                if (tokens.size() > 1)
                    speedLevel = tokens.last();

                if (pointName != "VFR" && pointName != "IFR" && pointName != "C" && pointName != "DCT")
                {
                    QSharedPointer<CPoint> itPoint;
                    QList<QSharedPointer<CRoute> > listitRoute;
                    CView::TSceneList::const_iterator itFIR = view->GetScenes().constBegin();
                    for(; itFIR != view->GetScenes().constEnd(); ++itFIR)
                    {
                        Scene::TPointsMap::const_iterator itP = (*itFIR)->GetPoints().find(CPointKey(pointName));
                        if(itP != (*itFIR)->GetPoints().constEnd())
                            itPoint = *itP;
                        Scene::TRoutsMap::const_iterator itR = (*itFIR)->GetRoutes().find(pointName);
                        if(itR != (*itFIR)->GetRoutes().constEnd())
                        {
                            listitRoute.append(*itR);
                        }
                    }

                    if (itPoint)
                    {
                       /* if (itScan == routeParts.begin() && pSIDRoute && itPoint->GetType() == CPoint::enVHFNavAid)
                            continue;

                        if (itScan == (routeParts.end() - 1) && pSTARRoute && itPoint->GetType() == CPoint::enVHFNavAid)
                            continue;*/

                        // Point
                        if (!pPoint.isNull() && listRoute.size())
                        {
                            //CFPLFrag::FillPoints(listRoute, pPoint, itPoint, pRouteFrag);
                            listRoute.clear();
                        }
                        QSharedPointer<CPointFrag> pRoutePnt(new CPointFrag);
                        pRoutePnt->SetPoint(itPoint);
                        if (pRouteFrag->GetPointFragList().empty()
                            || pRouteFrag->GetPointFragList().back()->GetPoint()->GetName()
                            != itPoint->GetName())
                        {
                            /*if (pSFPL->GetCFL().GetM())
                            {
                                //pRoutePnt->SetLevel(pSFPL->GetCFL().GetM());
                            }
                            else
                            {
                                pRoutePnt->SetLevel(pSFPL->GetCOPinFL().GetM());
                            }*/

                            pRouteFrag->AddPointFrag(pRoutePnt);
                        }
                        pPoint = itPoint;
                    }
                    else if (listitRoute.size())
                    {
                        // Route
                        if (!pPoint.isNull() && !listRoute.size())
                            listRoute = listitRoute;
                        else    TVA_THROW(QObject::tr("Only SID or point should be before route\"%1\"")
                                          .arg(listitRoute.at(0)->GetName()), E_FAIL);
                    }
                    else
                    {
                        // Custom point OR VOR
                        try
                        {
                            CCoord Coord = CFPL::ParseCoordinate(pointName);
                            Coord.Center(view->GetCentralPoint()->GetCoord());
                            QSharedPointer<CPointFrag> pCustRoutePoint(new CPointFrag);
                            QSharedPointer<CPoint> pCustPoint(new CPoint);
                            pCustPoint->SetName(pointName);
                            pCustPoint->SetCoord(Coord);
                            pCustRoutePoint->SetPoint(pCustPoint);
                            pRouteFrag->AddPointFrag(pCustRoutePoint);
                            pPoint = pCustPoint;
                        }
                        catch(const Error& err)
                        {
                            LOG_STR(WARNING, QObject::tr("FragmentateFPL. Undefined string \"%1\". %2").arg(pointName).arg(err.reason()));
                        }
                    }
                }
            }

            TVA_CHECK(
                    !listRoute.size(),
                    QObject::tr("The last element \"%1\" should be STAR or point")
                    .arg(listRoute.at(0)->GetName()),
                    E_FAIL);

            if (!pRouteFrag.isNull())
                pResult->AddProcFrag(pRouteFrag);

            pResult->SetNextPoint(0);

            QString cop = "";
            if (view && view->GetMainFIR())
            {
                bool sign = true;
                foreach(CProcFrag::TPointFragPtr p, pResult->GetAllPoint())
                {
                    Scene::TPointsMap::const_iterator itPoint = view->GetMainFIR()->GetPoints().begin();
                    for (; itPoint != view->GetMainFIR()->GetPoints().end(); ++itPoint)
                    {
                        if((*itPoint)->GetName() == p->GetPoint()->GetName())
                        {
                            if((*itPoint)->GetSectorBorder() == true)
                            {
                                cop = (*itPoint)->GetName();
                                sign = false;
                                break;
                            }
                        }
                    }
                    if(!sign)
                        break;
                }
            }

            m_act_widget = new ActWidget(tr("ACT IN"), act_in_model_, this);
            Act act = {eq, mFpl.aircarftId(), code ,cop ,mFpl.departureTime(), NULL, mFpl.departureAerodrome(), mFpl.destinationAerodrome(), NULL, NULL};
            act_ins.push_back(act);
        }
        act_in_model_->setActs(act_ins);
        QSet<int> non_editable_fields;
        non_editable_fields.insert(1);
        act_in_model_->setNonEditableFields(non_editable_fields);
        act_in_model_->clearErrors();

        QStringList eqList;
        eqList << "w" << "y" << "wy" << "";
        QStringListModel* eqModel = new QStringListModel(eqList, this);
        ComboBoxDelegate* eq_delegate = new ComboBoxDelegate(this);
        eq_delegate->setModel(eqModel);
        eq_delegate->setHideKeyColumn(false);
        m_act_widget->setItemDelegateForColumn(0, eq_delegate);

        //QSharedPointer<QValidator> equip_validator(new QRegExpValidator(QRegExp("\\w{0,5}"), 0));
        //LineEditDelegate *equip_delegate = new LineEditDelegate(equip_validator, m_act_widget);
        //m_act_widget->setItemDelegateForColumn(0, equip_delegate);

        QSharedPointer<QValidator> acid_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,7}"), 0));
        QAbstractItemDelegate *acid_delegate = new LineEditDelegate(acid_validator, m_act_widget);
        m_act_widget->setItemDelegateForColumn(1, acid_delegate);

        QSharedPointer<QValidator> code_validator(new QRegExpValidator(QRegExp("[0-7]{4}"), 0));
        QAbstractItemDelegate *code_delegate = new LineEditDelegate(code_validator, m_act_widget);
        m_act_widget->setItemDelegateForColumn(2, code_delegate);

        if(!airpt.isEmpty() && mFpl.departureAerodrome() == airpt) {

            QAbstractItemDelegate *eto_delegate = new TimeDelegate(m_act_widget);
            m_act_widget->setItemDelegateForColumn(3, eto_delegate);

            QSharedPointer<QValidator> adep_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,4}"), 0));
            LineEditDelegate *adep_delegate = new LineEditDelegate(adep_validator, m_act_widget);
            m_act_widget->setItemDelegateForColumn(4, adep_delegate);

            QSharedPointer<QValidator> ades_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,4}"), 0));
            LineEditDelegate *ades_delegate = new LineEditDelegate(ades_validator, m_act_widget);
            m_act_widget->setItemDelegateForColumn(5, ades_delegate);
        }

        else {
            QSharedPointer<QValidator> cop_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,5}"), 0));
            LineEditDelegate *cop_delegate = new LineEditDelegate(cop_validator, m_act_widget);
            cop_delegate->setCompleter(getFir2Points());
            m_act_widget->setItemDelegateForColumn(3, cop_delegate);

            QAbstractItemDelegate *eto_delegate = new TimeDelegate(m_act_widget);
            m_act_widget->setItemDelegateForColumn(4, eto_delegate);

            QAbstractItemDelegate *pel_delegate = new FlightLevelDelegate(m_act_widget, this);
            m_act_widget->setItemDelegateForColumn(5, pel_delegate);

            QSharedPointer<QValidator> adep_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,4}"), 0));
            LineEditDelegate *adep_delegate = new LineEditDelegate(adep_validator, m_act_widget);
            m_act_widget->setItemDelegateForColumn(6, adep_delegate);

            QSharedPointer<QValidator> ades_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,4}"), 0));
            LineEditDelegate *ades_delegate = new LineEditDelegate(ades_validator, m_act_widget);
            m_act_widget->setItemDelegateForColumn(7, ades_delegate);
        }

        connect(m_act_widget, SIGNAL(act()), this, SLOT(onAct()));

        if (widget)
            m_act_widget->move(widget->pos());

        delete widget;
}

void PlanEditor::onAct()
{
    Act act_in = act_in_model_->act(0);
    sfpl_ = QSharedPointer<CSFPL>(new CSFPL);
    sfpl_->SetEquipment(act_in.eq);
    sfpl_->SetACID(act_in.acid);
    sfpl_->SetCODE(act_in.code.toInt(0, 8));
    sfpl_->SetCOPin(act_in.cop);
    sfpl_->SetCOPinFL(FLVal(act_in.pel, true));
    sfpl_->SetCFL(sfpl_->GetCOPinFL());
    sfpl_->SetDepAerodrome(act_in.adep);
    sfpl_->SetDestAerodrome(act_in.dest);
    sfpl_->SetRoute(newRoute);
    sfpl_->SetFRules(mFpl.flightRules());
    sfpl_->SetFType(mFpl.flightType());
    sfpl_->SetTurbulence(mFpl.turbulenceCategory());
    sfpl_->SetLit(mFpl.lit());
    sfpl_->SetSpeed(mFpl.speed());
    sfpl_->SetLevel(mFpl.level());
    sfpl_->SetTypeFlight(mFpl.aircraftType());
    sfpl_->SetAlt1Aerodrome(mFpl.altDestinationAerodrome());
    sfpl_->SetFPLEqf1(mFpl.equipment().split("/").front());
    sfpl_->SetFPLEqf2(mFpl.equipment().split("/").last());

    if (act_in_model_->type() == ActTableModel::act_dep)
        sfpl_->SetETD(act_in.eto);
    else
        sfpl_->SetETOin(act_in.eto);

    setInSector(sfpl_);

#ifdef QT_DEBUG
    if (!act_in.cs.isEmpty())
        sfpl_->SetCurSector(act_in.cs);
    if (!act_in.ns.isEmpty())
        sfpl_->SetNextSector(act_in.ns);
#endif

    if (master()->getRedisDB()) {
        bool ok = master()->getRedisDB()->saveSFPL(sfpl_);
        int min = networkSettings()->value("users/ORCAMLifeTime", 1).toInt();

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
            bool okk = master()->getRedisDB()->addOrcam(act_in.code, QDateTime::currentDateTime(), min);

        if (!ok || !okk)
            QMessageBox::critical(this, tr("Error"), tr("Error saving FPL"));
        else
            logOperation(tr("ACT IN: ( %1 )")
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

QStringList PlanEditor::getFirPoints()
{
    QStringList points;
    if (master()) {
        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (view) {
            CView::TSceneList::const_iterator itFIR = view->GetScenes().constBegin();
            for (; itFIR != view->GetScenes().constEnd(); ++itFIR)
            {
                Scene::TPointsMap::const_iterator itPoint = (*itFIR)->GetPoints().begin();
                for (; itPoint != (*itFIR)->GetPoints().end(); ++itPoint)
                    points << (*itPoint)->GetName();
            }
        }
    }
    points.removeAll("");
    points.removeDuplicates();
    return points;
}

QStringList PlanEditor::getFir2Points()
{
    QStringList points;
    if (master()) {
        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (view) {
            CView::TSceneList::const_iterator itFIR = view->GetScenes().constBegin();
            for (; itFIR != view->GetScenes().constEnd(); ++itFIR)
            {
                Scene::TPointsMap::const_iterator itPoint = (*itFIR)->GetPoints().begin();
                for (; itPoint != (*itFIR)->GetPoints().end(); ++itPoint)
                {
                    if((*itPoint)->GetSectorBorder())
                    points << (*itPoint)->GetName();
                }
            }
        }
    }
    points.removeAll("");
    points.removeDuplicates();
    return points;
}

void PlanEditor::setInSector(QSharedPointer<CSFPL> &sfpl)
{
    QString next_sector;
    if (CMaster *warehouse = master())
    {
        if (QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(warehouse->GetView()))
        {
            const QString fir_names = networkSettings()->value("FPL_handler/fir", "").toString();
            const QStringList fir_names_list = fir_names.split(",", QString::SkipEmptyParts);
            Scene::TSceneList sectors = view->FindSectorsByPoint(fir_names_list, sfpl->GetCOPin(), sfpl->GetCOPinFL() * 30.48);
            if (!sectors.empty()) {
                const QString main_sector = networkSettings()->value("users/current_sector").toString();
                QStringList all_controlled_sectors = networkSettings()->value("users/current_sectors_short_name", "").toString().split("-", QString::SkipEmptyParts);
                all_controlled_sectors.removeOne(main_sector);

                foreach (const Scene::TScenePtr &ptr, sectors) {
                    const QString sector = ptr->GetShortName();
                    qDebug() << sector;
                    if (sector == main_sector) {
                        sfpl->SetNextSector(main_sector);
                        return;
                    }

                    if (all_controlled_sectors.contains(sector)) {
                        sfpl->SetNextSector(main_sector);
                        return;
                    }
                }
                next_sector = sectors.first()->GetShortName();
            }
        }
    }

    sfpl->SetNextSector(next_sector);
}

void PlanEditor::SetUid(QUuid & uid)
{
    this->uid = uid;
}
void PlanEditor::SetId(int id)
{
    this->id = id;
}
void PlanEditor::onPushButtonFirstClicked()
{
    emit onFirstFPLClicked();

}
void PlanEditor::onPushButtonPrevClicked()
{
    emit onPrevFPLClicked();
}
void PlanEditor::onPushButtonNextClicked()
{
    emit onNextFPLClicked();
}
void PlanEditor::onPushButtonLastClicked()
{
    emit onLastFPLClicked();
}

void PlanEditor::onComboBoxActivated(const QString & text)
{
   /* QObject *o = QObject::sender();
    QComboBox * box = dynamic_cast<QComboBox*>(o);
    if(box)
    {
        box->blockSignals(true);
        if(box == MesTypeCombo)
        {
            box->lineEdit()->setText(MesTypeCombo_text);
        }
        if(box == FlightTypeCombo)
        {
            box->lineEdit()->setText(FlightTypeCombo_text);
        }
        if(box == RulesCombo)
        {
            box->lineEdit()->setText(RulesCombo_text);
        }
        if(box == AirplaneCombo)
        {
            box->lineEdit()->setText(AirplaneCombo_text);
        }
        if(box == TurbulenceCombo)
        {
            box->lineEdit()->setText(TurbulenceCombo_text);
        }
        if(box == Equipment1Combo)
        {
            box->lineEdit()->setText(Equipment1Combo_text);
        }

        box->blockSignals(false);
    }*/
}

void PlanEditor::showEvent(QShowEvent *event)
{
    SetNewData();
    QDialog::showEvent(event);
}

void PlanEditor::SetNewData()
{
    if(uid.isNull())
    {
        return;
    }
    // this->setWindowTitle(uid.toString());
    //setWindowTitle(tr("Editing Flight Plane #%1").arg(uid.toString()));
    sfpl_ = QSharedPointer<CSFPL>(new CSFPL);
    bool ok;
    ok = master()->getRedisDB()->loadSFPL(sfpl_,uid);
    if (!ok)
       logOperation(tr("Fail load from Redis '%1':").arg(uid.toString()));
    else
    {

        int index = MesTypeCombo->findText(sfpl_->GetMesType(),Qt::MatchExactly);
        if(index > -1)
        {
            MesTypeCombo_text = sfpl_->GetMesType();
        }
        else
        {
            MesTypeCombo_text = "FPL";
            index = MesTypeCombo->findText(MesTypeCombo_text, Qt::MatchExactly);
        }
        MesTypeCombo->setCurrentIndex(index);
        ACIDEdit->setText(sfpl_->GetACID());
        ACIDEdit->setStyleSheet(validValue);
        CODEEdit->setText(sfpl_->GetCODEtoString());
        CODEEdit->setStyleSheet(validValue);
        //FlightTypeCombo_text = sfpl_->GetFlightType();
        //qDebug() << FlightTypeCombo_text;
        //FlightTypeCombo->setCurrentIndex(FlightTypeCombo->findText(FlightTypeCombo_text,Qt::MatchExactly));
        //RulesCombo_text = sfpl_->GetFRules();
        //RulesCombo->setCurrentIndex(RulesCombo->findText(RulesCombo_text,Qt::MatchExactly));
        NumberEdit->setValue(sfpl_->GetNum());
        //qDebug() << AirplaneCombo_text;

        TurbulenceCombo->setEditText(sfpl_->GetTurbulence());
        RulesCombo->setEditText(sfpl_->GetFRules());
        FlightTypeCombo->setEditText(sfpl_->GetFType());

        //AirplaneCombo_text = sfpl_->GetTypeFlight();
        //AirplaneCombo->setCurrentIndex(AirplaneCombo->findText(AirplaneCombo_text,Qt::MatchExactly));
        AirplaneCombo->setEditText(sfpl_->GetTypeFlight());

        //TurbulenceCombo_text = sfpl_->GetTurbulence();
        //TurbulenceCombo->setCurrentIndex(TurbulenceCombo->findText(TurbulenceCombo_text,Qt::MatchExactly));
        //Equipment1Combo_text = sfpl_->GetEquipment();
        //Equipment1Combo->lineEdit()->setText(Equipment1Combo_text);
        //Equipment1Combo->setStyleSheet(validValue);
        Equipment2Edit->setText(sfpl_->GetFPLEqf1());
        Equipment2Edit->setStyleSheet(validValue);
        Equipment3Edit->setText(sfpl_->GetFPLEqf2());
        Equipment3Edit->setStyleSheet(validValue);
        AerodromeDepEdit->setText(sfpl_->GetDepAerodrome());
        AerodromeDepEdit->setStyleSheet(validValue);
        TimeDepEdit->setTime(sfpl_->GetDepTime().time());
        SpeedEdit->setText(sfpl_->GetSpeedS());
        SpeedEdit->setStyleSheet(validValue);

        RFLEdit->setText(sfpl_->GetLevelS());
        RFLEdit->setStyleSheet(validValue);
        RouteEdit->clear();
        RouteEdit->insertPlainText(sfpl_->GetRoute());


        AerodromeDestEdit->setText(sfpl_->GetDestAerodrome());
        AerodromeDestEdit->setStyleSheet(validValue);

        QStringList list = sfpl_->GetEET().split(":");
        QTime eet_time;
        eet_time.setHMS(0,0,0);
        if(list.size() > 1)
        {
            int th = list[0].toInt();
            int tm = list[1].toInt();
            eet_time = eet_time.addSecs(60*60*th + 60*tm);
        }
        CopouttimeEdit->setDisplayFormat("HH:mm");
        CopouttimeEdit->setTime(eet_time);
        ALTNEdit->setText(sfpl_->GetAlt1Aerodrome());
        ALTN2Edit->setText(sfpl_->GetAlt2Aerodrome());

        OtherEdit->setText(sfpl_->GetOther());
        lineEdit_2->setText(sfpl_->GetCurSector());
        lineEdit_4->setText(sfpl_->GetNextSector());
        lineEdit_3->setText(sfpl_->GetCODEtoString());
    }
}

void PlanEditor::SetNewData(int id)
{
    if(id == NULL)
    {
        return;
    }
    SetId(id);
    InitFpl();
    FillFields();

}



/*void PlanEditor::process12StandartEquipment(QString & first, QString & second, QString & third)
{
    if ( aftnMessageProcessor )
    {
        QVariantMap map;
        AFTNMessageProcessor::AftnMessageErrors errorCodes;
        aftnMessageProcessor->processType10(first, map, errorCodes);
        if ( !errorCodes.isEmpty() )
        {
            aftnMessageProcessor->validateType10(map, errorCodes);
            if ( !errorCodes.isEmpty() )
            {
                // TODO
            }

        }
    }
}*/


/** Update controls values from database */
void PlanEditor::init()
{


    aircrafttypesModel = new QSqlTableModel(this);
    aircrafttypesModel->setTable("airplane_type");
    aircrafttypesModel->sort(0, Qt::AscendingOrder);
    aircrafttypesModel->select();
    AirplaneCombo->setModel(aircrafttypesModel);
    AirplaneCombo->setModelColumn(aircrafttypesModel->fieldIndex("name"));

    // Flight types
    flightTypesModel = new QSqlTableModel(this);
    flightTypesModel->setTable("flight_types");
    flightTypesModel->sort(0, Qt::AscendingOrder);
    flightTypesModel->select();
    FlightTypeCombo->setModel(flightTypesModel);
    FlightTypeCombo->setModelColumn(flightTypesModel->fieldIndex("name"));

    // Turbulence category
    turbulenceModel = new QSqlTableModel(this);
    turbulenceModel->setTable("turbulence_category");
    turbulenceModel->sort(0, Qt::AscendingOrder);
    turbulenceModel->select();
    TurbulenceCombo->setModel(turbulenceModel);
    TurbulenceCombo->setModelColumn(turbulenceModel->fieldIndex("name"));

    // Flight rules
    flightRulesModel = new QSqlTableModel(this);
    flightRulesModel->setTable("flight_rules");
    flightRulesModel->sort(0, Qt::AscendingOrder);
    flightRulesModel->select();
    RulesCombo->setModel(flightRulesModel);
    RulesCombo->setModelColumn(flightRulesModel->fieldIndex("name"));

    // Level parameters
    levelTypesModel = new QSqlTableModel(this);
    levelTypesModel->setTable("level_types");
    levelTypesModel->select();
    ////LevelCombo->setModel(levelTypesModel);
    ////LevelCombo->setModelColumn(levelTypesModel->fieldIndex("name"));

    // Speed parameters
    /*speedTypesModel = new QSqlTableModel(this);
    speedTypesModel->setTable("speed_types");
    speedTypesModel->select();
    SpeedCombo->setModel(speedTypesModel);
    SpeedCombo->setModelColumn(speedTypesModel->fieldIndex("name"));

    statusTypesModel = new QSqlTableModel(this);
    statusTypesModel->setTable("fpl_status_types");
    statusTypesModel->select();*/
    ///status->setModel(statusTypesModel);
    ////status->setModelColumn(statusTypesModel->fieldIndex("name"));
    // Status parameters
    MesTypeCombo->addItem("FPL");
    //StatusCombo->addItem("ACT");
    //StatusCombo->addItem("DEP");
    // Rule flight parameters
    //RulesCombo->addItem("I");
   // RulesCombo->addItem("V");
    //RulesCombo->addItem("Y");
    //RulesCombo->addItem("Z");
    // Type flight parameters
    //FlightTypeCombo->addItem("S");
    //FlightTypeCombo->addItem("N");
    //FlightTypeCombo->addItem("G");
    //FlightTypeCombo->addItem("M");
    //FlightTypeCombo->addItem("X");

    //Equipment1Combo->addItem("N");
    //Equipment1Combo->addItem("S");
}

/** Save changes (update old plan or save as new) */
void PlanEditor::doSavePlan()
{
    /*bool isValid = true;

    sfpl_ = QSharedPointer<CSFPL>(new CSFPL);

    if(!uid.isNull())
        sfpl_->SetUid(uid);
    // field 3
    sfpl_->SetMesType(MesTypeCombo->currentText());
    if (!PlValidator::ValidateAircraftId(ACIDEdit->text().toUpper()))
    {
        ACIDEdit->setStyleSheet(invalidValue);
        ACIDEdit->setFocus();
        isValid = false;
    }
    else
    {
        sfpl_->SetACID(ACIDEdit->text().toUpper());
        ACIDEdit->setStyleSheet(validValue);
    }
    //sfpl_->SetCODE(CODEEdit->text().toInt(0, 8));
    if (!PlValidator::ValidateCode(CODEEdit->text()))
    {
        CODEEdit->setStyleSheet(invalidValue);
        CODEEdit->setFocus();
        isValid = false;
    }
    else
    {
        sfpl_->SetCODE(CODEEdit->text().toInt(0, 8));
        CODEEdit->setStyleSheet(validValue);
    }
    sfpl_->SetFRules(RulesCombo->currentText());
    sfpl_->SetFType(FlightTypeCombo->currentText());
    // field 9
    sfpl_->SetNum(NumberEdit->value());
    sfpl_->SetTypeFlight(AirplaneCombo->currentText());
    sfpl_->SetTurbulence(TurbulenceCombo->currentText());
    //sfpl_->SetFPLEqf1(Equipment1Edit->text());
    //sfpl_->SetFPLEqf1(Equipment1Combo->currentText());
    //sfpl_->SetFPLEqf2(Equipment2Edit->text());
    if (!PlValidator::ValidateEquipment2(Equipment2Edit->text()))
    {
        Equipment2Edit->setStyleSheet(invalidValue);
        Equipment2Edit->setFocus();
        isValid = false;
    }
    else
    {
        sfpl_->SetFPLEqf2(Equipment2Edit->text().toUpper());
        Equipment2Edit->setStyleSheet(validValue);
    }
    //sfpl_->SetFPLEqf3(Equipment3Edit->text());
    if (!PlValidator::ValidateEquipment3(Equipment3Edit->text()))
    {
        Equipment3Edit->setStyleSheet(invalidValue);
        Equipment3Edit->setFocus();
        isValid = false;
    }
    else
    {
        sfpl_->SetFPLEqf3(Equipment3Edit->text().toUpper());
        Equipment3Edit->setStyleSheet(validValue);
    }

   if(!(Equipment2Edit->text().contains("W",Qt::CaseSensitive)) && !(Equipment2Edit->text().contains("Y",Qt::CaseSensitive)))
        sfpl_->SetEquipment("wy");
    else
    {
        if (!(Equipment2Edit->text().contains("W",Qt::CaseSensitive)))
            sfpl_->SetEquipment("w");
        if (!Equipment2Edit->text().contains("Y",Qt::CaseSensitive))
            sfpl_->SetEquipment("y");
    }

    //field 13
    //sfpl_->SetDepAerodrome(AerodromeDepEdit->text());
    if (!PlValidator::ValidateAerodrome(AerodromeDepEdit->text()))
    {
        AerodromeDepEdit->setStyleSheet(invalidValue);
        AerodromeDepEdit->setFocus();
        isValid = false;
    }
    else
    {
        sfpl_->SetDepAerodrome(AerodromeDepEdit->text().toUpper());
        AerodromeDepEdit->setStyleSheet(validValue);
    }

    sfpl_->SetETD(TimeDepEdit->time());

    //field 15
    //sfpl_->SetSpeedS(SpeedEdit->text());
    if (!PlValidator::ValidateSpeed(SpeedEdit->text()))
    {
        SpeedEdit->setStyleSheet(invalidValue);
        SpeedEdit->setFocus();
        isValid = false;
    }
    else
    {
        sfpl_->SetSpeedS(SpeedEdit->text().toUpper());
        SpeedEdit->setStyleSheet(validValue);
    }
    //sfpl_->SetRFL(RFLEdit->text());
    if (!PlValidator::ValidateLevel(RFLEdit->text()))
    {
        RFLEdit->setStyleSheet(invalidValue);
        RFLEdit->setFocus();
        isValid = false;
    }
    else
    {
        sfpl_->SetRFL(RFLEdit->text().toUpper());
        RFLEdit->setStyleSheet(validValue);
    }
    sfpl_->SetCOPin(RouteEdit->toPlainText().left(RouteEdit->toPlainText().indexOf(' ')));
    sfpl_->SetCOPout(RouteEdit->toPlainText().right(RouteEdit->toPlainText().indexOf(' ')));
    sfpl_->SetRoute(RouteEdit->toPlainText());

    //field 16
    //sfpl_->SetDestAerodrome(AerodromeDestEdit->text());
    if (!PlValidator::ValidateAerodrome(AerodromeDestEdit->text()))
    {
        AerodromeDestEdit->setStyleSheet(invalidValue);
        AerodromeDestEdit->setFocus();
        isValid = false;
    }
    else
    {
        sfpl_->SetDestAerodrome(AerodromeDestEdit->text().toUpper());
        AerodromeDestEdit->setStyleSheet(validValue);
    }

    sfpl_->SetETD(TimeDepEdit->time());


    QTime t;
    int th = CopouttimeEdit->time().hour();
    int tm = CopouttimeEdit->time().minute();
    t = TimeDepEdit->time().addSecs(60*60*th + 60*tm);
    sfpl_->SetETA(t);
    //sfpl_->SetAlt1Aerodrome(ALTNEdit->text());
    if (!PlValidator::ValidateAerodrome(ALTNEdit->text()))
    {
        ALTNEdit->setStyleSheet(invalidValue);
        ALTNEdit->setFocus();
        isValid = false;
    }
    else
    {
        sfpl_->SetAlt1Aerodrome(ALTNEdit->text().toUpper());
        ALTNEdit->setStyleSheet(validValue);
    }
    //sfpl_->SetAlt2Aerodrome(ALTN2Edit->text());
    if (!PlValidator::ValidateAerodrome(ALTN2Edit->text()))
    {
        ALTN2Edit->setStyleSheet(invalidValue);
        ALTN2Edit->setFocus();
        isValid = false;
    }
    else
    {
        sfpl_->SetAlt2Aerodrome(ALTN2Edit->text().toUpper());
        ALTN2Edit->setStyleSheet(validValue);
    }

    //field 18
    //sfpl_->SetOther(OtherEdit->toPlainText());
    if (!PlValidator::ValidateOther(OtherEdit->toPlainText()))
    {
        OtherEdit->setStyleSheet(invalidValue);
        OtherEdit->setFocus();
        isValid = false;
    }
    else
    {
        sfpl_->SetOther(OtherEdit->toPlainText().toUpper().trimmed());
        OtherEdit->setStyleSheet(validValue);
    }


    if(isValid){
      if (master()->getRedisDB()) {
        bool ok = master()->getRedisDB()->saveSFPL(sfpl_);
        if (!ok)
            QMessageBox::critical(this, tr("Error"), tr("Error saving FPL"));
        else
            logOperation(tr("Plan: ( %1 )")
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
    close();
    }
    else return;*/

}

void PlanEditor::onClose()
{
    close();
}


void PlanEditor::onTextChanged()
{
    if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(sender())) {
        QString original = textEdit->toPlainText();
        QString uppered = original.toUpper();

        if (original != uppered) {
            QTextCursor textCursor = textEdit->textCursor();
            int iPosition = textCursor.position();
            textEdit->setPlainText(uppered);
            textEdit->moveCursor(QTextCursor::Start);

            for (; iPosition > 0; --iPosition)
                textEdit->moveCursor(QTextCursor::NextCharacter);
        }
    }
}
/*
void PlanEditor::on_ACIDEdit_editingFinished()
{
    if (!PlValidator::ValidateAircraftId(ACIDEdit->text()))
        ACIDEdit->setStyleSheet(invalidValue);
    else
        ACIDEdit->setStyleSheet(validValue);
}

void PlanEditor::on_CODEEdit_editingFinished()
{
    if (!PlValidator::ValidateCode(CODEEdit->text()))
    {
        CODEEdit->setStyleSheet(invalidValue);
        CODEEdit->setFocus();
    }
    else
        CODEEdit->setStyleSheet(validValue);
}

void PlanEditor::on_Equipment2Edit_editingFinished()
{
    if (!PlValidator::ValidateEquipment2(Equipment2Edit->text()))
    {
        Equipment2Edit->setStyleSheet(invalidValue);
        Equipment2Edit->setFocus();
    }
    else
        Equipment2Edit->setStyleSheet(validValue);
}

void PlanEditor::on_Equipment3Edit_editingFinished()
{
    if (!PlValidator::ValidateEquipment3(Equipment3Edit->text()))
    {
        Equipment3Edit->setStyleSheet(invalidValue);
        Equipment3Edit->setFocus();
    }
    else
        Equipment3Edit->setStyleSheet(validValue);
}

void PlanEditor::on_AerodromeDepEdit_editingFinished()
{
    if (!PlValidator::ValidateAerodrome(AerodromeDepEdit->text()))
    {
        AerodromeDepEdit->setStyleSheet(invalidValue);
        AerodromeDepEdit->setFocus();
    }
    else
        AerodromeDepEdit->setStyleSheet(validValue);
}

void PlanEditor::on_SpeedEdit_editingFinished()
{
    if (!PlValidator::ValidateSpeed(SpeedEdit->text()))
    {
        SpeedEdit->setStyleSheet(invalidValue);
        SpeedEdit->setFocus();
    }
    else
        SpeedEdit->setStyleSheet(validValue);
}

void PlanEditor::on_RFLEdit_editingFinished()
{
    if (!PlValidator::ValidateLevel(RFLEdit->text()))
    {
        RFLEdit->setStyleSheet(invalidValue);
        RFLEdit->setFocus();
    }
    else
        RFLEdit->setStyleSheet(validValue);
}

void PlanEditor::on_RouteEdit_editingFinished()
{
    //if (mFpl.setRoute(RouteEdit->toPlainText()))
    //     RouteEdit->setStyleSheet(validValue);
    //else
    //    RouteEdit->setStyleSheet(invalidValue);

    // Logic route control

    //QVariantMap map;
    //AFTNMessageProcessor::AftnMessageErrors errorCodes;
    //map.insert(AFTN_MESSAGE_ROUTE, RouteEdit->toPlainText());
    //bool bEmptyFIR = aftnMessageProcessor->FIRName().isEmpty();
    //if(bEmptyFIR)
    //    aftnMessageProcessor->setFIRName(mFpl.firName());
    //aftnMessageProcessor->validateType15(map, errorCodes);
    //if (!errorCodes.isEmpty()) {
    //    RouteEdit->setStyleSheet(invalidValue);
    //} else {
    //    mFpl.setRoute(map.value(AFTN_MESSAGE_ROUTE).toString());
    //    RouteEdit->setStyleSheet(validValue);
    //}
    //if(bEmptyFIR)
    //    aftnMessageProcessor->setFIRName("");

}

void PlanEditor::on_AerodromeDestEdit_editingFinished()
{
    if (!PlValidator::ValidateAerodrome(AerodromeDestEdit->text()))
    {
        AerodromeDestEdit->setStyleSheet(invalidValue);
        AerodromeDestEdit->setFocus();
    }
    else
        AerodromeDestEdit->setStyleSheet(validValue);
}

void PlanEditor::on_ALTNEdit_editingFinished()
{
    if (!PlValidator::ValidateAerodrome(ALTNEdit->text()))
    {
        ALTNEdit->setStyleSheet(invalidValue);
        ALTNEdit->setFocus();
    }
    else
        ALTNEdit->setStyleSheet(validValue);
}

void PlanEditor::on_ALTN2Edit_editingFinished()
{
    if (!PlValidator::ValidateAerodrome(ALTN2Edit->text()))
    {
        ALTN2Edit->setStyleSheet(invalidValue);
        ALTN2Edit->setFocus();
    }
    else
        ALTN2Edit->setStyleSheet(validValue);
}

void PlanEditor::on_OtherEdit_editingFinished()
{
    if (!PlValidator::ValidateOther(OtherEdit->toPlainText()))
    {
        OtherEdit->setStyleSheet(invalidValue);
        OtherEdit->setFocus();
    }
    else
        OtherEdit->setStyleSheet(validValue);
}*/

void PlanEditor::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);

    if (event->type() == QEvent::LanguageChange) {
        retranslateUi(this);
        ////printButton->setText(tr("Print..."));
    }
}

void PlanEditor::OnFixedSize()
{
    setFixedSize(width(), height());
}

void PlanEditor::setCanOtherInfoEmpty(bool bCan)
{
    canOtherInfoEmpty = bCan;
}

void PlanEditor::InitFpl()
{
    model_fpl = new QSqlQueryModel();
    model_fpl->setQuery("SELECT * FROM fpl WHERE id = " + QString::number(this->id));
    mFpl.updateInfo(model_fpl->record(0));
}

void PlanEditor::FillFields()
{
    ACIDEdit->setText(mFpl.aircarftId());
    ACIDEdit->setStyleSheet(validValue);
    QFont font = this->font();
    QFontMetrics fm(font);
    int lenM = 0;
    lenM = fm.width(mFpl.aircarftId());
    ACIDEdit->setMinimumWidth(lenM+15);
    ACIDEdit->setMaximumWidth(lenM+15);
    CODEEdit->setMinimumWidth(15);
    CODEEdit->setMaximumWidth(15);

    lenM = fm.width(MesTypeCombo->currentText());
    MesTypeCombo->setMinimumWidth(lenM+40);
    MesTypeCombo->setMaximumWidth(lenM+40);

    //TurbulenceCombo->setCurrentIndex(TurbulenceCombo->findText(mFpl.turbulenceCategory()));
    //RulesCombo->setCurrentIndex(RulesCombo->findText(mFpl.flightRules(),Qt::MatchExactly));
    //FlightTypeCombo->setCurrentIndex(FlightTypeCombo->findText(mFpl.flightType()));

    TurbulenceCombo->setEditText(mFpl.turbulenceCategory());
    lenM = fm.width(mFpl.turbulenceCategory());
    TurbulenceCombo->setMinimumWidth(lenM+40);
    TurbulenceCombo->setMaximumWidth(lenM+40);
    RulesCombo->setEditText(mFpl.flightRules());
    lenM = fm.width(mFpl.flightRules());
    RulesCombo->setMinimumWidth(lenM+40);
    RulesCombo->setMaximumWidth(lenM+40);

    FlightTypeCombo->setEditText(mFpl.flightType());
    lenM = fm.width(mFpl.flightType());
    FlightTypeCombo->setMinimumWidth(lenM+40);
    FlightTypeCombo->setMaximumWidth(lenM+40);

    NumberEdit->setValue(mFpl.aircraftNumber());
    lenM = fm.width(mFpl.aircraftNumber());
    NumberEdit->setMinimumWidth(lenM+40);
    NumberEdit->setMaximumWidth(lenM+40);

    AirplaneCombo->setStyleSheet(validValue);
    AirplaneCombo->setCurrentIndex(AirplaneCombo->findText(mFpl.aircraftType()));
    if (AirplaneCombo->currentIndex() == -1)
        AirplaneCombo->setEditText(mFpl.aircraftType());
    lenM = fm.width(mFpl.aircraftType());
    AirplaneCombo->setMinimumWidth(lenM+40);
    AirplaneCombo->setMaximumWidth(lenM+40);

    Equipment2Edit->setText(mFpl.equipment().split("/").front());
    Equipment2Edit->setStyleSheet(validValue);
    lenM = fm.width(mFpl.equipment().split("/").front());
    Equipment2Edit->setMinimumWidth(lenM+15);
    Equipment2Edit->setMaximumWidth(lenM+15);

    Equipment3Edit->setText(mFpl.equipment().split("/").last());
    Equipment3Edit->setStyleSheet(validValue);
    lenM = fm.width(mFpl.equipment().split("/").last());
    Equipment3Edit->setMinimumWidth(lenM+15);
    Equipment3Edit->setMaximumWidth(lenM+15);

    AerodromeDepEdit->setText(mFpl.departureAerodrome());
    AerodromeDepEdit->setStyleSheet(validValue);
    lenM = fm.width(mFpl.departureAerodrome());
    AerodromeDepEdit->setMinimumWidth(lenM+15);
    AerodromeDepEdit->setMaximumWidth(lenM+15);

    TimeDepEdit->setTime(mFpl.departureTime());

    //entryFIREET->setText(mFpl.entryFIREET());
    //entryFIREET->setStyleSheet(validValue);

    //EET->setText(mFpl.EET());
    //EET->setStyleSheet(validValue);

    RouteEdit->setText(mFpl.route());
    RouteEdit->setStyleSheet(validValue);
    lenM = fm.width(mFpl.route());
    RouteEdit->setFont(font);
    /*if(lenM < 700)
    {
        RouteEdit->setMinimumWidth(lenM+15);
        RouteEdit->setMaximumWidth(lenM+15);
    }*/

    AerodromeDestEdit->setText(mFpl.destinationAerodrome());
    AerodromeDestEdit->setStyleSheet(validValue);
    lenM = fm.width(mFpl.destinationAerodrome());
    AerodromeDestEdit->setMinimumWidth(lenM+15);
    AerodromeDestEdit->setMaximumWidth(lenM+15);

    OtherEdit->setText(mFpl.otherInformation());
    OtherEdit->setStyleSheet(validValue);
    lenM = fm.width(mFpl.otherInformation());
    /*if(lenM < 700)
    {
        OtherEdit->setMinimumWidth(lenM+15);
        OtherEdit->setMaximumWidth(lenM+15);
    }*/

    ALTNEdit->setText(mFpl.altDestinationAerodrome());
    ALTNEdit->setStyleSheet(validValue);
    lenM = fm.width(mFpl.altDestinationAerodrome());
    ALTNEdit->setMinimumWidth(lenM+15);
    ALTNEdit->setMaximumWidth(lenM+15);

    ALTN2Edit->setText(mFpl.altDestinationAero2());
    ALTN2Edit->setStyleSheet(validValue);
    lenM = fm.width(mFpl.altDestinationAero2());
    ALTN2Edit->setMinimumWidth(lenM+15);
    ALTN2Edit->setMaximumWidth(lenM+15);

    //SpeedCombo->setCurrentIndex(SpeedCombo->findText(mFpl.speedUnit()));
    //SpeedCombo->setStyleSheet(validValue);

    SpeedEdit->setText(mFpl.speed());
    SpeedEdit->setStyleSheet(validValue);
    lenM = fm.width(mFpl.speed());
    SpeedEdit->setMinimumWidth(lenM+15);
    SpeedEdit->setMaximumWidth(lenM+15);

    RFLEdit->setText(mFpl.level());
    RFLEdit->setStyleSheet(validValue);
    lenM = fm.width(mFpl.level());
    RFLEdit->setMinimumWidth(lenM+15);
    RFLEdit->setMaximumWidth(lenM+15);

    //LevelCombo->setCurrentIndex(LevelCombo->findText(mFpl.levelUnit()));
    //LevelCombo->setStyleSheet(validValue);

    //LevelEdit->setText(mFpl.levelValue());
    //LevelEdit->setStyleSheet(validValue);

    //status->setCurrentIndex(status->findText(mFpl.exFPL()));

    //FIRCombo->setFocus();

    //Рассчитываем какая строка в формуляре длиннее
    // 1-ая строка
    int MesTypeCombo_w = MesTypeCombo->size().width();
    int ACIDEdit_w = ACIDEdit->size().width();
    int CODEEdit_w = CODEEdit->size().width();
    int RulesCombo_w = RulesCombo->size().width();
    int FlightTypeCombo_w = FlightTypeCombo->size().width();
    int line_1_w = MesTypeCombo_w + ACIDEdit_w + CODEEdit_w +RulesCombo_w + FlightTypeCombo_w + 30;

    // 2-ая строка
    int NumberEdit_w = NumberEdit->size().width();
    int AirplaneCombo_w = AirplaneCombo->size().width();
    int TurbulenceCombo_w = TurbulenceCombo->size().width();
    int Equipment2Edit_w = Equipment2Edit->size().width();
    int Equipment3Edit_w = Equipment3Edit->size().width();
    int line_2_w = NumberEdit_w + AirplaneCombo_w + TurbulenceCombo_w +Equipment2Edit_w + Equipment3Edit_w + 15;

    QSize size = this->size();
    if(line_1_w > line_2_w)
    {
        this->setMaximumSize(line_1_w + 160, size.height());
        this->setMinimumSize(line_1_w + 160, size.height());
    }
    else
    {
        if(line_2_w > 300)
            line_2_w += 80;
        else
            line_2_w += 50;

        this->setMaximumSize(line_2_w + 110, size.height());
        this->setMinimumSize(line_2_w + 110, size.height());
    }
    pushButton_11->setFont(font);
}

/*
void PlanEditor::showEvent(QShowEvent *event)
{

    if(!uid.isNull())
   {
       // this->setWindowTitle(uid.toString());
        setWindowTitle(tr("Editing Flight Plane #%1").arg(uid.toString()));
        sfpl_ = QSharedPointer<CSFPL>(new CSFPL);
        bool ok;
        ok = master()->getRedisDB()->loadSFPL(sfpl_,uid);
        if (!ok)
           logOperation(tr("Fail load from Redis '%1':").arg(uid.toString()));
        else
        {

            MesTypeCombo->setCurrentIndex(MesTypeCombo->findText(sfpl_->GetMesType(),Qt::MatchExactly));
            ACIDEdit->setText(sfpl_->GetACID());
            ACIDEdit->setStyleSheet(validValue);
            //CODEEdit->setText(QString::number(sfpl_->GetCODE()));
            CODEEdit->setText(sfpl_->GetCODEtoString());
            CODEEdit->setStyleSheet(validValue);
            FlightTypeCombo->setCurrentIndex(FlightTypeCombo->findText(sfpl_->GetFType(),Qt::MatchExactly));
            RulesCombo->setCurrentIndex(RulesCombo->findText(sfpl_->GetFRules(),Qt::MatchExactly));


            NumberEdit->setValue(sfpl_->GetNum());
            AirplaneCombo->setCurrentIndex(AirplaneCombo->findText(sfpl_->GetTypeFlight(),Qt::MatchExactly));
            TurbulenceCombo->setCurrentIndex(TurbulenceCombo->findText(sfpl_->GetTurbulence(),Qt::MatchExactly));
            //Equipment1Edit->setText(sfpl_->GetFPLEqf1());
            Equipment1Combo->setCurrentIndex(Equipment1Combo->findText(sfpl_->GetFPLEqf1(),Qt::MatchExactly));
            Equipment1Combo->setStyleSheet(validValue);
            Equipment2Edit->setText(sfpl_->GetFPLEqf2());
            Equipment2Edit->setStyleSheet(validValue);
            Equipment3Edit->setText(sfpl_->GetFPLEqf3());
            Equipment3Edit->setStyleSheet(validValue);

            AerodromeDepEdit->setText(sfpl_->GetDepAerodrome());
            AerodromeDepEdit->setStyleSheet(validValue);
            TimeDepEdit->setTime(sfpl_->GetETD());
            SpeedEdit->setText(sfpl_->GetSpeedS());
            SpeedEdit->setStyleSheet(validValue);
            RFLEdit->setText(sfpl_->GetRFL().toString());
            RFLEdit->setStyleSheet(validValue);
            RouteEdit->insertPlainText(sfpl_->GetRoute());


            AerodromeDestEdit->setText(sfpl_->GetDestAerodrome());
            AerodromeDestEdit->setStyleSheet(validValue);

             QTime t = sfpl_->GetETA();
             int th = TimeDepEdit->time().hour();
             int tm = TimeDepEdit->time().minute();
             t = t.addSecs(-60*60*th - 60*tm);

            CopouttimeEdit->setTime(t);
            ALTNEdit->setText(sfpl_->GetAlt1Aerodrome());
            ALTN2Edit->setText(sfpl_->GetAlt2Aerodrome());

            OtherEdit->setText(sfpl_->GetOther());
        }
    }
}
*/

void PlanEditor::AdaptateRoute(QString route)
{
    AFTNMessageProcessor mProcessor;
    mProcessor.setValidateRoute(false);
    mProcessor.setSaveOnlyFIR(true);
    const QString fir_names = networkSettings()->value("FPL_handler/fir", "").toString();
    mProcessor.setFIRName(fir_names);
    AFTNMessageProcessor::AftnMessageErrors errorCodes;
    newRoute = mProcessor.cutFIRRoute2(route, errorCodes, false);
}

void PlanEditor::keyPressEvent(QKeyEvent *event)
{
    if ((Qt::CTRL|Qt::ALT) == event->modifiers())
            if (Qt::Key_A == event->key())
                label->isHidden() ? setVis(true) : setVis(false);

    QDialog::keyPressEvent(event);
}

void PlanEditor::setVis(bool visible)
{
    int heightL = height() + label->height()*3;
    int heightS = height() - label->height()*3;
    //visible ? (setMinimumSize(QSize(1269, heightL)), setMaximumSize(QSize(1269, heightL))) : (setMinimumSize(QSize(1069, heightS)), setMaximumSize(QSize(1069, heightS)));

    label->setVisible(visible);
    label_7->setVisible(visible);
    lineEdit_2->setVisible(visible);
    label_9->setVisible(visible);
    lineEdit_4->setVisible(visible);
    label_11->setVisible(visible);
    lineEdit_6->setVisible(visible);
    label_2->setVisible(visible);
    //label_8->setVisible(visible);
    lineEdit_3->setVisible(visible);
    label_10->setVisible(visible);
    lineEdit_5->setVisible(visible);
    label_12->setVisible(visible);
    lineEdit_7->setVisible(visible);
}
void PlanEditor::InitSize()
{
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setSizePolicy(sizePolicy);
    QSize size = this->size();
    this->setMaximumSize(size);
    this->setMinimumSize(size);
    //setMinimumSize(QSize(1069, 460));
    //setMaximumSize(QSize(1069, 460));
    //horizontalGroupBox->setMinimumHeight(45);
    //horizontalGroupBox->setMaximumHeight(45);
}

QString PlanEditor::FreeCodeOrcam()
{
    QString code = "";
    QMultiMap<QString, QString> map;
    if(master() && master()->getRedisDB()){
        master()->getRedisDB()->loadOrcams(map);
        foreach(QString lst_code, lst_orcamrange){
            code = lst_code;
            QMapIterator <QString, QString> iter(map);
            while (iter.hasNext()){
                iter.next();
                if(lst_code == iter.key()){
                    code = "";
                    break;
                }
              if(code.isEmpty())
                  break;
            }
            if(!code.isEmpty())
               break;
        }
    }

    return code;
}
