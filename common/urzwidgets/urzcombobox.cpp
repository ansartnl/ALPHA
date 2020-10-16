#include "urzcombobox.h"
#include <QStandardItemModel>
#include <QTreeView>
#include <QtSql/QSqlQuery>
#include "../dbconnectionsmanager/dbconnectionsmanager.h"

URZComboBox::URZComboBox(bool bAdmin, QWidget* parent)
    : TreeComboBox(parent)
{
    QStandardItemModel* pModel = static_cast<QStandardItemModel*>(model());
    if(pModel)
    {
        pModel->setColumnCount(2);
        setModelColumn(0);
        QSqlQuery query(QSqlDatabase::database());
        QString sSQL =
                "SELECT r1.name, r1.id "
                "FROM restriction_user_type r1 "
                "LEFT JOIN restriction_type r2 ON r1.id = r2.id "
                "WHERE r2.id IS NULL";
        if(!bAdmin)
            sSQL += " AND r1.id <> 79";
        query.prepare(sSQL);
        if(query.exec())
        {
            while(query.next())
            {
                QList<QStandardItem*> items;
                items.append(new QStandardItem(query.value(0).toString()));
                items.append(new QStandardItem(query.value(1).toString()));
                pModel->appendRow(items);
            }
        }
        QStandardItem* parentItem(0);
        {
            QList<QStandardItem*> items;
            parentItem = new QStandardItem(tr("Zones"));
            parentItem->setFlags(Qt::NoItemFlags);
            items.append(parentItem);
            items.append(new QStandardItem(QString()));
            pModel->appendRow(items);
        }
        query.prepare(
                    "SELECT r1.name, r1.id "
                    "FROM restriction_user_type r1, restriction_type r2 "
                    "WHERE r1.id = r2.id");
        if(query.exec())
        {
            while(query.next())
            {
                QList<QStandardItem*> items;
                items.append(new QStandardItem(query.value(0).toString()));
                items.append(new QStandardItem(query.value(1).toString()));
                if(parentItem)
                    parentItem->appendRow(items);
                else
                    pModel->appendRow(items);
            }
        }
        QTreeView* pView = static_cast<QTreeView*>(view());
        if(pView)
        {
            pView->hideColumn(1);
        }
    }
}

URZComboBox::~URZComboBox()
{

}

void URZComboBox::hidePopup()
{
//    QStandardItemModel* pModel = static_cast<QStandardItemModel*>(model());
//    if(pModel)
//    {
//        if(!pModel->index(view()->currentIndex().row(), 1).data().toInt())
//            return;
//    }
    TreeComboBox::hidePopup();
}
