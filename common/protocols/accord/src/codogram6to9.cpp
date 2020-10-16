#include "codogram6to9.h"
#include "accordutility.h"

#include <QtCore/QIODevice>


Codogram6to9::Codogram6to9()
    : mType(TypeUnknown)
{
}

int Codogram6to9::classCategoryBegin()
{
    return 6;
}

int Codogram6to9::classCategoryEnd()
{
    return 9;
}

QString Codogram6to9::classEdition()
{
    return "1.0";
}

int Codogram6to9::category() const
{
    return ( mType == TypeUnknown ? -1 : (int(mType)+6) );
}

QString Codogram6to9::edition() const
{
    return classEdition();
}

AccordErrors::ErrorCode Codogram6to9::parse(QDataStream &stream)
{
    try
    {
        quint8 byte = read<quint8>(stream);
        quint8 tc = byte >> 3;
        if ( tc != 0x05 )
            throw AccordErrors::Error_InvalidParser;

        quint8 value = byte & M3;
        testType( value );
        mType = EType(value);

        Q_ASSERT(stream.device());

        // Move pointer to the end of codogram record.
        QIODevice *device = stream.device();
        device->seek( device->pos()+88 );
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

AccordErrors::ErrorCode Codogram6to9::generate(QByteArray &result) const
{
    result.clear();
    try
    {
        quint8 value = quint8(mType);
        testType( value );

        QDataStream stream(&result, QIODevice::WriteOnly);

        quint8 byte = 0x05 << 3;
        byte |= value;
        write8(stream, value);

        if ( mType == Type1 )
        {
            write16(stream, 0x5555);

            write32(stream, 0xAAAAAAAA);

            write32(stream, 0x55555555);
        }
        else if ( mType == Type2 )
        {
            write16(stream, 0xAAAA);

            write32(stream, 0x55555555);

            write32(stream, 0xAAAAAAAA);
        }
        else if ( mType == Type3 )
        {
            write16(stream, 0xAAAA);

            write32(stream, 0xAAAAAAAA);

            write32(stream, 0xAAAAAAAA);
        }
        else if ( mType == Type4 )
        {
            write16(stream, 0x5555);

            write32(stream, 0x55555555);

            write32(stream, 0x55555555);
        }
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

void Codogram6to9::testType(quint8 value) const
{
    if ( value < quint8(Type1) || value > quint8(Type4) )
        throw AccordErrors::Error_6to9_Type_IncorrectValue;
}
