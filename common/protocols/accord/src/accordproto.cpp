#include "accordproto.h"

#include "codogram0.h"
#include "codogram1.h"
#include "codogram2.h"
#include "codogram3.h"
#include "codogram4.h"
#include "codogram5.h"
#include "codogram6to9.h"
#include "codogram10.h"
#include "codogram11.h"

#include <QtCore/QDebug>

AccordProtocol::AccordProtocol()
    : lastResult(AccordErrors::OK)
{

}

TAccordCodogram AccordProtocol::fromArray(const QByteArray& source)
{
    quint16 length = lengthMessage();

    int dataSize = source.size();
    // Bytes are always 3 words (32 bit each)
    if (dataSize < length)
    {
        setLastResult(AccordErrors::Error_MessageIsShort);
        return TAccordCodogram();
    }

    QDataStream stream(source);
    stream.setByteOrder(QDataStream::BigEndian);

    quint8 TC;
    stream >> TC;

    TC >>= 3;

    TAccordCodogram codogram;
    if ( TC == 0x00 )
        codogram = TAccordCodogram(new Codogram0);
    else if ( TC == 0x01 )
        codogram = TAccordCodogram(new Codogram1);
    if ( TC == 0x02 )
    {
        quint8 DC;
        stream >> DC;

        DC >>= 7;

        if ( DC == 0 )
            codogram = TAccordCodogram(new Codogram2);
        else
            codogram = TAccordCodogram(new Codogram3);
    }
    else if ( TC == 0x03 )
        codogram = TAccordCodogram(new Codogram4);
    else if ( TC == 0x04 )
        codogram = TAccordCodogram(new Codogram5);
    else if ( TC == 0x05 )
        codogram = TAccordCodogram(new Codogram6to9);
    else if ( TC == 0x06 )
        codogram = TAccordCodogram(new Codogram10);
    else if ( TC == 0x07 )
        codogram = TAccordCodogram(new Codogram11);

    Q_ASSERT( stream.device() );

    // Move position of current device pointer to the begining of the stream.
    stream.device()->seek(0);

    AccordErrors::ErrorCode resultCode = AccordErrors::Error_ProtocolNotSupported;
    if (codogram)
        resultCode = codogram->parse(stream);
    setLastResult(resultCode);

    return codogram;
}

int AccordProtocol::codogramCategory(const QByteArray& data)
{
    // Bytes are always 3 words (32 bit each)
    if ( data.size() < lengthMessage() )
    {
        setLastResult( AccordErrors::Error_MessageIsShort );
        return -1;
    }

    QDataStream stream(data);
    stream.setByteOrder(QDataStream::BigEndian);

    quint8 TC;
    stream >> TC;

    TC >>= 3;

    int category = -1;
    AccordErrors::ErrorCode resultCode = AccordErrors::OK;

    TAccordCodogram codogram;
    if ( TC == 0x00 )
        category = Codogram0::classCategory();
    else if ( TC == 0x01 )
        category = Codogram1::classCategory();
    if ( TC == 0x02 )
    {
        quint8 DC;
        stream >> DC;

        DC >>= 7;

        if ( DC == 0 )
            category = Codogram2::classCategory();
        else
            category = Codogram3::classCategory();
    }
    else if ( TC == 0x03 )
        category = Codogram4::classCategory();
    else if ( TC == 0x04 )
        category = Codogram5::classCategory();
    else if ( TC == 0x05 )
    {
        // Move file pointer to the begining of the file.
        stream.device()->seek(0);

        // Parse codogram to find out what category codogram belongs.
        Codogram6to9 codogram6to9;
        resultCode = codogram6to9.parse(stream);
        category = codogram6to9.category();
    }
    else if ( TC == 0x06 )
        category = Codogram10::classCategory();
    else if ( TC == 0x07 )
        category = Codogram11::classCategory();

    setLastResult( resultCode );
    return category;
}

QByteArray AccordProtocol::toArray(const TAccordCodogram &codogram) const
{
    QByteArray result;

    if ( codogram.isNull() )
        setLastResult(AccordErrors::Error_NullCodogram);
    else
    {
        AccordErrors::ErrorCode resultCode = codogram->generate(result);
        setLastResult(resultCode);
    }

    return result;
}

const AccordErrors &AccordProtocol::lastError() const
{
    return lastResult;
}

const AccordErrors &AccordProtocol::setLastResult(const AccordErrors &error) const
{
    lastResult = error;
    return lastResult;
}
