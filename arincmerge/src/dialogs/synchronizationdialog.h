#ifndef SYNCHRONIZATIONDIALOG_H
#define SYNCHRONIZATIONDIALOG_H

#include "ui_synchronizationdialog.h"

#include <QtSql/QSqlDatabase>

struct ServerInfo;

class Synchronizer;

class SynchronizationDialog : public QDialog, private Ui::SynchronizationDialog
{
    Q_OBJECT
public:
    explicit SynchronizationDialog(QWidget *parent = 0, const QSqlDatabase &db_ = QSqlDatabase::database());

public slots:
    void onSynchronizeButton();

signals:
    void pushServer(const ServerInfo &info);
    void synchronize(const QString &id);

private:
    void loadServers();
    void addServer(const ServerInfo &info);

private slots:
    void onChangeLastSynchronization(const QString &id, const QString &lastSynchronization);
    void onChangeStatus(const QString &id, const QString &status);

private:
    enum {
        kServer = 0
        , kLastSynchronization = 1
        , kStatus = 2
    };

    Synchronizer *mSynchronizer;
    QMap < QString, int > mRows;
};

#endif // SYNCHRONIZATIONDIALOG_H
