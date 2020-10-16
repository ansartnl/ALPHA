#include "qtheaderview.h"
#include "private/qtheaderview_p.h"

#include <QtDebug>

#include <QApplication>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QItemEditorFactory>
#include <QListView>
#include <QWidgetAction>

//#include "filtermodel.h"
#include "sectionsproxymodel.h"


QtHeaderDelegate::QtHeaderDelegate(QObject* parent): QStyledItemDelegate(parent)
{
  setItemEditorFactory(new QItemEditorFactory());
}
QtHeaderDelegate::~QtHeaderDelegate()
{
  delete itemEditorFactory();
}

QWidget* QtHeaderDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, Qt::Orientation orientation, int section) const
{
  return itemEditorFactory()->createEditor(QVariant::String, parent);
}

void QtHeaderDelegate::setEditorData(QWidget* editor, QAbstractItemModel* model
                                     , Qt::Orientation orientation, int section) const
{
  QVariant data;
  data = model->headerData(section, orientation, Qt::EditRole);
  editor->setProperty("text", data);
}

void QtHeaderDelegate::setModelData(QWidget* editor, QAbstractItemModel* model
                                    , Qt::Orientation orientation, int section) const
{
  model->setHeaderData(section, orientation, editor->property("text"), Qt::EditRole);
}

void QtHeaderDelegate::updateEditorGeometry(QWidget* editor
    , const QStyleOptionViewItem& option
    , const QModelIndex& index)
{
  editor->setGeometry(option.rect);
}



void QtHeaderView::init()
{
  setObjectName("qtHeaderView");
  Q_D(QtHeaderView);
  d->q_ptr = this;

  setItemDelegate(new QtHeaderDelegate(this));
  setClickable(true);

  QAction* a;

  a = d->movableAction = new QAction(tr("Moveable"), this);
  a->setObjectName("movableAction");
  a->setCheckable(true);
  a->setChecked(isMovable());
  addAction(a);

  a = d->hightlightAction = new QAction(tr("Highlight"), this);
  a->setObjectName("highlightAction");
  a->setCheckable(true);
  a->setChecked(highlightSections());
  addAction(a);

  a = d->showHideSeparator = new QAction(this);
  a->setSeparator(true);

  a = d->hideSectionAction = new QAction(tr("Hide"), this);
  a->setObjectName("hideSectionAction");
  a->setToolTip(tr("Hide section"));

  a = d->showAllSections = new QAction(tr("Show all"), this);
  a->setObjectName("showAllSections");
  a->setToolTip(tr("Show all hidden sections"));

  d->showHideSectionsMenu = new QMenu(tr("Show/hide sections"), this);

  a = d->frozenActionSeparator = new QAction(this);
  a->setSeparator(true);

  d->frozenMenu = new QMenu(tr("Frozen sections"), this);
  d->frozenSectionsListView = new QListView(this);

  QWidgetAction* wA;
  d->frozenSectionsListViewAction = wA = new QWidgetAction(this);
  wA->setDefaultWidget(d->frozenSectionsListView);
  d->frozenMenu->addAction(wA);

  d->frozenSectionsModel = new SectionsProxyModel(orientation(), this);
  d->frozenSectionsModel->setCheckStatePolicy(SectionsProxyModel::False);
  d->frozenSectionsListView->setModel(d->frozenSectionsModel);

  a = d->sortSeparator = new QAction(this);
  a->setSeparator(true);

  a = d->sortAscAction = new QAction(tr("Ascending order"), this);
  a->setObjectName("sortAscAction");
  a->setToolTip(tr("Set ascending order for section"));

  a = d->sortDescAction = new QAction(tr("Descending order"), this);
  a->setObjectName("sortDescAction");
  a->setToolTip(tr("Set descending order for section"));

  a = d->removeSortAction = new QAction(tr("Remove sort"), this);
  a->setObjectName("removeSortAction");
  a->setToolTip(tr("Unset sort for section"));

  a = d->filterViewSeparator = new QAction(this);
  a->setSeparator(true);

  d->filterView = new QListView(0);
  d->filterView->setUniformItemSizes(true);
  d->filterView->setMinimumHeight(20);

  d->filterViewAction = wA = new QWidgetAction(this);
  wA->setDefaultWidget(d->filterView);
  wA->setVisible(d->enableFilter);

  //  d->filterModel = new FilterModel(orientation(), this);
  //  d->filterView->setModel(d->filterModel);


  d->menu = new QMenu(this);
  d->menu->addAction(d->movableAction);
  d->menu->addAction(d->hightlightAction);

  d->menu->addAction(d->showHideSeparator);
  d->menu->addAction(d->hideSectionAction);
  d->menu->addAction(d->showAllSections);
  d->menu->addAction(d->showHideSectionsMenu->menuAction());

  d->menu->addAction(d->frozenActionSeparator);
  d->menu->addAction(d->frozenMenu->menuAction());

  d->menu->addAction(d->sortSeparator);
  d->menu->addAction(d->sortAscAction);
  d->menu->addAction(d->sortDescAction);
  d->menu->addAction(d->removeSortAction);

  d->menu->addAction(d->filterViewSeparator);
  d->menu->addAction(d->filterViewAction);

  connect(d->movableAction, SIGNAL(triggered(bool))
          , this, SLOT(setMovableTriggered(bool)));
  connect(d->hightlightAction, SIGNAL(triggered(bool))
          , this, SLOT(setHighlightSectionsTriggered(bool)));
  connect(d->hideSectionAction, SIGNAL(triggered()),
          this, SLOT(hideSectionTriggered()));
  connect(d->showAllSections, SIGNAL(triggered()),
          this, SLOT(showAllSections()));
  connect(d->showHideSectionsMenu, SIGNAL(aboutToShow())
          , this, SLOT(showHideMenuAboutToShow()));

  connect(d->sortAscAction, SIGNAL(triggered()),
          this, SLOT(sortAscTriggered()));
  connect(d->sortDescAction, SIGNAL(triggered()),
          this, SLOT(sortDescTriggered()));
  connect(d->removeSortAction, SIGNAL(triggered()),
          this, SLOT(removeSortTriggered()));

  //  connect(d->filterModel, SIGNAL(filterChanged())
  //          , this, SIGNAL(filteredSectionsChanged()));
  connect(d->frozenSectionsModel, SIGNAL(selectedSectionsChanged())
          , this, SIGNAL(frozenSectionsChanged()));

}

