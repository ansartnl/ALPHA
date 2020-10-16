#ifndef OCIQUERYVIEW_H
#define OCIQUERYVIEW_H

#include "qtsqltableview.h"

#include <QtGui/QTableView>
#include <QtGui/QStyledItemDelegate>

#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>

class OciQueryModel;
class QSqlRecord;
class QtHeaderView;

class OciQueryViewPrivate;
/**
* @brief Table view for oci query model
*
* @class OciQueryView ociqueryview.h
*/
class OciQueryView : public QtSqlTableView
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(OciQueryView)

  void init();
protected:
  OciQueryView(OciQueryViewPrivate& dd, QWidget* parent);
public:
  explicit OciQueryView(QWidget* parent = 0);

  virtual void setModel(OciQueryModel* model);

  virtual void setItemDelegateForColumn(int column, QAbstractItemDelegate* delegate);
  virtual void setItemDelegateForColumn(const QString& column, QAbstractItemDelegate* delegate);

  void freezeSqlColumns(const QStringList& columns);

public slots:
  void hideColumn(int column);
  void hideColumn(const QString& column);
  void hideColumns(const QStringList& columns);

  virtual void refresh();

protected slots:
  virtual void handleSortIndicatorsChanged();

  virtual void preRefresh();
  virtual void postRefresh();

protected:

private slots:
  // internal
  virtual void disconnectFromModel(OciQueryModel* model);
  virtual void connectToModel(OciQueryModel* model);
};


#endif // OCIQUERYVIEW_H
