/********************************************************************************
** Form generated from reading UI file 'acdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACDIALOG_H
#define UI_ACDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ACDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *tableView;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ACDialog)
    {
        if (ACDialog->objectName().isEmpty())
            ACDialog->setObjectName(QStringLiteral("ACDialog"));
        ACDialog->resize(712, 251);
        verticalLayout = new QVBoxLayout(ACDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableView = new QTableView(ACDialog);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableView->setSortingEnabled(true);

        verticalLayout->addWidget(tableView);

        buttonBox = new QDialogButtonBox(ACDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close|QDialogButtonBox::Save|QDialogButtonBox::SaveAll);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ACDialog);

        QMetaObject::connectSlotsByName(ACDialog);
    } // setupUi

    void retranslateUi(QDialog *ACDialog)
    {
        ACDialog->setWindowTitle(QApplication::translate("ACDialog", "Arinc changes", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ACDialog: public Ui_ACDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACDIALOG_H
