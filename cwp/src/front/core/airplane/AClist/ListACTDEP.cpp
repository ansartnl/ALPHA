#include "ListACTDEP.h"

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

CListActDep::CListActDep(CMaster* pMaster, QWidget* pParent) :
        CListAC(pMaster, pParent),
        edit_model_(new ActTableModel(ActTableModel::act_dep, 0, pParent)),
        act_in_widget_(new ActWidget("ACT DEP", edit_model_, pParent))
{
    Init();
}

CListActDep::CListActDep(QStringList _lst, CMaster* pMaster, QWidget* pParent) :
        CListAC(pMaster, pParent),
        edit_model_(new ActTableModel(ActTableModel::act_dep, 0, pParent)),
        act_in_widget_(new ActWidget("ACT DEP", edit_model_, pParent))
{
    Init();
    lst_orcamrange = _lst;
}

void CListActDep::Init()
{
    setListType(QString("ACT_DEP"));

    fillSortModel();

    m_sortModel.SetRowFilterType(CListACSortModel::TYPE_ACT_DEP);

    //QHBoxLayout *hor_layout = new QHBoxLayout();
    //hor_layout->setContentsMargins(0, 0, 0, 0);
    //hor_layout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    m_ControlsLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));

    but_act_ = new QPushButton(this);
    m_ControlsLayout->addWidget(but_act_);
    //hor_layout->addWidget(but_act_);

    //qobject_cast<QVBoxLayout*>(layout())->addLayout(hor_layout);
    connect(but_act_, SIGNAL(clicked()), this, SLOT(onButtonActIn()));

    connect(&m_list, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(OnDblClickItemEdit(const QModelIndex&)));

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

    Translate();
    UpdateViewData();

    m_list.setContextMenuPolicy(Qt::NoContextMenu);

#ifdef _MSC_VER
    m_listView.append(this);
#endif
}
void CListActDep::Translate()
{
    but_act_->setText(tr("ACT"));
    SizeToContents();
}

int CListActDep::InsertRow(const QSharedPointer<CAirplane> &airplane)
{
    Q_UNUSED(airplane)
    return -1;
}

void CListActDep::ModifyRow(int row, const QSharedPointer<CAirplane> &airplane)
{
    Q_UNUSED(airplane)
}

bool CListActDep::IsCanDeleteRow(int row, const QSharedPointer<CAirplane> &airplane)
{
    Q_UNUSED(airplane)
    return false;
}

void CListActDep::TimerSlot()
{
}

/*const QSize CListActDep::GetVisibleSize() const
{
    return CListAC::GetVisibleSize() + QSize(0, but_act_->height() + layout()->spacing());
}*/

void CListActDep::onButtonActIn()
{
    //QString code = ReadOrcam();
    QString code = FreeCodeOrcam();
    
    if (master()->getRedisDB()) {
        sfpl_ = QSharedPointer<CSFPL>(new CSFPL);
        QList<Act> act_ins;
        //Act act = {QString(), QString(), 0, QString(), QTime(), 0, QString(), QString()};
        Act act = {QString(), QString(), code, QString(), QTime(), 0, QString(), QString()};
        act_ins.push_back(act);
        edit_model_->setActs(act_ins);
        QSet<int> non_editable_fields;
        edit_model_->setNonEditableFields(non_editable_fields);
        edit_model_->clearErrors();
        act_in_widget_->show();
    }
}

void CListActDep::OnDblClickItemEdit(const QModelIndex &index)
{
    if (index.isValid()) {
        if (master()->getRedisDB()) {
            sfpl_ = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
            QList<Act> act_ins;
            Act act = {sfpl_->GetEquipment(), sfpl_->GetACID(), sfpl_->GetCODEtoString(), sfpl_->GetCOPin(), sfpl_->GetETD(),
                sfpl_->GetCOPinFL(), sfpl_->GetDepAerodrome(), sfpl_->GetDestAerodrome(),
                sfpl_->GetCurSector(), sfpl_->GetNextSector()};
            act_ins.push_back(act);
            edit_model_->setActs(act_ins);
            QSet<int> non_editable_fields;
            non_editable_fields.insert(1);
            edit_model_->setNonEditableFields(non_editable_fields);
            act_in_widget_->show();
        }
    }
}

void CListActDep::onAct()
{
    Act act_in = edit_model_->act(0);
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
    sfpl_->SetEquipment(act_in.eq);
    sfpl_->SetACID(act_in.acid);
    sfpl_->SetCODE(act_in.code.toInt(0, 8));
    sfpl_->SetETD(act_in.eto);
    if(act_in.adep.isEmpty() && !networkSettings()->value("meteo/airport").toString().isEmpty())
        act_in.adep.append(networkSettings()->value("meteo/airport").toString().toUpper());
    sfpl_->SetDepAerodrome(act_in.adep);
    sfpl_->SetCOPin(sfpl_->GetDepAerodrome());
    sfpl_->SetDestAerodrome(act_in.dest);

    setInSector(sfpl_);

    if (master()->getRedisDB()) {
        ok = master()->getRedisDB()->saveSFPL(sfpl_);
        int min = networkSettings()->value("users/ORCAMLifeTime", 1).toInt();
        bool okk = master()->getRedisDB()->addOrcam(act_in.code, QDateTime::currentDateTime(), min);
        if (!ok || !okk)
            QMessageBox::critical(this, tr("Error"), tr("Error saving FPL from CListActDep"));
        else
            logOperation(tr("ACT DEP: ( %1 )")
                         .arg((QStringList()
                               << tr("equipment = '%1'").arg(sfpl_->GetEquipment())
                               << tr("acid = '%1'").arg(sfpl_->GetACID())
                               << tr("code = '%1'").arg(sfpl_->GetCODEtoString())
                               << tr("eto = '%1'").arg(sfpl_->GetETOin().toString())
                               << tr("dep = '%1'").arg(sfpl_->GetDepAerodrome())
                               << tr("cop in = '%1'").arg(sfpl_->GetCOPin())
                               << tr("dest = '%1'").arg(sfpl_->GetDestAerodrome())).join(" | ")));
     }
    }
}

void CListActDep::setInSector(QSharedPointer<CSFPL> &sfpl)
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

QString CListActDep::ReadOrcam()
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

QString CListActDep::FreeCodeOrcam()
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
