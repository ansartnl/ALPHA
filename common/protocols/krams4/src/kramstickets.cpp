#include "kramstickets.h"


/*!
    Получение информационного сообщения системой АТИС подтверждается квитанцией,
    передаваемой на центральную систему станции КРАМС-4 в виде:

    <SOH>OK<EOT> 	– Данные приняты
    <SOH>ER<EOT>	– Ошибка приема данных (для версии КРАМС-4 NT)
    <SOH>RQ<EOT> 	– Запрос передачи данных (для версии КРАМС-4 NT)
*/


////////////////////////////////////////////////////////////////////////////////////
// KramsTicketOk implementation.

KramsTicketOk::KramsTicketOk()
{}

Krams::Type KramsTicketOk::classType()
{
    return Krams::TicketOk;
}

QString KramsTicketOk::classVersion()
{
    return "2.0";
}

Krams::Type KramsTicketOk::type() const
{
    return classType();
}

QString KramsTicketOk::version() const
{
    return classVersion();
}

QList<TKramsError> KramsTicketOk::parse(const QByteArray &input)
{
    return ( QList<TKramsError>() <<
             TKramsError(new KramsError(KramsError::Error_ProtocolNotSupported)));
}

QList<TKramsError> KramsTicketOk::generate(QByteArray &result) const
{
    static char ticketMessage[] = { 1, 'O', 'K', 4 };

    result.resize(4);
    memcpy((void *)result.data(), (const void *)ticketMessage, 4);

    // Set length for generated messaged.
    mLength = 4;

    return QList<TKramsError>();
}


////////////////////////////////////////////////////////////////////////////////////
// KramsTicketEr implementation.

KramsTicketEr::KramsTicketEr()
{}

Krams::Type KramsTicketEr::classType()
{
    return Krams::TicketError;
}

QString KramsTicketEr::classVersion()
{
    return "2.0";
}

Krams::Type KramsTicketEr::type() const
{
    return classType();
}

QString KramsTicketEr::version() const
{
    return classVersion();
}

QList<TKramsError> KramsTicketEr::parse(const QByteArray &input)
{
    return ( QList<TKramsError>() <<
             TKramsError(new KramsError(KramsError::Error_ProtocolNotSupported)));
}

QList<TKramsError> KramsTicketEr::generate(QByteArray &result) const
{
    static char ticketMessage[] = { 1, 'E', 'R', 4 };

    result.resize(4);
    memcpy((void *)result.data(), (const void *)ticketMessage, 4);

    // Set length for generated messaged.
    mLength = 4;

    return QList<TKramsError>();
}


////////////////////////////////////////////////////////////////////////////////////
// KramsTicketEr implementation.

KramsTicketRq::KramsTicketRq()
{}

Krams::Type KramsTicketRq::classType()
{
    return Krams::TicketRequest;
}

QString KramsTicketRq::classVersion()
{
    return "2.0";
}

Krams::Type KramsTicketRq::type() const
{
    return classType();
}

QString KramsTicketRq::version() const
{
    return classVersion();
}

QList<TKramsError> KramsTicketRq::parse(const QByteArray &input)
{
    return ( QList<TKramsError>() <<
             TKramsError(new KramsError(KramsError::Error_ProtocolNotSupported)));
}

QList<TKramsError> KramsTicketRq::generate(QByteArray &result) const
{
    static char ticketMessage[] = { 1, 'R', 'Q', 4 };

    result.resize(4);
    memcpy((void *)result.data(), (const void *)ticketMessage, 4);

    // Set length for generated messaged.
    mLength = 4;

    return QList<TKramsError>();
}
