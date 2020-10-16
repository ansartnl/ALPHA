#ifndef AEROTECHNICA_ASTERIX001_H
#define AEROTECHNICA_ASTERIX001_H

#include "asterix001.h"

#include <QtCore/QMap>
#include <QtCore/QPair>

class PresentData;

//! Aerotechnica standard of Asterix protocol
namespace Aerotechnica {

//! RADAR TARGET REPORT MESSAGES
/*!
    A1000 generates radar plot reports only. No track reports will be sent.
    Radar plot reports have Category 001 and are assembled according to standard
    Category 001 User Application Profile, as it is defined in ASTERIX.
*/
class ASTERIX_EXPORT Asterix001 : public Asterix001Proto
{
protected:
    virtual void parse_sp(QDataStream &stream, AsterixData &result);
    virtual bool generate_sp(AsterixData &map, QDataStream &stream);

public:
    //! Altitude from SSR national channel data structure
    /*! For garbled Altitude information, zero value will be transmitted
        into bits 14-1, with bit 16 set to “1”.
    */
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

public:
    Asterix001();

    virtual QStringList fields() const;


    // Special Data Item:

    // DATA ITEM I001/142, TIME IN STORAGE
    static const QString SP_TimeInStorage;          // QTime

    // DATA ITEM I001/N01, BOARD NUMBER - SSR NATIONAL CHANNEL
    static const QString SP_BoardNumber;            // quint32,   1 - 99999

    // DATA ITEM I001/N02, ALTITUDE - SSR NATIONAL CHANNEL
    static const QString SP_Altitude;               // SAltitude

    // DATA ITEM I001/N03, FUEL RESERVE - SSR NATIONAL CHANNEL
    static const QString SP_Fuel;                   // quint8, %

    // DATA ITEM I001/N04, EMERGENCY - SSR NATIONAL CHANNEL
    static const QString SP_Emergency_S;            // bool, true - Emergency, false - Default
};

}   // Aerotechnica

Q_DECLARE_METATYPE(Aerotechnica::Asterix001::SAltitude)

#endif // AEROTECHNICA_ASTERIX001_H
