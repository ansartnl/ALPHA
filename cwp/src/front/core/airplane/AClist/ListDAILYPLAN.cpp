#include "ListDAILYPLAN.h"

#include "ControlDlg.h"
#include "ListACItemDelegate.h"

#include "main.h"
#include "sfpleditor.h"
#include "Master.h"

#include "acttablemodel.h"
#include "actwidget.h"
#include "lineeditdelegate.h"
#include "flightleveldelegate.h"
#include "timedelegate.h"
#include "comboboxdelegate.h"

#include "planeditor.h"
#include "ListACHeaderView.h"
#include "dbconnectionsmanager.h"
#include "databasesubscriber.h"

#include "ACTDlg.h"

#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>

#include <QHeaderView>
#include <QCheckBox>
#include <QMenu>

/*bool calcNextSector(const QString &cop_in, int pel)
{
    if (CMaster *warehouse = master()) {
        if (QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(warehouse->GetView())) {
            const QString fir_names = networkSettings()->value("FPL_handler/fir", "").toString();
            const QStringList fir_names_list = fir_names.sp(",", QString::SkipEmptyParts);
            Scene::TSceneList sectors = view->FindSectorsByPoint(fir_names_list, cop_in, pel * 30.48);
            return !sectors.empty();
        }
    }
    return false;
}*/

class ANOModel : public QSqlQueryModel
{
public:
    ANOModel(QObject *parent = 0) : QSqlQueryModel(parent)
    {
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
        return CListACModel::COL_COUNT;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        if (role == Qt::DisplayRole)
        {
            QSqlRecord rec = record(index.row());
            switch (index.column())
            {
            case CListACModel::ACID:
                return rec.value("aircraft_id");
            case CListACModel::DATE:
                return rec.value("time");
            case CListACModel::ADEP:
                return rec.value("adep");
            case CListACModel::ETD:
                return rec.value("t");
            case CListACModel::DEST:
                return rec.value("ades");
            case CListACModel::ETA:
                return rec.value("eet");
            case CListACModel::ALTN:
                return rec.value("alt1");
            case CListACModel::LIT:
                return rec.value("lit");
            case CListACModel::Type:
                return rec.value("taircraft");
            case CListACModel::STATUS:
                return rec.value("exfpl");
            default:
                return QVariant();
            }
        }
        if (role == Qt::ToolTipRole)
        {
            if (index.column() == CListACModel::STATUS)
            {
                QString tooltip = data(index, Qt::DisplayRole).toString();
                if (!tooltip.trimmed().isEmpty())
                    return tooltip;
            }
            return QVariant();
        }

        return QSqlQueryModel::data(index, role);
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
        {
            if ((section >= 0) && (section < CListACModel::COL_COUNT))
            {
                switch (section)
                {
                case CListACModel::ACID: return tr("ACID");
                case CListACModel::CODE: return tr("CODE");
                case CListACModel::COPin: return tr("COPin");
                case CListACModel::SID: return tr("SID");
                case CListACModel::COPin_SID: return tr("COPin nSID nRW");
                case CListACModel::ETOin: return tr("ETO");
                case CListACModel::ETD_ATD: return tr("ETD ATD");
                case CListACModel::ETO_ATOin: return tr("ETO ATO");
                case CListACModel::ETO_ATO_ETD: return tr("ETO ATO nETD ATD");
                case CListACModel::PEL: return tr("PEL");
                case CListACModel::PEL_CFL: return tr("PEL CFL");
                case CListACModel::COPout: return tr("COPout");
                case CListACModel::STAR: return tr("STAR");
                case CListACModel::COPout_STAR: return tr("COPout nSTAR nRW");
                case CListACModel::ETOout: return tr("ETO");
                case CListACModel::ETO_ATOout: return tr("ETO ATO");
                case CListACModel::ETO_ATO_ATA: return tr("ATA");
                case CListACModel::RFL_TFL: return tr("RFL TFL");
                case CListACModel::Type:
                case CListACModel::Type_AC: return tr("TYPE");
                case CListACModel::Wct: return tr("Wct");
                case CListACModel::Stand: return tr("Stand");
                case CListACModel::ADEP: return tr("ADEP");
                case CListACModel::DEST: return tr("DEST");
                case CListACModel::ALTN: return tr("ALTN");
                case CListACModel::REM: return tr("WARN");
                case CListACModel::RVSM: return tr("RVSM");
                case CListACModel::Azimuth: return tr("Course Angle");
                case CListACModel::CurrentSpeed: return tr("Speed");
                case CListACModel::Eq: return tr("Eq");
                case CListACModel::STATUS: return tr("STATUS");
                case CListACModel::LIT: return tr("LIT");
                case CListACModel::FPLEqf1: return tr("FPLEqf1");
                case CListACModel::FPLEqf2: return tr("FPLEqf2");
                case CListACModel::FPLEqf3: return tr("FPLEqf3");
                case CListACModel::ETD: return tr("ETD");
                case CListACModel::ETA: return tr("ETA");
                case CListACModel::FType: return tr("FType");
                case CListACModel::MesType: return tr("MesType");
                case CListACModel::DATE: return tr("DATE");
                case CListACModel::Scroll:
                case CListACModel::None: return QVariant();
                }
            }
        }
        if((orientation == Qt::Horizontal) && (role == Qt::DecorationRole))
        {
            if((section >= 0) && (section < CListACModel::COL_COUNT))
            {
                if(section == CListACModel::Scroll)
                {
                    QPixmap pixmap(":/Arrows/TwoWayArrow");
                    QVariant var;
                    var.setValue(pixmap);
                    return var;
                }
            }
        }
        if((orientation == Qt::Horizontal) && (role == Qt::UserRole+1))
            return section;

        if((orientation == Qt::Horizontal) && (role == Qt::FontRole))
        {
            QFont font1 = QFont();
            font1.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
            return font1;
        }

        return QSqlQueryModel::headerData(section, orientation, role);
    }
};

class ANOProxyModel : public QSortFilterProxyModel
{
public:
    ANOProxyModel(QObject *parent = 0) : QSortFilterProxyModel(parent),
        mFilterAlternative(false), mFilterArrival(false), mFilterDeparture(false)
    {
        mAirport = networkSettings()->value("meteo/airport").toString();
    }

    void refresh()
    {
        invalidateFilter();
    }

    void setFilterAlternative(bool enable) {mFilterAlternative = enable;}
    void setFilterArrival(bool enable) {mFilterArrival = enable;}
    void setFilterDeparture(bool enable) {mFilterDeparture = enable;}

protected:
    bool filterAcceptsRow(int row, const QModelIndex &parent) const
    {
        if (mFilterAlternative || mFilterArrival || mFilterDeparture)
        {
            if (mFilterAlternative && sourceModel()->index(row, CListACModel::ALTN, parent).data().toString() == mAirport)
                return true;

            if (mFilterArrival && sourceModel()->index(row, CListACModel::DEST, parent).data().toString() == mAirport)
                return true;

            if (mFilterDeparture && sourceModel()->index(row, CListACModel::ADEP, parent).data().toString() == mAirport)
                return true;

            return false;
        }
        return true;
    }

private:
    QString mAirport;
    bool mFilterAlternative;
    bool mFilterArrival;
    bool mFilterDeparture;
};

