#include "sectionsproxymodel.h"
#include "private/sectionsproxymodel_p.h"

#include <QtDebug>

#include <QtCore/QMetaMethod>
#include <QtCore/QDateTime>

namespace
{
  class NullModel : public QAbstractItemModel
  {
  public:
    explicit NullModel(QObject* parent = 0) : QAbstractItemModel(parent) {}
    QModelIndex index(int, int, const QModelIndex&) const {return QModelIndex();}
    QModelIndex parent(const QModelIndex&) const {return QModelIndex();}
    int rowCount(const QModelIndex&) const {return 0;}
    int columnCount(const QModelIndex&) const {return 0;}
    bool hasChildren(const QModelIndex&) const {return false;}
    QVariant data(const QModelIndex&, int) const {return QVariant();}
  };

  static NullModel nullModel;
}

SectionsProxyModel::SectionsProxyModel(SectionsProxyModelPrivate& dd, Qt::Orientation orientation, QObject* parent)
  : QAbstractTableModel(parent)
  , d_ptr(&dd)
{
  init(orientation);
}

SectionsProxyModel::SectionsProxyModel(Qt::Orientation orientation, QObject* parent)
  : QAbstractTableModel(parent)
  , d_ptr(new SectionsProxyModelPrivate)
{
  init(orientation);
}

SectionsProxyModel::~SectionsProxyModel()
{
}

void SectionsProxyModel::init(Qt::Orientation orientation)
{
  setObjectName("sectionsProxyModel");
  Q_D(SectionsProxyModel);
  d->q_ptr = this;
  d->orientation = orientation;
}

void SectionsProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
  Q_D(SectionsProxyModel);
  if (d->sourceModel == sourceModel) return;

  disconnect(d->sourceModel, 0, this, 0);

  d->sourceModel = sourceModel ? sourceModel : &nullModel;
  d->sourceRootIndex = QModelIndex();

  const QMetaObject* sourceModelMetaObject = d->sourceModel->metaObject();
  QMetaMethod metaMethod;

  for (int i = QAbstractItemModel::staticMetaObject.methodOffset(); i < sourceModelMetaObject->methodCount(); ++i) {
    metaMethod = sourceModelMetaObject->method(i);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString sourceModelSignal = QString(metaMethod.signature());
#else
    QString sourceModelSignal = QString(metaMethod.methodSignature());
#endif

    sourceModelSignal.replace(0, 1, sourceModelSignal.at(0).toTitleCase());
    sourceModelSignal.prepend("sourceModel");
    QString thisSlotSignature = sourceModelSignal;
    if (metaMethod.methodType() == QMetaMethod::Signal
        && (metaObject()->indexOfSlot(thisSlotSignature.toLatin1().data()) != -1)) {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        connect(d->sourceModel, QString("%1%2").arg(QSIGNAL_CODE).arg(metaMethod.signature()).toLatin1().data()
                , this, QString("%1%2").arg(QSLOT_CODE).arg(thisSlotSignature.toLatin1().data()).toLatin1().data());
#else
        connect(d->sourceModel, QString("%1%2").arg(QSIGNAL_CODE).arg(QString(metaMethod.methodSignature())).toLatin1().data()
                , this, QString("%1%2").arg(QSLOT_CODE).arg(thisSlotSignature.toLatin1().data()).toLatin1().data());
#endif
    }
  }
  refresh();
}

QAbstractItemModel* SectionsProxyModel::sourceModel() const
{
  Q_D(const SectionsProxyModel);
  return d->sourceModel != &nullModel ? d->sourceModel : 0;
}

void SectionsProxyModel::setSourceRootIndex(const QModelIndex& rootIndex)
{
  Q_D(SectionsProxyModel);
  if (d->sourceRootIndex == rootIndex) return;
  d->sourceRootIndex = rootIndex;
  refresh();
}

QModelIndex SectionsProxyModel::sourceRootIndex() const
{
  Q_D(const SectionsProxyModel);
  return d->sourceRootIndex;
}


void SectionsProxyModel::setOrientation(Qt::Orientation orientation)
{
  Q_D(SectionsProxyModel);
  if (d->orientation == orientation) return;
  d->orientation = orientation;
  refresh();
}

Qt::Orientation SectionsProxyModel::orientation() const
{
  Q_D(const SectionsProxyModel);
  return d->orientation;
}

QSet<int> SectionsProxyModel::deselectedSections() const
{
  Q_D(const SectionsProxyModel);
  return d->falseStateMap;
}

QSet<int> SectionsProxyModel::selectedSections() const
{
  Q_D(const SectionsProxyModel);
  QSet<int> result;
  int rowCountVar = rowCount();
  for (int i = 0; i < rowCountVar; ++i)
    if (!d->falseStateMap.contains(i))
      result.insert(i);
  return result;
}

void SectionsProxyModel::setCheckStatePolicy(CheckStatePolicy checkStatePolicy)
{
  Q_D(SectionsProxyModel);
  if (d->checkStatePolicy == checkStatePolicy) return;
  d->checkStatePolicy = checkStatePolicy;
  refresh();
}

SectionsProxyModel::CheckStatePolicy SectionsProxyModel::checkStatePolicy() const
{
  Q_D(const SectionsProxyModel);
  return d->checkStatePolicy;
}

int SectionsProxyModel::rowCount(const QModelIndex& parent) const
{
  Q_D(const SectionsProxyModel);

  if (parent.isValid()) return 0;

  return d->orientation == Qt::Horizontal
      ?d->sourceModel->columnCount(d->sourceRootIndex)
        :d->sourceModel->rowCount(d->sourceRootIndex);
}

int SectionsProxyModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}

QVariant SectionsProxyModel::data(const QModelIndex& index, int role) const
{
  Q_D(const SectionsProxyModel);
  QVariant itemData;

  if (!index.isValid())
    return itemData;

  if (role != Qt::CheckStateRole)
    itemData = d->sourceModel->headerData(index.row(), d->orientation, role);
  else
    itemData = d->falseStateMap.contains(index.row())?Qt::Unchecked:Qt::Checked;

  return itemData;
}

bool SectionsProxyModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  Q_D(SectionsProxyModel);

  if (!index.isValid()) return false;

  if (role == Qt::CheckStateRole) {
    if (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked)
      d->falseStateMap.remove(index.row());
    else
      d->falseStateMap.insert(index.row());

    emit dataChanged(index, index);
    emit selectedSectionsChanged();
    return true;
  }

  return QAbstractTableModel::setData(index, value, role);
}

Qt::ItemFlags SectionsProxyModel::flags(const QModelIndex& index) const
{
  if (!index.isValid()) return 0;

  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsUserCheckable;
  return flags;
}

void SectionsProxyModel::sourceModelColumnsAboutToBeInserted(const QModelIndex &parent, int start, int end)
{
  Q_D(SectionsProxyModel);
  if (d->sourceRootIndex != parent) return;
  if (d->orientation != Qt::Horizontal) return;
  beginInsertRows(QModelIndex(), start, end);
}


void SectionsProxyModel::sourceModelColumnsInserted(const QModelIndex& parent, int start, int end)
{
  Q_D(SectionsProxyModel);
  if (d->sourceRootIndex != parent) return;
  if (d->orientation != Qt::Horizontal) return;
  if (d->checkStatePolicy != True) {
    for (int i = start; i <= end; ++i)
      d->falseStateMap.insert(i);
    emit selectedSectionsChanged();
  }
  endInsertRows();
}

void SectionsProxyModel::sourceModelColumnsAboutToBeRemoved(const QModelIndex& parent, int start, int end)
{
  Q_D(SectionsProxyModel);
  if (d->sourceRootIndex != parent) return;
  if (d->orientation != Qt::Horizontal) return;
  beginRemoveRows(QModelIndex(), start, end);
}

void SectionsProxyModel::sourceModelColumnsRemoved(const QModelIndex &parent, int start, int end)
{
  Q_D(SectionsProxyModel);
  if (d->sourceRootIndex != parent) return;
  if (d->orientation != Qt::Horizontal) return;
  bool anyRemoved = false;
  for (int i = start; i <= end; ++i)
    anyRemoved |= d->falseStateMap.remove(i);
  endRemoveRows();
  if (anyRemoved)
    emit selectedSectionsChanged();
}


void SectionsProxyModel::sourceModelDataChanged(const QModelIndex& topLeft
                                               , const QModelIndex& /*bottomRight*/)
{
  Q_D(SectionsProxyModel);
  if (topLeft.parent() != d->sourceRootIndex) return;
}

void SectionsProxyModel::sourceModelHeaderDataChanged(Qt::Orientation orientation, int first, int last)
{
  Q_D(SectionsProxyModel);
  if (d->orientation != orientation) return;
  emit dataChanged(index(first, 0), index(last, 0));
}

void SectionsProxyModel::sourceModelModelReset()
{
  refresh();
}


void SectionsProxyModel::sourceModelRowsAboutToBeInserted(const QModelIndex &parent, int start, int end)
{
  Q_D(SectionsProxyModel);
  if (d->sourceRootIndex != parent) return;
  if (d->orientation != Qt::Vertical) return;
  beginInsertRows(QModelIndex(), start, end);
}

void SectionsProxyModel::sourceModelRowsInserted(const QModelIndex& parent, int start, int end)
{
  Q_D(SectionsProxyModel);
  if (d->sourceRootIndex != parent) return;
  if (d->orientation != Qt::Vertical) return;
  if (d->checkStatePolicy != True) {
    for (int i = start; i <= end; ++i)
      d->falseStateMap.insert(i);
    emit selectedSectionsChanged();
  }
  endInsertRows();
}

void SectionsProxyModel::sourceModelRowsAboutToBeRemoved(const QModelIndex& parent, int start, int end)
{
  Q_D(SectionsProxyModel);
  if (d->sourceRootIndex != parent) return;
  if (d->orientation != Qt::Vertical) return;
  beginRemoveRows(QModelIndex(), start, end);
}

void SectionsProxyModel::sourceModelRowsRemoved(const QModelIndex &parent, int start, int end)
{
  Q_D(SectionsProxyModel);
  if (d->sourceRootIndex != parent) return;
  if (d->orientation != Qt::Vertical) return;
  bool anyRemoved = false;
  for (int i = start; i <= end; ++i)
    anyRemoved |= d->falseStateMap.remove(i);
  endRemoveRows();
  if (anyRemoved)
    emit selectedSectionsChanged();
}

void SectionsProxyModel::refresh()
{
  Q_D(SectionsProxyModel);
  if (d->checkStatePolicy == True)
     d->falseStateMap.clear();
  else
    for (int i = 0; i < rowCount(); ++i)
      d->falseStateMap.insert(i);
  reset();
  emit selectedSectionsChanged();
}

SectionsProxyModelPrivate::SectionsProxyModelPrivate()
  : sourceModel(&nullModel), orientation(Qt::Horizontal), checkStatePolicy(SectionsProxyModel::True) {}

SectionsProxyModelPrivate::~SectionsProxyModelPrivate() {}
