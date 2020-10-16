/********************************************************************************
** Form generated from reading UI file 'ArincDatabaseImporterDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARINCDATABASEIMPORTERDIALOG_H
#define UI_ARINCDATABASEIMPORTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ArincDatabaseImporterDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *stateLabel;
    QProgressBar *progressBar;
    QDialogButtonBox *operButtons;

    void setupUi(QDialog *ArincDatabaseImporterDialog)
    {
        if (ArincDatabaseImporterDialog->objectName().isEmpty())
            ArincDatabaseImporterDialog->setObjectName(QStringLiteral("ArincDatabaseImporterDialog"));
        ArincDatabaseImporterDialog->resize(352, 87);
        ArincDatabaseImporterDialog->setMaximumSize(QSize(16777215, 87));
        verticalLayout = new QVBoxLayout(ArincDatabaseImporterDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        stateLabel = new QLabel(ArincDatabaseImporterDialog);
        stateLabel->setObjectName(QStringLiteral("stateLabel"));
        stateLabel->setText(QStringLiteral("#State"));

        verticalLayout->addWidget(stateLabel);

        progressBar = new QProgressBar(ArincDatabaseImporterDialog);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setMaximum(0);
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);

        operButtons = new QDialogButtonBox(ArincDatabaseImporterDialog);
        operButtons->setObjectName(QStringLiteral("operButtons"));
        operButtons->setOrientation(Qt::Horizontal);
        operButtons->setStandardButtons(QDialogButtonBox::Cancel);

        verticalLayout->addWidget(operButtons);


        retranslateUi(ArincDatabaseImporterDialog);

        QMetaObject::connectSlotsByName(ArincDatabaseImporterDialog);
    } // setupUi

    void retranslateUi(QDialog *ArincDatabaseImporterDialog)
    {
        ArincDatabaseImporterDialog->setWindowTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class ArincDatabaseImporterDialog: public Ui_ArincDatabaseImporterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARINCDATABASEIMPORTERDIALOG_H
