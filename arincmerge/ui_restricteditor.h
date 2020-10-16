/********************************************************************************
** Form generated from reading UI file 'restricteditor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESTRICTEDITOR_H
#define UI_RESTRICTEDITOR_H

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

class Ui_RestrictEditor
{
public:
    QVBoxLayout *verticalLayout_3;
    QLabel *firLabel;
    QSplitter *splitter_2;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QTableView *restrictionsView;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *addRestrictionButton;
    QToolButton *deleteRestrictionButton;
    QSpacerItem *horizontalSpacer;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QTableView *pointView;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *addPointButton;
    QToolButton *deletePointButtun;
    QSpacerItem *horizontalSpacer_2;
    QDialogButtonBox *operButtons;

    void setupUi(QWidget *RestrictEditor)
    {
        if (RestrictEditor->objectName().isEmpty())
            RestrictEditor->setObjectName(QStringLiteral("RestrictEditor"));
        RestrictEditor->resize(721, 576);
        verticalLayout_3 = new QVBoxLayout(RestrictEditor);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        firLabel = new QLabel(RestrictEditor);
        firLabel->setObjectName(QStringLiteral("firLabel"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        firLabel->setFont(font);
        firLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(firLabel);

        splitter_2 = new QSplitter(RestrictEditor);
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
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);

        verticalLayout->addWidget(label_2);

        restrictionsView = new QTableView(layoutWidget);
        restrictionsView->setObjectName(QStringLiteral("restrictionsView"));
        restrictionsView->setSelectionBehavior(QAbstractItemView::SelectRows);
        restrictionsView->setSortingEnabled(true);
        restrictionsView->setWordWrap(false);
        restrictionsView->horizontalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(restrictionsView);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        addRestrictionButton = new QToolButton(layoutWidget);
        addRestrictionButton->setObjectName(QStringLiteral("addRestrictionButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(addRestrictionButton->sizePolicy().hasHeightForWidth());
        addRestrictionButton->setSizePolicy(sizePolicy1);
        addRestrictionButton->setMinimumSize(QSize(33, 36));

        horizontalLayout_2->addWidget(addRestrictionButton);

        deleteRestrictionButton = new QToolButton(layoutWidget);
        deleteRestrictionButton->setObjectName(QStringLiteral("deleteRestrictionButton"));
        sizePolicy1.setHeightForWidth(deleteRestrictionButton->sizePolicy().hasHeightForWidth());
        deleteRestrictionButton->setSizePolicy(sizePolicy1);
        deleteRestrictionButton->setMinimumSize(QSize(33, 36));

        horizontalLayout_2->addWidget(deleteRestrictionButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        splitter_2->addWidget(layoutWidget);
        layoutWidget_2 = new QWidget(splitter_2);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        verticalLayout_2 = new QVBoxLayout(layoutWidget_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget_2);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font);

        verticalLayout_2->addWidget(label);

        pointView = new QTableView(layoutWidget_2);
        pointView->setObjectName(QStringLiteral("pointView"));
        pointView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        pointView->setSelectionBehavior(QAbstractItemView::SelectRows);
        pointView->setSortingEnabled(true);
        pointView->setWordWrap(false);
        pointView->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_2->addWidget(pointView);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        addPointButton = new QToolButton(layoutWidget_2);
        addPointButton->setObjectName(QStringLiteral("addPointButton"));
        sizePolicy1.setHeightForWidth(addPointButton->sizePolicy().hasHeightForWidth());
        addPointButton->setSizePolicy(sizePolicy1);
        addPointButton->setMinimumSize(QSize(33, 36));

        horizontalLayout_4->addWidget(addPointButton);

        deletePointButtun = new QToolButton(layoutWidget_2);
        deletePointButtun->setObjectName(QStringLiteral("deletePointButtun"));
        sizePolicy1.setHeightForWidth(deletePointButtun->sizePolicy().hasHeightForWidth());
        deletePointButtun->setSizePolicy(sizePolicy1);
        deletePointButtun->setMinimumSize(QSize(33, 36));

        horizontalLayout_4->addWidget(deletePointButtun);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_4);

        splitter_2->addWidget(layoutWidget_2);

        verticalLayout_3->addWidget(splitter_2);

        operButtons = new QDialogButtonBox(RestrictEditor);
        operButtons->setObjectName(QStringLiteral("operButtons"));
        operButtons->setEnabled(false);
        operButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

        verticalLayout_3->addWidget(operButtons);


        retranslateUi(RestrictEditor);

        QMetaObject::connectSlotsByName(RestrictEditor);
    } // setupUi

    void retranslateUi(QWidget *RestrictEditor)
    {
        RestrictEditor->setWindowTitle(QApplication::translate("RestrictEditor", "Restriction zone editor", Q_NULLPTR));
        firLabel->setText(QString());
        label_2->setText(QApplication::translate("RestrictEditor", "Zone", Q_NULLPTR));
        addRestrictionButton->setText(QApplication::translate("RestrictEditor", "+", Q_NULLPTR));
        deleteRestrictionButton->setText(QApplication::translate("RestrictEditor", "-", Q_NULLPTR));
        label->setText(QApplication::translate("RestrictEditor", "Points", Q_NULLPTR));
        addPointButton->setText(QApplication::translate("RestrictEditor", "+", Q_NULLPTR));
        deletePointButtun->setText(QApplication::translate("RestrictEditor", "-", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RestrictEditor: public Ui_RestrictEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESTRICTEDITOR_H
