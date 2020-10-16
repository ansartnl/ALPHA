#include "krams4metarspeci.h"
#include "MetarSpeciParser.h"

#include <QtCore/QStringList>
#include <QtCore/QDateTime>

/*!
    Вид информационного сообщения:

    <STX>METAR<CR><LF>@METAR DATA<ETX>

    Душанбе dump:

    METAR
    @METAR UTDD 190930Z 27002MPS 230V340 9999 FEW066 37/M04 Q1006 09CLRD70 NOSIG RMK QFE688/0917

*/


Krams4MetarSpeci::Krams4MetarSpeci()
{
}

Krams::Type Krams4MetarSpeci::classType()
{
    return Krams::MetarSpeciTel;
}

QString Krams4MetarSpeci::classVersion()
{
    return "1.0";
}

Krams::Type Krams4MetarSpeci::type() const
{
    return classType();
}

QString Krams4MetarSpeci::version() const
{
    return classVersion();
}

QList<TKramsError> Krams4MetarSpeci::parse(const QByteArray &input)
{
    QList<TKramsError> errors;

    try
    {
        QTextStream stream(input);
        QString inputString = stream.readAll();

        int pos = inputString.indexOf( "@METAR" );

        // Message does not look as "METAR" or "SPECI" telegram.
        if ( pos == -1 )
            throw TKramsError(new KramsError(KramsError::Error_InvalidInput));

        int posETX = -1;
        // Try to find end of message.
        for ( int i = pos+1; i < input.size(); i++ )
        {
            if ( input[i] == 0x03 )
            {
                posETX = i;
                break;
            }
        }

        if ( posETX == -1 )
            throw TKramsError(new KramsError(KramsError::Error_MessageIsShort));

        inputString = inputString.mid(pos+1, posETX-pos-1) + "=";

        MetarSpeciParser parser;
        MetarSpeci *metarSpeci = parser.doit(inputString);

        if ( parser.lastError != MetarSpeci::Ok )
        {
            if ( parser.lastError == MetarSpeci::NotMetarSpeci )
                throw TKramsError(new KramsErrorMessage(KramsError::Error_InvalidInput,
                                                        tr("None METAR or SPECI telegram format")));
            else if ( parser.lastError == MetarSpeci::InvalidAirport )
                throw TKramsError(new KramsErrorMessage(KramsError::Error_InputCorrupted,
                                                        tr("Airport is invalid")));
            else if ( parser.lastError == MetarSpeci::InvalidTime )
                throw TKramsError(new KramsErrorMessage(KramsError::Error_InputCorrupted,
                                                        tr("Time is invalid")));
            else if ( parser.lastError == MetarSpeci::InvalidWind )
                throw TKramsError(new KramsErrorMessage(KramsError::Error_InputCorrupted,
                                                        tr("Wind is invalid")));
            else if ( parser.lastError == MetarSpeci::InvalidVisibility )
                throw TKramsError(new KramsErrorMessage(KramsError::Error_InputCorrupted,
                                                        tr("Visibility is invalid")));
            else if ( parser.lastError == MetarSpeci::InvalidTemperature )
                throw TKramsError(new KramsErrorMessage(KramsError::Error_InputCorrupted,
                                                        tr("Temperature is invalid")));
            else if ( parser.lastError == MetarSpeci::InvalidPressure )
                throw TKramsError(new KramsErrorMessage(KramsError::Error_InputCorrupted,
                                                        tr("Pressure is invalid")));
            else if ( parser.lastError == MetarSpeci::AdditionalUnknownFields )
                throw TKramsError(new KramsErrorMessage(KramsError::Error_InputCorrupted,
                                                        tr("Additional not parsed fields found")));
        }

        mTelegram = TMetarSpeci(metarSpeci);

        // Set length of parsed messaged.
        mLength = posETX+1;
    }
    catch ( const TKramsError &err )
    {
        errors.append(err);
    }

    return errors;
}

QList<TKramsError> Krams4MetarSpeci::generate(QByteArray &result) const
{
    QList<TKramsError> errors;

    qDebug() << tr("Not implemented yet");
    return errors;
}
