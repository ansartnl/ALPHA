#include "ArincDatabaseImporterDialog.h"
#include <QtGui/QCloseEvent>

ArincDatabaseImporterDialog::ArincDatabaseImporterDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    thread = new DeferredCallThread(this);
    Qt::WindowFlags flags = windowFlags() | Qt::CustomizeWindowHint;
    flags &= ~Qt::WindowCloseButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    setWindowFlags(flags);

    connect(operButtons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(thread, SIGNAL(finished()), this, SLOT(threadFinished()));
}

int ArincDatabaseImporterDialog::startImport(const ArincNativeStorage *st, int firPos, const QSqlDatabase &db)
{
    setWindowTitle(tr("Importing '%1' FIR to database...").arg(st->firs[firPos].name));
    stateLabel->clear();
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    ArincDatabaseImporter importer;
    connect(&importer, SIGNAL(posChanged(int)), progressBar, SLOT(setValue(int)));
    connect(&importer, SIGNAL(rangeEvaluated(int)), progressBar, SLOT(setMaximum(int)));
    connect(&importer, SIGNAL(statusChanged(QString)), this, SLOT(showState(QString)));
    connect(operButtons, SIGNAL(rejected()), &importer, SLOT(cancel()));
    ArincDatabaseImporterDC dc(&importer, st, firPos, db, "ArincDatabaseImporterThread");
    thread->call(&dc);
    exec();
    return importer.isCanceled() ? QDialog::Rejected : QDialog::Accepted;
}

void ArincDatabaseImporterDialog::reject()
{
    operButtons->setEnabled(false);
}

void ArincDatabaseImporterDialog::threadFinished()
{
    QDialog::accept();
}

void ArincDatabaseImporterDialog::closeEvent(QCloseEvent *e)
{
    if (thread->isFinished())
        QDialog::closeEvent(e);
    else
        e->ignore();
}

void ArincDatabaseImporterDialog::showState(const QString &state)
{
    stateLabel->setText(state);
}
