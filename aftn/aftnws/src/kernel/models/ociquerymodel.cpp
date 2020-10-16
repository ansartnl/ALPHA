#include "ociquerymodel.h"
#include "private/ociquerymodel_p.h"

#include <QtCore/QStringList>
#include <QtCore/QMimeData>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

namespace {
  QString toHtml(const QList<QStringList>& plainText, const QString& tableTag =
      "table", const QString& tdTag = "td", const QString& trTag = "tr")
  {

    QString htmlText(QString("<%1>"));

    for (int i = 0; i < plainText.count(); ++i) {
      htmlText.append("<%2>");
      QStringList items = plainText.at(i);
      for (int g = 0; g < items.count(); ++g) {
        htmlText.append("<%3>");
        htmlText.append(items.at(g));
        htmlText.append("</%3>");
      }
      htmlText.append("</%2>");
    }
    htmlText.append("</%1>");
    return htmlText.arg(tableTag, trTag, tdTag);
  }

  QString toCsv(const QList<QStringList>& plainText, const QChar& delimiter =
      QLatin1Char(','), const QChar& enclose = QLatin1Char('"'),
    const QString& terminator = QString(QLatin1Char('\n')))
  {
    if (!plainText.count())
      return QString::null;
    QStringList resultRow;
    QStringList resultRows;
    QString resultValue;
    foreach(QStringList row, plainText)
      {
        foreach(QString value, row)
          {
            //value.replace(QRegExp("(\r\n|\n)"), " ");
            if (!enclose.isNull()) {
              resultValue.append(enclose);
              resultValue.append(value.replace(enclose, QString(2, enclose)));
              resultValue.append(enclose);
            } else
              resultValue.append(value);

            resultRow.append(resultValue);
            resultValue.clear();
          }
        resultRows.append(resultRow.join(delimiter));
        resultRow.clear();
      }
    return resultRows.join(terminator);
  }
}

#define PRINT_DEBUG(x) { qWarning() << Q_FUNC_INFO; qWarning() << x; }

OciSqlRelation::OciSqlRelation() :
  QHash<QVariant, QVariant> ()
{
}

OciSqlRelation::OciSqlRelation(const OciSqlRelation &other) :
  QHash<QVariant, QVariant> (other), mDatabase(other.mDatabase), mTableName(
    other.mTableName), mIndexColumn(other.mIndexColumn), mDisplayColumn(
    other.mDisplayColumn)
{

  const_iterator otherIt = other.begin();
  const_iterator otherEnd = other.end();

  for (; otherIt != otherEnd; ++otherIt)
    insert(otherIt.key(), otherIt.value());
}

OciSqlRelation::~OciSqlRelation()
{
  this->~QHash<QVariant, QVariant> ();
}

OciSqlRelation& OciSqlRelation::operator=(const OciSqlRelation& other)
{
  mDatabase = other.mDatabase;
  mTableName = other.mTableName;
  mIndexColumn = other.mIndexColumn;
  mDisplayColumn = other.mDisplayColumn;

  const_iterator otherIt = other.begin();
  const_iterator otherEnd = other.end();

  for (; otherIt != otherEnd; ++otherIt)
    insert(otherIt.key(), otherIt.value());
  return *this;
}

bool OciSqlRelation::setRelation(const QString &tableName,
  const QString &indexColumn, const QString &displayColumn,
  QSqlDatabase database)

{
  mDatabase = database.isValid() ? database : QSqlDatabase::database();
  mTableName = tableName;
  mIndexColumn = indexColumn;
  mDisplayColumn = displayColumn;
  return refresh();
}