QtHeaderView::QtHeaderView(Qt::Orientation orientation, QWidget* parent)
  : QHeaderView(orientation, parent)
  , d_ptr(new QtHeaderViewPrivate)
{
  init();
}
QtHeaderView::QtHeaderView(QtHeaderViewPrivate& dd, Qt::Orientation orientation, QWidget* parent)
  : QHeaderView(orientation, parent)
  , d_ptr(&dd)
{
  init();
}


QtHeaderView::~QtHeaderView()
{
}

// -------------------------------------------------------------------
// sorting methods
QtHeaderView::SortFlag QtHeaderView::sortFlag() const
{
  Q_D(const QtHeaderView);
  return d->sortFlag;
}

void QtHeaderView::setSortFlag(SortFlag flag)
{
  Q_D(QtHeaderView);
  if (d->sortFlag != flag) {
    d->sortSeparator->setVisible(flag == SimpleSort || flag == MultiSort);
    d->sortAscAction->setVisible(flag == SimpleSort || flag == MultiSort);
    d->sortDescAction->setVisible(flag == SimpleSort || flag == MultiSort);
    d->removeSortAction->setVisible(flag == SimpleSort || flag == MultiSort);
    d->sortFlag = flag;
    d->clearSortIndicatorsWithoutRepaint();
    viewport()->repaint();
  }
}

void QtHeaderView::removeSortIndicator(int logicalIndex)
{
  Q_D(QtHeaderView);

  d->removeSortIndicatorWithoutRepaint(logicalIndex);
  viewport()->repaint();
  emit sortIndicatorsChanged();
}

