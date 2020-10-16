
#include "standardtreemodel.h"

#include <QtGui/QStandardItem>

void StandardTreeModel::init()
{
  delimiter = '/';
}

StandardTreeModel::StandardTreeModel(int rows, int columns, QObject *parent)
  :QStandardItemModel(rows, columns, parent)
{
  init();
}

StandardTreeModel::StandardTreeModel(QObject *parent)
  :QStandardItemModel(parent)
{
  init();
}

QStandardItem* StandardTreeModel::itemFromPath(const QString &path)
{
  return itemFromPath(path, 0, 0);
}

QStandardItem* StandardTreeModel::itemFromPath(const QString& path
    , int sectionPos, QStandardItem* parentItem)
{
  if (!parentItem)
    parentItem = invisibleRootItem();

  QString section = path.section(delimiter, sectionPos + 1, sectionPos + 1);
  if (section.isNull())
    return parentItem;


  for (int i = 0; i < parentItem->rowCount(); ++i)
    if (!QString::compare(parentItem->child(i, 0)->text(), section))
      return itemFromPath(path, sectionPos + 1, parentItem->child(i, 0));

  QStandardItem* item = new QStandardItem(section);
  parentItem->setChild(parentItem->rowCount(), item);
  return itemFromPath(path, sectionPos + 1, item);
}

bool StandardTreeModel::removeItemWithPath(const QString &path)
{
  return removeItemWithPath(path, 0, 0);
}

bool StandardTreeModel::removeItemWithPath(const QString& path
    , int sectionPos, QStandardItem* parentItem)
{
  if (!parentItem)
    parentItem = invisibleRootItem();

  QString section = path.section(delimiter, sectionPos + 1, sectionPos + 1);
  if (section.isNull()) {
    delete parentItem;
    return true;
  }

  for (int i = 0; i < parentItem->rowCount(); ++i)
    if (!QString::compare(parentItem->child(i, 0)->text(), section))
      return itemFromPath(path, sectionPos + 1, parentItem->child(i, 0));

  return false;
}

QString StandardTreeModel::pathFromItem(QStandardItem *item)
{
  QString result ;
  if (!item)
    return result;

  result = item->text();

  QStandardItem *parent = item->parent();

  while (parent) {
    result.prepend(delimiter);
    result.prepend(parent->text());
    parent = parent->parent();
  }

  result.prepend(delimiter);

  return result;
}
