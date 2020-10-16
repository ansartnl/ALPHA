#ifndef KRAMS_TICKETS_H
#define KRAMS_TICKETS_H

#include "krams.h"
#include "kramserrors.h"

#include <QtCore/QMap>
#include <QtCore/QPair>


//! Квитанция - Данные приняты.
class KRAMS_EXPORT KramsTicketOk : public Krams
{
public:
    KramsTicketOk();

    static Type classType();
    static QString classVersion();

    virtual Type type() const;
    virtual QString version() const;

    virtual QList<TKramsError> parse(const QByteArray &input);
    virtual QList<TKramsError> generate(QByteArray &result) const;
};


//! Квитанция - Ошибка приема данных (для версии КРАМС-4 NT).
class KRAMS_EXPORT KramsTicketEr : public Krams
{
public:
    KramsTicketEr();

    static Type classType();
    static QString classVersion();

    virtual Type type() const;
    virtual QString version() const;

    virtual QList<TKramsError> parse(const QByteArray &input);
    virtual QList<TKramsError> generate(QByteArray &result) const;
};


//! Квитанция - Запрос передачи данных (для версии КРАМС-4 NT).
class KRAMS_EXPORT KramsTicketRq : public Krams
{
public:
    KramsTicketRq();

    static Type classType();
    static QString classVersion();

    virtual Type type() const;
    virtual QString version() const;

    virtual QList<TKramsError> parse(const QByteArray &input);
    virtual QList<TKramsError> generate(QByteArray &result) const;
};

#endif // KRAMS_TICKETS_H