void QtHeaderView::clearSortIndicators()
{
  Q_D(QtHeaderView);
  d->clearSortIndicatorsWithoutRepaint();
  viewport()->repaint();
  emit sortIndicatorsChanged();
}

bool QtHeaderView::isSortIndicatorShown(int logicalIndex) const
{
  Q_D(const QtHeaderView);
  return (d->sortingSections.contains(logicalIndex));
}

void QtHeaderView::setSortIndicator(int logicalIndex, Qt::SortOrder order)
{
  Q_D(QtHeaderView);

  if (-1 < logicalIndex && logicalIndex < count()) {
    d->sortingSections.insert(logicalIndex, order);
    if (!d->orderList.contains(logicalIndex))
      d->orderList.append(logicalIndex);
  }

  viewport()->repaint();

  emit sortIndicatorsChanged();
}

QList<int> QtHeaderView::sortIndicatorSections() const
{
  Q_D(const QtHeaderView);
  return d->orderList;
}

Qt::SortOrder QtHeaderView::sortIndicatorOrder(int logicalIndex) const
{
  Q_D(const QtHeaderView);

  if (d->sortingSections.contains(logicalIndex))
    return d->sortingSections.value(logicalIndex);

  return Qt::AscendingOrder;
}

void QtHeaderView::sortAscTriggered()
{
  Q_D(QtHeaderView);
  if (d->sortFlag == NoSort) return;

  if ((d->sortFlag == MultiSort)) {
    setSortIndicator(d->currentLogicalIndex, Qt::AscendingOrder);
  } else {
    d->clearSortIndicatorsWithoutRepaint();
    setSortIndicator(d->currentLogicalIndex, Qt::AscendingOrder);
  }
}

void QtHeaderView::sortDescTriggered()
{
  Q_D(QtHeaderView);
  if (d->sortFlag == NoSort) return;

  if ((d->sortFlag == MultiSort)) {
    setSortIndicator(d->currentLogicalIndex, Qt::DescendingOrder);
  } else {
    d->clearSortIndicatorsWithoutRepaint();
    setSortIndicator(d->currentLogicalIndex, Qt::DescendingOrder);
  }
}

void QtHeaderView::removeSortTriggered()
{
  Q_D(QtHeaderView);
  if (d->sortFlag == NoSort) return;

  if (d->sortingSections.contains(d->currentLogicalIndex))
    removeSortIndicator(d->currentLogicalIndex);
}

void QtHeaderView::mousePressEvent(QMouseEvent* event)
{
  Q_D(QtHeaderView);

  // Remember section for hide|show|sortAsc|sortDesc|removeSort Actions
  d->currentLogicalIndex = logicalIndexAt(event->pos());

  QHeaderView::mousePressEvent(event);
}

void QtHeaderView::mouseReleaseEvent(QMouseEvent* event)
{
  Q_D(QtHeaderView);

  if (event->button() == Qt::LeftButton) {
    int currentLogicalIndex = d->currentLogicalIndex;//logicalIndexAt(event->pos());
    if (currentLogicalIndex != -1) {
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
#ifndef QT_NO_CURSOR
            Qt::CursorShape splitCursor = (orientation() == Qt::Horizontal)
                    ? Qt::SplitHCursor : Qt::SplitVCursor;
            if (cursor().shape() != splitCursor) {
                edit(currentLogicalIndex);
#endif
            }
        } else
      // Можно сортировать
      if (d->sortFlag != NoSort) {
        // If multisort and shift pressed
        // Если можно сортировать по нескольким столбцам и зажат контрол
        if ((d->sortFlag == MultiSort) && (QApplication::keyboardModifiers() & Qt::ShiftModifier)) {
          if (sortIndicatorOrder(currentLogicalIndex) == Qt::DescendingOrder)
            removeSortIndicator(currentLogicalIndex);
          else if (!isSortIndicatorShown(currentLogicalIndex))
            setSortIndicator(currentLogicalIndex, Qt::AscendingOrder);
          else
            setSortIndicator(currentLogicalIndex, Qt::DescendingOrder);
          // Нужно соритровать по одному столбцу
        } else {
          if (isSortIndicatorShown(currentLogicalIndex)) {
            if (sortIndicatorOrder(currentLogicalIndex) == Qt::DescendingOrder) {
              clearSortIndicators();
            } else {
              if (sortIndicatorSections().count())
                d->clearSortIndicatorsWithoutRepaint();
              setSortIndicator(currentLogicalIndex, Qt::DescendingOrder);
            }
          } else {
              if (sortIndicatorSections().count())
                  d->clearSortIndicatorsWithoutRepaint();
              setSortIndicator(currentLogicalIndex, Qt::AscendingOrder);
          }
        }
      }
    }
  }

  QHeaderView::mouseReleaseEvent(event);
}

