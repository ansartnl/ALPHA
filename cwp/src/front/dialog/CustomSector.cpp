#include "CustomSector.h"
#include <QSqlTableModel>
#include <QMessageBox>
#include "import/Zone.h"
#include "main.h"
#include "QtCheckHeaderView.h"

CustomSector::CustomSector(uint idFir, QWidget *pParent, Qt::WindowFlags f)
    : QDialog(pParent, f), m_pTypeModel(0), m_bChanging(false)
{
	setupUi(this);

    tableWidgetSector->setColumnCount(2);
    tableWidgetSector->setHorizontalHeaderLabels(
            QStringList() << tr("Name") << tr("Short name"));
    QtCheckHeaderView* pHeader = new QtCheckHeaderView(Qt::Horizontal, tableWidgetSector);
    pHeader->setClickable(true);
    tableWidgetSector->setHorizontalHeader(pHeader);
    pHeader->addCheckable(0);

    // FIR
    QList<Zone> ZoneList;
    TVA_CATCHALL_TRY
    {
        dao()->LoadZones(ZoneList);
    } TVA_CATCHALL(err)
    {
        QMessageBox::critical(this, tr("Zones"), err.reason());
    }
    QList<Zone>::const_iterator itScan = ZoneList.begin();
    for(; itScan != ZoneList.end(); ++itScan)
    {
        if(itScan->GetType() == Zone::enFIR)
            comboBoxFIR->addItem(itScan->GetIDName(), itScan->GetID());
    }
    if(comboBoxFIR->count())
    {
        int index = 0;
        for(int i = 0;  i < comboBoxFIR->count(); ++i)
        {
            if(comboBoxFIR->itemData(i) == idFir)
            {
                index = i;
                break;
            }
        }
        comboBoxFIR->setCurrentIndex(index);
        OnComboFir(index);
    }

    QString sCurSector = networkSettings()->value("users/current_sector").toString();
    QStringList sCurSectorsList = networkSettings()->value("users/current_sectors_short_name", "").toString().split("-", QString::SkipEmptyParts);
    for(int i = 0; i < tableWidgetSector->rowCount(); ++i)
    {
        if(sCurSectorsList.contains(tableWidgetSector->item(i, 1)->text()))
            tableWidgetSector->item(i, 0)->setCheckState(Qt::Checked);
        if(!sCurSector.isEmpty() && sCurSector == tableWidgetSector->item(i, 1)->text())
            tableWidgetSector->item(i, 0)->setData(Qt::UserRole+1, 1);
    }
    QTableWidgetItem* pItem = tableWidgetSector->currentItem();
    if(pItem)
    {
        if(tableWidgetSector->item(pItem->row(), 0)->data(Qt::UserRole+1).toInt())
            checkBoxMain->setChecked(true);
    }
    tableWidgetSector->resizeColumnToContents(0);
    tableWidgetSector->sortByColumn(0, Qt::AscendingOrder);

    // Privilige
    m_pTypeModel = new QSqlTableModel();
    m_pTypeModel->setTable("sectoring_type");
    comboBoxType->setModel(m_pTypeModel);
    comboBoxType->setModelColumn(m_pTypeModel->fieldIndex("name"));
    m_pTypeModel->select();

    connect(comboBoxFIR, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboFir(int)));
    connect(checkBoxMain, SIGNAL(stateChanged(int)), this, SLOT(OnCheckMain(int)));
    connect(tableWidgetSector, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*))
            , this, SLOT(OnItemChanged(QTableWidgetItem*,QTableWidgetItem*)));
}

CustomSector::~CustomSector()
{

}

void CustomSector::accept()
{
    bool bAccept = true;
    for(int i = 0; i < tableWidgetSector->rowCount(); ++i)
    {
        if(Qt::Checked == tableWidgetSector->item(i, 0)->checkState())
        {
            bAccept = false;
            break;
        }
    }
    for(int i = 0; i < tableWidgetSector->rowCount(); ++i)
    {
        QTableWidgetItem* pItem = tableWidgetSector->item(i, 0);
        if(Qt::Checked == pItem->checkState() && pItem->data(Qt::UserRole+1).toInt())
        {
            bAccept = true;
            break;
        }
    }
    if(bAccept)
        QDialog::accept();
    else
    {
        QMessageBox::warning(this, windowTitle(), tr("Not selected main sector."));
        checkBoxMain->setFocus();
    }
}

