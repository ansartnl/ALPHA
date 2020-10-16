#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "ui_logindialog.h"

#include <QtGui/QDialog>

class QSqlQueryModel;
class QAbstractButton;

/// Login dialog which is displayed duting program start
class LoginDialog : public QDialog, public Ui::LoginDialog
{
  Q_OBJECT
public:
  LoginDialog(QWidget *parent = 0);
  virtual ~LoginDialog();

  bool tryToLogin();
  bool allFilled;

public slots:
  virtual void accept(); /*!< Accept button have been pressed. Try to login*/
  virtual void reject();

protected:
  virtual void changeEvent(QEvent *event);
  void loadFromArguments();

private:
  QSqlQueryModel *usersModel;
};


#endif // LOGINDIALOG_H
