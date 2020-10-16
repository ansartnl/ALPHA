#include "notambrowser.h"
#include "notamviewer.h"
#include "notam.h"
#include "aftnmessageprocessor.h"
#include "AFTNDao.h"

#include "translations.h"

#include "qtheaderview.h"

#include <QtCore/QVariant>

#include <QtGui/QMessageBox>

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#define QS_LOWER(s) QString(s).toLower()

/** Constructor and destructor */
NotamBrowser::NotamBrowser(QWidget* parent) :
  AbstractBrowser(parent), notamViewer(NULL)
{
  setupUi(this);

  // create and initialize model
  model = new QSqlTableModel(this);
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  model->setTable(TBL_NOTAM);
  QStringList systemColumns = QStringList()
  //<< "ID"
      << "VALID_UNTIL_T" << "SEQUENCE_ID" << "STATE";
//  QStringList systemColumns = QStringList() << TBL_NOTAM_ID
//      << TBL_NOTAM_VALID_UNTIL_T << TBL_NOTAM_SEQUENCE_ID << TBL_NOTAM_STATE;

  foreach (const QString& column, systemColumns)
      model->removeColumn(model->fieldIndex(column), QModelIndex());

  // create table view
  table->setModel(model);
  table->horizontalHeader()->setSortFlag(QtHeaderView::SimpleSort);

  //    fromDateFilter->setDisplayFormat(DATETIME_FORMAT);
  fromDateFilter->setDateTime(QDateTime::currentDateTime().toUTC());

  //    untilDateFilter->setDisplayFormat(DATETIME_FORMAT);
  toDateFilter->setDateTime(QDateTime::currentDateTime().toUTC());

  connect(table->selectionModel(),
    SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
    SLOT(onTableCurrentChanged(const QModelIndex&, const QModelIndex&)));
  connect(table, SIGNAL(doubleClicked(const QModelIndex&)), this,
    SLOT(onDisplayItem(const QModelIndex&)));
  connect(fromDateFilter, SIGNAL(dateTimeChanged(QDateTime)), this,
    SLOT(onFromTimeFilterChanged(QDateTime)));
  connect(toDateFilter, SIGNAL(dateTimeChanged(QDateTime)), this,
    SLOT(onUntilTimeFilterChanged(QDateTime)));

  natureFilterModel = new QSqlQueryModel(natureFilter);
  natureFilterModel->setQuery(
    "SELECT DISTINCT " TBL_NOTAM_NATURE " FROM " TBL_NOTAM " UNION SELECT NULL FROM DUAL ORDER BY 1 NULLS FIRST");
  natureFilter->setModel(natureFilterModel);
  natureFilter->setCurrentIndex(-1);

  firFilterModel = new QSqlQueryModel(firFilter);
  firFilterModel->setQuery(
    "SELECT DISTINCT " TBL_NOTAM_FIR " FROM " TBL_NOTAM " UNION SELECT NULL FROM DUAL ORDER BY 1 NULLS FIRST");
  firFilter->setModel(firFilterModel);
  firFilter->setCurrentIndex(-1);

  connect(firFilter, SIGNAL(currentIndexChanged(QString)), this,
    SLOT(updateModel()));
  connect(natureFilter, SIGNAL(currentIndexChanged(QString)), this,
    SLOT(updateModel()));
  connect(filterEdit, SIGNAL(textChanged(QString)), this,
    SLOT(onTextFilterChanged(QString)));
  connect(filterAction, SIGNAL(clicked()), this, SLOT(updateModel()));

  updateModel();

  Translations::translateSqlTableModel(model);
  restoreState(staticMetaObject.className());
}


void NotamBrowser::onFromTimeFilterChanged(const QDateTime& from)
{
  if (from > toDateFilter->dateTime()) {
    fromDateFilter->setDateTime(toDateFilter->dateTime());
    return;
  }

  if (applyTimeFilter->isChecked())
    updateModel();
}

void NotamBrowser::onUntilTimeFilterChanged(const QDateTime& until)
{
  if (until < fromDateFilter->dateTime())
    toDateFilter->setDateTime(fromDateFilter->dateTime());
  else if (applyTimeFilter->isChecked())
    updateModel();
}

