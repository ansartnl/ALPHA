#include "SqlModelHelper.h"

#include <QMessageBox>
#include <QLineEdit>

#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>

QString CSqlModelHelper::GetUniqueName(QSqlQueryModel& model, const QString& field, const QString& base)
{
    QString sMakeName(base);
    int iIndex(1);
    while (iIndex < 200)
    {
        int iFind = 0;
        for (; iFind < model.rowCount(); ++iFind)
        {
            if (model.record(iFind).value(field).toString() == sMakeName)
                break;
        }
        if (iFind == model.rowCount())
        {
            return sMakeName;
        }
        else
        {
            sMakeName = base + QString::number(iIndex++);
        }
    }
    return QString();
}

CUniqueDelegate::CUniqueDelegate(QSqlTableModel* pModel, const QString& sField, QObject * parent) :
    QItemDelegate(parent), m_pGroupModel(pModel), m_sField(sField)
{

}

void CUniqueDelegate::setModelData(QWidget* pEditor, QAbstractItemModel* pModel, const QModelIndex& index) const
{
    QLineEdit* pLineEditor = dynamic_cast<QLineEdit*> (pEditor);
    if (pLineEditor && pModel == m_pGroupModel)
    {
        QString sText = pLineEditor->text();
        // Check names
        int iInd(0);
        for (; iInd < m_pGroupModel->rowCount(); ++iInd)
        {
            if (m_pGroupModel->record(iInd).value(m_sField).toString() == sText && index.row() != iInd)
                break;
        }
        if (iInd != m_pGroupModel->rowCount())
        {
            QSqlRecord qRec = m_pGroupModel->record(index.row());
            QMessageBox::critical(pEditor, tr("Error"), tr("Item with name \"%1\" already exists").arg(pLineEditor->text()));
        }
        else
        {
            QItemDelegate::setModelData(pEditor, pModel, index);
        }
    }
}