bool OciSqlRelation::refresh()
{
  clear();
  QSqlQuery sqlQuery(mDatabase);

  QSqlRecord record = mDatabase.record(mTableName);
  QSqlDriver *driver = mDatabase.driver();

  //if (record.contains(mIndexColumn) && record.contains(mDisplayColumn)) {
  QSqlRecord selectRecord;
  selectRecord.append(record.field(mIndexColumn));
  selectRecord.append(record.field(mDisplayColumn));

  //QString query = mDatabase.driver()->sqlStatement(QSqlDriver::SelectStatement, mTableName, selectRecord, false);
  QString query =
      QString("SELECT %1 as first_column, %2 as second_column FROM %3") .arg(
        driver->escapeIdentifier(mIndexColumn, QSqlDriver::FieldName),
        mDisplayColumn, driver->escapeIdentifier(mTableName,
          QSqlDriver::TableName));
//  qDebug() << query;
  sqlQuery.setForwardOnly(true);
  if (sqlQuery.exec(query)) {
    while (sqlQuery.next())
      insert(sqlQuery.value(0), sqlQuery.value(1));
    return true;
  } else {
    PRINT_DEBUG(sqlQuery.lastError().text());
  }
  //}
  return false;
}

OciSqlRelation::operator const QHash<QVariant, QVariant>&() const
{
  return *this;
}

OciSqlRelation::operator QHash<QVariant, QVariant> &() const
{
  return *const_cast<OciSqlRelation*> (this);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const OciSqlRelation &relation)
{
  dbg.nospace() << "SqlRelation(" << "database:" << relation.mDatabase << ' '
      << "table:" << relation.mTableName << ' ' << "key column:"
      << relation.mIndexColumn << ' ' << "display column:"
      << relation.mDisplayColumn << ')';

  return dbg.space();
}
#endif  // #ifndef QT_NO_DEBUG_STREAM
OciQueryModel::OciQueryModel(QObject* parent) :
  QAbstractTableModel(parent), d_ptr(new OciQueryModelPrivate)
{
  init();
}

OciQueryModel::OciQueryModel(OciQueryModelPrivate &dd, QObject* parent) :
  QAbstractTableModel(parent), d_ptr(&dd)
{
  init();
}

OciQueryModel::~OciQueryModel()
{
}

void OciQueryModel::init()
{
  setObjectName("OciQueryModel");
  Q_D(OciQueryModel);
  d->q_ptr = this;
  setSupportedDragActions(Qt::CopyAction);
}

QSqlError OciQueryModel::lastError() const
{
  Q_D(const OciQueryModel);
  return d->lastError;
}

QString OciQueryModel::queryString() const
{
  Q_D(const OciQueryModel);
  return d->queryString;
}

QSqlDatabase OciQueryModel::database() const
{
  Q_D(const OciQueryModel);
  return d->database;
}

QSqlIndex OciQueryModel::primaryKey(int row) const
{
  Q_D(const OciQueryModel);

  return d->primaryKeyForRow(row);

}

void OciQueryModel::setPrimaryKey(const QSqlIndex& primaryKey)
{
  Q_D(OciQueryModel);
  d->primaryKey = primaryKey;
}

QSqlRecord OciQueryModel::record(int row) const
{
  Q_D(const OciQueryModel);

  QSqlRecord result = d->record;
  if (-1 < row && row < rowCount()) {
    for (int i = 0; i < d->record.count(); ++i) {
      result.setValue(i, index(row, i).data(Qt::EditRole));
      result.setGenerated(i, false);
    }
  }

  return result;
}

void OciQueryModel::setQuery(const QString &query, QSqlDatabase database)
{
  Q_D(OciQueryModel);

  d->filter.clear();
  d->columnHeaders.clear();
  d->lastError.setType(QSqlError::NoError);

  d->queryString = query;
  d->database = database.isValid() ? database : QSqlDatabase::database();
}

void OciQueryModel::clear()
{
  Q_D(OciQueryModel);
  d->clearModel();
  reset();
}

/*!
 Номер строки для первичного ключа
 */
int OciQueryModel::rowForPrimaryKey(const QSqlIndex& primaryKey) const
{
  Q_D(const OciQueryModel);

  return d->rowForPrimaryKey(primaryKey);
}

