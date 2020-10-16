#ifndef BASE32_H
#define BASE32_H

#include <QtCore/QByteArray>

namespace Base32
{
QByteArray encode(const QByteArray &data);
}


#endif // BASE32_H