void QtHeaderView::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QtHeaderView);

    d->currentLogicalIndex = -1;
    QHeaderView::mouseMoveEvent(event);
}

void QtHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
  Q_D(const QtHeaderView);

  if (isSortIndicatorShown(logicalIndex)) {
    QStyleOptionHeader opt;
    initStyleOption(&opt);
    QStyle::State state = QStyle::State_None;
    if (isEnabled())
      state |= QStyle::State_Enabled;
    if (window()->isActiveWindow())
      state |= QStyle::State_Active;

    QPoint cursorPos = mapFromGlobal(cursor().pos());
    bool containMouse = rect.contains(cursorPos);
    if (isClickable()) {
      if (containMouse)
        state |= QStyle::State_MouseOver;
      if (containMouse && (QApplication::mouseButtons() && (Qt::LeftButton || Qt::RightButton)))
        state |= QStyle::State_Sunken;
      else if (highlightSections()) {
        if (d->sectionIntersectsSelection(logicalIndex))
          //if (selectionModel()->SectionIntersectsSelection(logicalIndex, rootIndex()))
          state |= QStyle::State_On;
        //            if (d->isSectionSelected(logicalIndex))
        //if (containMouse && (QApplication::mouseButtons() && (Qt::LeftButton || Qt::RightButton)))
        //  state |= QStyle::State_Sunken;
      }
    }

    opt.sortIndicator = d->sortingSections.value(logicalIndex) == Qt::AscendingOrder
                        ? QStyleOptionHeader::SortDown
                        : QStyleOptionHeader::SortUp;

    // setup the style options structure
    QVariant textAlignment = d->model->headerData(logicalIndex, orientation(),
                             Qt::TextAlignmentRole);
    opt.rect = rect;
    opt.section = logicalIndex;
    opt.state |= state;
    opt.textAlignment = Qt::Alignment(textAlignment.isValid()
                                      ? Qt::Alignment(textAlignment.toInt())
                                      : defaultAlignment());

    opt.iconAlignment = Qt::AlignVCenter;
    opt.text = d->model->headerData(logicalIndex, orientation(),
                                    Qt::DisplayRole).toString();
    if (d->orderList.count() > 1) {
      int i;
      for (i = 0; i < d->orderList.count(); ++i)
        if (d->orderList.at(i) == logicalIndex)
          break;
      // create your indication sort ordering sections
      opt.text.append('/');
      opt.text.append(QString::number(i + 1));
      // http://prog.org.ru
      /*+ (d->sortingSections.value(logicalIndex)==Qt::AscendingOrder
             ?QString(0x2193) : QString(0x2191))*/;
    }
    if (textElideMode() != Qt::ElideNone)
      opt.text = opt.fontMetrics.elidedText(opt.text, textElideMode(), rect.width() - 4);

    QVariant variant = d->model->headerData(logicalIndex, orientation(),
                                            Qt::DecorationRole);
    opt.icon = qvariant_cast<QIcon>(variant);
    if (opt.icon.isNull())
      opt.icon = qvariant_cast<QPixmap>(variant);
    QVariant foregroundBrush = d->model->headerData(logicalIndex, orientation(),
                               Qt::ForegroundRole);
    if (foregroundBrush.canConvert<QBrush>())
      opt.palette.setBrush(QPalette::ButtonText, qvariant_cast<QBrush>(foregroundBrush));

    QPointF oldBO = painter->brushOrigin();
    QVariant backgroundBrush = d->model->headerData(logicalIndex, orientation(),
                               Qt::BackgroundRole);
    if (backgroundBrush.canConvert<QBrush>()) {
      opt.palette.setBrush(QPalette::Button, qvariant_cast<QBrush>(backgroundBrush));
      opt.palette.setBrush(QPalette::Window, qvariant_cast<QBrush>(backgroundBrush));
      painter->setBrushOrigin(opt.rect.topLeft());
    }

    // the section position
    int visual = visualIndex(logicalIndex);
    Q_ASSERT(visual != -1);
    if (count() == 1)
      opt.position = QStyleOptionHeader::OnlyOneSection;
    else if (visual == 0)
      opt.position = QStyleOptionHeader::Beginning;
    else if (visual == count() - 1)
      opt.position = QStyleOptionHeader::End;
    else
      opt.position = QStyleOptionHeader::Middle;
    opt.orientation = orientation();
    // the selected position
    bool previousSelected = d->sectionIntersectsSelection(this->logicalIndex(visual - 1));
    bool nextSelected =  d->sectionIntersectsSelection(this->logicalIndex(visual + 1));
    if (previousSelected && nextSelected)
      opt.selectedPosition = QStyleOptionHeader::NextAndPreviousAreSelected;
    else if (previousSelected)
      opt.selectedPosition = QStyleOptionHeader::PreviousIsSelected;
    else if (nextSelected)
      opt.selectedPosition = QStyleOptionHeader::NextIsSelected;
    else
      opt.selectedPosition = QStyleOptionHeader::NotAdjacent;
    // draw the section
    style()->drawControl(QStyle::CE_Header, &opt, painter, this);

    painter->setBrushOrigin(oldBO);
  } else
    QHeaderView::paintSection(painter, rect, logicalIndex);
}

