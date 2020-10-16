#include "fplonfuturebrowser.h"

#include "aftnmessageprocessor.h"
#include "fpl.h"

#include "qtsqltableview.h"
#include "qtheaderview.h"

#include "translations.h"

#include <QtDebug>

#include <QtGui/QMessageBox>

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlTableModel>

#define QS_LOWER(s) QString(s)).toLower()

namespace gui {
  FplOnFutureBrowser::FplOnFutureBrowser(QWidget* parent) :
    AbstractBrowser(parent), fplViewer(NULL)
  {
    setupUi(this);

    model = new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable(TBL_FPL_ON_FUTURE);
    QStringList systemColumns = QStringList() << "FIR" << "WTC" << "TRTIME"
        << "EXFPL" << "VISIBILITY";
//    QStringList systemColumns = QStringList() << TBL_FPL_ID << TBL_FPL_FIR_NAME
//        << TBL_FPL_TURBULENCE_CATEGORY << TBL_FPL_TRTIME
//        << TBL_FPL_EXFPL << TBL_FPL_VISIBILITY;
//    QStringList systemColumns = QStringList() << TBL_FPL_ID << TBL_FPL_TRTIME
//        << TBL_FPL_EXFPL << TBL_FPL_VISIBILITY;

    foreach (const QString& column, systemColumns)
        model->removeColumn(model->fieldIndex(column), QModelIndex());

    table->setModel(model);
    table->horizontalHeader()->setSortFlag(QtHeaderView::SimpleSort);

    QDateTime nowUTC = QDateTime::currentDateTime().toUTC();
    dateFilter->setDate(nowUTC.date().addDays(1));
    dateFilter->setMinimumDate(nowUTC.date().addDays(1));

    QSqlQuery query("delete from " TBL_FPL_ON_FUTURE); // clear temp table

    if (query.lastError().isValid())
      QMessageBox::information(this, windowTitle(), tr("SQL execution error: ")
          + query.lastError().text());

    filterAction->setShortcut(QString("Ctrl+F"));

    connect(table, SIGNAL(doubleClicked(QModelIndex)), this,
      SLOT(onDisplayItem(QModelIndex)));
    connect(dateFilter, SIGNAL(dateChanged(QDate)), this,
      SLOT(onDateFilterChanged(QDate)));
    connect(filterAction, SIGNAL(clicked()), this, SLOT(updateModel()));
    connect(filterEdit, SIGNAL(textChanged(QString)), this,
      SLOT(onTextFilterChanged(QString)));

    prepareTable(dateFilter->date());

    updateModel();
    Translations::translateSqlTableModel(model);
    restoreState(staticMetaObject.className());
  }

