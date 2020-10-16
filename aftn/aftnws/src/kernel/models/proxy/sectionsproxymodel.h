#ifndef SECTIONSPROXYMODEL_H
#define SECTIONSPROXYMODEL_H

#include <QtCore/QAbstractTableModel>

class SectionsProxyModelPrivate;
/**
* @brief Прокси модель для отображения списка заголовков чужой модели в виде списко
*
* @class SectionsProxyModel sectionsproxymodel.h
*/
class SectionsProxyModel : public QAbstractTableModel
{
  Q_OBJECT

  Q_DECLARE_PRIVATE(SectionsProxyModel)

  void init(Qt::Orientation orientation);
protected:
  SectionsProxyModel(SectionsProxyModelPrivate& dd, Qt::Orientation orientation, QObject* parent);
  const QScopedPointer<SectionsProxyModelPrivate> d_ptr;

public:
  SectionsProxyModel(Qt::Orientation orientation, QObject* parent = 0);
  virtual ~SectionsProxyModel();

  virtual void setSourceModel(QAbstractItemModel* sourceModel);
  QAbstractItemModel* sourceModel() const;

  void setSourceRootIndex(const QModelIndex& rootIndex);
  QModelIndex sourceRootIndex() const;

  void setOrientation(Qt::Orientation orientation);
  Qt::Orientation orientation() const;

  QSet<int> deselectedSections() const;
  QSet<int> selectedSections() const;

  enum CheckStatePolicy {True, False};
  void setCheckStatePolicy(CheckStatePolicy checkStatePolicy);
  CheckStatePolicy checkStatePolicy()const;

  virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;

  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

  virtual Qt::ItemFlags flags(const QModelIndex& index) const;

signals:
  void selectedSectionsChanged();

protected slots:
  virtual void sourceModelColumnsAboutToBeInserted(const QModelIndex& parent, int start, int end);
  virtual void sourceModelColumnsInserted(const QModelIndex& parent, int start, int end);
  virtual void sourceModelColumnsRemoved(const QModelIndex& parent, int start, int end);
  virtual void sourceModelColumnsAboutToBeRemoved(const QModelIndex& parent, int start, int end);

  virtual void sourceModelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
  virtual void sourceModelHeaderDataChanged(Qt::Orientation orientation, int first, int last);
  virtual void sourceModelModelReset();

  virtual void sourceModelRowsAboutToBeInserted(const QModelIndex& parent, int start, int end);
  virtual void sourceModelRowsInserted(const QModelIndex& parent, int start, int end);
  virtual void sourceModelRowsAboutToBeRemoved(const QModelIndex& parent, int start, int end);
  virtual void sourceModelRowsRemoved(const QModelIndex& parent, int start, int end);

protected:
  void refresh();

  bool isSourceSectionIndex(const QModelIndex& index) const;
  bool isSourceSectionData(const QModelIndex& index) const;
};

#endif //#ifndef SECTIONPROXYMODEL_H
