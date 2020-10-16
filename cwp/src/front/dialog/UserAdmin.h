#ifndef XUSERADMIN_H
#define XUSERADMIN_H

#include"ui_UserAdmin.h"
#include "core/User.h"
#include "Dao.h"
#include "core/user/RoleModelImpl.h"
//#include <QSortFilterProxyModel> //Vovan_465

class QSqlTableModel;
class QSqlRelationalTableModel;

//! User administration
class UserAdmin: public QDialog, public Ui::UserAdmin

{
    Q_OBJECT
    public:
        UserAdmin(const QSharedPointer<CUser>& pLoginUser, QWidget* pParent = 0);
        ~UserAdmin();

    protected:
        void changeEvent(QEvent* event);

    private:
        void loadSettings();
        void saveSettings();

    private slots:
        void OnOperButton(QAbstractButton* pButton);
        void OnAddUser();
        void OnRemoveUser();
        void OnChangePass();
        void OnAllow(int iState);
        void _OnGroupSelect();
        void OnAddGroup();
        void OnRemoveGroup();
        //void onHeaderClicked(int n); //Vovan_465

    private:

        QSharedPointer<CUser> m_pLoginUser;
        QSharedPointer<CRoleModel> m_pModel;
        QSqlRelationalTableModel* m_pUserModel;
        QSqlTableModel* m_pGroupModel;
        //QSortFilterProxyModel* m_pProxyUserModel; //Vovan_465
        Dao::TUserSectoringList m_UserSectorList;
        int m_iGroupModelIndex;
};

#endif // XUSERADMIN_H