void QtHeaderView::setModel(QAbstractItemModel* model)
{
  Q_D(QtHeaderView);
  d->model = model;
  d->clearSortIndicatorsWithoutRepaint();
  viewport()->repaint();

  //  if (d->enableFilter)
  //    d->filterModel->setSourceModel(model);
  if (d->enableFreeze)
    d->frozenSectionsModel->setSourceModel(model);
  QHeaderView::setModel(model);
}

void QtHeaderView::setRootIndex(const QModelIndex& index)
{
  Q_D(QtHeaderView);
  //  if (d->enableFilter)
  //    d->filterModel->setSourceRootIndex(index);
  if (d->enableFreeze)
    d->frozenSectionsModel->setSourceRootIndex(index);
  QHeaderView::setRootIndex(index);
}

QMenu* QtHeaderView::menuForSection(int logicalIndex)
{
  Q_D(QtHeaderView);

  d->movableAction->setChecked(isMovable());
  d->hightlightAction->setChecked(highlightSections());

  if (d->enableShowHideSections) {
    d->hideSectionAction->setEnabled(logicalIndex != -1);
    if (hiddenSectionCount() + 1 == d->showhideSectionActionList.count())
      d->hideSectionAction->setEnabled(false);
    else
      d->hideSectionAction->setEnabled(logicalIndex != -1);

    d->showAllSections->setEnabled(hiddenSectionCount());
  }

  if (d->enableFreeze)
    d->frozenSectionsListView->reset();

  if (d->sortFlag != NoSort) {
    if (d->sortingSections.contains(logicalIndex)) {
      d->removeSortAction->setEnabled(true);
      if (d->sortingSections.value(logicalIndex) == Qt::AscendingOrder) {
        d->sortDescAction->setEnabled(true);
        d->sortAscAction->setEnabled(false);
      } else {
        d->sortDescAction->setEnabled(false);
        d->sortAscAction->setEnabled(true);
      }
    } else if (d->currentLogicalIndex != -1) {
      d->removeSortAction->setEnabled(false);
      d->sortDescAction->setEnabled(true);
      d->sortAscAction->setEnabled(true);
    } else {
      d->removeSortAction->setEnabled(false);
      d->sortDescAction->setEnabled(false);
      d->sortAscAction->setEnabled(false);
    }
  }

  if (d->enableFilter) {
    if (-1 < logicalIndex && logicalIndex < count()) {
      d->filterViewSeparator->setVisible(true);
      d->filterViewAction->setVisible(true);
      //d->filterView->setRootIndex(d->filterModel->index(logicalIndex, 0, QModelIndex()));
    } else {
      d->filterViewSeparator->setVisible(false);
      d->filterViewAction->setVisible(false);
    }
  }
  return d->menu;
}

