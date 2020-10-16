#ifndef KRAMS_PING_H
#define KRAMS_PING_H

#include "krams.h"

#include <QtCore/QMap>
#include <QtCore/QPair>


//! Сообщение PING
/*!
    Сообщение PING передается не реже одного раза в 10 секю
*/
class KRAMS_EXPORT KramsPing : public Krams
{
public:
    KramsPing();

    static Type classType();
    static QString classVersion();

    virtual Type type() const;
    virtual QString version() const;

    virtual QList<TKramsError> parse(const QByteArray &input);
    virtual QList<TKramsError> generate(QByteArray &result) const;
};

#endif // KRAMS_PING_H
