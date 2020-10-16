#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include "ui_AddUserDialog.h"

#include "usDeclare.h"

namespace UserSystem
{

class UserClient;

}

class AddUserDialog : public QDialog, private Ui::AddUserDialog
{
    Q_OBJECT

public:
    explicit AddUserDialog(QWidget *parent = 0);

    void setUserClient(UserSystem::UserClient *userClient);

public slots:
    void addUserResult(const UserSystem::Request::Result &result);
    void onAdded();
    void onError(const QString &error);

protected slots:
    void onAccepted();
    void onRejected();

signals:
    void accepted(const QString &name, const QString &password);
};

#endif // ADDUSERDIALOG_H
