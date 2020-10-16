/********************************************************************************
** Form generated from reading UI file 'pointeditor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POINTEDITOR_H
#define UI_POINTEDITOR_H

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

class Ui_PointEditor
{
public:
    QVBoxLayout *verticalLayout_4;
    QLabel *firLabel;
    QSplitter *splitter_2;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QTableView *pointsView;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QToolButton *addFirPointButton;
    QToolButton *deleteFirPointButton;
    QSpacerItem *verticalSpacer;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QTableView *firPointsView;
    QHBoxLayout *horizontalLayout;
    QToolButton *addPointButton;
    QToolButton *deletePointButton;
    QDialogButtonBox *operButtons;

    void setupUi(QWidget *PointEditor)
    {
        if (PointEditor->objectName().isEmpty())
            PointEditor->setObjectName(QStringLiteral("PointEditor"));
        PointEditor->resize(700, 608);
        verticalLayout_4 = new QVBoxLayout(PointEditor);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        firLabel = new QLabel(PointEditor);
        firLabel->setObjectName(QStringLiteral("firLabel"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        firLabel->setFont(font);
        firLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(firLabel);

        splitter_2 = new QSplitter(PointEditor);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter_2->sizePolicy().hasHeightForWidth());
        splitter_2->setSizePolicy(sizePolicy);
        splitter_2->setOrientation(Qt::Horizontal);
        splitter_2->setChildrenCollapsible(false);
        widget_3 = new QWidget(splitter_2);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        verticalLayout_2 = new QVBoxLayout(widget_3);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget_3);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font);

        verticalLayout_2->addWidget(label);

        pointsView = new QTableView(widget_3);
        pointsView->setObjectName(QStringLiteral("pointsView"));
        pointsView->setSelectionBehavior(QAbstractItemView::SelectRows);
        pointsView->setSortingEnabled(true);
        pointsView->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_2->addWidget(pointsView);

        splitter_2->addWidget(widget_3);
        widget = new QWidget(splitter_2);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(51, 0));
        widget->setMaximumSize(QSize(51, 16777215));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        addFirPointButton = new QToolButton(widget);
        addFirPointButton->setObjectName(QStringLiteral("addFirPointButton"));
        addFirPointButton->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(addFirPointButton->sizePolicy().hasHeightForWidth());
        addFirPointButton->setSizePolicy(sizePolicy1);
        addFirPointButton->setMinimumSize(QSize(33, 36));
        addFirPointButton->setArrowType(Qt::RightArrow);

        verticalLayout->addWidget(addFirPointButton);

        deleteFirPointButton = new QToolButton(widget);
        deleteFirPointButton->setObjectName(QStringLiteral("deleteFirPointButton"));
        deleteFirPointButton->setEnabled(false);
        sizePolicy1.setHeightForWidth(deleteFirPointButton->sizePolicy().hasHeightForWidth());
        deleteFirPointButton->setSizePolicy(sizePolicy1);
        deleteFirPointButton->setMinimumSize(QSize(33, 36));
        deleteFirPointButton->setArrowType(Qt::LeftArrow);

        verticalLayout->addWidget(deleteFirPointButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        splitter_2->addWidget(widget);
        widget_2 = new QWidget(splitter_2);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        verticalLayout_3 = new QVBoxLayout(widget_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);

        verticalLayout_3->addWidget(label_2);

        firPointsView = new QTableView(widget_2);
        firPointsView->setObjectName(QStringLiteral("firPointsView"));
        firPointsView->setSelectionBehavior(QAbstractItemView::SelectRows);
        firPointsView->setSortingEnabled(true);
        firPointsView->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_3->addWidget(firPointsView);

        splitter_2->addWidget(widget_2);

        verticalLayout_4->addWidget(splitter_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        addPointButton = new QToolButton(PointEditor);
        addPointButton->setObjectName(QStringLiteral("addPointButton"));
        sizePolicy1.setHeightForWidth(addPointButton->sizePolicy().hasHeightForWidth());
        addPointButton->setSizePolicy(sizePolicy1);
        addPointButton->setMinimumSize(QSize(33, 36));

        horizontalLayout->addWidget(addPointButton);

        deletePointButton = new QToolButton(PointEditor);
        deletePointButton->setObjectName(QStringLiteral("deletePointButton"));
        sizePolicy1.setHeightForWidth(deletePointButton->sizePolicy().hasHeightForWidth());
        deletePointButton->setSizePolicy(sizePolicy1);
        deletePointButton->setMinimumSize(QSize(33, 36));

        horizontalLayout->addWidget(deletePointButton);

        operButtons = new QDialogButtonBox(PointEditor);
        operButtons->setObjectName(QStringLiteral("operButtons"));
        operButtons->setEnabled(false);
        operButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

        horizontalLayout->addWidget(operButtons);


        verticalLayout_4->addLayout(horizontalLayout);


        retranslateUi(PointEditor);

        QMetaObject::connectSlotsByName(PointEditor);
    } // setupUi

    void retranslateUi(QWidget *PointEditor)
    {
        PointEditor->setWindowTitle(QApplication::translate("PointEditor", "Point editor", Q_NULLPTR));
        label->setText(QApplication::translate("PointEditor", "Points", Q_NULLPTR));
        label_2->setText(QApplication::translate("PointEditor", "FIR points", Q_NULLPTR));
        addPointButton->setText(QApplication::translate("PointEditor", "+", Q_NULLPTR));
        deletePointButton->setText(QApplication::translate("PointEditor", "-", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PointEditor: public Ui_PointEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POINTEDITOR_H
