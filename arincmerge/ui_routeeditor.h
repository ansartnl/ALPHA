/********************************************************************************
** Form generated from reading UI file 'routeeditor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROUTEEDITOR_H
#define UI_ROUTEEDITOR_H

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

class Ui_RouteEditor
{
public:
    QVBoxLayout *verticalLayout_5;
    QLabel *firLabel;
    QSplitter *splitter;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTableView *pointsView;
    QWidget *widget;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer;
    QToolButton *addRoutePointButton;
    QToolButton *deleteRoutePointButton;
    QSpacerItem *verticalSpacer_3;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QTableView *routeView;
    QVBoxLayout *verticalLayout_3;
    QToolButton *addRouteButton;
    QToolButton *deleteRouteButton;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_3;
    QTableView *routePointView;
    QDialogButtonBox *operButtons;

    void setupUi(QWidget *RouteEditor)
    {
        if (RouteEditor->objectName().isEmpty())
            RouteEditor->setObjectName(QStringLiteral("RouteEditor"));
        RouteEditor->resize(853, 643);
        verticalLayout_5 = new QVBoxLayout(RouteEditor);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        firLabel = new QLabel(RouteEditor);
        firLabel->setObjectName(QStringLiteral("firLabel"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        firLabel->setFont(font);
        firLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(firLabel);

        splitter = new QSplitter(RouteEditor);
        splitter->setObjectName(QStringLiteral("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setChildrenCollapsible(false);
        widget_3 = new QWidget(splitter);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        verticalLayout = new QVBoxLayout(widget_3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget_3);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font);

        verticalLayout->addWidget(label);

        pointsView = new QTableView(widget_3);
        pointsView->setObjectName(QStringLiteral("pointsView"));
        pointsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        pointsView->setSelectionBehavior(QAbstractItemView::SelectRows);
        pointsView->setSortingEnabled(true);
        pointsView->horizontalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(pointsView);

        splitter->addWidget(widget_3);
        widget = new QWidget(splitter);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(51, 0));
        widget->setMaximumSize(QSize(51, 16777215));
        verticalLayout_4 = new QVBoxLayout(widget);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        addRoutePointButton = new QToolButton(widget);
        addRoutePointButton->setObjectName(QStringLiteral("addRoutePointButton"));
        addRoutePointButton->setEnabled(false);
        addRoutePointButton->setMinimumSize(QSize(33, 36));
        addRoutePointButton->setArrowType(Qt::RightArrow);

        verticalLayout_4->addWidget(addRoutePointButton);

        deleteRoutePointButton = new QToolButton(widget);
        deleteRoutePointButton->setObjectName(QStringLiteral("deleteRoutePointButton"));
        deleteRoutePointButton->setEnabled(false);
        deleteRoutePointButton->setMinimumSize(QSize(33, 36));
        deleteRoutePointButton->setArrowType(Qt::LeftArrow);

        verticalLayout_4->addWidget(deleteRoutePointButton);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        verticalLayout_4->setStretch(0, 4);
        verticalLayout_4->setStretch(3, 1);
        splitter->addWidget(widget);
        widget_2 = new QWidget(splitter);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);

        verticalLayout_2->addWidget(label_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        routeView = new QTableView(widget_2);
        routeView->setObjectName(QStringLiteral("routeView"));
        routeView->setSelectionBehavior(QAbstractItemView::SelectRows);
        routeView->setSortingEnabled(true);

        horizontalLayout->addWidget(routeView);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        addRouteButton = new QToolButton(widget_2);
        addRouteButton->setObjectName(QStringLiteral("addRouteButton"));
        addRouteButton->setEnabled(false);
        addRouteButton->setMinimumSize(QSize(33, 36));

        verticalLayout_3->addWidget(addRouteButton);

        deleteRouteButton = new QToolButton(widget_2);
        deleteRouteButton->setObjectName(QStringLiteral("deleteRouteButton"));
        deleteRouteButton->setEnabled(false);
        deleteRouteButton->setMinimumSize(QSize(33, 36));

        verticalLayout_3->addWidget(deleteRouteButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout_2->addLayout(horizontalLayout);

        label_3 = new QLabel(widget_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);

        verticalLayout_2->addWidget(label_3);

        routePointView = new QTableView(widget_2);
        routePointView->setObjectName(QStringLiteral("routePointView"));
        routePointView->setSelectionBehavior(QAbstractItemView::SelectRows);
        routePointView->setSortingEnabled(true);

        verticalLayout_2->addWidget(routePointView);

        splitter->addWidget(widget_2);

        verticalLayout_5->addWidget(splitter);

        operButtons = new QDialogButtonBox(RouteEditor);
        operButtons->setObjectName(QStringLiteral("operButtons"));
        operButtons->setEnabled(false);
        operButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

        verticalLayout_5->addWidget(operButtons);


        retranslateUi(RouteEditor);

        QMetaObject::connectSlotsByName(RouteEditor);
    } // setupUi

    void retranslateUi(QWidget *RouteEditor)
    {
        RouteEditor->setWindowTitle(QApplication::translate("RouteEditor", "Route editor", Q_NULLPTR));
        label->setText(QApplication::translate("RouteEditor", "FIR points", Q_NULLPTR));
        label_2->setText(QApplication::translate("RouteEditor", "Routes", Q_NULLPTR));
        addRouteButton->setText(QApplication::translate("RouteEditor", "+", Q_NULLPTR));
        deleteRouteButton->setText(QApplication::translate("RouteEditor", "-", Q_NULLPTR));
        label_3->setText(QApplication::translate("RouteEditor", "Route points", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RouteEditor: public Ui_RouteEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROUTEEDITOR_H
