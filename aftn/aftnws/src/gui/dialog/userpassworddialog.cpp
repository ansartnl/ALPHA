#include "userpassworddialog.h"

UserPasswordDialog::UserPasswordDialog(QWidget* parent) :
  QDialog(parent)
{
  setupUi(this);
  errorLabel->clear();
}

void UserPasswordDialog::changeEvent(QEvent* event)
{
  QDialog::changeEvent(event);

  switch (event->type()) {
    case QEvent::LanguageChange:
      retranslateUi(this);
      break;
    default:
      break;
  }
}

void UserPasswordDialog::accept()
{
  if (password->text().length() < 4) {
    errorLabel->setText(tr("The new passwords should be more than 4 symbols."));
    password->setFocus();
    return ;
  }

  if (password->text() != confirmPassword->text()) {
    errorLabel->setText(tr("The passwords should match."));
    confirmPassword->setFocus();
    return;
  }

  QDialog::accept();
}

void UserPasswordDialog::reject()
{
  QDialog::reject();
}
