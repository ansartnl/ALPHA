#ifndef STANDARDTREEMODEL_H
#define STANDARDTREEMODEL_H

#include <QtGui/QStandardItemModel>

//class QStandardItem;
class StandardTreeModel : public QStandardItemModel
{
	Q_OBJECT


  QChar delimiter;
  void init();
public:
	StandardTreeModel(int rows, int columns, QObject *parent = 0);
	StandardTreeModel(QObject *parent = 0);

	/**
  * @brief QStandardItem returns for a given path. All the elements found in the path are created if they were not.
	*
  * @param path Specifies the path to an element of the hierarchical model. Elements in the path separated by a slash '/'
  * @return QStandardItem Created or found an item.
	*/
  QStandardItem* itemFromPath(const QString& path);

    /**
    * @brief
    *
    * @fn pathFromItem
    * @param item Item
    * @return QString Path for a given element
    */
  QString pathFromItem(QStandardItem* item);

	/**
  * @brief Removes the item specified by the model.
	* @return bool
  \retval true Item was deleted
  \retval false Item was not deleted
	*/
  bool removeItemWithPath(const QString& path);


protected:
  QStandardItem* itemFromPath(const QString& path, int sectionPos, QStandardItem* parentItem);
  bool removeItemWithPath(const QString& path, int sectionPos, QStandardItem* parentItem);

};

#endif //#ifndef STANDARDTREEMODEL_H
