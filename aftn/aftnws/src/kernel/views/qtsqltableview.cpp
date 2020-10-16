#include "qtsqltableview.h"
#include "private/qtsqltableview_p.h"

#include <QtDebug>

#include <QtCore/QRegExp>
#include <QtCore/QMimeData>

#include <QtSql/QSqlField>

#include <QApplication>
#include <QItemSelectionModel>
#include <QDateTimeEdit>
#include <QCalendarWidget>
#include <QLineEdit>
#include <QCompleter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QMenu>
#include <QStylePainter>

#include "qtheaderview.h"

namespace
{
  class NullModel : public QAbstractItemModel
  {
    public:
      explicit NullModel(QObject* parent = 0) : QAbstractItemModel(parent) {}
      QModelIndex index(int, int, const QModelIndex&) const {
        return QModelIndex();
      }
      QModelIndex parent(const QModelIndex&) const {
        return QModelIndex();
      }
      int rowCount(const QModelIndex&) const {
        return 0;
      }
      int columnCount(const QModelIndex&) const {
        return 0;
      }
      bool hasChildren(const QModelIndex&) const {
        return false;
      }
      QVariant data(const QModelIndex&, int) const {
        return QVariant();
      }
  };

  static NullModel nullModel;
}

QtSqlItemDelegate::QtSqlItemDelegate(QObject* parent): QStyledItemDelegate(parent)
  , mTrueValue(true)
  , mFalseValue(false)
{
  setObjectName("qtSqlItemDelegate");
}

QWidget* QtSqlItemDelegate::createEditor(QWidget* parent,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
  QVariant data = index.data(Qt::EditRole);
  if (data.type() == QVariant::Bool) {
    return 0;
  } else {
    QWidget* editor = QStyledItemDelegate::createEditor(parent, option, index);

    QDateTimeEdit* dateTime = qobject_cast<QDateTimeEdit*>(editor);
    if (dateTime) {
      dateTime->setCalendarPopup(true);
      if (dateTime->calendarWidget())
        dateTime->calendarWidget()->setFirstDayOfWeek(Qt::Monday);
      return dateTime;
    }

    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
    if (lineEdit) {
      QMap<QString, int> values;
      QString itemData;
      int start = qMax(0, index.row() - 600);
      int end = qMin(index.model()->rowCount(index.parent()), index.row() + 600);
      for (int row = start; row < end; ++row) {
        itemData = index.model()->index(row, index.column(), index.parent()).data().toString();
        values[itemData]++;
      }

      QCompleter* completer = new QCompleter(values.keys(), lineEdit);
      completer->setCompletionColumn(0);
      completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
      completer->setCompletionMode(QCompleter::InlineCompletion);
      lineEdit->setCompleter(completer);
      return lineEdit;
    }

    return editor;
  }
}

void QtSqlItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
  QDateTimeEdit* dateTime = qobject_cast<QDateTimeEdit*>(editor);
  if (dateTime) {
    QVariant data = index.data(Qt::EditRole);
    if (data.isNull()) {
      QDate date = QDate::currentDate();
      QTime time = QTime::currentTime();
      time.setHMS(time.hour(), time.minute(), 0, 0);
      switch (data.type()) {
        case QVariant::Date:
          dateTime->setDate(date);
          break;
        case QVariant::DateTime:
          dateTime->setDate(date);
          dateTime->setTime(time);
          break;
        default:
          break;
      }
    }
  }

  QStyledItemDelegate::setEditorData(editor, index);
}

void QtSqlItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QVariant data = index.data(Qt::EditRole);
  QStyleOptionViewItemV4 v4(option);
  initStyleOption(&v4, index);
  const QWidget* widget;
  widget = v4.widget;
  QStyle* style = widget ? widget->style() : QApplication::style();

  if (data.type() == QVariant::Bool) {
    // Отключаем ChechStateRole и DisplayRole
    v4.features &= (~QStyleOptionViewItemV2::HasCheckIndicator);
    v4.features &= (~QStyleOptionViewItemV2::HasDisplay);
    //v4.checkState = Unchecked
    v4.text.clear();

    style->drawControl(QStyle::CE_ItemViewItem, &v4, painter, widget);

    QStyleOptionViewItem opt(option);
    opt.rect = checkRect(option, option.rect);
    opt.state = opt.state & ~QStyle::State_HasFocus;
    if (index.data() == mTrueValue)
      opt.state |= QStyle::State_On;
    else
      opt.state |= QStyle::State_Off;
    //opt.state|=(index.data().toInt() > 0 ? QStyle::State_On : QStyle::State_Off);
    style->drawPrimitive(QStyle::PE_IndicatorViewItemCheck, &opt, painter);
  } else if (data.type() == QVariant::ByteArray) {

    QStyleOptionButton pushButtonOpt;
    pushButtonOpt.initFrom(widget);

    pushButtonOpt.rect = v4.rect;
    //pushButtonOpt.features = QStyleOptionButton::Flat;
    if (v4.state & QStyle::State_HasFocus)
      pushButtonOpt.state |= QStyle::State_HasFocus;
    else
      pushButtonOpt.state &= ~QStyle::State_HasFocus;

    if (v4.state & QStyle::State_Selected)
      pushButtonOpt.state |= QStyle::State_MouseOver;
    else
      pushButtonOpt.state &= ~QStyle::State_MouseOver;

    pushButtonOpt.text = tr("BLOB");

    style->drawControl(QStyle::CE_ItemViewItem, &v4, painter, widget);

    style->drawControl(QStyle::CE_PushButton, &pushButtonOpt, painter, widget);
  } else {
    QStyledItemDelegate::paint(painter, option, index);
  }
}

QString QtSqlItemDelegate::displayText(const QVariant& value, const QLocale& locale) const
{
  switch (value.type()) {
    case QVariant::Date:
      return locale.toString(value.toDate(), "dd.MM.yyyy");
    case QVariant::DateTime:
      return locale.toString(value.toDateTime(), "dd.MM.yyyy hh:mm");
    default:
      return QStyledItemDelegate::displayText(value, locale);
  }
}

QRect QtSqlItemDelegate::checkRect
(const QStyleOptionViewItem& option,
 const QRect& bounding) const
{
  QStyleOptionButton opt;
  opt.QStyleOption::operator=(option);
  opt.rect = bounding;
  QRect cr = qApp->style()->subElementRect(QStyle::SE_ViewItemCheckIndicator, &opt);
  int deltaX = (bounding.width() - cr.width()) / 2;
  int deltaY = (bounding.height() - cr.height()) / 2;
  return QRect(bounding.left() + deltaX, bounding.top() + deltaY, cr.width(), cr.height());
}

bool QtSqlItemDelegate::editorEvent(QEvent* event
                                    , QAbstractItemModel* model
                                    , const QStyleOptionViewItem& option
                                    , const QModelIndex& index)
{
  if (!event || ! model || index.data(Qt::EditRole).type() != QVariant::Bool) {
    return QStyledItemDelegate::editorEvent(event, model, option, index);
  }

  Qt::ItemFlags flags = model->flags(index);
  if (!(option.state & QStyle::State_Enabled) || !(flags & Qt::ItemIsEnabled))
    return QStyledItemDelegate::editorEvent(event, model, option, index);

  switch (event->type()) {
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick: {
        QRect checkBoxRect(checkRect(option, option.rect));
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() != Qt::LeftButton || !checkBoxRect.contains(mouseEvent->pos()))
          return QStyledItemDelegate::editorEvent(event, model, option, index);

        // eat the double click events inside the check rect
        if (event->type() == QEvent::MouseButtonDblClick)
          return true;
        break;
      }
    case QEvent::KeyPress: {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() != Qt::Key_Space && keyEvent->key() != Qt::Key_Select)
          return QStyledItemDelegate::editorEvent(event, model, option, index);
        break;
      }
    default:
      return false;
  }

  QVariant value;
  if (index.data() == mTrueValue)
    value = mFalseValue;
  else
    value = mTrueValue;

  return model->setData(index, value, Qt::EditRole);
}

bool QtSqlItemDelegate::eventFilter(QObject* object, QEvent* event)
{
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    if (keyEvent) {
      switch (keyEvent->key()) {
        case Qt::Key_Enter:     // цифровая клавиатура
        case Qt::Key_Return: {   // основная клавиатура
            Qt::KeyboardModifiers kbrdMod = QApplication::keyboardModifiers();
            if (kbrdMod & Qt::ControlModifier) {
              emit commitData(static_cast<QWidget*>(object));
              emit closeEditor(static_cast<QWidget*>(object), QAbstractItemDelegate::EditPreviousItem);
              return true;
            }

            emit commitData(static_cast<QWidget*>(object));
            emit closeEditor(static_cast<QWidget*>(object), QAbstractItemDelegate::EditNextItem);
            return true;
          }
        default:
          break;
      }
    }
  }
  if (event->type() == QEvent::FocusIn) {
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(object);
    if (lineEdit) {
      if (lineEdit->completer())
        lineEdit->completer()->setCompletionMode(QCompleter::PopupCompletion);
    }
  }
  if (event->type() == QEvent::FocusOut) {
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(object);
    if (lineEdit) {
      if (lineEdit->completer())
        if (qApp->activeWindow() == lineEdit->completer()->popup())
          return true;
    }
  }
  return QStyledItemDelegate::eventFilter(object, event);
}


