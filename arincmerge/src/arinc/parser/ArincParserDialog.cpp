#include "ArincParserDialog.h"
#include <QtGui/QCloseEvent>

ArincParserDialog::ArincParserDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    thread = new ArincParserThread(this);
    Qt::WindowFlags flags = windowFlags() | Qt::CustomizeWindowHint;
    flags &= ~Qt::WindowCloseButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    setWindowFlags(flags);

    connect(operButtons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(thread, SIGNAL(finished()), this, SLOT(threadFinished()));
}

int ArincParserDialog::startParsing(ArincFilePosIterator *it, ArincStatusStorage *st)
{
    stateLabel->clear();
    progressBar->setRange(0, it->size());
    setWindowTitle(tr("Loading '%1' ARINC file...").arg(it->fileName()));
    connect(it, SIGNAL(posChanged(int)), progressBar, SLOT(setValue(int)));
    connect(st, SIGNAL(statusChanged(QString)), stateLabel, SLOT(setText(QString)));
    thread->startParser(it, st);
    int res = exec();
    disconnect(it, SIGNAL(posChanged(int)), progressBar, SLOT(setValue(int)));
    disconnect(st, SIGNAL(statusChanged(QString)), stateLabel, SLOT(setText(QString)));
    return res;
}

void ArincParserDialog::reject()
{
    operButtons->setEnabled(false);
    thread->cancel();
}

void ArincParserDialog::threadFinished()
{
    if (thread->isCanceled())
        QDialog::reject();
    else
        QDialog::accept();
}

void ArincParserDialog::closeEvent(QCloseEvent *e)
{
    if (thread->isFinished())
        QDialog::closeEvent(e);
    else
        e->ignore();
}
