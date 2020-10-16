#ifndef USERPASSWORDDIALOG_H
#define USERPASSWORDDIALOG_H

#include "ui_userpassworddialog.h"

class UserPasswordDialog : public QDialog, public Ui::UserPasswordDialog
{
  Q_OBJECT
public:
  UserPasswordDialog(QWidget* parent = 0);

public slots:
  virtual void  accept();
  virtual void  reject();

protected:
  void changeEvent(QEvent* e);
};

#endif // USERPASSWORDDIALOG_H
