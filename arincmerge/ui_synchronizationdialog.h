/********************************************************************************
** Form generated from reading UI file 'synchronizationdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYNCHRONIZATIONDIALOG_H
#define UI_SYNCHRONIZATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SynchronizationDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *synchronizeButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *closeButton;

    void setupUi(QDialog *SynchronizationDialog)
    {
        if (SynchronizationDialog->objectName().isEmpty())
            SynchronizationDialog->setObjectName(QStringLiteral("SynchronizationDialog"));
        SynchronizationDialog->resize(883, 498);
        verticalLayout = new QVBoxLayout(SynchronizationDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableWidget = new QTableWidget(SynchronizationDialog);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->horizontalHeader()->setDefaultSectionSize(200);
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setDefaultSectionSize(60);

        verticalLayout->addWidget(tableWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        synchronizeButton = new QPushButton(SynchronizationDialog);
        synchronizeButton->setObjectName(QStringLiteral("synchronizeButton"));

        horizontalLayout->addWidget(synchronizeButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        closeButton = new QPushButton(SynchronizationDialog);
        closeButton->setObjectName(QStringLiteral("closeButton"));

        horizontalLayout->addWidget(closeButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(SynchronizationDialog);

        QMetaObject::connectSlotsByName(SynchronizationDialog);
    } // setupUi

    void retranslateUi(QDialog *SynchronizationDialog)
    {
        SynchronizationDialog->setWindowTitle(QApplication::translate("SynchronizationDialog", "Synchronization Dialog", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("SynchronizationDialog", "Server", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("SynchronizationDialog", "Last synchronization", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("SynchronizationDialog", "Status", Q_NULLPTR));
        synchronizeButton->setText(QApplication::translate("SynchronizationDialog", "Synchronize", Q_NULLPTR));
        closeButton->setText(QApplication::translate("SynchronizationDialog", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SynchronizationDialog: public Ui_SynchronizationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYNCHRONIZATIONDIALOG_H
