#ifndef ARINCDATABASEIMPORTERDIALOG_H
#define ARINCDATABASEIMPORTERDIALOG_H

#include "ui_ArincDatabaseImporterDialog.h"
#include "DeferredCallThread.h"
#include "ArincDatabaseImporterDC.h"

class ArincDatabaseImporterDialog : public QDialog, private Ui::ArincDatabaseImporterDialog
{
    Q_OBJECT

public:
    explicit ArincDatabaseImporterDialog(QWidget *parent = 0);

    int startImport(const ArincNativeStorage *st, int firPos, const QSqlDatabase &db = QSqlDatabase::database());

public slots:
    virtual void reject();

protected:
    virtual void closeEvent(QCloseEvent *e);

private slots:
    void threadFinished();
    void showState(const QString &state);

private:
    DeferredCallThread *thread;
};

#endif // ARINCDATABASEIMPORTERDIALOG_H
