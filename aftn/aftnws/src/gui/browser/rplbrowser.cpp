#include "rplbrowser.h"
#include "fpl.h"
#include "aftnmessageprocessor.h"
#include "AFTNDao.h"

#include "qtsqltableview.h"
#include "qtheaderview.h"

#include "translations.h"

#include <QtDebug>

#include <QtGui/QMessageBox>

#include <QtSql/QSqlTableModel>

/** Constructor and destructor */
RplBrowser::RplBrowser(QWidget* parent) :
  AbstractBrowser(parent), rplEditor(NULL)
{
  setupUi(this);
  model = new QSqlTableModel(this);
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  model->setTable(TBL_RPL);
  QStringList systemColumns = QStringList()
  //<< "ID"
  ;
//  QStringList systemColumns = QStringList() << TBL_RPL_ID;

  foreach (const QString& column, systemColumns)
      model->removeColumn(model->fieldIndex(column), QModelIndex());

  table->horizontalHeader()->setSortFlag(QtHeaderView::SimpleSort);
  table->setModel(model);

  connect(table->selectionModel(),
    SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this,
    SLOT(onTableCurrentChanged(const QModelIndex&, const QModelIndex&)));
  connect(table, SIGNAL(doubleClicked(const QModelIndex&)), this,
    SLOT(onEditItem()));

  connect(filterEdit, SIGNAL(textChanged(QString)), this,
    SLOT(onTextFilterChanged(QString)));
  connect(createButton, SIGNAL(clicked()), this, SLOT(onCreateItem()));
  connect(cloneButton, SIGNAL(clicked()), this, SLOT(onCloneItem()));
  connect(editButton, SIGNAL(clicked()), this, SLOT(onEditItem()));
  connect(deleteButton, SIGNAL(clicked()), this, SLOT(onDeleteItem()));
  connect(filterButton, SIGNAL(clicked()), this, SLOT(updateModel()));

  updateModel();
  Translations::translateSqlTableModel( model);
  restoreState(staticMetaObject.className());
}

void RplBrowser::onTextFilterChanged(const QString&)
{
  if (filterButton->isChecked())
    updateModel();
}

void RplBrowser::onCreateItem()
{
  editor()->setRpl(RPL());
  editor()->exec();
  updateModel();
}

void RplBrowser::onCloneItem()
{
  QModelIndex currentIndex = table->selectionModel()->currentIndex();

  if (currentIndex.isValid()) {
    RPL rpl(model->record(currentIndex.row()));

    editor()->setRpl(rpl.clone());
    editor()->exec();
    updateModel();
  }
}

void RplBrowser::onEditItem()
{
  QModelIndex currentIndex = table->selectionModel()->currentIndex();

  if (currentIndex.isValid()) {
    RPL rpl(model->record(currentIndex.row()));
    editor()->setRpl(rpl);
    editor()->exec();
    updateModel();
  }
}

void RplBrowser::onDeleteItem()
{
  int row = table->currentIndex().row();
  if (row < 0)
    return;

  bool answerYes = QMessageBox::question(this, windowTitle(), tr(
    "Do you really want to delete RPL record?"), QMessageBox::Yes
      | QMessageBox::No) == QMessageBox::Yes;

  if (answerYes) {

    model->removeRow(row);
    if (!model->submitAll()) {
      QMessageBox::critical(this, windowTitle(), tr("Error when deleting FPL."));
      model->revertAll();
    }
  }
}

void RplBrowser::onTableCurrentChanged(const QModelIndex& cur,
  const QModelIndex&)
{
  bool enableActions = cur.isValid();
  cloneButton->setEnabled(enableActions);
  editButton->setEnabled(enableActions);
  deleteButton->setEnabled(enableActions);
}

/** Update objects model */
void RplBrowser::updateModel()
{
  QStringList filters;
  QString textFilter = filterEdit->text();

  if (filterButton->isChecked() && !textFilter.isEmpty()) {
    QString sqlLikePattern = QString(" LIKE '%%1%' ").arg(textFilter);

    filters.append("FIR_NAME" + sqlLikePattern + " OR " + "AIRCRAFT_ID"
        + sqlLikePattern + " OR " + "TAIRCRAFT" + sqlLikePattern + " OR "
        + "WTC" + sqlLikePattern + " OR " + "ADEP" + sqlLikePattern + " OR "
        + SQL_TIMESTAMP_TO_CHAR("TIME") + sqlLikePattern + " OR " + "SPEED"
        + sqlLikePattern + " OR " + "FLEVEL" + sqlLikePattern + " OR "
        + "ROUTE" + sqlLikePattern + " OR " + "ADES" + sqlLikePattern + " OR "
        + "EET" + sqlLikePattern + " OR " + "ALT1" + sqlLikePattern + " OR "
        + "ALT2" + sqlLikePattern + " OR " + "DAYS" + sqlLikePattern + " OR "
        + "OTHER" + sqlLikePattern);
  }

  QString filter = filters.join(" AND ");

  //    if (model->filter() != filter) {
  model->setFilter(filter);
  model->select();
  table->resizeColumnsToContents();
  onTableCurrentChanged(QModelIndex(), QModelIndex());
  //    }
}

RplEditor* RplBrowser::editor(void)
{
  if (!rplEditor)
    rplEditor.reset(new RplEditor(this));

  return rplEditor.data();
}

void RplBrowser::changeEvent(QEvent *event)
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
void RplBrowser::closeEvent(QCloseEvent *event)
{
  saveState(staticMetaObject.className());
  AbstractBrowser::closeEvent(event);
}

// ----------------------------------------------
RplBrowserHelper::RplBrowserHelper(QObject* parent) :
  ObjectsManagerHelper(parent), mAction(0)
{
  mAction = new QAction(icon(), name(), this);
  mAction->setObjectName(id());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}

QString RplBrowserHelper::id() const
{
  return RplBrowser::staticMetaObject.className();
}

QString RplBrowserHelper::path() const
{
  return tr("/View/RPL browser");
}

QString RplBrowserHelper::name() const
{
  return tr("RPL browser");
}

QString RplBrowserHelper::description() const
{
  return tr("Current RPLs");
}

QWidget* RplBrowserHelper::widget(QWidget* parent) const
{
  return new RplBrowser(parent);
}

QAction *RplBrowserHelper::action() const
{
  return mAction;
}

void RplBrowserHelper::retranslateUi()
{
  mAction->setText(name());
  mAction->setIcon(icon());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}