void QtHeaderView::showAllSections()
{
  for (int i = 0; i < count(); ++i)
    setSectionHidden(i, false);
}

void QtHeaderView::hideSectionTriggered()
{
  Q_D(QtHeaderView);
  hideSection(d->currentLogicalIndex);
}


void QtHeaderView::showHideActionToggled(bool checked)
{
  Q_D(QtHeaderView);
  QAction* senderAction = qobject_cast<QAction*> (sender());
  if (senderAction)
    setSectionHidden(d->showhideSectionActionList.indexOf(senderAction), !checked);
}

void QtHeaderView::showHideMenuAboutToShow()
{
  Q_D(QtHeaderView);

  foreach(QAction * action, d->showhideSectionActionList) {
    disconnect(action, SIGNAL(toggled(bool))
               , this, SLOT(showHideActionToggled(bool)));
  }
  qDeleteAll(d->showhideSectionActionList);
  d->showhideSectionActionList.clear();

  QAction* inserting;
  for (int i = 0; i < count(); ++i) {
    inserting = new QAction(d->model->headerData(i, orientation(), Qt::DisplayRole).toString(), this);
    inserting->setCheckable(true);
    inserting->setChecked(!isSectionHidden(i));
    inserting->setToolTip(d->model->headerData(i, orientation(), Qt::ToolTipRole).toString());
    inserting->setIcon(d->model->headerData(i, orientation(), Qt::DecorationRole).value<QIcon>());
    inserting->setStatusTip(d->model->headerData(i, orientation(), Qt::StatusTipRole).toString());
    inserting->setWhatsThis(d->model->headerData(i, orientation(), Qt::WhatsThisRole).toString());
    inserting->setFont(d->model->headerData(i, orientation(), Qt::FontRole).value<QFont>());

    d->showhideSectionActionList.append(inserting);
    connect(inserting, SIGNAL(toggled(bool)),
            this, SLOT(showHideActionToggled(bool)));
  }
  // check one Section
  if (hiddenSectionCount() + 1 == d->showhideSectionActionList.count())
    for (int i = 0; i < d->showhideSectionActionList.count(); ++i)
      if (d->showhideSectionActionList.value(i)->isChecked()) {
        d->showhideSectionActionList.value(i)->setEnabled(false);
        break;
      }
  d->showHideSectionsMenu->addActions(d->showhideSectionActionList);
}

void QtHeaderView::contextMenuEvent(QContextMenuEvent* event)
{
  Q_D(QtHeaderView);
  d->currentLogicalIndex = logicalIndexAt(event->pos());
  menuForSection(logicalIndexAt(event->pos()))->exec(event->globalPos());
}


// disabling method
Qt::SortOrder QtHeaderView::sortIndicatorOrder() const
{
  return Qt::AscendingOrder;
}

int QtHeaderView::sortIndicatorSection() const
{
  return -1;
}

// ---------------------------------------------------------------
// movable and highlighted functions
void QtHeaderView::setMovableTriggered(bool movable)
{
  QHeaderView::setMovable(movable);
}

void QtHeaderView::setHighlightSectionsTriggered(bool highlight)
{
  QHeaderView::setHighlightSections(highlight);
}

