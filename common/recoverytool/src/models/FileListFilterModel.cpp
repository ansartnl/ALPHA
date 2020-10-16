#include "FileListFilterModel.h"

CFileListFilterModel::CFileListFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    mMask("*")
{
}

bool CFileListFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
//    QModelIndex idx = sourceModel()->index(sourceRow,0, sourceParent);
//    return sourceModel()->data(idx).toString().contains(mMask);
    return true;
}
