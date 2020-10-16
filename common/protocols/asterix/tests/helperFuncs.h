#include <QtCore/QString>
#include <QtCore/QByteArray>

#include <time.h>

static bool testChance = (qsrand(time(0)), qrand() % 100 <= 50);

inline QString translate(const QByteArray& array)
{
    QString result;
    for (int i = 0; i < array.size(); ++i)
        result.append(QString("%1 ").arg(int(array[i]), 2, 16, QChar('0')).right(3));
    return result;
}

inline quint32 rnd()
{
    quint32 result = qrand() ^ qrand();
    result <<= 16;
    result |= qrand() ^ qrand();
    return result;
}

inline qint32 rndS(int i = 32)
{
    if (i >= 31)
        return rnd();

    quint32 mask = (0x01 << (i - 1)) - 1;
    qint32 result = rnd() & mask;
    return result * (rnd() % 2 ? -1 : 1);
}

inline quint32 rndU(int i = 32)
{
    if (i >= 32)
        return rnd();

    quint32 mask = (0x01 << i) - 1;
    return rnd() & mask;
}

inline QByteArray rndArray(int size)
{
    QByteArray result;
    for (int i = 0; i < size; ++i)
        result.push_back(rnd());
    return result;
}

inline bool myFuzzyCompare(double p1, double p2)
{
    return (qAbs(p1 - p2) <= 0.01 * (qAbs(p1) + qAbs(p2)));
}
