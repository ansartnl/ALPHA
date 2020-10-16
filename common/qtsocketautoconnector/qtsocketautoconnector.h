#ifndef QTSOCKETAUTOCONNECTOR_H
#define QTSOCKETAUTOCONNECTOR_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QAbstractSocket>

class QTimer;

/*!
  * \class QtSocketAutoConnector
  * \brief Автоматическое переподключение
  *
  * Класс предназначен для автоматического подключения отключившегося сокета
  * через заданный промежуток времени к заданным адресу и порту.
  */

// Provide automatic reconnections for QAbstractSocket in specified period of time
// by specified address and port
class QtSocketAutoConnector : public QObject
{
    Q_OBJECT
public:
    explicit QtSocketAutoConnector(QObject *parent = 0); // construct the AutoConnector
    virtual ~QtSocketAutoConnector();
    bool containsAddress(const QHostAddress &address, quint16 port); // check is connector contains address
    void addAddress(const QHostAddress &address, quint16 port); // set up address and port
    void removeAddress(const QHostAddress &address, quint16 port); // remove address and port from connection list
    void removeAddress(int n); // remove address and port from connection list from position n
    void removeAllAddresses(); // clear connection list
    void setInterval(int msInteval); // set up interval between the attempts to connect in ms; by default is 0;
    QAbstractSocket *setSocket(QAbstractSocket *newSocket); // set the socket and start connection procedure; return previous socket

    int interval() const; // return reconnect interval
    inline const QList<QPair<QHostAddress, quint16> > &addresses() const { return connectionList; }

public slots:
    void reconnect();

signals:
    void connectingAttempt(const QHostAddress &address, quint16 port);

private slots:
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void connectSocketToHost();

private:
    QPointer<QAbstractSocket> socket;
    QList<QPair<QHostAddress, quint16> > connectionList;
    int connectionListPos;
    QTimer *timer;
    int mInterval;
};

#endif // QTSOCKETAUTOCONNECTOR_H
