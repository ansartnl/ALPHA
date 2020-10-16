#include "ChangePasswordDialog.h"

#include "usHelperFunctions.h"
#include "UserClient.h"

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    stateLabel->clear();

//    setAttribute(Qt::WA_DeleteOnClose);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(onAccepted()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(onRejected()));
}

void ChangePasswordDialog::setUserClient(UserSystem::UserClient *userClient)
{
    connect(userClient, SIGNAL(changePasswordResult(UserSystem::Request::Result))
            , this, SLOT(changePasswordResult(UserSystem::Request::Result)));
    connect(this, SIGNAL(accepted(QString)), userClient, SLOT(changePassword(QString)));
}

void ChangePasswordDialog::changePasswordResult(const UserSystem::Request::Result &result)
{
    if (result == UserSystem::Request::Ok)
    {
        onChanged();
    } else
    {
        onError(UserSystem::toString(result));
    }
}

void ChangePasswordDialog::onChanged()
{
    accept();
}

void ChangePasswordDialog::onError(const QString &error)
{
    stateLabel->setText(error);
}

void ChangePasswordDialog::onAccepted()
{
    stateLabel->clear();
    if (!UserSystem::userClient()->testPassword(oldPasswordEdit->text()))
    {
        onError(tr("Wrong password"));
        return;
    }
    if (newPasswordEdit->text() != confirmPasswordEdit->text())
    {
        onError(tr("Passwords do not match"));
        return;
    }
    emit accepted(newPasswordEdit->text());
}

void ChangePasswordDialog::onRejected()
{
    reject();
}
