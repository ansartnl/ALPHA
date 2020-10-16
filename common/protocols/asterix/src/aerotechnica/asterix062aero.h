#ifndef AEROTECHNICA_ASTERIX062_H
#define AEROTECHNICA_ASTERIX062_H

#include "asterix062.h"

#include <QtCore/QMap>
#include <QtCore/QPair>

class PresentData;

//! Aerotechnica standard of Asterix protocol
namespace Aerotechnica {

class ASTERIX_EXPORT Asterix062 : public Asterix062Proto
{
protected:
    virtual void parse_sp(QDataStream &stream, AsterixData &result);
    virtual bool generate_sp(AsterixData &map, QDataStream &stream);

public:
    //! Altitude from SSR national channel data structure
    struct SAltitude
    {
        SAltitude() : code(0) {}

        enum {
            V   = 0x8000,   //!< 0 - Altitude validated; 1 - Altitude not validated
            A   = 0x4000    //!< 0 - Absolute altitude; 1 - Relative altitude
        };

        double altitude() const { return 25.*(code & 0x3FFF); }             // feet
        void setAltitude(double a) { code |= quint16(qRound(a / 25.)); }    // feet

    public:
        quint16     code;   /** Altitude (Mode S Altitude) converted into binary two's complement representation */
    };

    //! Valid status structure
    struct SValidStatus
    {
        SValidStatus() : isBoardNumberValid(false), isHeightValid(false)
        {}

        bool    isBoardNumberValid;
        bool    isHeightValid;
    };

public:
    Asterix062();

    virtual QStringList fields() const;


    // Special Data Item:

    static const QString SP_BoardNumber;            // quint32,   1 - 99999

    static const QString SP_Altitude;               // SAltitude

    static const QString SP_Fuel;                   // quint8, %

    static const QString SP_FplStatus;              // quint8

    static const QString SP_HeightQNH;              // qint16, feet

    //! Время локации (фактическое)
    static const QString SP_BRF;                    // quint8

    static const QString SP_Emergency;              // quint8: 0, 1

    static const QString SP_ValidStatus;            // SValidStatus
};

}   // Aerotechnica

Q_DECLARE_METATYPE(Aerotechnica::Asterix062::SAltitude)
Q_DECLARE_METATYPE(Aerotechnica::Asterix062::SValidStatus)

#endif // AEROTECHNICA_ASTERIX062_H
