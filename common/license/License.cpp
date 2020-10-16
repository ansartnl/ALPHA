#include <QtCore/QCryptographicHash>

#include "Base32.h"
#include "License.h"

namespace License
{

QByteArray generateLicKey(const QByteArray &regKey, const QByteArray &key)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(regKey);
    hash.addData(key);

    QByteArray data = Base32::encode(hash.result());
    QByteArray result;
    for (int i = 0; i < 15; ++i)
    {
        result.append(data[i * 2]);
    }
    return result;
}

bool verifyLicense(const QByteArray &regKey, const QByteArray &licKey, const QByteArray &key)
{
    if (regKey.isEmpty() || licKey.isEmpty())
        return false;

    return licKey == generateLicKey(regKey, key);
}

}