// ---------------------------------------------------------------
// Edit header data methods
void QtHeaderView::mouseDoubleClickEvent(QMouseEvent* event)
{
  //Q_D(QtHeaderView);

//  int currentLogicalIndex = logicalIndexAt(event->pos());

//  if (currentLogicalIndex != -1) {
//#ifndef QT_NO_CURSOR
//    Qt::CursorShape splitCursor = (orientation() == Qt::Horizontal)
//                                  ? Qt::SplitHCursor : Qt::SplitVCursor;
//    if (cursor().shape() != splitCursor) {
//      edit(currentLogicalIndex);
//    }
//#endif
//  }

  QHeaderView::mouseDoubleClickEvent(event);
}

void QtHeaderView::commitData(QWidget* editor)
{
  Q_D(QtHeaderView);
  (qobject_cast<QtHeaderDelegate*>(itemDelegate()))->setModelData(editor, d->model, orientation()
      , d->editingLogicalSection);
}

void QtHeaderView::closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint)
{
  //Q_D(QtHeaderView);
  releaseEditor(editor);
}

void QtHeaderView::edit(const QModelIndex& index)
{
  Q_D(QtHeaderView);
  if (index.model() != d->model || !index.isValid()) return;
  edit((orientation() == Qt::Horizontal) ? index.column() : index.row());
}

bool QtHeaderView::edit(int logicalIndex)
{
  Q_D(QtHeaderView);
  if (state() != EditingState && d->editable) {
    QtHeaderDelegate* delegate = qobject_cast<QtHeaderDelegate*>(itemDelegate());
    Q_ASSERT_X(delegate != 0, "editing", "delegate is null");
    QStyleOptionViewItemV4 option;
    option.init(this);
    option.font = font();
    option.rect = QRect();
    option.state |= QStyle::State_None;
    option.locale = locale();
    option.locale.setNumberOptions(QLocale::OmitGroupSeparator);
    option.widget = this;

    QWidget* editor = delegate->createEditor(this, option, orientation(), logicalIndex);
    if (editor) {
      setState(EditingState);

      d->editingLogicalSection = logicalIndex;

      QRect indexRect;
      int xOrY = sectionViewportPosition(logicalIndex);
      if (orientation() == Qt::Horizontal)
        indexRect.setRect(xOrY, 0, sectionSize(logicalIndex), height());
      else
        indexRect.setRect(0, xOrY, width(), sectionSize(logicalIndex));

      //mapToGlobal(indexRect);
      option.rect = indexRect;
      delegate->updateEditorGeometry(editor, option, QModelIndex());
      viewport()->stackUnder(editor);

      editor->show();
      editor->installEventFilter(delegate);
      editor->setFocus();

      delegate->setEditorData(editor, model(), orientation(), logicalIndex);

      connect(delegate, SIGNAL(commitData(QWidget*))
              , this, SLOT(commitData(QWidget*)));
      connect(delegate, SIGNAL(closeEditor(QWidget*, QAbstractItemDelegate::EndEditHint))
              , this, SLOT(closeEditor(QWidget*, QAbstractItemDelegate::EndEditHint)));

      connect(editor, SIGNAL(destroyed(QObject*)),
              SLOT(editorDestroyed(QObject*)));

      //d->editor = editor;
      return true;
    }
  }
  return false;
}

void QtHeaderView::editorDestroyed(QObject* object)
{
  releaseEditor(qobject_cast<QWidget*>(object));
}

void QtHeaderView::releaseEditor(QWidget* editor)
{
  Q_D(QtHeaderView);

  //setResizeMode(d->editingLogicalSection, d->oldEditingResizeMode);
  d->editingLogicalSection = -1;
  setState(NoState);

  disconnect(editor, SIGNAL(destroyed(QObject*)),
             this, SLOT(editorDestroyed(QObject*)));
  editor->removeEventFilter(itemDelegate());
  editor->hide();
  editor->deleteLater();
}

bool QtHeaderView::isShowHideSectionsEnabled() const
{
  Q_D(const QtHeaderView);
  return d->enableShowHideSections;
}

void QtHeaderView::setShowHideSectionsEnabled(bool enabled)
{
  Q_D(QtHeaderView);
  //if (d->enableShowHideSections == enabled) return;
  d->enableShowHideSections = enabled;
  d->showHideSeparator->setVisible(enabled);
  d->showAllSections->setVisible(enabled);
  d->showHideSectionsMenu->setVisible(enabled);
}

