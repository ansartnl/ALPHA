#include "globalhistorybrowser.h"
#include "aftnmessageprocessor.h"
#include "qtheaderview.h"
#include "translations.h"
#include <QtGui/QMessageBox>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

GlobalHistoryBrowser::GlobalHistoryBrowser(QWidget* parent) :
  AbstractBrowser(parent)

{
  setupUi(this);

  // create and initialize model
  model = new QSqlTableModel(this);
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  model->setTable(TBL_ARC_AFTN);
  QStringList systemColumns = QStringList() << "FIR" << "TRTIME"
      << "VISIBILITY";
//  QStringList systemColumns = QStringList() << TBL_ARC_AFTN_ID;

  foreach (const QString& column, systemColumns)
      model->removeColumn(model->fieldIndex(column), QModelIndex());

  table->horizontalHeader()->setSortFlag(QtHeaderView::SimpleSort);
  table->setModel(model);

  timeFilterAction->setShortcut(QString("Ctrl+T"));
  fromDateFilter->setDateTime(QDateTime::currentDateTime().toUTC());
  untilDateFilter->setDateTime(QDateTime::currentDateTime().toUTC());
  filterAction->setShortcut(QString("Ctrl+F"));

  connect(table->selectionModel(),
    SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this,
    SLOT(onTableCurrentChanged(const QModelIndex&, const QModelIndex&)));
  connect(timeFilterAction, SIGNAL(clicked()), this, SLOT(updateModel()));
  connect(fromDateFilter, SIGNAL(dateTimeChanged(QDateTime)), this,
    SLOT(onFromTimeFilterChanged(QDateTime)));
  connect(untilDateFilter, SIGNAL(dateTimeChanged(QDateTime)), this,
    SLOT(onUntilTimeFilterChanged(QDateTime)));
  connect(filterAction, SIGNAL(clicked()), this, SLOT(updateModel()));
  connect(filterEdit, SIGNAL(textChanged(QString)), this,
    SLOT(onTextFilterChanged(QString)));

  updateModel();
  Translations::translateSqlTableModel( model);

  restoreState(staticMetaObject.className());
}

void GlobalHistoryBrowser::updateModel()
{
  QString sqlFilter;

  if (filterAction->isChecked()) {
    sqlFilter = TBL_ARC_AFTN_TEXT " LIKE '%" + filterEdit->text() + "%'";

    if (timeFilterAction->isChecked())
      sqlFilter += " AND ";
  }

  if (timeFilterAction->isChecked())
    sqlFilter += TBL_ARC_AFTN_CREATED " <  timestamp'"
        + untilDateFilter->dateTime().toString("yyyy-MM-dd hh:mm:ss")
        + "' and  " TBL_ARC_AFTN_CREATED " > timestamp'"
        + fromDateFilter->dateTime().toString("yyyy-MM-dd hh:mm:ss") + "'";

  model->setFilter(sqlFilter);
  model->select();

  table->resizeColumnsToContents();
  onTableCurrentChanged(QModelIndex(), QModelIndex());

  QString sRecordAmount("SELECT count(*) FROM "TBL_ARC_AFTN);

  if (!sqlFilter.isEmpty())
    sRecordAmount += " WHERE " + sqlFilter;

  QSqlQuery query(sRecordAmount);

  if (query.exec()) {
    if (query.next())
      ;

    setStatus(QString(tr("Found %1 records")).arg(query.value(0).toString()));
  } else
    QMessageBox::critical(this, windowTitle(), tr("SQL execution error: ")
        + query.lastError().text());
}

void GlobalHistoryBrowser::onTableCurrentChanged(const QModelIndex& cur,
  const QModelIndex& prew)
{
  if (cur.isValid())
    description->setText(model->data(model->index(cur.row(), model->fieldIndex(
      TBL_ARC_AFTN_TEXT))).toString());
  else
    description->setText("");
}

void GlobalHistoryBrowser::onFromTimeFilterChanged(const QDateTime& dtFrom)
{
  if (dtFrom > untilDateFilter->dateTime()) {
    fromDateFilter->setDateTime(untilDateFilter->dateTime());
    return;
  }

  if (timeFilterAction->isChecked())
    updateModel();
}

void GlobalHistoryBrowser::onUntilTimeFilterChanged(const QDateTime& dtUntil)
{
  if (dtUntil < fromDateFilter->dateTime()) {
    untilDateFilter->setDateTime(fromDateFilter->dateTime());
    return;
  }

  if (timeFilterAction->isChecked())
    updateModel();
}

void GlobalHistoryBrowser::onTextFilterChanged(const QString&)
{
  if (filterAction->isChecked())
    updateModel();
}

void GlobalHistoryBrowser::setStatus(const QString& status)
{
  if (browserStatus != status) {
    browserStatus = status;
    setStatusTip(status);
    emit statusChanged(status);
  }
}

void GlobalHistoryBrowser::changeEvent(QEvent *event)
{
  QWidget::changeEvent(event);

  switch (event->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    Translations::translateSqlTableModel( model);
    break;
  default:
    break;
  }
}

void GlobalHistoryBrowser::closeEvent(QCloseEvent *event)
{
  saveState(staticMetaObject.className());
  AbstractBrowser::closeEvent(event);
}

// ---------------------------------------------------------------------
GlobalHistoryBrowserHelper::GlobalHistoryBrowserHelper(QObject* parent) :
  ObjectsManagerHelper(parent), mAction(0)
{
  mAction = new QAction(icon(), name(), this);
  mAction->setObjectName(id());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}

QString GlobalHistoryBrowserHelper::id() const
{
  return GlobalHistoryBrowser::staticMetaObject.className();
}

QString GlobalHistoryBrowserHelper::path() const
{
  return tr("/History/Global history browser");
}

QString GlobalHistoryBrowserHelper::name() const
{
  return tr("Global history browser");
}

QString GlobalHistoryBrowserHelper::description() const
{
  return tr("History of AFTN messages");
}

QWidget* GlobalHistoryBrowserHelper::widget(QWidget* parent) const
{
  return new GlobalHistoryBrowser(parent);
}

QAction *GlobalHistoryBrowserHelper::action() const
{
  return mAction;
}

void GlobalHistoryBrowserHelper::retranslateUi()
{
  mAction->setText(name());
  mAction->setIcon(icon());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}