class FixedWidthDelegate : public QStyledItemDelegate
{
public:
    FixedWidthDelegate(QObject *parent = 0) : QStyledItemDelegate(parent)
    {
        mWidth = networkSettings()->value("users/SizeListAC/widthColumn").toInt();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QSize sz = QStyledItemDelegate::sizeHint(option, index);
        sz.setWidth(mWidth);
        return sz;
    }

private:
    int mWidth;
};


CListDailyPlan::CListDailyPlan(CMaster *pMaster, QWidget *pParent) :
        CListAC(pMaster, pParent), act_in_model_(0), act_in_widget_(0), clickColumn(0)
{
    _pParent = pParent;
    Init();
}

CListDailyPlan::CListDailyPlan(QStringList _lst, CMaster *pMaster, QWidget *pParent) :
        CListAC(pMaster, pParent), act_in_model_(0), act_in_widget_(0), clickColumn(0)
{
    _pParent = pParent;
    Init();
    lst_orcamrange = _lst;
}

CListDailyPlan::~CListDailyPlan()
{

}

quint32 CListDailyPlan::CalcMinWidth(void)
{
    //max line
    quint32 max_line_width = 0;

    //qDebug()<<"\n\n\n";

    if (mRowCount->isVisibleTo(this))
    {
        //qDebug()<<"Element mRowCount with sizeHint="<< mRowCount->sizeHint().width();
        max_line_width += mRowCount->sizeHint().width();
    }

    {
        //qDebug()<<"Element 10 with sizeHint=10";
        max_line_width += 10;
    }

    if (mFilterAll->isVisibleTo(this))
    {
        //qDebug()<<"Element mFilterAll with sizeHint="<< mFilterAll->sizeHint().width();
        max_line_width += mFilterAll->sizeHint().width();
    }

    if (mFilterDeparture->isVisibleTo(this))
    {
        //qDebug()<<"Element mFilterDeparture with sizeHint="<< mFilterDeparture->sizeHint().width();
        max_line_width += mFilterDeparture->sizeHint().width();
    }

    if (mFilterArrival->isVisibleTo(this))
    {
        //qDebug()<<"Element mFilterArrival with sizeHint="<< mFilterArrival->sizeHint().width();
        max_line_width += mFilterArrival->sizeHint().width();
    }

    if (mFilterAlternative->isVisibleTo(this))
    {
        //qDebug()<<"Element mFilterAlternative with sizeHint="<< mFilterAlternative->sizeHint().width();
        max_line_width += mFilterAlternative->sizeHint().width();
    }

    if (but_actplan->isVisibleTo(this))
    {
        //qDebug()<<"Element but_actplan with sizeHint="<< but_actplan->sizeHint().width();
        max_line_width += but_actplan->sizeHint().width();
    }

    if (but_deactplan->isVisibleTo(this))
    {
        //qDebug()<<"Element but_deactplan with sizeHint="<< but_deactplan->sizeHint().width();
        max_line_width += but_deactplan->sizeHint().width();
    }

    if (but_viewplan->isVisibleTo(this))
    {
        //qDebug()<<"Element but_viewplan with sizeHint="<< but_viewplan->sizeHint().width();
        max_line_width += but_viewplan->sizeHint().width();
    }

    if (but_editplan->isVisibleTo(this))
    {
        //qDebug()<<"Element but_editplan with sizeHint="<< but_editplan->sizeHint().width();
        max_line_width += but_editplan->sizeHint().width();
    }

    if (ano_button->isVisibleTo(this))
    {
        //qDebug()<<"Element ano_button with sizeHint="<< ano_button->sizeHint().width();
        max_line_width += ano_button->sizeHint().width();
    }

    if (cwp_button->isVisibleTo(this))
    {
        //qDebug()<<"Element cwp_button with sizeHint="<< cwp_button->sizeHint().width();
        max_line_width += cwp_button->sizeHint().width();
    }

    //qDebug() << "max_line min_width="<<max_line_width;


    //min line
    quint32 min_line_width = 0;
    if (ano_button_s->isVisibleTo(this))
    {
        //qDebug()<<"Element ano_button_s with sizeHint="<< ano_button_s->sizeHint().width();
        min_line_width += ano_button_s->sizeHint().width();
    }

    if (cwp_button_s->isVisibleTo(this))
    {
        //qDebug()<<"Element cwp_button_s with sizeHint="<< cwp_button_s->sizeHint().width();
        min_line_width += cwp_button_s->sizeHint().width();
    }

    //qDebug() << "min_line min_width="<<min_line_width;

    return (max_line_width > min_line_width ? max_line_width:min_line_width );
}