bool OciQueryModel::addRelation(const QString& column,
  const QString& tableName, const QString& indexColumn,
  const QString& displayColumn)
{
  Q_D(OciQueryModel);
  return addRelation(d->record.indexOf(column), tableName, indexColumn,
    displayColumn);
}

bool OciQueryModel::addRelation(int column, const QString& tableName,
  const QString& indexColumn, const QString& displayColumn)
{
  Q_D(OciQueryModel);
  if (d->addRelation(column, tableName, indexColumn, displayColumn)) {
    emit dataChanged(index(0, column), index(rowCount() - 1, column));
    return true;
  }
  return false;
}

QString OciQueryModel::relationPart(int column, RelationPart part)
{
  Q_D(OciQueryModel);
  QString result;
  if (d->relations.contains(column)) {
    switch (part) {
    case RelationTableName:
      result = d->relations.value(column).tableName();
      break;
    case RelationIndexColumn:
      result = d->relations.value(column).indexColumn();
      break;
    case RelationDisplayColumn:
      result = d->relations.value(column).displayColumn();
      break;
    }
  }
  return result;
}

void OciQueryModel::removeRelation(const QString& column)
{
  Q_D(OciQueryModel);
  removeRelation(d->record.indexOf(column));
}

void OciQueryModel::removeRelation(int column)
{
  Q_D(OciQueryModel);
  if (d->removeRelation(column))
    emit dataChanged(index(0, column), index(rowCount() - 1, column));
}

void OciQueryModel::setFilter(const QString& filter)
{
  Q_D(OciQueryModel);
  d->filter = filter;
}

QString OciQueryModel::filter() const
{
  Q_D(const OciQueryModel);
  return d->filter;
}

/*!
 Данные модели, внешние связи перезагружаются. Перед обновлением генерируется preRefresh(), после postRefresh()
 */
void OciQueryModel::refresh()
{
  Q_D(OciQueryModel);

  emit
  preRefresh();

  if (d->values.count()) {
    beginRemoveRows(QModelIndex(), 0, d->values.count() - 1);
    d->values.clear();
    endRemoveRows();
  }

  d->select();
  d->refreshRelations();
  QList<int> columns = d->relations.keys();
  for (int i = 0; i < columns.count(); ++i)
    emit dataChanged(index(0, columns.at(i)), index(rowCount() - 1, columns.at(
      i)));

  emit
  headerDataChanged(Qt::Horizontal, 0, d->record.count() - 1);

  //reset();
  emit postRefresh();
}

QModelIndex OciQueryModel::indexStr(int row, const QString& column,
  const QModelIndex &parent) const
{
  Q_D(const OciQueryModel);
  return QAbstractTableModel::index(row, d->record.indexOf(column), parent);
}

/*!
 Доступно и можно выделять/таскать
 */
