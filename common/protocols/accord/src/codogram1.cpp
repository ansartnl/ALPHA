#include "codogram1.h"
#include "accordutility.h"


Codogram1::Codogram1()
    : mA1(false), mA2(false), mPackageNumberDiscarded(0),
    mCO(CO_Ready), mCB(CB_Ready), mCP(CP_Ready), mBI(BI_1),
    mTransferAPD1(false), mTransferAPD2(false), mReceiveAPD1(false), mReceiveAPD2(false),
    mFunctionBPRD(false), mRK(RK_Repeated), mI1(false), mI2(false), mDmax(Km250),
    mMI(MI_On), mZV(ZV_ModeA), mVPK(Channel_1), mVRF(0), mVP(false), mVV(false),
    mVO(false), mCKP(Channel_1)
{
}

int Codogram1::classCategory()
{
    return 1;
}

QString Codogram1::classEdition()
{
    return "1.0";
}

int Codogram1::category() const
{
    return classCategory();
}

QString Codogram1::edition() const
{
    return classEdition();
}

AccordErrors::ErrorCode Codogram1::parse(QDataStream &stream)
{
    try
    {
        quint8 value;
        quint16 value2;

        quint8 byte = read<quint8>(stream);
        if ( quint8(byte >> 3) != 1 )
            throw AccordErrors::Error_InvalidParser;

        mA2 = bool(quint8(byte >> 2) == 1);
        mA1 = bool(quint8(byte >> 1) == 1);

        mPackageNumberDiscarded = read<quint8>(stream);

        byte = read<quint8>(stream);
        {
            // CO
            mCO = ECO(byte >> 7 & M1);

            // CB
            value = byte >> 5;
            testCB(value);
            mCB = ECB(value);

            // СП
            value = byte >> 2;
            testCP(value);
            mCP = ECP(value);
        }

        byte = (read<quint8>(stream) >> 1) & M7;
        testBI(byte);
        mBI = EBI(value);

        byte = read<quint8>(stream);
        {
            value = byte & M2;
            testRK(value);
            mRK = EModeRK(value);

            mTransferAPD1 = bool((byte >> 7) & M1);
            mTransferAPD2 = bool((byte >> 6) & M1);
            mReceiveAPD1 = bool((byte >> 5) & M1);
            mReceiveAPD2 = bool((byte >> 4) & M1);
            mFunctionBPRD = bool((byte >> 3) & M1);
        }

        byte = read<quint8>(stream);
        {
            value = byte & M2;
            testMI(value);
            mMI = EMeteoInfo(value);

            value = (byte >> 2) & M3;
            testDmax(value);
            mDmax = EDistanceMax(value);

            mI1 = bool((byte >> 6) & M1);
            mI2 = bool((byte >> 7) & M1);
        }

        quint16 byte2 = read<quint16>(stream);
        {
            value2 = byte2 & M12;
            testVPK(value2);
            mVPK = EChannel(value2);

            value2 = byte2 >> 12;
            testZV(value2);
            mZV = EModeRequestVRL(value2);
        }

        mVRF = read<quint8>(stream) * 100;  // 1 = 100 ms

        byte = read<quint8>(stream);
        {
            mVO = bool((byte >> 4) & M1);
            mVV = bool((byte >> 5) & M1);
            mVP = bool((byte >> 6) & M1);
        }

        value2 = read<quint16>(stream) & M12;
        testCKP( value2 );
        mCKP = EChannel(value2);
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

AccordErrors::ErrorCode Codogram1::generate(QByteArray &result) const
{
    result.clear();
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        quint8 value;
        quint16 value2;

        quint8 byte = 0x00;
        byte |= 0x01 << 3;
        {
            byte |= quint8(mA1) << 1;
            byte |= quint8(mA1) << 2;
        }
        write8(stream, byte);

        write8(stream, mPackageNumberDiscarded);

        byte = 0x00;
        {
            value = mCP;
            testCP( value );
            byte |= value << 2;

            value = mCB;
            testCB( value );
            byte |= value << 5;

            byte |= quint8(mCO) << 7;
        }
        write8(stream, byte);

        value = mBI;
        testBI( value );
        write8(stream, value << 1);

        byte = 0x00;
        {
            value = mRK;
            testRK( value );
            byte |= value;

            byte |= quint8(mTransferAPD1) << 7;
            byte |= quint8(mTransferAPD2) << 6;
            byte |= quint8(mReceiveAPD1) << 5;
            byte |= quint8(mReceiveAPD2) << 4;
            byte |= quint8(mFunctionBPRD) << 3;
        }
        write8(stream, byte);

        byte = 0x00;
        {
            value = mMI;
            testMI( value );
            byte |= value;

            value = mDmax;
            testDmax( value );
            byte |= value << 2;

            byte |= quint8(mI1) << 6;
            byte |= quint8(mI2) << 7;
        }
        write8(stream, byte);

        quint16 byte2 = 0x0000;
        {
            value2 = mVPK;
            testVPK( value2 );
            byte |= value2;

            value2 = mZV;
            testZV( value2 );
            byte |= value2 << 12;
        }
        write16(stream, byte2);

        write8(stream, mVRF / 100);

        byte = 0x00;
        {
            byte |= quint8(mVO) << 4;
            byte |= quint8(mVV) << 5;
            byte |= quint8(mVP) << 6;
        }
        write8(stream, byte);

        value2 = mCKP;
        testCKP( value2 );
        write16(stream, value2);
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

void Codogram1::testCB(quint8 value) const
{
    if ( value == 0 )
        throw AccordErrors::Error_1_CB_IncorrectValue;
}

void Codogram1::testCP(quint8 value) const
{
    if ( value != quint8(CP_Ready) && value != quint8(CP_Reserve) && value != quint8(CP_FaultPartly) &&
        value != quint8(CP_FaultPartlyReserve) && value != quint8(CP_Refuse) )
        throw AccordErrors::Error_1_CP_IncorrectValue;
}

void Codogram1::testBI(quint8 value) const
{
    if ( value != quint8(BI_1) && value != quint8(BI_2) && value != quint8(BI_3) &&
        value != quint8(BI_4) && value != quint8(BI_5) )
        throw AccordErrors::Error_1_BI_IncorrectValue;
}

void Codogram1::testRK(quint8 value) const
{
    if ( value != quint8(RK_Repeated) && value != quint8(RK_Distributed) )
        throw AccordErrors::Error_1_RK_IncorrectValue;
}

void Codogram1::testMI(quint8 value) const
{
    if ( value != quint8(MI_On) && value != quint8(MI_Off) )
        throw AccordErrors::Error_1_MI_IncorrectValue;
}

void Codogram1::testDmax(quint8 value) const
{
    if ( value != quint8(Km250) && value != quint8(Km300) && value != quint8(Km350)
        && value != quint8(Km400) )
        throw AccordErrors::Error_1_Dmax_IncorrectValue;
}

void Codogram1::testVPK(quint16 value) const
{
    if ( value != quint8(Channel_1) && value != quint8(Channel_2) && value != quint8(Channel_3)
        && value != quint8(Channel_4) && value != quint8(Channel_5) && value != quint8(Channel_6)
        && value != quint8(Channel_7) && value != quint8(Channel_8) && value != quint8(Channel_9)
        && value != quint8(Channel_10) && value != quint8(Channel_11) && value != quint8(Channel_12) )
        throw AccordErrors::Error_1_VPK_IncorrectValue;
}

void Codogram1::testZV(quint8 value) const
{
    if ( value != quint8(ZV_ModeA) && value != quint8(ZV_ModeB) && value != quint8(ZV_ModeC) )
        throw AccordErrors::Error_1_ZV_IncorrectValue;
}

void Codogram1::testCKP(quint16 value) const
{
    if ( value != quint8(Channel_1) && value != quint8(Channel_2) && value != quint8(Channel_3)
        && value != quint8(Channel_4) && value != quint8(Channel_5) && value != quint8(Channel_6)
        && value != quint8(Channel_7) && value != quint8(Channel_8) && value != quint8(Channel_9)
        && value != quint8(Channel_10) && value != quint8(Channel_11) && value != quint8(Channel_12)
        && value != quint8(Channel_Refuse) )
        throw AccordErrors::Error_1_CKP_IncorrectValue;
}
