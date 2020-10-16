/***************************************************************************
 *   Copyright (C) 2009 by Weinzierl Stefan                                *
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
#ifndef QTMENUVIEW_H
#define QTMENUVIEW_H

// cmake auto defines
#if defined(qtmenuview_EXPORTS)
#  define QTMENUVIEW_EXPORT Q_DECL_EXPORT
#elif defined(QTMENUVIEW_IMPORTS)
#  define QTMENUVIEW_EXPORT Q_DECL_IMPORT  /**/
#else
#  define QTMENUVIEW_EXPORT
#endif

#include <QtGui/QMenu>
#include <QtCore/QModelIndex>

class QAbstractItemModel;

/**
  Item view class for qabstractitemmodel. Show items in menu.

	@author Weinzierl Stefan <Stefan@Weinzierl-Stefan.de>
*/
class QTMENUVIEW_EXPORT QtMenuView : public QMenu
{
Q_OBJECT
public:
	explicit QtMenuView(QWidget* parent = 0);
	QtMenuView(const QString &title, QWidget *parent=0);
	~QtMenuView();

	void setModel(QAbstractItemModel *model, bool recursive=false);

	QModelIndex indexForAction(QAction *act) const;
signals:
	void hoveredIndex(const QModelIndex &index);
	void triggeredIndex(const QModelIndex &index);

protected slots:
	void showMenu();
	void hideMenu();
	void menuDestroyed(QObject *);
	void actionDestroyed(QObject *);

	void hovered();
	void triggered();

private:
	QAbstractItemModel *m_model;
	bool m_recursive;
	QMap<QMenu*, QModelIndex> m_indexes;
	QMap<QAction*, QModelIndex> m_actions;
};

#endif //QTMENUVIEW_H
