#include "LargeData.h"

#include <QtCore/QBuffer>
#include <QtCore/QDataStream>

static int i01 = qRegisterMetaTypeStreamOperators< LargeData::DataPacket >();

QDataStream & operator<< (QDataStream &stream, const LargeData::DataPacket &data)
{
    stream.writeRawData(data.header.signature, LargeData::HEADER_SIZE);
    stream << data.header.identifier;
    stream << data.header.number;
    stream << data.header.count;
    stream << data.header.size;
    stream.writeRawData(data.data, data.header.size);

    return stream;
}

QDataStream & operator>> (QDataStream &stream, LargeData::DataPacket &data)
{
    char *b = new char[LargeData::MAX_DATA_SIZE];
    uint len = LargeData::HEADER_SIZE;
    stream.readRawData(b, len);
    memcpy(data.header.signature, b, LargeData::HEADER_SIZE);

    stream >> data.header.identifier;
    stream >> data.header.number;
    stream >> data.header.count;
    stream >> data.header.size;

    len = data.header.size;
    stream.readRawData(b, len);
    memcpy(data.data, b, data.header.size);

    delete[] b;

    return stream;
}

namespace LargeData
{

QByteArray toByteArray(const LargeData::DataPacket &data)
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << data;
    return result;
}

LargeData::DataPacket fromByteArray(const QByteArray &arr)
{
    LargeData::DataPacket result;
    QBuffer buf;
    buf.setData(arr);
    buf.open(QBuffer::ReadOnly);
    QDataStream stream(&buf);
    stream >> result;
    buf.close();
    return result;
}

int packetSize(const QByteArray &arr)
{
    LargeData::DataPacket result;
    QBuffer buf;
    buf.setData(arr);
    buf.open(QBuffer::ReadOnly);
    QDataStream stream(&buf);

    char *b = new char[8];
    uint len = LargeData::HEADER_SIZE;
    stream.readRawData(b, len);

    stream >> result.header.identifier;
    stream >> result.header.number;
    stream >> result.header.count;
    stream >> result.header.size;

    buf.close();

    delete[] b;

    return result.header.size + sizeof(result.header);
}

bool fullPacket(const QByteArray &arr)
{
    return (arr.size() >= packetSize(arr));
}

} // namespace LargeData