bool QtHeaderView::isFilterEnabled() const
{
  Q_D(const QtHeaderView);
  return d->enableFilter;
}

void QtHeaderView::setFilterEnabled(bool enabled)
{
  Q_D(QtHeaderView);
  //if (d->enableFilter == enabled) return;
  d->enableFilter = enabled;
  d->filterViewSeparator->setVisible(enabled);
  d->filterViewAction->setVisible(enabled);
  //d->filterModel->setSourceModel(enabled?model():0);
  //d->filterModel->setSourceRootIndex(enabled?rootIndex():QModelIndex());
}

bool QtHeaderView::isFreezeEnabled() const
{
  Q_D(const QtHeaderView);
  return d->enableFreeze;
}

void QtHeaderView::setFreezeEnabled(bool enabled)
{
  Q_D(QtHeaderView);
  //if (d->enableFreeze == enabled) return;
  d->enableFreeze = enabled;
  d->frozenActionSeparator->setVisible(enabled);
  d->frozenMenu->menuAction()->setVisible(enabled);
  d->frozenSectionsModel->setSourceModel(enabled ? model() : 0);
  d->frozenSectionsModel->setSourceRootIndex(enabled ? rootIndex() : QModelIndex());
}

bool QtHeaderView::isEditable() const
{
  Q_D(const QtHeaderView);
  return d->editable;
}

void QtHeaderView::setEditable(bool editable)
{
  Q_D(QtHeaderView);
  if (d->enableFreeze == editable) return;
  d->editable = editable;
}

//QSet<int> QtHeaderView::filteredSections() const
//{
//  Q_D(const QtHeaderView);
//  return d->filterModel->filteredSections();
//}

QList<int> QtHeaderView::frozenSections() const
{
  Q_D(const QtHeaderView);
  QList<int> result;
  result = d->frozenSectionsModel->selectedSections().toList();
  qSort(result);
  return result;
}

void QtHeaderView::setSectionFrozen(int logicalIndex, bool frozen)
{
  Q_D(QtHeaderView);
  d->frozenSectionsModel->setData(d->frozenSectionsModel->index(logicalIndex, 0)
                                  , frozen ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
}

bool QtHeaderView::isSectionFrozen(int logicalIndex) const
{
  Q_D(const QtHeaderView);
  return d->frozenSectionsModel->selectedSections().contains(logicalIndex);
}

void QtHeaderView::retranslateUi()
{
  Q_D(QtHeaderView);
  QAction *a;
  a = d->movableAction;
  a->setText(tr("Moveable"));

  a = d->hightlightAction;
  a->setText(tr("Highlight"));

  a = d->hideSectionAction;
  a->setText(tr("Hide"));
  a->setToolTip(tr("Hide section"));

  a = d->showAllSections;
  a->setText(tr("Show all"));
  a->setToolTip(tr("Show all hidden sections"));

  d->showHideSectionsMenu->setTitle(tr("Show/hide sections"));

  d->frozenMenu->setTitle(tr("Frozen sections"));

  a = d->sortAscAction;
  a->setText(tr("Ascending order"));
  a->setToolTip(tr("Set ascending order for section"));

  a = d->sortDescAction;
  a->setText(tr("Descending order"));
  a->setToolTip(tr("Set descending order for section"));

  a = d->removeSortAction;
  a->setText(tr("Remove sort"));
  a->setToolTip(tr("Unset sort for section"));
}

void QtHeaderView::changeEvent(QEvent *event)
{

  if (event->type() == QEvent::LanguageChange) {
    retranslateUi();
  }
  QHeaderView::changeEvent(event);
}

QtHeaderViewPrivate::QtHeaderViewPrivate()
  : model(0)
  , sortFlag(QtHeaderView::MultiSort)
  , enableShowHideSections(true)
  , enableFilter(false)
  , enableFreeze(true)
  , editable(true)
  , currentLogicalIndex(-1)
  , editingLogicalSection(-1) {}

QtHeaderViewPrivate::~QtHeaderViewPrivate() {}



