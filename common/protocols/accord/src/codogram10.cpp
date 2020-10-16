#include "codogram10.h"
#include "accordutility.h"


Codogram10::Codogram10()
    : mPackageNumberDiscarded(0), mAzimuthMark(0), mBufferOverload(true),
    mVRF(0)
{
}

int Codogram10::classCategory()
{
    return 10;
}

QString Codogram10::classEdition()
{
    return "1.0";
}

int Codogram10::category() const
{
    return classCategory();
}

QString Codogram10::edition() const
{
    return classEdition();
}

AccordErrors::ErrorCode Codogram10::parse(QDataStream &stream)
{
    try
    {
        quint8 byte = read<quint8>(stream) >> 3;
        if ( byte != 0x06 )
            throw AccordErrors::Error_InvalidParser;

        mPackageNumberDiscarded = read<quint8>(stream) >> 2;

        read<quint16>(stream);
        read<quint32>(stream);

        mVRF = read<quint8>(stream) * 100;

        byte = read<quint8>(stream);

        mAzimuthMark = byte >> 4;

        mBufferOverload = bool((byte >> 3) & M1);

        read<quint16>(stream);
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

AccordErrors::ErrorCode Codogram10::generate(QByteArray &result) const
{
    result.clear();
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        quint8 tc = 0x06 << 3;
        write8(stream, tc);

        write8(stream, mPackageNumberDiscarded << 2);
        write16(stream, 0);

        write32(stream, 0);

        write8(stream, mVRF / 100);

        quint8 byte = mAzimuthMark << 4;
        byte |= quint8(mBufferOverload) << 3;
        write8(stream, byte);

        write16(stream, 0);
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}
