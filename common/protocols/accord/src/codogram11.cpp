#include "codogram11.h"
#include "accordutility.h"

// One bit azimuth
static const qreal gAzimuthBit = 5.2734;


Codogram11::Codogram11()
    : mChannelNumber(0), mAzimuth(0.), mVRF(0)
{
}

int Codogram11::classCategory()
{
    return 11;
}

QString Codogram11::classEdition()
{
    return "1.0";
}

int Codogram11::category() const
{
    return classCategory();
}

QString Codogram11::edition() const
{
    return classEdition();
}

AccordErrors::ErrorCode Codogram11::parse(QDataStream &stream)
{
    try
    {
        quint8 byte = read<quint8>(stream) >> 3;
        if ( byte != 0x07 )
            throw AccordErrors::Error_InvalidParser;

        read<quint8>(stream);

        quint16 byte2 = read<quint16>(stream);

        mChannelNumber = byte2 >> 12;

        mAzimuth = (byte2 & M12) * gAzimuthBit;
        testAzimuth( mAzimuth );

        read<quint32>(stream);

        mVRF = read<quint8>(stream) * 100;

        read<quint8>(stream);
        read<quint16>(stream);
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

AccordErrors::ErrorCode Codogram11::generate(QByteArray &result) const
{
    result.clear();
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        quint8 tc = 0x07 << 3;
        write8(stream, tc);

        write8(stream, 0);

        quint16 byte2 = 0;
        {
            testChannelNumber( mChannelNumber );
            byte2 |= quint16(mChannelNumber) << 12;

            byte2 |= quint16(qRound(mAzimuth / gAzimuthBit)) & M12;
        }
        write16(stream, byte2);

        write32(stream, 0);

        write8(stream, mVRF / 100);

        write8(stream, 0);
        write16(stream, 0);
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

void Codogram11::testChannelNumber(quint8 value) const
{
    if ( value > 15 )
        throw AccordErrors::Error_11_ChannelNumber_IncorrectValue;
}

void Codogram11::testAzimuth(qreal azimuth) const
{
    if ( azimuth < 0. || azimuth > 360. )
        throw AccordErrors::Error_11_Azimuth_IncorrectValue;
}
