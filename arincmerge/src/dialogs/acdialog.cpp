#include "acdialog.h"

#include <QtSql/QSqlTableModel>
#include "Database.h"
#include <QtSql/QSqlError>
#include <QtCore/QUuid>
#include <QPushButton>
#include <QMessageBox>
#include "qtautotransaction.h"
#include "QDebug"

ACDialog::ACDialog(QWidget *parent, const QSqlDatabase &db)
    : QDialog(parent)
{
    setupUi(this);

    mDB = QSqlDatabase::cloneDatabase(db, QUuid::createUuid().toString());
    mDB.open();

    QSqlTableModel* model = new QSqlTableModel(this, mDB);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("ARINC_CHANGES");
    tableView->setModel(model);
    tableView->sortByColumn(0, Qt::AscendingOrder);
    tableView->resizeColumnsToContents();

    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(OnRuleButtons(QAbstractButton*)));
}

void ACDialog::OnRuleButtons(QAbstractButton *pButton)
{
    if(pButton == buttonBox->button(QDialogButtonBox::Save))
    {
        QModelIndex index = tableView->currentIndex();
        QSqlTableModel* model = static_cast<QSqlTableModel*>(tableView->model());
        if(index.isValid() && model)
        {
            QString sError;
            QString sID = model->data(model->index(index.row(), 0)).toString();
            QString sStatement = model->data(model->index(index.row(), 1)).toString();
            if(!execStatement(sStatement, sError))
                QMessageBox::critical(this, tr("Error id = %1").arg(sID), sError);
            else
                QMessageBox::information(this, tr("Save id = %1").arg(sID), tr("Success\t\t"));
        }
    }
    else if(pButton == buttonBox->button(QDialogButtonBox::SaveAll))
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QString sError;
        QString sID;
        bool bRes = true;
        bool bProc = false;
        QSharedPointer<db::CDBQuery> query = db::CreateQuery(mDB);
        query->Prepare("SELECT id, statement FROM arinc_changes ORDER BY id");
        if(query->exec())
        {
            while(bRes && query->next())
            {
                bProc = true;
                bRes = execStatement(query->value(1).toString(), sError);
                if(!bRes)
                    sID = query->value(0).toString();
            }
        }
        else
            qDebug() << query->lastError().text();
        QApplication::restoreOverrideCursor();
        if(!bRes)
            QMessageBox::critical(this, tr("Error id = %1").arg(sID), sError);
        else
        {
            if(bProc)
                QMessageBox::information(this, tr("Save"), tr("Success\t\t"));
        }
    }
    else if(pButton == buttonBox->button(QDialogButtonBox::Close))
    {
        reject();
    }
}

bool ACDialog::execStatement(const QString& sStatement, QString& sError)
{
    bool bRes = false;
    if(!sStatement.isEmpty())
    {
        QtAutoTransaction dbTrans(mDB);
        QSharedPointer<db::CDBQuery> query = db::CreateQuery(mDB);
        query->Prepare(sStatement);
        bRes = query->exec();
        if(!bRes)
        {
            sError = query->lastError().text();
        }
        else
            dbTrans.commit();
    }
    return bRes;
}
