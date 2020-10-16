#ifndef HARDWAREID_H
#define HARDWAREID_H

#include <QtCore/QString>
#include <QtCore/QMap>

class HardwareId
{
public:
    enum HardwareType
    {
        Board = 0,
        Drive,
        Ethernet,
        Memory
    };

    HardwareId();
    HardwareId(const QByteArray &data);

    bool setId(HardwareType type, QString id);
    bool isValid() const;
    bool verify(const HardwareId &currentId) const;
    void print() const;

    QByteArray toRegKey() const;

    QByteArray toByteArray() const;
    void fromByteArray(const QByteArray &data);

private:
    QMap<HardwareType, QByteArray> id;
};

QDataStream &operator>>(QDataStream &stream, HardwareId::HardwareType &type);

#endif // HARDWAREID_H
