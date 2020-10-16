#ifndef SMARTSENDER_H
#define SMARTSENDER_H

#include <QtCore/QVariant>

class SmartSender;

/*!
  * \class SmartRequst
  * \brief Запрос
  *
  * Класс хранит список сообщений для передачи и реализует потоковые оператры ввода-вывода.
  */

class SmartRequst
{
public:
    template<class T>
    SmartRequst(SmartSender *s, const T &t) : s(s)
    {
        *this << t;
    }

    virtual ~SmartRequst();

    template<class T>
    inline SmartRequst &operator<< (const T& t)
    {
        data.append(QVariant::fromValue<T>(t));
        return *this;
    }

private:
    SmartSender *s;
    QVariantList data;
};

/*!
  * \class SmartSender
  * \brief Умный отправитель
  *
  * Абстрактный класс, объявляющий функциональность передачи сообщений.
  */

class SmartSender
{
public:
    SmartSender();
    virtual void send(const QByteArray &arr) = 0;
    void sendVariant(const QVariant &data);
};

template<class T>
inline SmartRequst operator<< (SmartSender *server, const T& t)
{
    return SmartRequst(server, t);
}

#endif // SMARTSENDER_H
