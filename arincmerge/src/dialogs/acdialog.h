#ifndef AC_DIALOG_H
#define AC_DIALOG_H

#include "ui_acdialog.h"

#include <QtSql/QSqlDatabase>

class ACDialog : public QDialog, private Ui::ACDialog
{
    Q_OBJECT
public:
    explicit ACDialog(QWidget *parent = 0, const QSqlDatabase &db = QSqlDatabase::database());

private slots:
    void OnRuleButtons(QAbstractButton *pButton);

private:
    bool execStatement(const QString &sStatement, QString &sError);

private:
    QSqlDatabase mDB;
};

#endif
