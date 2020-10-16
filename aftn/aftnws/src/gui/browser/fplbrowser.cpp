#include "fplbrowser.h"

#include "fpl.h"
#include "aftnmessageprocessor.h"
#include "AFTNDao.h"

#include "qtsqltableview.h"
#include "qtheaderview.h"

#include "translations.h"

#include <QWidget>
#include <QMessageBox>

#include <QtSql/QSqlTableModel>

/** Constructor and destructor */
FplBrowser::FplBrowser(QWidget* pParent) :
    AbstractBrowser(pParent), fplEditor(NULL), canOtherInfoEmpty(false)
{

  setupUi(this);

  // create and initialize model
  model = new QSqlTableModel(this);
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  model->setTable(TBL_FPL);

  QStringList systemColumns = QStringList() << "TRTIME"
      << "EXFPL" << "MESSAGE_ID" << "VISIBILITY";
//  QStringList systemColumns = QStringList() << TBL_FPL_ID << TBL_FPL_TRTIME
//      << TBL_FPL_EXFPL << TBL_FPL_MESSAGE_ID << TBL_FPL_VISIBILITY;

  foreach (const QString& column, systemColumns)
      model->removeColumn(model->fieldIndex(column), QModelIndex());

  table->horizontalHeader()->setSortFlag(QtHeaderView::SimpleSort);
  table->setModel(model);

  connect(table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,
          QModelIndex)), this,
    SLOT(onTableCurrentChanged(QModelIndex, QModelIndex)));
  connect(table, SIGNAL(doubleClicked(const QModelIndex&)), this,
    SLOT(onEditItem()));

  connect(editButton, SIGNAL(clicked()), this, SLOT(onEditItem()));
  connect(createButton, SIGNAL(clicked()), this, SLOT(onCreateItem()));
  connect(cloneButton, SIGNAL(clicked()), this, SLOT(onCloneItem()));
  connect(deleteButton, SIGNAL(clicked()), this, SLOT(onDeleteItem()));
  connect(filterButton, SIGNAL(clicked()), this, SLOT(updateModel()));
  connect(filterEdit, SIGNAL(textChanged(QString)), this, SLOT(
      onTextFilterChanged(QString)));

  updateModel();
  Translations::translateSqlTableModel(model);

  restoreState(staticMetaObject.className());
}


void FplBrowser::onTextFilterChanged(const QString&)
{
  if (filterButton->isChecked())
    updateModel();
}

void FplBrowser::onCreateItem()
{
  editor()->setFpl(FPL());
  editor()->exec();
  updateModel();
}

void FplBrowser::onCloneItem()
{
  int row = table->currentIndex().row();

  if (row != -1) {
    FPL Fpl(model->record(row));

    editor()->setFpl(Fpl.clone());
    editor()->exec();
    updateModel();
  }
}

void FplBrowser::onEditItem()
{
  int row = table->currentIndex().row();

  if (row != -1) {
    FPL Fpl(model->record(row));
    editor()->setFpl(Fpl);
    editor()->exec();
    updateModel();
  }
}

void FplBrowser::onDeleteItem()
{
  int row = table->currentIndex().row();

  if (row == -1)
    return;

  int answerYes = QMessageBox::question(this, windowTitle(), tr(
    "Do you really want to delete FPL record?"), QMessageBox::Yes
      | QMessageBox::No) == QMessageBox::Yes;

  if (answerYes) {
    model->removeRow(row);

    if (!model->submitAll()) {
      QMessageBox::critical(this, windowTitle(), tr("Error when deleting FPL."));
      model->revertAll();
    }
  }
}

