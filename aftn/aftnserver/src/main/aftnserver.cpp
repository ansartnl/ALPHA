#include "aftnserver.h"

#include "serverthread.h"

#include <stdlib.h>

#define QDEBUG(X) qDebug() << Q_FUNC_INFO; \
qDebug() <<  (X)

AftnServer::AftnServer(const QStringList &allowNames, QObject *parent)
  : QTcpServer(parent)
  , mAllowNames(allowNames)
{
}

void AftnServer::incomingConnection(int socketDescriptor)
{
  ServerThread *thread = new ServerThread(socketDescriptor, mAllowNames, this);

  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
  thread->start();
}