void CListDailyPlan::Init()
{
    plan_widget = 0;
    fillSortModel();
    m_list.setSortingEnabled(true);
    last_colomn_clicked = -1;
    last_row_clicked = -1;
    selectedIdFpl = -1;
    selectedUid = NULL;
    mFixedWidthDelegate = new FixedWidthDelegate(this);
    m_sortModel.SetRowFilterType(CListACSortModel::TYPE_DAILY_PLAN);
    //act_in_model_ = new ActTableModel(ActTableModel::act_in, 0, this);

    model_fpl = new ANOModel(this);
    sqlproxy = new ANOProxyModel(this);
    sqlproxy->setSourceModel(model_fpl);
    sqlproxy->setDynamicSortFilter(true);

    mRowCount          = new QLabel("Total: 9999");
    mRowCount->setMinimumWidth(mRowCount->sizeHint().width());
    mFilterAll         = new QCheckBox(tr("ALL"));
    mFilterAlternative = new QCheckBox(tr("ALTN"));
    mFilterArrival     = new QCheckBox(tr("DEST"));
    mFilterDeparture   = new QCheckBox(tr("ADEP"));



    m_ControlsLayout->addWidget(mRowCount);
    m_ControlsLayout->addSpacing(10);
    m_ControlsLayout->addWidget(mFilterAll);
    m_ControlsLayout->addWidget(mFilterDeparture);
    m_ControlsLayout->addWidget(mFilterArrival);
    m_ControlsLayout->addWidget(mFilterAlternative);

    m_ControlsLayout->addStretch();

    but_actplan = new QPushButton();
    but_actplan->setVisible(false);
    m_ControlsLayout->addWidget(but_actplan);

    but_deactplan = new QPushButton();
    but_deactplan->setVisible(false);
    m_ControlsLayout->addWidget(but_deactplan);

    but_viewplan = new QPushButton();
    but_viewplan->setVisible(false);
    m_ControlsLayout->addWidget(but_viewplan);
    but_editplan = new QPushButton(QObject::tr("PLAN"));
   // but_editplan->setCheckable(true);
    m_ControlsLayout->addWidget(but_editplan);

    ano_button = new QRadioButton(QObject::tr("ANO FPLs"));
    cwp_button = new QRadioButton(QObject::tr("CWP FPLs"));
    cwp_button->setCheckable(true);
    ano_button->setChecked(true);


    m_ControlsLayout->addWidget(ano_button);
    m_ControlsLayout->addWidget(cwp_button);


    ano_button_s = new QRadioButton(QObject::tr("ANO FPLs"));
    cwp_button_s = new QRadioButton(QObject::tr("CWP FPLs"));
    cwp_button_s->setCheckable(true);
    ano_button_s->setCheckable(true);
    m_ControlsLayout_add->addStretch();
    m_ControlsLayout_add->addWidget(ano_button_s);
    m_ControlsLayout_add->addWidget(cwp_button_s);

    ano_button_s->hide();
    cwp_button_s->hide();

    connect(mFilterAll, SIGNAL(toggled(bool)), SLOT(onFilterAllChecked(bool)));
    connect(mFilterAlternative, SIGNAL(toggled(bool)), SLOT(onFilterChanged()));
    connect(mFilterArrival, SIGNAL(toggled(bool)), SLOT(onFilterChanged()));
    connect(mFilterDeparture, SIGNAL(toggled(bool)), SLOT(onFilterChanged()));

    connect(but_actplan, SIGNAL(clicked()), this, SLOT(onButtonActPlan()));
    connect(but_deactplan, SIGNAL(clicked()), this, SLOT(onButtonDeActPlan()));
    connect(but_viewplan, SIGNAL(clicked()), this, SLOT(onButtonViewPlan()));
    connect(but_editplan, SIGNAL(clicked()), this, SLOT(onButtonEditPlan()));

    connect(&m_list, SIGNAL(scrollChanged(bool)), SLOT(onScrollChanged(bool)));
    connect(&m_list, SIGNAL(mouseEvent(const QModelIndex&, QMouseEvent*)), this, SLOT(OnMouseEvent(const QModelIndex&, QMouseEvent*)));
    connect(m_list.horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onHeaderClicked(int)));
    connect(m_list.selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,
            SLOT(OnChangeSelect(const QItemSelection &, const QItemSelection &)));

    connect(ano_button, SIGNAL(toggled(bool)), this, SLOT(onANOToggle(bool)));
    connect(cwp_button, SIGNAL(toggled(bool)), this, SLOT(onCWPToggle(bool)));
    connect(ano_button_s, SIGNAL(toggled(bool)), this, SLOT(onANOToggle(bool)));
    connect(cwp_button_s, SIGNAL(toggled(bool)), this, SLOT(onCWPToggle(bool)));

    timerAutoActFpl = new QTimer(this);
    connect(timerAutoActFpl, SIGNAL(timeout()), this, SLOT(OnAutoActFpl()));
    timerAutoActFpl->start(60000); // 5 sec

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(_OnTimer()));
    //timer->start(60000); // 1 min
    connect(DatabaseSubscriber::instance().subscribe("fpl"), SIGNAL(notification()), SLOT(OnTimerRefreshFpl()));

    timerRefreshFpl = new QTimer(this);

    Translate();
    UpdateViewData();


    m_list.setContextMenuPolicy(Qt::NoContextMenu);
    RefreshFpl();
    onFilterChanged();

#ifdef _MSC_VER
    m_listView.append(this);
#endif

}

void CListDailyPlan::Translate()
{
    but_actplan->setText(tr("ACT"));
    but_viewplan->setText(tr("VIEW"));
    //but_editplan->setText(tr("EDIT"));
    but_editplan->setText(tr("VIEW"));
    but_deactplan->setText(tr("DEACT"));
    SetMinimumWidthForTable(CalcMinWidth());
    SizeToContents();

}

int CListDailyPlan::InsertRow(const QSharedPointer<CAirplane> &airplane)
{
    Q_UNUSED(airplane)
    return -1;
}

void CListDailyPlan::ModifyRow(int row, const QSharedPointer<CAirplane> &airplane)
{
    Q_UNUSED(airplane)
}

bool CListDailyPlan::IsCanDeleteRow(int row, const QSharedPointer<CAirplane> &airplane)
{
    Q_UNUSED(airplane)
    return false;
}

void CListDailyPlan::TimerSlot()
{
}

void CListDailyPlan::onFilterAllChecked(bool checked)
{
    if (checked)
    {
        mFilterAlternative->setChecked(false);
        mFilterArrival->setChecked(false);
        mFilterDeparture->setChecked(false);
    }

    mFilterAll->setEnabled(!checked);
}

void CListDailyPlan::onFilterChanged()
{
    mFilterAll->setChecked(!(mFilterAlternative->isChecked() || mFilterArrival->isChecked() || mFilterDeparture->isChecked()));

    if (m_list.horizontalHeader()->sectionSize(CListACModel::ALTN) > 0)
        sqlproxy->setFilterAlternative(mFilterAlternative->isChecked());

    if (m_list.horizontalHeader()->sectionSize(CListACModel::DEST) > 0)
        sqlproxy->setFilterArrival(mFilterArrival->isChecked());

    if (m_list.horizontalHeader()->sectionSize(CListACModel::ADEP) > 0)
        sqlproxy->setFilterDeparture(mFilterDeparture->isChecked());

    sqlproxy->refresh();

    mRowCount->setText(tr("Total: %1").arg(sqlproxy->rowCount()));
    SetMinimumWidthForTable(CalcMinWidth());
    SizeToContents();
}

void CListDailyPlan::onButtonEditPlan()
{

    //QModelIndex index = m_sortModel.index(last_row_clicked,last_colomn_clicked,QModelIndex());

    //if (master()->getRedisDB() && index.isValid())
    //{
    //    sfpl_ = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
    //    uid = sfpl_->GetUid();

     //   plan_widget = new PlanEditor(1,uid,_pParent);
    if(!selectedUid.isNull())
    {
        plan_widget = new PlanEditor(1,selectedUid,_pParent);
        PlanWidgetShow();
    }
}

void CListDailyPlan::onButtonViewPlan()
{
    if(selectedIdFpl != -1)
    {
        plan_widget = new PlanEditor(selectedIdFpl, lst_orcamrange, _pParent);
        PlanWidgetShow();
    }
}

