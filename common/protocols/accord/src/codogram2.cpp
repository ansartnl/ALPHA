#include "codogram2.h"
#include "accordutility.h"


Codogram2::Codogram2()
    : mChannel(Domestic), mInformationType(IT_XY), mSPI(0), mDisaster(Dis_None),
    mAltitudeLevel(Absolute), mDisasterDomesticChannel(true), mMinumumFuel(MinFuel_Enough),
    mProhibition(Proh_None), mInformationSource(IS_PRL), mX(0), mY(0),
    mBarometricAltitude(0), mBoardNumber(0), mVRF(0)
{
}

int Codogram2::classCategory()
{
    return 2;
}

QString Codogram2::classEdition()
{
    return "1.0";
}

int Codogram2::category() const
{
    return classCategory();
}

QString Codogram2::edition() const
{
    return classEdition();
}

AccordErrors::ErrorCode Codogram2::parse(QDataStream &stream)
{
    try
    {
        quint8 value;
        quint32 value4;

        quint32 byte4 = read<quint32>(stream);

        quint8 tc = byte4 >> 27;
        bool dc = bool(byte4 >> 23);

        if ( !(tc == 0x02 && !dc) )
            throw AccordErrors::Error_InvalidParser;

        {
            value = (byte4 >> 24) & M3;
            testInformationType(value);
            mInformationType = EInformationType(value);

            mSPI = (byte4 >> 22) & M1;

            mDisaster = EDisaster((byte4 >> 20) & M2);

            mBarometricAltitude = ((byte4 >> 6) & M14) * 10;

            mAltitudeLevel = EAltitudeLevel((byte4 >> 5) & M1);

            mDisasterDomesticChannel = bool((byte4 >> 4) & M1);

            mMinumumFuel = EMinimumFuel(byte4 & M4);
        }

        quint16 byte2 = read<quint16>(stream);
        {
            value = (byte2 >> 13) & M2;
            testProhibition(value);
            mProhibition = EProhibition(value);

            mY = parseCoordinate(byte2);

            mChannel = EChannel(byte2 >> 15 & M1);
        }

        byte2 = read<quint16>(stream);
        {
            value = (byte2 >> 13) & M3;
            testInformationSource(value);
            mInformationSource = EInformationSource(value);

            mX = parseCoordinate(byte2);
        }

        byte4 = read<quint32>(stream);
        {
            mVRF = (byte4 >> 24) * 100;  // 1 = 100 ms

            value4 = byte4 & M20;
            testBoardNumber(value4);
            mBoardNumber = value4;
        }
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

AccordErrors::ErrorCode Codogram2::generate(QByteArray &result) const
{
    result.clear();
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        quint8 value;

        quint32 byte4 = 0;
        {
            byte4 |= 0x00000002 << 27;

            byte4 |= quint32(mMinumumFuel) & M4;

            byte4 |= quint32(mDisasterDomesticChannel) << 4;

            byte4 |= quint32(quint32(mAltitudeLevel) & M1) << 5;

            byte4 |= quint32((mBarometricAltitude / 10) & M14) << 5;

            byte4 |= quint32(mDisaster) << 20;

            byte4 |= quint32(mSPI & M1) << 22;

            value = quint8(mInformationType);
            testInformationType( value );
            byte4 |= quint32(value) << 24;
        }
        write32(stream, byte4);

        quint16 byte2 = 0;
        {
            byte2 |= generateCoordinate(mY);

            value = quint8(mProhibition);
            testProhibition( value );
            byte2 |= quint16(value) << 13;

            if (mChannel == Domestic)
                byte2 |= 0x8000;
        }
        write16(stream, byte2);

        byte2 = 0;
        {
            byte2 |= generateCoordinate(mX);

            value = quint8(mInformationSource);
            testInformationSource( value );
            byte2 |= quint16(value) << 13;
        }
        write16(stream, byte2);

        byte4 = 0;
        {
            byte4 |= quint32(mVRF / 100) << 24;

            testBoardNumber( mBoardNumber );
            byte4 |= mBoardNumber;
        }
        write32(stream, byte4);

    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

void Codogram2::testInformationType(quint8 value) const
{
    if ( !CodogramPlane::testInformationType(value) )
        throw AccordErrors::Error_2_InformationType_IncorrectValue;
}

void Codogram2::testProhibition(quint8 value) const
{
    if ( !CodogramPlane::testProhibition(value) )
        throw AccordErrors::Error_2_Prohibition_IncorrectValue;
}

void Codogram2::testInformationSource(quint8 value) const
{
    if ( !CodogramPlane::testInformationSource(value) )
        throw AccordErrors::Error_2_InformationSource_IncorrectValue;
}

void Codogram2::testBoardNumber(quint32 value) const
{
    if ( (mChannel == International && (value < 1 || value > 7777)) ||
         (mChannel == Domestic && (value < 1 || value > 99999)) )
        throw AccordErrors::Error_2_BoardNumber_IncorrectValue;
}
