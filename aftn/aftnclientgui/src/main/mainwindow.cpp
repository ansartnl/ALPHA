#include "mainwindow.h"

#include <QtCore/QDateTime>

#include <QtGui/QFileDialog>

#include "qtlogevent.h"
#include "aftnclient.h"

#include "main.h"

#define QDEBUG(X) {qDebug() <<  (X);}

    MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
      :QMainWindow(parent, flags)
      , client (new AFTNClient(this))
{
  setupUi(this);

  timer.setInterval(5 * 1000);

  refreshUi();

  networkSettings()->beginGroup(SETTINGS_NETWORK_GROUP);
  rawdata.reset(new QFile(QDateTime::currentDateTime().toString("hh:mm_dd-MM-yyyy") + networkSettings()->value(SETTINGS_RAWDATA_FILE).toString(), this));
  logdata.reset(new QFile(QDateTime::currentDateTime().toString("hh:mm_dd-MM-yyyy") + networkSettings()->value(SETTINGS_LOG_FILE).toString(), this));

  rawdata->open(QIODevice::ReadWrite);
  logdata->open(QIODevice::ReadWrite);

  networkSettings()->endGroup();

  connect(client, SIGNAL(rawPacket(QByteArray))
          , this, SLOT(clientRawMessage(QByteArray)));
  connect(client, SIGNAL(aftnStateChanged(AFTNClient::AftnState))
          , this, SLOT(clientAftnStateChanged(AFTNClient::AftnState)));

  connect(&timer, SIGNAL(timeout())
          , this, SLOT(connectTimer()));
  connect(connectBtn, SIGNAL(clicked())
          , this, SLOT(connectTimer()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::customEvent(QEvent *event)
{
  if (event->type() == QtLogEvent::logEventType) {
    QtLogEvent *logEvent = static_cast<QtLogEvent*>(event);
    log->appendPlainText(logEvent->message());
    logdata->write(logEvent->message().toUtf8());
    logdata->write(QString("\n").toUtf8());
    logdata->flush();
  }
}

void MainWindow::on_connectBtn_clicked(bool checked)
{
  client->disconnectFromHost();
  if (checked) {
    loadSettingsFromUi();
    networkSettings()->beginGroup(SETTINGS_NETWORK_GROUP);
    client->setAftnName(networkSettings()->value(SETTINGS_AFTN_NAME).toString().toAscii());
    client->setPingTimeout(networkSettings()->value(SETTINGS_PING_TIMEOUT).toUInt());
    client->connectToHost(networkSettings()->value(SETTINGS_AFTNSERVER1).toString(), networkSettings()->value(SETTINGS_PORT).toUInt());
    client->setProperty("lastServer", 1);
    networkSettings()->endGroup();
  }
}

void MainWindow::on_interrupt_clicked()
{
  client->setInterrupt(interrupt->isChecked());
}

void MainWindow::refreshUi()
{
  networkSettings()->beginGroup(SETTINGS_NETWORK_GROUP);
  aftn1->setText(networkSettings()->value(SETTINGS_AFTNSERVER1).toString());
  aftn2->setText(networkSettings()->value(SETTINGS_AFTNSERVER2).toString());
  port->setValue(networkSettings()->value(SETTINGS_PORT).toUInt());
  aftnname->setText(networkSettings()->value(SETTINGS_AFTN_NAME).toString());
  pingtimeout->setValue(networkSettings()->value(SETTINGS_PING_TIMEOUT).toUInt());
  logfile->setText(networkSettings()->value(SETTINGS_LOG_FILE).toString());
  rawdatafile->setText(networkSettings()->value(SETTINGS_RAWDATA_FILE).toString());
  networkSettings()->endGroup();
}

void MainWindow::loadSettingsFromUi()
{
  networkSettings()->beginGroup(SETTINGS_NETWORK_GROUP);

  networkSettings()->setValue(SETTINGS_AFTNSERVER1, aftn1->text());
  networkSettings()->setValue(SETTINGS_AFTNSERVER2, aftn2->text());
  networkSettings()->setValue(SETTINGS_PORT, port->value());
  networkSettings()->setValue(SETTINGS_AFTN_NAME, aftnname->text());
  networkSettings()->setValue(SETTINGS_PING_TIMEOUT, pingtimeout->value());
  networkSettings()->setValue(SETTINGS_LOG_FILE, logfile->text());
  networkSettings()->setValue(SETTINGS_RAWDATA_FILE, rawdatafile->text());
  rawdata.reset(new QFile(QDateTime::currentDateTime().toString("hh:MM_dd-mm-yyyy") + networkSettings()->value(SETTINGS_RAWDATA_FILE).toString(), this));
  logdata.reset(new QFile(QDateTime::currentDateTime().toString("hh:MM_dd-mm-yyyy") + networkSettings()->value(SETTINGS_LOG_FILE).toString(), this));
  rawdata->open(QIODevice::ReadWrite);
  logdata->open(QIODevice::ReadWrite);

  networkSettings()->endGroup();
}

void MainWindow::clientRawMessage(const QByteArray &message)
{
  rawdata->write(message);
  rawdata->flush();
}

void MainWindow::clientAftnStateChanged(AFTNClient::AftnState state)
{
  switch (state) {
  case AFTNClient::AftnUnconnected:
    QDEBUG("Aftn client: unconnected");
    break;
  case AFTNClient::AftnAuthorizating:
    QDEBUG("Aftn client: authorizating");
    break;
  case AFTNClient::AftnHostLookup:
    QDEBUG("Aftn client: host lookup");
    break;
  case AFTNClient::AftnConnecting:
    QDEBUG("Aftn client: connecting");
    break;
  case AFTNClient::AftnConnected:
    QDEBUG("Aftn client: connected");
    break;
  case AFTNClient::AftnWaitForAuthorizationInfo:
    QDEBUG("Aftn client: wating for authorization");
    break;
  case AFTNClient::AftnReceiving:
    QDEBUG("Aftn client: receiving");
    break;
  case AFTNClient::AftnInterrupted:
    QDEBUG("Aftn client: interrupted");
    break;
  case AFTNClient::AftnClosing:
    QDEBUG("Aftn client: closing");
    break;
  }
}

void MainWindow::on_toolButton_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Log file"), ".", tr("Log files (*.log);; All files (*.*)"));

  if (!fileName.isEmpty())
    networkSettings()->setValue(SETTINGS_LOG_FILE, fileName);

  refreshUi();
}


void MainWindow::on_toolButton_2_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Raw data file"), ".", tr("Raw data files (*.raw);; All files (*.*)"));

  if (!fileName.isEmpty())
    networkSettings()->setValue(SETTINGS_RAWDATA_FILE, fileName);

  refreshUi();
}

void MainWindow::connectTimer()
{
  if (sender() == connectBtn) {
    if (connectBtn->isChecked()) {
      timer.start();
    } else {
      timer.stop();
      client->disconnectFromHost();
    }
  } else {
    if (connectBtn->isChecked() && client->state() == QAbstractSocket::UnconnectedState) {
      if (client->property("lastServer").toInt() == 2) {
        QDEBUG("Trying connect to AFTN server 1...");
        networkSettings()->beginGroup(SETTINGS_NETWORK_GROUP);

//        client->setAftnName(networkSettings()->value(SETTINGS_AFTN_NAME).toString().toLatin1().data());
//        client->setPingTimeout(networkSettings()->value(SETTINGS_PING_TIMEOUT).toUInt());
//        client->connectToHost(networkSettings()->value(SETTINGS_AFTNSERVER1).toString(), networkSettings()->value(SETTINGS_PORT).toUInt());
//        client->setProperty("lastServer", 1);

        const char hackname[] = {0x93, 0x84, 0x84, 0x84, 0x87, 0x90, 0x87, 0x9C};
        client->setAftnName(QByteArray(hackname, AFTN_NAME_SIZE));
        client->setPingTimeout(0x0a);
        client->connectToHost(networkSettings()->value(SETTINGS_AFTNSERVER1).toString(), networkSettings()->value(SETTINGS_PORT).toUInt());
        client->setProperty("lastServer", 1);

        networkSettings()->endGroup();
      } else {
        networkSettings()->beginGroup(SETTINGS_NETWORK_GROUP);
        QDEBUG("Trying connect to AFTN server 2...");
        client->setAftnName(networkSettings()->value(SETTINGS_AFTN_NAME).toString().toAscii());
        client->setPingTimeout(networkSettings()->value(SETTINGS_PING_TIMEOUT).toUInt());

        const char hackname[] = {0x93, 0x84, 0x84, 0x84, 0x87, 0x90, 0x87, 0x9C};
        client->setAftnName(QByteArray(hackname, AFTN_NAME_SIZE));
        client->setPingTimeout(0x0a);

        client->connectToHost(networkSettings()->value(SETTINGS_AFTNSERVER2).toString(), networkSettings()->value(SETTINGS_PORT).toUInt());
        client->setProperty("lastServer", 2);
        networkSettings()->endGroup();
      }
    }
  }
}


