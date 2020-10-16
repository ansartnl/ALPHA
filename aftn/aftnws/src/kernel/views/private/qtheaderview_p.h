#ifndef QTHEADERVIEW_P_H
#define QTHEADERVIEW_P_H

#include "qtheaderview.h"

class FilterModel;
class QListView;
class QWidgetAction;
class QAction;
class SectionsProxyModel;
class QtHeaderViewPrivate {
  Q_DECLARE_PUBLIC(QtHeaderView)
public:
  QtHeaderView * q_ptr;

  QtHeaderViewPrivate();
  virtual ~QtHeaderViewPrivate();

  QAbstractItemModel *model;

  QtHeaderView::SortFlag sortFlag;
  QHash<int, Qt::SortOrder > sortingSections; //< section, SortOrder>
  QList<int> orderList;

  bool enableShowHideSections;
  bool enableFilter;
  bool enableFreeze;
  bool editable;

  QAction *hideSectionAction;
  QAction *showAllSections;

  QAction *showHideSeparator;
  QMenu *showHideSectionsMenu;
  QList<QAction *> showhideSectionActionList;


  QAction *sortSeparator;
  QAction *sortAscAction;
  QAction *sortDescAction;
  QAction *removeSortAction;

  QAction *movableAction;
  QAction *hightlightAction;

  int currentLogicalIndex;

  int editingLogicalSection;

  QAction *frozenActionSeparator;
  QMenu *frozenMenu;
  SectionsProxyModel *frozenSectionsModel;
  QListView *frozenSectionsListView;
  QWidgetAction *frozenSectionsListViewAction;

  QAction *filterViewSeparator;
  QListView *filterView;
  QWidgetAction *filterViewAction;

  QMenu *menu;

  FilterModel *filterModel;

  inline void clearSortIndicatorsWithoutRepaint()
  {
    sortingSections.clear();
    orderList.clear();
  }

  inline void removeSortIndicatorWithoutRepaint(int logicalIndex)
  {
    sortingSections.remove(logicalIndex);
    orderList.removeAll(logicalIndex);
  }

  inline bool rowIntersectsSelection(int row) const
  {
      return (q_ptr->selectionModel() ? q_ptr->selectionModel()->rowIntersectsSelection(row, q_ptr->rootIndex()) : false);
  }

  inline bool columnIntersectsSelection(int column) const
  {
      return (q_ptr->selectionModel() ? q_ptr->selectionModel()->columnIntersectsSelection(column, q_ptr->rootIndex()) : false);
  }

  inline bool sectionIntersectsSelection(int logical) const
  {
      return (q_ptr->orientation() == Qt::Horizontal ? columnIntersectsSelection(logical) : rowIntersectsSelection(logical));
  }

  inline bool isRowSelected(int row) const
  {
      return (q_ptr->selectionModel() ? q_ptr->selectionModel()->isRowSelected(row, q_ptr->rootIndex()) : false);
  }

  inline bool isColumnSelected(int column) const
  {
      return (q_ptr->selectionModel() ? q_ptr->selectionModel()->isColumnSelected(column, q_ptr->rootIndex()) : false);
  }
};


#endif // HEADERVIEW_P_H
