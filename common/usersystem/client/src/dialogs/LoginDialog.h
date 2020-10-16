#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "ui_LoginDialog.h"

#include "usDeclare.h"

namespace UserSystem
{

class UserClient;

}

const char s_userName[] = "UserName";

class LoginDialog : public QDialog, private Ui::LoginDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);

    void setUserClient(UserSystem::UserClient *userClient);
    void setUnclearError(bool ue = true);
    void setUserName(const QString &name);

    bool attemptToLogin(const QString &username, const QString &password);

    QString userName();

public slots:
    void loginResult(const UserSystem::Request::Result &result);
    void onLogined();
    void onError(const QString &error);

protected slots:
    void onAccepted();
    void onRejected();

signals:
    void accepted(const QString &name, const QString &password);
    void wrongLogin();

protected:
    bool mUnclearError;
};

#endif // LOGINDIALOG_H