/** Update objects model */
void FplBrowser::updateModel()
{
  QString sqlFilter;
  QString textFilter = filterEdit->text();

  if (filterButton->isChecked() && !textFilter.isEmpty()) {
    QString sqlLikePattern;
    QString sqlLikeOrPattern;

    // LIKE '%filter%'
    sqlLikePattern.append(" LIKE '%");
    sqlLikePattern.append(textFilter);
    sqlLikePattern.append("%' ");

    sqlLikeOrPattern = sqlLikePattern;
    sqlLikeOrPattern.append(" OR ");

    sqlFilter = TBL_FPL_FIR_NAME + sqlLikeOrPattern + TBL_FPL_AIRCRAFT_ID
        + sqlLikeOrPattern + TBL_FPL_FRULES + sqlLikeOrPattern
        + TBL_FPL_TFLIGHT + sqlLikeOrPattern + SQL_NUM_TO_CHAR(TBL_FPL_NUM) + sqlLikeOrPattern
        + TBL_FPL_TAIRCRAFT + sqlLikeOrPattern + TBL_FPL_TURBULENCE_CATEGORY
        + sqlLikeOrPattern + TBL_FPL_EQUIPMENT + sqlLikeOrPattern
        + TBL_FPL_ADEP + sqlLikeOrPattern + SQL_TIMESTAMP_TO_CHAR(
        TBL_FPL_TIME) + sqlLikeOrPattern + TBL_FPL_SPEED + sqlLikeOrPattern
        + TBL_FPL_FLEVEL + sqlLikeOrPattern + TBL_FPL_ROUTE + sqlLikeOrPattern
        + TBL_FPL_ADES + sqlLikeOrPattern + TBL_FPL_EET + sqlLikeOrPattern
        + TBL_FPL_ALT1 + sqlLikeOrPattern + TBL_FPL_ALT2 + sqlLikeOrPattern
        + SQL_NUM_TO_CHAR(TBL_FPL_ID) + sqlLikeOrPattern + TBL_FPL_OTHER + sqlLikePattern;
  }

  model->setFilter(sqlFilter);
  model->select();
  onTableCurrentChanged(QModelIndex(), QModelIndex());
}

void FplBrowser::onTableCurrentChanged(const QModelIndex& current,
  const QModelIndex&)
{
  bool enableActions = current.isValid();
  cloneButton->setEnabled(enableActions);
  editButton->setEnabled(enableActions);
  deleteButton->setEnabled(enableActions);

  QString  sId;
  if(current.isValid())
  {
      QVariant vId = model->data(model->index(current.row(), 0));
      sId = vId.toString();
  }
  emit selectionItemChanged(sId);
}

FplEditor* FplBrowser::editor()
{
  if (!fplEditor)
  {
    fplEditor.reset(new FplEditor(this));
    fplEditor->setCanOtherInfoEmpty(canOtherInfoEmpty);
  }

  return fplEditor.data();
}

void FplBrowser::changeEvent(QEvent* event)
{
  AbstractBrowser::changeEvent(event);

  switch (event->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    Translations::translateSqlTableModel(model);
    break;
  default:
    break;
  }
}

void FplBrowser::closeEvent(QCloseEvent *event)
{
  saveState(staticMetaObject.className());
  AbstractBrowser::closeEvent(event);
}

// Id of selected object
QString FplBrowser::SelectedObject(void)
{
    QModelIndexList qIndexes = table->selectionModel()->selectedIndexes();
    if(qIndexes.isEmpty()) return QString();

    QVariant vId = model->data(model->index(qIndexes[0].row(), 0));
    QString  sId = vId.toString();
    return sId;
}

void FplBrowser::SetCurSelByID(const QString& sID)
{
    for(int i = 0; i < model->rowCount(); ++i)
    {
        if(model->data(model->index(i, 0)).toString() == sID)
        {
            table->selectRow(i);
            break;
        }
    }
}

void FplBrowser::setCanOtherInfoEmpty(bool bCan)
{
    canOtherInfoEmpty = bCan;
}


// ------------------------------------------------------
#ifdef AFTNWS_PRJ_APP

FplBrowserHelper::FplBrowserHelper(QObject* parent) :
  ObjectsManagerHelper(parent), mAction(0)
{
  mAction = new QAction(icon(), name(), this);
  mAction->setObjectName(id());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}

QString FplBrowserHelper::id() const
{
  return FplBrowser::staticMetaObject.className();
}

QString FplBrowserHelper::path() const
{
  return tr("/View/FPL browser");
}

QString FplBrowserHelper::name() const
{
  return tr("FPL browser");
}

QString FplBrowserHelper::description() const
{
  return tr("Current FPL messages");
}

QWidget* FplBrowserHelper::widget(QWidget* parent) const
{
  return new FplBrowser(parent);
}

QAction *FplBrowserHelper::action() const
{
  return mAction;
}

void FplBrowserHelper::retranslateUi()
{
  mAction->setText(name());
  mAction->setIcon(icon());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}

#endif
