#ifndef KRAMS_H
#define KRAMS_H

#include "kramsexport.h"
#include "kramserrors.h"

#include <QtCore/QSharedPointer>


class KRAMS_EXPORT Krams
{
public:
    //! Type of message
    enum Type
    {
        InfoMessage = 1,    //!< Krams-4 information message
        RawMessage,         //!< Krams-4 raw data, based on Dushanbe netwrok dump
        MetarSpeciTel,      //!< Metar or Speci telegram
        Ping,               //!< Ping
        TicketOk,           //!<
        TicketError,
        TicketRequest
    };

    virtual ~Krams();

    virtual Type type() const = 0;
    virtual QString version() const = 0;

    virtual QList<TKramsError> parse(const QByteArray &input) = 0;
    virtual QList<TKramsError> generate(QByteArray &result) const = 0;

    //! Retrieve message length.
    int length() const { return mLength; }

protected:
    Krams();

    /** Message length */
    mutable int     mLength;
};

typedef QSharedPointer<Krams>   TKramsMessage;

#endif // ACCORD_H
