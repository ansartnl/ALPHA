#ifndef QTSQLTABLEVIEW_H
#define QTSQLTABLEVIEW_H

#include <QTableView>
#include <QStyledItemDelegate>

#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>

/**
* @brief Делегат автоматически отображающий check box для boolean полей
*
* @class QtSqlItemDelegate qtsqltableview.h
*/
class QtSqlItemDelegate : public QStyledItemDelegate
{
  Q_OBJECT
  QRect checkRect
    (const QStyleOptionViewItem &option,
     const QRect &bounding) const;

  Q_PROPERTY (QVariant trueValue READ trueValue WRITE setTrueValue)
  Q_PROPERTY (QVariant falseValue READ falseValue WRITE setFalseValue)
public:
  explicit QtSqlItemDelegate(QObject* parent = 0);

  QWidget* createEditor (QWidget* parent,
      const QStyleOptionViewItem& option,
      const QModelIndex& index ) const;

  void paint (QPainter* painter,
       const QStyleOptionViewItem& option,
       const QModelIndex& index) const;

  QString displayText(const QVariant &value
                      , const QLocale &locale) const;

  bool editorEvent (QEvent* event,
       QAbstractItemModel* model,
       const QStyleOptionViewItem& option,
       const QModelIndex& index);

  virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;

  QVariant trueValue() const;
  void setTrueValue(const QVariant& value);

  QVariant falseValue() const;
  void setFalseValue(const QVariant& value);

protected:
  virtual bool eventFilter(QObject *object, QEvent *event);

private:
  QVariant mTrueValue;
  QVariant mFalseValue;
};

class QtHeaderView;

class QtSqlTableViewPrivate;
/**
* @brief Tabular representation of the model, allowing a lot of the column sorting, and fixed columns
*
* @class QtSqlTableView qtsqltableview.h
*/
class QtSqlTableView : public QTableView
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(QtSqlTableView)

  void init();
protected:
  QtSqlTableView(QtSqlTableViewPrivate& dd, QWidget* parent);
  const QScopedPointer<QtSqlTableViewPrivate> d_ptr;
public:
  explicit QtSqlTableView(QWidget* parent = 0);
  virtual ~QtSqlTableView();

  virtual void sortByColumn(int column, Qt::SortOrder order);
  virtual void setSortingEnabled(bool enable);

  QtHeaderView* horizontalHeader() const;
  void setHorizontalHeader(QtHeaderView *header);

  void setColumnFrozen(int column, bool frozen);
  bool isColumnFrozen(int column) const;

protected:
  void freezeColumns(const QList<int>& columns);
  QList<int> frozenColumns() const;

  using QTableView::horizontalHeader;
  using QTableView::setHorizontalHeader;

  virtual void resizeEvent(QResizeEvent *event);
  virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);

  virtual bool eventFilter(QObject *object, QEvent *event);

  virtual bool edit(const QModelIndex &index, EditTrigger trigger,QEvent * event);
protected slots:
  virtual void handleSortIndicatorsChanged();
  virtual void handleFilterChanged();
  virtual void handleFrozenColumnsChanged();

private slots:
  void updateSectionWidth(int logicalIndex, int oldSize, int newSize);
  void updateFrozenSectionWidth(int logicalIndex, int oldSize, int newSize);
  void updateSectionHeight(int logicalIndex, int oldSize, int newSize);

private:


  void updateFrozenTableGeometry();
};


#endif // QTSQLTABLEVIEW_H
