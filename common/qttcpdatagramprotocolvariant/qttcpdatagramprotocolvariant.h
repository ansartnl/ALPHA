#ifndef QTTCPDATAGRAMPROTOCOLVARIANT_H
#define QTTCPDATAGRAMPROTOCOLVARIANT_H

#include "qttcpdatagramprotocol.h"

/*!
  * \class QtTcpDatagramProtocolVariant
  * \brief Отправка пакетов данных по TCP/IP
  *
  * Данный класс предоставляет интерфейс для отправки данных типа QVariant
  * через стек TCP/IP.
  */

class QtTcpDatagramProtocolVariant : public QtTcpDatagramProtocol
{
    Q_OBJECT
public:
    explicit QtTcpDatagramProtocolVariant(QTcpSocket *socket, QObject *parent = 0);
    explicit QtTcpDatagramProtocolVariant(QObject *parent = 0);
    virtual ~QtTcpDatagramProtocolVariant();

signals:
    void newMessages(const QVariantList &data);

private slots:
    void onProtocolMessage();
};

/*!
  * \class QtTcpDatagramProtocolVariantRequest
  * \brief Отправка пакетов данных по TCP/IP
  *
  * Вспомогательный класс для отправки данных типа QVariant
  * через стек TCP/IP.
  */

class QtTcpDatagramProtocolVariantRequest
{
public:
    template<class T>
    QtTcpDatagramProtocolVariantRequest(QtTcpDatagramProtocol *proto, const T& t) : s(proto)
    {
        Q_ASSERT(s);
        *this << t;
    }

    virtual ~QtTcpDatagramProtocolVariantRequest();

    template<class T>
    inline QtTcpDatagramProtocolVariantRequest &operator<< (const T& t)
    {
        data.append(QVariant::fromValue<T>(t));
        return *this;
    }

private:
    QtTcpDatagramProtocol *s;
    QVariantList data;
};

template<class T>
inline QtTcpDatagramProtocolVariantRequest operator<< (QtTcpDatagramProtocol &proto, const T &t)
{
    return QtTcpDatagramProtocolVariantRequest(&proto, t);
}

template<class T>
inline QtTcpDatagramProtocolVariantRequest operator<< (QtTcpDatagramProtocol *proto, const T &t)
{
    return QtTcpDatagramProtocolVariantRequest(proto, t);
}

QtTcpDatagramProtocol &operator>> (QtTcpDatagramProtocol &proto, QVariantList &result);
QtTcpDatagramProtocol &operator>> (QtTcpDatagramProtocol *proto, QVariantList &result);

#endif // QTTCPDATAGRAMPROTOCOLVARIANT_H
