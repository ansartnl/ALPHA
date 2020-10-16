#include "codogram5.h"
#include "accordutility.h"

// One bit azimuth
static const qreal gAzimuthBit = 5.2734;


Codogram5::Codogram5()
    : mX(0), mY(0), mAzimuth(0), mVRF(0)
{
}

int Codogram5::classCategory()
{
    return 5;
}

QString Codogram5::classEdition()
{
    return "1.0";
}

int Codogram5::category() const
{
    return classCategory();
}

QString Codogram5::edition() const
{
    return classEdition();
}

AccordErrors::ErrorCode Codogram5::parse(QDataStream &stream)
{
    try
    {
        quint8 byte = read<quint8>(stream) >> 3;
        if ( byte != 0x04 )
            throw AccordErrors::Error_InvalidParser;

        read<quint8>(stream);
        read<quint16>(stream);

        mY = parseCoordinate( read<quint16>(stream) );
        mX = parseCoordinate( read<quint16>(stream) );

        mVRF = read<quint8>(stream) * 100;

        read<quint8>(stream);

        mAzimuth = gAzimuthBit * qreal(read<quint16>(stream) >> 1);
        testAzimuth( mAzimuth );
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

AccordErrors::ErrorCode Codogram5::generate(QByteArray &result) const
{
    result.clear();
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        quint8 tc = 0x04 << 3;
        write8(stream, tc);

        write8(stream, 0);
        write16(stream, 0);

        write16(stream, generateCoordinate(mY));
        write16(stream, generateCoordinate(mX));

        write8(stream, mVRF / 100);
        write8(stream, 0);

        testAzimuth( mAzimuth );
        quint16 byte2 = quint16(mAzimuth / gAzimuthBit) << 1;
        write16(stream, byte2);
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

void Codogram5::testAzimuth(qreal azimuth) const
{
    if ( azimuth < 0. || azimuth > 360. )
        throw AccordErrors::Error_5_Azimuth_IncorrectValue;
}
