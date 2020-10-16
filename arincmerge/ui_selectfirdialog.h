/********************************************************************************
** Form generated from reading UI file 'selectfirdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTFIRDIALOG_H
#define UI_SELECTFIRDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SelectFirDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *view;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *selectButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *SelectFirDialog)
    {
        if (SelectFirDialog->objectName().isEmpty())
            SelectFirDialog->setObjectName(QStringLiteral("SelectFirDialog"));
        SelectFirDialog->resize(431, 341);
        verticalLayout = new QVBoxLayout(SelectFirDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        view = new QTableView(SelectFirDialog);
        view->setObjectName(QStringLiteral("view"));
        view->setEditTriggers(QAbstractItemView::NoEditTriggers);
        view->setSelectionMode(QAbstractItemView::SingleSelection);
        view->setSelectionBehavior(QAbstractItemView::SelectRows);
        view->horizontalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(view);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        selectButton = new QPushButton(SelectFirDialog);
        selectButton->setObjectName(QStringLiteral("selectButton"));

        horizontalLayout->addWidget(selectButton);

        cancelButton = new QPushButton(SelectFirDialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(SelectFirDialog);

        QMetaObject::connectSlotsByName(SelectFirDialog);
    } // setupUi

    void retranslateUi(QDialog *SelectFirDialog)
    {
        SelectFirDialog->setWindowTitle(QApplication::translate("SelectFirDialog", "Select FIR", Q_NULLPTR));
        selectButton->setText(QApplication::translate("SelectFirDialog", "Select", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("SelectFirDialog", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SelectFirDialog: public Ui_SelectFirDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTFIRDIALOG_H
