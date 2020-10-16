#include "tabledelegate.h"
#include "private/tabledelegate_p.h"

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QTimerEvent>

#include <QMenu>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QApplication>

#include <QDesktopWidget>
#include <QScrollBar>
#include <QRadioButton>

#include <QMouseEvent>
#include <QResizeEvent>

void TableDelegate::init()
{
  //Q_D(TableDelegate);
}

TableDelegate::TableDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
    , d_ptr(new TableDelegatePrivate)
{
  init();
}

TableDelegate::TableDelegate(TableDelegatePrivate &dd, QObject *parent)
  : QStyledItemDelegate(parent)
  , d_ptr(&dd)
{
  init();
}

TableDelegate::~TableDelegate()
{
  delete d_ptr;
}

QWidget *TableDelegate::createEditor(QWidget *parent
                                     , const QStyleOptionViewItem &option
                                     , const QModelIndex &index) const
{
  Q_D(const TableDelegate);

  emit createDataEditor(option, index);
  if (d->modelsOrder.count() || !index.isValid()) {
    // create table view
    TableDelegatePrivateContainer *container = new TableDelegatePrivateContainer(option, index,
                                                                                 d_ptr, parent, Qt::Popup);
    container->initialClickPosition = parent->mapToGlobal(parent->cursor().pos());
    container->blockMouseReleaseTimer.start(QApplication::doubleClickInterval());

    connect(container, SIGNAL(commitData(QWidget*))
            , this, SIGNAL(commitData(QWidget*)));
    connect(container, SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint))
            , this, SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)));
    //container->setFocus();
    return container;
  }
  return 0;
}

void TableDelegate::setEditorData(QWidget *editor
                                  , const QModelIndex &index) const
{
  //Q_D(const TableDelegate);
  TableDelegatePrivateContainer *container = qobject_cast<TableDelegatePrivateContainer *>(editor);
  container->setEditorData(index);
}

void TableDelegate::setModelData(QWidget *editor
                                 , QAbstractItemModel *model
                                 , const QModelIndex &index) const
{
  //Q_D(const TableDelegate);
  TableDelegatePrivateContainer *container = qobject_cast<TableDelegatePrivateContainer *>(editor);
  //container->close();
  container->setModelData(model, index);
}

void TableDelegate::updateEditorGeometry(QWidget *editor
                                         , const QStyleOptionViewItem &option
                                         , const QModelIndex &index) const
{
  //Q_D(const TableDelegate);
  TableDelegatePrivateContainer *container = qobject_cast<TableDelegatePrivateContainer *>(editor);
  container->updateEditorGeometry(option, index);
}

bool TableDelegate::eventFilter(QObject *object, QEvent *event)
{
  //Q_D(TableDelegate);
  QWidget *editor = qobject_cast<QWidget*>(object);
  if (!editor)
      return false;
  if (event->type() == QEvent::FocusOut || (event->type() == QEvent::Hide && editor->isWindow())) {
    //the Hide event will take care of he editors that are in fact complete dialogs
    if (!editor->isActiveWindow() || (QApplication::focusWidget() != editor)) {
      QWidget *w = QApplication::focusWidget();
      while (w) { // don't worry about focus changes internally in the editor
        if (w == editor)
          return false;
        w = w->parentWidget();
      }

      // dont commit
      //emit commitData(editor);
      emit closeEditor(editor, NoHint);
    }
  }

  return QStyledItemDelegate::eventFilter(object, event);
}

void TableDelegate::addModel(QAbstractItemModel *model)
{
  Q_D(TableDelegate);
  d->addModel(model);
}

void TableDelegate::addModel(QAbstractItemModel *model, const QString &tag, int keyColumn
                             , const QModelIndex &rootIndex)
{
  Q_D(TableDelegate);
  d->addModel(model);
  TableDelegatePrivate::ModelInfo& modelInfo = d->modelInfo(model);
  modelInfo.tag = tag;
  modelInfo.keyColumn = keyColumn;
  modelInfo.rootIndex = rootIndex;
}

void TableDelegate::insertModel(QAbstractItemModel *before, QAbstractItemModel *model)
{
  Q_D(TableDelegate);
  d->insertModel(before, model);
}

void TableDelegate::removeModel(QAbstractItemModel *model)
{
  Q_D(TableDelegate);
  d->modelsOrder.removeAll(model);
  d->models.remove(model);
}