uint CustomSector::GetIdFIR() const
{
    int idFIR = 0;
    if(comboBoxFIR->currentIndex() != -1)
        idFIR = comboBoxFIR->itemData(comboBoxFIR->currentIndex()).toInt();
    return idFIR;
}

QString CustomSector::GetPrivileges() const
{
    QString sPrivileges;
    if(comboBoxType->currentIndex() != -1)
        sPrivileges = m_pTypeModel->data(m_pTypeModel->index(comboBoxType->currentIndex(), m_pTypeModel->fieldIndex("permission"))).toString();
    return sPrivileges;
}

QString CustomSector::GetSectorType() const
{
    QString sType;
    if(comboBoxType->currentIndex() != -1)
        sType = m_pTypeModel->data(m_pTypeModel->index(comboBoxType->currentIndex(), m_pTypeModel->fieldIndex("name"))).toString();
    return sType;
}

QList<uint> CustomSector::GetSectors() const
{
    QList<uint> listSector;
    for(int i = 0; i < tableWidgetSector->rowCount(); ++i)
    {
        QTableWidgetItem* pItem = tableWidgetSector->item(i, 0);
        if(Qt::Checked == pItem->checkState())
        {
            if(pItem->data(Qt::UserRole+1).toInt())
                listSector.prepend(pItem->data(Qt::UserRole).toUInt());
            else
                listSector.append(pItem->data(Qt::UserRole).toUInt());
        }
    }
    return listSector;
}

void CustomSector::OnComboFir(int index)
{
    m_bChanging = true;
    tableWidgetSector->setSortingEnabled(false);
    tableWidgetSector->clearContents();
    tableWidgetSector->setRowCount(0);
    checkBoxMain->setCheckState(Qt::Unchecked);
    if(index != -1)
    {
        QList<Zone> ZoneList;
        TVA_CATCHALL_TRY
        {
            QString sWhere = "type = " + QString::number(Zone::enSector) + " AND parent_id = "
                             + QString::number(comboBoxFIR->itemData(index).toUInt());
            dao()->LoadZones(ZoneList, sWhere);
        }TVA_CATCHALL(err){
        LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
        if(ZoneList.size())
        {
            tableWidgetSector->setRowCount(ZoneList.size());
            QList<Zone>::const_iterator itSector = ZoneList.constBegin();
            for(int row = 0; itSector != ZoneList.constEnd(); ++itSector, ++row)
            {
                QTableWidgetItem* item = new QTableWidgetItem(itSector->GetName());
                item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable);
                item->setCheckState(Qt::Unchecked);
                item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
                item->setData(Qt::UserRole, itSector->GetID());
                tableWidgetSector->setItem(row, 0, item);

                item = new QTableWidgetItem(itSector->GetShortName());
                item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
                item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
                tableWidgetSector->setItem(row, 1, item);
            }
        }
    }
    tableWidgetSector->setSortingEnabled(true);
    m_bChanging = false;
}

void CustomSector::OnItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous)
{
    if(m_bChanging) return;
    Qt::CheckState check = Qt::Unchecked;
    if(current && current == tableWidgetSector->currentItem())
    {
        if(tableWidgetSector->item(current->row(), 0)->data(Qt::UserRole+1).toInt())
            check = Qt::Checked;
    }
    m_bChanging = true;
    checkBoxMain->setCheckState(check);
    m_bChanging = false;
}

void CustomSector::OnCheckMain(int state)
{
    if(m_bChanging) return;
    m_bChanging = true;
    for(int i = 0; i < tableWidgetSector->rowCount(); ++i)
        tableWidgetSector->item(i, 0)->setData(Qt::UserRole+1, 0);
    if(Qt::Checked == state)
    {
        QTableWidgetItem* pItem = tableWidgetSector->currentItem();
        if(pItem)
            tableWidgetSector->item(pItem->row(), 0)->setData(Qt::UserRole+1, 1);
    }
    m_bChanging = false;
}
