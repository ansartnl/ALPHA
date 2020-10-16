#include "notamhistorybrowser.h"
#include "notamviewer.h"
#include "notam.h"
#include "aftnmessageprocessor.h"
#include "AFTNDao.h"

#include "qtsqltableview.h"
#include "qtheaderview.h"

#include "translations.h"

#include <QtGui/QMessageBox>

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>

#define QL1S        QLatin1String
#define QS_LOWER(s) QString(s).toLower()

/** Constructor and destructor */
NotamHistoryBrowser::NotamHistoryBrowser(QWidget* parent) :
  AbstractBrowser(parent), notamViewer(NULL)
{
  setupUi(this);

  // create and initialize model
  model = new QSqlTableModel(this);
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  model->setTable(TBL_NOTAM);
  QStringList systemColumns = QStringList() << "FIR" << "WTC" << "TRTIME"
      << "EXFPL" << "VISIBILITY";
//  QStringList systemColumns = QStringList() << TBL_NOTAM_ID;

  foreach (const QString& column, systemColumns)
      model->removeColumn(model->fieldIndex(column), QModelIndex());

  // create table view
  table->setModel(model);
  table->horizontalHeader()->setSortFlag(QtHeaderView::SimpleSort);

  secondModel = new QSqlTableModel(this);
  secondModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
  secondModel->setTable(TBL_NOTAM);

  systemColumns = QStringList() << "FIR" << "WTC" << "TRTIME" << "EXFPL"
      << "VISIBILITY";
//  systemColumns = QStringList() << TBL_NOTAM_ID;

  foreach (const QString& column, systemColumns)
      secondModel->removeColumn(secondModel->fieldIndex(column), QModelIndex());

  secondTable->setModel(secondModel);
  secondTable->horizontalHeader()->setSortFlag(QtHeaderView::SimpleSort);

  filterAction->setShortcut(QString("Ctrl+F"));

  fromDateFilter->setDateTime(QDateTime::currentDateTime().toUTC());
  toDateFilter->setDateTime(QDateTime::currentDateTime().toUTC());

  applyTimeFilter->setShortcut(QString("Ctrl+T"));

  connect(table, SIGNAL(doubleClicked(const QModelIndex&)), this,
    SLOT(onDisplayItem(const QModelIndex&)));
  connect(secondTable, SIGNAL(doubleClicked(const QModelIndex&)), this,
    SLOT(onDisplayItem(const QModelIndex&)));
  connect(table->selectionModel(),
    SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
    SLOT(onTableCurrentChanged(const QModelIndex&, const QModelIndex&)));

  QSqlQueryModel* natureFilterModel = new QSqlQueryModel(natureFilter);
  natureFilterModel->setQuery(
    "SELECT DISTINCT " TBL_NOTAM_NATURE " FROM " TBL_NOTAM " UNION SELECT NULL FROM DUAL ORDER BY 1 NULLS FIRST");
  natureFilter->setModel(natureFilterModel);

  QSqlQueryModel* firFilterModel = new QSqlQueryModel(firFilter);
  firFilterModel->setQuery(
    "SELECT DISTINCT " TBL_NOTAM_FIR " FROM " TBL_NOTAM " UNION SELECT NULL FROM DUAL ORDER BY 1 NULLS FIRST");
  firFilter->setModel(firFilterModel);

  connect(firFilter, SIGNAL(currentIndexChanged(QString)), this,
    SLOT(updateModel()));
  connect(natureFilter, SIGNAL(currentIndexChanged(QString)), this,
    SLOT(updateModel()));
  connect(applyTimeFilter, SIGNAL(clicked()), this, SLOT(updateModel()));
  connect(fromDateFilter, SIGNAL(dateTimeChanged(QDateTime)), this,
    SLOT(onFromTimeFilterChanged(QDateTime)));
  connect(toDateFilter, SIGNAL(dateTimeChanged(QDateTime)), this,
    SLOT(onUntilTimeFilterChanged(QDateTime)));

  connect(filterAction, SIGNAL(clicked()), this, SLOT(updateModel()));
  connect(filterEdit, SIGNAL(textChanged(QString)), this,
    SLOT(onTextFilterChanged(QString)));

  updateModel();
  Translations::translateSqlTableModel(model);
  Translations::translateSqlTableModel(secondModel);

  restoreState(staticMetaObject.className());
}

