#ifndef OCITABLEMODEL_H
#define OCITABLEMODEL_H

#include "ociquerymodel.h"
#include "ociutils.h"

#include "modelinterfaces.h"

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QDebug>


class QUndoStack;
/*!
  \ingroup models
Модель для чтения, изменения, добавления, удаления данных sql таблицы.
  Модель содержит последнюю строку для вставки новых данных, like MS Access.
  */
class OciTableModelPrivate;
class OciTableModel : public OciQueryModel, public IUndoRedoModel
{
  Q_OBJECT
  Q_INTERFACES(IUndoRedoModel)
  Q_DECLARE_PRIVATE(OciTableModel)

  void init();
protected:
  /*!
    Создает модель с заданным приватным классом. Для pimpl наследования.
    */
  OciTableModel(OciTableModelPrivate &dd, QObject *parent);
public:

  /*!
    Создает пустую не активную модель
    */
  explicit OciTableModel(QObject *parent = 0);
  /*!
    Уничтожает модель
    */
  virtual ~OciTableModel();


  virtual int fieldIndex(const QString fieldName);

  /*!
    Устанавливает таблицу. Если primaryKey пустой устанавливается primaryKey полученный из базы данных.
    Для заполнения модели данными необходимо вызвать QSqlQueryModel::fetchMore().
    */
  void setTable(const QString &tableName, QSqlDatabase database = QSqlDatabase());
  /*!
    Возвращает имя установленной таблицы. Может вернуть QString::null
    */
  QString tableName() const;

  /*!
    */
  virtual Qt::ItemFlags flags(const QModelIndex &index) const;

  /*!
    Если значение устанавивается для последней строки, и submitPolicy = AutoSubmit модель пробует вставить новую запись в sql таблицу, если неудачно значение сохраняется для дальнейших попыток. Добавляет команду в undoStack.

    */
  virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

  /*!
    Переопределяем данную функцию так как она используется dropMimeData.
  * @param index
  * @param QMap<int
  * @param roles
  * @return bool
  */
  virtual bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles);

  /*!
    Данные таблицы. Переопределено для того чтобы показывать значения из последней строки.
    */
  virtual QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;

  /*!
    Строк на одну больше чем в sql таблице, так как последняя строка служит для добавления новых записей.
    */
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  /*!
    Умеет удалять строки из sql таблицы
    */
  virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
  /*!
    Последняя стока обозначается знаком *
    */
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

  /*!
    Вставка строк. Sql запись заполняеться значениями установленными методом setDefault.
  Так как модель не поддерживает кэширование, то перед вставкой генерируется
  сигнал primeInsert(int, QSqlRecord&), в котором можно установить значения для sql записи.
  Данные сразу же вставляются в базу данных.
\note Если ни одно значение по умолчанию не установлено, генерируется INSERT запрос с DEFAULT значениями для столбцов первичного ключа.
    */
  virtual bool insertRows(int row, int count, const QModelIndex& parent);
  /*!
      При отпускании данных над конкретной ячейкой текст содержащийся в буфере перетаскивания вставляется в ячейку.
    */
  virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

  /*!
    Установить значение по умолчанию для вставляемой записи. После установи отображается в послейдней строке.
    */
  void setDefault(const QString &section, const QVariant &value);
  void setDefault(int section, const QVariant &value);
  QVariant defaultValue(int section) const;

  /*!
    Удалить значения по умолчанию
    */
  void clearDefaultValues();
  /*!
    Столбец доступен только для чтения
    */
  bool isColumnReadOnly(int section) const;

  QUndoStack *undoStack() const;

  bool setRecord(int row, const QSqlRecord& record);
  bool insertRecord(int row, const QSqlRecord& record);
  bool prependRecord(const QSqlRecord& record);
  bool appendRecord(const QSqlRecord& record);

public slots:
  void setColumnReadOnly(const QString &section, bool readOnly = true);
  void setColumnReadOnly(int section, bool readOnly = true);
  void setColumnsReadOnly(const QStringList& sections);

  virtual void refresh();

  void showInsertingRow(bool yes);

signals:
  void preInsert(QSqlRecord &record);
  void postInsert(const QSqlRecord &record);

  void primeInsert(int row, QSqlRecord& record);
protected:
  using OciQueryModel::setQuery;
};

#endif // OCITABLEMODEL_H
