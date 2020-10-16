#include "ListCOST.h"

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

#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>

#include <./front/core/GUIAirplane.h>

CListCost::CListCost(CMaster *pMaster, QWidget *pParent) :
        CListAC(pMaster, pParent), show_model_(0)
        //edit_model_(new ActTableModel(ActTableModel::act_in, calcNextSector, pParent)),
        //act_in_widget_(new ActWidget("ACT IN", edit_model_, pParent))
{
    setListType(QString("COST"));

    fillSortModel();

    m_sortModel.SetRowFilterType(CListACSortModel::TYPE_COST);

    m_ControlsLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));

    but_cost_show = new QPushButton(this);
    m_ControlsLayout->addWidget(but_cost_show);

    connect(but_cost_show, SIGNAL(clicked()), this, SLOT(onButtonCostShow()));
    connect(&m_list, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(OnDblClickItemEdit(const QModelIndex&)));
    connect(&m_list, SIGNAL(clicked(const QModelIndex&)), this, SLOT(OnClicked(const QModelIndex&)));

    Translate();
    UpdateViewData();

    m_list.setContextMenuPolicy(Qt::NoContextMenu);

#ifdef _MSC_VER
    m_listView.append(this);
#endif
}

void CListCost::Translate()
{
    but_cost_show->setText(tr("Route"));
    SizeToContents();
}

int CListCost::InsertRow(const QSharedPointer<CAirplane> &airplane)
{
    Q_UNUSED(airplane)
    return -1;
}

void CListCost::ModifyRow(int row, const QSharedPointer<CAirplane> &airplane)
{
    Q_UNUSED(airplane)
}

bool CListCost::IsCanDeleteRow(int row, const QSharedPointer<CAirplane> &airplane)
{
    Q_UNUSED(airplane)
    return false;
}

void CListCost::TimerSlot()
{
}

void CListCost::onButtonCostShow()
{
    CListAC::OnShowRouteCost(sfpl_);
}

void CListCost::OnDblClickItemEdit(const QModelIndex &index)
{
    if (index.isValid()) {
        if (master()->getRedisDB()) {
            sfpl_ = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();

            if (sfpl_->GetCurSector().isEmpty())
            {
                const QString airport = networkSettings()->value("meteo/airport").toString();
                if(!airport.isEmpty() && sfpl_->GetDepAerodrome() == airport)
                    InitActDep();
                else
                    InitActIn();

                QList<Act> act_ins;
                Act act = {sfpl_->GetEquipment(), sfpl_->GetACID(), sfpl_->GetCODEtoString(), sfpl_->GetCOPin(), sfpl_->GetETOin(),
                           sfpl_->GetCOPinFL(), sfpl_->GetDepAerodrome(), sfpl_->GetDestAerodrome(),
                           sfpl_->GetCurSector(), sfpl_->GetNextSector()};
                act_ins.push_back(act);
                show_model_->setActs(act_ins);
                QSet<int> non_editable_fields;
                non_editable_fields.insert(1);
                show_model_->setNonEditableFields(non_editable_fields);
                act_in_widget_->show();
            }
        }
    }
}

void CListCost::OnClicked(const QModelIndex &index)
{
    if (index.isValid())
        if (master()->getRedisDB())
            sfpl_ = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
}

void CListCost::onAct()
{
    Act act_in = show_model_->act(0);
    //////
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
    //////
    bool createFPLFrag = (sfpl_->GetCOPin() != act_in.cop) || (sfpl_->GetCOPinFL() != FLVal(act_in.pel, true));

    sfpl_->SetEquipment(act_in.eq);
    sfpl_->SetACID(act_in.acid);
    sfpl_->SetCODE(act_in.code.toInt(0, 8));
    sfpl_->SetCOPin(act_in.cop);
    sfpl_->SetETOin(act_in.eto);
    sfpl_->SetCOPinFL(FLVal(act_in.pel, true));
    sfpl_->SetCFL(sfpl_->GetCOPinFL());
    sfpl_->SetDepAerodrome(act_in.adep);
    sfpl_->SetDestAerodrome(act_in.dest);

    setInSector(sfpl_);

#ifdef QT_DEBUG
    if (!act_in.cs.isEmpty())
        sfpl_->SetCurSector(act_in.cs);
    if (!act_in.ns.isEmpty())
        sfpl_->SetNextSector(act_in.ns);
#endif

    if (createFPLFrag)
    {
        sfpl_->CreateFPLFrag(sfpl_);
        CAirplane::CalculateSectors(sfpl_, 0);
        logOperation(tr("%1: COPin/COPinFL changed, new sectors list: %2").arg(sfpl_->GetACID()).arg(sfpl_->GetAllSectors()));
    }

    //if (master()->getRedisDB()) {
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
    }
}

