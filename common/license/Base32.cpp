#include <qmath.h>

#include "Base32.h"

namespace Base32
{

QByteArray encode(const QByteArray &data)
{
    static QByteArray alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

    QByteArray result;
    result.reserve(qCeil(data.size() * 8 / 5.0));

    for (int i = 0; i < data.size(); i += 5)
    {
        quint64 buffer = 0;

        int byteCount = qMin(5, data.size() - i);
        for (int j = 0; j < byteCount; ++j)
        {
            buffer = (buffer << 8) | (quint8)data[i + j];
        }

        for (int bitCount = byteCount * 8; bitCount > 0; bitCount -= 5)
        {
            int index;
            if (bitCount >= 5)
                index = (buffer >> (bitCount - 5)) & 0x1F;
            else
                index = (buffer & (quint64)(0x1F >> (5 - bitCount))) << (5 - bitCount);

            result.append(alphabet[index]);
        }
    }

    return result;
}

}
