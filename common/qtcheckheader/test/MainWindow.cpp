#include "MainWindow.h"

#include <QtGui/QStandardItemModel>

#include "QtCheckHeaderView.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    QStandardItemModel *model = new QStandardItemModel(this);

    QStandardItem *root = model->invisibleRootItem();
    QList<QList<QStandardItem *> > items;
    for (int i = 0; i < 10; ++i)
    {
        QList<QStandardItem *> res;
        for (int j = 0; j < 10; ++j)
        {
            res.append(new QStandardItem(QString("%1;%2").arg(j).arg(i)));
            res.at(j)->setCheckable(true);
            res.at(j)->setCheckState(Qt::CheckState(qrand()%3));
            res.at(j)->setTristate(true);
        }
        items.append(res);
        root->appendColumn(res);
    }

    tableView->setModel(model);
    QtCheckHeaderView *header = new QtCheckHeaderView(Qt::Horizontal, tableView);
    QtCheckHeaderView *vheader = new QtCheckHeaderView(Qt::Vertical, tableView);

    tableView->setHorizontalHeader(header);
    tableView->setVerticalHeader(vheader);

    for (int i = 0; i < 10; ++i)
    {
        header->addCheckable(i);
    }

    for (int i = 0; i < 10; ++i)
    {
        vheader->addCheckable(i);
    }
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
