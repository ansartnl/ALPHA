#ifndef DAILYDLG_TIME_H
#define DAILYDLG_TIME_H

#include "ui_DailyDlgTime.h"

#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>

class CDailyDlgTime: public QDialog
{
    Q_OBJECT
public:
    explicit CDailyDlgTime(QString acid, QString type,
                     QWidget *parent = 0);
    ~CDailyDlgTime(){delete ui;}

    const QTime getValue();

private slots:
    void onClickCancel();
    void onClickOk();

private:
    Ui::DailyDialogTime *ui;
    QPushButton* m_pButAct;
    QTime value;
};

#endif // DAILYDLG_TIME_H
