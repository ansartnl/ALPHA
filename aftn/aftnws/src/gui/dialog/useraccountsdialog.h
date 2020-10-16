#ifndef USERACCOUNTSDIALOG_H
#define USERACCOUNTSDIALOG_H

#include "abstractdialog.h"

#include "ui_useraccountsdialog.h"

class QSqlTableModel;
class QDataWidgetMapper;
class QSqlQueryModel;

/**
* @brief Dialog display user list, provide setup for user permissions.
\warning User with id == 1 is superuser
*
* @class UserAccountsDialog
*/
class UserAccountsDialog : public AbstractDialog, protected  Ui::UserAccountsDialog
{
  Q_OBJECT
public:
  UserAccountsDialog(QWidget* parent = 0);

private:

  QSqlTableModel* usersModel;
  QSqlTableModel* objectRightsModel;
  QDataWidgetMapper* mapper;

public slots:
  virtual void done(int code);

protected:
  virtual void changeEvent(QEvent *event);

  virtual void closeEvent(QCloseEvent *event);
private:
  bool newUserState;

private slots:
  void on_buttonBox_clicked(QAbstractButton* button);

  void usersListActivateUser(const QModelIndex& index);

  void objectTree_clicked(const QModelIndex& index);

  void on_allow_stateChanged(int checked);
  void on_newUserButton_clicked();
  void on_changePassword_clicked();
  void on_delUserButton_clicked();
};

#include "objectsmanagerhelper.h"

class UserAccountsDialogHelper : public ObjectsManagerHelper
{
  Q_OBJECT
public:
  UserAccountsDialogHelper(QObject* parent = 0);

  virtual QString id() const;
  virtual QString path() const;
  virtual QString name() const;
  virtual QString description() const;

  virtual QIcon icon() const {
    return QIcon("images:32x32/group/group.png");
  }

  virtual QWidget* widget(QWidget* parent = 0) const;
  virtual QAction* action() const;
  virtual void retranslateUi();
private:
  QAction* mAction;
};
#endif // USERACCOUNTSDIALOG_H
