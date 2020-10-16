#ifndef DAILYDLG_SIL_H
#define DAILYDLG_SIL_H

#include "ui_DailyDlgSID.h"

#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>

class CDailyDlgSID: public QDialog
{
    Q_OBJECT
public:
    //! Constructor

    explicit CDailyDlgSID(QString acid, QString type,
                     QWidget *parent = 0);
    ~CDailyDlgSID(){delete ui;}

    //! Inserted SFPL

    QString getValue();

private slots:
    void OnChangeSelect(const QItemSelection &itsnew, const QItemSelection & its);
    void OnChangeSelect2(const QItemSelection &itsnew, const QItemSelection & its);
    void onClickCancel();
    void onClickOk();

private:
    Ui::DailyDialogSID *ui;
    QPushButton* m_pButAct;
    QSqlQueryModel* model_SID;
    QSqlQueryModel* model_SID2;
    QSqlQuery query;
    QString value;
    //QLabel *lblRW3;
    //QLabel *lblRW4;
    //QListView *lv3;
    //QListView *lv4;
};

#endif // DAILYDLG_SID_H
