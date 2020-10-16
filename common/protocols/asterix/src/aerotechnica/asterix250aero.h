#ifndef AEROTECHNICA_ASTERIX250_H
#define AEROTECHNICA_ASTERIX250_H

#include "asterix.h"

#include <QtCore/QMap>

//! Aerotechnica standard of Asterix protocol
namespace Aerotechnica {

//! “Raw Video” Messages. RESERVED DATA CATEGORY.
/*!
    Non-standard data category 250 is reserved in A1000 Radar Extractor for transmitting
    of special “Raw Video” Messages. These messages are used by AEROTECHNICA RDP Systems.
*/
class ASTERIX_EXPORT Asterix250 : public Asterix
{
public:
    Asterix250();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;


    // Raw video data
    static const QString RawVideo;          // QByteArray
};

}   // Aerotechnica

#endif // AEROTECHNICA_ASTERIX250_H
