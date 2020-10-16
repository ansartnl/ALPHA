#ifndef LARGESERVER_H
#define LARGESERVER_H

#include <QtNetwork/QTcpServer>

class LargeSocket;

/*!
 * \class LargeServer
 *
 * \brief Сервер больших данных
 *
 * Данный класс - наследник \a QTcpServer, работает в связке с \a LargeSocket.
 * Его назначение - возвращать указатель на \a LargeSocket в функциях
 * \a LargeServer::nextPendingConnection() и \a LargeServer::nextLargeSocket().
 */

class LargeServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit LargeServer(QObject *parent = 0);

    QTcpSocket * nextPendingConnection();
    LargeSocket * nextLargeSocket();
};

#endif // LARGESERVER_H