QVariant QtSqlItemDelegate::trueValue() const
{
  return mTrueValue;
}
void QtSqlItemDelegate::setTrueValue(const QVariant& value)
{
  mTrueValue = value;
}

QVariant QtSqlItemDelegate::falseValue() const
{
  return mFalseValue;
}
void QtSqlItemDelegate::setFalseValue(const QVariant& value)
{
  mFalseValue = value;
}


QtSqlTableView::QtSqlTableView(QWidget* parent)
  : QTableView(parent)
  , d_ptr(new QtSqlTableViewPrivate)
{
  init();
}

QtSqlTableView::QtSqlTableView(QtSqlTableViewPrivate& dd, QWidget* parent)
  : QTableView(parent)
  , d_ptr(&dd)
{
  init();
}

QtSqlTableView::~QtSqlTableView()
{
}

void QtSqlTableView::init()
{
  setObjectName("qtSqlTableView");
  Q_D(QtSqlTableView);
  d->q_ptr = this;

  setAlternatingRowColors(true);

  // Настройка вертикального заголовка таблицы
  verticalHeader()->setDefaultSectionSize(verticalHeader()->minimumSectionSize());

  setDragDropMode(DragDrop);
  setDragEnabled(true);

  setItemDelegate(new QtSqlItemDelegate(this));

  // Настройка горизонтального заголовка таблицы
  setHorizontalHeader(new QtHeaderView(Qt::Horizontal, this));
}

void QtSqlTableView::handleSortIndicatorsChanged()
{
  Q_D(QtSqlTableView);
  if (!model()) return;
  QList<int> sections = d->horizontalHeader->sortIndicatorSections();

  if (d->frozenTableView) {
    d->frozenTableView->horizontalHeader()->clearSortIndicators();
    foreach(int section, sections) {
      d->frozenTableView->horizontalHeader()->setSortIndicator(section, d->horizontalHeader->sortIndicatorOrder(section));
    }
  } else {
    model()->sort(-1, Qt::AscendingOrder);
    foreach(int section, sections) {
      model()->sort(section, d->horizontalHeader->sortIndicatorOrder(section));
    }
  }
}

void QtSqlTableView::handleFilterChanged()
{
  if (!model())return;
  //QSet<int> filteredSections = d->horizontalHeader->filteredSections();
  QHeaderView* vHeader = verticalHeader();
  int rowCount = model()->rowCount(rootIndex());
  for (int i = 0; i < rowCount; ++i) {
    //vHeader->setSectionHidden(i, filteredSections.contains(i));
    vHeader->setSectionHidden(i, false);
    vHeader->resizeSection(i, vHeader->defaultSectionSize());
  }
}

void QtSqlTableView::handleFrozenColumnsChanged()
{
  Q_D(QtSqlTableView);
  if (!model())return;
  QList<int> frozen = d->horizontalHeader->frozenSections();
  freezeColumns(frozen);
}

void QtSqlTableView::sortByColumn(int column, Qt::SortOrder order)
{
  Q_D(QtSqlTableView);
  d->horizontalHeader->setSortIndicator(column, order);
}

void QtSqlTableView::setSortingEnabled(bool enable)
{
  Q_D(QtSqlTableView);
  d->horizontalHeader->setSortFlag(enable ? QtHeaderView::MultiSort : QtHeaderView::NoSort);
}

QtHeaderView* QtSqlTableView::horizontalHeader() const
{
  Q_D(const QtSqlTableView);
  return d->horizontalHeader;
}

void QtSqlTableView::setHorizontalHeader(QtHeaderView* header)
{
  Q_D(QtSqlTableView);
  if (d->horizontalHeader == header) return;

  // for first uninitialised d->horizontalHeader
  disconnect(QTableView::horizontalHeader(), 0, this, 0);

  QTableView::setHorizontalHeader(header);

  d->horizontalHeader = header;

  d->horizontalHeader->setSortFlag(QtHeaderView::MultiSort);
  connect(d->horizontalHeader, SIGNAL(sortIndicatorsChanged())
          , this, SLOT(handleSortIndicatorsChanged()));
  connect(d->horizontalHeader, SIGNAL(filteredSectionsChanged())
          , this, SLOT(handleFilterChanged()));
  connect(d->horizontalHeader, SIGNAL(frozenSectionsChanged())
          , this, SLOT(handleFrozenColumnsChanged()));
}

