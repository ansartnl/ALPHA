#include "User.h"

#include <QCryptographicHash>

CUserRole::CUserRole()
	: m_uiID(0)
{

}

CUserRole::~CUserRole()
{

}

void CUserRole::SetID(uint uiID)
{
	m_uiID = uiID;
}

uint CUserRole::GetID() const
{
	return m_uiID;
}

void CUserRole::SetName(const QString& sName)
{
	m_sName = sName;
}

const QString& CUserRole::GetName() const
{
	return m_sName;
}

void CUserRole::SetPermission(const QString& sPerm)
{
	m_sPermissions = sPerm;
}

const QString& CUserRole::GetPermission() const
{
	return m_sPermissions;
}

bool CUserRole::IsPermission(const QString& sOne)
{
	return IsPermission(m_sPermissions, sOne);
}

void CUserRole::AddPermission(const QString& sOne)
{
	AddPermission(m_sPermissions, sOne);
}

void CUserRole::DelPermission(const QString& sOne)
{
	DelPermission(m_sPermissions, sOne);
}

bool CUserRole::IsPermission(const QString& sPermissions, const QString& sOne)
{
	return sPermissions.contains(sOne, Qt::CaseInsensitive);
}

void CUserRole::AddPermission(QString& sPermissions, const QString& sOne)
{
	if (!IsPermission(sPermissions, sOne))
		sPermissions.append(sOne + DELIMITER);
}

void CUserRole::DelPermission(QString& sPermissions, const QString& sOne)
{
	sPermissions.remove(sOne + DELIMITER, Qt::CaseInsensitive);
}

const char CUserRole::DELIMITER[] = "; ";

CUser::CUser()
	: m_uiID(0), m_UserRole(0)
{

}

CUser::~CUser()
{

}

void CUser::SetID(uint uiID)
{
	m_uiID = uiID;
}

uint CUser::GetID() const
{
	return m_uiID;
}

void CUser::SetName(const QString& sName)
{
	m_sName = sName;
}

const QString& CUser::GetName() const
{
	return m_sName;
}

void CUser::SetPass(const QString& sPass)
{
	m_sHash = CalcHash(sPass);
}

void CUser::SetPassHash(const QByteArray& sHash)
{
	m_sHash = sHash;
}

const QByteArray& CUser::GetPassHash() const
{
	return m_sHash;
}

void CUser::SetRole(uint NewRole)
{
	m_UserRole = NewRole;
}

uint CUser::GetRole() const
{
	return m_UserRole;
}

void CUser::SetRoleObj(const TUserRolePtr& NewRole)
{
	m_pUserRole = NewRole;
}

//! Get role object
const CUser::TUserRolePtr& CUser::GetRoleObj() const
{
	return m_pUserRole;
}

void CUser::SetFullName(const QString& sFullName)
{
	m_sFullName = sFullName;
}

const QString& CUser::GetFullName() const
{
	return m_sFullName;
}

QByteArray CUser::CalcHash(const QString& sPass)
{
	return QCryptographicHash::hash(sPass.toUtf8(), QCryptographicHash::Md5);
}

bool CUser::CheckPassword(const QString& password)
{
	QByteArray bHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5);
	return (m_sHash.isEmpty() && password.isEmpty()) || bHash == m_sHash;
}

