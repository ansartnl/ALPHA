#include "logindialog.h"
#include "CmdLineArguments.h"

#include <QtDebug>

#include <QtCore/QCryptographicHash>

#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>

#include "main.h"

static const QString g_hostName = "host";
static const QString g_userName = "user";


LoginDialog::LoginDialog(QWidget* parent) :
  QDialog(parent)
{
  setupUi(this);

  loginStatus->clear();

  usersModel = new QSqlQueryModel(this);
  userName->setModel(usersModel);

  bool hostIsVisible = true;
  if (QSqlDatabase::database(QSqlDatabase::defaultConnection, false).isOpen()) {
      hostLabel->setVisible(false);
      host->setVisible(false);
      hostIsVisible = false;

      usersModel->setQuery("select user_name from users where isactive != 0 and id != 0"
          " order by user_name");
  }

  loadFromArguments();

  password->setFocus();
  if (userName->currentText().isEmpty())
  {
      userName->setEditText( globalSettings()->value(g_userName).toString() );
      if (userName->currentText().isEmpty())
          userName->setFocus();
  }
  if (host->text().isEmpty() && hostIsVisible)
  {
    host->setText( globalSettings()->value(g_hostName).toString() );
    if (host->text().isEmpty())
        host->setFocus();
  }

  if ( ((!host->text().isEmpty() && host->isVisible()) || !host->isVisible())
       && (!userName->currentText().isEmpty())
       && (!password->text().isEmpty()) )
  {
      allFilled = true;
  } else
  {
      allFilled = false;
  }
}

LoginDialog::~LoginDialog()
{
}

bool LoginDialog::tryToLogin()
{
    loginStatus->clear();

    if (!host->text().isEmpty()) {
        QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection, false);

        db.setHostName(host->text());
        if (!db.open()) {
            loginStatus->setText(db.lastError().text());
            return false;
        }
    }

    globalSettings()->setValue(g_hostName, host->text());
    globalSettings()->setValue(g_userName, userName->currentText());

    QString error = login(userName->currentText(), password->text());
    if (error.isEmpty()) {
      return true;
    }
    loginStatus->setText(error);
    return false;
}

void LoginDialog::accept()
{
    if (tryToLogin())
        QDialog::accept();
}

void LoginDialog::reject()
{
    globalSettings()->setValue(g_hostName, host->text());
    globalSettings()->setValue(g_userName, userName->currentText());

    QDialog::reject();
}

void LoginDialog::changeEvent(QEvent *event)
{
    QDialog::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void LoginDialog::loadFromArguments()
{
    CmdLineArguments &cmdLine = CmdLineArguments::instance(qApp->arguments());

    host->setText( cmdLine.host() );
    userName->setEditText( cmdLine.username() );
    password->setText( cmdLine.password() );
}
