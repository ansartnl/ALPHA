#ifndef NEWPASSWORD_H
#define NEWPASSWORD_H

#include "ui_NewPassword.h"
//! Change password
class NewPassword: public QDialog, public Ui::NewPassword
{
    Q_OBJECT
    public:
        NewPassword(QWidget* pParent = 0);

        QString password() const;
    protected:
        void changeEvent(QEvent* event);

    private slots:
        void accept();
};

#endif // NEWPASSWORD_H