void NotamBrowser::onTextFilterChanged(const QString&)
{
  if (filterAction->isChecked())
    updateModel();
}

void NotamBrowser::onTableCurrentChanged(const QModelIndex& index,
  const QModelIndex&)
{
  description->setText(index.sibling(index.row(), model->fieldIndex(
    TBL_NOTAM_DESCRIPTION)).data().toString());
}

void NotamBrowser::onDisplayItem(const QModelIndex& index)
{
  if (index.isValid()) {
    Notam notam(model->record(index.row()));
    viewer()->setNotam(notam);
    viewer()->exec();
  }
}


/** Update objects model */
void NotamBrowser::updateModel()
{
  QStringList filters;
  filters.append("(" TBL_NOTAM_STATE " is null " ")");
  filters.append("(" TBL_NOTAM_VALID_FROM " <  timestamp'"
      + QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss")
      + "' and ( " TBL_NOTAM_VALID_UNTIL " > timestamp'"
      + QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss")
      + "' or " TBL_NOTAM_VALID_UNTIL " is null))");

  QString
      sqlFilter(
        "(" TBL_NOTAM_VALID_FROM " <  timestamp'"
            + QDateTime::currentDateTime().toUTC().toString(
              "yyyy-MM-dd hh:mm:ss")
            + "' and ( " TBL_NOTAM_VALID_UNTIL " > timestamp'"
            + QDateTime::currentDateTime().toUTC().toString(
              "yyyy-MM-dd hh:mm:ss")
            + "' or " TBL_NOTAM_VALID_UNTIL " is null)) and " TBL_NOTAM_STATE " is null");
  QString textFilter = filterEdit->text();
  QString sqlFirFilter(firFilter->currentText());
  QString sqlNatureFilter(natureFilter->currentText());
  QString sqlLikeFilter;

  if (filterAction->isChecked() && !textFilter.isEmpty()) {
    QString sqlLikePattern;
    QString sqlLikeOrPattern;

    // LIKE '%filter%'
    sqlLikePattern.append(" LIKE '%");
    sqlLikePattern.append(textFilter);
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
    sqlFilter += " AND " TBL_NOTAM_FIR " = '" + sqlFirFilter + "'";

  if (!sqlNatureFilter.isEmpty())
    sqlFilter += " AND " TBL_NOTAM_NATURE " = '" + sqlNatureFilter + "'";

  if (!sqlLikeFilter.isEmpty())
    sqlFilter += " AND " + sqlLikeFilter;

  // only active notam
  sqlFilter.append(" and (state is null)");

  model->setFilter(sqlFilter);
  model->select();

  table->resizeColumnsToContents();

  onTableCurrentChanged(QModelIndex(), QModelIndex());
}


NotamViewer* NotamBrowser::viewer()
{
  if (!notamViewer)
    notamViewer.reset(new NotamViewer(this));

  return notamViewer.data();
}


void NotamBrowser::changeEvent(QEvent* event)
{
  QWidget::changeEvent(event);

  switch (event->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    Translations::translateSqlTableModel(model);
    break;
  default:
    break;
  }
}
void NotamBrowser::closeEvent(QCloseEvent* event)
{
  saveState(staticMetaObject.className());
  AbstractBrowser::closeEvent(event);
}

// --------------------------------------------------------------------
NotamBrowserHelper::NotamBrowserHelper(QObject* parent) :
  ObjectsManagerHelper(parent), mAction(0)
{
  mAction = new QAction(icon(), name(), this);
  mAction->setObjectName(id());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}

QString NotamBrowserHelper::id() const
{
  return NotamBrowser::staticMetaObject.className();
}

QString NotamBrowserHelper::path() const
{
  return tr("/View/NOTAM browser");
}

QString NotamBrowserHelper::name() const
{
  return tr("NOTAM browser");
}

QString NotamBrowserHelper::description() const
{
  return tr("Current NOTAM messages");
}

QWidget* NotamBrowserHelper::widget(QWidget* parent) const
{
  return new NotamBrowser(parent);
}

QAction* NotamBrowserHelper::action() const
{
  return mAction;
}

void NotamBrowserHelper::retranslateUi()
{
  mAction->setText(name());
  mAction->setIcon(icon());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}
