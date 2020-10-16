#include "UserAdmin.h"
#include "NewPassword.h"
#include "SqlModelHelper.h"

#include "qtautotransaction.h"

#include "TVAError.h"
#include "main.h"

#include <QMessageBox>
#include <QItemDelegate>

#include <QtSql/QSqlRelationalTableModel>
#include <QtSql/QSqlRelationalDelegate>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>

UserAdmin::UserAdmin(const QSharedPointer<CUser>& pLoginUser, QWidget* pParent) :
    QDialog(pParent), m_pLoginUser(pLoginUser)
{
    setupUi(this);

    // User model
    m_pModel = QSharedPointer<CRoleModel> (new CRoleModel());

    m_pUserModel = new QSqlRelationalTableModel;
    m_pUserModel->setTable("client");
    m_iGroupModelIndex = m_pUserModel->fieldIndex("role_id");
    int iGMName = m_pUserModel->fieldIndex("name");
    int iGMFullName = m_pUserModel->fieldIndex("full_name");

    m_pUserModel->setHeaderData(iGMName, Qt::Horizontal, tr("Name"));
    m_pUserModel->setHeaderData(m_iGroupModelIndex, Qt::Horizontal, tr("Role"));
    m_pUserModel->setHeaderData(iGMFullName, Qt::Horizontal, tr("Full name"));

    m_pUserModel->setRelation(m_pUserModel->fieldIndex("role_id"), QSqlRelation("client_role", "id", "name"));

    m_pUserModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_pUserModel->sort(m_pUserModel->fieldIndex("name"),Qt::AscendingOrder);
    UserView->setModel(m_pUserModel);
    // Vovan_465
    /*
    m_pProxyUserModel = new QSortFilterProxyModel(this);
    m_pProxyUserModel->setSourceModel(m_pUserModel);
    m_pProxyUserModel->setDynamicSortFilter(true);

    UserView->setModel(m_pProxyUserModel);
    */

    UserView->setColumnHidden(m_pUserModel->fieldIndex("id"), true);
    UserView->setColumnHidden(m_pUserModel->fieldIndex("password"), true);

    UserView->setItemDelegate(new QSqlRelationalDelegate(UserView));
    UserView->setItemDelegateForColumn(m_pUserModel->fieldIndex("name"),
        new CUniqueDelegate(m_pUserModel, "name", this));
    UserView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    m_pUserModel->select();

    UserView->horizontalHeader()->moveSection(UserView->horizontalHeader()->visualIndex(iGMName), 0);
    UserView->horizontalHeader()->moveSection(UserView->horizontalHeader()->visualIndex(m_iGroupModelIndex), 1);
    UserView->horizontalHeader()->moveSection(UserView->horizontalHeader()->visualIndex(iGMFullName), 2);

    // Group model
    m_pGroupModel = new QSqlTableModel(this);
    m_pGroupModel->setTable("client_role");
    m_pGroupModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_pGroupModel->sort(m_pGroupModel->fieldIndex("name"), Qt::AscendingOrder);
    m_pGroupModel->select();
    GroupView->setModel(m_pGroupModel);
    GroupView->setColumnHidden(m_pGroupModel->fieldIndex("id"), true);
    GroupView->setColumnHidden(m_pGroupModel->fieldIndex("permission"), true);
    GroupView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    connect(GroupView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this,
        SLOT(_OnGroupSelect()));
    GroupView->setItemDelegateForColumn(m_pGroupModel->fieldIndex("name"), new CUniqueDelegate(m_pGroupModel, "name",
        this));

    loadSettings();

    // Privileges
    PrivilegeView->setModel(m_pModel.data());
    connect(PrivilegeView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this,
        SLOT(_OnGroupSelect()));
    //Vovan_465
    //connect(UserView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onHeaderClicked(int)));

    TVA_CATCHALL_TRY
    {
        dao()->LoadUserSectorAll(m_UserSectorList);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
}

UserAdmin::~UserAdmin()
{
    saveSettings();
}

void UserAdmin::OnOperButton(QAbstractButton* pButton)
{
    TVA_CATCHALL_TRY
        {
            QDialogButtonBox::ButtonRole BtnRole = ButtonBox->buttonRole(pButton);
            switch (BtnRole)
            {
            case QDialogButtonBox::AcceptRole:
            case QDialogButtonBox::ApplyRole:
            {
                for (int i = 0; i < m_pUserModel->rowCount(); ++i)
                {
                    QSqlRecord record = m_pUserModel->record(i);
                    if (!record.isNull("name") && record.isNull("client_role_name_2"))
                        TVA_THROW(tr("Group for user \"%1\" isn't set").arg(record.value("name").toString()), E_FAIL);
                }

                QtAutoTransaction transaction(QSqlDatabase::database());
                TVA_CHECK(
                                m_pUserModel->submitAll(),
                                m_pUserModel->lastError().text(),
                                E_FAIL);
                TVA_CHECK(
                                m_pGroupModel->submitAll(),
                                m_pGroupModel->lastError().text(),
                                E_FAIL);
                dao()->SaveUserSectorAll(m_UserSectorList);
                transaction.commit();
                // refresh types in combo box in groupsector
                m_pUserModel->relationModel(m_iGroupModelIndex)->select();
            }
                break;
            case QDialogButtonBox::RejectRole:
            case QDialogButtonBox::DestructiveRole:
                m_pUserModel->revertAll();
                m_pGroupModel->revertAll();
                dao()->LoadUserSectorAll(m_UserSectorList);
                break;
            default:
                break;
            }
        }TVA_CATCHALL(err){
    LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
    QMessageBox::critical(this, tr("Error"), err.reason());
}
}

void UserAdmin::OnAddUser()
{
    QString sBaseName("New user");
    QString sUserName(sBaseName);
    int iIndex(1);
    while (iIndex < 200)
    {
        int iFind = 0;
        for (; iFind < m_pUserModel->rowCount(); ++iFind)
        {
            if (m_pUserModel->record(iFind).value("name").toString() == sUserName)
                break;
        }
        if (iFind == m_pUserModel->rowCount())
        {
            int iRow = m_pUserModel->rowCount();
            m_pUserModel->insertRow(iRow);
            QSqlRecord qRec = m_pUserModel->record(iRow);
            qRec.setValue("name", sUserName);
            m_pUserModel->setRecord(iRow, qRec);
            QModelIndex qmIndex = m_pUserModel->index(iRow, m_pUserModel->fieldIndex("name"));
            UserView->setCurrentIndex(qmIndex);
            UserView->edit(qmIndex);
            break;
        }
        else
        {
            sUserName = sBaseName + QString::number(iIndex++);
        }
    }
}

void UserAdmin::OnRemoveUser()
{
    QModelIndexList IndexList = UserView->selectionModel()->selectedRows();
    QModelIndexList::iterator itIndex = IndexList.begin();
    for (; itIndex != IndexList.end(); ++itIndex)
    {
        if (itIndex->isValid() && m_pUserModel->record(itIndex->row()).value("name").toString()
                        != m_pLoginUser->GetName())
        {
            m_pUserModel->removeRow(itIndex->row());
        }
    }
}

void UserAdmin::OnChangePass()
{
    QModelIndex qInd = UserView->currentIndex();
    if (qInd.isValid())
    {
        NewPassword NewPass(this);
        if (NewPass.exec())
        {
            m_pUserModel->setData(m_pUserModel->index(qInd.row(), m_pUserModel->fieldIndex("password"))
                                  , QVariant(CUser::CalcHash(NewPass.password())));
        }
    }
}

void UserAdmin::OnAllow(int iState)
{
    AllowCheck->blockSignals(true);
    QModelIndex qGroupIndex = GroupView->selectionModel()->currentIndex();
    QModelIndex qPrivIndex = PrivilegeView->selectionModel()->currentIndex();
    if (qGroupIndex.isValid() && qPrivIndex.isValid())
    {
        CRoleItem* pRole = dynamic_cast<CRoleItem*> (m_pModel->itemFromIndex(qPrivIndex));
        if (pRole)
        {
            QSqlRecord qRecord = m_pGroupModel->record(qGroupIndex.row());

            if (pRole->GetUniqueName() == CRoleModel::SECTORING_UNIQUE)
            {
                if (iState == Qt::Checked)
                {
                    if (m_UserSectorList.find(qRecord.value("id").toUInt(), pRole->GetUserData().toUInt())
                                    == m_UserSectorList.end())
                    {
                        m_UserSectorList.insert(qRecord.value("id").toUInt(), pRole->GetUserData().toUInt());
                    }

                }
                else if (iState == Qt::Unchecked)
                {
                    m_UserSectorList.remove(qRecord.value("id").toUInt(), pRole->GetUserData().toUInt());
                }
            }
            else
            {
                QString sPermission = qRecord.value("permission").toString();
                if (iState == Qt::Checked)
                {
                    CUserRole::AddPermission(sPermission, pRole->GetUniqueName());
                }
                else if (iState == Qt::Unchecked)
                {
                    CUserRole::DelPermission(sPermission, pRole->GetUniqueName());
                }
                qRecord.setValue("permission", sPermission);
                m_pGroupModel->setRecord(qGroupIndex.row(), qRecord);
            }
        }
        else
        {
            AllowCheck->setChecked(false);
            AllowCheck->setEnabled(false);
        }
    }
    else
    {
        AllowCheck->setChecked(false);
        AllowCheck->setEnabled(false);
    }
    AllowCheck->blockSignals(false);
}

void UserAdmin::_OnGroupSelect()
{
    AllowCheck->blockSignals(true);
    QModelIndex qGroupIndex = GroupView->selectionModel()->currentIndex();
    QModelIndex qPrivIndex = PrivilegeView->selectionModel()->currentIndex();
    if (qGroupIndex.isValid() && qPrivIndex.isValid())
    {
        CRoleItem* pRole = dynamic_cast<CRoleItem*> (m_pModel->itemFromIndex(qPrivIndex));
        if (pRole)
        {
            AllowCheck->setEnabled(true);
            if (pRole->GetUniqueName() == CRoleModel::SECTORING_UNIQUE)
            {
                if (m_pGroupModel->record(qGroupIndex.row()).value("id").isValid())
                {
                    AllowCheck->setChecked(m_UserSectorList.find(
                        m_pGroupModel->record(qGroupIndex.row()).value("id").toUInt(), pRole->GetUserData().toUInt())
                                    != m_UserSectorList.end());
                }
                else
                {
                    AllowCheck->setChecked(false);
                    AllowCheck->setEnabled(false);
                }
            }
            else
            {
                QString sPermission = m_pGroupModel->record(qGroupIndex.row()).value("permission").toString();
                AllowCheck->setChecked(CUserRole::IsPermission(sPermission, pRole->GetUniqueName()));
            }
        }
        else
        {
            AllowCheck->setChecked(false);
            AllowCheck->setEnabled(false);
        }
    }
    else
    {
        AllowCheck->setChecked(false);
        AllowCheck->setEnabled(false);
    }
    AllowCheck->blockSignals(false);
}

void UserAdmin::OnAddGroup()
{
    QString sBaseName("New group");
    QString sGroupName(sBaseName);
    int iIndex(1);
    while (iIndex < 200)
    {
        int iFind = 0;
        for (; iFind < m_pGroupModel->rowCount(); ++iFind)
        {
            if (m_pGroupModel->record(iFind).value("name").toString() == sGroupName)
                break;
        }
        if (iFind == m_pGroupModel->rowCount())
        {
            int iRow = m_pGroupModel->rowCount();
            m_pGroupModel->insertRow(iRow);
            QSqlRecord qRec = m_pGroupModel->record(iRow);
            qRec.setValue("name", sGroupName);
            m_pGroupModel->setRecord(iRow, qRec);
            QModelIndex qmIndex = m_pGroupModel->index(iRow, m_pGroupModel->fieldIndex("name"));
            GroupView->setCurrentIndex(qmIndex);
            GroupView->edit(qmIndex);
            break;
        }
        else
        {
            sGroupName = sBaseName + QString::number(iIndex++);
        }
    }
}

void UserAdmin::OnRemoveGroup()
{
    QModelIndexList IndexList = GroupView->selectionModel()->selectedRows();
    QModelIndexList::iterator itIndex = IndexList.begin();
    for (; itIndex != IndexList.end(); ++itIndex)
    {
        if (itIndex->isValid() && m_pGroupModel->record(itIndex->row()).value("name").toString()
                        != m_pLoginUser->GetRoleObj()->GetName())
        {
            m_pGroupModel->removeRow(itIndex->row());
        }
    }
}

void UserAdmin::changeEvent(QEvent *event)
{
    //LanguageChange events should not be confused with LocaleChange events.
    if (event->type() == QEvent::LocaleChange)
    {
        // HERE we can un|install qtranslator
        //    Locale-Change events are generated by the system and tell the application,
        //    "Maybe you should load a new translation."
    }
    else if (event->type() == QEvent::LanguageChange)
    {
        //    LanguageChange events are generated by Qt and tell the application's widgets,
        //    "Maybe you should retranslate all your strings."
        retranslateUi(this);
    }

    QDialog::changeEvent(event);
}

void UserAdmin::loadSettings()
{
    restoreGeometry(globalSettings()->value("XMasterWindow/Settings/UserAdminGeometry").toByteArray());
}

void UserAdmin::saveSettings()
{
    globalSettings()->setValue("XMasterWindow/Settings/UserAdminGeometry", saveGeometry());
}

// Vovan_465
/*
void UserAdmin::onHeaderClicked(int n)
{
    //clickColumn = n;
    m_pProxyUserModel->sort(n,UserView->horizontalHeader()->sortIndicatorOrder());
    UserView->horizontalHeader()->setSortIndicatorShown(true);
}*/
