/***************************************************************************
 *   Copyright (C) 2009 by Stefan Weinzierl                                *
 *   Stefan@Weinzierl-Stefan.de                                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "qtwindowmenu.h"

#include <QtCore/QSignalMapper>
#include <QtCore/QEvent>

#include <QtGui/QDialog>
#include <QtGui/QMdiArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QListWidget>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QToolBar>
#include <QtGui/QDockWidget>

QtWindowMenu::QtWindowMenu(QWidget* parent)
    :QMenu(parent), m_customMenu(0), m_toolBars(0), m_docks(0)
{
    setTitle(tr("&Window"));
    initMenu();
}

QtWindowMenu::QtWindowMenu(const QString &title, QWidget *parent)
    : QMenu(title, parent), m_customMenu(0), m_toolBars(0), m_docks(0)
{
    initMenu();
}

QtWindowMenu::~QtWindowMenu()
{
}

void QtWindowMenu::initMenu()
{
    m_windowMapper = new QSignalMapper(this);

    m_closeAct = new QAction(tr("Cl&ose"), this);
    //m_closeAct->setShortcut(QKeySequence::Close);
    //m_closeAct->setPriority(QAction::LowPriority);
    m_closeAct->setStatusTip(tr("Close the active window"));

    m_closeAllAct = new QAction(tr("Close &All"), this);
    m_closeAllAct->setShortcut(tr("Ctrl+Shift+W"));
    //m_closeAllAct->setPriority(QAction::LowPriority);
    m_closeAllAct->setStatusTip(tr("Close all the windows"));

    m_tileAct = new QAction(tr("&Tile"), this);
    m_tileAct->setStatusTip(tr("Tile the windows"));
    m_tileAct->setIcon(QIcon(":/images/32x32/application/application_tile.png"));

    m_cascadeAct = new QAction(tr("&Cascade"), this);
    m_cascadeAct->setStatusTip(tr("Cascade the windows"));
    m_cascadeAct->setIcon(QIcon(":/images/32x32/application/application_cascade.png"));

    m_nextAct = new QAction(tr("Ne&xt"), this);
    m_nextAct->setStatusTip(tr("Move the focus to the next window"));

    m_previousAct = new QAction(tr("Pre&vious"), this);
    m_previousAct->setStatusTip(tr("Move the focus to the previous window"));

    m_moreAct = new QAction(tr("&More Windows..."), this);
    m_moreAct->setStatusTip(tr("Show more windows"));

    m_toolBars=addMenu(tr("Toolbars"));
    m_docks=addMenu(tr("Docks"));

    addSeparator();
    addAction(m_closeAct);
    addAction(m_closeAllAct);
    addSeparator();
    addAction(m_tileAct);
    addAction(m_cascadeAct);
    addSeparator();
    addAction(m_nextAct);
    addAction(m_previousAct);
    addSeparator();
    addAction(m_moreAct);

    connect(this, SIGNAL(aboutToShow()), this, SLOT(showMenu()));
    connect(m_moreAct, SIGNAL(triggered()), this, SLOT(showMoreWindows()));

    setMdi(parent()->findChild<QMdiArea*>());
}


void QtWindowMenu::setMdi(QMdiArea *mdi)
{
    m_mdi=mdi;

    m_closeAct->disconnect();
    m_closeAllAct->disconnect();
    m_tileAct->disconnect();
    m_cascadeAct->disconnect();
    m_nextAct->disconnect();
    m_previousAct->disconnect();
    m_windowMapper->disconnect();

    if (m_mdi) {
        connect(m_closeAct, SIGNAL(triggered()), m_mdi, SLOT(closeActiveSubWindow()));
        connect(m_closeAllAct, SIGNAL(triggered()), m_mdi, SLOT(closeAllSubWindows()));
        connect(m_tileAct, SIGNAL(triggered()), m_mdi, SLOT(tileSubWindows()));
        connect(m_cascadeAct, SIGNAL(triggered()), m_mdi, SLOT(cascadeSubWindows()));
        connect(m_nextAct, SIGNAL(triggered()), m_mdi, SLOT(activateNextSubWindow()));
        connect(m_previousAct, SIGNAL(triggered()), m_mdi, SLOT(activatePreviousSubWindow()));
        connect(m_windowMapper, SIGNAL(mapped(QWidget *)), this, SLOT(setActive(QWidget *)));
    }
}

void QtWindowMenu::setActive(QWidget *w)
{
    if (m_mdi) m_mdi->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(w));
}
void QtWindowMenu::showMenu()
{
    bool hasChild=false;

    qDeleteAll(m_actions);
    m_actions.clear();

    if (m_mdi) {
        QList<QMdiSubWindow*> windows=m_mdi->subWindowList();
        QMdiSubWindow *activeChild=m_mdi->activeSubWindow();

        m_moreAct->setVisible(windows.count()>9);
        hasChild=!windows.isEmpty();

        for (int i = 0; i<windows.size() && i<9; ++i) {
            QMdiSubWindow *child=windows.at(i);

            QAction *action=new QAction(tr("&%1 %2").arg(i+1).arg(child->windowTitle()), this);
            m_actions.append(action);
            insertAction(m_moreAct, action);
            action->setIcon(child->windowIcon());
            action->setStatusTip(child->statusTip());
            action->setToolTip(child->toolTip());
            action->setCheckable(true);
            action ->setChecked(child==activeChild);
            connect(action, SIGNAL(triggered()), m_windowMapper, SLOT(map()));
            m_windowMapper->setMapping(action, child);
        }
    }

    m_closeAct->setEnabled(hasChild);
    m_closeAllAct->setEnabled(hasChild);
    m_tileAct->setEnabled(hasChild);
    m_cascadeAct->setEnabled(hasChild);
    m_nextAct->setEnabled(hasChild);
    m_previousAct->setEnabled(hasChild);


    m_toolBars->clear();
    QList<QToolBar*> toolBars = qFindChildren<QToolBar*>(parent());
    m_toolBars->menuAction()->setVisible(!toolBars.isEmpty());
    foreach (QToolBar *bar, toolBars) {
        if (!bar->actions().isEmpty()) {
            m_toolBars->addAction(bar->toggleViewAction());
        }
    }

    m_docks->clear();
    QList<QDockWidget*> docks= qFindChildren<QDockWidget*>(parent());
    m_docks->menuAction()->setVisible(!docks.isEmpty());
    foreach (QDockWidget *dock, docks) {
        m_docks->addAction(dock->toggleViewAction());
    }
}

void QtWindowMenu::showMoreWindows()
{
    QDialog dlg;
    QVBoxLayout *layout=new QVBoxLayout(&dlg);

    QListWidget *list=new QListWidget(&dlg);
    layout->addWidget(list);

    QDialogButtonBox *box=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, &dlg);
    layout->addWidget(box);

    connect(box, SIGNAL(accepted()), &dlg, SLOT(accept()));
    connect(box, SIGNAL(rejected()), &dlg, SLOT(reject()));
    connect(list, SIGNAL(itemActivated(QListWidgetItem *)), &dlg, SLOT(accept()));

    if (m_mdi) {
        QList<QMdiSubWindow*> windows=m_mdi->subWindowList();
        QMdiSubWindow *activeChild=m_mdi->activeSubWindow();

        for (int i=0; i<windows.size(); ++i) {
            QWidget *child=windows.at(i);
            QListWidgetItem *item=new QListWidgetItem(list);

            item->setText(child->windowTitle());
            item->setIcon(child->windowIcon());
            item->setToolTip(child->toolTip());
            item->setData(Qt::UserRole, qVariantFromValue((void *)child));
            if (child==activeChild) {
                item->setSelected(true);
                list->setCurrentItem(item);
            }
        }
    }

    if (dlg.exec()==QDialog::Accepted && m_mdi) {
        QListWidgetItem *item=list->currentItem();
        if (item) {
            QVariant var=item->data(Qt::UserRole);
            QMdiSubWindow *wdg=static_cast<QMdiSubWindow*>(var.value<void*>());
            m_mdi->setActiveSubWindow(wdg);
        }
    }
}

void QtWindowMenu::retranslateUi()
{
    setTitle(tr("&Window"));

    m_closeAct->setText(tr("Cl&ose"));
    m_closeAct->setStatusTip(tr("Close the active window"));

    m_closeAllAct->setText(tr("Close &All"));
    m_closeAllAct->setShortcut(tr("Ctrl+Shift+W"));
    m_closeAllAct->setStatusTip(tr("Close all the windows"));

    m_tileAct->setText(tr("&Tile"));
    m_tileAct->setStatusTip(tr("Tile the windows"));

    m_cascadeAct->setText(tr("&Cascade"));
    m_cascadeAct->setStatusTip(tr("Cascade the windows"));

    m_nextAct->setText(tr("Ne&xt"));
    m_nextAct->setStatusTip(tr("Move the focus to the next window"));

    m_previousAct->setText(tr("Pre&vious"));
    m_previousAct->setStatusTip(tr("Move the focus to the previous window"));

    m_moreAct->setText(tr("&More Windows..."));
    m_moreAct->setStatusTip(tr("Show more windows"));

    m_toolBars->setTitle(tr("Toolbars"));
    m_docks->setTitle(tr("Docks"));
}


void QtWindowMenu::changeEvent(QEvent* event)
{

    //LanguageChange events should not be confused with LocaleChange events.
    if (event->type() == QEvent::LocaleChange) {

        // HERE we can un|install qtranslator

        //    Locale-Change events are generated by the system and tell the application,
        //    "Maybe you should load a new translation."
    } else if (event->type() == QEvent::LanguageChange) {
        //    LanguageChange events are generated by Qt and tell the application's widgets,
        //    "Maybe you should retranslate all your strings."
        retranslateUi();
    }

    QMenu::changeEvent(event);
}
