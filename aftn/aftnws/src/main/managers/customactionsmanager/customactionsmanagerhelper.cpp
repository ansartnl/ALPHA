#include "customactionsmanagerhelper.h"

#include <QtDebug>

#include <QtCore/QMimeData>

#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QAction>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>

CustomActionsManagerHelper::CustomActionsManagerHelper(QObject* parent)
  : QObject(parent) {}

CustomActionsManagerHelper::~CustomActionsManagerHelper() {}

#include <QtGui/QAbstractItemView>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QHeaderView>
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>

#include <QtSql/QSqlQueryModel>
//#include "pgquerymodel.h"
#include "modelinterfaces.h"
#include "ociquerymodel.h"
// ---------------------------------------------
// ItemView
ItemViewCustomActionsManagerHelper::ItemViewCustomActionsManagerHelper(QObject* parent)
  : CustomActionsManagerHelper(parent)
  , itemView(0)
{

  QAction* a;

  a = cutLinesAction =  new QAction(QIcon(":/images/table/table_row_cut.png")
                                    , tr("Cut lines"), this);
  a->setObjectName("cutLinesAction");
  a->setToolTip(tr("Cut selected lines to clipboard"));
  a->setStatusTip(tr("Cut selected lines to clipboard"));
  a->setShortcutContext(Qt::WidgetShortcut);
  a->setEnabled(false);
  connect(this, SIGNAL(canCutLinesChanged(bool))
          , a, SLOT(setEnabled(bool)));

  // connect to fire signal
  connect(a, SIGNAL(triggered())
          , this, SLOT(cutLines()));

  a = copyLinesAction = new QAction(QIcon(":/images/table/table_row_copy.png")
                                    , tr("Copy lines"), this);
  a->setObjectName("copyLinesAction");
  a->setToolTip(tr("Copy selected lines to clipboard"));
  a->setStatusTip(tr("Copy selected lines to clipboard"));
  a->setEnabled(false);
  connect(this, SIGNAL(canCopyLinesChanged(bool))
          , a, SLOT(setEnabled(bool)));
  // connect to fire signal
  connect(a, SIGNAL(triggered())
          , this, SLOT(copyLines()));

  //insertAction(copyLinesAction, a);
  a = pasteLinesAction = new QAction(QIcon(":/images/table/table_row_insert.png")
                                     , tr("Paste lines"), this);
  a->setObjectName("pasteLinesAction");
  a->setToolTip(tr("Paste lines from clipboard"));
  a->setStatusTip(tr("Paste lines from clipboard"));
  a->setShortcutContext(Qt::WidgetShortcut);
  a->setEnabled(false);
  connect(this, SIGNAL(canPasteLinesChanged(bool))
          , a, SLOT(setEnabled(bool)));
  // connect to fire signal
  connect(a, SIGNAL(triggered())
          , this, SLOT(pasteLines()));

  a = removeLinesAction = new QAction(QIcon(":/images/table/table_row_delete.png")
                                      , tr("Remove lines"), this);
  a->setObjectName("removeLinesAction");
  a->setToolTip(tr("Remove selected rows"));
  a->setStatusTip(tr("Remove selected rows"));
  a->setShortcutContext(Qt::WidgetShortcut);
  a->setEnabled(false);
  connect(this, SIGNAL(canRemoveLinesChanged(bool))
          , a, SLOT(setEnabled(bool)));
  // connect to fire signal
  connect(a, SIGNAL(triggered())
          , this, SLOT(removeLines()));

  a = refreshActionSeparator = new QAction(this);
  a->setSeparator(true);

  a = refreshAction = new QAction(QIcon::fromTheme("reload"
                                  , QIcon(":/images/table/table_refresh.png"))
                                  , tr("Refresh"), this);
  a->setObjectName("refreshAction");
  a->setShortcuts(QKeySequence::Refresh);
  a->setToolTip(tr("Refresh table model"));
  a->setStatusTip(tr("Refresh table model"));
  a->setEnabled(false);
  connect(this, SIGNAL(canRefreshChanged(bool))
          , a, SLOT(setEnabled(bool)));
  // connect to fire signal
  connect(a, SIGNAL(triggered())
          , this, SLOT(refresh()));
}

unsigned char ItemViewCustomActionsManagerHelper::approach(QWidget* widget) const
{
  return qobject_cast<QAbstractItemView*>(widget) ? 10 : 0;
}

