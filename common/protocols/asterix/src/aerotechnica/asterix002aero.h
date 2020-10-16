#ifndef AEROTECHNICA_ASTERIX002_H
#define AEROTECHNICA_ASTERIX002_H

#include "asterix002.h"

#include <QtCore/QMap>
#include <QtCore/QPair>

class PresentData;

//! Aerotechnica standard of Asterix protocol
namespace Aerotechnica {

//! RADAR SERVICE MESSAGES
/*!
    Radar Service Messages, used in A1000 output protocol, are transmitted
    as ASTERIX Category 002 Messages. Only North marker messages and Sector
    marker messages are applicable. Standard UAP is used for message assembling.
*/
class ASTERIX_EXPORT Asterix002 : public Asterix002Proto
{
    virtual void parse_050(QDataStream &stream, AsterixData &result);
    virtual void parse_060(QDataStream &stream, AsterixData &result);

    virtual bool generate_050(AsterixData &map, QDataStream &stream);
    virtual bool generate_060(AsterixData &map, QDataStream &stream);

protected:
    virtual void parse_sp(QDataStream &stream, AsterixData &result);
    virtual bool generate_sp(AsterixData &map, QDataStream &stream);

public:
    //! Station configuration status data structure
    /*!
        ERROR CODE 000	no Adapter errors detected
    */
    struct SStationConfigStatus
    {
        SStationConfigStatus() : part(0) {}

        enum
        {
            AC = 0x80,  /** number of on-line chain of A1000 Extractor:
                            0 – chain 1 is active; 1 – chain 2 is active */
            I = 0x10    /** Adapter/Module Status:
                            0 - Adapter/Module installed; 1 - Adapter/Module not installed */
        };

        enum C
        {
            Active = 1, //!< chain is active
            Hot,        //!< chain is “hot” standby
            Off         //!< chain is off / not present
        };

        quint8 c1() const { return (part >> 5) & 0x03; }
        quint8 c2() const { return (part >> 3) & 0x03; }
        quint8 type() const { Q_ASSERT(!extents.empty()); return (extents[0] >> 5) & 0x07; }
        quint8 errorCode() const { Q_ASSERT(!extents.empty()); return (extents[0] >> 1) & 0x07; }

        void setC1(quint8 v) { part |= (v & 0x03) << 5; }
        void setC2(quint8 v) { part |= (v & 0x03) << 3; }
        void setType(quint8 v) { if (extents.empty()) extents.append(0); extents[0] |= (v & 0x07) << 5; }
        void setErrorCode(quint8 v) { if (extents.empty()) extents.append(0); extents[0] |= (v & 0x07) << 1; }

    public:
        quint8 part;                /** First part byte */
        QList<quint8>   extents;    /** Extent bytes */
    };

    //! Station processing mode data structure
    struct SStationProcessingMode
    {
        SStationProcessingMode() : part(0) {}

        enum
        {
            A = 0x80,   //!< 0/1 Mode A OFF/ON
            B = 0x40,   //!< 0/1 Mode B OFF/ON
            C = 0x20    //!< 0/1 Mode C OFF/ON
        };

    public:
        quint8 part;
    };

    //! North SDI data structure
    struct SNorthSDI
    {
        SNorthSDI() : channels(0) {}

        // ADF channel
        // aC-al - 0/1  ADF channel 12-1 OFF/ON
        // fC-f1 - 0/1	ADF channel 12-1 OK/Failure
        enum
        {
            f1 = 0x00000001,
            f2 = 0x00000002,
            f3 = 0x00000004,
            f4 = 0x00000008,
            f5 = 0x00000010,
            f6 = 0x00000020,
            f7 = 0x00000040,
            f8 = 0x00000080,
            f9 = 0x00000100,
            fA = 0x00000200,
            fB = 0x00000400,
            fC = 0x00000800,
            a1 = 0x00001000,
            a2 = 0x00002000,
            a3 = 0x00004000,
            a4 = 0x00008000,
            a5 = 0x00010000,
            a6 = 0x00020000,
            a7 = 0x00040000,
            a8 = 0x00080000,
            a9 = 0x00100000,
            aA = 0x00200000,
            aB = 0x00400000,
            aC = 0x00800000
        };

    public:
        quint32 channels;
    };

public:
    Asterix002();

    virtual QStringList fields() const;


    // Special Data Item:

    // DATA ITEM I002/N01, NORTH SDI
    static const QString SP_TimeInStorage;          // QTime

    // DATA ITEM I002/N02, SECTOR SDI
    static const QString SP_ADF;                    // SNorthSDI
};

}   // Aerotechnica

Q_DECLARE_METATYPE(Aerotechnica::Asterix002::SStationConfigStatus)
Q_DECLARE_METATYPE(Aerotechnica::Asterix002::SStationProcessingMode)
Q_DECLARE_METATYPE(Aerotechnica::Asterix002::SNorthSDI)

#endif // AEROTECHNICA_ASTERIX002_H