void NotamHistoryBrowser::onTableCurrentChanged(const QModelIndex& cur,
  const QModelIndex& prew) // [slot]
{

  if (cur.isValid()) {
    description->setText(model->data(model->index(cur.row(), model->fieldIndex(
      TBL_NOTAM_DESCRIPTION))).toString());
    QString curSequenceId = model->data(model->index(cur.row(),
      model->fieldIndex(TBL_NOTAM_SEQUENCE_ID))).toString();

    if ("0" != curSequenceId) {
      secondModel->setSort(0, Qt::DescendingOrder);
      secondModel->setFilter(TBL_NOTAM_ID " = " + model->data(model->index(
        cur.row(), model->fieldIndex(TBL_NOTAM_ID))).toString()
          + " OR " TBL_NOTAM_SEQUENCE_ID " = " + curSequenceId);
      secondTable->resizeColumnsToContents();
      prevSequenceId = curSequenceId;
    } else if (prevSequenceId != curSequenceId) {
      secondModel->setFilter("1=0");
      prevSequenceId = "0";
    }
  } else {
    description->setText("");
    secondModel->setFilter("1=0");
    prevSequenceId = "0";
  }

  secondModel->select();
}

void NotamHistoryBrowser::onDisplayItem(const QModelIndex& index) //[slot]
{
  if (index.isValid()) {
    const QSqlTableModel* currentModel = qobject_cast<const QSqlTableModel*> (
      index.model());
    if (!currentModel)
      return;
    Notam notam(currentModel->record(index.row()));

    viewer()->setNotam(notam);
    viewer()->exec();
  }
}

void NotamHistoryBrowser::onFromTimeFilterChanged(const QDateTime& dtFrom) //[slot]
{
  if (dtFrom > toDateFilter->dateTime()) {
    fromDateFilter->setDateTime(toDateFilter->dateTime());
    return;
  }

  if (applyTimeFilter->isChecked())
    updateModel();
}

void NotamHistoryBrowser::onUntilTimeFilterChanged(const QDateTime& dtUntil) //[slot]
{
  if (dtUntil < fromDateFilter->dateTime())
    toDateFilter->setDateTime(fromDateFilter->dateTime());
  else if (applyTimeFilter->isChecked())
    updateModel();
}

void NotamHistoryBrowser::onTextFilterChanged(const QString&) //[slot]
{
  if (filterAction->isChecked())
    updateModel();
}

