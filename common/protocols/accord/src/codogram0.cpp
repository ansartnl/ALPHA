#include "codogram0.h"
#include "accordutility.h"


Codogram0::Codogram0()
    : mTargetNumberPRL(0), mPackageNumberDiscarded(0), mTargetNumberVRL(0),
    mTargetNumberUnited(0), mExternalFailureSyncNumber(0),
    mOrderProcessing1VRL(false), mOrderProcessing2VRL(false), mOrderProcessing1PRL(false),
    mOrderProcessing2PRL(false), mBasicDeviceUnioinQuota(false),
    mDamageProcessing1VRL(false), mDamageProcessing2VRL(false),
    mDamageProcessing1PRL(false), mDamageProcessing2PRL(false)
{
}

int Codogram0::classCategory()
{
    return 0;
}

QString Codogram0::classEdition()
{
    return "1.0";
}

int Codogram0::category() const
{
    return classCategory();
}

QString Codogram0::edition() const
{
    return classEdition();
}

AccordErrors::ErrorCode Codogram0::parse(QDataStream &stream)
{
    try
    {
        quint8 byte = read<quint8>(stream) >> 3;
        if ( byte != 0 )
            throw AccordErrors::Error_InvalidParser;

        mPackageNumberDiscarded = read<quint8>(stream);

        mTargetNumberPRL = read<quint16>(stream);

        mTargetNumberUnited = read<quint16>(stream);

        mTargetNumberVRL = read<quint16>(stream);

        mExternalFailureSyncNumber = read<quint16>(stream);

        quint16 byte2 = read<quint16>(stream);
        mOrderProcessing2VRL = bool((byte2 >> 1) & M1);
        mOrderProcessing1PRL = bool((byte2 >> 2) & M1);
        mOrderProcessing1VRL = bool((byte2 >> 3) & M1);
        mOrderProcessing2VRL = bool((byte2 >> 4) & M1);
        mBasicDeviceUnioinQuota = bool((byte2 >> 5) & M1);
        mDamageProcessing2VRL = bool((byte2 >> 9) & M1);
        mDamageProcessing2PRL = bool((byte2 >> 10) & M1);
        mDamageProcessing1VRL = bool((byte2 >> 11) & M1);
        mDamageProcessing1PRL = bool((byte2 >> 12) & M1);
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

AccordErrors::ErrorCode Codogram0::generate(QByteArray &result) const
{
    result.clear();
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        write8(stream, 0x00);

        write8(stream, mPackageNumberDiscarded);

        write16(stream, mTargetNumberPRL);

        write16(stream, mTargetNumberUnited);

        write16(stream, mTargetNumberVRL);

        write16(stream, mExternalFailureSyncNumber);

        quint16 byte2 = 0x0000;
        byte2 |= quint8(mOrderProcessing2VRL) << 1;
        byte2 |= quint8(mOrderProcessing2PRL) << 2;
        byte2 |= quint8(mOrderProcessing1VRL) << 3;
        byte2 |= quint8(mOrderProcessing1PRL) << 4;
        byte2 |= quint8(mBasicDeviceUnioinQuota) << 5;
        byte2 |= quint8(mDamageProcessing2VRL) << 9;
        byte2 |= quint8(mDamageProcessing2PRL) << 10;
        byte2 |= quint8(mDamageProcessing1VRL) << 11;
        byte2 |= quint8(mDamageProcessing1PRL) << 12;
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}
