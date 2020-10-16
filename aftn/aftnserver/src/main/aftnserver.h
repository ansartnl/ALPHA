#ifndef AFTNSERVER_H
#define AFTNSERVER_H

#include <QtCore/QStringList>

#include <QtNetwork/QTcpServer>

class ServerThread;
class AftnServer : public QTcpServer
{
  Q_OBJECT

public:
  AftnServer(const QStringList& allowNames, QObject *parent = 0);

protected:
  void incomingConnection(int socketDescriptor);

private:
  QStringList mAllowNames;
};

#endif //#define AFTNSERVER_H
