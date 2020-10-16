#include "Compare.h"

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>

namespace Compare
{

bool equal(const QVariant &v1, const QVariant &v2)
{
    QByteArray a1;
    QByteArray a2;
    QDataStream stream1(&a1, QIODevice::WriteOnly);
    QDataStream stream2(&a2, QIODevice::WriteOnly);
    stream1 << v1;
    stream2 << v2;
    return (a1 == a2);
}

bool equal(const QVariantList &v1, const QVariantList &v2)
{
    if (v1.size() != v2.size())
        return false;
    for (int i = 0; i < v1.size(); ++i)
    {
        if (notEqual(v1[i], v2[i]))
            return false;
    }
    return true;
}

bool alike(const QVariantList &v1, const QVariantList &v2)
{
    if (v1.size() != v2.size())
        return false;
    QVariantList l1 = v1;
    QVariantList l2 = v2;
    foreach (const QVariant &v, l1)
    {
        bool contains = false;
        int index = 0;
        for (int i = 0; i < l2.size(); ++i)
        {
            if (equal(v, l2[i]))
            {
                contains = true;
                index = i;
            }
        }
        if (!contains)
            return false;
        l2.removeAt(index);
    }
    return true;
}

bool equal(const QVariantHash &v1, const QVariantHash &v2)
{
    if (v1.size() != v2.size())
        return false;
    foreach (const QString &key, v1.keys())
    {
        if (!v2.contains(key))
            return false;
        if (notAlike(v1.values(key), v2.values(key)))
            return false;
    }
    return true;
}

bool equal(const QVariantMap &v1, const QVariantMap &v2)
{
    if (v1.size() != v2.size())
        return false;
    foreach (const QString &key, v1.keys())
    {
        if (!v2.contains(key))
            return false;
        if (notAlike(v1.values(key), v2.values(key)))
            return false;
    }
    return true;
}

} //namespace
