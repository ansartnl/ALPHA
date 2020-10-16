#ifndef METEOWARNING_H
#define METEOWARNING_H

#include <QtCore/QDataStream>
#include <QtCore/QVariant>

#include "StcaObject.h"

struct MeteoWarning : public StcaObject
{
    //! Meteo zone type
    enum ZoneType
    {
        Unknown     = -1,
        Cloudburst	= 0,    // LVN
        LoStorm		= 1,	// (R)
        MedStorm	= 2,	// R)
        HiStorm		= 3,	// R
        Hail		= 4,	// G
        Squall		= 5, 	// H00
        Snow        = 6,    //S
        Rain        = 7,    //N
        Fog         = 8     //F
    };

    //! Position of a plane relative to zone.
    enum Position
    {
        Inside,
        Vicinity
    };

    MeteoWarning(int bn = 0, ZoneType t = Unknown, quint32 ID = 0, Position pos = Inside);

    int         boardNumber;
    ZoneType    type;
    quint32     id;
    /*! Position of a plane relative to zone */
    Position    position;
};

QDataStream &operator<<(QDataStream &, const MeteoWarning &);
QDataStream &operator>>(QDataStream &, MeteoWarning &);

Q_DECLARE_METATYPE(MeteoWarning)

#endif // METEOWARNING_H