  void FplOnFutureBrowser::prepareTable(const QDate& date)
  {
    QSqlQuery query("truncate table " TBL_FPL_ON_FUTURE); // clear temp table

    if (query.lastError().isValid()) {
      QMessageBox::information(this, windowTitle(), tr("SQL execution error: ")
          + query.lastError().text());
      return;
    }

    // copy records from FPL, FPL почти никогда не трогаем
    query.prepare(
      "insert into "TBL_FPL_ON_FUTURE"("
      TBL_FPL_ID", "
      TBL_FPL_FIR_NAME", "
      TBL_FPL_AIRCRAFT_ID", "
      TBL_FPL_FRULES", "
      TBL_FPL_TFLIGHT", "
      TBL_FPL_NUM", "
      TBL_FPL_TAIRCRAFT", "
      TBL_FPL_TURBULENCE_CATEGORY", "
      TBL_FPL_EQUIPMENT", "
      TBL_FPL_ADEP", "
      TBL_FPL_TIME", "
      TBL_FPL_SPEED", "
      TBL_FPL_FLEVEL", "
      TBL_FPL_ROUTE", "
      TBL_FPL_ADES", "
      TBL_FPL_EET", "
      TBL_FPL_ALT1", "
      TBL_FPL_ALT2", "
      TBL_FPL_OTHER", "
      TBL_FPL_TRTIME", "
      TBL_FPL_EXFPL", "
      TBL_FPL_MESSAGE_ID", "
      TBL_FPL_ENTRY_FIR_EET" "
      ") select "
      TBL_FPL_ID", "
      TBL_FPL_FIR_NAME", "
      TBL_FPL_AIRCRAFT_ID", "
      TBL_FPL_FRULES", "
      TBL_FPL_TFLIGHT", "
      TBL_FPL_NUM", "
      TBL_FPL_TAIRCRAFT", "
      TBL_FPL_TURBULENCE_CATEGORY", "
      TBL_FPL_EQUIPMENT", "
      TBL_FPL_ADEP", "
      TBL_FPL_TIME", "
      TBL_FPL_SPEED", "
      TBL_FPL_FLEVEL", "
      TBL_FPL_ROUTE", "
      TBL_FPL_ADES", "
      TBL_FPL_EET", "
      TBL_FPL_ALT1", "
      TBL_FPL_ALT2", "
      TBL_FPL_OTHER", "
      TBL_FPL_TRTIME", "
      TBL_FPL_EXFPL", "
      TBL_FPL_MESSAGE_ID", "
      TBL_FPL_ENTRY_FIR_EET" "
      " from "TBL_FPL" where to_char("TBL_FPL_TIME", 'DD.MM.YYYY') = to_char(?, 'DD.MM.YYYY')");
    query.addBindValue(date);

    if (!query.exec()) {
      qWarning() << Q_FUNC_INFO;
      qWarning() << query.lastQuery();
      qWarning() << query.lastError().text();
      QMessageBox::information(this, windowTitle(), tr(
        "SQL execution error: %1").arg(query.lastError().text()));
      return;
    }

    // copy records from FFPL
    query.prepare(
      "insert into "
      TBL_FPL_ON_FUTURE
      "( "
      TBL_FPL_ID", "
      TBL_FPL_FIR_NAME", "
      TBL_FPL_AIRCRAFT_ID", "
      TBL_FPL_FRULES", "
      TBL_FPL_TFLIGHT", "
      TBL_FPL_NUM", "
      TBL_FPL_TAIRCRAFT", "
      TBL_FPL_TURBULENCE_CATEGORY", "
      TBL_FPL_EQUIPMENT", "
      TBL_FPL_ADEP", "
      TBL_FPL_TIME", "
      TBL_FPL_SPEED", "
      TBL_FPL_FLEVEL", "
      TBL_FPL_ROUTE", "
      TBL_FPL_ADES", "
      TBL_FPL_EET", "
      TBL_FPL_ALT1", "
      TBL_FPL_ALT2", "
      TBL_FPL_OTHER", "
      TBL_FPL_TRTIME", "
      TBL_FPL_EXFPL", "
      TBL_FPL_ENTRY_FIR_EET" "
      ")"
      " select "
      TBL_FPL_ID", "
      TBL_FPL_FIR_NAME", "
      TBL_FPL_AIRCRAFT_ID", "
      TBL_FPL_FRULES", "
      TBL_FPL_TFLIGHT", "
      TBL_FPL_NUM", "
      TBL_FPL_TAIRCRAFT", "
      TBL_FPL_TURBULENCE_CATEGORY", "
      TBL_FPL_EQUIPMENT", "
      TBL_FPL_ADEP", "
      TBL_FPL_TIME", "
      TBL_FPL_SPEED", "
      TBL_FPL_FLEVEL", "
      TBL_FPL_ROUTE", "
      TBL_FPL_ADES", "
      TBL_FPL_EET", "
      TBL_FPL_ALT1", "
      TBL_FPL_ALT2", "
      TBL_FPL_OTHER", "
      TBL_FPL_TRTIME", "
      TBL_FPL_EXFPL", "
      TBL_FPL_ENTRY_FIR_EET" "
      " from "TBL_FFPL" where to_char("TBL_FPL_TIME", 'DD.MM.YYYY') = to_char(?, 'DD.MM.YYYY')"
      " and not exists"
      " (select * from "TBL_FPL_ON_FUTURE
      " where "TBL_FPL_AIRCRAFT_ID" = "TBL_FFPL"."TBL_FPL_AIRCRAFT_ID
      " and "TBL_FPL_ADEP" = "TBL_FFPL"."TBL_FPL_ADEP
      " and "TBL_FPL_ADES" = "TBL_FFPL"."TBL_FPL_ADES")");
    query.addBindValue(date);

    if (!query.exec()) {
      qWarning() << Q_FUNC_INFO;
      qWarning() << query.lastQuery();
      qWarning() << query.lastError().text();
      QMessageBox::information(this, windowTitle(), tr("SQL execution error: ")
          + query.lastError().text());
      return;
    }

    //converting RPL to FPL
    static const char* daysPattern = "1234567";
    QString rplDays("_______");
    int dayOfWeekIndex = date.dayOfWeek() - 1;
    rplDays[dayOfWeekIndex] = QChar(daysPattern[dayOfWeekIndex]);
    query.prepare("SELECT "
    TBL_RPL_ID ", "
    TBL_RPL_FIR_NAME ", "
    TBL_RPL_AIRCRAFT_ID ", "
    TBL_RPL_FRULES ", "
    TBL_RPL_TFLIGHT ", "
    TBL_RPL_NUM ", "
    TBL_RPL_TAIRCRAFT ", "
    TBL_RPL_TURBULENCE_CATEGORY ", "
    TBL_RPL_EQUIPMENT ", "
    TBL_RPL_ADEP ", "
    TBL_RPL_TIME ", "
    TBL_RPL_SPEED ", "
    TBL_RPL_FLEVEL ", "
    TBL_RPL_ROUTE ", "
    TBL_RPL_ADES ", "
    TBL_RPL_EET ", "
    TBL_RPL_ALT1 ", "
    TBL_RPL_ALT2 ", "
    TBL_RPL_OTHER ", "
    TBL_RPL_TRTIME ", "
    TBL_FPL_ENTRY_FIR_EET" "
    " FROM "
    TBL_RPL
    " WHERE "
    // Если RPL валиден
      " (SYS_EXTRACT_UTC(SYSTIMESTAMP) BETWEEN "TBL_RPL_VALID_FROM" AND "TBL_RPL_VALID_TO")"
      " AND "
      TBL_RPL_DAYS " LIKE ? AND"
      " not exists ("
      " select * from "TBL_FPL_ON_FUTURE
      " where "TBL_FPL_ON_FUTURE"."TBL_FPL_AIRCRAFT_ID" = "TBL_RPL"."TBL_FPL_AIRCRAFT_ID
      " and "TBL_FPL_ON_FUTURE"."TBL_FPL_ADEP" = "TBL_RPL"."TBL_FPL_ADEP
      " and "TBL_FPL_ON_FUTURE"."TBL_FPL_ADES" = "TBL_RPL"."TBL_FPL_ADES")");
    query.addBindValue(rplDays);

    if (!query.exec()) {
      qWarning() << Q_FUNC_INFO;
      qWarning() << query.lastQuery();
      qWarning() << query.lastError().text();
      QMessageBox::information(this, windowTitle(), tr("SQL execution error: ")
          + query.lastError().text());
      return;
    }

    while (query.next()) {
      QSqlRecord record = query.record();

      QDateTime time = record.value(TBL_RPL_TIME).toDateTime();
      time.setDate(date);
      record.setValue(TBL_RPL_TIME, time);

      QString sqlStatement = QSqlDatabase::database().driver()->sqlStatement(
        QSqlDriver::InsertStatement, TBL_FPL_ON_FUTURE, record, false);

      QSqlQuery queryInsert(sqlStatement);

      if (queryInsert.lastError().isValid()) {
        qWarning() << Q_FUNC_INFO;
        qWarning() << queryInsert.lastQuery();
        qWarning() << queryInsert.lastError().text();
        QMessageBox::information(this, windowTitle(), tr(
          "SQL execution error: ") + queryInsert.lastError().text());
        return;
      }
    }
  }

