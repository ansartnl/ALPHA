/********************************************************************************
** Form generated from reading UI file 'holdingeditor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOLDINGEDITOR_H
#define UI_HOLDINGEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HoldingEditor
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *firLabel;
    QLabel *label;
    QTableView *holdingView;
    QHBoxLayout *horizontalLayout;
    QToolButton *addButton;
    QToolButton *deleteButton;
    QDialogButtonBox *operButtons;

    void setupUi(QWidget *HoldingEditor)
    {
        if (HoldingEditor->objectName().isEmpty())
            HoldingEditor->setObjectName(QStringLiteral("HoldingEditor"));
        HoldingEditor->resize(866, 606);
        verticalLayout = new QVBoxLayout(HoldingEditor);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        firLabel = new QLabel(HoldingEditor);
        firLabel->setObjectName(QStringLiteral("firLabel"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        firLabel->setFont(font);
        firLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(firLabel);

        label = new QLabel(HoldingEditor);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font);

        verticalLayout->addWidget(label);

        holdingView = new QTableView(HoldingEditor);
        holdingView->setObjectName(QStringLiteral("holdingView"));
        holdingView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        holdingView->setSelectionBehavior(QAbstractItemView::SelectRows);
        holdingView->setSortingEnabled(true);
        holdingView->horizontalHeader()->setStretchLastSection(true);
        holdingView->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(holdingView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        addButton = new QToolButton(HoldingEditor);
        addButton->setObjectName(QStringLiteral("addButton"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(addButton->sizePolicy().hasHeightForWidth());
        addButton->setSizePolicy(sizePolicy);
        addButton->setMinimumSize(QSize(33, 36));

        horizontalLayout->addWidget(addButton);

        deleteButton = new QToolButton(HoldingEditor);
        deleteButton->setObjectName(QStringLiteral("deleteButton"));
        sizePolicy.setHeightForWidth(deleteButton->sizePolicy().hasHeightForWidth());
        deleteButton->setSizePolicy(sizePolicy);
        deleteButton->setMinimumSize(QSize(33, 36));

        horizontalLayout->addWidget(deleteButton);

        operButtons = new QDialogButtonBox(HoldingEditor);
        operButtons->setObjectName(QStringLiteral("operButtons"));
        operButtons->setEnabled(false);
        operButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

        horizontalLayout->addWidget(operButtons);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(holdingView, operButtons);

        retranslateUi(HoldingEditor);

        QMetaObject::connectSlotsByName(HoldingEditor);
    } // setupUi

    void retranslateUi(QWidget *HoldingEditor)
    {
        HoldingEditor->setWindowTitle(QApplication::translate("HoldingEditor", "Holding editor", Q_NULLPTR));
        firLabel->setText(QString());
        label->setText(QApplication::translate("HoldingEditor", "Holding zone", Q_NULLPTR));
        addButton->setText(QApplication::translate("HoldingEditor", "+", Q_NULLPTR));
        deleteButton->setText(QApplication::translate("HoldingEditor", "-", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class HoldingEditor: public Ui_HoldingEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOLDINGEDITOR_H
