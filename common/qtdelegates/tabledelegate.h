#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QtCore/QTimer>

class QTableView;
//class QTreeView;
class QRadioButton;
class TableDelegatePrivate;
class TableDelegatePrivateContainer : public QFrame
{
  Q_OBJECT
protected:
  const TableDelegatePrivate * ptr;
public:
  TableDelegatePrivateContainer(const QStyleOptionViewItem &option
                                , const QModelIndex &index
                                , const TableDelegatePrivate* dd, QWidget* parent = 0, Qt::WindowFlags flags = 0);
  virtual ~TableDelegatePrivateContainer();

  virtual void setEditorData(const QModelIndex &index) const;

  virtual void setModelData(
                    QAbstractItemModel *model,
                    const QModelIndex &index) const;

  virtual void updateEditorGeometry(const QStyleOptionViewItem &option
                                    , const QModelIndex &index);
  void setCurrentModel(int index);

  QTimer blockMouseReleaseTimer;
  QPoint initialClickPosition;
signals:
  //void indexChoosed(const QModelIndex& index);
  void commitData(QWidget* editor);
  void closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint endEditHint);
protected:
  bool eventFilter(QObject *object, QEvent *event);
  void updateGeometryAsPopup(const QRect& rect);

private slots:
  void buttonToggled(bool checked);

private:
  QTableView *tableView;
  QList<QRadioButton *> buttons;

};


class TableDelegate : public QStyledItemDelegate
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(TableDelegate)

  Q_PROPERTY(bool hideHorizontalHeader READ isHideHorizontalHeader WRITE setHideHorizontalHeader)
  Q_PROPERTY(bool hideKeyColumn READ isHideKeyColumn WRITE setHideKeyColumn)
  Q_PROPERTY(int endEditHint READ endEditHint WRITE setEndEditHint)

  void init();
protected:
  TableDelegatePrivate * const d_ptr;

  TableDelegate(TableDelegatePrivate& dd, QObject* parent);
public:
  explicit TableDelegate(QObject *parent = 0);
  virtual ~TableDelegate();

  // editing
  virtual QWidget *createEditor(QWidget *parent,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;

  virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;

  virtual void setModelData(QWidget *editor,
                    QAbstractItemModel *model,
                    const QModelIndex &index) const;

  virtual void updateEditorGeometry(QWidget *editor
                                    , const QStyleOptionViewItem &option
                                    , const QModelIndex &index) const;

  void addModel(QAbstractItemModel *model);
  void addModel(QAbstractItemModel *model, const QString& tag, int keyColumn = 0
                , const QModelIndex& rootIndex = QModelIndex());
  void insertModel(QAbstractItemModel *before, QAbstractItemModel *model);
  void removeModel(QAbstractItemModel *model);

  QList<QAbstractItemModel *> models() const;

  void setTag(QAbstractItemModel *model, const QString& tag);
  QString tag(QAbstractItemModel *model) const;
  void setKeyColumn(QAbstractItemModel *model, int keyColumn);
  int keyColumn(QAbstractItemModel *model)const;
  void setRootIndex(QAbstractItemModel *model, QModelIndex rootIndex);
  QModelIndex rootIndex(QAbstractItemModel *model) const;

  void setHideKeyColumn(bool hideKeyColumn);
  bool isHideKeyColumn() const;
  void setHideHorizontalHeader(bool hideHorizontalHeader);
  bool isHideHorizontalHeader() const;

  int endEditHint() const;
  void setEndEditHint(int endEditHint);
signals:
  void createDataEditor(const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;
protected:
  virtual bool eventFilter(QObject *object, QEvent *event);

};

#endif // TABLEDELEGATE_H
