#include "ListACTOUT.h"

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

CListActOut::CListActOut(CMaster* pMaster, QWidget* pParent) :
        CListAC(pMaster, pParent),
        edit_model_(new ActTableModel(ActTableModel::act_out, 0, pParent,getFir2Points())),
        act_in_widget_(new ActWidget("ACT OUT", edit_model_, pParent))
{
    setListType(QString("ACT_OUT"));

    fillSortModel();

    m_sortModel.SetRowFilterType(CListACSortModel::TYPE_ACT_OUT);

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

    QSharedPointer<QValidator> cop_validator(new QRegExpValidator(QRegExp("[\\d,\\w]{0,5}"), 0));
    LineEditDelegate *cop_delegate = new LineEditDelegate(cop_validator, act_in_widget_);
    cop_delegate->setCompleter(getFirPoints());
    act_in_widget_->setItemDelegateForColumn(3, cop_delegate);

    QAbstractItemDelegate *eto_delegate = new TimeDelegate(act_in_widget_);
    act_in_widget_->setItemDelegateForColumn(4, eto_delegate);

    act_in_widget_->setItemDelegateForColumn(5, new FlightLevelDelegate(act_in_widget_, this));

    act_in_widget_->setItemDelegateForColumn(6, new FlightLevelDelegate(act_in_widget_, this));

    connect(act_in_widget_, SIGNAL(act()), this, SLOT(onAct()));

    Translate();
    UpdateViewData();

    m_list.setContextMenuPolicy(Qt::NoContextMenu);

#ifdef _MSC_VER
    m_listView.append(this);
#endif
}

void CListActOut::Translate()
{
    SizeToContents();
}

int CListActOut::InsertRow(const QSharedPointer<CAirplane> &airplane)
{
    Q_UNUSED(airplane)
    return -1;
}

void CListActOut::ModifyRow(int row, const QSharedPointer<CAirplane> &airplane)
{
    Q_UNUSED(airplane)
}

bool CListActOut::IsCanDeleteRow(int row, const QSharedPointer<CAirplane> &airplane)
{
    Q_UNUSED(airplane)
    return false;
}

void CListActOut::TimerSlot()
{
}

void CListActOut::OnDblClickItemEdit(const QModelIndex &index)
{
    if (index.isValid()) {
        if (master()->getRedisDB()) {
            sfpl_ = m_model.GetItemData(m_sortModel.mapToSource(index)).GetSFPL();
            QList<Act> act_ins;
            Act act = {sfpl_->GetEquipment(), sfpl_->GetACID(), sfpl_->GetCODEtoString(), sfpl_->GetCOPout(), sfpl_->GetETOout(),
                sfpl_->GetCOPinFL(), sfpl_->GetDepAerodrome(), sfpl_->GetDestAerodrome(),
                sfpl_->GetCurSector(), sfpl_->GetNextSector(), sfpl_->GetTFL(), sfpl_->GetRFL()};
            act_ins.push_back(act);
            edit_model_->setActs(act_ins);
            QSet<int> non_editable_fields;
            non_editable_fields.insert(1);
            edit_model_->setNonEditableFields(non_editable_fields);
            act_in_widget_->show();
        }
    }
}

void CListActOut::onAct()
{
    Act act_in = edit_model_->act(0);
    sfpl_->SetEquipment(act_in.eq);
    sfpl_->SetACID(act_in.acid);
    sfpl_->SetCODE(act_in.code.toInt(0, 8));
    sfpl_->SetCOPout(act_in.cop);
    sfpl_->SetETOout(act_in.eto);
    sfpl_->SetTFL(FLVal(act_in.tfl, true));
    sfpl_->SetRFL(FLVal(act_in.rfl, true));

    if (master()->getRedisDB()) {
        bool ok = master()->getRedisDB()->saveSFPL(sfpl_);
        if (!ok)
            QMessageBox::critical(this, tr("Error"), tr("Error saving FPL"));
        else
            logOperation(tr("ACT OUT: ( %1 )")
                         .arg((QStringList()
                               << tr("equipment = '%1'").arg(sfpl_->GetEquipment())
                               << tr("acid = '%1'").arg(sfpl_->GetACID())
                               << tr("code = '%1'").arg(sfpl_->GetCODEtoString())
                               << tr("cop out = '%1'").arg(sfpl_->GetCOPout())
                               << tr("eto out = '%1'").arg(sfpl_->GetETOout().toString())
                               << tr("tfl = '%1'").arg(sfpl_->GetTFL().toString())
                               << tr("rfl = '%1'").arg(sfpl_->GetRFL().toString())).join(" | ")));
    }
}
