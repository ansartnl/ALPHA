#ifndef OBJECTSMANAGERHELPER_H
#define OBJECTSMANAGERHELPER_H

#include <QtCore/QCoreApplication>
#include <QtCore/QString>

#include <QtGui/QIcon>

class QWidget;
class QStandardItem;
class QAction;

/**
* @brief Interface class for containg application/database objects.
*
* @class ObjectsManagerHelper objectsmanagerhelper.h
*/
class ObjectsManagerHelper : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString id READ id)
  Q_PROPERTY(QString path READ path)
  Q_PROPERTY(QString name READ name)
  Q_PROPERTY(QString description READ description)
  Q_PROPERTY(QIcon icon READ icon)
public:
  ObjectsManagerHelper(QObject* parent);
  virtual ~ObjectsManagerHelper();

    /**
    * @brief Строковый идентификатор объекта
    *
    * @fn id
    * @return QString
    */
  virtual QString id() const = 0;
    /**
    * @brief Путь доступа к объекту. Используется в иерархической модели.
    *
    * @fn path
    * @return QString
    */
  virtual QString path() const = 0;
    /**
    * @brief Имя объекта. Отображается пользователю.
    *
    * @fn name
    * @return QString
    */
  virtual QString name() const = 0;
    /**
    * @brief Описание объекта. Отображается пользователю.
    *
    * @fn description
    * @return QString
    */
  virtual QString description() const = 0;
    /**
    * @brief Иконка объекта. Отображается пользователю.
    *
    * @fn icon
    * @return QIcon
    */
  virtual QIcon icon() const = 0;

  virtual QWidget* widget(QWidget* parent = 0) const = 0;

  virtual QAction* action() const = 0;

  virtual void retranslateUi() = 0;
signals:
  void nameChanged();
private:
  friend class ObjectsManager;
  QStandardItem *item;
};

Q_DECLARE_INTERFACE(ObjectsManagerHelper,
                     "com.itos.aftnws.objectmanagerhelper/1.0")

#include "sqltablebrowser.h"

/**
* @brief Класс реализует доступ к объекту SqlTableBrowser, который в свою очередь отображает таблицу airplane_types
*
* @class AirplaneTypesTableHelper objectsmanagerhelper.h
*/
class AirplaneTypesTableHelper : public ObjectsManagerHelper
{
  Q_OBJECT
  Q_INTERFACES(ObjectsManagerHelper)
#define AirplaneTypesTableHelperId "airplaneTypesTable"
public:
  AirplaneTypesTableHelper(QObject* parent = 0):ObjectsManagerHelper(parent), mAction(0){
    mAction = new QAction(icon(), name(), this);
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
  }
  virtual ~AirplaneTypesTableHelper() {}

  virtual QString id() const {
    return AirplaneTypesTableHelperId;
  }
  virtual QString path() const  {
    return tr("/View/Airplane types");
  }

  virtual QString name() const {
    return tr("Airplane types");
  }

  virtual QString description() const {
    return tr("Table with airplane types");
  }

  virtual QIcon icon() const {
    return QIcon();
  }

  virtual QWidget* widget(QWidget* parent = 0) const {
    return new SqlTableBrowser("airplane_type", parent);
  }

  QAction *action()const
  {
    return mAction;
  }

  virtual void retranslateUi()
  {mAction->setText(name());
    mAction->setIcon(icon());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
  }
private:
  QAction* mAction;
};

#endif // OBJECTSMANAGERHELPER_H
