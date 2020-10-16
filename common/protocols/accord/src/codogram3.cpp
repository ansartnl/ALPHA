#include "codogram3.h"
#include "accordutility.h"


Codogram3::Codogram3()
    : mChannel1(Domestic), mChannel2(Domestic), mSPI1(0), mSPI2(0), mInformationType(IT_XY),
    mDisaster1(Dis_None), mDisaster2(Dis_None),
    mInformationSource1(IS_PRL), mInformationSource2(IS_PRL),
    mProhibition1(Proh_None), mProhibition2(Proh_None),
    mX1(0), mX2(0), mY1(0), mY2(0), mVRF1(0), mVRF2(0)
{
}

int Codogram3::classCategory()
{
    return 3;
}

QString Codogram3::classEdition()
{
    return "1.0";
}

int Codogram3::category() const
{
    return classCategory();
}

QString Codogram3::edition() const
{
    return classEdition();
}

AccordErrors::ErrorCode Codogram3::parse(QDataStream &stream)
{
    try
    {
        quint8 value;

        quint32 byte4 = read<quint32>(stream);

        quint8 tc = byte4 >> 27;
        bool dc = bool(byte4 >> 23);

        if ( !(tc == 0x02 && dc) )
            throw AccordErrors::Error_InvalidParser;

        {
            value = (byte4 >> 24) & M3;
            testInformationType(value);
            mInformationType = EInformationType(value);

            mSPI1 = (byte4 >> 22) & M1;
            mSPI2 = (byte4 >> 19) & M1;

            mDisaster1 = EDisaster((byte4 >> 20) & M2);
            mDisaster1 = EDisaster((byte4 >> 17) & M2);

            value = (byte4 >> 13) & M3;
            testInformationSource(value);
            mInformationSource1 = EInformationSource(value);

            mX1 = parseCoordinate( quint16(byte4) );
        }

        quint16 byte2 = read<quint16>(stream);
        {
            mChannel1 = EChannel((byte2 >> 15) & M1);

            value = (byte2 >> 13) & M2;
            testProhibition(value);
            mProhibition1 = EProhibition(value);

            mY1 = parseCoordinate(byte2);
        }

        byte2 = read<quint16>(stream);
        {
            value = (byte2 >> 13) & M2;
            testInformationSource(value);
            mInformationSource2 = EInformationSource(value);

            mX2 = parseCoordinate(byte2);
        }

        mVRF1 = read<quint8>(stream) * 100;  // 1 = 100 ms
        mVRF2 = read<quint8>(stream) * 100;  // 1 = 100 ms

        byte2 = read<quint16>(stream);
        {
            mChannel2 = EChannel((byte2 >> 15) & M1);

            value = (byte2 >> 13) & M2;
            testProhibition(value);
            mProhibition2 = EProhibition(value);

            mY2 = parseCoordinate(byte2);
        }
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

AccordErrors::ErrorCode Codogram3::generate(QByteArray &result) const
{
    result.clear();
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        quint8 value;

        quint32 byte4 = 0;
        {
            byte4 |= 0x00000002 << 27;  // ТС
            byte4 |= 1 << 23;           // ДС - Сообщение о  координатах двух  ВС

            value = quint8(mInformationType);
            testInformationType( value );
            byte4 |= quint32(value) << 24;

            byte4 |= quint32(mSPI1 & M1) << 22;

            byte4 |= quint32(quint32(mDisaster1) & M2) << 20;

            byte4 |= quint32(mSPI2 & M1) << 19;

            byte4 |= quint32(quint32(mDisaster2) & M2) << 17;

            value = quint8(mInformationSource1);
            testInformationSource( value );
            byte4 |= quint16(value) << 13;

            byte4 |= quint32( generateCoordinate(mX1) );
        }
        write32(stream, byte4);

        quint16 byte2 = 0;
        {
            byte2 |= quint16(mChannel1) << 15;

            value = quint8(mProhibition1);
            testProhibition( value );
            byte2 |= quint16(value) << 13;

            byte2 |= generateCoordinate(mY1);
        }
        write16(stream, byte2);

        byte2 = 0;
        {
            value = quint8(mInformationSource2);
            testInformationSource( value );
            byte2 |= quint16(value) << 13;

            byte2 |= generateCoordinate(mX2);
        }
        write16(stream, byte2);

        write8(stream, mVRF1 / 100);
        write8(stream, mVRF2 / 100);

        byte2 = 0;
        {
            byte2 |= quint16(mChannel2) << 15;

            value = quint8(mProhibition2);
            testProhibition( value );
            byte2 |= quint16(value) << 13;

            byte2 |= generateCoordinate(mY2);
        }
        write16(stream, byte2);
    }
    catch (AccordErrors::ErrorCode error)
    {
        return error;
    }
    return AccordErrors::OK;
}

void Codogram3::testInformationType(quint8 value) const
{
    if ( !CodogramPlane::testInformationType(value) )
        throw AccordErrors::Error_3_InformationType_IncorrectValue;
}

void Codogram3::testProhibition(quint8 value) const
{
    if ( !CodogramPlane::testProhibition(value) )
        throw AccordErrors::Error_3_Prohibition_IncorrectValue;
}

void Codogram3::testInformationSource(quint8 value) const
{
    if ( !CodogramPlane::testInformationSource(value) )
        throw AccordErrors::Error_3_InformationSource_IncorrectValue;
}