void CListDailyPlan::onButtonActPlan()
{
        if(!indexProxy.isValid())
            return;
        QList<Act> act_ins;

        mFpl.setAircraftId(model_fpl->record(indexProxy.row()).value("aircraft_id").toString());
        mFpl.setDepartureTime(model_fpl->record(indexProxy.row()).value("eet").toTime());
        mFpl.setDepartureAerodrome(model_fpl->record(indexProxy.row()).value("adep").toString());
        mFpl.setDestAerodrome(model_fpl->record(indexProxy.row()).value("ades").toString());
        mFpl.setRoute(model_fpl->record(indexProxy.row()).value("route").toString());
        mFpl.setFlightRules(model_fpl->record(indexProxy.row()).value("frules").toString());
        mFpl.setFlightType(model_fpl->record(indexProxy.row()).value("tflight").toString());
        mFpl.setTurbulenceCategory(model_fpl->record(indexProxy.row()).value("wtc").toString());
        mFpl.setLit(model_fpl->record(indexProxy.row()).value("lit").toString());
        mFpl.setSpeed(model_fpl->record(indexProxy.row()).value("speed").toString());
        mFpl.setLevel(model_fpl->record(indexProxy.row()).value("flevel").toString());
        mFpl.setAircraftType(model_fpl->record(indexProxy.row()).value("taircraft").toString());
        mFpl.setAltDestinationAerodrome(model_fpl->record(indexProxy.row()).value("alt1").toString());
        mFpl.setExFPL(model_fpl->record(indexProxy.row()).value("exfpl").toString());
        mFpl.setETD(model_fpl->record(indexProxy.row()).value("t").toTime());
        mFpl.setETA(model_fpl->record(indexProxy.row()).value("eet").toTime());
        mFpl.setEquipment(model_fpl->record(indexProxy.row()).value("equipment").toString());

        QString eq;
        if(mFpl.equipment().split("/").front().contains("W"))
            eq += "w";
        if(mFpl.equipment().split("/").front().contains("Y"))
            eq += "y";

        if (act_in_model_)
            act_in_model_->deleteLater();

        ActWidget *widget = act_in_widget_;
        QString airpt = networkSettings()->value("meteo/airport").toString();

        QStringList codes;
        codes.clear();

        if (master()) {
            CView *view = dynamic_cast<CView*>(master()->GetView().data());
            if (view) {
                foreach (const QSharedPointer<CAirplane> &airplane, view->GetAirplanes().values()) {
                    if (airplane->GetACID() == mFpl.aircarftId())
                        codes.append(airplane->GetMode3AReplyToString());
                }
            }
        }
        if(!airpt.isEmpty() && mFpl.departureAerodrome() == airpt) {
            act_in_model_ = new ActTableModel(ActTableModel::act_dep, 0, this);
            act_in_widget_ = new ActWidget(tr("ACT DEP"), act_in_model_, this);
            QString code;
            if (codes.size() == 1)
                code = codes.first();
            else code = FreeCodeOrcam();

            Act act = {eq, mFpl.aircarftId(), code, mFpl.departureAerodrome(), mFpl.ETD(), 0, mFpl.departureAerodrome(), mFpl.destinationAerodrome()};
            act_ins.push_back(act);
        }
        else {
            QString code;
            if (codes.size() == 1)
                code = codes.first();

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

            act_in_model_ = new ActTableModel(ActTableModel::act_in, 0, this,getFir2Points());
            act_in_widget_ = new ActWidget(tr("ACT IN"), act_in_model_, this);
            Act act = {eq, mFpl.aircarftId(), code, cop ,mFpl.departureTime(), NULL, mFpl.departureAerodrome(), mFpl.destinationAerodrome(), NULL, NULL};
            //Act act = {eq, mFpl.aircarftId(), code ,"" ,mFpl.departureTime(), NULL, mFpl.departureAerodrome(), mFpl.destinationAerodrome(), NULL, NULL};
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
        act_in_widget_->setItemDelegateForColumn(0, eq_delegate);

        /*QSharedPointer<QValidator> equip_validator(new QRegExpValidator(QRegExp("\\w{0,5}"), 0));
        LineEditDelegate *equip_delegate = new LineEditDelegate(equip_validator, act_in_widget_);
        act_in_widget_->setItemDelegateForColumn(0, equip_delegate);*/

        QSharedPointer<QValidator> acid_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,7}"), 0));
        QAbstractItemDelegate *acid_delegate = new LineEditDelegate(acid_validator, act_in_widget_);
        act_in_widget_->setItemDelegateForColumn(1, acid_delegate);

        QSharedPointer<QValidator> code_validator(new QRegExpValidator(QRegExp("[0-7]{4}"), 0));
        QAbstractItemDelegate *code_delegate = new LineEditDelegate(code_validator, act_in_widget_);
        act_in_widget_->setItemDelegateForColumn(2, code_delegate);

        if(!airpt.isEmpty() && mFpl.departureAerodrome() == airpt) {
            QAbstractItemDelegate *eto_delegate = new TimeDelegate(act_in_widget_);
            act_in_widget_->setItemDelegateForColumn(3, eto_delegate);

            QSharedPointer<QValidator> adep_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,4}"), 0));
            LineEditDelegate *adep_delegate = new LineEditDelegate(adep_validator, act_in_widget_);
            act_in_widget_->setItemDelegateForColumn(4, adep_delegate);

            QSharedPointer<QValidator> ades_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,4}"), 0));
            LineEditDelegate *ades_delegate = new LineEditDelegate(ades_validator, act_in_widget_);
            act_in_widget_->setItemDelegateForColumn(5, ades_delegate);
        }
        else {
            QSharedPointer<QValidator> cop_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,5}"), 0));
            LineEditDelegate *cop_delegate = new LineEditDelegate(cop_validator, act_in_widget_);
            cop_delegate->setCompleter(getFir2Points());
            act_in_widget_->setItemDelegateForColumn(3, cop_delegate);

            QAbstractItemDelegate *eto_delegate = new TimeDelegate(act_in_widget_);
            act_in_widget_->setItemDelegateForColumn(4, eto_delegate);

            QAbstractItemDelegate *pel_delegate = new FlightLevelDelegate(act_in_widget_, this);
            act_in_widget_->setItemDelegateForColumn(5, pel_delegate);

            QSharedPointer<QValidator> adep_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,4}"), 0));
            LineEditDelegate *adep_delegate = new LineEditDelegate(adep_validator, act_in_widget_);
            act_in_widget_->setItemDelegateForColumn(6, adep_delegate);

            QSharedPointer<QValidator> ades_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,4}"), 0));
            LineEditDelegate *ades_delegate = new LineEditDelegate(ades_validator, act_in_widget_);
            act_in_widget_->setItemDelegateForColumn(7, ades_delegate);
        }
        connect(act_in_widget_, SIGNAL(act()), this, SLOT(onAct()));

        AdaptateRoute(mFpl.route());

        if (widget)
            act_in_widget_->move(widget->pos());

        delete widget;

        act_in_widget_->show();
}

