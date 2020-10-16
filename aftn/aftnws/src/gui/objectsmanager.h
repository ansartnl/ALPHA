#ifndef OBJECTSMANAGER_H
#define OBJECTSMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QModelIndex>

class QAbstractItemModel;
class QWidget;
class ObjectsManagerHelper;
class QAction;

class ObjectsManagerPrivate;

/**
* @brief Class to manage graphical objects (windows, dialogs), provides a hierarchical model of appropriate scientific facilities available to all current user.
\warning User with id == SUPERUSER_ID is superuser
*
* @class ObjectsManager objectsmanager.h
*/
class ObjectsManager : public QObject
{
  Q_OBJECT

  Q_DECLARE_PRIVATE(ObjectsManager)

  void init();

  explicit ObjectsManager(QObject* parent = 0);

  virtual ~ObjectsManager();

protected:
  QScopedPointer<ObjectsManagerPrivate> d_ptr;
  ObjectsManager(ObjectsManagerPrivate &dd, QObject* parent);

public:

  static ObjectsManager* instance();

  /**
   * List of objects
   */
  QStringList objects() const;

  /**
   * Returns true if object contains in manager.
   */
  bool contains(const QString& id) const;

  bool allow(const QString& id) const;

    /**
    * @brief Hierarchical model of objects registered in the application and for which there is access rights
    *
    * @fn model
    * @return QAbstractItemModel
    */
  QAbstractItemModel* model() const;

    /**
    * @brief
    *
    * @fn objectIdFromIndex
    * @param index index of object in model
    * @return QString string identifier of object
    */
  QString objectIdFromIndex(const QModelIndex& index) const;

    /**
    * @brief
    *
    * @fn indexFromId
    * @param id
    * @return QModelIndex
    */
  QModelIndex indexFromId(const QString& id) const;

    /**
    * @brief Call this function when you want to display object to user
    *
    * @fn widget
    * @param id
    * @param parent
    * @return QWidget widget, which provide user some functionality
    */
  QWidget* widget(const QString& id, QWidget* parent = 0) const;
  QWidget* widget(const QModelIndex& index, QWidget* parent = 0) const;

    /**
    * @brief
    *
    * @fn action
    * @param id
    * @return QAction
    */
  QAction* action(const QString& id) const;
    /**
    * @brief Actions of objects, may contain name of object, icon, description
    *
    * @fn actions
    * @return QList<QAction *>
    */
  QList<QAction*> actions() const;

    /**
    * @brief Name of object from it string identifier
    *
    * @fn objectName
    * @param id
    * @return QString
    */
  QString objectName(const QString& id) const;
    /**
    * @brief Description of object from it string identifier
    *
    * @fn objectDescription
    * @param id
    * @return QString
    */
  QString objectDescription(const QString& id);

    /**
    * @brief Register object helper (may be for widget or dialog) in object system. For builtin object call this function in
    registerBuiltinObjects
    *
    * @fn registerObject
    * @param helper
    */
  void registerObject(ObjectsManagerHelper* helper);

    /**
    * @brief Refresh object manager
    *
    * @fn refresh
    */
  void refresh();
public slots:
  void retranslateUi();

signals:
  void objectTriggered(const QString& id);

protected:
  ObjectsManagerHelper *helperFor(const QString& id) const;
  ObjectsManagerHelper *helperFor(const QModelIndex& index) const;
  void buildModel();
  bool validateObject(ObjectsManagerHelper* helper);

private:
  void registerBuiltinObjects();

};

#endif // OBJECTSMANAGER_H
