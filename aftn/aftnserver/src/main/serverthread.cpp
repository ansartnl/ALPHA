#include "serverthread.h"
//#include "moc_serverthread.h"

#include <QtCore/QMutexLocker>

#define QDEBUG(X) qDebug() << Q_FUNC_INFO; \
qDebug() <<  (X)

ServerThread::ServerThread(int socketDescriptor, const QStringList &allowNames,
  QObject *parent) :
  QThread(parent), socketDescriptor(socketDescriptor), mState(
    NotAuthorizatedYet), mAllowNames(allowNames)
{
  tcpSocket.setSocketDescriptor(socketDescriptor);
  memcpy(channelId, QByteArray::number(socketDescriptor).data(),
    sizeof(channelId));

  connect(&pingTimeoutTimer, SIGNAL(timeout()), this, SLOT(timeoutPing()));

  connect(&pingTimer, SIGNAL(timeout()), this, SLOT(sendPing()));

  connect(&confirmationTimer, SIGNAL(timeout()), this, SLOT(
    confirmationTimeout()));
}

void ServerThread::run()
{
  if (!tcpSocket.isValid()) {
    emit error(tcpSocket.error());
    return;
  }
  connect(&tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this,
    SLOT(socketStateChanged(QAbstractSocket::SocketState)));
  connect(&tcpSocket, SIGNAL(disconnected()), this, SLOT(quit()));

  connect(&tcpSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));

  file.setFileName("101.txt");
  file.open(QIODevice::ReadOnly);
  if (tcpSocket.bytesAvailable())
    socketReadyRead();

  exec();
}

void ServerThread::socketStateChanged(QTcpSocket::SocketState state)
{
  switch (state) {
  case QAbstractSocket::HostLookupState:
    QDEBUG("Host searching...")
    ;
    break;
  case QAbstractSocket::ConnectingState:
    QDEBUG("Aftn connecting...")
    ;
    break;
  case QAbstractSocket::ConnectedState:
    QDEBUG("Aftn client connected...")
    ;
    QDEBUG("Sending authorization request...")
    ;
    break;
  case QAbstractSocket::ClosingState:
    QDEBUG("Closing aftn connection...")
    ;
    break;
  case QAbstractSocket::UnconnectedState:
    QDEBUG("Aftn disconnected...")
    ;
    pingTimeoutTimer.stop();
    pingTimer.stop();
    //    confirmationTimer.stop();
    break;
  }
}

void ServerThread::sendAuthorizationReply()
{
  AftnAuthorizationReply reply;
  reply.code = 0;
  tcpSocket.write((const char*) &reply, sizeof(AftnAuthorizationReply));
  tcpSocket.flush();
  AftnAuthorizationInfo info;
  memcpy(info.channelId, channelId, AFTN_CHANNEL_ID_SIZE);
  memcpy(info.aftnName, mAftnName, AFTN_NAME_SIZE);
  tcpSocket.write((const char*) &info, sizeof(AftnAuthorizationInfo));
  tcpSocket.flush();
}

void ServerThread::socketReadyRead()
{
  QByteArray buffer = tcpSocket.readAll();

  // WORKAROUND FOR REMOVE PING
  while (buffer.startsWith((char) 0)) {
    buffer = buffer.mid(1);
  }

  pingTimeoutTimer.stop();
  if (buffer.isEmpty())
    return;

  AftnAuthorizationRequest authRequest;
  switch (mState) {
  case NotAuthorizatedYet:
    QDEBUG("Aftn authorizating...")
    ;
    memcpy(&authRequest, buffer, sizeof(authRequest));

    memset(mAftnName, 0, AFTN_NAME_SIZE + 1);
    memcpy(mAftnName, &authRequest.aftnName, AFTN_NAME_SIZE);
    //if (mAllowNames.contains(mAftnName)) {
    pingTimeout = authRequest.pingTimeout ? authRequest.pingTimeout : 10;
    setState( Normal);
    sendAuthorizationReply();
    pingTimer.start(pingTimeout * 1000);

//    startTimer(5000);

    tcpSocket.write(file.readAll());
    tcpSocket.flush();
    //    } else {
    //      tcpSocket.disconnectFromHost();
    //    }
    break;
  case Interrupted:
    break;
  case Normal:
    QDEBUG("Receive: data...")
    ;

    //emit rawMessage(buffer);
    switch ((const quint8) buffer.at(0)) {
    case AFTN_COMMAND_MAGIC:
      QDEBUG("Receive: command...")
      ;
      // It's command
      if (buffer.length() > 1) {
        switch (buffer.at(1)) {
        case AFTN_COMMAND_CODE_CONFIRM_MESSAGE:
          QDEBUG("Receive: message confimed.")
          ;
          //          confirmationTimer.stop();
          break;
        default:
          break;
        }
      } else {

      }
      break;
    case AFTN_COMMAND_PING:
      QDEBUG("Receive: ping.")
      ;
      // It's ping
      break;
    default:
      QDEBUG("Receive: message.")
      ;
      // It's message
      sendMessageConfirmation();
      break;
    }
    break;

  }

  pingTimeoutTimer.start(pingTimeout * 1000 * 3);
}

void ServerThread::setState(ServerThread::State state)
{
  if (mState != state) {
    mState = state;
    emit stateChanged(state);
  }
}

ServerThread::State ServerThread::state()
{
  return mState;
}

QByteArray ServerThread::aftnName() const
{
  return QByteArray((const char*) mAftnName, AFTN_NAME_SIZE);
}

void ServerThread::sendPing()
{
  QDEBUG("Send: ping.");
  AftnPing ping;
  ping.code = AFTN_PING_CODE_PING;
  tcpSocket.write((const char*) &ping, sizeof(AftnPing));
  tcpSocket.flush();
}

void ServerThread::timeoutPing()
{
  tcpSocket.disconnectFromHost();
}

void ServerThread::sendMessage(const QByteArray &message)
{
  QDEBUG("Send: message.");
  QDEBUG("Set: timer for server confirmation.");
  tcpSocket.write(message);
  tcpSocket.flush();
  //  confirmationTimer.start(pingTimeout * 3 * 1000);
}

void ServerThread::timerEvent(QTimerEvent* event)
{

}

void ServerThread::sendInterruptCode()
{
  sendCommand( AFTN_COMMAND_CODE_INTERRUPT);
  QDEBUG("Send: interrupt command.")
  ;
}

void ServerThread::sendContinueCode()
{
  sendCommand( AFTN_COMMAND_CODE_CONTINUE);
  QDEBUG("Send: continue command.")
  ;
}

void ServerThread::sendMessageConfirmation()
{
  QDEBUG("Send: message confirmation.");
  sendCommand( AFTN_COMMAND_CODE_CONFIRM_MESSAGE);
}

void ServerThread::sendCommand(int code)
{
  AftnCommand aftnCommand;
  aftnCommand.magic = AFTN_COMMAND_MAGIC;
  aftnCommand.code = code;
  tcpSocket.write((const char*) &aftnCommand, sizeof(aftnCommand));
}

void ServerThread::confirmationTimeout()
{
  tcpSocket.disconnectFromHost();
}
