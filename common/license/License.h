#ifndef LICENSE_H
#define LICENSE_H

#include <QtCore/QByteArray>

namespace License
{
QByteArray generateLicKey(const QByteArray &regKey, const QByteArray &key);
bool verifyLicense(const QByteArray &regKey, const QByteArray &licKey, const QByteArray &key);
}


#endif // LICENSE_H