  void FplOnFutureBrowser::updateModel()
  {
    setWindowTitle(tr("FPL List on ")
        + dateFilter->date().toString(DATE_FORMAT));

    QString sqlFilter;

    if (filterAction->isChecked()) {
      QString sqlLikePattern;
      QString sqlLikeOrPattern;

      // LIKE '%filter%'
      sqlLikePattern.append(" LIKE '%");
      sqlLikePattern.append(filterEdit->text());
      sqlLikePattern.append("%' ");

      sqlLikeOrPattern = sqlLikePattern;
      sqlLikeOrPattern.append(" OR ");

      sqlFilter = TBL_FPL_FIR_NAME + sqlLikeOrPattern +
        TBL_FPL_AIRCRAFT_ID + sqlLikeOrPattern + TBL_FPL_FRULES
          + sqlLikeOrPattern + TBL_FPL_TFLIGHT + sqlLikeOrPattern +
        TBL_FPL_NUM + sqlLikeOrPattern + TBL_FPL_TAIRCRAFT
          + sqlLikeOrPattern + TBL_FPL_TURBULENCE_CATEGORY + sqlLikeOrPattern +
        TBL_FPL_EQUIPMENT + sqlLikeOrPattern + TBL_FPL_ADEP
          + sqlLikeOrPattern + SQL_TIMESTAMP_TO_CHAR(TBL_FPL_TIME)
          + sqlLikeOrPattern + TBL_FPL_SPEED + sqlLikeOrPattern +
        TBL_FPL_FLEVEL + sqlLikeOrPattern + TBL_FPL_ROUTE
          + sqlLikeOrPattern + TBL_FPL_ADES + sqlLikeOrPattern
          + TBL_FPL_EET + sqlLikeOrPattern +
        TBL_FPL_ALT1 + sqlLikeOrPattern + TBL_FPL_ALT2
          + sqlLikeOrPattern + TBL_FPL_ID + sqlLikeOrPattern +
        TBL_FPL_OTHER + sqlLikePattern;
    }

    model->setFilter(sqlFilter);
    model->select();
    table->resizeColumnsToContents();
  }

