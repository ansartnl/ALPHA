/********************************************************************************
** Form generated from reading UI file 'ArincParserDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARINCPARSERDIALOG_H
#define UI_ARINCPARSERDIALOG_H

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

class Ui_ArincParserDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *stateLabel;
    QProgressBar *progressBar;
    QDialogButtonBox *operButtons;

    void setupUi(QDialog *ArincParserDialog)
    {
        if (ArincParserDialog->objectName().isEmpty())
            ArincParserDialog->setObjectName(QStringLiteral("ArincParserDialog"));
        ArincParserDialog->resize(352, 87);
        ArincParserDialog->setMaximumSize(QSize(16777215, 87));
        verticalLayout = new QVBoxLayout(ArincParserDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        stateLabel = new QLabel(ArincParserDialog);
        stateLabel->setObjectName(QStringLiteral("stateLabel"));
        stateLabel->setText(QStringLiteral("#State"));

        verticalLayout->addWidget(stateLabel);

        progressBar = new QProgressBar(ArincParserDialog);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setMaximum(0);
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);

        operButtons = new QDialogButtonBox(ArincParserDialog);
        operButtons->setObjectName(QStringLiteral("operButtons"));
        operButtons->setOrientation(Qt::Horizontal);
        operButtons->setStandardButtons(QDialogButtonBox::Cancel);

        verticalLayout->addWidget(operButtons);


        retranslateUi(ArincParserDialog);

        QMetaObject::connectSlotsByName(ArincParserDialog);
    } // setupUi

    void retranslateUi(QDialog *ArincParserDialog)
    {
        ArincParserDialog->setWindowTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class ArincParserDialog: public Ui_ArincParserDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARINCPARSERDIALOG_H
