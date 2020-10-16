#include "useractionsbrowser.h"
#include "translations.h"

#include <QtDebug>

#include <QtCore/QStringList>

#include <QtSql/QSqlQueryModel>

#include "ociquerymodel.h"

UserActionsBrowser::UserActionsBrowser(QWidget *parent) :
  AbstractBrowser(parent), Ui::UserActionsBrowserClass()
{
  setupUi(this);

  users = new QSqlQueryModel(this);
  users->setQuery("select NULL user_name, NULL id from dual"
    " union select user_name, id from users "
    "   order by 1 nulls first");
  userBox->setModel(users);

  types = new QSqlQueryModel(this);
  types->setQuery("select NULL type from dual"
    " union select type from modification_types"
    "   order by 1 nulls first");
  typeBox->setModel(types);

  fpls = new OciQueryModel(this);
  fpls->setQuery("select * from hist_fpl");
  fplView->setModel(fpls);
  rpls = new OciQueryModel(this);
  rpls->setQuery("select * from hist_rpl");
  rplView->setModel(rpls);
  mans = new OciQueryModel(this);
  mans->setQuery("select * from hist_man_aftn");
  manView->setModel(mans);
  telegerams = new OciQueryModel(this);
  telegerams->setQuery("select * from hist_outputtelegrams");
  telegramView->setModel(telegerams);

  // Load only headers
  fpls->setFilter("0 != 0");
  rpls->setFilter("0 != 0");
  mans->setFilter("0 != 0");
  telegerams->setFilter("0 != 0");
  fpls->refresh();
  rpls->refresh();
  mans->refresh();
  telegerams->refresh();

  // add relations
  fpls->addRelation("modified_by", "USERS", "ID", "USER_NAME");
  rpls->addRelation("modified_by", "USERS", "ID", "USER_NAME");
  mans->addRelation("modified_by", "USERS", "ID", "USER_NAME");
  telegerams->addRelation("modified_by", "USERS", "ID", "USER_NAME");

  start->setDateTime(QDateTime::currentDateTime().toUTC().addDays(-30));
  end->setDateTime(QDateTime::currentDateTime().toUTC());

  Translations::translateOciSqlModel(fpls);
  Translations::translateOciSqlModel(rpls);
  Translations::translateOciSqlModel(mans);
  Translations::translateOciSqlModel(telegerams);

  connect(filterButton, SIGNAL(clicked()), this, SLOT(updateModel()));
  connect(between, SIGNAL(clicked()), this, SLOT(updateModel()));
  connect(userBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateModel()));
  connect(typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateModel()));
  connect(start, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(updateModel()));
  connect(end, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(updateModel()));

  restoreState(staticMetaObject.className());

#if defined(MICROKOD_BUILD)
  layoutWidget1->hide();
  layoutWidget2->hide();
  fplView->hide();
  label_3->hide();
#endif
}

UserActionsBrowser::~UserActionsBrowser()
{
}

void UserActionsBrowser::closeEvent(QCloseEvent* event)
{
  saveState(staticMetaObject.className());
  AbstractBrowser::closeEvent(event);
}

void UserActionsBrowser::changeEvent(QEvent *event)
{
  QWidget::changeEvent(event);

  switch (event->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    Translations::translateOciSqlModel(fpls);
    Translations::translateOciSqlModel(rpls);
    Translations::translateOciSqlModel(mans);
    Translations::translateOciSqlModel(telegerams);
    break;
  default:
    break;
  }
}

void UserActionsBrowser::updateModel()
{
  QStringList filters;

  if (filterButton->isChecked()) {
    if (userBox->currentIndex() > 0) {
      int userId = users->record(userBox->currentIndex()).value("id").toInt();
      filters.append(QString(" (modified_by = %1) ").arg(userId));
    }

    if (typeBox->currentIndex() > 0) {
      QString modificationType = types->record(typeBox->currentIndex()).value(
        "type").toString();

      filters.append(QString(" (modification_type = '%1') ").arg(
        modificationType));
    }

    if (between->checkState() != Qt::Unchecked) {
      QString startStr = start->dateTime().toString("yyyy-MM-dd hh:mm:ss");
      QString endStr = end->dateTime().toString("yyyy-MM-dd hh:mm:ss");
      filters.append(QString(
        " (modified_at between timestamp'%1' and timestamp'%2') ").arg(
        startStr, endStr));
    }
  }

  QString filter = filters.join(" AND ");
  if (filter != fpls->filter()) {
    fpls->setFilter(filter);
    rpls->setFilter(filter);
    mans->setFilter(filter);
    telegerams->setFilter(filter);

    fpls->refresh();
    rpls->refresh();
    mans->refresh();
    telegerams->refresh();
  }
}

// -------------------------------------------------------
UserActionsBrowserHelper::UserActionsBrowserHelper(QObject* parent) :
  ObjectsManagerHelper(parent), mAction(0)
{
  mAction = new QAction(icon(), name(), this);
  mAction->setObjectName(id());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}

QString UserActionsBrowserHelper::id() const
{
  return UserActionsBrowserHelper::staticMetaObject.className();
}

QString UserActionsBrowserHelper::path() const
{
  return tr("/Administration/User actions");
}

QString UserActionsBrowserHelper::name() const
{
  return tr("User actions");
}

QString UserActionsBrowserHelper::description() const
{
  return tr("What user do with database in same period");
}

QWidget* UserActionsBrowserHelper::widget(QWidget* parent) const
{
  QWidget *wdg = new UserActionsBrowser(parent);
  wdg->setWindowTitle(name());
  return wdg;
}

QAction* UserActionsBrowserHelper::action() const
{
  return mAction;
}

void UserActionsBrowserHelper::retranslateUi()
{
  mAction->setText(name());
  mAction->setIcon(icon());
  mAction->setToolTip(description());
  mAction->setStatusTip(description());
}