void ItemViewCustomActionsManagerHelper::activateFor(QWidget* widget)
{
  if ((itemView = qobject_cast<QAbstractItemView*>(widget))) {
    connect(QApplication::clipboard(), SIGNAL(dataChanged())
            , this, SLOT(clipboardDataChanged()));
    if (itemView->selectionModel()) {
      connect(itemView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
              this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
      selectionChanged(itemView->selectionModel()->selection(), itemView->selectionModel()->selection());
      clipboardDataChanged();
    }

    QList<QHeaderView*> headerViews = itemView->findChildren<QHeaderView*>();
    foreach(QHeaderView * headerView, headerViews)
    if (headerView->orientation() == Qt::Vertical)
      headerView->installEventFilter(const_cast<ItemViewCustomActionsManagerHelper*>(this));

    cutLinesAction->setEnabled(canCutLines());
    copyLinesAction->setEnabled(canCopyLines());
    pasteLinesAction->setEnabled(canPasteLines());
    removeLinesAction->setEnabled(canRemoveLines());

    if (canRefresh()) {
      itemView->addAction(refreshActionSeparator);
      itemView->addAction(refreshAction);
      refreshAction->setEnabled(true);
    }

    itemView->installEventFilter(const_cast<ItemViewCustomActionsManagerHelper*>(this));
  }
}

void ItemViewCustomActionsManagerHelper::deactivate()
{
  disconnect(QApplication::clipboard(), SIGNAL(dataChanged())
             , this, SLOT(clipboardDataChanged()));

  if (itemView->selectionModel())
    disconnect(itemView->selectionModel(), 0,
               this, 0);

  QList<QHeaderView*> headerViews = itemView->findChildren<QHeaderView*>();
  foreach(QHeaderView * headerView, headerViews)
  if (headerView->orientation() == Qt::Vertical)
    headerView->removeEventFilter(const_cast<ItemViewCustomActionsManagerHelper*>(this));

  cutLinesAction->setEnabled(false);
  copyLinesAction->setEnabled(false);
  pasteLinesAction->setEnabled(false);
  removeLinesAction->setEnabled(false);

  if (canRefresh()) {
    itemView->removeAction(refreshActionSeparator);
    itemView->removeAction(refreshAction);
    refreshAction->setEnabled(false);
  }

  itemView = 0;

}

bool ItemViewCustomActionsManagerHelper::canCutLines() const
{
  if (itemView->selectionModel())
    return itemView->selectionModel()->selectedRows().count();
  return false;
}

bool ItemViewCustomActionsManagerHelper::canCopyLines() const
{
  if (itemView->selectionModel())
    return itemView->selectionModel()->selectedRows().count();
  return false;
}

bool ItemViewCustomActionsManagerHelper::canPasteLines() const
{
  if (itemView->selectionModel())
    return QApplication::clipboard()->mimeData()->hasFormat(COPYPASTEROWSFORMAT);
  return false;
}

bool ItemViewCustomActionsManagerHelper::canRemoveLines() const
{
  if (itemView->selectionModel()) {
    if (itemView->model()->metaObject()->className() == QSqlQueryModel::staticMetaObject.className())
      return false;
    if (itemView->model()->metaObject()->className() == OciQueryModel::staticMetaObject.className())
      return false;

    return itemView->selectionModel()->selectedRows().count();
  }
  return false;
}

bool ItemViewCustomActionsManagerHelper::canRefresh() const
{
  return qobject_cast<IRefreshModel*>(itemView->model());
  return false;
}

QList<QAction*> ItemViewCustomActionsManagerHelper::additionalActions() const
{
  QList<QAction*> result;
  result
      //<< cutLinesAction
      //<< copyLinesAction
      //<< pasteLinesAction
      << removeLinesAction
      << refreshActionSeparator
      << refreshAction;
  return result;
}

bool ItemViewCustomActionsManagerHelper::eventFilter(QObject* object, QEvent* event)
{
  if (event->type() == QEvent::ContextMenu) {
    QContextMenuEvent* menuEvent = static_cast<QContextMenuEvent*>(event);

    QHeaderView* headerView = qobject_cast<QHeaderView*>(object);
    if (headerView) {
      if (headerView->orientation() == Qt::Vertical) {
        QList<QAction*> actions = headerView->actions();
        actions.append(additionalActions());
        // FIXME globalPos to pos
        QMenu::exec(actions, menuEvent->globalPos(), 0, headerView);
        return true;
      }
    }
  }
  return QObject::eventFilter(object, event);
}

void ItemViewCustomActionsManagerHelper::clipboardDataChanged()
{
  emit canPasteLinesChanged(canPasteLines());
}

void ItemViewCustomActionsManagerHelper::cutLines()
{
  copyLines();
  removeLines();
}

void ItemViewCustomActionsManagerHelper::copyLines()
{
  QAbstractItemModel* model;
  if ((model = itemView->model())) {
    QItemSelectionModel* selectionModel = itemView->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    if (indexes.isEmpty())
      return;

    QModelIndex rootIndex = indexes.at(0).parent();

    QMimeData* mimeData = new QMimeData(); //model->mimeData(selectionModel->selectedIndexes());
    QByteArray linesByteArray;
    QDataStream dataStream(&linesByteArray, QIODevice::WriteOnly);

    int rowCount = indexes.count();
    int columnCount = model->columnCount(rootIndex);
    dataStream << rowCount;
    dataStream << columnCount;
    int currentRow;
    for (int i = 0; i < rowCount; ++i) {
      currentRow = indexes.at(i).row();
      for (int g = 0; g < columnCount; ++g) {
        dataStream << model->itemData(model->index(currentRow, g, rootIndex));
      }
    }
    mimeData->setData(COPYPASTEROWSFORMAT, linesByteArray);
    QApplication::clipboard()->setMimeData(mimeData);
  }
}

void ItemViewCustomActionsManagerHelper::pasteLines()
{
  QAbstractItemModel* model;
  if ((model = itemView->model())) {
    const QMimeData* mimeData = QApplication::clipboard()->mimeData();
    if (mimeData->hasFormat(COPYPASTEROWSFORMAT)) {
      QByteArray byteArray = mimeData->data(COPYPASTEROWSFORMAT);
      QDataStream dataStream(&byteArray, QIODevice::ReadOnly);

      QModelIndex currentIndex = itemView->currentIndex();
      QModelIndex rootIndex = currentIndex.parent();
      int rowCount;
      int columnCount;
      int originalColumnCount = model->columnCount(rootIndex);
      dataStream >> rowCount >> columnCount;
      int prependRow = (currentIndex.isValid()) ? currentIndex.row() : model->rowCount(rootIndex);
      QMap<int, QVariant> value;
      if (model->insertRows(prependRow, rowCount, rootIndex)) {
        for (int i = 0; i < rowCount; ++i) {
          for (int g = 0; g < columnCount && g < originalColumnCount; ++g) {
            dataStream >> value;
            model->setItemData(model->index(prependRow + i, g, rootIndex), value);
          }
        }
      }
    }
  }
}

void ItemViewCustomActionsManagerHelper::removeLines()
{
  QAbstractItemModel* model;
  if ((model = itemView->model())) {
    QItemSelectionModel* selectionModel = itemView->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    if (indexes.isEmpty())
      return;

    emit blockFocusChanged(true);
    int messageBoxResult;
    messageBoxResult = QMessageBox::question(QApplication::activeWindow()
                       , tr("Removing data")
                       , tr("Data will be removed permanently. Continue?"),
                       QMessageBox::Yes, QMessageBox::No);
    emit blockFocusChanged(false);

    if (messageBoxResult == QMessageBox::No)
      return;

    int count = 1;
    int startRow = indexes.at(0).row();
    QModelIndex rootIndex = indexes.at(0).parent();

    // delete onle first continuous selection
    for (int i = 1; i < indexes.count(); ++i) {
      if (indexes.at(i - 1).row() != indexes.at(i).row() - 1)
        break;
      else
        ++count;
    }
    model->removeRows(startRow, count, rootIndex);
    if (!model->submit())
      model->revert();

    return;
  }
}

void ItemViewCustomActionsManagerHelper::refresh()
{
  IRefreshModel* model = qobject_cast<IRefreshModel*>(itemView->model());
  if (model)
    model->refresh();
}

void ItemViewCustomActionsManagerHelper::selectionChanged(const QItemSelection& selected
    , const QItemSelection &/*deselected*/)
{
  QAbstractItemModel* model;
  if ((model = itemView->model())) {
    emit canCutLinesChanged(canCutLines());
    emit canCopyLinesChanged(canCopyLines());
    emit canRemoveLinesChanged(canRemoveLines());
  }
}

void ItemViewCustomActionsManagerHelper::retranslateUi()
{
  QAction* a;

  a = cutLinesAction ;
  a->setText(tr("Cut lines"));
  a->setToolTip(tr("Cut selected lines to clipboard"));
  a->setStatusTip(tr("Cut selected lines to clipboard"));

  a = copyLinesAction;
  a->setText(tr("Copy lines"));
  a->setToolTip(tr("Copy selected lines to clipboard"));
  a->setStatusTip(tr("Copy selected lines to clipboard"));

  a = pasteLinesAction;
  a->setText(tr("Paste lines"));
  a->setToolTip(tr("Paste lines from clipboard"));
  a->setStatusTip(tr("Paste lines from clipboard"));

  a = removeLinesAction;
  a->setText(tr("Remove lines"));
  a->setToolTip(tr("Remove selected rows"));
  a->setStatusTip(tr("Remove selected rows"));

  a = refreshAction;
  a->setText(tr("Refresh"));
  a->setToolTip(tr("Refresh table model"));
  a->setStatusTip(tr("Refresh table model"));
}

