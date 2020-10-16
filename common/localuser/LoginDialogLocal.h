#ifndef LOGINDIALOGLOCAL_H
#define LOGINDIALOGLOCAL_H

#include "ui_LoginDialogLocal.h"

/*!
  * \class LoginDialogLocal
  * \brief Диалог входа пользователя в систему
  */

class LoginDialogLocal : public QDialog, public Ui::LoginDialogLocal
{
    Q_OBJECT
public:
    explicit LoginDialogLocal(QWidget *parent = 0);
    virtual ~LoginDialogLocal();

public slots:
    virtual void accept();

protected:
    void changeEvent(QEvent *e);
};

#endif // LOGINDIALOGLOCAL_H