QList<QAbstractItemModel *> TableDelegate::models() const
{
  Q_D(const TableDelegate);
  return d->modelsOrder;
}

void TableDelegate::setTag(QAbstractItemModel *model, const QString &tag)
{
  Q_D(TableDelegate);
  d->modelInfo(model).tag = tag;
}

QString TableDelegate::tag(QAbstractItemModel *model) const
{
  Q_D(const TableDelegate);
  return d->modelInfo(model).tag;
}

void TableDelegate::setKeyColumn(QAbstractItemModel *model, int keyColumn)
{
  Q_D(TableDelegate);
  d->modelInfo(model).keyColumn = keyColumn;
}

int TableDelegate::keyColumn(QAbstractItemModel *model) const
{
  Q_D(const TableDelegate);
  return d->modelInfo(model).keyColumn;
}

void TableDelegate::setRootIndex(QAbstractItemModel *model, QModelIndex rootIndex)
{
  Q_D(TableDelegate);
  d->modelInfo(model).rootIndex = rootIndex;
}

QModelIndex TableDelegate::rootIndex(QAbstractItemModel *model) const
{
  Q_D(const TableDelegate);
  return d->modelInfo(model).rootIndex;
}

void TableDelegate::setHideKeyColumn(bool hideKeyColumn)
{
  Q_D(TableDelegate);
  d->hideKeyColumn = hideKeyColumn;
}

bool TableDelegate::isHideKeyColumn() const
{
  Q_D(const TableDelegate);
  return d->hideKeyColumn;
}

void TableDelegate::setHideHorizontalHeader(bool hideHorizontalHeader)
{
  Q_D(TableDelegate);
  d->hideHorizontalHeader = hideHorizontalHeader;
}

bool TableDelegate::isHideHorizontalHeader() const
{
  Q_D(const TableDelegate);
  return d->hideHorizontalHeader;
}

int TableDelegate::endEditHint() const
{
  Q_D(const TableDelegate);
  return d->endEditHint;
}

void TableDelegate::setEndEditHint(int endEditHint)
{
  Q_D(TableDelegate);
  d->endEditHint = static_cast<EndEditHint>(endEditHint);
}

TableDelegatePrivate::TableDelegatePrivate()
  :hideHorizontalHeader(true)
  , hideKeyColumn(true)
  , endEditHint(TableDelegate::NoHint)
{}
TableDelegatePrivate::~TableDelegatePrivate(){}

void TableDelegatePrivate::addModel(QAbstractItemModel *model)
{
  modelsOrder.append(model);
  models.insert(model, ModelInfo());
}

void TableDelegatePrivate::insertModel(QAbstractItemModel *before, QAbstractItemModel *model)
{
  modelsOrder.insert(modelsOrder.indexOf(before), model);
  models.insert(model, ModelInfo());
}

void TableDelegatePrivate::removeModel(QAbstractItemModel *model)
{
  modelsOrder.removeAll(model);
  models.remove(model);
}

TableDelegatePrivate::ModelInfo & TableDelegatePrivate::modelInfo(QAbstractItemModel *model)
{
  return models[model];
}

TableDelegatePrivate::ModelInfo TableDelegatePrivate::modelInfo(QAbstractItemModel *model) const
{
  return models.value(model);
}

