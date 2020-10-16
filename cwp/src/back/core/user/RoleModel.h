#ifndef ROLEMODEL_H
#define ROLEMODEL_H

#include <QtGui/QStandardItem>
class XMasterWindow;
class QAction;
class CRoleItem: public QObject, public QStandardItem
{
    Q_OBJECT
public:
    CRoleItem(QStandardItem* pDescr);
    CRoleItem(QStandardItem* pDescr, const QString& sUnName, const QString& sName, const QString& sDescr,
              const QVariant& qUserData = QVariant());
    ~CRoleItem();

    void SetName(const QString& sName);
    const QString& GetName() const;
    void SetUniqueName(const QString& sName);
    const QString& GetUniqueName() const;
    void SetDescription(const QString& sDescr);
    const QString& GetDescription() const;
    void SetUserData(const QVariant& qUserData);
    const QVariant& GetUserData() const;

    QStandardItem* m_pDescr;

private:
    QString m_sName;
    QString m_sUniqueName;
    QString m_sDescription;
    QVariant m_qUserData;
};
#endif // ROLEMODEL_H