void QtSqlTableView::setColumnFrozen(int column, bool frozen)
{
  Q_D(QtSqlTableView);
  d->horizontalHeader->setSectionFrozen(column, frozen);
}

bool QtSqlTableView::isColumnFrozen(int column) const
{
  Q_D(const QtSqlTableView);
  return d->horizontalHeader->isSectionFrozen(column);
}


void QtSqlTableView::freezeColumns(const QList<int> &columns)
{
  Q_D(QtSqlTableView);
  if (!columns.count()) {
    if (d->frozenTableView) {
      disconnect(d->horizontalHeader, SIGNAL(sectionResized(int, int, int))
                 , this, SLOT(updateSectionWidth(int, int, int)));
      disconnect(verticalHeader(), SIGNAL(sectionResized(int, int, int))
                 , this, SLOT(updateSectionHeight(int, int, int)));

      d->frozenTableView->deleteLater();
      d->frozenTableView = 0;
      d->frozenColumns.clear();

      setHorizontalScrollMode(d->oldHScrollModel);
      setVerticalScrollMode(d->oldVScrollModel);
    }
    // remove freezed columns tableView
  } else {
    QtHeaderView* frozenHeader;
    if (!d->frozenTableView) {
      // freezed columns tableView not created
      d->frozenTableView = new QtSqlTableView(this);
      d->frozenTableView->setAutoScroll(false);
      d->frozenTableView->setObjectName("");
      d->frozenTableView->setEditTriggers(editTriggers());
      d->frozenTableView->setModel(model());
      d->frozenTableView->setRootIndex(rootIndex());
      d->frozenTableView->setFocusProxy(this);
      //d->frozenTableView->setFocusPolicy(Qt::NoFocus);
      d->frozenTableView->setFrameShape(QFrame::NoFrame);
      d->frozenTableView->verticalHeader()->hide();
      d->frozenTableView->setSelectionBehavior(selectionBehavior());
      d->frozenTableView->setSelectionMode(selectionMode());
      d->frozenTableView->setSelectionModel(selectionModel());
      d->frozenTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      d->frozenTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

      frozenHeader = d->frozenTableView->horizontalHeader();
      frozenHeader->setFilterEnabled(false);
      frozenHeader->setFreezeEnabled(false);
      frozenHeader->setShowHideSectionsEnabled(false);
      frozenHeader->setMovable(false);
      frozenHeader->setMinimumHeight(d->horizontalHeader->minimumHeight());

      viewport()->stackUnder(d->frozenTableView);

      d->oldHScrollModel = horizontalScrollMode();
      d->oldVScrollModel = verticalScrollMode();

      setHorizontalScrollMode(ScrollPerPixel);
      setVerticalScrollMode(ScrollPerPixel);

      d->frozenTableView->setVerticalScrollMode(ScrollPerPixel);
      d->frozenTableView->verticalScrollBar()->setValue(verticalScrollBar()->value());

      connect(d->frozenTableView->verticalScrollBar(), SIGNAL(valueChanged(int))
              , verticalScrollBar(), SLOT(setValue(int)));
      connect(verticalScrollBar(), SIGNAL(valueChanged(int))
              , d->frozenTableView->verticalScrollBar(), SLOT(setValue(int)));

      connect(d->horizontalHeader, SIGNAL(sectionResized(int, int, int))
              , this, SLOT(updateSectionWidth(int, int, int)));
      connect(frozenHeader, SIGNAL(sectionResized(int, int, int))
              , this, SLOT(updateFrozenSectionWidth(int, int, int)));
      connect(verticalHeader(), SIGNAL(sectionResized(int, int, int))
              , this, SLOT(updateSectionHeight(int, int, int)));
      frozenHeader->viewport()->installEventFilter(this);
    } else {
      frozenHeader = d->frozenTableView->horizontalHeader();
    }

    d->frozenColumns = columns;
    int columnCount = model()->columnCount(rootIndex());
    for (int column = 0; column < columnCount; ++column) {
      if (columns.contains(column)) {
        // new frozen column
        frozenHeader->setSectionHidden(column, false);
        if (d->horizontalHeader->isSectionHidden(column)) {
          d->horizontalHeader->setSectionHidden(column, false);
          d->horizontalHeader->resizeSection(column, d->horizontalHeader->defaultSectionSize());
        }
        frozenHeader->moveSection(frozenHeader->visualIndex(column), columns.indexOf(column));
        d->horizontalHeader->moveSection(d->horizontalHeader->visualIndex(column), columns.indexOf(column));
        frozenHeader->resizeSection(column, d->horizontalHeader->sectionSize(column));
        d->frozenTableView->setItemDelegateForColumn(column, itemDelegateForColumn(column));
      } else {
        // unfreeze
        frozenHeader->setSectionHidden(column, true);
      }
    }

    updateFrozenTableGeometry();
    d->frozenTableView->show();
  }
}

