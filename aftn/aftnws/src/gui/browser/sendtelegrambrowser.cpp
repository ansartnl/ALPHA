#include "sendtelegrambrowser.h"

#include "qtheaderview.h"
#include "translations.h"

#include <QSqlTableModel>

SendTelegramBrowser::SendTelegramBrowser(QWidget *parent) :
    AbstractBrowser(parent)
{
    setupUi(this);

    // create and initialize model
    model = new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("SENT_AFTN");

    table->horizontalHeader()->setSortFlag(QtHeaderView::SimpleSort);
    table->setModel(model);

    connect(table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,
            QModelIndex)), this,
      SLOT(onTableCurrentChanged(QModelIndex, QModelIndex)));
    connect(timeFilterAction, SIGNAL(clicked()), this, SLOT(updateModel()));
    connect(fromDateFilter, SIGNAL(dateTimeChanged(QDateTime)), this,
      SLOT(onFromTimeFilterChanged(QDateTime)));
    connect(untilDateFilter, SIGNAL(dateTimeChanged(QDateTime)), this,
      SLOT(onUntilTimeFilterChanged(QDateTime)));
    connect(filterAction, SIGNAL(clicked()), this, SLOT(updateModel()));
    connect(filterEdit, SIGNAL(textChanged(QString)), this,
      SLOT(onTextFilterChanged(QString)));

    updateModel();
    Translations::translateSqlTableModel(model);

    restoreState(staticMetaObject.className());
}

void SendTelegramBrowser::changeEvent(QEvent *event)
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

void SendTelegramBrowser::closeEvent(QCloseEvent *event)
{
    saveState(staticMetaObject.className());
    AbstractBrowser::closeEvent(event);
}

void SendTelegramBrowser::updateModel()
{
    QString sqlFilter;

    if (filterAction->isChecked()) {
      sqlFilter = "(text LIKE '%" + filterEdit->text() + "%'";
      sqlFilter += " OR header LIKE '%" + filterEdit->text() + "%'";
      sqlFilter += " OR message LIKE '%" + filterEdit->text() + "%'";
      sqlFilter += " OR addresses LIKE '%" + filterEdit->text() + "%')";

      if (timeFilterAction->isChecked())
          sqlFilter += " AND ";
    }

    if (timeFilterAction->isChecked())
    {
      sqlFilter += "created <  timestamp'"
          + untilDateFilter->dateTime().toString("yyyy-MM-dd hh:mm:ss")
          + "' and  created > timestamp'"
          + fromDateFilter->dateTime().toString("yyyy-MM-dd hh:mm:ss") + "'";
    }

    model->setFilter(sqlFilter);
    model->select();
    onTableCurrentChanged(QModelIndex(), QModelIndex());
}

void SendTelegramBrowser::onTableCurrentChanged(const QModelIndex &cur, const QModelIndex &)
{
    if (cur.isValid())
    {
      QString message = model->data(model->index(cur.row(), model->fieldIndex(
                                                     "message"))).toString();
      QString text = model->data(model->index(cur.row(), model->fieldIndex(
                                                  "text"))).toString();
      QString header = model->data(model->index(cur.row(), model->fieldIndex(
                                                    "header"))).toString();
      QString address = model->data(model->index(cur.row(), model->fieldIndex(
                                                     "addresses"))).toString();
      if (message.isEmpty())
      {
          message = text;
          if (!header.isEmpty())
              message.prepend(header + "\n\n-\n\n");
          else
              message.prepend(address + "\n\n-\n\n");
      }
      description->setText(message);
    } else
    {
      description->setText("");
    }
}

void SendTelegramBrowser::onFromTimeFilterChanged(const QDateTime &dtFrom)
{
    if (dtFrom > untilDateFilter->dateTime()) {
      fromDateFilter->setDateTime(untilDateFilter->dateTime());
      return;
    }

    if (timeFilterAction->isChecked())
      updateModel();
}

void SendTelegramBrowser::onUntilTimeFilterChanged(const QDateTime &dtUntil)
{
    if (dtUntil < fromDateFilter->dateTime()) {
      untilDateFilter->setDateTime(fromDateFilter->dateTime());
      return;
    }

    if (timeFilterAction->isChecked())
      updateModel();
}

void SendTelegramBrowser::onTextFilterChanged(const QString &)
{
    if (filterAction->isChecked())
      updateModel();
}

// ---------------------------------------------------------------------
SendTelegramBrowserHelper::SendTelegramBrowserHelper(QObject *parent) :
    ObjectsManagerHelper(parent), mAction(0)
{
    mAction = new QAction(icon(), name(), this);
    mAction->setObjectName(id());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
}

QString SendTelegramBrowserHelper::id() const
{
    return SendTelegramBrowser::staticMetaObject.className();
}

QString SendTelegramBrowserHelper::path() const
{
    return tr("/Telegrams/Sent");
}

QString SendTelegramBrowserHelper::name() const
{
    return tr("Sent telegrams browser");
}

QString SendTelegramBrowserHelper::description() const
{
    return tr("Sent telegrams");
}

QWidget *SendTelegramBrowserHelper::widget(QWidget *parent) const
{
    return new SendTelegramBrowser(parent);
}

QAction *SendTelegramBrowserHelper::action() const
{
    return mAction;
}

void SendTelegramBrowserHelper::retranslateUi()
{
    mAction->setText(name());
    mAction->setIcon(icon());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
}
