#ifndef USER_H
#define USER_H

#include <QtCore/qglobal.h>
#include <QtCore/QString>
#include <QtCore/QSharedPointer>

//! User role
class CUserRole
{
  public:
    //! Constructor
    CUserRole();
    //! Destructor
    virtual ~CUserRole();
    //! Set identificator
    void SetID(uint uiID);
    //! Get identificator
    uint GetID() const;
    //! Set name
    void SetName(const QString& sName);
    //! Get name
    const QString& GetName() const;
    //! Set permissions
    void SetPermission(const QString& sPerm);
    //! Get permissions
    const QString& GetPermission() const;

    //! Check privilege
    bool IsPermission(const QString& sOne);
    //! Add permission
    void AddPermission(const QString& sOne);
    //! Remove permission
    void DelPermission(const QString& sOne);

    //! Check privilege
    static bool IsPermission(const QString& sPermissions, const QString& sOne);
    //! Add permission
    static void AddPermission(QString& sPermissions, const QString& sOne);
    //! Remove permission
    static void DelPermission(QString& sPermissions, const QString& sOne);

  private:
    static const char DELIMITER[];
    uint m_uiID;
    QString m_sName;
    QString m_sPermissions;
};

//! User
class CUser
{
  public:
    typedef QSharedPointer<CUserRole> TUserRolePtr;
    //	enum UserRole
    //	{
    //		enUknown		= 0,
    //		enAdmin			= 1,
    //		enUser			= 2
    //	};

    //! Constructor
    CUser();
    //! Destructor
    virtual ~CUser();

    //! Set identificator
    void SetID(uint uiID);
    //! Get identificator
    uint GetID() const;
    //! Set name
    void SetName(const QString& sName);
    //! Get name
    const QString& GetName() const;
    //! Set password
    void SetPass(const QString& sPass);
    //! Set password hash
    void SetPassHash(const QByteArray& sPass);
    //! Get password hash
    const QByteArray& GetPassHash() const;
    //! Set role
    void SetRole(uint NewRole);
    //! Get role
    uint GetRole() const;
    //! Set role object
    void SetRoleObj(const TUserRolePtr& NewRole);
    //! Get role object
    const TUserRolePtr& GetRoleObj() const;
    //! Set full name
    void SetFullName(const QString& sFullName);
    //! Get full name
    const QString& GetFullName() const;

    static QByteArray CalcHash(const QString& sPass);

    //! Check password
    bool CheckPassword(const QString& sPass);
  private:
    uint m_uiID;
    QString m_sName;
    QByteArray m_sHash;
    uint m_UserRole;
    TUserRolePtr m_pUserRole;
    QString m_sFullName;
};

#endif // USER_H
