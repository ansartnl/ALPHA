#ifndef SQLMODELHELPER_H
#define SQLMODELHELPER_H

#include <QItemDelegate>
#include <QtSql/QSqlTableModel>

class QSqlQueryModel;

class CSqlModelHelper
{
    public:
        static QString GetUniqueName(QSqlQueryModel& qModel, const QString& sField, const QString& sBase);
};

class CUniqueDelegate: public QItemDelegate
{
	Q_OBJECT
    public:
        CUniqueDelegate(QSqlTableModel* pModel, const QString& sField, QObject * parent = 0);

        virtual void setModelData(QWidget* pEditor, QAbstractItemModel* pModel, const QModelIndex& index) const;

    private:
        QSqlTableModel* m_pGroupModel;
        QString m_sField;
};

#endif // SQLMODELHELPER_H
