#include "kramsproto.h"

#include "kramsping.h"
#include "kramstickets.h"
#include "krams4.h"
#include "krams4dump.h"
#include "krams4metarspeci.h"

#include <QtCore/QDebug>


TKramsMessage KramsProtocol::fromArray(const QByteArray &data, int *readed)
{
    mLastErrors.clear();

    // Try to find "METEODISPLAY" "METAR" "SPECI" key words.
    // If they found - krams message is in the data buffer.

    static const QString gMeteodisplayKey = "METEODISPLAY";
    static const QString gMetarKey = "METAR";
    static const QString gSpeciKey = "SPECI";

    int posKeyMd = data.indexOf(gMeteodisplayKey),
        posKeyMetar = data.indexOf(gMetarKey),
        posKeySpeci = data.indexOf(gSpeciKey);

    TKramsMessage message;

    // Check what message we received.

    if ( posKeyMd != -1 &&
         (posKeyMetar == -1 || (posKeyMetar != -1 && posKeyMd < posKeyMetar)) &&
         (posKeySpeci == -1 || (posKeySpeci != -1 && posKeyMd < posKeySpeci)) )
    {
        message = TKramsMessage(new Krams4dump);
    }
    else if ( posKeyMetar != -1 || posKeySpeci != -1 )
    {
        message = TKramsMessage(new Krams4MetarSpeci);
    }

    if ( message )
    {
        mLastErrors = message->parse(data);

        if ( readed )
            *readed = message->length();

        return message;
    }

    // Parsing of Krams-4 message according to the document "proto-KRAMS-ATIS V2.0 .doc" ...

    // First byte should be start of a message <SOH> byte.
    // To point data byte array on this <SOH> byte is not the task for
    // protocol parser.

    if ( data.isNull() || data[0] != 1 )
        return TKramsMessage();

    // Check what message we received.

    // PING message occupies 12 bytes. "Info message" takes more than 12 bytes.
    // Thus check for 12 bytes is fast way to find out what message is received.
    if ( data.size() >= 12 && data[0] == 1 && data[11] == 3 )
        message = TKramsMessage(new KramsPing);
    else
        message = TKramsMessage(new Krams4);

    if ( message )
    {
        mLastErrors = message->parse(data);

        if ( readed )
            *readed = message->length();
    }

    return message;
}

QByteArray KramsProtocol::toArray(const TKramsMessage &message) const
{
    mLastErrors.clear();

    QByteArray result;

    if ( message.isNull() )
        mLastErrors.append( TKramsError(new KramsError(KramsError::Error_NullMessage)) );
    else
        mLastErrors = message->generate(result);

    return result;
}

const QList<TKramsError> &KramsProtocol::lastErrors() const
{
    return mLastErrors;
}
