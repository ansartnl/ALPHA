#ifndef RESTRICTIONWARNING_H
#define RESTRICTIONWARNING_H

#include "StcaObject.h"

#include <QtCore/QDataStream>
#include <QtCore/QVariant>

struct RestrictionWarning : public StcaObject
{
    //! Restriction zone type
    enum ZoneType
    {
        Alert = 'A', //!< Alert
        Caution = 'C', //!< Caution
        Danger = 'D', //!< Danger
        Military = 'M', //!< Military
        Prohibited = 'P', //!< Prohibited
        Restricted = 'R', //!< Restricted
        Training = 'T', //!< Training
        Warning = 'W', //!< Warning
        User = 'X', //!< User defined restriction zone
        UserObstacle = 'Y', //!< User defined obstacle
        Unknown = 'U' //!< Unknown
    };

    //! Position of a plane relative to zone.
    enum Position
    {
        Inside,
        Vicinity
    };

    RestrictionWarning(int bn = 0, ZoneType t = Unknown, quint32 ID = 0,
                       Position pos = Inside);

    int         boardNumber;
    ZoneType    type;
    quint32     id;
    /*! Position of a plane relative to zone */
    Position    position;
};

QDataStream &operator<<(QDataStream &, const RestrictionWarning &);
QDataStream &operator>>(QDataStream &, RestrictionWarning &);

Q_DECLARE_METATYPE(RestrictionWarning)

#endif // RESTRICTIONWARNING_H
