#ifndef PULSARSHAREFUNCTIONS_H
#define PULSARSHAREFUNCTIONS_H

#include <QtCore/QVariant>

#include <QtNetwork/QHostAddress>

const int mSendTime = 2000;
const int mReceiveTime = 7000;

#if defined(DUSHANBE_BUILD)
const int PulsarDefaultPort = 14000;
const int PulsarDefaultPort2 = 14001;

const int AFTNPulsarDefaultPort = 14002;
const int AFTNPulsarDefaultPort2 = 14003;
#else
const int PulsarDefaultPort = 12000;
const int PulsarDefaultPort2 = 12001;

const int AFTNPulsarDefaultPort = 12002;
const int AFTNPulsarDefaultPort2 = 12003;
#endif

#ifndef DECLARE_METATYPE_HOST_ADDRESS
#define DECLARE_METATYPE_HOST_ADDRESS
Q_DECLARE_METATYPE(QHostAddress)
#endif // DECLARE_METATYPE_HOST_ADDRESS

QString msecToString(quint64 msec);

bool operator <(const QHostAddress &x, const QHostAddress &y);

QDebug stringToConsole(const QString &str);

void broadcast(const QByteArray &data, quint16 port);

#endif // PULSARSHAREFUNCTIONS_H