void CListDailyPlan::onButtonDeActPlan()
{
    if (QMessageBox::warning(this, windowTitle(), tr("Are you sure want to delete?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    QUuid uid;
    if (master()->getRedisDB())
    {
        sfpl_ = QSharedPointer<CSFPL>(new CSFPL);
        if(!selectedUid.isNull())
        {
            bool ok = master()->getRedisDB()->loadSFPL(sfpl_,selectedUid);
            if(ok)
             if(master()->getRedisDB()->deleteSFPL(sfpl_));
				logOperation(tr("Delete plan: ACID = '%1'").arg(sfpl_->GetACID()));
            //master()->getRedisDB()->deleteOrcam(sfpl_->GetCODEtoString());
        }
     }
    RefreshCwp();
}

void CListDailyPlan::OnClickItemEdit(const QModelIndex &index, QString str)
{
    if(index.isValid()){
        int i = index.column();
        //QString str = m_list.model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();

        if(cwp_button->isChecked() || cwp_button_s->isChecked()){
            if(str == "SID" || str == "STAR")
                if (master()->getRedisDB())
                {
                    QModelIndex sourceIndex = sourceIndex_MouseEvent;// m_sortModel.mapToSource(index);
                    //QString acid = m_model.GetItemData(sourceIndex).GetSFPL()->GetACID();
                    QString acid = sfpl_MouseEvent->GetACID();
                    CDailyDlgSID dlg(acid, str, this);
                    dlg.setWindowFlags(Qt::Dialog |Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);
                    int result = dlg.exec();
                    if(result == QDialog::Accepted)
                    {
                        CListACData &item = m_model.GetItemData(sourceIndex);
                        sfpl_ = sfpl_MouseEvent;//item.GetSFPL();
                        QString sid = dlg.getValue();
                        if(str == "SID")
                        {
                            sfpl_->SetSID(sid);
                            if(sfpl_->GetSTAR().size() != 0)
                               sfpl_->SetSTAR("");
                        }
                        if(str == "STAR")
                        {
                            sfpl_->SetSTAR(sid);
                            if(sfpl_->GetSID().size() != 0)
                               sfpl_->SetSID("");
                        }
                        m_model.setData(index,sid);

                        if (item.GetAirplane())
                        {
                            item.GetAirplane()->UpdateFPLFrag(sfpl_->GetFPLFrag()->saveState());
                            item.GetAirplane()->CalculateSectors(sfpl_);
                        }
                        else
                        {
                            sfpl_->CreateFPLFrag(sfpl_);
                            sfpl_->GetFPLFrag()->restoreState(sfpl_->GetFPLFrag()->saveState());
                            CAirplane::CalculateSectors(sfpl_, 0);
                        }
                        logOperation(tr("%1: SID/STAR changed, new sectors list: %2").arg(sfpl_->GetACID()).arg(sfpl_->GetAllSectors()));

                        bool ok = master()->getRedisDB()->saveSFPL(sfpl_);
                        if (!ok) {
                            QMessageBox::critical(0, tr("Error"), tr("Error saving FPL"));
                            return;
                        }
                    }

                }
            if(str == "ATA")
                if (master()->getRedisDB()){
                    QModelIndex sourceIndex = m_sortModel.mapToSource(index);
                    QString acid = m_model.GetItemData(sourceIndex).GetSFPL()->GetACID();
                    CDailyDlgTime dlg(acid, str, this);
                    dlg.setWindowFlags(Qt::Dialog |Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);
                    int result = dlg.exec();
                    if(result == QDialog::Accepted)
                    {
                        sfpl_ = m_model.GetItemData(sourceIndex).GetSFPL();
                        sfpl_->SetETA(dlg.getValue());
                        m_model.setData(index,dlg.getValue());
                        bool ok = master()->getRedisDB()->saveSFPL(sfpl_);
                        if (!ok) {
                            QMessageBox::critical(0, tr("Error"), tr("Error saving FPL"));
                            return;
                        }
                    }
             }
        }
    }
}

void CListDailyPlan::OnDblClickItemEdit(const QModelIndex &index)
{
    if(index.isValid())
    {
        int i = index.column();
        QString str = m_list.model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
        //if(str == "ACID")
        //{
            last_colomn_clicked = i;
            last_row_clicked = index.row();

            if(cwp_button->isChecked() || cwp_button_s->isChecked())
            {
                if (master()->getRedisDB())
                {
                    //sfpl_ = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
                    //uid = sfpl_->GetUid();
                    if(sfpl_MouseEvent)
                       uid = sfpl_MouseEvent->GetUid();
                    plan_widget = new PlanEditor(i,uid,_pParent);
                    PlanWidgetShow();
                }
            }
            if(ano_button->isChecked() || ano_button_s->isChecked())
            {
                plan_widget = new PlanEditor(selectedIdFpl, lst_orcamrange, _pParent);
                PlanWidgetShow();
            }
        //}
    }
}

void CListDailyPlan::OnMouseEvent(const QModelIndex &index, QMouseEvent *event)
{
    if(cwp_button->isChecked() || cwp_button_s->isChecked())
    {
        if(event->button() == Qt::RightButton)
        {
            indexMouseEvent = index;
            if(index.isValid())
                if (master()->getRedisDB())
                {
                    sfpl_MouseEvent = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
                    sourceIndex_MouseEvent = m_sortModel.mapToSource(index);
                }
            QMenu *menu = new QMenu(this);
            QAction * editDevice = new QAction(trUtf8("View"), this);
            QAction * routeDevice = new QAction(trUtf8("Route"), this);
            QAction * sidDevice = new QAction(trUtf8("SID"), this);
            QAction * starDevice = new QAction(trUtf8("STAR"), this);

            connect(editDevice, SIGNAL(triggered()), this, SLOT(OnEditDevice()));
            connect(routeDevice, SIGNAL(triggered()), this, SLOT(OnRouteDevice()));
            connect(sidDevice, SIGNAL(triggered()), this, SLOT(OnSidDevice()));
            connect(starDevice, SIGNAL(triggered()), this, SLOT(OnStarDevice()));

            menu->addAction(editDevice);
            menu->addAction(routeDevice);
            menu->addAction(sidDevice);
            menu->addAction(starDevice);
            menu->exec(m_list.viewport()->mapToGlobal(event->pos()));
        }
    }
}

void CListDailyPlan::OnEditDevice()
{
    OnDblClickItemEdit(indexMouseEvent);
}

void CListDailyPlan::OnRouteDevice()
{
    CListAC::OnShowRouteCost(sfpl_MouseEvent);
}

void CListDailyPlan::OnSidDevice()
{
    OnClickItemEdit(indexMouseEvent, "SID");
}

void CListDailyPlan::OnStarDevice()
{
    OnClickItemEdit(indexMouseEvent, "STAR");
}

void CListDailyPlan::OnChangeSelect(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected.isEmpty())
        return;

    QModelIndex index = selected.indexes().at(1);
    last_colomn_clicked = index.column();
    last_row_clicked = index.row();
    QModelIndexList selectionProxy = sqlproxy->mapSelectionToSource(selected).indexes();
    if (!selectionProxy.isEmpty())
    {
        indexProxy = selectionProxy.at(1);
        if (ano_button->isChecked() || ano_button_s->isChecked())
            selectedIdFpl = model_fpl->record(indexProxy.row()).value("id").toInt();
    }
    if (cwp_button->isChecked() || cwp_button_s->isChecked())
    {
        if (master()->getRedisDB())
        {
            if (m_sortModel.mapToSource(index).isValid())
            {
                sfpl_ = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
                selectedUid = sfpl_->GetUid();
            }
        }
    }
}

void CListDailyPlan::setFilterVisible(bool visible)
{
    mRowCount->setVisible(visible);
    mFilterAll->setVisible(visible);
    mFilterDeparture->setVisible(visible);
    mFilterArrival->setVisible(visible);
    mFilterAlternative->setVisible(visible);
}

void CListDailyPlan::CheckIndexNavigation(QModelIndex & index, int row)
{
    if(index.isValid())
    {
        //QString str = m_list.model()->headerData(last_colomn_clicked,Qt::Horizontal,Qt::DisplayRole).toString();
        //if(str == "ACID")
        //{
        //    last_row_clicked = row;
        CurrentIndex(index);
            if(cwp_button->isChecked() || cwp_button_s->isChecked())
            {
                if (master()->getRedisDB())
                {
                    sfpl_ = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
                    uid = sfpl_->GetUid();
                    plan_widget->SetUid(uid);
                    plan_widget->SetNewData();
                }
            }
            if(ano_button->isChecked() || ano_button_s->isChecked())
            {
                //int id = model_fpl->record(last_row_clicked).value("id").toInt();
                int id = model_fpl->record(index.row()).value("id").toInt();
                plan_widget->SetNewData(id);
            }
        //}
    }
}

void CListDailyPlan::onFplPlanFirstClicked()
{
    if(plan_widget)
    {
        QModelIndex index = m_list.model()->index(0,last_colomn_clicked);
        CheckIndexNavigation(index, 0);
    }
}
void CListDailyPlan::onFplPlanPrevClicked()
{
    if(plan_widget)
    {
        QModelIndex index = m_list.model()->index(last_row_clicked-1,last_colomn_clicked);
        CheckIndexNavigation(index, last_row_clicked-1);
    }
}
void CListDailyPlan::onFplPlanNextClicked()
{
    if(plan_widget)
    {
        QModelIndex index = m_list.model()->index(last_row_clicked+1,last_colomn_clicked);
        CheckIndexNavigation(index, last_row_clicked+1);
    }
}
void CListDailyPlan::onFplPlanLastClicked()
{
    if(plan_widget)
    {
        int row_count = m_list.model()->rowCount();
        QModelIndex index = m_list.model()->index(row_count-1,last_colomn_clicked);
        CheckIndexNavigation(index, row_count-1);
    }
}
void CListDailyPlan::onANOToggle(bool checked)
{
    if (checked)
    {
        RefreshFpl();
    }
}

void CListDailyPlan::onCWPToggle(bool checked)
{
    if (checked)
    {
        RefreshCwp();
        SetMinimumWidthForTable(CalcMinWidth());
        SizeToContents();
    }
}

void CListDailyPlan::PlanWidgetShow()
{
    //connect(plan_widget, SIGNAL(onFirstFPLClicked()), this, SLOT(onFplPlanFirstClicked()));
    //connect(plan_widget, SIGNAL(onPrevFPLClicked()), this, SLOT(onFplPlanPrevClicked()));
    //connect(plan_widget, SIGNAL(onNextFPLClicked()), this, SLOT(onFplPlanNextClicked()));
    //connect(plan_widget, SIGNAL(onLastFPLClicked()), this, SLOT(onFplPlanLastClicked()));
    plan_widget->show();
}

void CListDailyPlan:: onAct()
{
    Act act_in = act_in_model_->act(0);

    QUuid uid;
    QList<QSharedPointer<CSFPL> > list;
    if (master()->getRedisDB())
    {
        sfpl_ = QSharedPointer<CSFPL>(new CSFPL);
        bool ok = master()->getRedisDB()->loadSFPLs(list);
        if(ok)
        {

            QList<QSharedPointer<CSFPL> >::iterator itList = list.begin();
            for(; itList != list.end(); ++itList)
            {
                  QString coderedis = (*itList)->GetCODEtoString();
                  QString acidredis = (*itList)->GetACID();
                  if(act_in.code == coderedis && act_in.acid == acidredis)
                  {
                    uid = (*itList)->GetUid();
                    master()->getRedisDB()->loadSFPL(sfpl_,uid);
                    break;
                  }
            }
        }

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
    sfpl_->SetStatus(mFpl.exFPL());
    sfpl_->SetETA(mFpl.ETA());
    sfpl_->SetFPLEqf1(mFpl.equipment().split("/").front());
    sfpl_->SetFPLEqf2(mFpl.equipment().split("/").last());

    if (act_in_model_->type() == ActTableModel::act_dep)
    {
        sfpl_->SetETOin(mFpl.departureTime());
        sfpl_->SetETD(act_in.eto);
    }
    else
    {
        sfpl_->SetETOin(act_in.eto);
        sfpl_->SetETD(mFpl.ETD());
    }

    sfpl_->CreateFPLFrag(sfpl_);
    CAirplane::CalculateSectors(sfpl_, 0);
    logOperation(tr("%1: ACT IN, sectors list: %2").arg(sfpl_->GetACID()).arg(sfpl_->GetAllSectors()));

    QString airpt = networkSettings()->value("meteo/airport").toString();
    if(!airpt.isEmpty() && mFpl.departureAerodrome() == airpt)
        setInSector2(sfpl_);
    else
        setInSector(sfpl_);

#ifdef QT_DEBUG
    if (!act_in.cs.isEmpty())
        sfpl_->SetCurSector(act_in.cs);
    if (!act_in.ns.isEmpty())
        sfpl_->SetNextSector(act_in.ns);
#endif
        cod = act_in.code;
        ok = master()->getRedisDB()->saveSFPL(sfpl_);

        if (!ok)
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
        QTimer::singleShot(100, this, SLOT(afterSleep()));
    }
}

void CListDailyPlan::setInSector(QSharedPointer<CSFPL> &sfpl)
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

void CListDailyPlan::setInSector2(QSharedPointer<CSFPL> &sfpl)
{
    // calculate sector for airport
    if(sfpl)
    {
        QMap<QPair<int, int>, QSharedPointer<Scene> > mapSector;
        if(master() && master()->GetView())
        {
            QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
            if(view && view->GetMainFIR())
            {
                Scene::TAirportsMap::const_iterator itAirport = view->GetMainFIR()->GetAirports().constFind(sfpl->GetDepAerodrome());
                if(itAirport != view->GetMainFIR()->GetAirports().constEnd())
                {
                    Scene::TSceneList::const_iterator itSector = view->GetMainFIR()->GetSectorList().constBegin();
                    for(; itSector != view->GetMainFIR()->GetSectorList().constEnd(); ++itSector)
                    {
                        if((*itSector)->IsIncluded((*itAirport)->GetCoord()))
                        {
                            mapSector.insert(QPair<int, int>((*itSector)->GetLowLimit(), (*itSector)->GetUpLimit()), *itSector);
                        }
                    }
                }
            }
        }
        if(mapSector.size())
        {
            //sfpl->SetInSector((*mapSector.constBegin())->GetShortName());
            sfpl->SetNextSector((*mapSector.constBegin())->GetShortName());
        }
    }
}

void CListDailyPlan::_OnTimer()
{
    if(ano_button->isChecked() || ano_button_s->isChecked())
        RefreshFpl();
}

void CListDailyPlan::OnAutoActFpl()
{
    QString signAutoAct = networkSettings()->value("FPL_handler/sign_auto_act", "").toString();
    if(signAutoAct != "true")
        return;

    for( int i = 0; i < model_fpl->rowCount(); i++)
    {
        QDate date = model_fpl->record(i).value("time").toDate();
        QTime time = model_fpl->record(i).value("t").toTime();
        QDateTime dt = QDateTime(date, time);
        qint64 fpl_dtMS =  dt.toMSecsSinceEpoch();
        qint64 cur_dtMS = QDateTime::currentMSecsSinceEpoch();
        qint64 parametr_dtMS = networkSettings()->value("FPL_handler/time_before_act").toInt() * 60 * 1000;
        if(abs(fpl_dtMS - cur_dtMS) <= parametr_dtMS)
        {
            QString airpt = networkSettings()->value("meteo/airport").toString();
            if(fpl_dtMS > cur_dtMS && model_fpl->record(i).value("adep").toString() == airpt)
            {
                QList<Act> act_ins;
                mFpl.setAircraftId(model_fpl->record(i).value("aircraft_id").toString());
                mFpl.setDepartureTime(model_fpl->record(i).value("eet").toTime());
                mFpl.setDepartureAerodrome(model_fpl->record(i).value("adep").toString());
                mFpl.setDestAerodrome(model_fpl->record(i).value("ades").toString());
                mFpl.setRoute(model_fpl->record(i).value("route").toString());
                mFpl.setFlightRules(model_fpl->record(i).value("frules").toString());
                mFpl.setFlightType(model_fpl->record(i).value("tflight").toString());
                mFpl.setTurbulenceCategory(model_fpl->record(i).value("wtc").toString());
                mFpl.setLit(model_fpl->record(i).value("lit").toString());
                mFpl.setSpeed(model_fpl->record(i).value("speed").toString());
                mFpl.setLevel(model_fpl->record(i).value("flevel").toString());
                mFpl.setAircraftType(model_fpl->record(i).value("taircraft").toString());
                mFpl.setAltDestinationAerodrome(model_fpl->record(i).value("alt1").toString());
                mFpl.setExFPL(model_fpl->record(i).value("exfpl").toString());
                mFpl.setETD(model_fpl->record(i).value("t").toTime());
                mFpl.setETA(model_fpl->record(i).value("eet").toTime());
                mFpl.setEquipment(model_fpl->record(i).value("equipment").toString());

                QString eq;
                if(mFpl.equipment().split("/").front().contains("W"))
                    eq += "w";
                if(mFpl.equipment().split("/").front().contains("Y"))
                    eq += "y";

                //if (act_in_model_)
                //    act_in_model_->deleteLater();

                QStringList codes;
                codes.clear();

                if (master()) {
                    CView *view = dynamic_cast<CView*>(master()->GetView().data());
                    if (view) {
                        foreach (const QSharedPointer<CAirplane> &airplane, view->GetAirplanes().values()) {
                            if (airplane->GetACID() == mFpl.aircarftId())
                                codes.append(airplane->GetMode3AReplyToString());
                        }
                    }
                }

                Act act;

                if(!airpt.isEmpty() && mFpl.departureAerodrome() == airpt) {
                    //act_in_model_ = new ActTableModel(ActTableModel::act_dep, 0, this);
                    QString code;
                    if (codes.size() == 1)
                        code = codes.first();
                    else code = FreeCodeOrcam();

                    act = {eq, mFpl.aircarftId(), code, mFpl.departureAerodrome(), mFpl.ETD(), 0, mFpl.departureAerodrome(), mFpl.destinationAerodrome()};
                    act_ins.push_back(act);
                }
                else {
                    QString code;
                    if (codes.size() == 1)
                        code = codes.first();

                    act_in_model_ = new ActTableModel(ActTableModel::act_in, 0, this,getFir2Points());
                    act = {eq, mFpl.aircarftId(), code ,"" ,mFpl.departureTime(), NULL, mFpl.departureAerodrome(), mFpl.destinationAerodrome(), NULL, NULL};
                    act_ins.push_back(act);
                }
                //act_in_model_->setActs(act_ins);

                AdaptateRoute(mFpl.route());

                // из fpl получаем sfpl
                Act act_in = act; //act_in_model_->act(0);

                QUuid uid;
                QList<QSharedPointer<CSFPL> > list;
                if (master()->getRedisDB())
                {
                    bool signIsPlanAct = false;
                    sfpl_ = QSharedPointer<CSFPL>(new CSFPL);
                    bool ok = master()->getRedisDB()->loadSFPLs(list);
                    if(ok)
                    {

                        QList<QSharedPointer<CSFPL> >::iterator itList = list.begin();
                        for(; itList != list.end(); ++itList)
                        {
                            QString coderedis = (*itList)->GetCODEtoString();
                            QString acidredis = (*itList)->GetACID();
                            if(act_in.acid == acidredis)
                                signIsPlanAct = true;
                            if(act_in.code == coderedis && act_in.acid == acidredis)
                            {
                                uid = (*itList)->GetUid();
                                master()->getRedisDB()->loadSFPL(sfpl_,uid);
                                break;
                            }
                        }
                    }
                    if(signIsPlanAct)
                        break;
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
                    sfpl_->SetStatus(mFpl.exFPL());
                    sfpl_->SetETA(mFpl.ETA());
                    sfpl_->SetFPLEqf1(mFpl.equipment().split("/").front());
                    sfpl_->SetFPLEqf2(mFpl.equipment().split("/").last());

                    if(act_in_model_)
                        if (act_in_model_->type() == ActTableModel::act_dep)
                        {
                            sfpl_->SetETOin(mFpl.departureTime());
                            sfpl_->SetETD(act_in.eto);
                        }
                        else
                        {
                            sfpl_->SetETOin(act_in.eto);
                            sfpl_->SetETD(mFpl.ETD());
                        }

                    sfpl_->CreateFPLFrag(sfpl_);
                    CAirplane::CalculateSectors(sfpl_, 0);
                    logOperation(tr("%1: ACT IN, sectors list: %2").arg(sfpl_->GetACID()).arg(sfpl_->GetAllSectors()));

                    QString airpt = networkSettings()->value("meteo/airport").toString();
                    if(!airpt.isEmpty() && mFpl.departureAerodrome() == airpt)
                        setInSector2(sfpl_);
                    else
                        setInSector(sfpl_);

#ifdef QT_DEBUG
if (!act_in.cs.isEmpty())
    sfpl_->SetCurSector(act_in.cs);
if (!act_in.ns.isEmpty())
    sfpl_->SetNextSector(act_in.ns);
#endif
                    cod = act_in.code;
                    ok = master()->getRedisDB()->saveSFPL(sfpl_);

                    if (!ok)
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
                    //QTimer::singleShot(100, this, SLOT(afterSleep()));
                    afterSleep();
                }
            }
        }

    }
}

void CListDailyPlan::OnThread()
{
    timerAutoActFpl = new QTimer(this);
    connect(timerAutoActFpl, SIGNAL(timeout()), this, SLOT(OnAutoActFpl()), Qt::DirectConnection);
    timerAutoActFpl->start(60000); // 5 sec
}

void CListDailyPlan::OnTimerRefreshFpl()
{
    if(lastTimeRefresh.isNull())
    {
        lastTimeRefresh = QDateTime::currentDateTime().addSecs(-11);
    }
    QDateTime lastTime = lastTimeRefresh;
    QDateTime currentTime = QDateTime::currentDateTime();

    if(currentTime.toMSecsSinceEpoch() - lastTimeRefresh.toMSecsSinceEpoch() > 10000)
    {
        if(!timerRefreshFpl->isActive())
        {
            RefreshFpl();
        }
    }
    else
    {
        if(!timerRefreshFpl->isActive())
        {
            QTimer::singleShot(10000, this, SLOT(RefreshFpl()));
            timerRefreshFpl->start(1000);
        }
    }
}

void CListDailyPlan::RefreshFpl()
{
    lastTimeRefresh = QDateTime::currentDateTime();
    timerRefreshFpl->stop();
    if(!ano_button->isChecked() && !ano_button_s->isChecked())
        return;

    if (!m_list.isScrolled())
        setFilterVisible(true);

    but_actplan->setVisible(true);
    but_viewplan->setVisible(true);
    but_editplan->hide();
    but_deactplan->hide();
    if(DBConnectionsManager::instance().getConnectionStatus() == 1)
    //if (DBConnectionsManager::instance().isConnected())
    {
        m_list.setModel(0);

        model_fpl->setQuery("SELECT id,aircraft_id,time::date,adep,time::time as t,"
                            "ades,eet,alt1,lit,taircraft,exfpl,frules,tflight,wtc,"
                            "speed,flevel,route, equipment FROM fpl WHERE time::date >= NOW()::date - INTERVAL '1 days' "
                            "AND time::date <= NOW()::date + INTERVAL '1 days'");

        m_list.setModel(sqlproxy);
        m_list.setItemDelegateForColumn(CListACModel::STATUS, mFixedWidthDelegate);

        setListType("DAILY_PLAN_ANO");
        UpdateViewData();

        m_list.sortByColumn(clickColumn);

        connect(m_list.selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,
                SLOT(OnChangeSelect(const QItemSelection &, const QItemSelection &)));

        QModelIndex index1, ind;

        for (int i = 0; i < model_fpl->rowCount(); ++i)
        {
            int id = model_fpl->record(i).value("id").toInt();
            if(id == selectedIdFpl)
            {
                index1 = model_fpl->index(i,1,QModelIndex());
                ind = sqlproxy->mapFromSource(index1);
                break;
            }

        }
        if(ind.isValid())
            CurrentIndex(ind);
        mRowCount->setText(tr("Total: %1").arg(sqlproxy->rowCount()));
        SetMinimumWidthForTable(CalcMinWidth());
        SizeToContents();
    }
}

void CListDailyPlan::RefreshCwp()
{
    Refresh();
    m_list.setModel(0);
    last_colomn_clicked = -1;
    last_row_clicked = -1;
    setFilterVisible(false);
    but_editplan->setVisible(true);
    but_deactplan->setVisible(true);
    but_actplan->hide();
    but_viewplan->hide();
    m_sortModel.setSourceModel(&m_model);
    m_list.setModel(&m_sortModel);
    m_list.setItemDelegateForColumn(CListACModel::STATUS, 0);

    connect(m_list.selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,
            SLOT(OnChangeSelect(const QItemSelection &, const QItemSelection &)));

    setListType("DAILY_PLAN");
    UpdateViewData();

    QModelIndex index1, ind;

    for (int i = 0; i < m_model.GetModelData().size(); ++i)
    {
        CListACData& itemData = m_model.GetItemData(i);

        if(!selectedUid.isNull())
            if(itemData.GetSFPL()->GetUid() == selectedUid)
            {
                index1 = m_model.index(i,1,QModelIndex());
                ind = m_sortModel.mapFromSource(index1);
                break;
            }
    }
    if(ind.isValid())
        CurrentIndex(ind);

}

void CListDailyPlan::CurrentIndex(QModelIndex ind)
{
    m_list.selectionModel()->select (
        QItemSelection (
                    m_list.model()->index(ind.row(), 0),
            m_list.model()->index(ind.row(), m_list.model()->columnCount()-1)),
        QItemSelectionModel::Select);

}

void CListDailyPlan::AdaptateRoute(QString route)
{
    AFTNMessageProcessor mProcessor;
    mProcessor.setValidateRoute(false);
    mProcessor.setSaveOnlyFIR(true);
    const QString fir_names = networkSettings()->value("FPL_handler/fir", "").toString();
    mProcessor.setFIRName(fir_names);
    AFTNMessageProcessor::AftnMessageErrors errorCodes;
    newRoute = mProcessor.cutFIRRoute2(route, errorCodes, false);

}
void CListDailyPlan::onHeaderClicked(int n)
{
    clickColumn = n;
    if(ano_button->isChecked() || ano_button_s->isChecked())
    {
        if(n == 35 || n == 36)
        {
            sqlproxy->sort(n,m_list.horizontalHeader()->sortIndicatorOrder());
            sqlproxy->sort(39,m_list.horizontalHeader()->sortIndicatorOrder());
            m_list.horizontalHeader()->setSortIndicatorShown(true);
        }
        else
        {
            sqlproxy->sort(n,m_list.horizontalHeader()->sortIndicatorOrder());
            m_list.horizontalHeader()->setSortIndicatorShown(true);
        }
    }
    if(cwp_button->isChecked() || cwp_button_s->isChecked())
    {
        m_sortModel.sort(n,m_list.horizontalHeader()->sortIndicatorOrder());
        m_list.horizontalHeader()->setSortIndicatorShown(true);
    }
}

void CListDailyPlan::onScrollChanged(bool scrolled)
{
    cwp_button->setVisible(!scrolled);
    ano_button->setVisible(!scrolled);
    cwp_button_s->setVisible(scrolled);
    ano_button_s->setVisible(scrolled);
    if (scrolled)
    {
        if (cwp_button->isChecked())
        {
            cwp_button_s->setChecked(true);
        }
        if (ano_button->isChecked())
        {
            ano_button_s->setChecked(true);
        }
    }
    else
    {
        if (cwp_button_s->isChecked())
        {
            cwp_button->setChecked(true);
        }
        if (ano_button_s->isChecked())
        {
            ano_button->setChecked(true);
        }
    }
    if (ano_button->isChecked() || ano_button_s->isChecked())
    {
        setFilterVisible(!scrolled);
    }
    SetMinimumWidthForTable(CalcMinWidth());
}

QString CListDailyPlan::ReadOrcam()
{
    int minCode = networkSettings()->value("users/ORCAMMinCode", "3000").toInt();
    int maxCode = networkSettings()->value("users/ORCAMMaxCode", "3500").toInt();

    QString code = "";

    for(int i = minCode; i <= maxCode; i++){
        code = QString::number(i);

        QFile file("orcam_list.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return code;
        QTextStream in(&file);
        in.skipWhiteSpace();

        while (!in.atEnd()) {
            QStringList fields = in.readLine().split(' ');

            foreach(QString field, fields){
                if(QString::number(i) == field){
                    code = "";
                    break;
                }
            }

            if(code.isEmpty())
                break;
        }

        if(!code.isEmpty())
        break;
    }

    return code;
}


QString CListDailyPlan::FreeCodeOrcam()
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
void CListDailyPlan::afterSleep()
{
    int min = networkSettings()->value("users/ORCAMLifeTime", 1).toInt();

    QMultiMap<QString, QString> map;
    master()->getRedisDB()->loadOrcams(map);
    QMapIterator <QString, QString> iter(map);
    bool isCodeOrcam = false;
    while (iter.hasNext()){
        iter.next();
        if(cod == iter.key()){
            isCodeOrcam = true;
            break;
        }
    }
    bool okk = true;
    if (!isCodeOrcam)
        okk = master()->getRedisDB()->addOrcam(cod, QDateTime::currentDateTime(), min);
    if (!okk)
        QMessageBox::critical(this, tr("Error"), tr("Error saving ORCAM"));
    else
        logOperation(tr("ORCAM: ( %1 )").arg(cod));

}


