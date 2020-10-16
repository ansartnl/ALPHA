#include "addressbook.h"

#include "abonentdlg.h"

#include "qtautotransaction.h"

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtGui/QCloseEvent>
#include <QtCore/QDebug>
#include <QtCore/QSettings>

const QString AddressBook::tableAbonent       ="AFTN_ABONENT";
const QString AddressBook::tableAbonentID     ="ID";
const QString AddressBook::tableAbonentNAME   ="NAME";
const QString AddressBook::tableAbonentADDRESS="ADDRESS";

const QString AddressBook::tableGroup         ="AFTN_GROUP";
const QString AddressBook::tableGroupID       ="ID";
const QString AddressBook::tableGroupNAME     ="NAME";


AddressBook::AddressBook(QWidget* parent) :
    QDialog(parent), bModified(false), iGroupID(0)
{
    setupUi(this);

#ifndef DUSHANBE_BUILD
    mAutoTypesWidget = new AutoTypesWidget(this);
    connect(mAutoTypesWidget, SIGNAL(typesChanged(AutoTypesWidget::Types))
            , SLOT(onTypesChanged(AutoTypesWidget::Types)));
    abonentsLayout->addWidget(mAutoTypesWidget);
#endif

    labelGroups->setText("");

    // Abonent view
    abonentTableModel = new QSqlTableModel(this);
    abonentTableModel->setTable(tableAbonent);
    abonentTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    abonentIndexes[tableAbonentID] = abonentTableModel->fieldIndex(tableAbonentID);
    abonentIndexes[tableAbonentNAME] = abonentTableModel->fieldIndex(tableAbonentNAME);
    abonentIndexes[tableAbonentADDRESS] = abonentTableModel->fieldIndex(tableAbonentADDRESS);

    abonentTableModel->select();
    while(abonentTableModel->canFetchMore())
        abonentTableModel->fetchMore();

    tableViewA->setModel(abonentTableModel);
    tableViewA->horizontalHeader()->moveSection(tableViewA->horizontalHeader()->visualIndex(abonentIndexes[tableAbonentNAME]), 0);
    tableViewA->horizontalHeader()->moveSection(tableViewA->horizontalHeader()->visualIndex(abonentIndexes[tableAbonentADDRESS]), 1);
    tableViewA->setColumnHidden(abonentIndexes[tableAbonentID], true);
    tableViewA->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tableViewA->setSortingEnabled(true);
    tableViewA->sortByColumn(abonentIndexes[tableAbonentNAME], Qt::AscendingOrder);

    // Group view
    groupTableModel = new QSqlTableModel(this);
    groupTableModel->setTable(tableGroup);
    groupTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    groupIndexes[tableGroupID] = groupTableModel->fieldIndex(tableGroupID);
    groupIndexes[tableGroupNAME] = groupTableModel->fieldIndex(tableGroupNAME);

    groupTableModel->select();
    while(groupTableModel->canFetchMore())
        groupTableModel->fetchMore();

    tableViewG->setModel(groupTableModel);
    tableViewG->horizontalHeader()->moveSection(tableViewG->horizontalHeader()->visualIndex(groupIndexes[tableGroupNAME]), 0);
    tableViewG->setColumnHidden(groupIndexes[tableGroupID], true);
    tableViewG->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tableViewG->setSortingEnabled(true);
    tableViewG->sortByColumn(groupIndexes[tableGroupNAME], Qt::AscendingOrder);

    // Group's abonent view
    groupAbonentModel = new QSqlQueryModel(this);
    SetGroupID(0);
    tableViewGA->setModel(groupAbonentModel);
    tableViewGA->setColumnHidden(0, true);
    tableViewGA->setColumnHidden(1, true);
    tableViewGA->setColumnHidden(3, true);
    tableViewGA->setColumnHidden(4, true);
    tableViewGA->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tableViewGA->setSortingEnabled(false);

    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onOperButtons(QAbstractButton*)));
    connect(butAddG, SIGNAL(clicked()), this, SLOT(addGroup()));
    connect(butDelG, SIGNAL(clicked()), this, SLOT(deleteGroup()));
    connect(butAddA, SIGNAL(clicked()), this, SLOT(addAbonent()));
    connect(butDelA, SIGNAL(clicked()), this, SLOT(deleteAbonent()));
    connect(butAddGA, SIGNAL(clicked()), this, SLOT(addGroupAbonent()));
    connect(butDelGA, SIGNAL(clicked()), this, SLOT(deleteGroupAbonent()));
    connect(tableViewA->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(abonentSelected(const QModelIndex&)));
    connect(tableViewG->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(groupSelected(const QModelIndex&)));
    connect(abonentTableModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(tableDataChanged()));
    connect(groupTableModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(tableDataChanged()));
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    Translate();
}

