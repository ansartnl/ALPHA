#include "XtObject.h"

void Xt::CObject::Serialize(QDataStream& out) const
{
    out << mVersion;
}

inline QDataStream &operator<<(QDataStream &out, const Xt::CObject &object)
{
    object.Serialize(out);
    return out;
}
