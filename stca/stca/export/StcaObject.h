#ifndef STCAOBJECT_H
#define STCAOBJECT_H

#include <QtCore/QDataStream>
#include <QtCore/QVariant>

struct StcaObject
{
    /** Unique object identifier */
    QString     identifier;
};

QDataStream &operator<<(QDataStream &, const StcaObject &);
QDataStream &operator>>(QDataStream &, StcaObject &);

#endif // STCAOBJECT_H