/** Update objects model */
void NotamHistoryBrowser::updateModel() //[slot]
{
  QString sqlFilter;
  QString sqlFirFilter(firFilter->currentText());
  QString sqlNatureFilter(natureFilter ? natureFilter->currentText() : "");
  QString sqlLikeFilter;

  if (applyTimeFilter && applyTimeFilter->isChecked()) // select all records which were valid between FromTimeFilter and UntilFimeFilter
    sqlFilter = "(" TBL_NOTAM_VALID_FROM " <  timestamp'"
        + toDateFilter->dateTime().toString("yyyy-MM-dd hh:mm:ss")
        + "' and ( " TBL_NOTAM_VALID_UNTIL " > timestamp'"
        + fromDateFilter->dateTime().toString("yyyy-MM-dd hh:mm:ss")
        + "' or " TBL_NOTAM_VALID_UNTIL " is null))";

  if (filterAction->isChecked()) {
    QString sqlLikePattern;
    QString sqlLikeOrPattern;

    // LIKE '%filter%'
    sqlLikePattern.append(" LIKE '%");
    sqlLikePattern.append(filterEdit->text());
    sqlLikePattern.append("%' ");

    sqlLikeOrPattern = sqlLikePattern;
    sqlLikeOrPattern.append("OR ");

    sqlLikeFilter = "(" TBL_NOTAM_SERIES + sqlLikeOrPattern + TBL_NOTAM_SNUMBER
        + sqlLikeOrPattern + TBL_NOTAM_YEAR + sqlLikeOrPattern
        + TBL_NOTAM_REPLACE_SNUMBER + sqlLikeOrPattern + TBL_NOTAM_REPLACE_YEAR
        + sqlLikeOrPattern + TBL_NOTAM_SUBJECT + sqlLikeOrPattern
        + TBL_NOTAM_CONDITION + sqlLikeOrPattern + TBL_NOTAM_TRAFFIC
        + sqlLikeOrPattern + TBL_NOTAM_PURPOSE + sqlLikeOrPattern
        + TBL_NOTAM_SCOPE + sqlLikeOrPattern + TBL_NOTAM_Q_LOWER_LIMIT
        + sqlLikeOrPattern + TBL_NOTAM_Q_UPPER_LIMIT + sqlLikeOrPattern
        + TBL_NOTAM_LOCATION + sqlLikeOrPattern + TBL_NOTAM_RADIUS
        + sqlLikeOrPattern + TBL_NOTAM_AERODROME + sqlLikeOrPattern
        + SQL_TIMESTAMP_TO_CHAR(TBL_NOTAM_VALID_FROM) + sqlLikeOrPattern
        + SQL_TIMESTAMP_TO_CHAR(TBL_NOTAM_VALID_UNTIL) + sqlLikeOrPattern
        + TBL_NOTAM_SCHEDULE + sqlLikeOrPattern + TBL_NOTAM_DESCRIPTION
        + sqlLikeOrPattern + TBL_NOTAM_LOWER_LIMIT + sqlLikeOrPattern
        + TBL_NOTAM_UPPER_LIMIT + sqlLikePattern;

    if (sqlFirFilter.isEmpty())
      sqlLikeFilter += " OR " TBL_NOTAM_FIR + sqlLikePattern;

    if (sqlNatureFilter.isEmpty())
      sqlLikeFilter += " OR " TBL_NOTAM_NATURE + sqlLikePattern;

    sqlLikeFilter += ")";
  }

  if (!sqlFirFilter.isEmpty())
    sqlFilter = TBL_NOTAM_FIR " = '" + sqlFirFilter + "'";

  if (!sqlNatureFilter.isEmpty()) {
    if (!sqlFilter.isEmpty())
      sqlFilter += " AND ";

    sqlFilter += TBL_NOTAM_NATURE " = '" + sqlNatureFilter + "'";
  }

  if (!sqlLikeFilter.isEmpty()) {
    if (!sqlFilter.isEmpty())
      sqlFilter += " AND ";

    sqlFilter += sqlLikeFilter;
  }

  model->setFilter(sqlFilter);
  model->select();

  onTableCurrentChanged(QModelIndex(), QModelIndex());
}

NotamViewer* NotamHistoryBrowser::viewer()
{
  if (!notamViewer)
    notamViewer.reset(new NotamViewer(this));

  return notamViewer.data();
}

void NotamHistoryBrowser::changeEvent(QEvent* event)
{
  QWidget::changeEvent(event);

  switch (event->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    Translations::translateSqlTableModel(model);
    Translations::translateSqlTableModel(secondModel);
    break;
  default:
    break;
  }
}
void NotamHistoryBrowser::closeEvent(QCloseEvent *event)
{
  saveState(staticMetaObject.className());
  AbstractBrowser::closeEvent(event);
}

// ----------------------------------------------
NotamHistoryBrowserHelper::NotamHistoryBrowserHelper(QObject* parent) :
  ObjectsManagerHelper(parent), mAction(0)
{
  mAction = new QAction(icon(), name(), this);
  mAction->setObjectName(id());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}

QString NotamHistoryBrowserHelper::id() const
{
  return NotamHistoryBrowser::staticMetaObject.className();
}

QString NotamHistoryBrowserHelper::path() const
{
  return tr("/History/NOTAM history browser");
}

QString NotamHistoryBrowserHelper::name() const
{
  return tr("NOTAM history browser");
}

QString NotamHistoryBrowserHelper::description() const
{
  return tr("History of NOTAM messages");
}

QWidget* NotamHistoryBrowserHelper::widget(QWidget* parent) const
{
  return new NotamHistoryBrowser(parent);
}

QAction *NotamHistoryBrowserHelper::action() const
{
  return mAction;
}

void NotamHistoryBrowserHelper::retranslateUi()
{
  mAction->setText(name());
  mAction->setIcon(icon());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}

