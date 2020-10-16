#ifndef USERSDIALOG_H
#define USERSDIALOG_H

#include "ui_UsersDialog.h"

class QStandardItemModel;

/*!
  * \class UsersDialog
  * \brief Диалог списка пользователей
  *
  * Данный класс реализует функциональную часть диалога, позволяющего работать со списком
  * пользователей, а именно - добавлять, удалять, менять тип, описание и пароль.
  */

class UsersDialog : public QDialog, private Ui::UsersDialog
{
    Q_OBJECT
public:
    explicit UsersDialog(QWidget *parent = 0);

public slots:
    void accept();

protected slots:
    void on_add_clicked();
    void on_remove_clicked();
    void on_changePassword_clicked();
    void on_changeType_clicked();
    void on_changeFullname_clicked();

protected:
    void changeEvent(QEvent *e);

    QStandardItemModel *model;
};

#endif // USERSDIALOG_H
