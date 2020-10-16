/********************************************************************************
** Form generated from reading UI file 'importarincdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPORTARINCDIALOG_H
#define UI_IMPORTARINCDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ImportArincDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ImportArincDialog)
    {
        if (ImportArincDialog->objectName().isEmpty())
            ImportArincDialog->setObjectName(QStringLiteral("ImportArincDialog"));
        ImportArincDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(ImportArincDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableWidget = new QTableWidget(ImportArincDialog);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget->setSortingEnabled(true);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setDefaultSectionSize(17);

        verticalLayout->addWidget(tableWidget);

        buttonBox = new QDialogButtonBox(ImportArincDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ImportArincDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ImportArincDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ImportArincDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ImportArincDialog);
    } // setupUi

    void retranslateUi(QDialog *ImportArincDialog)
    {
        ImportArincDialog->setWindowTitle(QApplication::translate("ImportArincDialog", "Select FIR", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ImportArincDialog: public Ui_ImportArincDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPORTARINCDIALOG_H