TableDelegatePrivateContainer::TableDelegatePrivateContainer(const QStyleOptionViewItem &option
                                                             , const QModelIndex &index
                                                             , const TableDelegatePrivate *dd
                                                             , QWidget *parent, Qt::WindowFlags flags)
                                                               : QFrame(parent, flags)
                                                               , ptr(dd)
{
  setFrameShape(QFrame::Box);

  blockMouseReleaseTimer.setSingleShot(true);

  if (parent) // fix Qt 5.10 crash
    setAttribute(Qt::WA_WindowPropagation);

  QVBoxLayout *layout = new QVBoxLayout(this);
  //layout->setMargin(0);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  tableView = new QTableView(this);
  layout->addWidget(tableView);
  //setFocusProxy(tableView);

  const QStyleOptionViewItemV2 *option2 = qstyleoption_cast<const QStyleOptionViewItemV2*>(&option);
  const QStyleOptionViewItemV3 *option3 = qstyleoption_cast<const QStyleOptionViewItemV3*>(&option);
  //QStyleOptionViewItemV4 *option4 = qstyleoption_cast<QStyleOptionViewItemV4*>(&option);
  if (option2) {
    if (option3) {
      tableView->setLocale(option3->locale);
      tableView->setPalette(option3->palette);
    }
  }

  setLayoutDirection(option.direction);
  setFont(option.font);
  setLocale(option3->locale);
  setPalette(option3->palette);

  tableView->setLayoutDirection(option.direction);
  tableView->setFont(option.font);
  tableView->verticalHeader()->setDefaultSectionSize(tableView->verticalHeader()->minimumSectionSize());
  tableView->verticalHeader()->hide();
  tableView->horizontalHeader()->setHidden(ptr->hideHorizontalHeader);
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setFrameStyle(QFrame::NoFrame);
  tableView->setAttribute(Qt::WA_MacShowFocusRect, false);
  tableView->setLineWidth(0);
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tableView->setMouseTracking(true);

  tableView->installEventFilter(this);
  tableView->viewport()->installEventFilter(this);

  tableView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

  setFocusProxy(tableView);

  if (ptr->modelsOrder.count() > 1) {
    QRadioButton *radioButton;
    for (int i = 0; i < ptr->modelsOrder.count(); ++i) {
      radioButton = new QRadioButton(ptr->modelInfo(ptr->modelsOrder.at(i)).tag, this);
      radioButton->setProperty("_pg_index", i);
      layout->addWidget(radioButton);
      connect(radioButton, SIGNAL(toggled(bool))
              , this, SLOT(buttonToggled(bool)));
      buttons.append(radioButton);
    }
    buttons.at(0)->setChecked(true);
  } else {
    setCurrentModel(0);
  }
}

TableDelegatePrivateContainer::~TableDelegatePrivateContainer(){}

bool TableDelegatePrivateContainer::eventFilter(QObject *object, QEvent *event)
{
  switch (event->type()) {
  case QEvent::ShortcutOverride:
    switch (static_cast<QKeyEvent*>(event)->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
#ifdef QT_KEYPAD_NAVIGATION
    case Qt::Key_Select:
#endif
      if (tableView->currentIndex().isValid() && (tableView->currentIndex().flags() & Qt::ItemIsEnabled) ) {
        emit commitData(this);
        emit closeEditor(this, ptr->endEditHint);
      }
      return true;
      break;
    }
  case QEvent::MouseMove:
    if (tableView->isVisible()) {
      QMouseEvent *m = static_cast<QMouseEvent *>(event);
      QWidget *widget = static_cast<QWidget *>(object);

      QPoint vector = widget->mapToGlobal(m->pos()) - initialClickPosition;
      if (vector.manhattanLength() > 9 && blockMouseReleaseTimer.isActive())
        blockMouseReleaseTimer.stop();
      QModelIndex indexUnderMouse = tableView->indexAt(m->pos());
      if (indexUnderMouse.isValid() && indexUnderMouse != tableView->currentIndex()) {
        tableView->setCurrentIndex(indexUnderMouse);
      }
    }
    break;
  case QEvent::MouseButtonRelease: {
      QMouseEvent *m = static_cast<QMouseEvent *>(event);
      if (tableView->isVisible() && tableView->rect().contains(m->pos()) && tableView->currentIndex().isValid()
        && !blockMouseReleaseTimer.isActive()
        && m->button() == Qt::LeftButton
        && (tableView->currentIndex().flags() & Qt::ItemIsEnabled)
        && (tableView->currentIndex().flags() & Qt::ItemIsSelectable)) {
        emit commitData(this);
        emit closeEditor(this, ptr->endEditHint);
        return true;
      }
      break;
    }
  default:
    break;
  }
  return QWidget::eventFilter(object, event);
}

void TableDelegatePrivateContainer::buttonToggled(bool checked)
{
  if (checked) {
    QRadioButton *radioButton = qobject_cast<QRadioButton *>(sender());
    setCurrentModel(radioButton->property("_pg_index").toInt());
  }
}

void TableDelegatePrivateContainer::setEditorData(const QModelIndex &index) const
{
  QAbstractItemModel *tableModel = tableView->model();
  TableDelegatePrivate::ModelInfo modelInfo = ptr->modelInfo(tableModel);
  int keyColumn = modelInfo.keyColumn;
  QModelIndex rootIndex = modelInfo.rootIndex;
  int rowCount = tableModel->rowCount(rootIndex);
  QVariant data = index.data(Qt::EditRole);
  for (int i = 0; i < rowCount; ++i) {
    if (data == tableModel->index(i, keyColumn, rootIndex).data()) {
      QModelIndex index = tableModel->index(i, keyColumn, rootIndex);
      tableView->selectRow(i);
      tableView->scrollTo(index, QAbstractItemView::PositionAtCenter);
      break;
    }
  }
}

