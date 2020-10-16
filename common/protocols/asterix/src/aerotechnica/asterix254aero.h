#ifndef AEROTECHNICA_ASTERIX254_H
#define AEROTECHNICA_ASTERIX254_H

#include "asterix.h"

#include <QtCore/QMap>

//! Aerotechnica standard of Asterix protocol
namespace Aerotechnica {

//! CHANNEL TEST MESSAGES
/*!
    A1000 Radar Extractor periodically transmits “Channel Test Message” – a signal
    of Extractor operability, even if there is no output data. Two modes of Channel
    Test Messages transmitting are provided:
        - transmitting on request of the interfacing RDP
        - transmitting in pre-defined time intervals, without requests
    Combination of these two modes is also possible.

    Message type is always = 2. Message length = 6 or 7 bytes.
*/
class ASTERIX_EXPORT Asterix254 : public Asterix
{
public:
    //! Message Type – Channel Test
    enum EMessageType
    {
        Request = 1,  //!< 6 bytes length message
        Regular = 2   //!< 7 bytes length message
    };

public:
    Asterix254();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;


    // Data Source Identifier
    static const QString DataSourceIdentifier_SAC; // System Area Code
    static const QString DataSourceIdentifier_SIC; // System Identification Code

    // Message type
    static const QString MessageType;               // EMessageType - int
};

}   // Aerotechnica

#endif // AEROTECHNICA_ASTERIX254_H