QList<int> QtSqlTableView::frozenColumns() const
{
  Q_D(const QtSqlTableView);
  return d->frozenColumns;
}

void QtSqlTableView::resizeEvent(QResizeEvent* event)
{
  QTableView::resizeEvent(event);
  updateFrozenTableGeometry();
}

QModelIndex QtSqlTableView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
  Q_D(QtSqlTableView);
  QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);

  if (d->frozenTableView) {
    if (cursorAction == MoveLeft && !d->frozenColumns.contains(current.column())
        && visualRect(current).topLeft().x() < d->frozenTableView->horizontalHeader()->length()) {

      const int newValue = horizontalScrollBar()->value() + visualRect(current).topLeft().x() - d->frozenTableView->width();
      horizontalScrollBar()->setValue(newValue);
    }
  }
  return current;
}

void QtSqlTableView::updateSectionWidth(int logicalIndex, int oldSize, int newSize)
{
  Q_D(QtSqlTableView);
  if (d->frozenColumns.contains(logicalIndex)) {
    d->frozenTableView->setColumnWidth(logicalIndex, newSize);
    if (!oldSize)
      d->frozenTableView->showColumn(logicalIndex);
    if (!newSize)
      d->frozenTableView->hideColumn(logicalIndex);

    updateFrozenTableGeometry();
  }
}

void QtSqlTableView::updateFrozenSectionWidth(int logicalIndex, int, int newSize)
{
  Q_D(QtSqlTableView);
  if (d->frozenColumns.contains(logicalIndex)) {
    if (d->frozenTableView->horizontalHeader()->isSectionHidden(logicalIndex))
      hideColumn(logicalIndex);
    else
      setColumnWidth(logicalIndex, newSize);

    updateFrozenTableGeometry();
  }
}

void QtSqlTableView::updateSectionHeight(int logicalIndex, int oldSize, int newSize)
{
  Q_D(QtSqlTableView);
  d->frozenTableView->setRowHeight(logicalIndex, newSize);
  if (!oldSize)
    d->frozenTableView->showRow(logicalIndex);
  if (!newSize)
    d->frozenTableView->hideRow(logicalIndex);
}

void QtSqlTableView::updateFrozenTableGeometry()
{
  Q_D(QtSqlTableView);
  if (d->frozenTableView) {
    int frozenWidth = 0;
    for (int i = 0; i < d->frozenColumns.count(); ++i) {
      if (!d->horizontalHeader->isSectionHidden(d->frozenColumns.at(i)))
        frozenWidth += columnWidth(d->frozenColumns.at(i));
    }
    d->frozenTableView->setGeometry(verticalHeader()->width() + frameWidth()
                                    , frameWidth(), frozenWidth
                                    , viewport()->height() + horizontalHeader()->height());
  }
}

bool QtSqlTableView::eventFilter(QObject* object, QEvent* event)
{
  Q_D(QtSqlTableView);
  if (d->frozenTableView) {
    if (object == d->frozenTableView->horizontalHeader()->viewport()) {
      if (event->type() == QEvent::ContextMenu) {
        QContextMenuEvent* menuEvent = static_cast<QContextMenuEvent*>(event);
        QCoreApplication::sendEvent(d->horizontalHeader->viewport(), menuEvent);
        return true;
      }
    }
  }
  return QTableView::eventFilter(object, event);
}


QtSqlTableViewPrivate::QtSqlTableViewPrivate(): horizontalHeader(0), frozenTableView(0) {}

QtSqlTableViewPrivate::~QtSqlTableViewPrivate() {}

bool QtSqlTableView::edit(const QModelIndex& index, EditTrigger trigger, QEvent* event)
{
  Q_D(QtSqlTableView);
  if (d->frozenTableView) {
    if (d->frozenColumns.contains(index.column()))
      return d->frozenTableView->edit(index, trigger, event);
  }

  return QTableView::edit(index, trigger, event);
}

