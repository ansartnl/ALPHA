#ifndef ASTERIX253PELENG_H
#define ASTERIX253PELENG_H

#include "asterix.h"

namespace Peleng {

class ASTERIX_EXPORT Asterix253 : public Asterix
{
protected:
    void parse_000(QDataStream &stream, AsterixData &map);
    void parse_010(QDataStream &stream, AsterixData &map);
    void parse_015(QDataStream &stream, AsterixData &map);
    void parse_020(QDataStream &stream, AsterixData &map);
    void parse_021(QDataStream &stream, AsterixData &map);
    void parse_030(QDataStream &stream, AsterixData &map);
    void parse_031(QDataStream &stream, AsterixData &map);

    bool generate_000(AsterixData &map, QDataStream &stream);
    bool generate_010(AsterixData &map, QDataStream &stream);
    bool generate_015(AsterixData &map, QDataStream &stream);
    bool generate_020(AsterixData &map, QDataStream &stream);
    bool generate_021(AsterixData &map, QDataStream &stream);
    bool generate_030(AsterixData &map, QDataStream &stream);
    bool generate_031(AsterixData &map, QDataStream &stream);

public:
    Asterix253();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    static const QString MessageType;
    static const QString DataSourceIdentifier_SAC;
    static const QString DataSourceIdentifier_SIC;
    static const QString ServiceIdentifier;
    static const QString State;
    static const QString Mode;
    static const QString ChannelState_SAC;
    static const QString ChannelState_SIC;
    static const QString ChannelState_First;
    static const QString ChannelState_Second;
    static const QString AdapterState_First;
    static const QString AdapterState_Second;
};

}

#endif // ASTERIX253PELENG_H
