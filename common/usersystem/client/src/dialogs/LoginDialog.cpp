#include "LoginDialog.h"

#include "usHelperFunctions.h"
#include "UserClient.h"

#include <QEventLoop>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    stateLabel->clear();

//    setAttribute(Qt::WA_DeleteOnClose);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(onAccepted()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(onRejected()));

    mUnclearError = false;

    loginEdit->setFocus();
}

void LoginDialog::setUserClient(UserSystem::UserClient *userClient)
{
    if (connect(userClient, SIGNAL(loginResult(UserSystem::Request::Result))
            , this, SLOT(loginResult(UserSystem::Request::Result))))
    {
        connect(this, SIGNAL(accepted(QString,QString)), userClient, SLOT(login(QString,QString)), Qt::QueuedConnection);
    }
}

void LoginDialog::setUnclearError(bool ue)
{
    mUnclearError = ue;
}

void LoginDialog::setUserName(const QString &name)
{
    loginEdit->setText(name);
    passwordEdit->setFocus();
    // Make sure there is not error message (we can have it if 'attemptToLogin' is called).
    stateLabel->setText(QString());
}

QString LoginDialog::userName()
{
    return loginEdit->text();
}

bool LoginDialog::attemptToLogin(const QString &username, const QString &password)
{
    QEventLoop eventLoop;
    connect(this, SIGNAL(accepted()), &eventLoop, SLOT(quit()));
    connect(this, SIGNAL(wrongLogin()), &eventLoop, SLOT(quit()));

    // Start login request.
    emit accepted(username, password);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    return result() == QDialog::Accepted;
}

void LoginDialog::loginResult(const UserSystem::Request::Result &result)
{
    if (result == UserSystem::Request::Ok)
    {
        onLogined();
    } else
    {
        emit wrongLogin();

        if (mUnclearError)
            onError(tr("Wrong login or password"));
        else
            onError(UserSystem::toString(result));
    }
}

void LoginDialog::onLogined()
{
    accept();
}

void LoginDialog::onError(const QString &error)
{
    stateLabel->setText(error);
}

void LoginDialog::onAccepted()
{
    stateLabel->clear();
    emit accepted(loginEdit->text(), passwordEdit->text());
}

void LoginDialog::onRejected()
{
    reject();
}