void CListCost::setInSector(QSharedPointer<CSFPL> &sfpl)
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
void CListCost::InitActIn()
{

    show_model_ = new ActTableModel(ActTableModel::act_in, 0, this,getFir2Points());
    act_in_widget_ = new ActWidget(tr("ACT IN"), show_model_, this);

    QStringList eqList;
    eqList << "w" << "y" << "wy" << "";
    QStringListModel* eqModel = new QStringListModel(eqList, this);
    ComboBoxDelegate* eq_delegate = new ComboBoxDelegate(this);
    eq_delegate->setModel(eqModel);
    eq_delegate->setHideKeyColumn(false);
    act_in_widget_->setItemDelegateForColumn(0, eq_delegate);

    QSharedPointer<QValidator> acid_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,7}"), 0));
    QAbstractItemDelegate *acid_delegate = new LineEditDelegate(acid_validator, act_in_widget_);
    act_in_widget_->setItemDelegateForColumn(1, acid_delegate);

    QSharedPointer<QValidator> code_validator(new QRegExpValidator(QRegExp("[0-7]{4}"), 0));
    QAbstractItemDelegate *code_delegate = new LineEditDelegate(code_validator, act_in_widget_);
    act_in_widget_->setItemDelegateForColumn(2, code_delegate);

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

    connect(act_in_widget_, SIGNAL(act()), this, SLOT(onAct()));
}
void CListCost::InitActDep()
{
    show_model_ = new ActTableModel(ActTableModel::act_dep, 0, this);
    act_in_widget_ = new ActWidget(tr("ACT DEP"), show_model_, this);

    QStringList eqList;
    eqList << "w" << "y" << "wy" << "";
    QStringListModel* eqModel = new QStringListModel(eqList, this);
    ComboBoxDelegate* eq_delegate = new ComboBoxDelegate(this);
    eq_delegate->setModel(eqModel);
    eq_delegate->setHideKeyColumn(false);
    act_in_widget_->setItemDelegateForColumn(0, eq_delegate);

    QSharedPointer<QValidator> acid_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,7}"), 0));
    QAbstractItemDelegate *acid_delegate = new LineEditDelegate(acid_validator, act_in_widget_);
    act_in_widget_->setItemDelegateForColumn(1, acid_delegate);

    QSharedPointer<QValidator> code_validator(new QRegExpValidator(QRegExp("[0-7]{4}"), 0));
    QAbstractItemDelegate *code_delegate = new LineEditDelegate(code_validator, act_in_widget_);
    act_in_widget_->setItemDelegateForColumn(2, code_delegate);

    QAbstractItemDelegate *eto_delegate = new TimeDelegate(act_in_widget_);
    act_in_widget_->setItemDelegateForColumn(3, eto_delegate);

    QSharedPointer<QValidator> adep_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,4}"), 0));
    LineEditDelegate *adep_delegate = new LineEditDelegate(adep_validator, act_in_widget_);
    act_in_widget_->setItemDelegateForColumn(4, adep_delegate);

    QSharedPointer<QValidator> ades_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,4}"), 0));
    LineEditDelegate *ades_delegate = new LineEditDelegate(ades_validator, act_in_widget_);
    act_in_widget_->setItemDelegateForColumn(5, ades_delegate);

    connect(act_in_widget_, SIGNAL(act()), this, SLOT(onAct()));
}


