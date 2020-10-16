/********************************************************************************
** Form generated from reading UI file 'arincmergemainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARINCMERGEMAINWINDOW_H
#define UI_ARINCMERGEMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ArincMergeMainWindow
{
public:
    QAction *editFir;
    QAction *editAirport;
    QAction *editHolding;
    QAction *editPoint;
    QAction *editRestrict;
    QAction *editRoute;
    QAction *editSsa;
    QAction *importArinc;
    QAction *actionImport_FIR;
    QAction *actionRelease_ARINC;
    QAction *editSector;
    QAction *action_Exit;
    QAction *editRestrictUser;
    QAction *actionArinc_changes;
    QAction *actionAbout;
    QAction *actionSynchronize;
    QAction *actionChange_FIR;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QMdiArea *mdiArea;
    QMenuBar *mainMenuBar;
    QMenu *menuEdit;
    QMenu *menuFile;
    QMenu *menuHelp;
    QStatusBar *mainStatusBar;

    void setupUi(QMainWindow *ArincMergeMainWindow)
    {
        if (ArincMergeMainWindow->objectName().isEmpty())
            ArincMergeMainWindow->setObjectName(QStringLiteral("ArincMergeMainWindow"));
        ArincMergeMainWindow->resize(1280, 768);
        ArincMergeMainWindow->setMinimumSize(QSize(1280, 768));
        ArincMergeMainWindow->setToolButtonStyle(Qt::ToolButtonTextOnly);
        editFir = new QAction(ArincMergeMainWindow);
        editFir->setObjectName(QStringLiteral("editFir"));
        editAirport = new QAction(ArincMergeMainWindow);
        editAirport->setObjectName(QStringLiteral("editAirport"));
        editHolding = new QAction(ArincMergeMainWindow);
        editHolding->setObjectName(QStringLiteral("editHolding"));
        editPoint = new QAction(ArincMergeMainWindow);
        editPoint->setObjectName(QStringLiteral("editPoint"));
        editRestrict = new QAction(ArincMergeMainWindow);
        editRestrict->setObjectName(QStringLiteral("editRestrict"));
        editRoute = new QAction(ArincMergeMainWindow);
        editRoute->setObjectName(QStringLiteral("editRoute"));
        editSsa = new QAction(ArincMergeMainWindow);
        editSsa->setObjectName(QStringLiteral("editSsa"));
        importArinc = new QAction(ArincMergeMainWindow);
        importArinc->setObjectName(QStringLiteral("importArinc"));
        actionImport_FIR = new QAction(ArincMergeMainWindow);
        actionImport_FIR->setObjectName(QStringLiteral("actionImport_FIR"));
        actionImport_FIR->setEnabled(false);
        actionRelease_ARINC = new QAction(ArincMergeMainWindow);
        actionRelease_ARINC->setObjectName(QStringLiteral("actionRelease_ARINC"));
        actionRelease_ARINC->setEnabled(false);
        editSector = new QAction(ArincMergeMainWindow);
        editSector->setObjectName(QStringLiteral("editSector"));
        action_Exit = new QAction(ArincMergeMainWindow);
        action_Exit->setObjectName(QStringLiteral("action_Exit"));
        editRestrictUser = new QAction(ArincMergeMainWindow);
        editRestrictUser->setObjectName(QStringLiteral("editRestrictUser"));
        actionArinc_changes = new QAction(ArincMergeMainWindow);
        actionArinc_changes->setObjectName(QStringLiteral("actionArinc_changes"));
        actionArinc_changes->setVisible(false);
        actionAbout = new QAction(ArincMergeMainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionSynchronize = new QAction(ArincMergeMainWindow);
        actionSynchronize->setObjectName(QStringLiteral("actionSynchronize"));
        actionSynchronize->setVisible(false);
        actionChange_FIR = new QAction(ArincMergeMainWindow);
        actionChange_FIR->setObjectName(QStringLiteral("actionChange_FIR"));
        centralWidget = new QWidget(ArincMergeMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        gridLayout->setHorizontalSpacing(0);
        gridLayout->setVerticalSpacing(6);
        mdiArea = new QMdiArea(centralWidget);
        mdiArea->setObjectName(QStringLiteral("mdiArea"));
        mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        mdiArea->setViewMode(QMdiArea::TabbedView);
        mdiArea->setDocumentMode(false);
        mdiArea->setTabPosition(QTabWidget::North);

        gridLayout->addWidget(mdiArea, 0, 0, 1, 1);

        ArincMergeMainWindow->setCentralWidget(centralWidget);
        mainMenuBar = new QMenuBar(ArincMergeMainWindow);
        mainMenuBar->setObjectName(QStringLiteral("mainMenuBar"));
        mainMenuBar->setGeometry(QRect(0, 0, 1280, 25));
        menuEdit = new QMenu(mainMenuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuFile = new QMenu(mainMenuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(mainMenuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        ArincMergeMainWindow->setMenuBar(mainMenuBar);
        mainStatusBar = new QStatusBar(ArincMergeMainWindow);
        mainStatusBar->setObjectName(QStringLiteral("mainStatusBar"));
        ArincMergeMainWindow->setStatusBar(mainStatusBar);

        mainMenuBar->addAction(menuFile->menuAction());
        mainMenuBar->addAction(menuEdit->menuAction());
        mainMenuBar->addAction(menuHelp->menuAction());
        menuEdit->addAction(editAirport);
        menuEdit->addAction(editFir);
        menuEdit->addAction(editSector);
        menuEdit->addAction(editPoint);
        menuEdit->addAction(editRoute);
        menuEdit->addAction(editHolding);
        menuEdit->addAction(editRestrict);
        menuEdit->addAction(editRestrictUser);
        menuEdit->addAction(editSsa);
        menuFile->addAction(importArinc);
        menuFile->addAction(actionImport_FIR);
        menuFile->addAction(actionRelease_ARINC);
        menuFile->addSeparator();
        menuFile->addAction(actionChange_FIR);
        menuFile->addSeparator();
        menuFile->addAction(actionArinc_changes);
        menuFile->addAction(actionSynchronize);
        menuFile->addSeparator();
        menuFile->addAction(action_Exit);
        menuHelp->addAction(actionAbout);

        retranslateUi(ArincMergeMainWindow);
        QObject::connect(action_Exit, SIGNAL(triggered()), ArincMergeMainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(ArincMergeMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ArincMergeMainWindow)
    {
        ArincMergeMainWindow->setWindowTitle(QApplication::translate("ArincMergeMainWindow", "Environment Editor", Q_NULLPTR));
        editFir->setText(QApplication::translate("ArincMergeMainWindow", "FIR", Q_NULLPTR));
        editAirport->setText(QApplication::translate("ArincMergeMainWindow", "Airport", Q_NULLPTR));
        editHolding->setText(QApplication::translate("ArincMergeMainWindow", "Holding", Q_NULLPTR));
        editPoint->setText(QApplication::translate("ArincMergeMainWindow", "Point", Q_NULLPTR));
        editRestrict->setText(QApplication::translate("ArincMergeMainWindow", "Restrict", Q_NULLPTR));
        editRoute->setText(QApplication::translate("ArincMergeMainWindow", "Route", Q_NULLPTR));
        editSsa->setText(QApplication::translate("ArincMergeMainWindow", "SSA", Q_NULLPTR));
        importArinc->setText(QApplication::translate("ArincMergeMainWindow", "Load ARINC file", Q_NULLPTR));
        actionImport_FIR->setText(QApplication::translate("ArincMergeMainWindow", "Import FIR in database", Q_NULLPTR));
        actionRelease_ARINC->setText(QApplication::translate("ArincMergeMainWindow", "Release ARINC file", Q_NULLPTR));
        editSector->setText(QApplication::translate("ArincMergeMainWindow", "Sector", Q_NULLPTR));
        action_Exit->setText(QApplication::translate("ArincMergeMainWindow", "&Exit", Q_NULLPTR));
        editRestrictUser->setText(QApplication::translate("ArincMergeMainWindow", "Restrict User", Q_NULLPTR));
        actionArinc_changes->setText(QApplication::translate("ArincMergeMainWindow", "Arinc changes", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("ArincMergeMainWindow", "About ...", Q_NULLPTR));
        actionSynchronize->setText(QApplication::translate("ArincMergeMainWindow", "Synchronize", Q_NULLPTR));
        actionChange_FIR->setText(QApplication::translate("ArincMergeMainWindow", "Change current FIR", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("ArincMergeMainWindow", "Edit", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("ArincMergeMainWindow", "File", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("ArincMergeMainWindow", "Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ArincMergeMainWindow: public Ui_ArincMergeMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARINCMERGEMAINWINDOW_H
