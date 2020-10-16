#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include "ui_ChangePasswordDialog.h"

#include "usDeclare.h"

namespace UserSystem
{

class UserClient;

}

class ChangePasswordDialog : public QDialog, private Ui::ChangePasswordDialog
{
    Q_OBJECT

public:
    explicit ChangePasswordDialog(QWidget *parent = 0);

    void setUserClient(UserSystem::UserClient *userClient);

public slots:
    void changePasswordResult(const UserSystem::Request::Result &result);
    void onChanged();
    void onError(const QString &error);

protected slots:
    void onAccepted();
    void onRejected();

signals:
    void accepted(const QString &password);
};

#endif // CHANGEPASSWORDDIALOG_H
