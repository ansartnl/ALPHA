#include "AddUserDialog.h"

#include "UserClient.h"

AddUserDialog::AddUserDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    stateLabel->clear();

//    setAttribute(Qt::WA_DeleteOnClose);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(onAccepted()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(onRejected()));
}

void AddUserDialog::setUserClient(UserSystem::UserClient *userClient)
{
    connect(userClient, SIGNAL(addUserResult(UserSystem::Request::Result))
            , this, SLOT(addUserResult(UserSystem::Request::Result)));
    connect(this, SIGNAL(accepted(QString, QString)), userClient, SLOT(addUser(QString,QString)));
}

void AddUserDialog::addUserResult(const UserSystem::Request::Result &result)
{
    if (result == UserSystem::Request::Ok)
    {
        onAdded();
    } else
    {
        onError(UserSystem::toString(result));
    }
}

void AddUserDialog::onAdded()
{
    accept();
}

void AddUserDialog::onError(const QString &error)
{
    stateLabel->setText(error);
}

void AddUserDialog::onAccepted()
{
    stateLabel->clear();
    if (passwordEdit->text() != confirmPasswordEdit->text())
    {
        onError(tr("Passwords do not match"));
        return;
    }
    emit accepted(loginEdit->text(), passwordEdit->text());
}

void AddUserDialog::onRejected()
{
    reject();
}
