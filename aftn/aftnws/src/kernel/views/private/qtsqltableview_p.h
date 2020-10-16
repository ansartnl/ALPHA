#ifndef QTSQLTABLEVIEW_P_H
#define QTSQLTABLEVIEW_P_H

#include "qtsqltableview.h"

#include <QtSql/QSqlIndex>

class QtHeaderView;
class QtSqlTableViewPrivate {
  Q_DECLARE_PUBLIC(QtSqlTableView)
protected:
  QtSqlTableView *q_ptr;
public:
  QtSqlTableViewPrivate();
  virtual ~QtSqlTableViewPrivate();

  QtHeaderView *horizontalHeader;

  QList<int> frozenColumns;
  QtSqlTableView *frozenTableView;

  QtSqlTableView::ScrollMode oldHScrollModel;
  QtSqlTableView::ScrollMode oldVScrollModel;
};

#endif //QTSQLTABLEVIEW_P_H
