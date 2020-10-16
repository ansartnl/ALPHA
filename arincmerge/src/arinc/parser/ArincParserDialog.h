#ifndef ARINCPARSERDIALOG_H
#define ARINCPARSERDIALOG_H

#include "ui_ArincParserDialog.h"
#include "ArincFilePosIterator.h"
#include "ArincStatusStorage.h"
#include "ArincParserThread.h"

class ArincParserDialog : public QDialog, public Ui::ArincParserDialog
{
    Q_OBJECT
public:
    explicit ArincParserDialog(QWidget *parent = 0);

    int startParsing(ArincFilePosIterator *iterator, ArincStatusStorage *storage);

public slots:
    virtual void reject();

protected:
    virtual void closeEvent(QCloseEvent *e);

private slots:
    void threadFinished();

private:
    ArincParserThread *thread;
};

#endif // ARINCPARSERDIALOG_H