void TableDelegatePrivateContainer::setModelData(QAbstractItemModel *model, const QModelIndex &index) const
{
  QAbstractItemModel *tableModel = tableView->model();
  QModelIndex currentIndex = tableView->currentIndex();
  TableDelegatePrivate::ModelInfo modelInfo = ptr->modelInfo(tableModel);
  int keyColumn = modelInfo.keyColumn;
  //QModelIndex rootIndex = modelInfo.rootIndex;
  QVariant data = currentIndex.sibling(currentIndex.row(), keyColumn).data(Qt::EditRole);
  model->setData(index, data, Qt::EditRole);
}

void TableDelegatePrivateContainer::updateEditorGeometry(const QStyleOptionViewItem &option, const QModelIndex &index)
{
  updateGeometryAsPopup(option.rect);
}

void TableDelegatePrivateContainer::setCurrentModel(int index)
{
  QAbstractItemModel *currentModel = ptr->modelsOrder.at(index);
  const TableDelegatePrivate::ModelInfo modelInfo = ptr->modelInfo(currentModel);
  tableView->setModel(currentModel);
  tableView->setRootIndex(modelInfo.rootIndex);
  for (int i = 0; i < currentModel->columnCount(modelInfo.rootIndex); ++i)
    tableView->showColumn(i);
  tableView->horizontalHeader()->setSectionHidden(modelInfo.keyColumn, ptr->hideKeyColumn);

  updateGeometryAsPopup(property("_pg_initialRect").toRect());
}

void TableDelegatePrivateContainer::updateGeometryAsPopup(const QRect &rect)
{
  QSize buttonBoxSize(0, 0);
  if (buttons.count() > 1) {
    for (int i = 0; i < buttons.count(); ++i) {
      //qDebug() << buttons.at(i)->sizeHint();
      buttonBoxSize.setHeight(buttonBoxSize.height() + buttons.at(i)->sizeHint().height());
      buttonBoxSize.setWidth(qMax(buttonBoxSize.width(), buttons.at(i)->sizeHint().width()));
    }
  }


  tableView->horizontalHeader()->setStretchLastSection(false);
  tableView->resizeColumnsToContents();

  QSize tableViewRequiredSize(tableView->horizontalHeader()->length()
                              , tableView->verticalHeader()->length() +
                              ((tableView->horizontalHeader()->isHidden())?0
                                :tableView->horizontalHeader()->sizeHint().height()));

  QSize requieredSize(qMax(tableViewRequiredSize.width(), buttonBoxSize.width())
                      , buttonBoxSize.height() + tableViewRequiredSize.height());

  QPoint optionTopLeft = parentWidget()->mapToGlobal(rect.topLeft());

  QRect availableRect = qApp->desktop()->availableGeometry(parentWidget());
  QRect requiredRect = QRect(optionTopLeft.x(), optionTopLeft.y()
                             , qMin(requieredSize.width(), availableRect.width())
                             , qMin(requieredSize.height(), availableRect.height()));

// add width, because vertical scrollbar visible
  if (requieredSize.height() > availableRect.height())
    requiredRect.setWidth(requieredSize.width() + tableView->verticalScrollBar()->sizeHint().width() + 0);

  if (requiredRect.width() < rect.width()) {
    requiredRect.setWidth(rect.width());
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->horizontalHeader()->reset();
    //tableView->horizontalHeader()->setStretchLastSection(false);
  }

  requiredRect.setWidth(requiredRect.width() + 2*frameWidth());
  requiredRect.setHeight(requiredRect.height() + 2*frameWidth());

  if (requiredRect.left() < availableRect.left())
    requiredRect.moveRight(availableRect.left() + (requiredRect.width() - 1));

  if (requiredRect.top() < availableRect.top())
    requiredRect.moveBottom(availableRect.top() + (requiredRect.height() - 1));

  if (requiredRect.right() > availableRect.right())
    requiredRect.moveLeft(availableRect.right() - (requiredRect.width() - 1));

  if (requiredRect.bottom() > availableRect.bottom())
    requiredRect.moveTop(availableRect.bottom() - (requiredRect.height() - 1));

  setGeometry(requiredRect);
  //resize(sizeHint());
  setProperty("_pg_initialRect", rect);
}
