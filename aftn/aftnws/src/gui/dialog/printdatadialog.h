#ifndef PRINTDATADIALOG_H
#define PRINTDATADIALOG_H

#include "ui_printdatadialog.h"

class PrintActionsManager;

class PrintDataDialog : public QDialog, private Ui::PrintDataDialog
{
    Q_OBJECT
public:
    explicit PrintDataDialog(QWidget *parent = 0);

signals:
    void print();

protected:
    void changeEvent(QEvent *e);

    void generate();

    virtual const QString documentName() const = 0;
    virtual const QString userName() const;
    virtual const QString securityLevel() const = 0;
    virtual const QStringList dataList() const;

protected slots:
    void onPrintButton();
    void onCancelButton();

protected:
    PrintActionsManager *printActionsManager;
};

#endif // PRINTDATADIALOG_H
