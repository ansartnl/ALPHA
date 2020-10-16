#include "mainwindow.h"

#include "PulsReceiver.h"

#include <QtCore/QMap>
#include <QtCore/QTimer>

#include <QtGui/QMessageBox>
#include <QtGui/QScrollBar>
#include <QtGui/QFileDialog>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QStackedLayout>

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), receiver(new PulsReceiver(AFTNPulsarDefaultPort, this)), wsReceiver(new PulsReceiver(AFTNPulsarDefaultPort2, this))
{
    setupUi(this);

    connect(receiver, SIGNAL(pulsarChanged(QHostAddress,QString,QHostAddress,QVariantHash)), this, SLOT(pulsarChanged(QHostAddress,QString,QHostAddress)));
    connect(receiver, SIGNAL(status(QHostAddress,QString,QVariantHash)), this, SLOT(status(QHostAddress,QString)));

    logLayout = new QStackedLayout(logContainer);

    on_serversView_currentItemChanged(serversView->currentItem(), serversView->currentItem());

    receiver->start();
    // -----------------------------------
    connect(wsReceiver, SIGNAL(status(QHostAddress,QString,QVariantHash)), this, SLOT(wsStatus(QHostAddress,QString)));

    wsLogLayout = new QStackedLayout(wsLogContainer);

    on_wsView_currentItemChanged(wsView->currentItem(), wsView->currentItem());

    vsplitter1->setSizes(vsplitter2->sizes());

    wsReceiver->start();
}

MainWindow::~MainWindow()
{
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_serversView_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    activate->setEnabled(current);
    save->setEnabled(current);
    clear->setEnabled(current);

    information->clear();

    if (current) {
        information->setText(current->data(Qt::UserRole + 1).toString());
        activate->setEnabled(!current->data(Qt::UserRole + 1).toString().contains("Status: active"));

        logLayout->setCurrentWidget(logs.value(current));
    }
}

void MainWindow::on_activate_clicked()
{
    if (serversView->currentItem())
        if (!serversView->currentItem()->data(Qt::UserRole + 1).toString().contains("Status: active"))
            receiver->activatePulsar(QHostAddress(serversView->currentItem()->data(Qt::UserRole + 4).toString()));
}

void MainWindow::pulsarChanged(const QHostAddress &pulsar, const QString& data, const QHostAddress &oldpulsar)
{
    for(int i = 0; i < serversView->count(); ++i) {
        if (serversView->item(i)->data(Qt::UserRole + 4).toString() == pulsar.toString())
        {
            serversView->item(i)->setForeground(Qt::red);
            QFont font;
            font.setItalic(true);
            font.setBold(true);
            serversView->item(i)->setFont(font);
        } else
        {
            serversView->item(i)->setForeground(Qt::black);
            serversView->item(i)->setFont(QFont());
        }
    }
}

void MainWindow::status(const QHostAddress &pulsar, const QString &data)
{
    bool changed = false;
    foreach(QListWidgetItem* item, serversView->findItems(pulsar.toString(), Qt::MatchContains))
    {
        item->setData(Qt::UserRole + 1, data);
        changed = true;
        hosts.key(item)->start();
    }

    if (!changed)
    {
        QString pulsarName;
        QListWidgetItem *item = new QListWidgetItem(pulsar.toString(), serversView);

        item->setData(Qt::UserRole + 1, data);

        item->setData(Qt::UserRole + 3, QHostInfo::lookupHost(pulsar.toString(), this, SLOT(hostFounded(QHostInfo))));

        item->setData(Qt::UserRole + 4, pulsar.toString());

        QTimer *timer(new QTimer(this));
        connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
        timer->setInterval(10000);
        timer->setSingleShot(true);
        timer->start();
        hosts.insert(timer, item);



        QTcpSocket *socket(new QTcpSocket(this));
        connect(socket, SIGNAL(readyRead()), this, SLOT(telnetReadyRead()));
        socket->connectToHost(pulsar, 10023);

        telnets.insert(item, socket);

        QPlainTextEdit *edit = new QPlainTextEdit(logContainer);
        logLayout->addWidget(edit);
        logs.insert(item, edit);
        edit->setMaximumBlockCount(1000);

        if (!serversView->currentItem())
            serversView->setCurrentItem(item);
    } else
    {
        on_serversView_currentItemChanged(serversView->currentItem(), serversView->currentItem());
    }
}


void MainWindow::on_about_triggered()
{
    QMessageBox::about(this, qApp->applicationName(), tr("Application name: %1\nVersion: %2").arg(qApp->applicationName(), qApp->applicationVersion()));
}

void MainWindow::on_aboutQt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::timeout()
{
    QListWidgetItem *item = hosts.take(qobject_cast<QTimer*>(sender()));
    QTcpSocket *socket = telnets.take(item);
    QPlainTextEdit *edit = logs.take(item);
    socket->abort();
    socket->deleteLater();
    edit->deleteLater();
    delete item;

    sender()->deleteLater();
}

