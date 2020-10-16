#include "presentdata.h"
#include "shareutility.h"

PresentData::PresentData()
{
    //nothing
}

PresentData::PresentData(QDataStream& stream)
{
    stream >> *this;
}

bool PresentData::isSet(int bit) const // bit from 1 to 7. 8bit is extansion marker
{
    if (--bit < 0)
        return false;

    int byte = bit / 7;
    if (m_data.size() <= byte)
        return false;

    quint8 mask = 0x80 >> (bit % 7);
    return m_data[byte] & mask;
}

void PresentData::set(int bit)
{
    if (--bit < 0)
        return;

    int byte = bit / 7;
    int appendAmount = byte + 1 - m_data.size();
    if (appendAmount > 0) { //expand the data byte array
        if (m_data.size() > 0)
            m_data[m_data.size() - 1] = m_data[m_data.size() - 1] | 0x01;
        if (--appendAmount > 0)
            m_data.append(QByteArray(appendAmount, '\x01'));
        m_data.append('\x00');
    }

    quint8 mask = 0x80 >> (bit % 7);
    m_data[byte] = m_data[byte] | mask;
}

QDataStream & operator<< (QDataStream& stream, const PresentData& out)
{
    writeArray(stream, out.data());
    return stream;
}

QDataStream & operator>> (QDataStream& stream, PresentData& in)
{
    in.m_data.clear();
    quint8 byte;
    do
    {
        byte = read<quint8>(stream);
        in.m_data.append(byte);
    } while(byte & M1);
    return stream;
}
