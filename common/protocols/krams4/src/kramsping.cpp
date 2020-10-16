#include "kramsping.h"

#include <QtCore/QTextStream>


/*!
    Не реже одного раза в 10 сек. КРАМС-4 передает контрольные посылки вида:

    <SOH><CR><CR><LF>PING<CR><CR><LF><ETX>

    АТИС не позднее 5 секунд после приема этой посылки отсылает эту контрольную
    посылку назад в КРАМС.
*/

Krams::Type KramsPing::classType()
{
    return Krams::Ping;
}

QString KramsPing::classVersion()
{
    return "2.0";
}


KramsPing::KramsPing()
{}

Krams::Type KramsPing::type() const
{
    return classType();
}

QString KramsPing::version() const
{
    return classVersion();
}

QList<TKramsError> KramsPing::parse(const QByteArray &input)
{
    QList<TKramsError> errors;

    if ( input.size() < 12 )
        errors.append( TKramsError(new KramsError(KramsError::Error_MessageIsShort)) );
    else
    {
        static char pingMessage[] = { 1, 13, 13, 10, 'P', 'I', 'N', 'G', 13, 13, 10, 3 };

        if ( memcmp(input.constData(), (const void *)pingMessage, 12) != 0 )
            errors.append( TKramsError(new KramsError(KramsError::Error_InvalidInput)) );
    }

    // Set length of parsed messaged.
    mLength = 12;

    return errors;
}

QList<TKramsError> KramsPing::generate(QByteArray &result) const
{
    result.resize(12);

    // <SOH><CR><CR><LF>PING<CR><CR><LF><ETX>
    static char pingMessage[] = { 1, 13, 13, 10, 'P', 'I', 'N', 'G', 13, 13, 10, 3 };

    memcpy((void *)result.data(), (const void *)pingMessage, 12);

    // Set length for generated messaged.
    mLength = 12;

    return QList<TKramsError>();
}
