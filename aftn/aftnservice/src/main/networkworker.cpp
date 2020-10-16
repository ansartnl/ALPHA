#include "networkworker.h"

#include "aftnclient.h"
#include "aftnservice.h"
#include "aftnmessageprocessor.h"

#include "qtxmlsettings.h"

#include "qita2textcodec.h"

#include "logmanager.h"
#include "telnetappender.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

NetworkWorker::NetworkWorker(QObject* parent) :
  AFTNWorkerBase(parent)
{
  if (objectName().isEmpty())
    setObjectName("network_worker");
}

NetworkWorker::~NetworkWorker()
{
}

void NetworkWorker::run()
{
  NetworkWorkerBase ptr;

  connect(&ptr, SIGNAL(aftnStatusChanged(bool)), this, SIGNAL(aftnStatusChanged(bool)));
  qloginfo(tr("Network processing"));

  if (!ptr.start())
    return;
  // main loop
  exec();
  ptr.stop();

  qloginfo(tr("Stop network processing"));
}

// --------------------------------------
NetworkWorkerBase::NetworkWorkerBase(QObject* parent) :
  WorkerBase(parent),  codec(0), ita2codec(
    new QITA2TextCodec()), mClient(new AFTNClient(this)), mProcessor(
            new AFTNMessageProcessor(QSqlDatabase(), this))
{
  init();

  using namespace Log4Qt;
  telnetAppender = qobject_cast<TelnetAppender*> (
    LogManager::rootLogger()->appender("telnetAppender"));
  telnetAppender->setWelcomeMessage("AFTN client: unconnected");
}

NetworkWorkerBase::~NetworkWorkerBase()
{
}

void NetworkWorkerBase::initDefaultSettings()
{
  qloginfo("Init default settings");
  settings()->beginGroup("tcp_ip_connector");
  settings()->setValue("comment",
    "Настройки модуля tcp/ip подключения к AFTN серверу.");
  settings()->setValue("timer_interval", 5);
  settings()->setValue("timer_interval/comment", "Таймер проверки подключения.");
  settings()->setValue("host1", "127.0.0.1");
  settings()->setValue("host1/comment", "Адрес сервера AFTN сообщений.");
  settings()->setValue("host2", "192.168.1.1");
  settings()->setValue("host2/comment",
    "Адрес запасного сервера AFTN сообщений.");
  settings()->setValue("port1", "10200");
  settings()->setValue("port1/comment", "Порт сервера AFTN сообщений.");
  settings()->setValue("port2", "10200");
  settings()->setValue("port2/comment",
    "Порт запасного сервера AFTN сообщений.");
  settings()->setValue("aftn_name", "somename");
  settings()->setValue("aftn_name/comment", "Имя абонента.");
  settings()->setValue("ping_timeout", 10);
  settings()->setValue("ping_timeout/comment", "Таймер ping'а.");
  settings()->setValue("text_codec", "ita2");
  settings()->setValue("text_codec/comment", "Кодировка сообщений.");

  settings()->beginGroup("aftn_message_processor");
  settings()->setValue("comment", "Настройки модуля парсинга AFTN сообщений.");
  settings()->setValue("fir_name", "UMMV");
  settings()->setValue("fir_name/comment", "FIR");
  settings()->setValue("cut_fir", true);
  settings()->setValue("cut_fir/comment", "Вырезать FIR из маршрута.");
  settings()->setValue("validate_route", true);
  settings()->setValue("validate_route/comment", "Проверять корректность маршрута.");
  settings()->endGroup();

  settings()->endGroup();
}

void NetworkWorkerBase::loadSettings()
{
  qloginfo("Load settings");
  settings()->beginGroup("tcp_ip_connector");

  interval = settings()->value("timer_interval").toInt();

  mHost1 = settings()->value("host1").toString();
  mHost2 = settings()->value("host2").toString();
  mPort1 = settings()->value("port1").toInt();
  mPort2 = settings()->value("port2").toInt();

  codecName = settings()->value("text_codec").toString();

  // FIXME Workaround as static text coder
  if (codecName == "ITA2") {
    codec = ita2codec.data();
  } else {
    codec = QTextCodec::codecForName(codecName.toLatin1());
  }

  // TODO remove from HEX later, when authorizating encoding released
  mAftnName = settings()->value("aftn_name").toString().toAscii().toByteArray();
  mPingTimeout = settings()->value("ping_timeout").toInt();

  settings()->beginGroup("aftn_message_processor");
  mProcessor->setFIRName(settings()->value("fir_name").toString());
  mProcessor->setSaveOnlyFIR(settings()->value("cut_fir").toBool());
  mProcessor->setValidateRoute(settings()->value("validate_route").toBool());
  settings()->endGroup();
  settings()->endGroup();
}

bool NetworkWorkerBase::start()
{
  connect(mClient, SIGNAL(aftnStateChanged(AFTNClient::AftnState)), this,
    SLOT(clientStateChanged(AFTNClient::AftnState)));

  if (!settings()->contains("tcp_ip_connector/host1"))
    initDefaultSettings();

  loadSettings();

  mProcessor->setDatabase(QSqlDatabase::database(connectionName()));

  mClient->setTextCodec(codec);
  mClient->setProperty("lastServer", 2);
  timerId = startTimer(interval * 1000);
  return QSqlDatabase::database(connectionName()).isOpen();
}

