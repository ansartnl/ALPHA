#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QtCore/QThread>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QtCore/QFile>

#include <QtNetwork/QTcpSocket>

#include "aftnproto.h"

class ServerThread: public QThread
{
Q_OBJECT

Q_PROPERTY(State state READ state)
Q_PROPERTY(QString aftnName READ aftnName)
public:
  ServerThread(int socketDescriptor, const QStringList& allowNames,
      QObject *parent);

  virtual void run();

  void stop();

  enum State
  {
    NotAuthorizatedYet, Interrupted, Normal
  };

  State state();

  QByteArray aftnName() const;

signals:
  void error(QTcpSocket::SocketError socketError);
  void stateChanged(ServerThread::State state);

public slots:

  void socketStateChanged(QAbstractSocket::SocketState state);

  void socketReadyRead();

  void sendMessage(const QByteArray& message);

protected:
  void timerEvent(QTimerEvent* event);

private slots:
  void sendAuthorizationReply();
  void setState(ServerThread::State state);

  void sendPing();
  void timeoutPing();
  void confirmationTimeout();

  void sendInterruptCode();
  void sendContinueCode();

  void sendMessageConfirmation();

  void sendCommand(int code);

private:
  int socketDescriptor;
  QTcpSocket tcpSocket;

  QString message;
  State mState;
  QStringList mAllowNames;

  quint8 pingTimeout;
  QTimer pingTimer;
  QTimer pingTimeoutTimer;
  QTimer confirmationTimer;

  quint8 mAftnName[AFTN_NAME_SIZE + 1];

  quint8 channelId[AFTN_CHANNEL_ID_SIZE + 1];
  QFile file;
};

#endif
