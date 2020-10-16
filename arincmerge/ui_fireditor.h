/********************************************************************************
** Form generated from reading UI file 'fireditor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIREDITOR_H
#define UI_FIREDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FirEditor
{
public:
    QVBoxLayout *verticalLayout_3;
    QSplitter *splitter_2;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTableView *firView;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QToolButton *addFirButton;
    QToolButton *deleteFirButton;
    QSpacerItem *horizontalSpacer_2;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QTableView *edgeView;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *addPointButton;
    QToolButton *deletePointButton;
    QSpacerItem *horizontalSpacer_4;
    QDialogButtonBox *operButtons;

    void setupUi(QWidget *FirEditor)
    {
        if (FirEditor->objectName().isEmpty())
            FirEditor->setObjectName(QStringLiteral("FirEditor"));
        FirEditor->resize(916, 655);
        verticalLayout_3 = new QVBoxLayout(FirEditor);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        splitter_2 = new QSplitter(FirEditor);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        splitter_2->setChildrenCollapsible(false);
        layoutWidget = new QWidget(splitter_2);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        verticalLayout->addWidget(label);

        firView = new QTableView(layoutWidget);
        firView->setObjectName(QStringLiteral("firView"));
        firView->setSelectionBehavior(QAbstractItemView::SelectRows);
        firView->setSortingEnabled(true);
        firView->setWordWrap(false);
        firView->horizontalHeader()->setCascadingSectionResizes(true);
        firView->horizontalHeader()->setStretchLastSection(true);
        firView->verticalHeader()->setCascadingSectionResizes(true);

        verticalLayout->addWidget(firView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        addFirButton = new QToolButton(layoutWidget);
        addFirButton->setObjectName(QStringLiteral("addFirButton"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(addFirButton->sizePolicy().hasHeightForWidth());
        addFirButton->setSizePolicy(sizePolicy);
        addFirButton->setMinimumSize(QSize(33, 36));

        horizontalLayout->addWidget(addFirButton);

        deleteFirButton = new QToolButton(layoutWidget);
        deleteFirButton->setObjectName(QStringLiteral("deleteFirButton"));
        sizePolicy.setHeightForWidth(deleteFirButton->sizePolicy().hasHeightForWidth());
        deleteFirButton->setSizePolicy(sizePolicy);
        deleteFirButton->setMinimumSize(QSize(33, 36));

        horizontalLayout->addWidget(deleteFirButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        splitter_2->addWidget(layoutWidget);
        layoutWidget_2 = new QWidget(splitter_2);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        verticalLayout_2 = new QVBoxLayout(layoutWidget_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);

        verticalLayout_2->addWidget(label_2);

        edgeView = new QTableView(layoutWidget_2);
        edgeView->setObjectName(QStringLiteral("edgeView"));
        edgeView->setSelectionBehavior(QAbstractItemView::SelectRows);
        edgeView->setSortingEnabled(true);
        edgeView->setWordWrap(false);
        edgeView->horizontalHeader()->setCascadingSectionResizes(true);
        edgeView->horizontalHeader()->setStretchLastSection(true);
        edgeView->verticalHeader()->setVisible(false);
        edgeView->verticalHeader()->setCascadingSectionResizes(true);

        verticalLayout_2->addWidget(edgeView);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        addPointButton = new QToolButton(layoutWidget_2);
        addPointButton->setObjectName(QStringLiteral("addPointButton"));
        addPointButton->setEnabled(false);
        sizePolicy.setHeightForWidth(addPointButton->sizePolicy().hasHeightForWidth());
        addPointButton->setSizePolicy(sizePolicy);
        addPointButton->setMinimumSize(QSize(33, 36));

        horizontalLayout_2->addWidget(addPointButton);

        deletePointButton = new QToolButton(layoutWidget_2);
        deletePointButton->setObjectName(QStringLiteral("deletePointButton"));
        deletePointButton->setEnabled(true);
        sizePolicy.setHeightForWidth(deletePointButton->sizePolicy().hasHeightForWidth());
        deletePointButton->setSizePolicy(sizePolicy);
        deletePointButton->setMinimumSize(QSize(33, 36));

        horizontalLayout_2->addWidget(deletePointButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout_2->addLayout(horizontalLayout_2);

        splitter_2->addWidget(layoutWidget_2);

        verticalLayout_3->addWidget(splitter_2);

        operButtons = new QDialogButtonBox(FirEditor);
        operButtons->setObjectName(QStringLiteral("operButtons"));
        operButtons->setEnabled(false);
        operButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

        verticalLayout_3->addWidget(operButtons);


        retranslateUi(FirEditor);

        QMetaObject::connectSlotsByName(FirEditor);
    } // setupUi

    void retranslateUi(QWidget *FirEditor)
    {
        FirEditor->setWindowTitle(QApplication::translate("FirEditor", "FIR editor", Q_NULLPTR));
        label->setText(QApplication::translate("FirEditor", "FIR", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        addFirButton->setToolTip(QApplication::translate("FirEditor", "Add empty FIR", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        addFirButton->setText(QApplication::translate("FirEditor", "+", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        deleteFirButton->setToolTip(QApplication::translate("FirEditor", "Delete selected FIR", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        deleteFirButton->setText(QApplication::translate("FirEditor", "-", Q_NULLPTR));
        label_2->setText(QApplication::translate("FirEditor", "Points", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        addPointButton->setToolTip(QApplication::translate("FirEditor", "Add empty point", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        addPointButton->setText(QApplication::translate("FirEditor", "+", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        deletePointButton->setToolTip(QApplication::translate("FirEditor", "Delete selected point", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        deletePointButton->setText(QApplication::translate("FirEditor", "-", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FirEditor: public Ui_FirEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIREDITOR_H
