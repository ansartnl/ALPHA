#include "abonentdlg.h"
#include "QtCheckHeaderView.h"
#include "QTableWidgetItem"
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtGui/QMessageBox>
#include <QtCore/QDebug>

AbonentDlg::AbonentDlg(TypeDlg type, int idGroup, QWidget* parent) :
    QDialog(parent)
{
    setupUi(this);

    QSqlQueryModel abonentModel(this);
    QSqlQueryModel groupModel(this);
    QString sAbonent = "SELECT NULL name, NULL id, NULL address FROM dual";
    QString sGroup = "SELECT NULL name, NULL id, NULL address FROM dual";
    switch(type)
    {
    case ALL:
        sAbonent = "SELECT name, id, address FROM aftn_abonent";
        sGroup =
                "SELECT g1.name name, g1.id id, "
                    "a1.address address "
                "FROM aftn_abonent_group ag "
                    "RIGHT OUTER JOIN aftn_abonent a1 "
                        "ON ag.abonent_id = a1.id "
                    "RIGHT OUTER JOIN aftn_group g1 "
                        "ON ag.group_id = g1.id "
                "ORDER BY g1.id";
        break;
    case Abonents:
        sAbonent = "SELECT name, id, address FROM aftn_abonent";
        break;
    case Groups:
        sGroup =
                "SELECT g1.name name, g1.id id, "
                    "a1.address address "
                "FROM aftn_abonent_group ag "
                    "RIGHT OUTER JOIN aftn_abonent a1 "
                        "ON ag.abonent_id = a1.id "
                    "RIGHT OUTER JOIN aftn_group g1 "
                        "ON ag.group_id = g1.id "
                "ORDER BY g1.id";
        break;
    case AbonentInGroup:
        sAbonent = QString("SELECT name, id, address FROM aftn_abonent "
                           "WHERE id IN (SELECT abonent_id FROM aftn_abonent_group WHERE group_id = %1)").arg(idGroup);
        break;
    case AbonentOutGroup:
        sAbonent = QString("SELECT name, id, address FROM aftn_abonent "
                           "WHERE id NOT IN (SELECT abonent_id FROM aftn_abonent_group WHERE group_id = %1)").arg(idGroup);
        break;
    }
    abonentModel.setQuery(sAbonent);
    while(abonentModel.canFetchMore())
        abonentModel.fetchMore();
    groupModel.setQuery(sGroup);
    while(groupModel.canFetchMore())
        groupModel.fetchMore();

    tableWidgetA->setColumnCount(3);
    QtCheckHeaderView* pHeader = new QtCheckHeaderView(Qt::Horizontal, tableWidgetA);
    pHeader->setClickable(true);
    tableWidgetA->setHorizontalHeader(pHeader);
    pHeader->addCheckable(0);
    tableWidgetA->setColumnHidden(1, true);
    tableWidgetA->setColumnHidden(2, true);
    tableWidgetA->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    tableWidgetG->setColumnCount(3);
    pHeader = new QtCheckHeaderView(Qt::Horizontal, tableWidgetG);
    pHeader->setClickable(true);
    tableWidgetG->setHorizontalHeader(pHeader);
    pHeader->addCheckable(0);
    tableWidgetG->setColumnHidden(1, true);
    tableWidgetG->setColumnHidden(2, true);
    tableWidgetG->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    tableWidgetA->setSortingEnabled(false);
    tableWidgetA->setRowCount(abonentModel.rowCount());
    for(int i = 0; i < abonentModel.rowCount(); ++i)
    {
        QSqlRecord rec = abonentModel.record(i);
        QTableWidgetItem* item = new QTableWidgetItem(rec.value("name").toString());
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        tableWidgetA->setItem(i, 0, item);

        item = new QTableWidgetItem(rec.value("id").toString());
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        tableWidgetA->setItem(i, 1, item);

        item = new QTableWidgetItem(rec.value("address").toString());
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        tableWidgetA->setItem(i, 2, item);

    }
    tableWidgetA->setSortingEnabled(true);
    tableWidgetA->sortByColumn(0, Qt::AscendingOrder);

    tableWidgetG->setSortingEnabled(false);
    tableWidgetG->setRowCount(groupModel.rowCount());
    int rowcount = 0;
    for(int i = 0, j = -1, lastID = -1; i < groupModel.rowCount(); ++i)
    {
        QSqlRecord rec = groupModel.record(i);
        if(rec.value("id").toInt() != lastID)
        {
            lastID = rec.value("id").toInt();
            ++j;
            QTableWidgetItem* item = new QTableWidgetItem(rec.value("name").toString());
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
            item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            tableWidgetG->setItem(j, 0, item);

            item = new QTableWidgetItem(rec.value("id").toString());
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            tableWidgetG->setItem(j, 1, item);

            item = new QTableWidgetItem(rec.value("address").toString());
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            tableWidgetG->setItem(j, 2, item);
        }
        else
        {
            QString sText = tableWidgetG->item(j, 2)->text();
            if(!sText.isEmpty())
                sText += " ";
            sText += rec.value("address").toString();
            tableWidgetG->item(j, 2)->setText(sText);
        }
        rowcount = j+1;
    }
    tableWidgetG->setRowCount(rowcount);
    tableWidgetG->setSortingEnabled(true);
    tableWidgetG->sortByColumn(0, Qt::AscendingOrder);

    switch(type)
    {
    case Abonents:
    case AbonentInGroup:
    case AbonentOutGroup:
        tabWidget->removeTab(1);
        break;
    case Groups:
        tabWidget->removeTab(0);
        break;
    default: break;
    }

    Translate();
}

AbonentDlg::~AbonentDlg()
{
}

void AbonentDlg::Translate()
{
    retranslateUi(this);
    tableWidgetA->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("ID") << tr("Address"));
    tableWidgetG->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("ID") << tr("Address"));
}

void AbonentDlg::changeEvent(QEvent* event)
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

void AbonentDlg::accept()
{
    QDialog::accept();
}

QList<QPair<int, QString> > AbonentDlg::selectedAbonents() const
{
    QList<QPair<int, QString> > res;
    for(int i = 0; i < tableWidgetA->rowCount(); ++i)
    {
        if(Qt::Checked == tableWidgetA->item(i, 0)->checkState())
        {
            res.append(QPair<int, QString>(tableWidgetA->item(i, 1)->text().toInt(), tableWidgetA->item(i, 0)->text()));
        }
    }
    return res;
}

QList<QPair<int, QString> > AbonentDlg::selectedGroups() const
{
    QList<QPair<int, QString> > res;
    for(int i = 0; i < tableWidgetG->rowCount(); ++i)
    {
        if(Qt::Checked == tableWidgetG->item(i, 0)->checkState())
        {
            res.append(QPair<int, QString>(tableWidgetA->item(i, 1)->text().toInt(), tableWidgetA->item(i, 0)->text()));
        }
    }
    return res;
}

QStringList AbonentDlg::selectedAddresses() const
{
    QStringList res;
    for(int i = 0; i < tableWidgetA->rowCount(); ++i)
    {
        if(Qt::Checked == tableWidgetA->item(i, 0)->checkState())
        {
            res.append(tableWidgetA->item(i, 2)->text().split(" "));
        }
    }
    for(int i = 0; i < tableWidgetG->rowCount(); ++i)
    {
        if(Qt::Checked == tableWidgetG->item(i, 0)->checkState())
        {
            res.append(tableWidgetG->item(i, 2)->text().split(" "));
        }
    }
    res.removeDuplicates();
    res.removeOne("");
    return res;
}
