#ifndef AEROTECHNICA_ASTERIX253_H
#define AEROTECHNICA_ASTERIX253_H

#include "asterix.h"

#include <QtCore/QMap>
#include <QtCore/QPair>


//! Aerotechnica standard of Asterix protocol
namespace Aerotechnica {

//! DIRECTION TO TARGET MESSAGES. Non-standard ASTERIX data category 250.
/*!
    When A1000 processes information from Automatic Direction Finder (ADF),
    it is possible to transmit the Direction to Target Messages. A1000 uses
    Category 253 for these messages.

    ADF Channels Control Message is used by interfacing RDP System to open or
    close the output from A1000 of “Direction to Target” Messages for each ADF
    channel. When A1000 receives “Switch OFF” message for ADF Channel N (1-12),
    the “Direction to Target” messages for this ADF Channel will NOT be transmitted
    to the outside, even if they are detected. After receiving of “Switch ON”
    message for this ADF Channel, the transmitting of “Direction to Target”
    messages will be resumed.
*/
class ASTERIX_EXPORT Asterix253 : public Asterix
{
    void parse_N01(QDataStream &stream, AsterixData &result);
    void parse_N02(QDataStream &stream, AsterixData &result);
    void parse_N03(QDataStream &stream, AsterixData &result);
    void parse_N04(QDataStream &stream, AsterixData &result);

    bool generate_N01(AsterixData &map, QDataStream &stream);
    bool generate_N02(AsterixData &map, QDataStream &stream);
    bool generate_N03(AsterixData &map, QDataStream &stream);
    bool generate_N04(AsterixData &map, QDataStream &stream);

public:
    //! Channel data structure
    struct SChannel
    {
        SChannel() : channels(0) {}

        // Channel number
        enum
        {
            A1 = 0x0001,
            A2 = 0x0002,
            A3 = 0x0004,
            A4 = 0x0008,
            A5 = 0x0010,
            A6 = 0x0020,
            A7 = 0x0040,
            A8 = 0x0080,
            A9 = 0x0100,
            AA = 0x0200,
            AB = 0x0400,
            AC = 0x0800
        };

    public:
        quint16 channels;
    };

public:
    Asterix253();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;


    // Data Item I253/N01, Data Source Identifier
    static const QString DataSourceIdentifier_SAC; // System Area Code
    static const QString DataSourceIdentifier_SIC; // System Identification Code

    // DATA ITEM I253/N02 DIRECTION TO TARGET
    static const QString DirectionToTarget;         // double - degree
    static const QString ADFChannel;                // quint8 - 1-12

    // DATA ITEM I253/N03, ADF CHANNELS - SWITCH ON
    static const QString ADFChannelsOn;             // SChannel

    // DATA ITEM I253/N04, ADF CHANNELS - SWITCH OFF
    static const QString ADFChannelsOff;            // SChannel
};

}   // Aerotechnica

Q_DECLARE_METATYPE(Aerotechnica::Asterix253::SChannel)

#endif // AEROTECHNICA_ASTERIX253_H
