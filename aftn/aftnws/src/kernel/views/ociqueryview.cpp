#include "ociqueryview.h"
#include "private/ociqueryview_p.h"

#include <QtDebug>

#include <QtCore/QRegExp>

#include <QtSql/QSqlField>

#include "qtheaderview.h"

#include "ociquerymodel.h"

OciQueryView::OciQueryView(QWidget* parent)
  : QtSqlTableView(*new OciQueryViewPrivate, parent)
{
  init();
}

OciQueryView::OciQueryView(OciQueryViewPrivate& dd, QWidget* parent)
  : QtSqlTableView(dd, parent)
{
  init();
}

void OciQueryView::init()
{
  setObjectName("pgQueryView");
  //Q_D(PGQueryView);
}

void OciQueryView::setModel(OciQueryModel* model)
{
  Q_D(OciQueryView);

  if (d->model)
    // Disconnect from old model
    disconnectFromModel(d->model);

  QtSqlTableView::setModel(model);
  d->model = model;

  if (d->model)
    // Connect to new model
    connectToModel(d->model);
}


void OciQueryView::setItemDelegateForColumn(int column, QAbstractItemDelegate* delegate)
{
  //Q_D(PGQueryView);
  QtSqlTableView::setItemDelegateForColumn(column, delegate);
}

void OciQueryView::setItemDelegateForColumn(const QString& column, QAbstractItemDelegate* delegate)
{
  Q_D(OciQueryView);
  if (!d->model) return;
  QSqlRecord record = d->model->record();
  QtSqlTableView::setItemDelegateForColumn(record.indexOf(column), delegate);
}

void OciQueryView::disconnectFromModel(OciQueryModel* model)
{
  //Q_D(PGQueryView);
  // отключаемся от всех сигналов
  disconnect(model, 0, this, 0);
}

void OciQueryView::connectToModel(OciQueryModel* model)
{
  connect(model, SIGNAL(preRefresh()),
          this, SLOT(preRefresh()));
  connect(model, SIGNAL(postRefresh()),
          this, SLOT(postRefresh()));
}

void OciQueryView::preRefresh()
{
  Q_D(OciQueryView);
  if (!d->model) return;
  d->lastUsed = d->model->primaryKey(currentIndex().row());

  d->lastUsedColumn = currentIndex().column();
}

void OciQueryView::postRefresh()
{
  Q_D(OciQueryView);
  if (!d->model) return;
  int row = d->model->rowForPrimaryKey(d->lastUsed);
  QModelIndex idx = d->model->index(row, d->lastUsedColumn, rootIndex());
  setCurrentIndex(idx);
  scrollTo(idx, EnsureVisible);
}

void OciQueryView::hideColumn(int column)
{
  Q_D(OciQueryView);
  if (!d->model) return;
  d->horizontalHeader->hideSection(column);
}


void OciQueryView::hideColumn(const QString& column)
{
  Q_D(OciQueryView);
  if (!d->model) return;
  hideColumn(d->model->record().indexOf(column));
}

void OciQueryView::hideColumns(const QStringList& columns)
{
  for (int i = 0; i < columns.count(); ++i)
    hideColumn(columns.at(i));
}

void OciQueryView::refresh()
{
  Q_D(OciQueryView);
  if (d->model) d->model->refresh();
}

void OciQueryView::freezeSqlColumns(const QStringList& columns)
{
  Q_D(OciQueryView);
  if (!d->model) return;
  for (int i = 0; i < columns.count(); ++i)
    setColumnFrozen(d->model->record().indexOf(columns.at(i)), true);
}

void OciQueryView::handleSortIndicatorsChanged()
{
  Q_D(OciQueryView);
  QtSqlTableView::handleSortIndicatorsChanged();
  d->model->refresh();
}

OciQueryViewPrivate::OciQueryViewPrivate()
  : QtSqlTableViewPrivate()
  , model(0){}

OciQueryViewPrivate::~OciQueryViewPrivate() {}
