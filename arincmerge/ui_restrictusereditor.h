/********************************************************************************
** Form generated from reading UI file 'restrictusereditor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESTRICTUSEREDITOR_H
#define UI_RESTRICTUSEREDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RestrictUserEditor
{
public:
    QVBoxLayout *verticalLayout_3;
    QSplitter *splitter;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QComboBox *firCombo;
    QSpacerItem *horizontalSpacer;
    QTableView *restrictionsView;
    QToolButton *addRestrictionButton;
    QToolButton *deleteRestrictionButton;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QTableView *pointView;
    QToolButton *addPointButton;
    QToolButton *deletePointButtun;
    QToolButton *toolButtonPastePolygon;
    QToolButton *toolButtonPasteCircle;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *operButtons;

    void setupUi(QWidget *RestrictUserEditor)
    {
        if (RestrictUserEditor->objectName().isEmpty())
            RestrictUserEditor->setObjectName(QStringLiteral("RestrictUserEditor"));
        RestrictUserEditor->resize(635, 576);
        verticalLayout_3 = new QVBoxLayout(RestrictUserEditor);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        splitter = new QSplitter(RestrictUserEditor);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        splitter->setChildrenCollapsible(false);
        groupBox = new QGroupBox(splitter);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        firCombo = new QComboBox(groupBox);
        firCombo->setObjectName(QStringLiteral("firCombo"));

        gridLayout_2->addWidget(firCombo, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(469, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 1, 1, 1);

        restrictionsView = new QTableView(groupBox);
        restrictionsView->setObjectName(QStringLiteral("restrictionsView"));
        restrictionsView->setSelectionBehavior(QAbstractItemView::SelectRows);
        restrictionsView->setSortingEnabled(true);
        restrictionsView->setWordWrap(false);

        gridLayout_2->addWidget(restrictionsView, 1, 0, 3, 2);

        addRestrictionButton = new QToolButton(groupBox);
        addRestrictionButton->setObjectName(QStringLiteral("addRestrictionButton"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(addRestrictionButton->sizePolicy().hasHeightForWidth());
        addRestrictionButton->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(addRestrictionButton, 1, 2, 1, 1);

        deleteRestrictionButton = new QToolButton(groupBox);
        deleteRestrictionButton->setObjectName(QStringLiteral("deleteRestrictionButton"));
        sizePolicy.setHeightForWidth(deleteRestrictionButton->sizePolicy().hasHeightForWidth());
        deleteRestrictionButton->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(deleteRestrictionButton, 2, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 162, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 3, 2, 1, 1);

        splitter->addWidget(groupBox);
        groupBox_2 = new QGroupBox(splitter);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pointView = new QTableView(groupBox_2);
        pointView->setObjectName(QStringLiteral("pointView"));
        pointView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        pointView->setSelectionBehavior(QAbstractItemView::SelectRows);
        pointView->setSortingEnabled(true);
        pointView->setWordWrap(false);

        gridLayout->addWidget(pointView, 0, 0, 5, 1);

        addPointButton = new QToolButton(groupBox_2);
        addPointButton->setObjectName(QStringLiteral("addPointButton"));
        sizePolicy.setHeightForWidth(addPointButton->sizePolicy().hasHeightForWidth());
        addPointButton->setSizePolicy(sizePolicy);

        gridLayout->addWidget(addPointButton, 0, 1, 1, 1);

        deletePointButtun = new QToolButton(groupBox_2);
        deletePointButtun->setObjectName(QStringLiteral("deletePointButtun"));
        sizePolicy.setHeightForWidth(deletePointButtun->sizePolicy().hasHeightForWidth());
        deletePointButtun->setSizePolicy(sizePolicy);

        gridLayout->addWidget(deletePointButtun, 1, 1, 1, 1);

        toolButtonPastePolygon = new QToolButton(groupBox_2);
        toolButtonPastePolygon->setObjectName(QStringLiteral("toolButtonPastePolygon"));
        sizePolicy.setHeightForWidth(toolButtonPastePolygon->sizePolicy().hasHeightForWidth());
        toolButtonPastePolygon->setSizePolicy(sizePolicy);

        gridLayout->addWidget(toolButtonPastePolygon, 2, 1, 1, 1);

        toolButtonPasteCircle = new QToolButton(groupBox_2);
        toolButtonPasteCircle->setObjectName(QStringLiteral("toolButtonPasteCircle"));
        sizePolicy.setHeightForWidth(toolButtonPasteCircle->sizePolicy().hasHeightForWidth());
        toolButtonPasteCircle->setSizePolicy(sizePolicy);

        gridLayout->addWidget(toolButtonPasteCircle, 3, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 85, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 4, 1, 1, 1);

        splitter->addWidget(groupBox_2);

        verticalLayout_3->addWidget(splitter);

        operButtons = new QDialogButtonBox(RestrictUserEditor);
        operButtons->setObjectName(QStringLiteral("operButtons"));
        operButtons->setEnabled(false);
        operButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

        verticalLayout_3->addWidget(operButtons);


        retranslateUi(RestrictUserEditor);

        QMetaObject::connectSlotsByName(RestrictUserEditor);
    } // setupUi

    void retranslateUi(QWidget *RestrictUserEditor)
    {
        RestrictUserEditor->setWindowTitle(QApplication::translate("RestrictUserEditor", "User Restriction zone editor", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("RestrictUserEditor", "User Restriction zone", Q_NULLPTR));
        addRestrictionButton->setText(QApplication::translate("RestrictUserEditor", "Add", Q_NULLPTR));
        deleteRestrictionButton->setText(QApplication::translate("RestrictUserEditor", "Delete", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("RestrictUserEditor", "Points", Q_NULLPTR));
        addPointButton->setText(QApplication::translate("RestrictUserEditor", "Add", Q_NULLPTR));
        deletePointButtun->setText(QApplication::translate("RestrictUserEditor", "Delete", Q_NULLPTR));
        toolButtonPastePolygon->setText(QApplication::translate("RestrictUserEditor", "Paste\n"
"as Polygon", Q_NULLPTR));
        toolButtonPasteCircle->setText(QApplication::translate("RestrictUserEditor", "Paste\n"
"as Circle", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RestrictUserEditor: public Ui_RestrictUserEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESTRICTUSEREDITOR_H
