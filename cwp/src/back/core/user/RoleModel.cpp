#include "RoleModel.h"
#include <QAction>

CRoleItem::CRoleItem(QStandardItem* pDescr) :
    m_pDescr(pDescr)
{
    appendRow(pDescr);
}

CRoleItem::CRoleItem(QStandardItem* pDescr, const QString& sUnName, const QString& sName, const QString& sDescr,
    const QVariant& qUserData) :
    m_pDescr(pDescr)
{
    SetUniqueName(sUnName);
    SetName(sName);
    SetDescription(sDescr);
    SetUserData(qUserData);
}

CRoleItem::~CRoleItem()
{

}

void CRoleItem::SetName(const QString& sName)
{
    m_sName = sName;
}

const QString& CRoleItem::GetName() const
{
    return m_sName;
}

void CRoleItem::SetUniqueName(const QString& sName)
{
    m_sUniqueName = sName;
}

const QString& CRoleItem::GetUniqueName() const
{
    return m_sUniqueName;
}

void CRoleItem::SetDescription(const QString& sDescr)
{
    m_sDescription = sDescr;
}

const QString& CRoleItem::GetDescription() const
{
    return m_sDescription;
}

void CRoleItem::SetUserData(const QVariant& qUserData)
{
    m_qUserData = qUserData;
}

const QVariant& CRoleItem::GetUserData() const
{
    return m_qUserData;
}
