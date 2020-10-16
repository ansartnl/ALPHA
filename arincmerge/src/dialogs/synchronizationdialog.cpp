#include "synchronizationdialog.h"

#include "main.h"
#include "synchronizer.h"

#include <QtCore/QThread>

SynchronizationDialog::SynchronizationDialog(QWidget *parent, const QSqlDatabase &db_) :
    QDialog(parent)
  , mSynchronizer(new Synchronizer(this, db_))
{
    setupUi(this);

    connect(synchronizeButton, SIGNAL(clicked()), SLOT(onSynchronizeButton()));
    connect(closeButton, SIGNAL(clicked()), SLOT(close()));

    QThread *thread = new QThread;
    mSynchronizer->moveToThread(thread);
    connect(mSynchronizer, SIGNAL(destroyed()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

    connect(this, SIGNAL(pushServer(ServerInfo)), mSynchronizer, SLOT(addServer(ServerInfo)));
    connect(this, SIGNAL(synchronize(QString)), mSynchronizer, SLOT(synchronize(QString)));

    connect(mSynchronizer, SIGNAL(changeLastSynchronization(QString,QString))
            , SLOT(onChangeLastSynchronization(QString,QString)));
    connect(mSynchronizer, SIGNAL(changeStatus(QString,QString)), SLOT(onChangeStatus(QString,QString)));

    loadServers();
}

void SynchronizationDialog::onSynchronizeButton()
{
    const int count = tableWidget->rowCount();
    for (int row = 0; row < count; ++row) {
        Q_ASSERT(!mRows.keys(row).isEmpty());
        if (tableWidget->item(row, kServer)->checkState() == Qt::Checked) {
            emit synchronize(mRows.key(row));
        } else {
            onChangeStatus(mRows.key(row), tr("Skipped."));
        }
    }
}

void SynchronizationDialog::loadServers()
{
    settings()->beginGroup("Servers");

    const QStringList servers = settings()->childGroups();
    foreach (const QString &server, servers) {
        settings()->beginGroup(server);
        const ServerInfo info(
                    server
                    , settings()->value("Driver").toString()
                    , settings()->value("Host").toString()
                    , settings()->value("Port").toInt()
                    , settings()->value("DatabaseName").toString()
                    , settings()->value("User").toString()
                    , settings()->value("Password").toString()
                    );
        settings()->endGroup();

        addServer(info);
    }

    settings()->endGroup();
}

void SynchronizationDialog::addServer(const ServerInfo &info)
{
    const int row = tableWidget->rowCount();

    tableWidget->insertRow(row);
    tableWidget->setItem(row, kServer, new QTableWidgetItem(info.name));
    tableWidget->item(row, kServer)->setCheckState(Qt::Checked);
    tableWidget->setItem(row, kLastSynchronization, new QTableWidgetItem(tr("Connecting...")));
    tableWidget->setItem(row, kStatus, new QTableWidgetItem(""));

    mRows.insert(info.identifier(), row);

    pushServer(info);
}

void SynchronizationDialog::onChangeLastSynchronization(const QString &id, const QString &lastSynchronization)
{
    Q_ASSERT(mRows.contains(id));
    tableWidget->item(mRows[id], kLastSynchronization)->setText(lastSynchronization);
}

void SynchronizationDialog::onChangeStatus(const QString &id, const QString &status)
{
    Q_ASSERT(mRows.contains(id));
    tableWidget->item(mRows[id], kStatus)->setText(status);
}
