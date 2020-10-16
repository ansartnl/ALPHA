#include "LoginDialogLocal.h"

#include "Users.h"

#include <QtCore/QSettings>

#include <QtGui/QMessageBox>

LoginDialogLocal::LoginDialogLocal(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    foreach (const QString& userName, users.keys())
        login->addItem(userName);
    QSettings settings;
    login->setCurrentIndex(login->findText(settings.value("LastUser").toString()));
    if (login->currentText().isEmpty())
        login->setFocus();
}

LoginDialogLocal::~LoginDialogLocal()
{
    QSettings settings;
    if (!currentUserLogin.isEmpty())
        settings.setValue("LastUser", currentUserLogin);
}

void LoginDialogLocal::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void LoginDialogLocal::accept()
{
    if (users.value(login->currentText()).toHash().value("password").toString()
            != passwordHash(password->text()))
        QMessageBox::warning(this, tr("Warning"), tr("Login or password was incorrect"));
    else {
        currentUser = users.value(login->currentText()).toHash();
        currentUserLogin = login->currentText();

        QDialog::accept();
    }
}
