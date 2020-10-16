#ifndef SECTIONSPROXYMODEL_P_H
#define SECTIONSPROXYMODEL_P_H

#include "sectionsproxymodel.h"

#include <QtCore/QSet>

class SectionsProxyModelPrivate
{
  Q_DECLARE_PUBLIC(SectionsProxyModel)
public:
  SectionsProxyModel * q_ptr;

  SectionsProxyModelPrivate();
  virtual ~SectionsProxyModelPrivate();

  QAbstractItemModel *sourceModel;
  QPersistentModelIndex sourceRootIndex;
  Qt::Orientation orientation;

  // list of sections ( list of row in this model )
  QSet<int> falseStateMap;

  SectionsProxyModel::CheckStatePolicy checkStatePolicy;
};


#endif //#ifndef SECTIONSPROXYMODEL_P_H
