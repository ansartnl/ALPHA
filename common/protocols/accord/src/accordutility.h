#ifndef ACCORDUTILITY_H
#define ACCORDUTILITY_H

#include "accorderrors.h"
#include "accord.h"

#include <QtCore/QDataStream>

enum BinaryMask
{
    M1  = 0x00000001,
    M2  = 0x00000003,
    M3  = 0x00000007,
    M4  = 0x0000000F,
    M5  = 0x0000001F,
    M6  = 0x0000003F,
    M7  = 0x0000007F,
    M8  = 0x000000FF,
    M9  = 0x000001FF,
    M10 = 0x000003FF,
    M11 = 0x000007FF,
    M12 = 0x00000FFF,
    M13 = 0x00001FFF,
    M14 = 0x00003FFF,
    M15 = 0x00007FFF,
    M16 = 0x0000FFFF,
    M17 = 0x0001FFFF,
    M18 = 0x0003FFFF,
    M19 = 0x0007FFFF,
    M20 = 0x000FFFFF,
    M21 = 0x001FFFFF,
    M22 = 0x003FFFFF,
    M23 = 0x007FFFFF,
    M24 = 0x00FFFFFF,
    M25 = 0x01FFFFFF,
    M26 = 0x03FFFFFF,
    M27 = 0x07FFFFFF,
    M28 = 0x0FFFFFFF,
    M29 = 0x1FFFFFFF,
    M30 = 0x3FFFFFFF,
    M31 = 0x7FFFFFFF,
    M32 = 0xFFFFFFFF
};

template<typename  T>
inline T read(QDataStream& stream)
{
    T result;
    stream >> result;
    if (QDataStream::Ok != stream.status())
        throw AccordErrors::Error_UnexpectedEndOfData;
    return result;
}

template<>
inline QByteArray read(QDataStream& stream)
{
    Q_UNUSED(stream);
    Q_ASSERT(false);
    return QByteArray();
}

inline quint32 read24(QDataStream& stream)
{
    return quint32(read<quint16>(stream)) << 8 | read<quint8>(stream);
}

inline qint32 read24s(QDataStream& stream)
{
    return qint32(read<qint16>(stream)) << 8 | read<quint8>(stream);
}

template<typename T>
inline void write8(QDataStream& stream, T value)
{
    stream << quint8(value);
}

template<typename T>
inline void write16(QDataStream& stream, T value)
{
    stream << quint16(value);
}

template<typename T>
inline void write24(QDataStream& stream, T value)
{
    qint32 data(value);
    stream << quint16(data >> 8) << quint8(data);
}

template<typename T>
inline void write32(QDataStream& stream, T value)
{
    stream << quint32(value);
}

template<typename T>
inline void write64(QDataStream& stream, T value)
{
    stream << quint64(value);
}

inline void writeArray(QDataStream& stream, const QByteArray &data)
{
    stream.writeRawData(data.data(), data.size());
}

inline QByteArray readArray(QDataStream& stream, int length)
{
    QByteArray result;
    result.resize(length);
    if (length != stream.readRawData(result.data(), length))
        throw AccordErrors::Error_UnexpectedEndOfData;
    return result;
}

template<typename T>
inline T restoreSign(T source, unsigned int bits) // bits starts from 1
{
    Q_ASSERT(sizeof(T) * 8 >= bits && bits != 0);
    if (sizeof(T) * 8 == bits)
        return source;

    T mask = 1 << (bits - 1);
    if (source & mask) // minus
    {
        mask = (mask << 1) - 1;
        mask = ~mask;
        return source | mask;
    }
    else // plus
    {
        mask = (mask << 1) - 1;
        return source & mask;
    }
}

inline QByteArray readCharacters(QDataStream& stream, int length)
{
    QByteArray result;
    for (int i = 0; i < length; ++i)
    {
        quint32 data = read24(stream);
        result.push_back(data >> 18 & M6);
        result.push_back(data >> 12 & M6);
        result.push_back(data >> 6 & M6);
        result.push_back(data & M6);
    }
    return result;
}

inline void writeCharacters(QDataStream& stream, const QByteArray &arr)
{
    int i = 0;
    while (i + 4 <= arr.size())
    {
        quint32 data = (arr[i++] & M6) << 18;
        data |= (arr[i++] & M6) << 12;
        data |= (arr[i++] & M6) << 6;
        data |= arr[i++] & M6;
        write24(stream, data);
    }
}

#endif // ACCORDUTILITY_H