AddressBook::~AddressBook()
{
}

void AddressBook::Translate()
{
    retranslateUi(this);

    abonentTableModel->setHeaderData(abonentIndexes[tableAbonentNAME], Qt::Horizontal, tr("Name"));
    abonentTableModel->setHeaderData(abonentIndexes[tableAbonentADDRESS], Qt::Horizontal, tr("Address"));

    groupTableModel->setHeaderData(groupIndexes[tableGroupNAME], Qt::Horizontal, tr("Name"));

    groupAbonentModel->setHeaderData(2, Qt::Horizontal, tr("Name"));
}

void AddressBook::closeEvent(QCloseEvent* pEvent)
{
    if (bModified)
    {
        switch(QMessageBox::question(this, tr("Unsaved data"), tr("Save modified data?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
        {
        case QMessageBox::Yes:
            {
                if(Save())
                    pEvent->accept();
                else
                    pEvent->ignore();
            }
            break;
        case QMessageBox::Cancel:
            pEvent->ignore();
            break;
        default:
            pEvent->accept();
            break;
        }

    }
    else
        pEvent->accept();
}

void AddressBook::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    switch(event->type())
    {
    case QEvent::LanguageChange:
        Translate();
        break;
    default:
        break;
    }
}

void AddressBook::onOperButtons(QAbstractButton* pButton)
{
    switch(buttonBox->buttonRole(pButton))
    {
    case QDialogButtonBox::AcceptRole:
        if(Save())
            accept();
        break;
    case QDialogButtonBox::ApplyRole:
        Save();
        break;
    case QDialogButtonBox::RejectRole:
        bModified = false;
        reject();
        break;
    default:
        break;
    }
}

void AddressBook::addGroup()
{
    bModified = true;
    groupTableModel->insertRow(0);
    QModelIndex qmIndex = groupTableModel->index(0, groupIndexes[tableGroupNAME]);
    tableViewG->setCurrentIndex(qmIndex);
    tableViewG->edit(qmIndex);
}

void AddressBook::deleteGroup()
{
    QModelIndexList selectList = tableViewG->selectionModel()->selectedRows();
    if (selectList.isEmpty() && tableViewG->currentIndex().isValid())
        selectList.append(tableViewG->currentIndex());
    int correction = 0;
    bool isChanged = false;
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == groupTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        groupTableModel->removeRow(rowForDelte);
        isChanged = true;
    }
    if (isChanged)
        bModified = true;
}

void AddressBook::addAbonent()
{
    bModified = true;
    abonentTableModel->insertRow(0);
    QModelIndex qmIndex = abonentTableModel->index(0, abonentIndexes[tableAbonentNAME]);
    tableViewA->setCurrentIndex(qmIndex);
    tableViewA->edit(qmIndex);
}

void AddressBook::deleteAbonent()
{
    QModelIndexList selectList = tableViewA->selectionModel()->selectedRows();
    if (selectList.isEmpty() && tableViewA->currentIndex().isValid())
        selectList.append(tableViewA->currentIndex());
    int correction = 0;
    bool isChanged = false;
    foreach (const QModelIndex &selected, selectList)
    {
        int rowForDelte = selected.row() - correction;
        if ("*" == abonentTableModel->headerData(rowForDelte, Qt::Vertical))
            ++correction;
        abonentTableModel->removeRow(rowForDelte);
        isChanged = true;
    }
    if (isChanged)
        bModified = true;
}

void AddressBook::addGroupAbonent()
{
    if(iGroupID)
    {
        AbonentDlg dlg(AbonentDlg::AbonentOutGroup, iGroupID, this);
        if(dlg.exec() == QDialog::Accepted)
        {
            QList<QPair<int, QString> > list = dlg.selectedAbonents();
            if(list.size())
            {
                bModified = true;
                QtAutoTransaction transaction;
                QSqlQuery query;
                query.prepare("INSERT INTO aftn_abonent_group (group_id, abonent_id) "
                                "VALUES (:1, :2)");
                for(int i = 0; i < list.size(); ++i)
                {
                    query.bindValue(":1", iGroupID);
                    query.bindValue(":2", list[i].first);
                    if(!query.exec())
                    {
                        bModified = false;
                        QString sErr = query.lastError().text();
                        qCritical() << sErr;
                        QMessageBox::critical(this, tr("Error"), sErr);
                        return;
                    }
                }
                transaction.commit();
                SetGroupID(iGroupID);
            }
        }
    }
}

void AddressBook::deleteGroupAbonent()
{
    if(iGroupID)
    {
        QModelIndexList selectList = tableViewGA->selectionModel()->selectedRows();
        if (selectList.isEmpty() && tableViewGA->currentIndex().isValid())
            selectList.append(tableViewGA->currentIndex());
        if(selectList.size())
        {
            bModified = true;
            QtAutoTransaction transaction;
            QSqlQuery query;
            query.prepare("DELETE FROM aftn_abonent_group "
                            "WHERE id = :1");
            foreach (const QModelIndex &selected, selectList)
            {
                QSqlRecord rec = groupAbonentModel->record(selected.row());
                query.bindValue(":1", rec.value("ID"));
                if(!query.exec())
                {
                    bModified = false;
                    QString sErr = query.lastError().text();
                    qCritical() << sErr;
                    QMessageBox::critical(this, tr("Error"), sErr);
                    return;
                }
            }
            transaction.commit();
            SetGroupID(iGroupID);
        }
    }
}

void AddressBook::tableDataChanged()
{
    bModified = true;
}

void AddressBook::groupSelected(const QModelIndex& qIndex)
{
    int groupID = 0;
    if(qIndex.isValid())
        groupID = groupTableModel->data(groupTableModel->index(qIndex.row(), groupIndexes[tableGroupID])).toInt();
    SetGroupID(groupID);
}

void AddressBook::abonentSelected(const QModelIndex& qIndex)
{
    QString sText;
    int abonentID = 0;
    if(qIndex.isValid())
        abonentID = abonentTableModel->data(abonentTableModel->index(qIndex.row(), abonentIndexes[tableAbonentID])).toInt();
    if(abonentID)
    {
        QSqlQuery query;
        query.prepare(QString(
            "SELECT g1.name name, g1.id id "
            "FROM aftn_abonent_group ag "
                "INNER JOIN aftn_abonent a1 "
                    "ON ag.abonent_id = a1.id "
                "INNER JOIN aftn_group g1 "
                    "ON ag.group_id = g1.id "
            "WHERE a1.id = %1 "
            "ORDER BY g1.name").arg(abonentID));
        if(query.exec())
        {
            while(query.next())
            {
                if(!sText.isEmpty()) sText += ", ";
                sText += query.value(0).toString();
            }
        }
        else
        {
            qWarning() << query.lastError().text();
        }
    }
    labelGroups->setText(sText);

#ifndef DUSHANBE_BUILD
    AutoTypesWidget::Types t;
    if(abonentID)
    {
        mAutoTypesWidget->show();
        QSqlQuery query;
        query.prepare(QString(
            "SELECT aaa.FREE_TEXT, aaa.FPL, aaa.ARR, aaa.CHG, aaa.CNL, aaa.DEP, aaa.DLA "
            "FROM aftn_abonent_autotypes aaa "
                "INNER JOIN aftn_abonent a1 "
                    "ON aaa.aftn_abonent_id = a1.id "
            "WHERE a1.id = %1").arg(abonentID));
        if(query.exec())
        {
            if(query.next())
            {
                if (query.value(0).toInt())
                    t |= AutoTypesWidget::FREE_TEXT;
                if (query.value(1).toInt())
                    t |= AutoTypesWidget::FPL;
                if (query.value(2).toInt())
                    t |= AutoTypesWidget::ARR;
                if (query.value(3).toInt())
                    t |= AutoTypesWidget::CHG;
                if (query.value(4).toInt())
                    t |= AutoTypesWidget::CNL;
                if (query.value(5).toInt())
                    t |= AutoTypesWidget::DEP;
                if (query.value(6).toInt())
                    t |= AutoTypesWidget::DLA;
            }
        }
        else
        {
            qWarning() << query.lastError().text();
        }
    } else
    {
        mAutoTypesWidget->hide();
    }
    mAutoTypesWidget->setTypes(t);
#endif
}

void AddressBook::tabChanged(int index)
{
    if(index == 0)
    {
        abonentSelected(tableViewA->currentIndex());
    }
}

#ifndef DUSHANBE_BUILD
void AddressBook::onTypesChanged(const AutoTypesWidget::Types &t)
{
    QModelIndexList	indexList = tableViewA->selectionModel()->selectedRows();
    if (indexList.isEmpty())
        return;
    int abonentID = abonentTableModel->data(abonentTableModel->index(indexList[0].row(), abonentIndexes[tableAbonentID])).toInt();

    QSqlQuery query;
    query.prepare(QString(
                      "UPDATE aftn_abonent_autotypes SET free_text = :free_text, fpl = :fpl, arr = :arr, "
                      "chg = :chg, cnl = :cnl, dep = :dep, dla = :dla "
                      "WHERE aftn_abonent_id = %1").arg(abonentID));
    query.bindValue(":free_text", t.testFlag(AutoTypesWidget::FREE_TEXT) ? 1 : 0);
    query.bindValue(":fpl", t.testFlag(AutoTypesWidget::FPL) ? 1 : 0);
    query.bindValue(":arr", t.testFlag(AutoTypesWidget::ARR) ? 1 : 0);
    query.bindValue(":chg", t.testFlag(AutoTypesWidget::CHG) ? 1 : 0);
    query.bindValue(":cnl", t.testFlag(AutoTypesWidget::CNL) ? 1 : 0);
    query.bindValue(":dep", t.testFlag(AutoTypesWidget::DEP) ? 1 : 0);
    query.bindValue(":dla", t.testFlag(AutoTypesWidget::DLA) ? 1 : 0);
    if(!query.exec())
    {
        qWarning() << query.lastError().text();
    }
}
#endif

bool AddressBook::Save()
{
    bool bRes = false;
    QVariant abonentSelectedRow = abonentTableModel->data(abonentTableModel->index(tableViewA->selectionModel()->currentIndex().row(), abonentIndexes[tableAbonentNAME]));
    QVariant groupSelectedRow = groupTableModel->data(groupTableModel->index(tableViewG->selectionModel()->currentIndex().row(), groupIndexes[tableGroupNAME]));
    try
    {
        QtAutoTransaction dbTrans;
        if (!groupTableModel->submitAll())
            throw (groupTableModel->lastError().text());
        if (!abonentTableModel->submitAll())
            throw (abonentTableModel->lastError().text());
        dbTrans.commit();
        bRes = true;

        groupTableModel->select();
        while(groupTableModel->canFetchMore())
            groupTableModel->fetchMore();
        QModelIndexList groupIndexList = groupTableModel->match(groupTableModel->index(0, groupIndexes[tableGroupNAME]), Qt::DisplayRole, groupSelectedRow, 1, Qt::MatchExactly);

        abonentTableModel->select();
        while(abonentTableModel->canFetchMore())
            abonentTableModel->fetchMore();
        QModelIndexList abonentIndexList = abonentTableModel->match(abonentTableModel->index(0, abonentIndexes[tableAbonentNAME]), Qt::DisplayRole, abonentSelectedRow, 1, Qt::MatchExactly);

        if(abonentIndexList.count())
        {
            tableViewA->selectRow(abonentIndexList.first().row());
            tableViewA->scrollTo(abonentIndexList.first());
        }
        if(groupIndexList.count())
        {
            tableViewG->selectRow(groupIndexList.first().row());
            tableViewG->scrollTo(groupIndexList.first());
        }
    }
    catch (const QString &err)
    {
        qCritical() << err;
        QMessageBox::critical(this, tr("Error"), err);
    }
    if(bRes)
        bModified = false;
    return bRes;
}

void AddressBook::SetGroupID(int idGroup)
{
    iGroupID = idGroup;
    QString sQuery = QString(
            "SELECT  ag.id ID, "
                    "a1.id ABONENT_ID, a1.name ABONENT_NAME, "
                    "g1.id GROUP_ID, g1.name GROUP_NAME "
            "FROM aftn_abonent_group ag "
                    "INNER JOIN aftn_abonent a1 "
                    "ON ag.abonent_id = a1.id "
                    "INNER JOIN aftn_group g1 "
                    "ON ag.group_id = g1.id "
            "WHERE ag.group_id = %1 "
            "ORDER BY a1.name").arg(idGroup);
    groupAbonentModel->setQuery(sQuery);
}