Qt::ItemFlags OciQueryModel::flags(const QModelIndex &/*index*/) const
{
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

/*!
 Кол-во загруженный строк
 */
int OciQueryModel::rowCount(const QModelIndex& parent) const
{
  Q_D(const OciQueryModel);
  return parent.isValid() ? 0 : d->values.count();
}

/*!
 Кол-во столбцов в запросе/модели
 */
int OciQueryModel::columnCount(const QModelIndex& parent) const
{
  Q_D(const OciQueryModel);
  return parent.isValid() ? 0 : d->record.count();
}

/*!
 Возращает значения для роли для заголовков, для горизонтального все роли, для вертикального возращается пордяковый номер строки и только DisplayRole
 */
QVariant OciQueryModel::headerData(int section, Qt::Orientation orientation,
  int role) const
{
  Q_D(const OciQueryModel);

  if (orientation == Qt::Horizontal) {
    QVariant value;
    if (d->columnHeaders.contains(section))
      value = d->columnHeaders.value(section).value(role);
    if (role == Qt::DisplayRole && !value.isValid())
      value = d->columnHeaders.value(section).value(Qt::EditRole);
    if (value.isValid())
      return value;
    if ((role == Qt::DisplayRole || role == Qt::EditRole) && section
        < d->record.count())
      return d->record.fieldName(section);
  }
  return QAbstractTableModel::headerData(section, orientation, role);
}

/*!
 Устновка данных для заголовка модели, поддерживаются все роли и только горизонтальный заголовок.
 */
bool OciQueryModel::setHeaderData(int section, Qt::Orientation orientation,
  const QVariant& value, int role)
{
  Q_D(OciQueryModel);
  if (orientation != Qt::Horizontal || 0 > section || section >= columnCount())
    return false;

  d->columnHeaders[section][role] = value;
  emit
  headerDataChanged(orientation, section, section);
  return true;
}

/*!
 Если установлена внешняя связь вернет значение из связанной таблицы
 */
QVariant OciQueryModel::data(const QModelIndex& index, int role) const
{
  Q_D(const OciQueryModel);

  if (!index.isValid())
    return QVariant();

  int column = index.column();
  int row = index.row();

  QVariant itemData;
  switch (role) {
  case Qt::DisplayRole:
  case Qt::EditRole:
  case Qt::ToolTipRole:
  case Qt::StatusTipRole:
    itemData = d->values.at(row).at(column);
    break;
  default:
    ;//return itemData;
  }

  // Если установлен внешний ключ для столбца
  if (d->relations.contains(column)) {
    switch (role) {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
    case Qt::StatusTipRole:
      itemData = d->relations.value(column).value(itemData, itemData);
      break;
    default:
      ;
    }
  }

  if (role == Qt::ToolTipRole || role == Qt::StatusTipRole)
    itemData = itemData.isNull() ? "NULL" : itemData.toString();

  return itemData;
}

/*!
 Порядок сортировки для данного столбца
 */
QString OciQueryModel::sortClause(int column) const
{
  Q_D(const OciQueryModel);
  return d->orderedColumns.value(column);
}
/*!
 Убрать сортировку со столбца. После данного метода необходим вызов refresh() для обновления модели
 */
void OciQueryModel::removeSort(int column)
{
  sort(column, QString::null);
}
/*!
 Убрать сортировку со всех столбцов. После данного метода необходим вызов refresh() для обновления модели
 */
void OciQueryModel::clearSort()
{
  sort(-1);
}

/*!
 Сортировать данные по столбцу. Данная сортировка добавляется к уже установленной. После установки сортировки необходимо обновить модель методом refresh().
 Для сброса все сортировок вместо column необходимо передать -1, или вызвать clearSort.
 */
void OciQueryModel::sort(int column, Qt::SortOrder order)
{
  QString orderString = (order == Qt::AscendingOrder) ? "ASC" : "DESC";
  sort(column, orderString);
}

void OciQueryModel::sort(const QString& column, Qt::SortOrder order)
{
  Q_D(OciQueryModel);
  sort(d->record.indexOf(column), order);
}

void OciQueryModel::sort(const QString &column, Qt::SortOrder order,
  bool nullsFirst)
{
  Q_D(OciQueryModel);
  sort(d->record.indexOf(column), order, nullsFirst);
}

void OciQueryModel::sort(int column, Qt::SortOrder order, bool nullsFirst)
{
  QString orderString = (order == Qt::AscendingOrder) ? "ASC" : "DESC";
  orderString += (nullsFirst) ? " NULLS FIRST" : " NULLS LAST";
  sort(column, orderString);
}

void OciQueryModel::sort(const QString &column, const QString &subClause)
{
  Q_D(OciQueryModel);
  sort(d->record.indexOf(column), subClause);
}

void OciQueryModel::sort(int column, const QString &subClause)
{
  Q_D(OciQueryModel);

  if (column < 0) {
    d->orderedColumnsList.clear();
    d->orderedColumns.clear();
    return;
  }

  if (subClause.isEmpty()) {
    d->orderedColumnsList.removeOne(column);
    d->orderedColumns.remove(column);
  } else {
    d->orderedColumnsList.append(column);
    d->orderedColumns.insert(column, subClause);
  }
}

QList<int> OciQueryModel::sortedColumns() const
{
  Q_D(const OciQueryModel);
  return d->orderedColumnsList;
}

QStringList OciQueryModel::mimeTypes() const
{
  // create
  QStringList types(QAbstractTableModel::mimeTypes());
  types << "text/plain" << "text/csv;encoding=utf8" << "text/html";
  return types;
}

QMimeData* OciQueryModel::mimeData(const QModelIndexList &indexes) const
{
  //Q_D(const OciQueryModel);
  QMimeData *mimeData = QAbstractTableModel::mimeData(indexes);
  if (!mimeData)
    mimeData = new QMimeData();
  QMap<int, QMap<int, QVariant> > mapGrid;
  QStringList row;
  QList<QStringList> linesList;

  foreach(QModelIndex item, indexes)
    {
      mapGrid[item.row()][item.column()] = item.data();
    }

  QMap<int, QMap<int, QVariant> >::ConstIterator rowIt = mapGrid.constBegin();
  QMap<int, QMap<int, QVariant> >::ConstIterator rowEnd = mapGrid.constEnd();
  QMap<int, QVariant>::ConstIterator columnIt;
  QMap<int, QVariant>::ConstIterator columnEnd;

  for (; rowIt != rowEnd; ++rowIt) {
    columnIt = rowIt.value().constBegin();
    columnEnd = rowIt.value().constEnd();
    for (; columnIt != columnEnd; ++columnIt) {
      row.append(columnIt.value().toString());
    }
    linesList.append(row);
    row.clear();
  }

  QString htmlText = toHtml(linesList);

  QString plainText = toCsv(linesList, '\t', QChar(), "\r\n");
  QString csvText = toCsv(linesList).toUtf8();

  mimeData->setText(plainText);
  mimeData->setHtml(htmlText);
  mimeData->setData("text/csv;encoding=utf8", csvText.toUtf8());

  return mimeData;
}

OciQueryModelPrivate::OciQueryModelPrivate()
{
}

OciQueryModelPrivate::~OciQueryModelPrivate()
{
}

/*!
 Загрузка данных из запроса
 */
bool OciQueryModelPrivate::select()
{
  Q_Q(OciQueryModel);

  QSqlQuery selectQuery(database);
  selectQuery.setForwardOnly(true);
  if (!selectQuery.exec(selectStatment()))
    return false;

  QSqlRecord newRecord = selectQuery.record();

  if (newRecord.count() > record.count()) {
    q->beginInsertColumns(QModelIndex(), record.count(), newRecord.count() - 1);
    record = newRecord;
    q->endInsertColumns();
  } else if (newRecord.count() < record.count()) {
    q->beginRemoveColumns(QModelIndex(), newRecord.count(), record.count() - 1);
    record = newRecord;
    q->endRemoveColumns();
  }

  if (selectQuery.lastError().isValid()) {
    PRINT_DEBUG(selectQuery.lastError().text());
    return false;
  }

  // OCI workaround
  int querySize = 0;
  while (selectQuery.next()) {
    values.append(valuesForRecord(selectQuery.record()));
    ++querySize;
  }
  if (querySize > 0) {
    q->beginInsertRows(QModelIndex(), 0, querySize - 1);
    q->endInsertRows();
  }

  return true;
}

/*!
 Полностью очищаем модель, столбцы и данные и запрос
 */
void OciQueryModelPrivate::clearModel()
{
  columnHeaders.clear();
  values.clear();

  orderedColumns.clear();

  queryString.clear();
  filter.clear();
  database = QSqlDatabase();

  relations.clear();
}

bool OciQueryModelPrivate::addRelation(int sourceColumn,
  const QString &tableName, const QString &indexColumn,
  const QString &displayColumn)
{
  OciSqlRelation relation;
  if (relation.setRelation(tableName, indexColumn, displayColumn, database)) {
    relations.insert(sourceColumn, relation);
    return true;
  }
  return false;
}

bool OciQueryModelPrivate::removeRelation(int sourceColumn)
{
  return relations.remove(sourceColumn);
}

void OciQueryModelPrivate::refreshRelations()
{
  QHash<int, OciSqlRelation>::iterator it = relations.begin();
  QHash<int, OciSqlRelation>::iterator end = relations.end();
  for (; it != end; ++it)
    it.value().refresh();
}

QString OciQueryModelPrivate::selectStatment()
{
  return QString("select a.* from (%1) a %2 %3").arg(queryString,
    whereStatment(), orderStatment());
}

QString OciQueryModelPrivate::whereStatment()
{
  return (filter.isEmpty()) ? QString::null : QString("WHERE %1").arg(filter);
}

QString OciQueryModelPrivate::orderStatment()
{
  QStringList sortingColumnsClause;
  QString localOrderClause = (orderedColumnsList.isEmpty()) ? "" : "ORDER BY ";

  for (int i = 0; i < orderedColumnsList.count(); ++i) {
    sortingColumnsClause << record.fieldName(orderedColumnsList.at(i)) + " "
        + orderedColumns.value(orderedColumnsList.at(i));
  }

  return localOrderClause + sortingColumnsClause.join(",");
}

QSqlIndex OciQueryModelPrivate::primaryKeyForRow(int row) const
{
  QSqlIndex result = primaryKey;

  if (-1 < row && row < values.count())
    for (int i = 0; i < result.count(); ++i) {
      result.setValue(i, values.value(row).value(record.indexOf(
        result.fieldName(i))));
      result.setGenerated(i, true);
    }

  return result;
}

int OciQueryModelPrivate::rowForPrimaryKey(const QSqlIndex &primaryKey) const
{
  int row = -1;

  QHash<int, int> pkeyMap;
  bool success;
  int pkeyColumnCount = primaryKey.count();
  for (int i = 0; i < pkeyColumnCount; ++i)
    pkeyMap[i] = record.indexOf(primaryKey.fieldName(i));

  for (int i = 0; i < values.count(); ++i) {
    success = true;
    for (int g = 0; g < pkeyColumnCount; ++g)
      if (primaryKey.value(g) != values.at(i).at(pkeyMap.value(g))) {
        success = false;
        break;
      }
    if (success) {
      row = i;
      break;
    }
  }
  return row;
}

QList<QVariant> OciQueryModelPrivate::valuesForRecord(const QSqlRecord &record) const
{
  QList<QVariant> result;
  for (int i = 0; i < record.count(); ++i)
    if (record.isNull(i))
      result.append(QVariant(record.field(i).type()));
    else
      result.append(castedValue(record.field(i), record.value(i)));

  return result;
}

QVariant OciQueryModelPrivate::castedValue(const QSqlField &field,
  const QVariant &value) const
{
  QVariant result(field.type());
  if (value.isNull())
    return result;

  switch (field.type()) {
  case QVariant::Bool:
    result = value.toBool();
    break;
  case QVariant::UInt:
    result = value.toUInt();
    break;
  case QVariant::ULongLong:
    result = value.toULongLong();
    break;
  case QVariant::LongLong:
    result = value.toLongLong();
    break;
  case QVariant::Double:
    result = value.toDouble();
    break;
  case QVariant::Int:
    result = value.toInt();
    break;
  case QVariant::DateTime:
    result = value.toDateTime();
    break;
  case QVariant::Date:
    result = value.toDate();
    break;
  default:
    result = value;
    break;
  }

  return result;
}
