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
#ifndef QTWINDOWMENU_H
#define QTWINDOWMENU_H

#include <QtGui/QMenu>

class QMdiArea;
class QSignalMapper;

/**
  Menu represents some main window objects, such as toolbars, dock widgets, mdi windows.
  @author Stefan Weinzierl <Stefan@Weinzierl-Stefan.de>
*/
class QtWindowMenu : public QMenu
{
  Q_OBJECT
public:
  explicit QtWindowMenu(QWidget* parent = 0);
  QtWindowMenu(const QString &title, QWidget *parent=0);
  virtual ~QtWindowMenu();

  void setMdi(QMdiArea *mdi);

  void retranslateUi();

protected:

  virtual void changeEvent(QEvent * event);

protected slots:
  void showMenu();
  void showMoreWindows();
  void setActive(QWidget *w);

private:
  void initMenu();

  QList<QAction*> m_actions;
  QMdiArea *m_mdi;
  QSignalMapper *m_windowMapper;
  QAction *m_closeAct;
  QAction *m_closeAllAct;
  QAction *m_tileAct;
  QAction *m_cascadeAct;
  QAction *m_nextAct;
  QAction *m_previousAct;
  QAction *m_moreAct;
  QMenu *m_customMenu;
  QMenu *m_toolBars;
  QMenu *m_docks;
};

#endif
