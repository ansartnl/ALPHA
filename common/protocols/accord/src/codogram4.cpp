#include "codogram4.h"
#include "accordutility.h"


Codogram4::Codogram4()
    : mX1(0), mX2(0), mY1(0), mY2(0), mVRF(0)
{
}

int Codogram4::classCategory()
{
    return 4;
}

QString Codogram4::classEdition()
{
    return "1.0";
}

int Codogram4::category() const
{
    return classCategory();
}

QString Codogram4::edition() const
{
    return classEdition();
}

AccordErrors::ErrorCode Codogram4::parse(QDataStream &stream)
{
    try
    {
        quint8 byte = read<quint8>(stream) >> 3;
        if ( byte != 0x03 )
            throw AccordErrors::Error_InvalidParser;

        read<quint8>(stream);

        mX1 = parseCoordinate( read<quint16>(stream) );
        mY1 = parseCoordinate( read<quint16>(stream) );
        mX2 = parseCoordinate( read<quint16>(stream) );

        mVRF = read<quint8>(stream) * 100;

        read<quint8>(stream);

        mY2 = parseCoordinate( read<quint16>(stream) );
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

AccordErrors::ErrorCode Codogram4::generate(QByteArray &result) const
{
    result.clear();
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        quint8 tc = 0x03 << 3;
        write8(stream, tc);

        write8(stream, 0);

        write16(stream, generateCoordinate(mX1));

        write16(stream, generateCoordinate(mY1));

        write16(stream, generateCoordinate(mX2));

        write8(stream, mVRF / 100);

        write8(stream, 0);

        write16(stream, generateCoordinate(mY2));
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}