  void FplOnFutureBrowser::onDateFilterChanged(const QDate& date)
  {
    prepareTable(date);
    setWindowTitle(tr("FPL List on ") + date.toString(DATE_FORMAT));
    updateModel();
  }

  void FplOnFutureBrowser::onTextFilterChanged(const QString&)
  {
    if (filterAction->isChecked())
      updateModel();
  }

  FplViewer* FplOnFutureBrowser::viewer()
  {
    if (!fplViewer)
      fplViewer.reset(new FplViewer(this));

    return fplViewer.data();
  }


  void FplOnFutureBrowser::onDisplayItem(const QModelIndex& index)
  {
    if (index.isValid()) {
      const QSqlTableModel* currentModel =
          qobject_cast<const QSqlTableModel*> (index.model());

      if (!currentModel)
        return;

      FPL Fpl(currentModel->record(index.row()), TBL_FPL_ON_FUTURE);
      viewer()->setFPL(Fpl);
      viewer()->exec();
    }
  }

  void FplOnFutureBrowser::changeEvent(QEvent* event)
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

  void FplOnFutureBrowser::closeEvent(QCloseEvent* event)
  {
    saveState(staticMetaObject.className());
    AbstractBrowser::closeEvent(event);
  }
}


// -------------------------------------------
FplOnFutureBrowserHelper::FplOnFutureBrowserHelper(QObject* parent) :
  ObjectsManagerHelper(parent), mAction(0)
{
  mAction = new QAction(icon(), name(), this);
  mAction->setObjectName(id());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}

QString FplOnFutureBrowserHelper::id() const
{
  return gui::FplOnFutureBrowser::staticMetaObject.className();
}

QString FplOnFutureBrowserHelper::path() const
{
  return tr("/View/FPL on future browser");
}

QString FplOnFutureBrowserHelper::name() const
{
  return tr("FPL on future browser");
}

QString FplOnFutureBrowserHelper::description() const
{
  return tr("FPL on some date");
}

QWidget* FplOnFutureBrowserHelper::widget(QWidget* parent) const
{
  return new gui::FplOnFutureBrowser(parent);
}

QAction* FplOnFutureBrowserHelper::action() const
{
  return mAction;
}

void FplOnFutureBrowserHelper::retranslateUi()
{
  mAction->setText(name());
  mAction->setIcon(icon());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}
