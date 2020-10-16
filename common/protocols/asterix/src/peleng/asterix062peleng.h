#ifndef ASTERIX062PELENG_H
#define ASTERIX062PELENG_H

#include "asterix062.h"

namespace Peleng {

class ASTERIX_EXPORT Asterix062 : public Asterix062Proto
{
public:
    struct BoardNumber
    {
        bool V;
        bool G;
        bool L;
        quint32 number;
    };

    struct FuelHeight
    {
        bool A;
        bool B;
        quint8 fuel;

        bool V;
        bool G;
        qint16 height;

    };

    struct Warning
    {
        quint8 type;
        QList<quint16> tracks;
        quint8 time;
        quint16 distance;
    };

    Asterix062();
    QStringList fields() const;

    static const QString SP_BoardNumber;
    static const QString SP_FuelHeight;
    static const QString SP_Warning;


protected:
    void parse_sp(QDataStream &stream, AsterixData &map);
    bool generate_sp(AsterixData &map, QDataStream &stream);
};

}

Q_DECLARE_METATYPE(Peleng::Asterix062::BoardNumber)
Q_DECLARE_METATYPE(Peleng::Asterix062::FuelHeight)
Q_DECLARE_METATYPE(Peleng::Asterix062::Warning)

#endif // ASTERIX062PELENG_H
