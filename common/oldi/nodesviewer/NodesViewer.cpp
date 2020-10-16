#include "NodesViewer.h"

#include <QtCore/QEvent>

SqlFMTPModel::SqlFMTPModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{

}

bool SqlFMTPModel::select()
{
    bool bRes = QSqlTableModel::select();
    if(bRes)
    {
        int iColStatus = fieldIndex("STATUSCODE");
        for(int i = 0; i < rowCount(); ++i)
        {
            QModelIndex ind = index(i, iColStatus);
            QString sText = data(ind).toString();
            bool bOk = false;
            int iValue = sText.toInt(&bOk);
            if(bOk)
            {
                switch(iValue)
                {
                case -1:
                    sText = QString("%1 (%2)").arg(iValue).arg(tr("disable state"));
                    break;
                case 0:
                    sText = QString(" %1 (%2)").arg(iValue).arg(tr("IDLE state"));
                    break;
                case 1:
                    sText = QString(" %1 (%2)").arg(iValue).arg(tr("system id pending"));
                    break;
                case 2:
                    sText = QString(" %1 (%2)").arg(iValue).arg(tr("id pending"));
                    break;
                case 3:
                    sText = QString(" %1 (%2)").arg(iValue).arg(tr("ready state"));
                    break;
                case 4:
                    sText = QString(" %1 (%2)").arg(iValue).arg(tr("association pending state"));
                    break;
                case 5:
                    sText = QString(" %1 (%2)").arg(iValue).arg(tr("data ready state"));
                    break;
                }
                setData(ind, sText);
            }
        }
    }
    return bRes;
}



NodesViewer::NodesViewer(QWidget *parent)
    :QWidget(parent)
{
    setupUi(this);
    // nodes
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("oldi_clients");
    nodeView->setModel(model);
    int ind = model->fieldIndex("ID");
    if(ind != -1)
        nodeView->hideColumn(ind);
    // fmtp
    SqlFMTPModel* modelFMTP = new SqlFMTPModel(this);
    modelFMTP->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelFMTP->setTable("currentstatus");
    fmtpView->setModel(modelFMTP);
    ind = modelFMTP->fieldIndex("STATUSID");
    if(ind != -1)
        fmtpView->hideColumn(ind);

    on_buttonRefresh_clicked();
}

void NodesViewer::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        on_buttonRefresh_clicked();
        break;
    default:
        break;
    }
}

void NodesViewer::on_buttonRefresh_clicked()
{
    static_cast<QSqlTableModel*>(nodeView->model())->select();
    nodeView->resizeColumnsToContents();
    static_cast<QSqlTableModel*>(fmtpView->model())->select();
    fmtpView->resizeColumnsToContents();
}