void MainWindow::telnetReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QString data = QString::fromUtf8(socket->readAll());

    QListWidgetItem *item = telnets.key(socket);

    logs.value(item)->appendHtml(data);
}

void MainWindow::on_save_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save log"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(logs.value(serversView->currentItem())->toPlainText().toUtf8());
        }
    }
}

void MainWindow::on_clear_clicked()
{
    logs.value(serversView->currentItem())->clear();
}


// ----------------------------------------
void MainWindow::wsStatus(const QHostAddress &pulsar, const QString &data)
{
    bool changed = false;
    foreach(QListWidgetItem* item, wsView->findItems(pulsar.toString(), Qt::MatchContains))
    {
        item->setData(Qt::UserRole + 1, data);
        changed = true;
        wsHosts.key(item)->start();
    }

    if (!changed)
    {
        QListWidgetItem *item = new QListWidgetItem(pulsar.toString(), wsView);

        item->setData(Qt::UserRole + 1, data);

        item->setData(Qt::UserRole + 3, QHostInfo::lookupHost(pulsar.toString(), this, SLOT(hostFounded(QHostInfo))));

        item->setData(Qt::UserRole + 4, pulsar.toString());

        QTimer *timer(new QTimer(this));
        connect(timer, SIGNAL(timeout()), this, SLOT(wsTimeout()));
        timer->setInterval(10000);
        timer->setSingleShot(true);
        timer->start();
        wsHosts.insert(timer, item);

        QTcpSocket *socket(new QTcpSocket(this));
        connect(socket, SIGNAL(readyRead()), this, SLOT(wsTelnetReadyRead()));
        socket->connectToHost(pulsar, 10024);

        wsTelnets.insert(item, socket);

        QPlainTextEdit *edit = new QPlainTextEdit(wsLogContainer);
        wsLogLayout->addWidget(edit);
        wsLogs.insert(item, edit);
        edit->setMaximumBlockCount(1000);

        if (!wsView->currentItem())
            wsView->setCurrentItem(item);
    } else
    {
        on_wsView_currentItemChanged(wsView->currentItem(), wsView->currentItem());
    }
}

void MainWindow::on_wsView_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    wsSave->setEnabled(current);
    wsClear->setEnabled(current);

    wsInformation->clear();

    if (current) {
        wsInformation->setText(current->data(Qt::UserRole + 1).toString());

        wsLogLayout->setCurrentWidget(wsLogs.value(current));
    }
}

void MainWindow::wsTimeout()
{
    QListWidgetItem *item = wsHosts.take(qobject_cast<QTimer*>(sender()));
    QTcpSocket *socket = wsTelnets.take(item);
    QPlainTextEdit *edit = wsLogs.take(item);
    socket->abort();
    socket->deleteLater();
    edit->deleteLater();
    delete item;

    sender()->deleteLater();
}

void MainWindow::wsTelnetReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QString data = QString::fromUtf8(socket->readAll());

    QListWidgetItem *item = wsTelnets.key(socket);

    wsLogs.value(item)->appendHtml(data);
}

void MainWindow::on_wsSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save log"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(wsLogs.value(wsView->currentItem())->toPlainText().toUtf8());
        }
    }
}

void MainWindow::on_wsClear_clicked()
{
    wsLogs.value(wsView->currentItem())->clear();
}

void MainWindow::on_vsplitter1_splitterMoved(int pos, int index)
{
    vsplitter2->setSizes(vsplitter1->sizes());
}


void MainWindow::on_vsplitter2_splitterMoved(int pos, int index)
{
    vsplitter1->setSizes(vsplitter2->sizes());
}

void MainWindow::showEvent(QShowEvent *e)
{
    static bool firstShow = true;
    if (firstShow) {
        firstShow = false;
        vsplitter2->setSizes(vsplitter1->sizes());
    }
}

void MainWindow::hostFounded(const QHostInfo& info)
{
    int success = false;
    for (int i = 0; i < serversView->count(); ++i) {
        QListWidgetItem *item = serversView->item(i);
        if (item->data(Qt::UserRole + 3).toInt() == info.lookupId()) {
            item->setText(QString("%1(%2)").arg(info.hostName(), item->text()));
            item->setData(Qt::UserRole + 3, -1);
            success = true;
            break;
        }
    }
    if (!success)
        for (int i = 0; i < wsView->count(); ++i) {
            QListWidgetItem *item = wsView->item(i);
            if (item->data(Qt::UserRole + 3).toInt() == info.lookupId()) {
                item->setText(QString("%1(%2)").arg(info.hostName(), item->text()));
                item->setData(Qt::UserRole + 3, -1);
                break;
            }
        }
}