void NetworkWorkerBase::stop()
{
  killTimer(timerId);

  if (mClient->aftnState() != AFTNClient::AftnUnconnected)
    mClient->disconnectFromHost();

  mProcessor->setDatabase(QSqlDatabase());
}

void NetworkWorkerBase::processMessages(const QStringList& messages)
{
  QSqlQuery arcAftnQuery(TBL_ARC_AFTN_INSERT, QSqlDatabase::database(
    connectionName()));
  QSqlQuery manAftnQuery(TBL_MAN_AFTN_INSERT, QSqlDatabase::database(
    connectionName()));
//  qloginfo(tr("Process network messages: %1").arg(messages.join(" ")));
  foreach(QString message, messages)
    {
      // FIXME WORKAROUND FOR MINSK AIRPORT. Char '&' interprets as new line
      message.replace(MESSAGE_CONCAT_SYMBOL, MESSAGE_CONCAT_REPLACE);
      if (message.isEmpty())
        continue;

      int messageBeginIndex = message.indexOf(MESSAGE_BEGIN);
      if (messageBeginIndex == -1)
        // Message not found
        continue;

      QString header;
      header = message.left(messageBeginIndex - 1);

      int messageEndIndex = message.lastIndexOf(MESSAGE_END);
      if (messageEndIndex == -1 || messageBeginIndex > messageEndIndex)
        // Invalid message format
        continue;
      message = message.mid(messageBeginIndex, messageEndIndex
          - messageBeginIndex + 1);
      arcAftnQuery.addBindValue(message);
      arcAftnQuery.addBindValue(header);
      if (!arcAftnQuery.exec()) {
        qloginfo(tr("Query execution error. Executed query: %1\nError: %2")
            .arg(arcAftnQuery.lastQuery(), arcAftnQuery.lastError().text()));
        break;
      }
      AFTNMessageProcessor::AftnMessageErrors errorCodes;
      QVariantMap messageMap = mProcessor->processMessage(message, errorCodes);
      if (errorCodes.isEmpty()) {
        mProcessor->insertMessage(messageMap, errorCodes);

        if (!errorCodes.isEmpty()) {
          // TODO send to some output
        }
      }

      if (!errorCodes.isEmpty()) {
        QStringList errors = mProcessor->errorStrings(errorCodes);
        qlogwarn(tr("Processing AFTN telegramm failed with errors %1").arg(errors.join("\n")));

        manAftnQuery.addBindValue(message);
        manAftnQuery.addBindValue(header);

        if (!manAftnQuery.exec()) {
          qloginfo(tr("Query execution error. Executed query: %1\nError: %2")
              .arg(manAftnQuery.lastQuery(), manAftnQuery.lastError().text()));
          break;
        }
      }
    }
}

void NetworkWorkerBase::clientStateChanged(AFTNClient::AftnState state)
{
  QString message;
  switch (state) {
  case AFTNClient::AftnUnconnected:
    message = tr("AFTN client: unconnected");
    telnetAppender->setWelcomeMessage(message);
    emit aftnStatusChanged(false);
    break;
  case AFTNClient::AftnAuthorizating:
    message = tr("AFTN client: authorizating");
    break;
  case AFTNClient::AftnHostLookup:
    message = tr("AFTN client: host lookup");
    break;
  case AFTNClient::AftnConnecting:
    message = tr("AFTN client: connecting...");
    break;
  case AFTNClient::AftnConnected:
    message = tr("AFTN client: connected");
    telnetAppender->setWelcomeMessage(message);
    emit aftnStatusChanged(true);
    break;
  case AFTNClient::AftnWaitForAuthorizationInfo:
    message = tr("AFTN client: wating for authorization");
    break;
  case AFTNClient::AftnReceiving:
    message = tr("AFTN client: receiving");
    break;
  case AFTNClient::AftnInterrupted:
    message = tr("AFTN client: interrupted");
    break;
  case AFTNClient::AftnClosing:
    message = tr("AFTN client: closing");
    break;
  }

  qloginfo(message);
}

void NetworkWorkerBase::timerEvent(QTimerEvent* event)
{
  if (event->timerId() == timerId) {
    if (mClient->state() == QAbstractSocket::UnconnectedState) {
      if (mClient->property("lastServer").toInt() == 2) {
        qlogwarn(tr("Connecting to host %1").arg(mHost1));
        mClient->setAftnName(mAftnName);
        mClient->setPingTimeout(mPingTimeout);
        mClient->connectToHost(mHost1, mPort1);
        mClient->setProperty("lastServer", 1);
      } else {
        qlogwarn(tr("Connecting to host %1").arg(mHost2));
        mClient->setAftnName(mAftnName);
        mClient->setPingTimeout(mPingTimeout);
        mClient->connectToHost(mHost2, mPort2);
        mClient->setProperty("lastServer", 2);
      }
    }
    return;
  }
  WorkerBase::timerEvent(event);
}
