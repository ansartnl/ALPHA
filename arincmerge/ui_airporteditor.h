/********************************************************************************
** Form generated from reading UI file 'airporteditor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AIRPORTEDITOR_H
#define UI_AIRPORTEDITOR_H

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

class Ui_AirportEditor
{
public:
    QVBoxLayout *verticalLayout_4;
    QLabel *firLabel;
    QSplitter *splitter_2;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QTableView *airportsView;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QToolButton *addFirAirportButton;
    QToolButton *deleteFirAirportButton;
    QSpacerItem *verticalSpacer_2;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QTableView *airportsByFirView;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *addAirportButton;
    QToolButton *deleteAirportButton;
    QDialogButtonBox *operButtons;

    void setupUi(QWidget *AirportEditor)
    {
        if (AirportEditor->objectName().isEmpty())
            AirportEditor->setObjectName(QStringLiteral("AirportEditor"));
        AirportEditor->resize(690, 516);
        verticalLayout_4 = new QVBoxLayout(AirportEditor);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        firLabel = new QLabel(AirportEditor);
        firLabel->setObjectName(QStringLiteral("firLabel"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        firLabel->setFont(font);
        firLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(firLabel);

        splitter_2 = new QSplitter(AirportEditor);
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
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font);

        verticalLayout_3->addWidget(label);

        airportsView = new QTableView(layoutWidget);
        airportsView->setObjectName(QStringLiteral("airportsView"));
        airportsView->setSelectionBehavior(QAbstractItemView::SelectRows);
        airportsView->setSortingEnabled(true);
        airportsView->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_3->addWidget(airportsView);

        splitter_2->addWidget(layoutWidget);
        widget = new QWidget(splitter_2);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        widget->setMinimumSize(QSize(51, 0));
        widget->setMaximumSize(QSize(51, 16777215));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        addFirAirportButton = new QToolButton(widget);
        addFirAirportButton->setObjectName(QStringLiteral("addFirAirportButton"));
        addFirAirportButton->setEnabled(false);
        addFirAirportButton->setMinimumSize(QSize(33, 36));
        addFirAirportButton->setArrowType(Qt::RightArrow);

        verticalLayout_2->addWidget(addFirAirportButton);

        deleteFirAirportButton = new QToolButton(widget);
        deleteFirAirportButton->setObjectName(QStringLiteral("deleteFirAirportButton"));
        deleteFirAirportButton->setEnabled(false);
        deleteFirAirportButton->setMinimumSize(QSize(33, 36));
        deleteFirAirportButton->setArrowType(Qt::LeftArrow);

        verticalLayout_2->addWidget(deleteFirAirportButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        splitter_2->addWidget(widget);
        layoutWidget_2 = new QWidget(splitter_2);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        verticalLayout = new QVBoxLayout(layoutWidget_2);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);

        verticalLayout->addWidget(label_2);

        airportsByFirView = new QTableView(layoutWidget_2);
        airportsByFirView->setObjectName(QStringLiteral("airportsByFirView"));
        airportsByFirView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        airportsByFirView->setSelectionBehavior(QAbstractItemView::SelectRows);
        airportsByFirView->setSortingEnabled(true);
        airportsByFirView->horizontalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(airportsByFirView);

        splitter_2->addWidget(layoutWidget_2);

        verticalLayout_4->addWidget(splitter_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        addAirportButton = new QToolButton(AirportEditor);
        addAirportButton->setObjectName(QStringLiteral("addAirportButton"));
        addAirportButton->setMinimumSize(QSize(33, 36));

        horizontalLayout_4->addWidget(addAirportButton);

        deleteAirportButton = new QToolButton(AirportEditor);
        deleteAirportButton->setObjectName(QStringLiteral("deleteAirportButton"));
        deleteAirportButton->setMinimumSize(QSize(33, 36));

        horizontalLayout_4->addWidget(deleteAirportButton);

        operButtons = new QDialogButtonBox(AirportEditor);
        operButtons->setObjectName(QStringLiteral("operButtons"));
        operButtons->setEnabled(false);
        operButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

        horizontalLayout_4->addWidget(operButtons);


        verticalLayout_4->addLayout(horizontalLayout_4);


        retranslateUi(AirportEditor);

        QMetaObject::connectSlotsByName(AirportEditor);
    } // setupUi

    void retranslateUi(QWidget *AirportEditor)
    {
        AirportEditor->setWindowTitle(QApplication::translate("AirportEditor", "Airport editor", Q_NULLPTR));
        label->setText(QApplication::translate("AirportEditor", "All airports", Q_NULLPTR));
        label_2->setText(QApplication::translate("AirportEditor", "FIR airports", Q_NULLPTR));
        addAirportButton->setText(QApplication::translate("AirportEditor", "+", Q_NULLPTR));
        deleteAirportButton->setText(QApplication::translate("AirportEditor", "-", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AirportEditor: public Ui_AirportEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AIRPORTEDITOR_H
