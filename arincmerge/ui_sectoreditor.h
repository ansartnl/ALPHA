/********************************************************************************
** Form generated from reading UI file 'sectoreditor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECTOREDITOR_H
#define UI_SECTOREDITOR_H

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

class Ui_SectorEditor
{
public:
    QVBoxLayout *verticalLayout_3;
    QLabel *firLabel;
    QSplitter *splitter_2;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTableView *sectorView;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *addSectorButton;
    QToolButton *deleteSectorButton;
    QSpacerItem *horizontalSpacer_3;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QTableView *edgeView;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_4;
    QToolButton *addPointButton;
    QToolButton *deletePointButton;
    QSpacerItem *horizontalSpacer_5;
    QDialogButtonBox *operButtons;

    void setupUi(QWidget *SectorEditor)
    {
        if (SectorEditor->objectName().isEmpty())
            SectorEditor->setObjectName(QStringLiteral("SectorEditor"));
        SectorEditor->resize(978, 649);
        verticalLayout_3 = new QVBoxLayout(SectorEditor);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        firLabel = new QLabel(SectorEditor);
        firLabel->setObjectName(QStringLiteral("firLabel"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        firLabel->setFont(font);
        firLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(firLabel);

        splitter_2 = new QSplitter(SectorEditor);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter_2->sizePolicy().hasHeightForWidth());
        splitter_2->setSizePolicy(sizePolicy);
        splitter_2->setOrientation(Qt::Horizontal);
        splitter_2->setChildrenCollapsible(false);
        layoutWidget = new QWidget(splitter_2);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font);

        verticalLayout->addWidget(label);

        sectorView = new QTableView(layoutWidget);
        sectorView->setObjectName(QStringLiteral("sectorView"));
        sectorView->setSelectionBehavior(QAbstractItemView::SelectRows);
        sectorView->setSortingEnabled(true);
        sectorView->setWordWrap(false);
        sectorView->horizontalHeader()->setCascadingSectionResizes(true);
        sectorView->horizontalHeader()->setStretchLastSection(true);
        sectorView->verticalHeader()->setCascadingSectionResizes(true);

        verticalLayout->addWidget(sectorView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        addSectorButton = new QToolButton(layoutWidget);
        addSectorButton->setObjectName(QStringLiteral("addSectorButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(addSectorButton->sizePolicy().hasHeightForWidth());
        addSectorButton->setSizePolicy(sizePolicy1);
        addSectorButton->setMinimumSize(QSize(33, 36));

        horizontalLayout->addWidget(addSectorButton);

        deleteSectorButton = new QToolButton(layoutWidget);
        deleteSectorButton->setObjectName(QStringLiteral("deleteSectorButton"));
        sizePolicy1.setHeightForWidth(deleteSectorButton->sizePolicy().hasHeightForWidth());
        deleteSectorButton->setSizePolicy(sizePolicy1);
        deleteSectorButton->setMinimumSize(QSize(33, 36));

        horizontalLayout->addWidget(deleteSectorButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


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

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        addPointButton = new QToolButton(layoutWidget_2);
        addPointButton->setObjectName(QStringLiteral("addPointButton"));
        addPointButton->setEnabled(false);
        sizePolicy1.setHeightForWidth(addPointButton->sizePolicy().hasHeightForWidth());
        addPointButton->setSizePolicy(sizePolicy1);
        addPointButton->setMinimumSize(QSize(33, 36));

        horizontalLayout_4->addWidget(addPointButton);

        deletePointButton = new QToolButton(layoutWidget_2);
        deletePointButton->setObjectName(QStringLiteral("deletePointButton"));
        deletePointButton->setEnabled(false);
        sizePolicy1.setHeightForWidth(deletePointButton->sizePolicy().hasHeightForWidth());
        deletePointButton->setSizePolicy(sizePolicy1);
        deletePointButton->setMinimumSize(QSize(33, 36));

        horizontalLayout_4->addWidget(deletePointButton);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);


        verticalLayout_2->addLayout(horizontalLayout_4);

        splitter_2->addWidget(layoutWidget_2);

        verticalLayout_3->addWidget(splitter_2);

        operButtons = new QDialogButtonBox(SectorEditor);
        operButtons->setObjectName(QStringLiteral("operButtons"));
        operButtons->setEnabled(false);
        operButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

        verticalLayout_3->addWidget(operButtons);


        retranslateUi(SectorEditor);

        QMetaObject::connectSlotsByName(SectorEditor);
    } // setupUi

    void retranslateUi(QWidget *SectorEditor)
    {
        SectorEditor->setWindowTitle(QApplication::translate("SectorEditor", "Sector editor", Q_NULLPTR));
        label->setText(QApplication::translate("SectorEditor", "ATM sectors", Q_NULLPTR));
        addSectorButton->setText(QApplication::translate("SectorEditor", "+", Q_NULLPTR));
        deleteSectorButton->setText(QApplication::translate("SectorEditor", "-", Q_NULLPTR));
        label_2->setText(QApplication::translate("SectorEditor", "Points", Q_NULLPTR));
        addPointButton->setText(QApplication::translate("SectorEditor", "+", Q_NULLPTR));
        deletePointButton->setText(QApplication::translate("SectorEditor", "-", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SectorEditor: public Ui_SectorEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECTOREDITOR_H
