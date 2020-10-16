/********************************************************************************
** Form generated from reading UI file 'ssaeditor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SSAEDITOR_H
#define UI_SSAEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
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

class Ui_SsaEditor
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *firLabel;
    QSplitter *splitter_3;
    QWidget *layoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QLabel *label;
    QComboBox *airportsComboBox;
    QHBoxLayout *horizontalLayout_4;
    QTableView *runwayView;
    QVBoxLayout *verticalLayout_2;
    QToolButton *addRunwayButton;
    QToolButton *deleteRunwayButton;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_2;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QComboBox *schemeComboBox;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_4;
    QVBoxLayout *verticalLayout_4;
    QTableView *ssaView;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *addSsaButton;
    QToolButton *deleteSsaButton;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_5;
    QTableView *ssaPointView;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *addSsaPointButton;
    QToolButton *deletSsaPointButton;
    QDialogButtonBox *operButtons;

    void setupUi(QWidget *SsaEditor)
    {
        if (SsaEditor->objectName().isEmpty())
            SsaEditor->setObjectName(QStringLiteral("SsaEditor"));
        SsaEditor->resize(874, 637);
        verticalLayout = new QVBoxLayout(SsaEditor);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        firLabel = new QLabel(SsaEditor);
        firLabel->setObjectName(QStringLiteral("firLabel"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        firLabel->setFont(font);
        firLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(firLabel);

        splitter_3 = new QSplitter(SsaEditor);
        splitter_3->setObjectName(QStringLiteral("splitter_3"));
        splitter_3->setOrientation(Qt::Vertical);
        splitter_3->setChildrenCollapsible(false);
        layoutWidget_2 = new QWidget(splitter_3);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        gridLayout_2 = new QGridLayout(layoutWidget_2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        label = new QLabel(layoutWidget_2);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font);

        gridLayout_2->addWidget(label, 0, 1, 1, 1);

        airportsComboBox = new QComboBox(layoutWidget_2);
        airportsComboBox->setObjectName(QStringLiteral("airportsComboBox"));

        gridLayout_2->addWidget(airportsComboBox, 1, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        runwayView = new QTableView(layoutWidget_2);
        runwayView->setObjectName(QStringLiteral("runwayView"));
        runwayView->setSelectionBehavior(QAbstractItemView::SelectRows);
        runwayView->setSortingEnabled(true);
        runwayView->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout_4->addWidget(runwayView);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        addRunwayButton = new QToolButton(layoutWidget_2);
        addRunwayButton->setObjectName(QStringLiteral("addRunwayButton"));
        addRunwayButton->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(addRunwayButton->sizePolicy().hasHeightForWidth());
        addRunwayButton->setSizePolicy(sizePolicy);
        addRunwayButton->setMinimumSize(QSize(33, 36));

        verticalLayout_2->addWidget(addRunwayButton);

        deleteRunwayButton = new QToolButton(layoutWidget_2);
        deleteRunwayButton->setObjectName(QStringLiteral("deleteRunwayButton"));
        deleteRunwayButton->setEnabled(true);
        sizePolicy.setHeightForWidth(deleteRunwayButton->sizePolicy().hasHeightForWidth());
        deleteRunwayButton->setSizePolicy(sizePolicy);
        deleteRunwayButton->setMinimumSize(QSize(33, 36));

        verticalLayout_2->addWidget(deleteRunwayButton);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);


        horizontalLayout_4->addLayout(verticalLayout_2);


        gridLayout_2->addLayout(horizontalLayout_4, 1, 1, 2, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 2, 0, 1, 1);

        splitter_3->addWidget(layoutWidget_2);
        layoutWidget = new QWidget(splitter_3);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);

        horizontalLayout->addWidget(label_3);

        schemeComboBox = new QComboBox(layoutWidget);
        schemeComboBox->setObjectName(QStringLiteral("schemeComboBox"));

        horizontalLayout->addWidget(schemeComboBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font);

        gridLayout->addWidget(label_4, 0, 1, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        ssaView = new QTableView(layoutWidget);
        ssaView->setObjectName(QStringLiteral("ssaView"));
        ssaView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ssaView->setSortingEnabled(true);
        ssaView->setWordWrap(false);
        ssaView->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_4->addWidget(ssaView);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        addSsaButton = new QToolButton(layoutWidget);
        addSsaButton->setObjectName(QStringLiteral("addSsaButton"));
        addSsaButton->setEnabled(false);
        sizePolicy.setHeightForWidth(addSsaButton->sizePolicy().hasHeightForWidth());
        addSsaButton->setSizePolicy(sizePolicy);
        addSsaButton->setMinimumSize(QSize(33, 36));

        horizontalLayout_2->addWidget(addSsaButton);

        deleteSsaButton = new QToolButton(layoutWidget);
        deleteSsaButton->setObjectName(QStringLiteral("deleteSsaButton"));
        deleteSsaButton->setEnabled(true);
        sizePolicy.setHeightForWidth(deleteSsaButton->sizePolicy().hasHeightForWidth());
        deleteSsaButton->setSizePolicy(sizePolicy);
        deleteSsaButton->setMinimumSize(QSize(33, 36));

        horizontalLayout_2->addWidget(deleteSsaButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_4->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout_4, 1, 0, 1, 1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        ssaPointView = new QTableView(layoutWidget);
        ssaPointView->setObjectName(QStringLiteral("ssaPointView"));
        ssaPointView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ssaPointView->setSortingEnabled(true);
        ssaPointView->setWordWrap(false);
        ssaPointView->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_5->addWidget(ssaPointView);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        addSsaPointButton = new QToolButton(layoutWidget);
        addSsaPointButton->setObjectName(QStringLiteral("addSsaPointButton"));
        addSsaPointButton->setEnabled(false);
        sizePolicy.setHeightForWidth(addSsaPointButton->sizePolicy().hasHeightForWidth());
        addSsaPointButton->setSizePolicy(sizePolicy);
        addSsaPointButton->setMinimumSize(QSize(33, 36));

        horizontalLayout_3->addWidget(addSsaPointButton);

        deletSsaPointButton = new QToolButton(layoutWidget);
        deletSsaPointButton->setObjectName(QStringLiteral("deletSsaPointButton"));
        deletSsaPointButton->setEnabled(true);
        sizePolicy.setHeightForWidth(deletSsaPointButton->sizePolicy().hasHeightForWidth());
        deletSsaPointButton->setSizePolicy(sizePolicy);
        deletSsaPointButton->setMinimumSize(QSize(33, 36));

        horizontalLayout_3->addWidget(deletSsaPointButton);

        operButtons = new QDialogButtonBox(layoutWidget);
        operButtons->setObjectName(QStringLiteral("operButtons"));
        operButtons->setEnabled(false);
        operButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

        horizontalLayout_3->addWidget(operButtons);


        verticalLayout_5->addLayout(horizontalLayout_3);


        gridLayout->addLayout(verticalLayout_5, 1, 1, 1, 1);

        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 3);
        splitter_3->addWidget(layoutWidget);

        verticalLayout->addWidget(splitter_3);

        QWidget::setTabOrder(airportsComboBox, runwayView);
        QWidget::setTabOrder(runwayView, addRunwayButton);
        QWidget::setTabOrder(addRunwayButton, deleteRunwayButton);
        QWidget::setTabOrder(deleteRunwayButton, schemeComboBox);
        QWidget::setTabOrder(schemeComboBox, ssaView);
        QWidget::setTabOrder(ssaView, ssaPointView);
        QWidget::setTabOrder(ssaPointView, addSsaButton);
        QWidget::setTabOrder(addSsaButton, deleteSsaButton);
        QWidget::setTabOrder(deleteSsaButton, addSsaPointButton);
        QWidget::setTabOrder(addSsaPointButton, deletSsaPointButton);
        QWidget::setTabOrder(deletSsaPointButton, operButtons);

        retranslateUi(SsaEditor);

        QMetaObject::connectSlotsByName(SsaEditor);
    } // setupUi

    void retranslateUi(QWidget *SsaEditor)
    {
        SsaEditor->setWindowTitle(QApplication::translate("SsaEditor", "SSA editor", Q_NULLPTR));
        firLabel->setText(QString());
        label_2->setText(QApplication::translate("SsaEditor", "Airport", Q_NULLPTR));
        label->setText(QApplication::translate("SsaEditor", "Runway", Q_NULLPTR));
        addRunwayButton->setText(QApplication::translate("SsaEditor", "+", Q_NULLPTR));
        deleteRunwayButton->setText(QApplication::translate("SsaEditor", "-", Q_NULLPTR));
        label_3->setText(QApplication::translate("SsaEditor", "Scheme", Q_NULLPTR));
        label_4->setText(QApplication::translate("SsaEditor", "Route", Q_NULLPTR));
        addSsaButton->setText(QApplication::translate("SsaEditor", "+", Q_NULLPTR));
        deleteSsaButton->setText(QApplication::translate("SsaEditor", "-", Q_NULLPTR));
        addSsaPointButton->setText(QApplication::translate("SsaEditor", "+", Q_NULLPTR));
        deletSsaPointButton->setText(QApplication::translate("SsaEditor", "-", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SsaEditor: public Ui_SsaEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SSAEDITOR_H
