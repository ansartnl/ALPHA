#ifndef QTHEADERVIEW_H
#define QTHEADERVIEW_H

#include <QHeaderView>
#include <QStyledItemDelegate>

/**
* @brief The delegate is designed to edit header
*
* @class QtHeaderDelegate qtheaderview.h
*/
class QtHeaderDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  QtHeaderDelegate(QObject* parent = 0);
  virtual ~QtHeaderDelegate();

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, Qt::Orientation orientation
                        , int section) const;

  void setEditorData(QWidget *editor, QAbstractItemModel *model, Qt::Orientation orientation
                     , int section) const;

  void setModelData(QWidget *editor, QAbstractItemModel *model, Qt::Orientation orientation
                    , int section) const;

  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index);
};

class FilterModel;
class SectionsProxyModel;

/*!
  View class header for the table. You can sort, hide, secure columns
*/
class QtHeaderViewPrivate;
class QtHeaderView : public QHeaderView
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(QtHeaderView)

  Q_ENUMS(SortFlag)

  Q_PROPERTY(SortFlag sortFlag READ sortFlag WRITE setSortFlag)

  Q_PROPERTY(bool enableShowHideSections READ isShowHideSectionsEnabled WRITE setShowHideSectionsEnabled)
  Q_PROPERTY(bool enableFilter READ isFilterEnabled WRITE setFilterEnabled)
  Q_PROPERTY(bool enableFreeze READ isFreezeEnabled WRITE setFreezeEnabled)
  Q_PROPERTY(bool editable READ isEditable WRITE setEditable)

  void init();
protected:
  QtHeaderView(QtHeaderViewPrivate &dd, Qt::Orientation orientation, QWidget* parent);

  const QScopedPointer<QtHeaderViewPrivate> d_ptr;
public:

  QtHeaderView(Qt::Orientation orientation, QWidget* parent = 0);
  virtual ~QtHeaderView();

  /*!
    Type of sorting policy
  * @enum SortFlag	*/
  enum SortFlag {
    NoSort, //! Can't sort
    SimpleSort, //! Sort by one section
    MultiSort //! Sort by one or more sections
  };
  SortFlag sortFlag() const;
  void setSortFlag(SortFlag enable);

    /**
    * @brief Is it possible to hide sections
    *
    * @fn isShowHideSectionsEnabled
    * @return bool
    */
  bool isShowHideSectionsEnabled() const;
  void setShowHideSectionsEnabled(bool enabled);

    /**
    * @brief NOT USED
    *
    * @fn isFilterEnabled
    * @return bool
    */
  bool isFilterEnabled() const;
  void setFilterEnabled(bool enabled);

    /**
    * @brief Is it possible to freeze sections
    *
    * @fn isFreezeEnabled
    * @return bool
    */
  bool isFreezeEnabled() const;
  void setFreezeEnabled(bool enabled);

    /**
    * @brief Is it possible to edit section label
    *
    * @fn isEditable
    * @return bool
    */
  bool isEditable() const;
  void setEditable(bool editable);

  // Sorting insterface
  //! Remove sort indicator
  void removeSortIndicator(int logicalIndex);
  //! Remove all sort indicator
  void clearSortIndicators();
  //! Is sort indicator shown for specified section
  bool isSortIndicatorShown(int logicalIndex) const;
  //! Set the indicator if this is the second indicator to the right show the aircraftNumber of order
  void setSortIndicator(int logicalIndex, Qt::SortOrder order);
  //! Which arrow is displayed for this column. If no, then the result of Qt:: Asceding
  Qt::SortOrder sortIndicatorOrder(int logicalIndex) const;
  //! The list of columns in which to sort in the order in which they were added
  QList<int> sortIndicatorSections() const;

  // Frozen sections interface
  QList<int> frozenSections() const;

    /**
    * @brief Freeze section
    *
    * @fn setSectionFrozen
    * @param logicalIndex
    * @param frozen
    */
  void setSectionFrozen(int logicalIndex, bool frozen);
    /**
      Is section frozen
    */
  bool isSectionFrozen(int logicalIndex) const;

  virtual void setModel(QAbstractItemModel* model);
  virtual void setRootIndex(const QModelIndex &index);

  virtual QMenu* menuForSection(int logicalIndex);

signals:
  void sortIndicatorsChanged();

  void filteredSectionsChanged();

  void frozenSectionsChanged();

public slots:
  void showAllSections();

  virtual void edit(const QModelIndex &index);
  virtual bool edit(int logicalIndex);

protected:
  // Отрисовка
  virtual void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);

  void mouseDoubleClickEvent(QMouseEvent *event);

  void contextMenuEvent(QContextMenuEvent *event);

  void checkOneSection();

  virtual void changeEvent(QEvent *event);

  virtual void retranslateUi();

protected slots:
  void hideSectionTriggered();
  void showHideActionToggled(bool checked);
  void showHideMenuAboutToShow();

  void setMovableTriggered(bool movable);
  void setHighlightSectionsTriggered(bool highlight);

  void sortAscTriggered();
  void sortDescTriggered();
  void removeSortTriggered();

  void commitData(QWidget* editor);
  void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);
  void editorDestroyed(QObject* object);
private:
  Qt::SortOrder sortIndicatorOrder() const;
  int sortIndicatorSection() const;

  void releaseEditor(QWidget* editor);
};

#endif // HEADERVIEW_H
