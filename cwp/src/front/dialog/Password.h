#ifndef PASSWORD_H
#define PASSWORD_H

#include "ui_Password.h"
#include "Dao.h"
#include "core/User.h"

//! Password dialog
class Password: public QDialog, public Ui::Password
{
    Q_OBJECT
    public:
        Password(bool cancel, bool lockMouse, QWidget* parent = 0);

        //! Get user
        QSharedPointer<CUser> user() const;

        bool loadUser(const QString &user, const QString &pswd);

    protected:
        virtual void changeEvent(QEvent* event);

    private slots:
        virtual void accept();

    private:
        virtual void closeEvent(QCloseEvent* pEvent);

        QSharedPointer<CUser> mUser;
        bool mCancel;
        bool mLockMouse;
};

#endif // PASSWORD_H
